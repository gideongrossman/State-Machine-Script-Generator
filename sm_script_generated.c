// Copyright (c) 2015 Velocitek Inc. All rights reserved.
// Author(s): Gideon Grossman, Alec Stewart
//
// Hierarchical State Machine Template by Ed Carryer, Stanford University
//
// Please follow the Velocitek C Style Guide when maintaining this code.
// http://www.velocitek.com/style-guide/c_style_guide.xml

// Include Files ---------------------------------------------------------------

#include "events.h"
#include "sm_script_generated_state.h"

// Module Constants ------------------------------------------------------------


// Module Functions ------------------------------------------------------------
static void DuringState1(Events event);
static void DuringState2(Events event);
static void DuringState3(Events event);

// Module Variables------------------------------------------------------------
static ScriptGeneratedStates current_script_generated_state_ = STATE_1_STATE;

// Module Code -----------------------------------------------------------------
Events RunScriptGeneratedSM(Events current_event)
{
  ScriptGeneratedStates next_state = current_script_generated_state_;

  switch (current_script_generated_state_)
  {
    case STATE_1_STATE:
      DuringState1(current_event);

      if (current_event != EV_NO_EVENT)
      {
        switch (current_event)
        {
          case EV_CHANGE_TO_STATE_2 :
            nextState = STATE_2;
            break;

          case EV_CHANGE_TO_STATE_3 :
            nextState = STATE_3;
            break;

          default:
            break;
        }
      }

      break;

    case STATE_2_STATE:
      DuringState2(current_event);

      if (current_event != EV_NO_EVENT)
      {
        switch (current_event)
        {
          case EV_CHANGE_TO_STATE_3 :
            nextState = STATE_3;
            break;

          default:
            break;
        }
      }

      break;

    case STATE_3_STATE:
      DuringState3(current_event);

      if (current_event != EV_NO_EVENT)
      {
        switch (current_event)
        {
        }
          default:
            break;
        }
      }

      break;

  }

  if (current_script_generated_state_ != next_state)
  {
    switch (current_script_generated_state_)
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

    current_script_generated_state_ = next_state;

    switch (current_script_generated_state_)
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

void StartScriptGeneratedSM(void)
{
  current_script_generated_state_ = STATE_1_STATE;
  RunScriptGeneratedSM(EV_ENTRY);
}

ScriptGeneratedStates QueryScriptGeneratedSM(void)
{
  return current_script_generated_state_;
}


// Private During Functions ----------------------------------------------------
static void DuringState1(Events event)
{

  if (event == EV_ENTRY)
  {

  }
  else if (event == EV_EXIT)
  {

  }
  else
  {

  }
}

static void DuringState2(Events event)
{

  if (event == EV_ENTRY)
  {

  }
  else if (event == EV_EXIT)
  {

  }
  else
  {

  }
}

static void DuringState3(Events event)
{

  if (event == EV_ENTRY)
  {

  }
  else if (event == EV_EXIT)
  {

  }
  else
  {

  }
}

