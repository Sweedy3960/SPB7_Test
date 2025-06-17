/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    apptouch.c

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

#include "apptouch.h"

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

APPTOUCH_DATA apptouchData;
S_AT42QT2120 s_newDataSensor; //Structure pour envoie des nouvelles datas
S_AT42QT2120 s_dataSensor; //Structure pour l'envoie des datas
S_AT42QT2120 s_getDataSensor; //Structure pour la rec�ption des datas
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
    void APPTOUCH_Initialize ( void )

  Remarks:
    See prototype in apptouch.h.
 */

void APPTOUCH_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    apptouchData.state = APPTOUCH_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APPTOUCH_Tasks ( void )

  Remarks:
    See prototype in apptouch.h.
 */

void APPTOUCH_Tasks ( void )
{

   /* Check the application's current state. */
    switch ( apptouchData.state )
    {
        /* Application's initial state. */
        case APPTOUCH_STATE_INIT:
        {
            AT42QT_Init();
            apptouchData.state = APPTOUCH_STATE_SERVICE_TASKS;
            DRV_TMR1_Start();
            LIFELED_GREENOff();
            break;
        }

        case APPTOUCH_STATE_SERVICE_TASKS:
        {
//             if (!touchTaskCtrl.isActive)
//                break;
             
            s_dataSensor.valKey0to7 = AT42QT_Read_Key0to7(0);
            s_dataSensor.valKey8to11 = AT42QT_Read_Key8to11(0);
            apptouchData.touchStates = ((s_dataSensor.valKey8to11 <<8)| s_dataSensor.valKey0to7);
             if (apptouchData.touchStates != apptouchData.lastTouchStates) {
                //set the flag 
               // touchTaskCtrl.isDirty = true;
                apptouchData.lastTouchStates = apptouchData.touchStates;
            }

            apptouchData.state = APPTOUCH_STATE_IDLE;
           

            break;
        }
        case APPTOUCH_STATE_IDLE:
        {
//            if (touchTaskCtrl.isDirty) {
//             //send the event
//             App_EventBus_Publish(EVT_TOUCH, &apptouchData.touchStates);
//             touchTaskCtrl.isDirty = false;  // clear after publishing
//    }
            
        }
        break;
        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 void APP_TIMER2_CALLBACK(void)
{
    //time the app
    apptouchData.state = APPTOUCH_STATE_SERVICE_TASKS;
}


/*******************************************************************************
 End of File
 */
