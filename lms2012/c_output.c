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


#include  "lms2012.h"
#include  "c_output.h"

#include  <stdio.h>
#include  <fcntl.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <string.h>
#include  <signal.h>
#include  <sys/mman.h>

OUTPUT_GLOBALS OutputInstance;

static void      OutputReset(void)
{
  DATA8   StopArr[3];

  StopArr[0] = (DATA8)opOUTPUT_STOP;
  StopArr[1] = 0x0F;
  StopArr[2] = 0x00;
  if (OutputInstance.PwmFile >= 0)
  {
    write(OutputInstance.PwmFile,StopArr,3);
  }
}

static RESULT    cOutputOpen(void)
{
  RESULT  Result = FAIL;

  UBYTE   PrgStart =  opPROGRAM_START;

  OutputReset();

  if (OutputInstance.PwmFile >= 0)
  {
    write(OutputInstance.PwmFile,&PrgStart,1);
  }

  Result  =  OK;

  return (Result);
}

RESULT    cOutputInit(void)
{
  RESULT      Result = FAIL;
  MOTORDATA   *pTmp;

  // To ensure that pMotor is never uninitialised
  OutputInstance.pMotor  =  OutputInstance.MotorData;

  // Open the handle for writing commands
  OutputInstance.PwmFile  =  open(PWM_DEVICE_NAME,O_RDWR);

  if (OutputInstance.PwmFile >= 0)
  {

    // Open the handle for reading motor values - shared memory
    OutputInstance.MotorFile  =  open(MOTOR_DEVICE_NAME,O_RDWR | O_SYNC);
    if (OutputInstance.MotorFile >= 0)
    {
      pTmp  =  (MOTORDATA*)mmap(0, sizeof(OutputInstance.MotorData), PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, OutputInstance.MotorFile, 0);

      if (pTmp == MAP_FAILED)
      {
        LogErrorNumber(OUTPUT_SHARED_MEMORY);
        close(OutputInstance.MotorFile);
        close(OutputInstance.PwmFile);
      }
      else
      {
        OutputInstance.pMotor  =  (MOTORDATA*)pTmp;
        Result  =  cOutputOpen();
      }
    }
  }

  return (Result);
}

#if 0
RESULT    cOutputClose(void)
{
  return (OK);
}
#endif

RESULT    cOutputExit(void)
{
  RESULT  Result = FAIL;

  OutputReset();

  if (OutputInstance.MotorFile >= 0)
  {
    munmap(OutputInstance.pMotor,sizeof(OutputInstance.MotorData));
    close(OutputInstance.MotorFile);
  }

  if (OutputInstance.PwmFile >= 0)
  {
    close(OutputInstance.PwmFile);
  }

  Result  =  OK;

  return (Result);
}


void    cOutputSetTypes(char *pTypes)
{
  DATA8   TypeArr[5];

  TypeArr[0] = opOUTPUT_SET_TYPE;
  memcpy(&TypeArr[1], pTypes, 4);

  if (OutputInstance.PwmFile >= 0)
  {
    write(OutputInstance.PwmFile,TypeArr,sizeof(TypeArr));
  }

}


UBYTE     cMotorGetBusyFlags(void)
{
  int     test, test2;
  char    BusyReturn[10]; // Busy mask

  if (OutputInstance.PwmFile >= 0)
  {
    read(OutputInstance.PwmFile,BusyReturn,4);
    sscanf(BusyReturn,"%u %u",&test,&test2);
  }
  else
  {
    test = 0;
  }

  return(test);
}


void      cMotorSetBusyFlags(UBYTE Flags)
{
  if (OutputInstance.MotorFile >= 0)
  {
    write(OutputInstance.MotorFile, &Flags, sizeof(Flags));
  }
}


//******* BYTE CODE SNIPPETS **************************************************


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_PRG_STOP ()  </b>
 *
 *- Program stop\n
 *- Dispatch status unchanged
 *
 */
/*! \brief  opOUTPUT_PRG_STOP byte code
 *
 */
void      cOutputPrgStop(void)
{
  DATA8   PrgStop;

  PrgStop = (DATA8)opPROGRAM_STOP;
  if (OutputInstance.PwmFile >= 0)
  {
    write(OutputInstance.PwmFile,&PrgStop,1);
  }
}


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_SET_TYPE (NO, TYPE)  </b>
 *
 *- Set output type\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NO      - Output no [0..3]
 *  \param  (DATA8)   TYPE    - Output device type
 */
/*! \brief  opOUTPUT_SET_TYPE byte code
 *
 */
void      cOutputSetType(DATA8 No, DATA8 Type)
{
    if ((No >= 0) && (No < OUTPUTS))
    {
      if (OutputInstance.OutputType[No] != Type)
      {
        OutputInstance.OutputType[No]  =  Type;

        if ((Type == TYPE_NONE) || (Type == TYPE_ERROR))
        {
          #ifdef DEBUG
            printf("                Output %c Disable\r\n",'A' + No);
          #endif
        }
        else
        {
          #ifdef DEBUG
            printf("                Output %c Enable\r\n",'A' + No);
          #endif
        }
      }
    }
}


/*! \page cOutput
 *  <hr size="1"/>
 *  <b>     opOUTPUT_RESET (NOS)  </b>
 *
 *- Resets the Tacho counts \n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 */
/*! \brief  opOUTPUT_RESET byte code
 *
 */
void      cOutputReset(DATA8 Nos)
{
  DATA8   ResetArr[2];

    ResetArr[0] = opOUTPUT_RESET;
    ResetArr[1] = Nos;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,ResetArr,sizeof(ResetArr));
    }
}


/*! \page cOutput
 *  <hr size="1"/>
 *  <b>     opOUTPUT_STOP (NOS)  </b>
 *
 *- Stops the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   BRAKE   - Brake [0,1]
 */
/*! \brief  opOUTPUT_STOP byte code
 *
 */
void      cOutputStop(DATA8 Nos, DATA8 Brake)
{
  DATA8   StopArr[3];

    StopArr[0] = (DATA8)opOUTPUT_STOP;
    StopArr[1] = Nos;
    StopArr[2] = Brake;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,StopArr,sizeof(StopArr));
    }
}


/*! \page cOutput
 *  <hr size="1"/>
 *  <b>     opOUTPUT_SPEED (NOS, SPEED)  </b>
 *
 *- Set speed of the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   SPEED   - Speed [-100..100%]
 */
/*! \brief  opOUTPUT_SPEED byte code
 *
 */
void      cOutputSpeed(DATA8 Nos, DATA8 Speed)
{
  DATA8   SetSpeed[3];

    SetSpeed[0] = (DATA8)opOUTPUT_SPEED;
    SetSpeed[1] = Nos;
    SetSpeed[2] = Speed;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,SetSpeed,sizeof(SetSpeed));
    }
}


/*! \page cOutput
 *  <hr size="1"/>
 *  <b>     opOUTPUT_POWER (NOS, SPEED)  </b>
 *
 *- Set power of the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   POWER   - Power [-100..100%]
 */
/*! \brief  opOUTPUT_POWER byte code
 *
 */
void      cOutputPower(DATA8 Nos, DATA8 Power)
{
  DATA8   SetPower[3];

    SetPower[0] = (DATA8)opOUTPUT_POWER;
    SetPower[1] = Nos;
    SetPower[2] = Power;
    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,SetPower,sizeof(SetPower));
    }
}


/*! \page cOutput
 *  <hr size="1"/>
 *  <b>     opOUTPUT_START (NOS)  </b>
 *
 *- Starts the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 */
/*! \brief  opOUTPUT_START byte code
 *
 */
void      cOutputStart(DATA8 Nos)
{
  DATA8   StartMotor[2];

    StartMotor[0] = (DATA8)opOUTPUT_START;
    StartMotor[1] = Nos;
    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,StartMotor,sizeof(StartMotor));
    }
}


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_POLARITY (NOS, POL)  </b>
 *
 *- Set polarity of the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   POL     - Polarity [-1,0,1]
 *
 *-  Polarity:
 *   - -1 makes the motor run backward
 *   -  1 makes the motor run forward
 *   -  0 makes the motor run the opposite direction
 */
/*! \brief  opOUTPUT_POLARITY byte code
 *
 */
void      cOutputPolarity(DATA8 Nos, DATA8 Pol)
{
  DATA8   Polarity[3];

  Polarity[0] =   (DATA8)opOUTPUT_POLARITY;
  Polarity[1] =  Nos;
  Polarity[2] =  Pol;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,Polarity,sizeof(Polarity));
    }
}


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_STEP_POWER (NOS, POWER, STEP1, STEP2, STEP3, BRAKE)  </b>
 *
 *- Set Ramp up, constant and rampdown steps and power of the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   POWER   - Power [-100..100]
 *  \param  (DATA32)  STEP1   - Tacho pulses [0..MAX]
 *  \param  (DATA32)  STEP2   - Tacho pulses [0..MAX]
 *  \param  (DATA32)  STEP3   - Tacho pulses [0..MAX]
 *  \param  (DATA8)   BRAKE   - 0 = Coast, 1 = BRAKE
 */
/*! \brief  opOUTPUT_STEP_POWER byte code
 *
 */
void      cOutputStepPower(DATA8 Nos, DATA8 Power, DATA32 Step1, DATA32 Step2, DATA32 Step3, DATA8 Brake)
{
  STEPPOWER   StepPower;

  StepPower.Cmd    =   opOUTPUT_STEP_POWER;
  StepPower.Nos    =  Nos;
  StepPower.Power  =  Power;
  StepPower.Step1  =  Step1;
  StepPower.Step2  =  Step2;
  StepPower.Step3  =  Step3;
  StepPower.Brake  =  Brake;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,(DATA8*)&(StepPower.Cmd),sizeof(StepPower));
    }
}


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_TIME_POWER (NOS, POWER, TIME1, TIME2, TIME3, BRAKE)  </b>
 *
 *- Set Ramp up, constant and rampdown steps and power of the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   POWER   - Power [-100..100]
 *  \param  (DATA32)  TIME1   - Time in Ms [0..MAX]
 *  \param  (DATA32)  TIME2   - Time in Ms [0..MAX]
 *  \param  (DATA32)  TIME3   - Time in Ms [0..MAX]
 *  \param  (DATA8)   BRAKE   - 0 = Coast, 1 = BRAKE
 */
/*! \brief  opOUTPUT_TIME_POWER byte code
 *
 */
void      cOutputTimePower(DATA8 Nos, DATA8 Power, DATA32 Time1, DATA32 Time2, DATA32 Time3, DATA8 Brake)
{
  TIMEPOWER   TimePower;

  TimePower.Cmd    =  opOUTPUT_TIME_POWER;
  TimePower.Nos    =  Nos;
  TimePower.Power  =  Power;
  TimePower.Time1  =  Time1;
  TimePower.Time2  =  Time2;
  TimePower.Time3  =  Time3;
  TimePower.Brake  =  Brake;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,(DATA8*)&(TimePower.Cmd),sizeof(TimePower));
    }
}

/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_STEP_SPEED (NOS, SPEED, STEP1, STEP2, STEP3, BRAKE)  </b>
 *
 *- Set Ramp up, constant and rampdown steps and speed of the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   SPEED   - Power [-100..100]
 *  \param  (DATA32)  STEP1   - Tacho pulses [0..MAX]
 *  \param  (DATA32)  STEP2   - Tacho pulses [0..MAX]
 *  \param  (DATA32)  STEP3   - Tacho pulses [0..MAX]
 *  \param  (DATA8)   BRAKE   - 0 = Coast, 1 = BRAKE
 */
/*! \brief  opOUTPUT_STEP_SPEED byte code
 *
 */
void      cOutputStepSpeed(DATA8 Nos, DATA8 Speed, DATA32 Step1, DATA32 Step2, DATA32 Step3, DATA8 Brake)
{
  STEPSPEED   StepSpeed;

  StepSpeed.Cmd    =   opOUTPUT_STEP_SPEED;
  StepSpeed.Nos    =  Nos;
  StepSpeed.Speed  =  Speed;
  StepSpeed.Step1  =  Step1;
  StepSpeed.Step2  =  Step2;
  StepSpeed.Step3  =  Step3;
  StepSpeed.Brake  =  Brake;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,(DATA8*)&(StepSpeed.Cmd),sizeof(StepSpeed));
    }
}


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_TIME_SPEED (NOS, SPEED, STEP1, STEP2, STEP3, BRAKE)  </b>
 *
 *- Set Ramp up, constant and rampdown steps and speed of the outputs\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \param  (DATA8)   SPEED   - Power [-100..100]
 *  \param  (DATA32)  STEP1   - Time in mS [0..MAX]
 *  \param  (DATA32)  STEP2   - Time in mS [0..MAX]
 *  \param  (DATA32)  STEP3   - Time in mS [0..MAX]
 *  \param  (DATA8)   BRAKE   - 0 = Coast, 1 = BRAKE
 */
/*! \brief  opOUTPUT_TIME_SPEED byte code
 *
 */
void      cOutputTimeSpeed(DATA8 Nos, DATA8 Speed, DATA32 Time1, DATA32 Time2, DATA32 Time3, DATA8 Brake)
{
  TIMESPEED   TimeSpeed;

  TimeSpeed.Cmd    =   (DATA8)opOUTPUT_TIME_SPEED;
  TimeSpeed.Nos    =   Nos;
  TimeSpeed.Speed  =   Speed;
  TimeSpeed.Time1  =   Time1;
  TimeSpeed.Time2  =   Time2;
  TimeSpeed.Time3  =   Time3;
  TimeSpeed.Brake  =   Brake;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile,(DATA8*)&(TimeSpeed.Cmd),sizeof(TimeSpeed));
    }
}


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_STEP_SYNC (NOS, SPEED, TURN, STEP, BRAKE)  </b>
 *
 *- Syncronizes 2 motors (and 2 motors only) in steps \n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field   [0x00..0x0F]
 *  \param  (DATA8)   SPEED   - Power              [-100..100]
 *  \param  (DATA16)  TURN    - Turn Ratio         [-200..200]
 *  \param  (DATA32)  STEP    - Tacho Pulses       [0..MAX]
 *  \param  (DATA8)   BRAKE   - 0 = Coast, 1 = BRAKE
 */
/*! \brief  opOUTPUT_STEP_SYNC byte code
 *
 */
void      cOutputStepSync(DATA8 Nos, DATA8 Speed, DATA16 Turn, DATA32 Step, DATA8 Brake)
{
  DATA8       Tmp;
  DATA8       No;
  STEPSYNC    StepSync;

  StepSync.Cmd   =   opOUTPUT_STEP_SYNC;
  StepSync.Nos   =   Nos;
  StepSync.Speed =   Speed;
  StepSync.Turn  =   Turn;
  StepSync.Step  =   Step;
  StepSync.Brake =   Brake;

  // Sync motor cmd is illegal if no of motors are different from 2
  for (Tmp = 0, No = 0; Tmp < OUTPUTS; Tmp++)
  {
    if (StepSync.Nos & (0x01 << Tmp))
    {
      No++;
    }
  }

  if (2 == No)
  {
      if (OutputInstance.PwmFile >= 0)
      {
        write(OutputInstance.PwmFile,(DATA8*)&(StepSync.Cmd),sizeof(StepSync));
      }
  }
}


/*! \page cOutput Output
 *  <hr size="1"/>
 *  <b>     opOUTPUT_TIME_SYNC (NOS, SPEED, TURN, STEP, BRAKE)  </b>
 *
 *- Syncronizes 2 motors (and 2 motors only) for time\n
 *- Dispatch status unchanged
 *
 *  \param  (DATA8)   NOS     - Output bit field   [0x00..0x0F]
 *  \param  (DATA8)   SPEED   - Power              [-100..100]
 *  \param  (DATA16)  TURN    - Turn Ratio         [-200..200]
 *  \param  (DATA32)  TIME    - Time in ms         [0..MAX]
 *  \param  (DATA8)   BRAKE   - 0 = Coast, 1 = BRAKE
 */
/*! \brief  opOUTPUT_STEP_SYNC byte code
 *
 */
void      cOutputTimeSync(DATA8 Nos, DATA8 Speed, DATA16 Turn, DATA32 Time, DATA8 Brake)
{
  TIMESYNC  TimeSync;
  DATA8     Tmp;
  DATA8     No;

  TimeSync.Cmd   =  opOUTPUT_TIME_SYNC;
  TimeSync.Nos   =  Nos;
  TimeSync.Speed =  Speed;
  TimeSync.Turn  =  Turn;
  TimeSync.Time  =  Time;
  TimeSync.Brake =  Brake;

  // Sync motor cmd is illegal if no of motors are different from 2
  for (Tmp = 0, No = 0; Tmp < OUTPUTS; Tmp++)
  {
    if (TimeSync.Nos & (0x01 << Tmp))
    {
      No++;
    }
  }

  if (2 == No)
  {
      if (OutputInstance.PwmFile >= 0)
      {
        write(OutputInstance.PwmFile,(DATA8*)&(TimeSync.Cmd),sizeof(TimeSync));
      }
  }
}

/*! \page   cOutput
 *  <hr size="1"/>
 *  <b>     opOUTPUT_READY (NOS) </b>
 *
 *- Wait for output ready (wait for completion)\n
 *- Dispatch status can change to BUSYBREAK
 *- cOUTPUT_START command has no effect on this command
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 */
/*! \brief  opOUTPUT_READY byte code
 *
 */
DSPSTAT      cOutputReady(DATA8 Nos)
{
  DATA8   Tmp;
  int     test;
  int     test2;
  char    BusyReturn[10]; // Busy mask

    if (OutputInstance.PwmFile >= 0)
    {
      read(OutputInstance.PwmFile,BusyReturn,4);

      sscanf(BusyReturn,"%u %u",&test,&test2);

      for (Tmp = 0;Tmp < OUTPUTS;Tmp++)
      {
        // Check resources for NOTREADY
        if (Nos & (1 << Tmp))
        {
          // Only relevant ones
          if (test & (1 << Tmp))
          {
			return BUSYBREAK;
          }
        }
      }
    }
  return NOBREAK;
}


/*! \page   cOutput
 *  \anchor opOUTPUT_TEST \n
 *  <hr size="1"/>
 *  <b>     opOUTPUT_TEST (NOS, BUSY) </b>
 *
 *- Testing if output is not used \n
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 *  \return (DATA8)   BUSY    - Output busy flag (0 = ready, 1 = Busy)
 */
/*! \brief  opOUTPUT_TEST byte code
 *
 */
DATA8      cOutputTest(DATA8 Nos)
{

  int     test;
  int     test2;
  char    BusyReturn[20]; // Busy mask
  DATA8   Busy = 0;

    if (OutputInstance.PwmFile >= 0)
    {
      read(OutputInstance.PwmFile,BusyReturn,10);
      sscanf(BusyReturn,"%u %u",&test,&test2);

      if (Nos & (DATA8)test2)
      {
        Busy = 1;
      }
    }
  return Busy;
}


/*! \page   cOutput
 *  \anchor opOUTPUT_TEST \n
 *  <hr size="1"/>
 *  <b>     opOUTPUT_CLR_COUNT (NOS) </b>
 *
 *- Clearing tacho count when used as sensor \n
 *
 *  \param  (DATA8)   NOS     - Output bit field [0x00..0x0F]
 */
/*! \brief  opOUTPUT_CLR_COUNT byte code
 *
 */
void      cOutputClrCount(DATA8 Nos)
{
  DATA8   ClrCnt[2];
  UBYTE   Tmp;

  ClrCnt[0]   =  opOUTPUT_CLR_COUNT;
  ClrCnt[1]   =  Nos;

    if (OutputInstance.PwmFile >= 0)
    {
      write(OutputInstance.PwmFile, ClrCnt, sizeof(ClrCnt));
    }

    // Also the user layer entry to get immediate clear
    for(Tmp = 0; Tmp < OUTPUTS; Tmp++)
    {
      if (ClrCnt[1] & (1 << Tmp))
      {
        OutputInstance.pMotor[Tmp].TachoSensor = 0;
      }
    }
}


/*! \page   cOutput
 *  \anchor opOUTPUT_TEST \n
 *  <hr size="1"/>
 *  <b>     opOUTPUT_GET_COUNT (NOS, *TACHO) </b>
 *
 *- Getting tacho count when used as sensor - values are in shared memory \n
 *
 *
 *  \param  (DATA8)   NOS     - Output number [0x00..0x0F]
 *  \return (DATA32)  *TACHO  - Tacho pulses [-MAX .. +MAX]
 */
/*! \brief  opOUTPUT_GET_COUNT byte code
 *
 */
DATA32      cOutputGetCount(DATA8 No)
{
    if (No < OUTPUTS)
    {
      return(OutputInstance.pMotor[No].TachoSensor);
    }
  return(0);
}


//*****************************************************************************
