// Copyright (c) 2015 Velocitek Inc. All rights reserved.
// Author(s): Gideon Grossman, Alec Stewart
//
// Hierarchical State Machine Template by Ed Carryer, Stanford University
//
// Please follow the Velocitek C Style Guide when maintaining this code.
// http://www.velocitek.com/style-guide/c_style_guide.xml

// Include Files ---------------------------------------------------------------

#include "main.h"
#include "avr_compiler.h"
#include "shiftLcd.h"
#include "SMevents.h"

#include "SMmaster.h"

// Module Constants ------------------------------------------------------------


// Module Functions ------------------------------------------------------------
static void DuringState1(uint8_t event);
static void DuringState2(uint8_t event);
static void DuringState3(uint8_t event);

// Module Variables------------------------------------------------------------
static MasterStates current_master_state_ = STATE_1_STATE;

// Module Code -----------------------------------------------------------------
uint8_t RunMasterSM(uint8_t current_event)
{
  MasterStates next_state = current_master_state_;

  switch (current_master_state_)
  {
    case STATE_1_STATE :
      DuringState1(current_event);

      if (current_event != EV_NO_EVENT)
      {
        switch (current_event)
        {
          case EV_BUTTON_PWR_RELEASED :
            next_state = STATE_2_STATE;
            break;

          case EV_BUTTON_PLUS_ONE_RELEASED :
            next_state = STATE_3_STATE;
            break;

          default:
            break;
        }
      }

      break;

    case STATE_2_STATE :
      DuringState2(current_event);

      if (current_event != EV_NO_EVENT)
      {
        switch (current_event)
        {
          case EV_BUTTON_RESET_PRESSED :
            next_state = STATE_1_STATE;
            break;

          case EV_BUTTON_PLUS_ONE_PRESSED :
            next_state = STATE_3_STATE;
            break;

          default:
            break;
        }
      }

      break;

    case STATE_3_STATE :
      DuringState3(current_event);

      if (current_event != EV_NO_EVENT)
      {
        switch (current_event)
        {
          case EV_BUTTON_RESET_PRESSED :
            next_state = STATE_1_STATE;
            break;

          case EV_BUTTON_PWR_RELEASED :
            next_state = STATE_2_STATE;
            break;

          default:
            break;
        }
      }

      break;

  }

  if (next_state != current_master_state_)
  {
    switch (current_master_state_)
    {
      case STATE_1_STATE:
        DuringState1(EV_EXIT);
        break;

      case STATE_2_STATE:
        DuringState2(EV_EXIT);
        break;

      case STATE_3_STATE:
        DuringState3(EV_EXIT);
        break;

      default:
        break;
    }

    current_master_state_ = next_state;

    switch (current_master_state_)
    {
      case STATE_1_STATE:
        DuringState1(EV_ENTRY);
        break;

      case STATE_2_STATE:
        DuringState2(EV_ENTRY);
        break;

      case STATE_3_STATE:
        DuringState3(EV_ENTRY);
        break;

      default:
        break;
    }
  }

  return (current_event);
}

void StartMasterSM(void)
{
  current_master_state_ = STATE_1_STATE;
  RunMasterSM(EV_ENTRY);
}

MasterStates QueryMasterSM(void)
{
  return current_master_state_;
}


// Private During Functions ----------------------------------------------------
static void DuringState1(uint8_t event)
{

  if (event == EV_ENTRY)
  {
		ClearLcdAll();
    SetDampingNumber(1);
    UpdateLcd();
  }
  else if (event == EV_EXIT)
  {

  }
  else
  {

  }
}

static void DuringState2(uint8_t event)
{

  if (event == EV_ENTRY)
  {
		ClearLcdAll();
    SetDampingNumber(2);
    UpdateLcd();
  }
  else if (event == EV_EXIT)
  {

  }
  else
  {

  }
}

static void DuringState3(uint8_t event)
{

  if (event == EV_ENTRY)
  {
		ClearLcdAll();
    SetDampingNumber(3);
    UpdateLcd();
  }
  else if (event == EV_EXIT)
  {

  }
  else
  {

  }
}

