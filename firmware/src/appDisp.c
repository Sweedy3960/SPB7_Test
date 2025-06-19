/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

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

#include "appDisp.h"
#include "Display.h"
#include "LCD_Driver.h"
#include "ugui.h"
#include "ugui_config.h"
#include "appBuzz.h"
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

APPDISP_DATA appDispData;
app_task_ctrl_t displayTaskCtrl;

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
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APPDISP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    appDispData.state = APPDISP_STATE_INIT;


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APPDISP_Tasks(void) {
    static uint8_t tmrScr = 0;
    /* Check the application's current state. */
    switch (appDispData.state) {
            /* Application's initial state. */
        case APPDISP_STATE_INIT:
        {
            LCD_RWOff();
            LCD_BLOn();
            DRV_TMR4_Start();
            DisplayInit();
            //timing set at 300
            appDispData.TimerScreen = 300;
            appDispData.state = APPDISP_STATE_SERVICE_TASKS;
            appDispData.needDisplayUpdate = 1;
            appDispData.dispInit = 0;

            break;
        }

        case APPDISP_STATE_SERVICE_TASKS:
        {
           if (!displayTaskCtrl.isActive)
                break;

            // === Startup logo sequence ===
            if (appDispData.needDisplayUpdate) {
                if (tmrScr >= 3) {
                    tmrScr = DISP_SIGN;           // move to default screen
                    appDispData.dispInit = 1;
                }
                App_Display_ChangeScreen(tmrScr, 0, true);    // show welcome screen (force)
                tmrScr++;
                appDispData.needDisplayUpdate = 0;
            }

            // test if we need display and reset flag afer
            if (displayTaskCtrl.isDirty && appDispData.dispInit) {
                App_Display_ChangeScreen(appDispData.currentScreen, 0, true);  //  current UI state (force)
                displayTaskCtrl.isDirty = false;    // clear dirty flag
            }

            Display_Task();  // update UG_GUI and such
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


void App_Display_HandleTouch(uint16_t *touchStates) {
    static uint16_t lastTouchStates =0;
    // mettre � jour l?affichage selon les touches d�tect�es
    // Par exemple?: changer l'�tat du menu
    // Pour l?instant, on se contente de marquer la t�che display comme "dirty"
    if (!appDispData.dispInit)
        return; // Ignore touches until startup is complete
    
    //displayTaskCtrl.isDirty = true;
    switch (*touchStates) {
            //SIMPLE TOUCH
        case KEY_UP_L_MASK:
           
        case KEY_MID_L_MASK:
            APPBUZZ_SetState(APPBUZZ_STATE_SERVICE_TASKS);
           
        case KEY_DOWN_L_MASK:

            
        case KEY_UP_C_MASK:

           
        case KEY_DOWN_C_MASK:

           
        case KEY_UP_R_MASK:

           
        case KEY_MID_R_MASK:

         
        case KEY_DOWN_R_MASK:

            
             switch (appDispData.currentScreen) {
                case DISP_SCR_MENU:
                    // Handle up left key in menu
                    //to make it work a non blockig way we call a function to set flag then flag is checked 
                    // and the screen is changed
                    // this is not good for performance but it is easier to understand
                    appDispData.needDisplayUpdate = 1;
                    App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, false);
                    
                    break;
                case DISP_CHANGE_SIGN_NAME:
                    // Handle up left key in change sign name screen
                    // This could be used to change the name of the sign
                    
                    break;
                default:
                   
                    break;
            }
            break;

            //SIMPLE COMBO TOUCH   

            //LEFFT
        case ((KEY_UP_L_MASK | KEY_MID_L_MASK)):
            //

            break;

        case ((KEY_UP_L_MASK | KEY_DOWN_L_MASK)):
            //

            break;
        case ((KEY_MID_L_MASK | KEY_DOWN_L_MASK)):
            /**
             * @brief Changement d'écran vers le menu via App_Display_ChangeScreen (non forcé).
             */
            App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, false);
            appDispData.currentScreen = DISP_SCR_MENU; // Update current screen state
            break;
            //RIGHT
        case ((KEY_UP_R_MASK | KEY_MID_R_MASK)):
            //
             App_Display_ChangeScreen(DISP_CHANGE_SIGN_NAME, touchStates, false);
             appDispData.currentScreen = DISP_CHANGE_SIGN_NAME; // Update current screen state
            break;

        case ((KEY_UP_R_MASK | KEY_DOWN_R_MASK)):
            //

            break;
        case ((KEY_MID_R_MASK | KEY_DOWN_R_MASK)):
            //

            break;
            //center 
        case ((KEY_UP_C_MASK | KEY_DOWN_C_MASK)):
            //
             
            break;




            //LEFT +CENTER


        case ((KEY_UP_L_MASK | KEY_DOWN_C_MASK)):
            //

            break;
        case ((KEY_UP_L_MASK | KEY_UP_C_MASK)):
            //

            break;

        case ((KEY_MID_L_MASK | KEY_DOWN_C_MASK)):
            //

            break;
        case ((KEY_MID_L_MASK | KEY_UP_C_MASK)):
            //

            break;
        case ((KEY_DOWN_L_MASK | KEY_DOWN_C_MASK)):
            //

            break;
        case ((KEY_DOWN_L_MASK | KEY_UP_C_MASK)):
            //

            break;


            //RIGHT +CENTER    

        case ((KEY_UP_R_MASK | KEY_DOWN_C_MASK)):
            //

            break;
        case ((KEY_UP_R_MASK | KEY_UP_C_MASK)):
            //

            break;

        case ((KEY_MID_R_MASK | KEY_DOWN_C_MASK)):
            //

            break;
        case ((KEY_MID_R_MASK | KEY_UP_C_MASK)):
            //

            break;
        case ((KEY_DOWN_R_MASK | KEY_DOWN_C_MASK)):
            //

            break;
        case ((KEY_DOWN_R_MASK | KEY_UP_C_MASK)):
            //

            break;

        case 0:
             //displayTaskCtrl.isDirty = true; 
            //touche released so rerender
            App_Display_ChangeScreen(appDispData.currentScreen,&lastTouchStates,true);
            break;

            /*
        //HARD COMBO TOUCH   
        case ((KEY_UP_L_MASK|KEY_MID_L_MASK|KEY_UP_C_MASK)):
            //
            // DisplayScreen(DISP_SCR_MENU,touchStates ,true);
            App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, true);
            break;  
            
        case ((KEY_UP_L_MASK | KEY_DOWN_L_MASK|KEY_UP_C_MASK)):
            //
            // DisplayScreen(DISP_SCR_MENU,touchStates ,true);
            App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, true);
            break;    
         case ((KEY_MID_L_MASK|KEY_DOWN_L_MASK|KEY_UP_C_MASK)):
            //
            // DisplayScreen(DISP_SCR_MENU,touchStates ,true);
            App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, true);
            break;   
            
        case ((KEY_UP_R_MASK|KEY_MID_R_MASK|KEY_UP_C_MASK)):
            //
            // DisplayScreen(DISP_SCR_MENU,touchStates ,true);
            App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, true);
            break;  
            
        case ((KEY_UP_R_MASK | KEY_DOWN_R_MASK|KEY_UP_C_MASK)):
            //
            // DisplayScreen(DISP_SCR_MENU,touchStates ,true);
            App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, true);
            break;    
         case ((KEY_MID_R_MASK|KEY_DOWN_R_MASK|KEY_UP_C_MASK)):
            //
            // DisplayScreen(DISP_SCR_MENU,touchStates ,true);
            App_Display_ChangeScreen(DISP_SCR_MENU, touchStates, true);
            break;           
             */




        default:
            //nothing is pressed or combo not handled
            //DisplayScreen(DISP_SCR_ERROR, touchStates, true);
            //if released continue last thing 
            break;
    }

   lastTouchStates =  *touchStates;


}
void App_Display_ChangeScreen(uint8_t newScreen, uint16_t *touchStates, bool forceUpdate)
{
    touchTaskCtrl.isActive = false; // disable touch task while updating display
    ledTaskCtrl.isActive = false; //disable SR led updates
    inputsTaskCtrl.isActive = fasle;
    if (appDispData.currentScreen == newScreen && !forceUpdate)
        return;  // Skip if already on this screen and no forced redraw

    appDispData.currentScreen = newScreen;

    DisplayScreen(newScreen, touchStates, true);

    displayTaskCtrl.isDirty = false; // reset after full redraw
    touchTaskCtrl.isActive = true; // re-enable touch task
    inputsTaskCtrl.isActive = true; //reenable adc inputs
    ledTaskCtrl.isActive = true;
}


void APP_DISP_TIMER5_CALLBACK(void) {

    appDispData.TimerScreen--;
    if (appDispData.TimerScreen == 0) {
        appDispData.TimerScreen = 200;
        if (!appDispData.dispInit) {
            appDispData.needDisplayUpdate = 1;
        }
    }
}

/*******************************************************************************
 End of File
 */
