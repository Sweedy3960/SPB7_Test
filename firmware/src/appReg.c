/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    appreg.c

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

#include "appReg.h"
#include "taskctrl.h"
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

APPREG_DATA appregData;
app_task_ctrl_t  ledTaskCtrl;
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
    void APPREG_Initialize ( void )

  Remarks:
    See prototype in appreg.h.
 */

void APPREG_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appregData.state = APPREG_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APPREG_Tasks ( void )

  Remarks:
    See prototype in appreg.h.
 */

void APPREG_Tasks ( void )
{

    /* Check the application's current state. */
    switch (appregData.state) {
            /* Application's initial state. */
        case APPREG_STATE_INIT:
        {

            SR_Init(&appregData.sysLeds);

            appregData.state = APPREG_STATE_SERVICE_TASKS;

            break;
        }

        case APPREG_STATE_SERVICE_TASKS:
        {
            if (!ledTaskCtrl.isActive)
                break;

            if (appregData.lastSysLeds.cmd_leds != appregData.sysLeds.cmd_leds) {
               // set the flag 
                ledTaskCtrl.isDirty = true;

            }


            appregData.state = APPREG_STATE_IDLE;
            break;
        }

            /* TODO: implement your application state machine.*/
        case APPREG_STATE_IDLE:
        {
            if (ledTaskCtrl.isDirty) {



                touchTaskCtrl.isActive = false; // disable touch task while updating display
                displayTaskCtrl.isActive = false;
                inputsTaskCtrl.isActive = false;
                SR_Update(&appregData.sysLeds);
                ledTaskCtrl.isDirty = false; // clear after updating
                touchTaskCtrl.isActive = true; // disable touch task while updating display
                displayTaskCtrl.isActive = true;
                inputsTaskCtrl.isActive = true;
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
}
void APP_WaitStart(uint16_t waitingTime_ms) {

    appregData.AppDelay = waitingTime_ms - 1;
    DRV_TMR3_Start();
    appregData.APP_DelayTimeIsRunning = 1;
    // Garde-fou : timeout logiciel (2x le temps demandÃ©)


    while (appregData.APP_DelayTimeIsRunning) {

    }
    DRV_TMR3_Stop();
}

void APP_TIMER4_CALLBACK(void) {
    if (appregData.AppDelay > 0) {
        appregData.AppDelay--;
    } else {
        appregData.APP_DelayTimeIsRunning = 0;
    }

}

void APP_SER_SET_CMD_LED(uint16_t cmd) {

    appregData.sysLeds.cmd_leds = cmd;
}

void App_LED_HandleTouch(uint16_t *touchStates) {
    // static uint16_t lastTouchStates = 0;
    // mettre ï¿½ jour l?affichage selon les touches dï¿½tectï¿½es
    // Par exemple?: changer l'ï¿½tat du menu
    // Pour l?instant, on se contente de marquer la tï¿½che display comme "dirty"

   //ledTaskCtrl.isDirty = true;
    switch (*touchStates) {
            //SIMPLE TOUCH
        case KEY_UP_L_MASK:
            APP_SER_SET_CMD_LED(~(*touchStates));
        case KEY_MID_L_MASK:
            APP_SER_SET_CMD_LED(~(*touchStates));

        case KEY_DOWN_L_MASK:

            APP_SER_SET_CMD_LED(~(*touchStates));
        case KEY_UP_C_MASK:

            APP_SER_SET_CMD_LED(~(*touchStates));
        case KEY_DOWN_C_MASK:

            APP_SER_SET_CMD_LED(~(*touchStates));
        case KEY_UP_R_MASK:

            APP_SER_SET_CMD_LED(~(*touchStates));
        case KEY_MID_R_MASK:
            APP_SER_SET_CMD_LED(~(*touchStates));

        case KEY_DOWN_R_MASK:
            APP_SER_SET_CMD_LED(~(*touchStates));
            break;
        default:
            break;
    }

    //lastTouchStates = *touchStates;
}
 
void App_LED_HandleInputs(uint16_t *InputStates) {
    // Teste l'état de "vanne" (signal 4)
    if (g_signalLineStates[4] == 1 || g_signalLineStates[4] == 0) {
        appregData.sysLeds.FREE_IN5_LED = 0; // Allume l'alarme si ER ou LN
        appregData.sysLeds.FREE_IN3_LED_SAVE = 0; // Garde la LED SAVE allumée
        Relay_CMD_ALARMOff();
        SPB_OUT1_CMDOff();
    } else {
        appregData.sysLeds.FREE_IN3_LED_SAVE = 1; // Éteint l'alarme si OK
        // Ne touche pas à ALARRM_LED_SAVE ici
       
        Relay_CMD_ALARMOn();
        SPB_OUT1_CMDOn();
    }
    ledTaskCtrl.isDirty = true;
    appregData.state = APPREG_STATE_IDLE;
}

/*******************************************************************************
 End of File
 */
