/* ************************************************************************** */
/** Descriptive File Name

  @Company
    ETML-ES

  @File Name
    PIC32130_AT42QT2120_I2C.h

  @Autor
    David Coelho

  @Description
    Déclaration des variables et constantes qu'on a besoin pour communiquer 
    en I2C avec l'AT42QT2120
 
***************************************************************************** */

#ifndef _PIC32130_AT42QT2120_I2C_H    /* Guard against multiple inclusion */
#define _PIC32130_AT42QT2120_I2C_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "appTouch.h"
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
/*-----------------------------Constants--------------------------------------*/

//#define I2C_ADDRESS_WRITE 0X1C //Address WRITE
//#define I2C_ADDRESS_READ  0X1D //Address READ

#define I2C_ADDRESS_WRITE 0X38 //Address WRITE 0X1C + WRITE 0
#define I2C_ADDRESS_READ  0X39 //Address READ  0X1C + READ 1
    
#define WHEEL_ENABLE      0X80 //Enable WHEEL on channels 0,1 and 2
#define DETECT_THRESHOLD  0X0C //Change the value for the key    
    
/*Déclaration des registres*/
 /*-----NAME REGISTRE    ADDRESSE  DESCRITPTION-------------------*/
#define CHIP_ID          0 //READ - 0X3E
#define FIRMWARE_VERSION 1 //READ - MSB = Majour version - LSB = Minor version
#define DETECTION_STATUS 2 //READ
#define KEY_STATUS0      3 //READ - Key0 To Key7
#define KEY_STATUS1      4 //READ - Key8 to Key 11- 4 bits MSB RESERVED
#define SLIDER_POSITION  5 //READ 
#define CALIBRATE        6 //READ/WRITE
#define RESET            7 //READ/WRITE
#define LP               8 //READ/WRITE - LOW POWER MODE
#define TTD              9 //READ/WRITE - TOWARDS TOUCH 
#define ATD             10 //READ/WRITE - AWAY FROM TOUCH
#define DI              11 //READ/WRITE - Detection Integrator
#define TRD             12 //READ/WRITE - Touch Recal Delay
#define DHT             13 //READ/WRITE - Drift Hold Time
#define SLIDER_OPTION   14 //READ/WRITE - Bit7 HIGH-configured-Bit 6 LOW = WHEEL
#define CHARGE_TIME     15 //NOT USE 
    
/*-----------------KEY DETECT THRESHOLD-------------------------*/    
#define KEY0_DETECT     16 //READ/WRITE
#define KEY1_DETECT     17 //READ/WRITE
#define KEY2_DETECT     18 //READ/WRITE
#define KEY3_DETECT     19 //READ/WRITE
#define KEY4_DETECT     20 //READ/WRITE
#define KEY5_DETECT     21 //READ/WRITE
#define KEY6_DETECT     22 //READ/WRITE
#define KEY7_DETECT     23 //READ/WRITE
#define KEY8_DETECT     24 //READ/WRITE
#define KEY9_DETECT     25 //READ/WRITE
#define KEY10_DETECT    26 //READ/WRITE
#define KEY11_DETECT    27 //READ/WRITE
    
/*KEY CONTROLE BIT7 TO5 RESERVED-BIT 4 GUARD-BIT3 TO 2 AKS-BIT1 GPO-BIT0 EN*/
#define KEYO_CONTROL    28 //READ/WRITE
#define KEY1_CONTROL    29 //READ/WRITE
#define KEY2_CONTROL    30 //READ/WRITE
#define KEY3_CONTROL    31 //READ/WRITE
#define KEY4_CONTROL    32 //READ/WRITE
#define KEY5_CONTROL    33 //READ/WRITE
#define KEY6_CONTROL    34 //READ/WRITE
#define KEY7_CONTROL    35 //READ/WRITE
#define KEY8_CONTROL    36 //READ/WRITE
#define KEY9_CONTROL    37 //READ/WRITE
#define KEY1O_CONTROL   38 //READ/WRITE
#define KEY11_CONTROL   39 //READ/WRITE

/*---------------------KEY PULSE SCALE - READ/WRITE-----------------------*/    
#define KEY0_PULSE_SCALE    40 //READ/WRITE
#define KEY1_PULSE_SCALE    41 //READ/WRITE
#define KEY2_PULSE_SCALE    42 //READ/WRITE
#define KEY3_PULSE_SCALE    43 //READ/WRITE
#define KEY4_PULSE_SCALE    44 //READ/WRITE
#define KEY5_PULSE_SCALE    45 //READ/WRITE
#define KEY6_PULSE_SCALE    46 //READ/WRITE
#define KEY7_PULSE_SCALE    47 //READ/WRITE
#define KEY8_PULSE_SCALE    48 //READ/WRITE
#define KEY9_PULSE_SCALE    49 //READ/WRITE
#define KEY10_PULSE_SCALE   50 //READ/WRITE
#define KEY11_PULSE_SCALE   51 //READ/WRITE

/*------------------------------KEY SIGNAL - READ----------------------------*/    
#define KEY0_SIGNAL_MSB     52
#define KEY0_SIGNAL_LSB     53
    
#define KEY1_SIGNAL_MSB     54
#define KEY1_SIGNAL_LSB     55
    
#define KEY2_SIGNAL_MSB     56
#define KEY2_SIGNAL_LSB     57
    
#define KEY3_SIGNAL_MSB     58
#define KEY3_SIGNAL_LSB     59
    
#define KEY4_SIGNAL_MSB     60
#define KEY4_SIGNAL_LSB     61
    
#define KEY5_SIGNAL_MSB     62
#define KEY5_SIGNAL_LSB     63
    
#define KEY6_SIGNAL_MSB     64
#define KEY6_SIGNAL_LSB     65
    
#define KEY7_SIGNAL_MSB     66
#define KEY7_SIGNAL_LSB     67
    
#define KEY8_SIGNAL_MSB     68
#define KEY8_SIGNAL_LSB     69   
    
#define KEY9_SIGNAL_MSB     70
#define KEY9_SIGNAL_LSB     71
    
#define KEY10_SIGNAL_MSB    72
#define KEY10_SIGNAL_LSB    73  
    
#define KEY11_SIGNAL_MSB    74
#define KEY11_SIGNAL_LSB    75  
    
/*--------------------REFERENCE DATA  - READ--------------------------------*/    
#define REFERENCE0_MSB      76
#define REFERENCE0_LSB      77
    
#define REFERENCE1_MSB      78
#define REFERENCE1_LSB      79
    
#define REFERENCE2_MSB      80
#define REFERENCE2_LSB      81
    
#define REFERENCE3_MSB      82
#define REFERENCE3_LSB      83
    
#define REFERENCE4_MSB      84
#define REFERENCE4_LSB      85
    
#define REFERENCE5_MSB      86
#define REFERENCE5_LSB      87
    
#define REFERENCE6_MSB      88
#define REFERENCE6_LSB      89
    
#define REFERENCE7_MSB      90
#define REFERENCE7_LSB      91
    
#define REFERENCE8_MSB      92
#define REFERENCE8_LSB      93   
    
#define REFERENCE9_MSB      94
#define REFERENCE9_LSB      95
    
#define REFERENCE10_MSB     96
#define REFERENCE10_LSB     97  
    
#define REFERENCE11_MSB     98
#define REFERENCE11_LSB     99  
    
    
    
    
    
// *****************************************************************************
// *****************************************************************************
// Section: Structure Types
// *****************************************************************************
// *****************************************************************************

 

// *****************************************************************************
// *****************************************************************************
// Section: Prototype Functions
// *****************************************************************************
// *****************************************************************************
void AT42QT_Init(void);
void AT42QT_Write(uint8_t adr_Registre_W, uint8_t data_Write);
uint8_t AT42QT_Read(uint8_t adr_Registre_R, bool n_ACK);

/*-- Fonction pour lire des registre spécifique --*/
uint8_t AT42QT_Read_Key0to7(bool n_ACK);
uint8_t AT42QT_Read_Key8to11(bool n_ACK);
uint8_t AT42QT_Read_Wheel(bool n_ACK);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _PIC32130_AT42QT2120_I2C_H */

/* *****************************************************************************
 End of File
 */
