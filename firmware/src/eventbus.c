//app_eventbus.c
#include "eventbus.h"
#include "appDisp.h"
#include "appIn.h"
#include "appTouch.h"
#include "appReg.h"
void App_EventBus_Publish(app_event_t evt, void *data)
{
    switch (evt) {
        case EVT_TOUCH:
            
            App_Display_HandleTouch(data);    //gestion touche du diplay
            App_LED_HandleTouch(data);         // gestion touche des  led 
            //pour test utilier pour commander les leds mais à l'avenir
            //Ce sont les EVT_INPUTS sauf si quittancage dans  menu or smth
            break;

        case EVT_INPUTS:
            
            App_Display_HandleInputs(data); //this need to be done  
            //App_LED_HandleInputs(data);//this need to be done      
            
            break;
        
        default:
            break;
    }
}
    