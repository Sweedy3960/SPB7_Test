/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    appin.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _APPIN_H
#define _APPIN_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
    // DOM-IGNORE-END 

    // *****************************************************************************
    // *****************************************************************************
    // Section: Type Definitions
    // *****************************************************************************
    // *****************************************************************************

    // *****************************************************************************

    /* Application states

      Summary:
        Application states enumeration

      Description:
        This enumeration defines the valid application states.  These states
        determine the behavior of the application at various times.
     */

    typedef enum {
        /* Application's state machine's initial state. */
        APPIN_STATE_INIT = 0,
        APPIN_STATE_SERVICE_TASKS,
        APPIN_STATE_IDLE,
        /* TODO: Define states used by the application state machine. */

    } APPIN_STATES;


    // *****************************************************************************

        /* Application Data

      Summary:
        Holds application data

      Description:
        This structure holds the application's data.

      Remarks:
        Application strings and buffers are be defined outside this structure.
     */
    typedef enum {
        //AIN switch can set if it's an 0-10v or 4-20mA input
        AIN_0_10V = 0,
        AIN_4_20mA = 1,
        //
    } ConfigStatesAIN;

    typedef struct {
        uint8_t state : 1;

    } SwithcConfig;

    typedef enum {
        //SPBIn are used to redirect the SPBin to alarm/derrangement output
        SPB_IN_DONOT = 0,
        SPB_IN_REPEAT = 1,
    } ConfigStatesSPBs;

    typedef enum {
        //FreeInputs are used to redirect the inputs  to the FreeOutputs corresponding
        FREE_IN_DONOT = 0,
        FREE_IN_REPEAT = 1,
    } ConfigStatesFreeIn;

    typedef union {

        struct {
            uint8_t Ain1_conf : 1;
            uint8_t Ain2_conf : 1;
            uint8_t Ain3_conf : 1;

            uint8_t FreeIn1_conf : 1;
            uint8_t FreeIn2_conf : 1;
            uint8_t FreeIn3_conf : 1;
            uint8_t FreeIn4_conf : 1;
            uint8_t FreeIn5_conf : 1;

            uint8_t SPBIn1_conf : 1;
            uint8_t SPBIn2_conf : 1;
            uint8_t SPBIn3_conf : 1;
        };
        //les 12 derniers bit seulement
        uint16_t SwitchConfigs;
    } INPUTS_DATA;

    typedef struct {
        /* The application's current state */
        APPIN_STATES state;
        ADC_SAMPLE valAD[14];
        ADC_SAMPLE lastvalAD[14];
        INPUTS_DATA SySwitch;
        INPUTS_DATA lastSySwitch;
        bool evt_ad;
        bool evt_sw; 
        /* TODO: Define any additional data used by the application. */

    } APPIN_DATA;


    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Callback Routines
    // *****************************************************************************
    // *****************************************************************************
    /* These routines are called by drivers when certain events occur.
     */

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Initialization and State Machine Functions
    // *****************************************************************************
    // *****************************************************************************

    /*******************************************************************************
      Function:
        void APPIN_Initialize ( void )

      Summary:
         MPLAB Harmony application initialization routine.

      Description:
        This function initializes the Harmony application.  It places the 
        application in its initial state and prepares it to run so that its 
        APP_Tasks function can be called.

      Precondition:
        All other system initialization routines should be called before calling
        this routine (in "SYS_Initialize").

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        APPIN_Initialize();
        </code>

      Remarks:
        This routine must be called from the SYS_Initialize function.
     */

    void APPIN_Initialize(void);


    /*******************************************************************************
      Function:
        void APPIN_Tasks ( void )

      Summary:
        MPLAB Harmony Demo application tasks function

      Description:
        This routine is the Harmony Demo application's tasks function.  It
        defines the application's state machine and core logic.

      Precondition:
        The system and application initialization ("SYS_Initialize") should be
        called before calling this.

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        APPIN_Tasks();
        </code>

      Remarks:
        This routine must be called from SYS_Tasks() routine.
     */

    void APPIN_Tasks(void);
    void APP_AdcReadAllSamples(void);
    void APP_GetInputsStates(void);
    void APP_TIMER_AD_CALL_BACK(void);

#endif /* _APPIN_H */

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

