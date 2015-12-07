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


#ifndef C_OUTPUT_H_
#define C_OUTPUT_H_

/*! \page OutputLibrary Output Library
 *
\verbatim
Byte codes:
opOUTPUT_GET_TYPE     NO       *TYPE                                   // Get output device type
opOUTPUT_SET_TYPE     NO       TYPE                                    // Set output device type
opOUTPUT_RESET        NOS                                              // Resets tacho count
opOUTPUT_STOP         NOS      BRAKE                                   // Stop outputs    (brake or coast)
opOUTPUT_POWER        NOS      POWER                                   // Set power       (suspend regulation and positioning)
opOUTPUT_SPEED        NOS      SPEED                                   // Set speed (relative to polarity - enables regulation if tacho)
opOUTPUT_START        NOS                                              // Starts outputs from present values
opOUTPUT_POLARITY     NOS      POL                                     // Set polarity    (0=toggle)
opOUTPUT_READ         NOS      *SPEED *STEPS                           // Read actual speed and steps from last reset
opOUTPUT_TEST         NOS      *Busy                                   //
opOUTPUT_READY        NOS                                              // Wait for new action completed or overruled
opOUTPUT_POSITION     NOS      POS                                     // Set position    (absolute from last reset)

opOUTPUT_STEP_POWER   NOS      POWER   STEP1   STEP2   STEP3   BRAKE   // Set all parameters, start if not started and power != 0
opOUTPUT_TIME_POWER   NOS      POWER   TIME1   TIME2   TIME3   BRAKE   // Set all parameters, start if not started and power != 0
opOUTPUT_STEP_SPEED   NOS      SPEED   STEP1   STEP2   STEP3   BRAKE   // Set all parameters, start if not started and power != 0
opOUTPUT_TIME_SPEED   NOS      SPEED   TIME1   TIME2   TIME3   BRAKE   // Set all parameters, start if not started and power != 0
opOUTPUT_STEP_SYNC    NOS      SPEED   TURN    STEP    BRAKE           // Set all parameters, start if not started and power != 0
opOUTPUT_TIME_SYNC    NOS      SPEED   TURN    TIME    BRAKE           // Set all parameters, start if not started and power != 0
opOUTPUT_CLR_COUNT    NOS                                              // Clears the tacho count used when in sensor mode
opOUTPUT_GET_COUNT    NO       *STEPS                                  // Gets the tacho count used in sensor mode


Parameters:
                      NO      DATA8    [0..3]              // Output port number
                      TYPE    DATA8    [0..255]            // Output device type                   (0=none)
                      NOS     DATA8    [0x00..0x0F]        // Bit field representing output 1 to 4 (0x01, 0x02, 0x04, 0x08)
                      BRAKE   DATA8    [0..1]              // Output state after stop              (0=Coast,  1=Brake)
                      POWER   DATA8    [+-0..100%]         // Power relative to polarity
                      SPEED   DATA8    [+-0..100%]         // Speed relative to polarity
                      STEPS   DATA32   [+-0..MAX]          // Steps in degrees                     (0=infinite)
                      POL     DATA8    [+-0..1]            // Polarity +-1, 0=toggle               (multiplied to SPEED and POWER)
                      POS     DATA32   [+-0..MAX]          // Steps in degrees                     (0=infinite)
                      STEP1   DATA32   [0..MAX]            // Steps used to ramp up
                      STEP2   DATA32   [0..MAX]            // Steps used for constant speed
                      STEP3   DATA32   [0..MAX]            // Steps used to ramp down
                      TIME1   DATA32   [0..MAX]            // Time [mS] to ramp up
                      TIME2   DATA32   [0..MAX]            // Time [mS] for constant speed
                      TIME3   DATA32   [0..MAX]            // Time [mS] to ramp down
                      TURN    DATA16   [-200..200]         // Turn ratio between two syncronized motors
*/

/*
 * SYNCRONIZATION:
 *
 *  Speed [-100 .. +100] is move forward or move backwards
 *  Turn ratio [-200 .. +200] is how tight you turn and to
 *  what direction you turn
 *   - 0 value is moving straight forward
 *   - Negative values turns to the left
 *   - Positive values turns to the right
 *   - Value -100 stops the left motor
 *   - Value +100 stops the right motor
 *   - Values less than -100 makes the left motor run the opposite
 *     direction of the right motor (Spin)
 *   - Values greater than +100 makes the right motor run the opposite
 *     direction of the left motor (Spin)
 *
 *  Example: opOUTPUT_TIME_SYNC(0, 10, 100, 50, 10000,1)
 *
 *  10    = Motor bit field - Motor B and D
 *  100   = Motor speed     - Motor B will run at speed 100 (because ratio is positive)
 *  50    = Turn ratio      - Motor D will run at speed 50
 *  10000 = time in mS      - Motors will run for 10 sec.
 *  1     = Brake bit       - When 10 sec. has elapsed then brake both motors
 *
 *
 *  Example: opOUTPUT_TIME_SYNC(0, 10, 100, 150, 10000,1)
 *
 *  10    = Motor bit field - Motor B and D
 *  100   = Motor speed     - Motor B will run at speed 100 (because ratio is positive)
 *  150   = Turn ratio      - Motor D will run at speed -50
 *  10000 = time in mS      - Motors will run for 10 sec.
 *  1     = Brake bit       - When 10 sec. has elapsed then brake both motors
 *
 *
 *  Example: opOUTPUT_TIME_SYNC(0, 10, 100, -50, 10000,1)
 *
 *  10    = Motor bit field - Motor B and D
 *  100   = Motor speed     - Motor B will run at speed  50 (because ratio is positive)
 *  -50   = Turn ratio      - Motor D will run at speed 100
 *  10000 = time in mS      - Motors will run for 10 sec.
 *  1     = Brake bit       - When 10 sec. has elapsed then brake both motors
 *
 *
 *  Example: opOUTPUT_TIME_SYNC(0, 10, 200, -150, 10000,1)
 *
 *  10    = Motor bit field - Motor B and D
 *  100   = Motor speed     - Motor B will run at speed -50 (because ratio is positive)
 *  -150  = Turn ratio      - Motor D will run at speed 100
 *  10000 = time in mS      - Motors will run for 10 sec.
 *  1     = Brake bit       - When 10 sec. has elapsed then brake both motors
 *
 *\

\endverbatim
 *
 */

RESULT    cOutputInit(void);

RESULT    cOutputExit(void);

void      cOutputSetTypes(char *pTypes);
void      cOutputSetType (DATA8 No, DATA8 Type);

UBYTE     cMotorGetBusyFlags(void);
void      cMotorSetBusyFlags(UBYTE Flags);

void      cOutputReset    (DATA8 Nos);
void      cOutputStop     (DATA8 Nos, DATA8 Brake);
void      cOutputPrgStop  (void);
void      cOutputPower    (DATA8 Nos, DATA8 Power);
void      cOutputSpeed    (DATA8 Nos, DATA8 Speed);
void      cOutputStart    (DATA8 Nos);
void      cOutputPolarity (DATA8 Nos, DATA8 Pol);

DATA8     cOutputTest  
(DATA8 Nos);
DSPSTAT   cOutputReady    (DATA8 Nos);
void      cOutputStepPower(DATA8 Nos, DATA8 Power, DATA32 Step1, DATA32 Step2, DATA32 Step3, DATA8 Brake);
void      cOutputTimePower(DATA8 Nos, DATA8 Power, DATA32 Time1, DATA32 Time2, DATA32 Time3, DATA8 Brake);
void      cOutputStepSpeed(DATA8 Nos, DATA8 Speed, DATA32 Step1, DATA32 Step2, DATA32 Step3, DATA8 Brake);
void      cOutputTimeSpeed(DATA8 Nos, DATA8 Speed, DATA32 Time1, DATA32 Time2, DATA32 Time3, DATA8 Brake);
void      cOutputStepSync (DATA8 Nos, DATA8 Speed, DATA16 Turn,  DATA32 Step,                DATA8 Brake);
void      cOutputTimeSync (DATA8 Nos, DATA8 Speed, DATA16 Turn,  DATA32 Time,                DATA8 Brake);
void      cOutputClrCount (DATA8 Nos);
DATA32    cOutputGetCount (DATA8 No);

typedef struct
{
  //*****************************************************************************
  // Output Global variables
  //*****************************************************************************

  DATA8       OutputType[OUTPUTS];

  int         PwmFile;
  int         MotorFile;

  MOTORDATA   MotorData[OUTPUTS];
  MOTORDATA   *pMotor;
}
OUTPUT_GLOBALS;

extern OUTPUT_GLOBALS OutputInstance;

#endif /* C_OUTPUT_H_ */
