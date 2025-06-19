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
#include "taskctrl.h"
#include "eventbus.h"
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
extern app_task_ctrl_t inputsTaskCtrl;
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

void APPIN_Initialize(void) {
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

void APPIN_Tasks(void) {
    static int8_t ret;

    /* Check the application's current state. */
    switch (appinData.state) {
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
            for (i = 0; i < 14; i++) {
                appinData.valAD[i] = 0;
            }
            appinData.state = APPIN_STATE_IDLE;
            appinData.evt_sw = 0;
            appinData.evt_ad = 0;
            break;

        }

        case APPDISP_STATE_SERVICE_TASKS:
        {




            touchTaskCtrl.isActive = false; // re-enable touch task
            ledTaskCtrl.isActive = false;
            displayTaskCtrl.isActive = false;
            //rtcTackCtrl.isActive = false;
            APP_AdcReadAllSamples();
            APP_GetInputsStates();

            if (appinData.lastSySwitch.SwitchConfigs != appinData.SySwitch.SwitchConfigs) {
                inputsTaskCtrl.isDirty = true; // reset after full redraw 
                appinData.evt_sw = 1;
            }
            appinData.lastSySwitch.SwitchConfigs = appinData.SySwitch.SwitchConfigs;

            ret = memcmp(appinData.lastvalAD, appinData.valAD, 14 * sizeof (ADC_SAMPLE));

            if (ret != 0) {
                appinData.evt_ad = 1;
                inputsTaskCtrl.isDirty = true; // reset after full redraw
            }

            memcpy(appinData.lastvalAD, appinData.valAD, 14 * sizeof (ADC_SAMPLE));
            appinData.state = APPIN_STATE_IDLE;
            break;
        }



        case APPIN_STATE_IDLE:
        {

            if (inputsTaskCtrl.isDirty = true) {
                if (appinData.evt_sw) {
                    appinData.evt_sw = 0;
                    App_EventBus_Publish(EVT_INPUTS, &appinData.SySwitch.SwitchConfigs);

                }
                if (appinData.evt_ad) {
                    appinData.evt_ad = 0;
                    App_EventBus_Publish(EVT_INPUTS, &appinData.valAD);

                }
            }

            inputsTaskCtrl.isDirty = false;
            touchTaskCtrl.isActive = true; // re-enable touch task
            ledTaskCtrl.isActive = true;
            displayTaskCtrl.isActive = true;
            rtcTaskCtrl.isActive = true;

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

//    lastSample = 1000, sample = 1051 ? abs(1051 - 1000) = 51 ? saved
//    lastSample = 1000, sample = 950 ? abs(950 - 1000) = 50 ? saved
//    lastSample = 1000, sample = 980 ? abs(980 - 1000) = 20 ? not saved

void APP_AdcReadAllSamples(void) {
    uint8_t i = 0;
    static uint8_t SampleReadyToRead;
  
    SampleReadyToRead = DRV_ADC_SamplesAvailable();

    if (SampleReadyToRead) {
        for (i = 0; i < 14; i++) {

            appinData.valAD[i] = DRV_ADC_SamplesRead(i);
            if ( !(abs(  appinData.valAD[i] -   appinData.lastvalAD[i]) >= 50)) {
                 appinData.valAD[i] =appinData.lastvalAD[i];
            }
            

        }
    }
}

void APP_GetInputsStates(void) {


    //SPB's outpus derrranement states 
    appinData.SySwitch.SPBIn1_conf = SC3StateGet();
    appinData.SySwitch.SPBIn2_conf = SC2StateGet();
    appinData.SySwitch.SPBIn2_conf = SC1StateGet();
    appinData.SySwitch.FreeIn1_conf = FC1StateGet();
    appinData.SySwitch.FreeIn2_conf = FC2StateGet();
    appinData.SySwitch.FreeIn3_conf = FC3StateGet();
    appinData.SySwitch.FreeIn4_conf = FC4StateGet();
    appinData.SySwitch.FreeIn5_conf = FC5StateGet();
}

void APP_TIMER_AD_CALL_BACK(void) {

    //200ms call back wee want read each time as its slow
    appinData.state = APPDISP_STATE_SERVICE_TASKS;


}


/*******************************************************************************
 End of File
 */
