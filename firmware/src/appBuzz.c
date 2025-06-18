/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    appbuzz.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "appbuzz.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APPBUZZ_DATA appbuzzData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APPBUZZ_Initialize ( void )

  Remarks:
    See prototype in appbuzz.h.
 */

void APPBUZZ_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appbuzzData.state = APPBUZZ_STATE_INIT;

    
    /* TODsO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APPBUZZ_Tasks ( void )

  Remarks:
    See prototype in appbuzz.h.
 */

void APPBUZZ_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appbuzzData.state )
    {
        /* Application's initial state. */
        case APPBUZZ_STATE_INIT:
        {
           float songMelody[10] = {
    //like composing a song "randomly"
    NOTE_DO, NOTE_RE, NOTE_DO, NOTE_RE, NOTE_MI, NOTE_FA,
    NOTE_SOL, NOTE_FA, NOTE_MI, NOTE_RE
};
            
                appbuzzData.state = APPBUZZ_STATE_IDLE;
            
            break;
        }

        case APPBUZZ_STATE_SERVICE_TASKS:
        {
            APP_PlaySong();
        
            break;
        }

        /* TODO: implement your application state machine.*/
        case APPBUZZ_STATE_IDLE:
        {
            // For now, we just stay here
            break;
        }

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
void APP_TIMER1_CALLBACK(void)
    {
        //between 1khz and 4khz 
        //timer should be set to 4khz 
        //with loop if necessary 
        BUZZ_CMDToggle();

    }
 void APP_SetTMR0_Frequency(float freq_hz)
    {
        //we should verify if prescaler value is disponible for the TMR used
        uint16_t prescaler_values[] = {1, 2, 4, 8, 16, 32, 64, 256};
        TMR_PRESCALE prescaler_enums[] = {
            TMR_PRESCALE_VALUE_1, TMR_PRESCALE_VALUE_2, TMR_PRESCALE_VALUE_4, TMR_PRESCALE_VALUE_8,
            TMR_PRESCALE_VALUE_16, TMR_PRESCALE_VALUE_32, TMR_PRESCALE_VALUE_64, TMR_PRESCALE_VALUE_256
        };
        uint8_t i = 0;

        uint32_t period = 0;
        for (i = 0; i < 8; i++)
        {
            period = (uint32_t) (PBCLK_FREQ / (prescaler_values[i] * freq_hz)) - 1;
            if (period <= 0xFFFF)
            {

                // Set prescaler
                PLIB_TMR_PrescaleSelect(TMR_ID_1, prescaler_enums[i]);
                // Set period
                DRV_TMR0_PeriodValueSet(period);
                break;
            }
        }
        // Optionally restart timer
        DRV_TMR0_Stop();
        DRV_TMR0_Start();
    }


    // Function to play a song (blocking, for demo)

    /**
     * @brief Joue une mélodie sur le buzzer (fonction bloquante).
     *
     * Cette fonction parcourt la séquence de notes et joue chaque note avec la durée correspondante.
     *
     * @return void
     */
    void APP_PlaySong(void)
    {
        static uint16_t songDurations[10] = {
            500, 700, 500, 700, 700,
            700, 700, 700, 700, 1000
        };
        uint8_t i = 0;
        for (i = 0; i < SONG_LENGTH; i++)
        {
            APP_SetTMR0_Frequency(songMelody[i]);
            DRV_TMR0_Start();
            APP_WaitStart(songDurations[i]);
            DRV_TMR0_Stop();
            APP_WaitStart(40); // Petite pause
        }
    } 

/*******************************************************************************
 End of File
 */
