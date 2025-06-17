/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef DRIVER_SR_SN74HCS596QPWRQ1_H    /* Guard against multiple inclusion */
#define DRIVER_SR_SN74HCS596QPWRQ1_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include "appReg.h"
/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
#define EXAMPLE_CONSTANT 0


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
     
    typedef enum {
        SR_IDLE,
        SR_SHIFTING,
        SR_LATCHING
    } SR_State;

    
    typedef union {

        struct {
            uint8_t FREE_IN2_LED : 1;
            uint8_t FREE_IN3_LED : 1;
            uint8_t FREE_IN4_LED : 1;
            uint8_t FREE_IN4_LED_SAVE : 1;
            uint8_t FREE_IN5_LED : 1;
            uint8_t FREE_IN5_LED_SAVE : 1;
            uint8_t FREE_IN3_LED_SAVE : 1;
            uint8_t led8 : 1;
            uint8_t FREE_IN2_LED_SAVE : 1;
            uint8_t FREE_IN1_LED : 1;
            uint8_t led11 : 1;
            uint8_t FREE_IN1_LED_SAVE : 1;
            uint8_t DERR_LED : 1;
            uint8_t DERR_LED_SAVE : 1;
            uint8_t ALARRM_LED:1;
            uint8_t ALARRM_LED_SAVE:1;

        };
        uint16_t cmd_leds;
        SR_State state;
    } SERIAL_REG_DATA;    
    typedef enum {
        ALARRM_LED = 15,
        ALARRM_LED_SAVE = 16,
        DERR_LED = 13,
        DERR_LED_SAVE = 14,
        FREE_IN1_LED = 10,
        FREE_IN1_LED_SAVE = 12,
        FREE_IN2_LED = 1,
        FREE_IN2_LED_SAVE = 9,
        FREE_IN3_LED = 2,
        FREE_IN3_LED_SAVE = 7,
        FREE_IN4_LED = 3,
        FREE_IN4_LED_SAVE = 4,
        FREE_IN5_LED = 5,
        FREE_IN5_LED_SAVE = 6,
        TEST_LED = 11,
    } FCT_LED;
 

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

      @Summary
        Brief one-line description of the function.

      @Description
        Full description, explaining the purpose and usage of the function.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Precondition
        List and describe any required preconditions. If there are no preconditions,
        enter "None."

      @Parameters
        @param param1 Describe the first parameter to the function.
    
        @param param2 Describe the second parameter to the function.

      @Returns
        List (if feasible) and describe the return values of the function.
        <ul>
          <li>1   Indicates an error occurred
          <li>0   Indicates an error did not occur
        </ul>

      @Remarks
        Describe any special behavior not described above.
        <p>
        Any additional remarks.

      @Example
        @code
        if(ExampleFunctionName(1, 2) == 0)
        {
            return 3;
        }
     */
   
void SR_Init(SERIAL_REG_DATA *SRegData);
void SR_Update(SERIAL_REG_DATA *SRegData);
void SR_LoadData(SERIAL_REG_DATA *SRegData, uint16_t data);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
