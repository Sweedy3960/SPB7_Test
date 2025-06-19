#include "taskctrl.h"

// Define task control instances
//start all active may be an error 
app_task_ctrl_t touchTaskCtrl    = { .isActive = true, .isDirty = false };
app_task_ctrl_t displayTaskCtrl  = { .isActive = true, .isDirty = false };
app_task_ctrl_t ledTaskCtrl  = { .isActive = true, .isDirty = false };
app_task_ctrl_t inputsTaskCtrl    = { .isActive = true, .isDirty = false };
app_task_ctrl_t rtcTackCtrl = {.isActive = true, .isDirty = false};
// Define others as needed
    