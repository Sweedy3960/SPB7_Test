/* ************************************************************************** */
/** Descriptive File Name

  @Company
    ETML-ES

  @File Name
    PIC32130_AT42QT2120_I2C.c

  @Autor
    David Coelho

  @Description
    Permet de communiquer en I2C avec le capteur capacitif AT42QT2120. 
    
************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "PIC32130_AT42QT2120_I2C.h"
#include "appTouch.h"
#include "Mc32_I2cUtilCCS.h"


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/***************************************************************************
* Fonction :  AT42QT_Init                                                  *
*                                                                          *
* Paramètre d'entrée : Rien                                                *
*                                                                          *
* Paramètre de sortie : Rien                                               *
*                                                                          *
* Description : Permet d'initialisé la communication entre le uC et le     *
* capteur capacitif.                                                       *
*                                                                          *
***************************************************************************/
 void AT42QT_Init(void) 
 {
     //bool Fast = false; //false = LOW speed / true= HIGH speed
     
     i2c_init(1);    //Initlialisation de l'I2C
    // AT42QT_Write(); //Detection du potentiomètre
     
     /*-- Changment de sensibilité des boutons --*/
     AT42QT_Write(KEY1_DETECT, DETECT_THRESHOLD);
     AT42QT_Write(KEY2_DETECT, DETECT_THRESHOLD);
     AT42QT_Write(KEY3_DETECT, DETECT_THRESHOLD);
     AT42QT_Write(KEY4_DETECT, DETECT_THRESHOLD);
     AT42QT_Write(KEY5_DETECT, DETECT_THRESHOLD);
     AT42QT_Write(KEY6_DETECT, DETECT_THRESHOLD);
     
 }
 
/***************************************************************************
* Fonction :  AT42QT_Write                                                 *
*                                                                          *
* Paramètre d'entrée : uint8_t adresse du registre pour écrire             *
*                      uint8_t data que l'on veut érire                    *
*                                                                          *
* Paramètre de sortie : Rien                                               *
*                                                                          *
* Description : Permet d'écrire une valeur sur un registre                 *
*                                                                          *
***************************************************************************/
void AT42QT_Write(uint8_t adr_Registre_W, uint8_t data_Write)
{
    /* Ecrire une valeur dans un registre */
    i2c_start();
    i2c_write(I2C_ADDRESS_WRITE);
    i2c_write(adr_Registre_W);
    i2c_write(data_Write);
    i2c_stop();
}

/***************************************************************************
* Fonction :  AT42QT_Read                                                  *
*                                                                          *
* Paramètre d'entrée : uint8_t adresse du registre pour lire               *
*                      bool savoir s'il faut un ACK ou non                 *
*                      true = ACK / false = NACK                           *
*                                                                          *
* Paramètre de sortie : une valeur en uint8_t                              *
*                                                                          *
* Description : Permet de lire une valeur qui se trouve dans un registre   *
*                                                                          *
***************************************************************************/

uint8_t AT42QT_Read(uint8_t adr_Registre_R, bool n_ACK)
{
    uint8_t valI2C = 0;
    
    /*Lire une valeur sur l'IC AT42QT2120*/
    i2c_start();
    i2c_write(I2C_ADDRESS_WRITE);
    i2c_write(adr_Registre_R);
    i2c_stop();

    i2c_start();
    i2c_write(I2C_ADDRESS_READ);
    valI2C = i2c_read(n_ACK);    
    i2c_stop();
    
    return(valI2C);//Retour de la valeur lu
}

/***************************************************************************
* Fonction :  AT42QT_Read_Key0to7                                          *
*                                                                          *
* Paramètre d'entrée : bool savoir s'il faut un ACK ou non                 *
*                      true = ACK / false = NACK                           *
*                                                                          *
* Paramètre de sortie : une valeur en uint8_t                              *
*                                                                          *
* Description : Permet de lire dans le registre "Key Status". Cela permet  * 
* de savoir s'il y a eu une détection sur un des boutons.                  *
*                                                                          *
***************************************************************************/
uint8_t AT42QT_Read_Key0to7(bool n_ACK)
{
    uint8_t valI2C = 0;
    
    i2c_start();
    i2c_write(I2C_ADDRESS_WRITE);
    i2c_write(KEY_STATUS0); 
    i2c_stop();

    i2c_start();
    i2c_write(I2C_ADDRESS_READ);
    valI2C = i2c_read(n_ACK);    
    i2c_stop();
    
    return(valI2C);
}

/***************************************************************************
* Fonction :  AT42QT_Read_Key8to11                                         *
*                                                                          *
* Paramètre d'entrée : bool savoir s'il faut un ACK ou non                 *
*                      true = ACK / false = NACK                           *
*                                                                          *
* Paramètre de sortie : une valeur en uint8_t                              *
*                                                                          *
* Description : Permet de lire dans le registre "Key Status". Cela permet  * 
* de savoir s'il y a eu une détection sur un des boutons.                  *
*                                                                          *
***************************************************************************/
uint8_t AT42QT_Read_Key8to11(bool n_ACK)
{
    uint8_t valI2C = 0;
    
    i2c_start();
    i2c_write(I2C_ADDRESS_WRITE);
    i2c_write(KEY_STATUS1); 
    i2c_stop();

    i2c_start();
    i2c_write(I2C_ADDRESS_READ);
    valI2C = i2c_read(n_ACK);    
    i2c_stop();
    
    return(valI2C);
}

/***************************************************************************
* Fonction :  AT42QT_Read_Wheel                                            *
*                                                                          *
* Paramètre d'entrée : bool savoir s'il faut un ACK ou non                 *
*                      true = ACK / false = NACK                           *
*                                                                          *
* Paramètre de sortie : une valeur en uint8_t                              *
*                                                                          *
* Description : Permet de lire dans le registre "SLIDER POSITION" ce qui   * 
* va nous renvoyer une valeur entre 0 à 255 par rapport ou l'utilisateur à    *
* toucher le potentiomètre                                                 *
*                                                                          *
***************************************************************************/
uint8_t AT42QT_Read_Wheel(bool n_ACK)
{
    uint8_t valI2C = 0;
    
    i2c_start();
    i2c_write(I2C_ADDRESS_WRITE);
    i2c_write(SLIDER_POSITION); 
    i2c_stop();
    i2c_start();
    i2c_write(I2C_ADDRESS_READ);
    valI2C = i2c_read(n_ACK);    
    i2c_stop();
    
    return(valI2C);
}


/* *****************************************************************************
 End of File
 */
