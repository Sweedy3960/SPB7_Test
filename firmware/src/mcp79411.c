#include "mcp79411.h"

#include "Mc32_I2cUtilCCS.h"


static mcp79411_obj mcp79411;

/**
 * @brief Convertit un entier décimal en BCD (Binary Coded Decimal).
 *
 * @param dec Valeur décimale à convertir.
 * @return uint8_t Valeur convertie en BCD.
 */
uint8_t mcp79411_dec2bcd(uint8_t dec)
{
    unsigned char l_highHalfByte = 0;
    unsigned char l_lowHalfByte  = 0;
    unsigned char ret = 0x00;

    if(dec > MCP79411_DEC_MAX)
    {
        /*Invalid decimal data*/
        ret = 0x00;
    }
    else
    {
        /*Convert decimal data to dec data*/
        l_highHalfByte = dec / MCP79411_DECADE;
        l_lowHalfByte  = dec % MCP79411_DECADE;

        ret = (unsigned char)(l_highHalfByte << MCP79411_4BIT_SHIFT) | (l_lowHalfByte);
    }

    return ret;
}

/**
 * @brief Convertit une valeur BCD en entier décimal.
 *
 * @param bcd Valeur BCD à convertir.
 * @return uint8_t Valeur convertie en décimal.
 */
uint8_t mcp79411_bcd2dec(uint8_t bcd)
{
    unsigned char l_highHalfByte = 0;
    unsigned char l_lowHalfByte  = 0;
    unsigned char ret = 0x00;

    if(bcd > MCP79411_DEC_MAX)
    {
        /*Invalid bcd data*/
        ret = 0x00;
    }
    else
    {
        /*Convert bcd data to decimal data*/
        l_highHalfByte = bcd >> MCP79411_4BIT_SHIFT;
        l_lowHalfByte  = bcd & MCP79411_HIGH_4BIT_MASK;

        ret = (l_highHalfByte * MCP79411_DECADE) + l_lowHalfByte;
    }

    return ret;    
}

/**
 * @brief Lit un ou plusieurs registres du RTC MCP79411 via I2C.
 *
 * @param reg_addr Adresse du registre à lire.
 * @param rx_buffer Pointeur vers le buffer de réception.
 * @param len Nombre d'octets à lire.
 * @return uint8_t 0 si succès, -1 en cas d'erreur.
 */
uint8_t mcp79411_rtc_reg_read(uint8_t reg_addr, uint8_t* rx_buffer, uint16_t len)
{
    int ret = -1;

    if( (rx_buffer == NULL_PTR) || (len <= 0) || (len >= MCP79411_BUFFER_MAX)){
        ret = -1;
    }else{
        ret = mcp79411_rtc_iic_write(&reg_addr, sizeof(reg_addr));
        if(ret == 0){
            ret = mcp79411_rtc_iic_read(rx_buffer,len);
        }else{
            /*nothing*/
        }
    }

    return ret;    
}

/**
 * @brief Écrit un ou plusieurs registres du RTC MCP79411 via I2C.
 *
 * @param reg_addr Adresse du registre à écrire.
 * @param tx_buffer Pointeur vers le buffer de données à écrire.
 * @param len Nombre d'octets à écrire.
 * @return uint8_t 0 si succès, -1 en cas d'erreur.
 */
uint8_t mcp79411_rtc_reg_write(uint8_t reg_addr, uint8_t* tx_buffer, uint16_t len)
{
	unsigned char* ptx = &mcp79411.buffers.tx_buffer[0];
    int ret = -1;
    int i = 0;

    if( (tx_buffer == NULL_PTR) || (len <= 0) || (len >= MCP79411_BUFFER_MAX)){
        ret = -1;
    }else{
        ptx[0] = reg_addr;
        for(i = 0; i < len; i++){
            ptx[i + MCP79411_REG_ADDR_LEN] = tx_buffer[i];
        }
        ret = mcp79411_rtc_iic_write(ptx, len + MCP79411_REG_ADDR_LEN);
    }

    return ret;    
}

/**
 * @brief Définit l'heure du RTC MCP79411.
 *
 * @param time Pointeur vers la structure contenant l'heure à programmer.
 * @return uint8_t 0 si succès, -1 en cas d'erreur.
 */
uint8_t mcp79411_set_time(mcp79411_time* time)
{
    mcp79411_TIME_KEEPING reg_time;
    int ret = -1;
    unsigned int i = 0;

    //TODO:check arg time
    if(time == NULL_PTR){
        ret = -1;
    }else{
        for( i = 0; i < sizeof(reg_time); i++){
            reg_time.time_bytes[i] = 0x00;
        }

        reg_time.regs.RTCSEC_bits.ST = 1;
        reg_time.regs.RTCSEC_bits.SEC = mcp79411_dec2bcd(time->sec);
        reg_time.regs.RTCMIN_bits.MIN = mcp79411_dec2bcd(time->min);
        reg_time.regs.RTCHOUR_bits.AM_PM = 0;
        reg_time.regs.RTCHOUR_bits.b12_24 = 0;
        reg_time.regs.RTCHOUR_bits.HOUR = mcp79411_dec2bcd(time->hour);
        reg_time.regs.RTCWKDAY_bits.PWRFAIL = 1;
        reg_time.regs.RTCWKDAY_bits.VBATEN = 0;
        reg_time.regs.RTCWKDAY_bits.WKDAY = mcp79411_dec2bcd(time->wkday);
        reg_time.regs.RTCDATE_bits.DATE = mcp79411_dec2bcd(time->date);
        reg_time.regs.RTCMTH_bits.MTH = mcp79411_dec2bcd(time->mth);
        reg_time.regs.RTCYEAR_bits.YEAR = mcp79411_dec2bcd(time->year);
        ret = mcp79411_rtc_reg_write(MCP79411_REG_RTCC_RTCSEC, reg_time.time_bytes, sizeof(reg_time));
    }

    return ret;
}

/**
 * @brief Lit l'heure courante du RTC MCP79411.
 *
 * @param time Pointeur vers la structure où stocker l'heure lue.
 * @return uint8_t 0 si succès, -1 en cas d'erreur.
 */
uint8_t mcp79411_get_time(mcp79411_time* time)
{
    mcp79411_TIME_KEEPING reg_time;
    int ret = -1;

    //TODO:check arg time
    if(time == NULL_PTR){
        ret = -1;
    }else{
        ret = mcp79411_rtc_reg_read(MCP79411_REG_RTCC_RTCSEC, reg_time.time_bytes, sizeof(reg_time));
        if(ret == 0){
            time->sec = mcp79411_bcd2dec(reg_time.regs.RTCSEC_bits.SEC);
            time->min = mcp79411_bcd2dec(reg_time.regs.RTCMIN_bits.MIN);
            time->hour = mcp79411_bcd2dec(reg_time.regs.RTCHOUR_bits.HOUR);
            time->wkday = mcp79411_bcd2dec(reg_time.regs.RTCWKDAY_bits.WKDAY);
            time->date = mcp79411_bcd2dec(reg_time.regs.RTCDATE_bits.DATE);
            time->mth = mcp79411_bcd2dec(reg_time.regs.RTCMTH_bits.MTH);
            time->year = mcp79411_bcd2dec(reg_time.regs.RTCYEAR_bits.YEAR);            
        }else{
            /*nohitng*/
        }
    }

    return ret;
}

/**
 * @brief Récupère le statut de l'oscillateur du RTC MCP79411.
 *
 * @param time Pointeur vers la structure d'heure (non utilisé ici).
 * @return uint8_t Statut de l'oscillateur (0 = arrêt, 1 = en marche).
 */
uint8_t mcp79411_get_status(mcp79411_time* time)
{
    mcp79411_TIME_KEEPING reg_time;
    int ret = -1;
    uint8_t Oscillator_status =0;
    //TODO:check arg time
    if(time == NULL_PTR){
        Oscillator_status =-1;
    }else{
        ret = mcp79411_rtc_reg_read(MCP79411_REG_RTCC_RTCWKDAY, reg_time.time_bytes, sizeof(reg_time));
        if(ret == 0){
        Oscillator_status= mcp79411_bcd2dec(reg_time.regs.RTCWKDAY_bits.OSCRUN);
           
            
        }else{
            
        }
    }
    return Oscillator_status;
}

/**
 * @brief Active l'oscillateur du RTC MCP79411.
 *
 * @param time Pointeur vers la structure d'heure (non utilisé ici).
 * @return uint8_t 0 si succès, 1 en cas d'erreur.
 */
uint8_t mcp79411_set_OscOn(mcp79411_time* time)
{
    mcp79411_CONTROL reg_CONTROL;
    int i = 0;
    int ret =1;

    for(i = 0; i < MCP79411_BUFFER_MAX; i++){
        mcp79411.buffers.tx_buffer[i] = 0;
        mcp79411.buffers.rx_buffer[i] = 0;
    }

    reg_CONTROL.ctrl_byte = 0;
    reg_CONTROL.bits.SQWFS = 0;
    reg_CONTROL.bits.CRSTRIM = 0;
    reg_CONTROL.bits.EXTOSC = 0;
    reg_CONTROL.bits.ALM0EN = 0;
    reg_CONTROL.bits.ALM1EN = 0;
    reg_CONTROL.bits.SQWEN = 1;
    reg_CONTROL.bits.OUT = 1;
    ret =mcp79411_rtc_reg_write(MCP79411_REG_RTCC_CONTROL, &reg_CONTROL.ctrl_byte, sizeof(reg_CONTROL));

    return ret;
}

/*
int mcp79411_set_alarm(mcp79411_alarm_channel chnl, mcp79411_alarm_mode mode,
 mcp79411_alarm *alarm)
{
    int ret = -1;
    unsigned int i = 0;
    mcp79411_ALARMS reg_alarms;
    mcp79411_CONTROL reg_CONTROL = {{0}};
    unsigned char alarm_reg_start_addr = MCP79411_REG_RTCC_ALM0SEC;
    unsigned char reg_addr_WKDAY = MCP79411_REG_RTCC_ALM0WKDAY;

    if( (chnl >= MCP79411_ALRM_CHANNEL_MAX) ||
        (mode >= MCP79411_ALRM_MODE_MAX) ||
        (alarm == NULL_PTR)){
            ret = -1;
    }else{
        if(chnl == MCP79411_ALRM_CHANNEL_1){
            alarm_reg_start_addr = MCP79411_REG_RTCC_ALM1SEC;
            reg_addr_WKDAY = MCP79411_REG_RTCC_ALM1WKDAY;
        }else{
            alarm_reg_start_addr = MCP79411_REG_RTCC_ALM0SEC;
            reg_addr_WKDAY = MCP79411_REG_RTCC_ALM0WKDAY;
        }
        for( i = 0; i < sizeof(reg_alarms); i++){
            reg_alarms.bytes[i] = 0x00;
        }
        //config alarm value
        reg_alarms.regs.ALMXSEC_bits.SEC = mcp79411_dec2bcd(alarm->sec);
        reg_alarms.regs.ALMXMIN_bits.MIN = mcp79411_dec2bcd(alarm->min);
        reg_alarms.regs.ALMXHOUR_bits.AM_PM = 0;
        reg_alarms.regs.ALMXHOUR_bits.b12_24 = 0;
        reg_alarms.regs.ALMXHOUR_bits.HOUR = mcp79411_dec2bcd(alarm->hour);
        reg_alarms.regs.ALMXWKDAY_bits.ALMPOL = 0;
        reg_alarms.regs.ALMXWKDAY_bits.ALMXMSK = (char)mode;
        reg_alarms.regs.ALMXWKDAY_bits.WKDAY = mcp79411_dec2bcd(alarm->wkday);
        reg_alarms.regs.ALMXDATE_bits.DATE = mcp79411_dec2bcd(alarm->date);
        reg_alarms.regs.ALMXMTH_bits.MTH = mcp79411_dec2bcd(alarm->mth);
        ret = mcp79411_rtc_reg_write(alarm_reg_start_addr, reg_alarms.bytes, sizeof(reg_alarms));
        if(ret == 0){
            //ensure ALMxIF flag is cleared
            ret = mcp79411_rtc_reg_write(reg_addr_WKDAY, (unsigned char*)&(reg_alarms.regs.ALMXWKDAY_bits),
             sizeof(reg_alarms.regs.ALMXWKDAY_bits));
            if(ret == 0){
                //enable ALMxEN
                ret = mcp79411_rtc_reg_read(MCP79411_REG_RTCC_CONTROL, &reg_CONTROL.byte, sizeof(reg_CONTROL));
                if(ret == 0){
                    if(chnl == MCP79411_ALRM_CHANNEL_1){
                        reg_CONTROL.bits.ALM1EN = 1;
                    }else{
                        reg_CONTROL.bits.ALM0EN = 1;
                    }
                }
                ret = mcp79411_rtc_reg_write(MCP79411_REG_RTCC_CONTROL, &reg_CONTROL.byte, sizeof(reg_CONTROL));
            }
        }
    }

    return ret;
}

int mcp79411_stop_alarm(mcp79411_alarm_channel chnl)
{
    mcp79411_CONTROL reg_CONTROL = {{0}};
    int ret = -1;

    ret = mcp79411_rtc_reg_read(MCP79411_REG_RTCC_CONTROL, &reg_CONTROL.byte, sizeof(reg_CONTROL));
    if(ret == 0){
        if(chnl == MCP79411_ALRM_CHANNEL_1){
            reg_CONTROL.bits.ALM1EN = 0;
        }else{
            reg_CONTROL.bits.ALM0EN = 0;
        }
        ret = mcp79411_rtc_reg_write(MCP79411_REG_RTCC_CONTROL, &reg_CONTROL.byte, sizeof(reg_CONTROL));
    }

    return ret;
}
*/

/**
 * @brief Initialise le RTC MCP79411 (configuration par défaut).
 *
 * @return void
 */
void mcp79411_init(void)
{
    
    mcp79411_CONTROL reg_CONTROL;
    mcp79411_OSCTRIM reg_OSCTRIM;
    int i = 0;

    for(i = 0; i < MCP79411_BUFFER_MAX; i++){
        mcp79411.buffers.tx_buffer[i] = 0;
        mcp79411.buffers.rx_buffer[i] = 0;
    }

    reg_CONTROL.ctrl_byte = 0;
    reg_CONTROL.bits.SQWFS = 0;
    reg_CONTROL.bits.CRSTRIM = 0;
    reg_CONTROL.bits.EXTOSC = 0;
    reg_CONTROL.bits.ALM0EN = 0;
    reg_CONTROL.bits.ALM1EN = 0;
    reg_CONTROL.bits.SQWEN = 1;
    reg_CONTROL.bits.OUT = 0;
    (void)mcp79411_rtc_reg_write(MCP79411_REG_RTCC_CONTROL, &reg_CONTROL.ctrl_byte, sizeof(reg_CONTROL));

    reg_OSCTRIM.osctrim_byte = 0;
    (void)mcp79411_rtc_reg_write(MCP79411_REG_RTCC_OSCTRIM, &reg_OSCTRIM.osctrim_byte, sizeof(reg_OSCTRIM));
     
}

/**
 * @brief Effectue une écriture I2C vers le RTC MCP79411.
 *
 * @param tx_buffer Pointeur vers le buffer de données à écrire.
 * @param len Nombre d'octets à écrire.
 * @return uint8_t 0 si succès, 1 en cas d'erreur.
 */
uint8_t mcp79411_rtc_iic_write(uint8_t  *tx_buffer, uint16_t len)
{
    int8_t ack = 1;
    static uint8_t i =0;

    i2c_start();
    ack=i2c_write(MCP79411_I2C_ADDR_W);//ADR
    for (i=0;i<len;i++)//DATA
    {
        ack =i2c_write(tx_buffer[i]);
    }
    i2c_stop();
    return ack;
}

/**
 * @brief Effectue une lecture I2C depuis le RTC MCP79411.
 *
 * @param rx_buffer Pointeur vers le buffer de réception.
 * @param len Nombre d'octets à lire.
 * @return uint8_t 0 si succès, 1 en cas d'erreur.
 */
uint8_t mcp79411_rtc_iic_read(uint8_t  *rx_buffer,uint16_t len)
{   static uint8_t i =0;
    int ret = 1;

    i2c_start();
    i2c_write(MCP79411_I2C_ADDR_R);//ADR
    for (i=0;i<(len-1);i++)
    {
        rx_buffer[i]=i2c_read(1);
    }
    rx_buffer[len]=i2c_read(0);
    i2c_stop();
    ret = 0;
    return ret;
}
