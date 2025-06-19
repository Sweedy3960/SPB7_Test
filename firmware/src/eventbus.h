// app_eventbus.h
#ifndef APP_EVENTBUS_H
#define APP_EVENTBUS_H
#include "stdio.h"
#include "stdint.h"
typedef enum {
    EVT_TOUCH,
    EVT_DISPLAY,
    EVT_SREG_LED,  
    EVT_INPUTS,       
    EVT_TIMER5_TOUCH_ELAPSED,
} app_event_t;

void App_EventBus_Publish(app_event_t evt, void *data);

#endif
