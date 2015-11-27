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

#ifndef C_SOUND_H_
#define C_SOUND_H_

#include  "lms2012.h"
#include  <stdio.h>
#include  <string.h>
#include  <sys/stat.h>

// Percentage to SoundLevel -
// Adjust the percentage, if non-linear SPL response is needed

#define SND_LEVEL_1   13  // 13% (12.5)
#define SND_LEVEL_2   25  // 25%
#define SND_LEVEL_3   38  // 38% (37.5)
#define SND_LEVEL_4   50  // 50%
#define SND_LEVEL_5   63  // 63% (62.5)
#define SND_LEVEL_6   75  // 75%
#define SND_LEVEL_7   88  // 88% (87.5)

#define TONE_LEVEL_1    8  //  8%
#define TONE_LEVEL_2   16  // 16%
#define TONE_LEVEL_3   24  // 24%
#define TONE_LEVEL_4   32  // 32%
#define TONE_LEVEL_5   40  // 40%
#define TONE_LEVEL_6   48  // 48%
#define TONE_LEVEL_7   56  // 56%
#define TONE_LEVEL_8   64  // 64%
#define TONE_LEVEL_9   72  // 72%
#define TONE_LEVEL_10  80  // 80%
#define TONE_LEVEL_11  88  // 88%
#define TONE_LEVEL_12  96  // 96%


RESULT    cSoundInit(void);

//RESULT    cSoundOpen(void);

//Muss alle 2ms aufgerufen werden
RESULT    cSoundUpdate(void);

RESULT    cSoundClose(void);

//RESULT    cSoundExit(void);

//void      cSoundEntry(void);
void      cSoundEntry(int Cmd, UWORD Volume,UWORD Frequency,UWORD Duration, DATA8 *pFileName);

//void      cSoundReady(void);

//void      cSoundTest(void);
DATA8		cSoundTest(void);

#define FILEFORMAT_RAW_SOUND      0x0100
#define FILEFORMAT_ADPCM_SOUND    0x0101
#define SOUND_MODE_ONCE           0x00
#define SOUND_LOOP                0x01
#define SOUND_ADPCM_INIT_VALPREV  0x7F
#define SOUND_ADPCM_INIT_INDEX    20


#endif /* C_SOUND_H_ */
