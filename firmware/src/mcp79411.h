#ifndef _MCP79411_H
#define _MCP79411_H
#include "stdint.h"
#ifndef NULL_PTR
#define NULL_PTR ((void*)0)

#define MCP79411_BUFFER_MAX     256
#define MCP79411_REG_ADDR_LEN   1

#define MCP79411_DEC_MAX        0x99
#define MCP79411_DECADE         10
#define MCP79411_4BIT_SHIFT     4
#define MCP79411_HIGH_4BIT_MASK    0X0fu
/**reg define**/
#define MCP79411_REG_RTCC_RTCSEC       0x00
#define MCP79411_REG_RTCC_RTCMIN       0x01
#define MCP79411_REG_RTCC_RTCHOUR      0x02
#define MCP79411_REG_RTCC_RTCWKDAY     0x03
#define MCP79411_REG_RTCC_RTCDATE      0x04
#define MCP79411_REG_RTCC_RTCMTH       0x05
#define MCP79411_REG_RTCC_RTCYEAR      0x06
#define MCP79411_REG_RTCC_CONTROL      0x07
#define MCP79411_REG_RTCC_OSCTRIM      0x08
#define MCP79411_REG_RTCC_EEUNLOCK     0x09
#define MCP79411_REG_RTCC_ALM0SEC      0x0A
#define MCP79411_REG_RTCC_ALM0MIN      0x0B
#define MCP79411_REG_RTCC_ALM0HOUR     0x0C
#define MCP79411_REG_RTCC_ALM0WKDAY    0x0D
#define MCP79411_REG_RTCC_ALM0DATE     0x0E
#define MCP79411_REG_RTCC_ALM0MTH      0x0F
#define MCP79411_REG_RTCC_ALM1SEC      0x11
#define MCP79411_REG_RTCC_ALM1MIN      0x12
#define MCP79411_REG_RTCC_ALM1HOUR     0x13
#define MCP79411_REG_RTCC_ALM1WKDAY    0x14
#define MCP79411_REG_RTCC_ALM1DATE     0x15
#define MCP79411_REG_RTCC_ALM1MTH      0x16
#define MCP79411_REG_RTCC_PWRDNMIN     0x18
#define MCP79411_REG_RTCC_PWRDNHOUR    0x19
#define MCP79411_REG_RTCC_PWRDNDATE    0x1A
#define MCP79411_REG_RTCC_PWRDNMTH     0x1B
#define MCP79411_REG_RTCC_PWRUPMIN     0x1C
#define MCP79411_REG_RTCC_PWRUPHOUR    0x1D
#define MCP79411_REG_RTCC_PWRUPDATE    0x1E
#define MCP79411_REG_RTCC_PWRUPMTH     0x1F
#define MCP79411_I2C_ADDR_W  0xDE
#define MCP79411_I2C_ADDR_R  0xDF

typedef union {
    struct{
        char SQWFS   :2;
        char CRSTRIM :1;
        char EXTOSC  :1;
        char ALM0EN  :1;
        char ALM1EN  :1;
        char SQWEN   :1;
        char OUT     :1;
    }bits;
    unsigned char ctrl_byte;
}mcp79411_CONTROL;

typedef union {
    struct{
        char TRIMVA  :7;
        char SIGN    :1;
    }bits;
    unsigned char osctrim_byte;
}mcp79411_OSCTRIM;

typedef union {
    struct {
        struct{
            char SEC :7;
            char ST  :1;
        }RTCSEC_bits;
        struct{
            char MIN   :7;
            char RES  :1;
        }RTCMIN_bits;
        struct{
            char HOUR   :5;
            char AM_PM  :1;
            char b12_24  :1;
            char RES   :1;
        }RTCHOUR_bits;
        struct{
            char WKDAY   :3;
            char VBATEN  :1;
            char PWRFAIL :1;
            char OSCRUN  :1;
            char RES    :2;
        }RTCWKDAY_bits;
        struct{
            char DATE    :6;
            char RES     :2;
        }RTCDATE_bits;
        struct{
            char MTH     :5;
            char LPYR    :1;
            char RES    :2;
        }RTCMTH_bits;
        struct{
            char YEAR :8;
        }RTCYEAR_bits;
    }regs;
    unsigned char time_bytes[7];
}mcp79411_TIME_KEEPING;

typedef union{
    struct{
        struct{
            char SEC     :7;
            char RES    :1;
        }ALMXSEC_bits;
        struct{
            char MIN     :7;
            char RES    :1;
        }ALMXMIN_bits;    
        struct{
            char HOUR    :5;
            char AM_PM   :1;
            char b12_24  :1;
            char RES    :1;
        }ALMXHOUR_bits;
        struct{
            char WKDAY   :3;
            char ALMXIF  :1;
            char ALMXMSK :3;
            char ALMPOL  :1;
        }ALMXWKDAY_bits;
        struct{
            char DATE;
        }ALMXDATE_bits;
        struct{
            char MTH    :5;
            char RES   :3;
        }ALMXMTH_bits;
    }regs;
    unsigned char bytes[6];
}mcp79411_ALARMS;

typedef struct{
    struct{
        unsigned char tx_buffer[MCP79411_BUFFER_MAX];
        unsigned char rx_buffer[MCP79411_BUFFER_MAX];
    }buffers;
}mcp79411_obj;



 uint8_t mcp79411_rtc_reg_read(uint8_t reg_addr, uint8_t* rx_buffer, uint16_t len);
 uint8_t mcp79411_rtc_reg_write(uint8_t reg_addr,uint8_t* tx_buffer, uint16_t len);

typedef struct {
    char sec;
    char min;
    char hour;
    char wkday;
    char date;
    char mth;
    char year;
}mcp79411_time;

typedef struct{
    char sec;
    char min;
    char hour;
    char wkday;
    char date;
    char mth;    
}mcp79411_alarm;

typedef enum{
    MCP79411_ALRM_MODE_SEC = 0x00,
    MCP79411_ALRM_MODE_MIN = 0x01,
    MCP79411_ALRM_MODE_HOUR = 0x02,
    MCP79411_ALRM_MODE_WKDAY = 0x03,
    MCP79411_ALRM_MODE_DATE = 0x04,
    MCP79411_ALRM_MODE_ALL = 0x07,
    MCP79411_ALRM_MODE_MAX
}mcp79411_alarm_mode;

typedef enum{
    MCP79411_ALRM_CHANNEL_0 = 0,  
    MCP79411_ALRM_CHANNEL_1,
    MCP79411_ALRM_CHANNEL_MAX,  
}mcp79411_alarm_channel;

extern void mcp79411_init(void);
 uint8_t mcp79411_set_time(mcp79411_time* time);
 uint8_t mcp79411_get_time(mcp79411_time* time);
 uint8_t mcp79411_set_alarm(mcp79411_alarm_channel chnl, mcp79411_alarm_mode mode,
 mcp79411_alarm *alarm);
 uint8_t mcp79411_stop_alarm(mcp79411_alarm_channel chnl);

//added to use it in app.c
extern uint8_t mcp79411_dec2bcd(uint8_t dec);
extern uint8_t mcp79411_bcd2dec(uint8_t bcd);
uint8_t mcp79411_rtc_iic_read(uint8_t  *rx_buffer,uint16_t len);
uint8_t mcp79411_rtc_iic_write(uint8_t  *tx_buffer, uint16_t len);
uint8_t mcp79411_get_status(mcp79411_time* time);
uint8_t mcp79411_set_OscOn(mcp79411_time* time);
#endif
#endif