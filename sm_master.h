// Copyright (c) 2015 Velocitek Inc. All rights reserved.
//
// Please follow the Velocitek C Style Guide when maintaining this code.
// http://www.velocitek.com/style-guide/c_style_guide.xml

#ifndef SMMASTER_H
#define SMMASTER_H

typedef enum
{
STATE_1_STATE,
STATE_2_STATE,
STATE_3_STATE,
}
MasterStates;

uint8_t RunMasterSM(uint8_t current_event);
void StartMasterSM(void);
MasterStates QueryMasterSM(void);

#endif