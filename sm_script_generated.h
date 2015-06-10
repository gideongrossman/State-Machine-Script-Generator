// Copyright (c) 2015 Velocitek Inc. All rights reserved.
    //
    // Please follow the Velocitek C Style Guide when maintaining this code.
    // http://www.velocitek.com/style-guide/c_style_guide.xml

#ifndef SM_SCRIPT_GENERATED_H
#define SM_SCRIPT_GENERATED_H

#include "events.h"

typedef enum
{
STATE_1_STATE,
STATE_2_STATE,
STATE_3_STATE,
}
ScriptGeneratedStates;

Events RunScriptGeneratedSM(Events current_event);
void StartScriptGeneratedSM(void);
ScriptGeneratedStates QueryScriptGeneratedSM(void);

#endif