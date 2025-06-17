/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    appin.c

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

#include "appin.h"

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

APPIN_DATA appinData;

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
    void APPIN_Initialize ( void )

  Remarks:
    See prototype in appin.h.
 */

void APPIN_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appinData.state = APPIN_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APPIN_Tasks ( void )

  Remarks:
    See prototype in appin.h.
 */

void APPIN_Tasks ( void )
{

     /* Check the application's current state. */
    switch ( appinData.state )
    {
        /* Application's initial state. */
        case APPIN_STATE_INIT:
        {
            uint8_t i;
            // Initialize ADC driver
            DRV_ADC_Initialize();
            // Open ADC driver
            DRV_ADC_Open();
            // Start ADC driver
            DRV_ADC_Start();
            
            DRV_TMR2_Start();
            DRV_TMR1_Start();
            // Initialize ADC samples
            for (i = 0; i < 14; i++)
            {
                appinData.valAD[i] = 0;
            }
            appinData.state = APPDISP_STATE_SERVICE_TASKS;
            break;
            
        }

        case APPDISP_STATE_SERVICE_TASKS:
        {
            APP_AdcReadAllSamples();
            APP_GetInputsStates();
            appinData.state = APPIN_STATE_IDLE;
            break;
        }
         case APPIN_STATE_IDLE:
        {
           
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}
void APP_AdcReadAllSamples(void)
    {
        uint8_t i = 0;
        static uint8_t SampleReadyToRead;
        SampleReadyToRead = DRV_ADC_SamplesAvailable();

        if (SampleReadyToRead)
        {
            for (i = 0; i < 14; i++)
            {
                appinData.valAD[i] = DRV_ADC_SamplesRead(i);


            }
        }
    }
  void APP_GetInputsStates(void)
    {


        //SPB's outpus derrranement states 
        appinData.SySwitch.SPBIn1_conf.state = SC3StateGet();
        appinData.SySwitch.SPBIn2_conf.state = SC2StateGet();
        appinData.SySwitch.SPBIn2_conf.state = SC1StateGet();
        appinData.SySwitch.FreeIn1_conf.state = FC1StateGet();
        appinData.SySwitch.FreeIn2_conf.state = FC2StateGet();
        appinData.SySwitch.FreeIn3_conf.state = FC3StateGet();
        appinData.SySwitch.FreeIn4_conf.state = FC4StateGet();
        appinData.SySwitch.FreeIn5_conf.state = FC5StateGet();
    }
  
  
  void APP_TIMER_AD_CALL_BACK(void)
  {
      
      //200ms call back wee want read each time as its slow
      appinData.state = APPDISP_STATE_SERVICE_TASKS;
      
      
  }
 

/*******************************************************************************
 End of File
 */
