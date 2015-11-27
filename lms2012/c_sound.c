/*
 * LEGO® MINDSTORMS EV3
 *
 * Copyright (C) 2010-2013 The LEGO Group
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include  "c_sound.h"
#include  "lms2012.h"


#include  <stdio.h>
#include  <fcntl.h>
#include  <stdlib.h>
#include  <unistd.h>
#include <sys/mman.h>
//#include  <sys/stat.h>
 
#define STEP_SIZE_TABLE_ENTRIES 89
#define INDEX_TABLE_ENTRIES     16

enum
{
  SOUND_STOPPED,
  SOUND_SETUP_FILE,
  SOUND_FILE_PLAYING,
  SOUND_FILE_LOOPING,
  SOUND_TONE_PLAYING,
  SOUND_TONE_LOOPING
}SOUND_STATES;
 
const SWORD StepSizeTable[STEP_SIZE_TABLE_ENTRIES] = 
{ 
        7,     8,     9,    10,    11,    12,    13,    14,    16,    17,
       19,    21,    23,    25,    28,    31,    34,    37,    41,    45,
       50,    55,    60,    66,    73,    80,    88,    97,   107,   118,
      130,   143,   157,   173,   190,   209,   230,   253,   279,   307,
      337,   371,   408,   449,   494,   544,   598,   658,   724,   796,
      876,   963,  1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066,
     2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,
     5894,  6484,  7132,  7845,  8630,  9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

const SWORD IndexTable[INDEX_TABLE_ENTRIES] = 
{
     -1, -1, -1, -1, 2, 4, 6, 8,
     -1, -1, -1, -1, 2, 4, 6, 8
};

typedef struct
{
  //*****************************************************************************
  // Sound Global variables
  //*****************************************************************************

  int     SoundDriverDescriptor;
  int     hSoundFile;

//DATA8   SoundOwner;
  DATA8   cSoundState;
  SOUND   Sound;
  SOUND	 *pSound;
  UWORD	  BytesLeft;
  UWORD	  SoundFileFormat;
  UWORD	  SoundDataLength;
  UWORD	  SoundSampleRate;
  UWORD	  SoundPlayMode;
  UWORD   SoundFileLength;
  SWORD   ValPrev;
  SWORD   Index;
  SWORD   Step;
  UBYTE   BytesToWrite;
  char    PathBuffer[MAX_FILENAME_SIZE];
  struct  stat FileStatus;
  UBYTE   SoundData[SOUND_FILE_BUFFER_SIZE + 1]; // Add up for CMD
}
SOUND_GLOBALS;


SOUND_GLOBALS SoundInstance;

#ifdef    DEBUG_C_SOUND
  #define   DEBUG
#endif

RESULT    cSoundInit(void)
{
  RESULT  Result = FAIL;
  SOUND  *pSoundTmp;
  int	  SndFile;

  SoundInstance.SoundDriverDescriptor = -1;
  SoundInstance.hSoundFile            = -1;
  SoundInstance.pSound                =  &SoundInstance.Sound;

  // Create a Shared Memory entry for signaling the driver state BUSY or NOT BUSY

  SndFile = open(SOUND_DEVICE_NAME,O_RDWR | O_SYNC);

  if(SndFile >= 0)
  {
    pSoundTmp  =  (SOUND*)mmap(0, sizeof(UWORD), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, SndFile, 0);

    if(pSoundTmp == MAP_FAILED)
    {
      LogErrorNumber(SOUND_SHARED_MEMORY);
    }
    else
    {
      SoundInstance.pSound = pSoundTmp;
      Result  =  OK;
    }

    close(SndFile);
  }

  return (Result);
}

#if 0
RESULT    cSoundOpen(void)
{
  RESULT  Result = FAIL;

  Result  =  OK;

  return (Result);
}
#endif

RESULT    cSoundClose(void)
{
  RESULT  Result = FAIL;
  UWORD   BytesToWrite;
  DATA8   SoundData[SOUND_FILE_BUFFER_SIZE + 1]; // Add up for CMD

  SoundData[0] = BREAK;
  BytesToWrite = 1;
  SoundInstance.SoundDriverDescriptor = open(SOUND_DEVICE_NAME, O_WRONLY);
  if (SoundInstance.SoundDriverDescriptor >= 0)
  {
    write(SoundInstance.SoundDriverDescriptor, SoundData, BytesToWrite);
    close(SoundInstance.SoundDriverDescriptor);
    SoundInstance.SoundDriverDescriptor = -1;
  }
  SoundInstance.cSoundState = SOUND_STOPPED;

  if (SoundInstance.hSoundFile >= 0)
  {
    close(SoundInstance.hSoundFile);
    SoundInstance.hSoundFile  = -1;
  }

  Result  =  OK;

  return (Result);
}

void cSoundInitAdPcm(void)
{
  // Reset ADPCM values to a known and initial value
  SoundInstance.ValPrev = SOUND_ADPCM_INIT_VALPREV;
  SoundInstance.Index   = SOUND_ADPCM_INIT_INDEX;
  SoundInstance.Step    = StepSizeTable[SoundInstance.Index];
}

UBYTE cSoundGetAdPcmValue(UBYTE Delta)  // Call ONLY when cSoundInitAdPcm has been called :-)
{
  SWORD VpDiff;
  UBYTE Sign;

  //
  SoundInstance.Step   = StepSizeTable[SoundInstance.Index];
  SoundInstance.Index += IndexTable[Delta]; // Find new index value (for later)

  if(SoundInstance.Index < 0)
    SoundInstance.Index = 0;
  else
  {
    if(SoundInstance.Index > (STEP_SIZE_TABLE_ENTRIES - 1))
      SoundInstance.Index = STEP_SIZE_TABLE_ENTRIES - 1;
  }

  Sign  = Delta & 8;                    // Separate sign
  Delta = Delta & 7;                    // Separate magnitude

  VpDiff = SoundInstance.Step >> 3;     // Compute difference and new predicted value

  if(Delta & 4) VpDiff += SoundInstance.Step;
  if(Delta & 2) VpDiff += SoundInstance.Step >> 1;
  if(Delta & 1) VpDiff += SoundInstance.Step >> 2;

  if(Sign)
    SoundInstance.ValPrev -= VpDiff;    // "Add" with sign
  else
    SoundInstance.ValPrev += VpDiff;

  if(SoundInstance.ValPrev > 255)       // Clamp value to 8-bit unsigned
  {
    SoundInstance.ValPrev = 255;
  }
  else
  {
    if(SoundInstance.ValPrev < 0)
    {
      SoundInstance.ValPrev = 0;
    }
  }

  SoundInstance.Step = StepSizeTable[SoundInstance.Index];  // Update step value

  return((UBYTE)SoundInstance.ValPrev);                     // Return decoded byte (nibble xlated -> 8 bit)
}

RESULT cSoundUpdate(void)
{
  int     BytesRead;
  int     i;
  UBYTE   AdPcmData[SOUND_ADPCM_CHUNK]; // Temporary ADPCM input buffer
  UWORD   BytesToRead;
  UBYTE   BytesWritten = 0;
  RESULT  Result = FAIL;

  switch(SoundInstance.cSoundState)
  {
    case  SOUND_STOPPED:        // Do nothing
                                break;

    case  SOUND_SETUP_FILE:     // Keep hands off - should only appear, when needed... but...
                                break;

    case  SOUND_FILE_LOOPING:   // Make it looping!!
                                // Fall through

    case  SOUND_FILE_PLAYING:   if(SoundInstance.BytesToWrite > 0)  // Do we have "NOT WRITTEN DATA"
                                {
                                  // Yes, write the pending stuff to Driver
                                  SoundInstance.SoundDriverDescriptor = open(SOUND_DEVICE_NAME, O_WRONLY);
                                  if (SoundInstance.SoundDriverDescriptor >= 0)
                                  {
                                    BytesWritten = write(SoundInstance.SoundDriverDescriptor, SoundInstance.SoundData, SoundInstance.BytesToWrite);
                                    close(SoundInstance.SoundDriverDescriptor);
                                    SoundInstance.SoundDriverDescriptor = -1;
                                    Result = OK;
                                    // Adjust BytesToWrite with Bytes actually written
                                    if (BytesWritten > 1)
                                    {
                                      if(SoundInstance.SoundFileFormat == FILEFORMAT_ADPCM_SOUND)
                                      {
                                        SoundInstance.SoundDataLength -= (UBYTE)(BytesWritten / 2);   // nibbles in file
                                        SoundInstance.BytesToWrite    -= (UBYTE)(BytesWritten + 1);   // Buffer data incl. CMD
                                      }
                                      else
                                      {
                                        SoundInstance.SoundDataLength -= BytesWritten;
                                        SoundInstance.BytesToWrite    -= (UBYTE)(BytesWritten + 1);   // Buffer data incl. CMD
                                      }
                                    }
                                  }
                                  else
                                  {
                                    // ERROR!!!!! in writing to driver
                                    SoundInstance.cSoundState = SOUND_STOPPED;  // Couldn't do the job :-(
                                    if (SoundInstance.hSoundFile >= 0)
                                    {
                                      close(SoundInstance.hSoundFile);
                                      SoundInstance.hSoundFile  = -1;
                                    }

                                  }
                                }
                                else  // Get new sound data
                                {     // No pending stuff
                                  if(SoundInstance.SoundDataLength > 0) // Any new data?
                                  {
                                    SoundInstance.SoundData[0] = SERVICE;

                                    if(SoundInstance.SoundFileFormat == FILEFORMAT_ADPCM_SOUND)
                                    {
                                      // Adjust the chunk size for ADPCM (nibbles) if necessary
                                      if (SoundInstance.SoundDataLength > SOUND_ADPCM_CHUNK)
                                        BytesToRead = SOUND_ADPCM_CHUNK;
                                      else
                                        BytesToRead = SoundInstance.SoundDataLength;

                                      if(SoundInstance.hSoundFile >= 0)  // Valid file
                                      {
                                        BytesRead  =  read(SoundInstance.hSoundFile,AdPcmData,BytesToRead);

                                        for(i = 0; i < BytesRead; i++)
                                        {
                                          SoundInstance.SoundData[2*i + 1] = cSoundGetAdPcmValue((AdPcmData[i] >> 4) & 0x0F);
                                          SoundInstance.SoundData[2*i + 2] = cSoundGetAdPcmValue(AdPcmData[i] & 0x0F);
                                        }

                                        SoundInstance.BytesToWrite = (UBYTE) (1 + BytesRead * 2);
                                      }
                                    }
                                    else // Non compressed data
                                    {
                                      // Adjust the chunk size if necessary
                                      if (SoundInstance.SoundDataLength > SOUND_CHUNK)
                                        BytesToRead = SOUND_CHUNK;
                                      else
                                        BytesToRead = SoundInstance.SoundDataLength;

                                      if(SoundInstance.hSoundFile >= 0)  // Valid file
                                      {
                                        BytesRead  =  read(SoundInstance.hSoundFile,&(SoundInstance.SoundData[1]),BytesToRead);

                                        SoundInstance.BytesToWrite = BytesRead + 1;
                                      }
                                    }
                                    // Now we have or should have some bytes to write down into the driver
                                    SoundInstance.SoundDriverDescriptor = open(SOUND_DEVICE_NAME, O_WRONLY);
                                    if (SoundInstance.SoundDriverDescriptor >= 0)
                                    {
                                      BytesWritten = write(SoundInstance.SoundDriverDescriptor, SoundInstance.SoundData, SoundInstance.BytesToWrite);
                                      close(SoundInstance.SoundDriverDescriptor);
                                      SoundInstance.SoundDriverDescriptor = -1;
                                      Result = OK;

                                      // Adjust BytesToWrite with Bytes actually written
                                      if (BytesWritten > 1)
                                      {
                                        if(SoundInstance.SoundFileFormat == FILEFORMAT_ADPCM_SOUND)
                                        {
                                          SoundInstance.SoundDataLength -= (UBYTE)(BytesWritten / 2); // nibbles in file
                                          SoundInstance.BytesToWrite  -= (UBYTE)(BytesWritten + 1);   // Buffer data incl. CMD
                                        }
                                        else
                                        {
                                          SoundInstance.SoundDataLength -= BytesWritten;
                                          SoundInstance.BytesToWrite  -= (UBYTE)(BytesWritten + 1);   // Buffer data incl. CMD
                                        }
                                      }
                                    }
                                    else
                                    {
                                      // ERROR!!!!! in writing to driver
                                      SoundInstance.cSoundState = SOUND_STOPPED;  // Couldn't do the job :-(
                                      if (SoundInstance.hSoundFile >= 0)
                                      {
                                        close(SoundInstance.hSoundFile);
                                        SoundInstance.hSoundFile  = -1;
                                      }

                                    }

                                  } // end new data
                                  else  // Shut down the SOUND stuff until new request/data i.e.
                                  {     // SoundDataLength > 0

                                    if(SoundInstance.cSoundState == SOUND_FILE_LOOPING)
                                    {
                                      lseek(SoundInstance.hSoundFile, 0, SEEK_SET);
                                      stat(SoundInstance.PathBuffer,&SoundInstance.FileStatus);
                                      SoundInstance.SoundDataLength   =  SoundInstance.FileStatus.st_size;
                                      // TODO make a new write here, so no zero-"sound"

                                    }
                                    else
                                    {
                                      SoundInstance.cSoundState = SOUND_STOPPED;

                                      if (SoundInstance.hSoundFile >= 0)
                                      {
                                        close(SoundInstance.hSoundFile);
                                        SoundInstance.hSoundFile  = -1;
                                      }

                                      if (SoundInstance.SoundDriverDescriptor >= 0)
                                      {
                                        close(SoundInstance.SoundDriverDescriptor);
                                        SoundInstance.SoundDriverDescriptor = -1;
                                      }

                                    }

                                  }
                                }   // No pending write
                                break;

    case  SOUND_TONE_PLAYING:   // Check for duration done in d_sound :-)

                                if ((*SoundInstance.pSound).Status == OK)
                                {
                                  // DO the finished stuff
                                  SoundInstance.cSoundState = SOUND_STOPPED;
                                  if (SoundInstance.SoundDriverDescriptor >= 0)
                                  {
                                    close(SoundInstance.SoundDriverDescriptor);
                                    SoundInstance.SoundDriverDescriptor = -1;
                                  }

                                  if (SoundInstance.hSoundFile >= 0)
                                  {
                                    close(SoundInstance.hSoundFile);
                                    SoundInstance.hSoundFile  = -1;
                                  }

                                  Result = OK;
                                }
                                break;

    default:                    // Do nothing
                                break;
  }
    return (Result);
}

#if 0
RESULT    cSoundExit(void)
{
  RESULT  Result = FAIL;

  Result  =  OK;

  return (Result);
}
#endif

//******* BYTE CODE SNIPPETS **************************************************

/*! \page cSound Sound
 *  <hr size="1"/>
 *  <b>     opSOUND ()  </b>
 *
 *- Memory file entry\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   CMD     - Specific command \ref soundsubcode
 *
 *  - CMD = BREAK\n
 *
 *\n
 *  - CMD = TONE
 *    -  \param  (DATA8)    VOLUME    - Volume [0..100]\n
 *    -  \param  (DATA16)   FREQUENCY - Frequency [Hz]\n
 *    -  \param  (DATA16)   DURATION  - Duration [mS]\n
 *
 *\n
 *  - CMD = PLAY
 *    -  \param  (DATA8)    VOLUME    - Volume [0..100]\n
 *    -  \param  (DATA8)    NAME      - First character in filename (character string ohne .rsf Endung)\n
 *
 *\n
 *  - CMD = REPEAT
 *    -  \param  (DATA8)    VOLUME    - Volume [0..100]\n
 *    -  \param  (DATA8)    NAME      - First character in filename (character string ohne .rsf Endung)\n
 *
 *\n
 *
 */
/*! \brief  opSOUND byte code
 *
 */
void      cSoundEntry(int Cmd, UWORD Volume,UWORD Frequency,UWORD Duration, DATA8 *pFileName)
{
//  int   	Cmd;
//	UWORD   Temp1 --> Volume;
    UBYTE   Loop = FALSE;
//  UWORD   Frequency;
//  UWORD   Duration;

    UWORD	BytesToWrite;
    UWORD	BytesWritten = 0;
    DATA8   SoundData[SOUND_FILE_BUFFER_SIZE + 1]; // Add up for CMD

//  DATA8   *pFileName;
    char    PathName[MAX_FILENAME_SIZE];
    UBYTE   Tmp1;
    UBYTE   Tmp2;

//  Cmd =  *(DATA8*)PrimParPointer();

    SoundData[0] = Cmd;	// General for all commands :-)
    BytesToWrite = 0;

    switch(Cmd)
    {

      case TONE:    (*SoundInstance.pSound).Status = BUSY;
//                  SoundInstance.SoundOwner = CallingObjectId();
//                  Volume = *(DATA8*)PrimParPointer();   // Volume level

                    // Scale the volume from 1-100% into 13 level steps
					// Could be linear but prepared for speaker and -box adjustments... :-)
					if(Volume > 0)
					{
					  if(Volume > TONE_LEVEL_6)           // >  48%
					  {
						if(Volume > TONE_LEVEL_9)         // >  72%
						{
						  if(Volume > TONE_LEVEL_11)      // >  88%
						  {
							if(Volume > TONE_LEVEL_12)    // >  96%
							{
							  SoundData[1] = 13;            // => 100%
							}
							else
							{
							  SoundData[1] = 12;            // => 100%
							}
						  }
						  else
						  {
							if(Volume > TONE_LEVEL_10)    // >  96%
							{
							  SoundData[1] = 11;            // => 100%
							}
							else
							{
							  SoundData[1] = 10;            // => 100%
							}
						  }
						}
						else
						{
						  if(Volume > TONE_LEVEL_8)       // >  62.5%
						  {
							SoundData[1] = 9;           // => 75%
						  }
						  else
						  {
							if(Volume > TONE_LEVEL_7)
							{
							  SoundData[1] = 8;           // => 62.5%
							}
							else
							{
							  SoundData[1] = 7;           // => 62.5%
							}
						  }
						}
					  }
					  else
					  {
						if(Volume > TONE_LEVEL_3)         // >  25%
						{
						  if(Volume > TONE_LEVEL_5)       // >  37.5%
						  {
							SoundData[1] = 6;           // => 50%
						  }
						  else
						  {
							if(Volume > TONE_LEVEL_4)       // >  37.5%
							{
							  SoundData[1] = 5;           // => 37.5%
							}
							else
							{
							  SoundData[1] = 4;
							}
						  }
						}
						else
						{
						  if(Volume > TONE_LEVEL_2)       // >  12.5%
						  {
							SoundData[1] = 3;
						  }
						  else
						  {
							if(Volume > TONE_LEVEL_1)
							{
							  SoundData[1] = 2;           // => 25%
							}
							else
							{
							  SoundData[1] = 1;           // => 25%
							}
						  }
						}											                           											                        }
					  }
					  else
						SoundData[1] = 0;

// 					  Frequency    = *(DATA16*)PrimParPointer();
//                    Duration     = *(DATA16*)PrimParPointer();
                      SoundData[2] = (UBYTE)(Frequency);
                      SoundData[3] = (UBYTE)(Frequency >> 8);
                      SoundData[4] = (UBYTE)(Duration);
                      SoundData[5] = (UBYTE)(Duration >> 8);
                      BytesToWrite = 6;
                      SoundInstance.cSoundState = SOUND_TONE_PLAYING;
                      break;

      case BREAK:     //SoundData[0] = Cmd;
                      BytesToWrite = 1;
                      SoundInstance.cSoundState = SOUND_STOPPED;

                      if (SoundInstance.hSoundFile >= 0)
                      {
                        close(SoundInstance.hSoundFile);
                        SoundInstance.hSoundFile  = -1;
                      }

                      break;

      case REPEAT: 	  Loop = TRUE;
                      SoundData[0] = PLAY;  // Yes, but looping :-)
                      // Fall through

      case PLAY:	    // If SoundFile is Flood filled, we must politely
                      // close the active handle - else we acts as a "BUG"
                      // eating all the crops (handles) ;-)

                      SoundInstance.cSoundState = SOUND_STOPPED;  // Yes but only shortly

                      if (SoundInstance.hSoundFile >= 0)  // An active handle?
                      {
                        close(SoundInstance.hSoundFile);  // No more use
                        SoundInstance.hSoundFile  = -1;   // Signal it
                      }

                      (*SoundInstance.pSound).Status = BUSY;
//                    SoundInstance.SoundOwner = CallingObjectId();

//                    Volume = *(DATA8*)PrimParPointer();  // Volume level
   	  	  	  	  	  // Scale the volume from 1-100% into 1 - 8 level steps
    	  	  	  	  // Could be linear but prepared for speaker and -box adjustments... :-)
                      if(Volume > 0)
                      {
                        if(Volume > SND_LEVEL_4)           // >  50%
                        {
                          if(Volume > SND_LEVEL_6)         // >  75%
                          {
                            if(Volume > SND_LEVEL_7)       // >  87.5%
                            {
                              SoundData[1] = 8;           // => 100%
                            }
                            else
                            {
                              SoundData[1] = 7;           // => 87.5%
                            }
                          }
                          else
                          {
                            if(Volume > SND_LEVEL_5)       // >  62.5%
                            {
                              SoundData[1] = 6;           // => 75%
                            }
                            else
                            {
                              SoundData[1] = 5;           // => 62.5%
                            }
                          }
                        }
                        else
                        {
                          if(Volume > SND_LEVEL_2)         // >  25%
                          {
                            if(Volume > SND_LEVEL_3)       // >  37.5%
                            {
                              SoundData[1] = 4;           // => 50%
                            }
                            else
                            {
                              SoundData[1] = 3;           // => 37.5%
                            }
                          }
                          else
                          {
                            if(Volume > SND_LEVEL_1)       // >  12.5%
                            {
                              SoundData[1] = 2;           // => 25%
                            }
                            else
                            {
                              SoundData[1] = 1;           // => 12.5%
                            }
                          }
                        }
                      }
                      else
                        SoundData[1] = 0;

    	  	  	  	  	BytesToWrite = 2;

    	  	  	  	  	// Get filename
//    	  	  	  	  	pFileName    = (DATA8*)PrimParPointer();

    	  	  	  	  	if(pFileName != NULL) // We should have a valid filename
    	  	  	  	    {
    	  	  	  	  	  // Get Path and concatenate

    	  	  	  	  	  PathName[0]  =  0;
//    	  	  	  	  	  if (pFileName[0] != '.')
//    	  	  	  	  	  {
//    	  	  	  	  	    GetResourcePath(PathName, MAX_FILENAME_SIZE);
//                          sprintf(SoundInstance.PathBuffer, "%s%s.rsf", (char*)PathName, (char*)pFileName);
//    	  	  	  	  	  }
//    	  	  	  	  	  else
    	  	  	  	  	  {
    	  	  	  	  	    sprintf(SoundInstance.PathBuffer, "%s.rsf", (char*)pFileName);
    	  	  	  	  	  }

    	  	  	  	  	  // Open SoundFile

    	  	  	  	  	  SoundInstance.hSoundFile  =  open(SoundInstance.PathBuffer,O_RDONLY,0666);

                        if(SoundInstance.hSoundFile >= 0)
                        {
                          // Get actual FileSize
                          stat(SoundInstance.PathBuffer,&SoundInstance.FileStatus);
                          SoundInstance.SoundFileLength   =  SoundInstance.FileStatus.st_size;

                          // BIG Endianess

                          read(SoundInstance.hSoundFile,&Tmp1,1);
                          read(SoundInstance.hSoundFile,&Tmp2,1);
                          SoundInstance.SoundFileFormat   =  (UWORD)Tmp1 << 8 | (UWORD)Tmp2;

                          read(SoundInstance.hSoundFile,&Tmp1,1);
                          read(SoundInstance.hSoundFile,&Tmp2,1);
                          SoundInstance.SoundDataLength   =  (UWORD)Tmp1 << 8 | (UWORD)Tmp2;

                          read(SoundInstance.hSoundFile,&Tmp1,1);
                          read(SoundInstance.hSoundFile,&Tmp2,1);
                          SoundInstance.SoundSampleRate   =  (UWORD)Tmp1 << 8 | (UWORD)Tmp2;

                          read(SoundInstance.hSoundFile,&Tmp1,1);
                          read(SoundInstance.hSoundFile,&Tmp2,1);
                          SoundInstance.SoundPlayMode     =  (UWORD)Tmp1 << 8 | (UWORD)Tmp2;

                          SoundInstance.cSoundState       =  SOUND_SETUP_FILE;

                          if(SoundInstance.SoundFileFormat == FILEFORMAT_ADPCM_SOUND)
                            cSoundInitAdPcm();
                        }


    	  	  	  	    }
    	  	  	  	    else
    	  	  	  	    {
    	  	  	  	      //Do some ERROR-handling :-)
    	  	  	  	      //NOT a valid name from above :-(
    	  	  	  	    }

    	  	  	  break;

      default:  BytesToWrite = 0; // An non-valid entry
                break;
    }

    if(BytesToWrite > 0)
    {
      SoundInstance.SoundDriverDescriptor = open(SOUND_DEVICE_NAME, O_WRONLY);
      if (SoundInstance.SoundDriverDescriptor >= 0)
      {
        BytesWritten = write(SoundInstance.SoundDriverDescriptor, SoundData, BytesToWrite);
        close(SoundInstance.SoundDriverDescriptor);
        SoundInstance.SoundDriverDescriptor = -1;

        if (SoundInstance.cSoundState == SOUND_SETUP_FILE)  // The one and only situation
        {
          SoundInstance.BytesToWrite = 0;                   // Reset
          if(TRUE == Loop)
            SoundInstance.cSoundState = SOUND_FILE_LOOPING;
          else
            SoundInstance.cSoundState = SOUND_FILE_PLAYING;
        }
      }
      else
        SoundInstance.cSoundState = SOUND_STOPPED;          // Couldn't do the job :-(
    }
    else
    {
      BytesToWrite  =  BytesWritten;
      BytesToWrite  =  0;
    }
}

/*! \page   cSound
 *  <hr size="1"/>
 *  <b>     opSOUND_TEST (BUSY) </b>
 *
 *- Test if sound busy (playing file or tone\n
 *- Dispatch status unchanged
 *
 *  \return  (DATA8)   BUSY    - Sound busy flag (0 = ready, 1 = busy)
 *
 */
/*! \brief  opSOUND_TEST byte code
 *
 */

//void      cSoundTest(void)
DATA8       cSoundTest(void)
{
  if ((*SoundInstance.pSound).Status == BUSY)
  {
//  *(DATA8*)PrimParPointer()  =  1;
	return 1;
  }
  else
  {
//  *(DATA8*)PrimParPointer()  =  0;
	return 0;
  }
}

/*! \page   cSound
 *  <hr size="1"/>
 *  <b>     opSOUND_READY () </b>
 *
 *- Wait for sound ready (wait until sound finished)\n
 *- Dispatch status can change to BUSYBREAK
 *
 */
/*! \brief  opSOUND_READY byte code
 *
 */
#if 0
void      cSoundReady(void)
{
  IP      TmpIp;
  DSPSTAT DspStat = NOBREAK;

  TmpIp   =  GetObjectIp();

  if ((*SoundInstance.pSound).Status == BUSY)
  { // If BUSY check for OVERRULED

    { // Rewind IP and set status
      DspStat  =  BUSYBREAK; // break the interpreter and waits busy
      SetDispatchStatus(DspStat);
      SetObjectIp(TmpIp - 1);
    }
  }
}
#endif