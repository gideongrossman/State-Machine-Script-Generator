/****************************************************************************
   SMmaster.c
****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
#include "main.h"
#include "avr_compiler.h"
#include "batteryMonitor.h"
#include "blinkGenerator.h"
#include "countdownTimer.h"
#include "coreEeprom.h"
#include "damping_adjustment.h"
#include "externalInterrupts.h"
#include "flag_manager.h"
#include "headingCalculations.h"
#include "leastSquaresDriver.h"
#include "mahony.h"
#include "mpu6050.h"
#include "openLog.h"
#include "ports.h"
#include "shiftLcd.h"
#include "coreSleep.h"
#include "usart.h"
#include "vector.h"

#include "SMautoshutoff.h"
#include "SMbattery.h"
#include "SMcalibration.h"
#include "SMcompassoffsetonlycalibration.h"
#include "SMevents.h"
#include "SMfunctionaltest.h"
#include "SMheading.h"
#include "SMmahony.h"
#include "SMmaster.h"
#include "SMmodes.h"
#include "SMprocessmpu6050.h"

/*----------------------------- Module Defines ----------------------------*/
#define NORMAL_OPERATION_STATE 1
#define DAMPING_ADJUSTMENT_STATE 2
#define DEAD_MAN_WALKING_STATE 3
#define SLEEP_STATE 4
#define CALIBRATION_STATE 5
#define LOW_BATTERY_STATE 6
#define FUNCTIONAL_TEST_STATE 7
#define USER_CALIBRATION_STATE 8
#define VALIDATION_STATE 9
#define SHIP_STATE 10

#define FIRMWARE_VERSION_NUMBER 100 //1.0.0
/*---------------------------- Module Functions ---------------------------*/
static void DuringNormalOperation(uint8_t currentEvent);
static void DuringDampingAdjustment(uint8_t currentEvent);
static void DuringDeadManWalking(uint8_t currentEvent);
static void DuringSleep(uint8_t currentEvent);
static void DuringCalibration(uint8_t currentEvent);
static void DuringLowBattery(uint8_t currentEvent);
static void DuringFunctionalTest(uint8_t currentEvent);
static void DuringUserCalibration(uint8_t currentEvent);
static void DuringValidation(uint8_t currentEvent);
static void DuringShip(uint8_t currentEvent);

static void InitializeUartZeroAndOpenLogInReleaseMode(void);
static void DisableUartZeroAndOpenLogInReleaseMode(void);
static void PrintAllEepromValuesToOpenLogAndUartZero(void);
static void PrintAllEepromValuesToOpenLog(void);
static void PrintAllEepromValuesToUartZero(void);
static void PrintVector(float *v);
static void PrintMatrix(float *m);
/*---------------------------- Module Variables ---------------------------*/
static uint8_t currentMasterState = NORMAL_OPERATION_STATE;
/*------------------------------ Module Code ------------------------------*/
uint8_t RunMasterSM(uint8_t currentEvent)
{
  uint8_t makeTransition = false;
  uint8_t nextState = NORMAL_OPERATION_STATE;

	nextState = currentMasterState;

	switch (currentMasterState)
	{
		case NORMAL_OPERATION_STATE :
			DuringNormalOperation(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_BUTTON_PWR_RELEASED :
						nextState = DAMPING_ADJUSTMENT_STATE;
						makeTransition = true;
						break;
          
					case EV_BUTTON_PWR_HELD :
						nextState = DEAD_MAN_WALKING_STATE;
						makeTransition = true;
						break;

					case EV_SLEEP :
						nextState = SLEEP_STATE;
						makeTransition = true;
						break;
						
					case EV_BUTTON_PLUS_ONE_AND_RESET_HELD :
						nextState = CALIBRATION_STATE;
						makeTransition = true;
						break;
						
					case EV_BATTERY_EMPTY :
						nextState = LOW_BATTERY_STATE;
						makeTransition = true;
						break;
						
					case EV_BUTTON_PLUS_ONE_AND_MODE_HELD :
						nextState = FUNCTIONAL_TEST_STATE;
						makeTransition = true;
						break;
						
					case EV_BUTTON_MODE_AND_RESET_HELD :
						nextState = USER_CALIBRATION_STATE;
						makeTransition = true;
						break;
					
					case EV_BUTTON_PLUS_ONE_AND_GUN_HELD :
						nextState = VALIDATION_STATE;
						makeTransition = true;
						break;

					case EV_BUTTON_PLUS_ONE_AND_PWR_HELD :
						nextState = SHIP_STATE;
						makeTransition = true;
						break;
				}
			}
			break;

    case DAMPING_ADJUSTMENT_STATE:
			DuringDampingAdjustment(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_BUTTON_PWR_RELEASED :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;
				}
			}
			break;

    case DEAD_MAN_WALKING_STATE:
    DuringDeadManWalking(currentEvent);
    if (currentEvent != EV_NO_EVENT)
    {
      switch (currentEvent)
      {
        case EV_BUTTON_PWR_RELEASED :
        nextState = SLEEP_STATE;
        makeTransition = true;
        break;
      }
    }
    break;
      
		case SLEEP_STATE :
			DuringSleep(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_WAKEUP :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;
				}
			}
			break;
			
		case CALIBRATION_STATE :
			DuringCalibration(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_CALIBRATION_COMPLETE :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;

					case EV_BUTTON_RESET_PRESSED :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;
				}
			}
			break;

		case LOW_BATTERY_STATE :
			DuringLowBattery(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_BUTTON_PWR_RELEASED :
						nextState = SLEEP_STATE;
						makeTransition = true;
						break;

					case EV_SLEEP :
						nextState = SLEEP_STATE;
						makeTransition = true;
						break;
					
					case EV_BATTERY_ALMOST_EMPTY :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;

					case EV_BATTERY_HALF :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;

					case EV_BATTERY_FULL :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;
				}
			}
			break;

		case FUNCTIONAL_TEST_STATE :
			DuringFunctionalTest(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_BUTTON_RESET_PRESSED :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;
				}
			}
			break;

		case USER_CALIBRATION_STATE :
			DuringUserCalibration(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_BUTTON_RESET_PRESSED :
						nextState = NORMAL_OPERATION_STATE;
						makeTransition = true;
						break;
				}
			}
			break;
			
		case VALIDATION_STATE :
			DuringValidation(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_BUTTON_RESET_PRESSED :
					nextState = NORMAL_OPERATION_STATE;
					makeTransition = true;
					break;
				}
			}
			break;
			
		case SHIP_STATE :
			DuringShip(currentEvent);
			if (currentEvent != EV_NO_EVENT)
			{
				switch (currentEvent)
				{
					case EV_BUTTON_PWR_RELEASED :
						nextState = SLEEP_STATE;
						makeTransition = true;
						break;
				}
			}
			break;
	}

	if (makeTransition == true)
	{
		switch (currentMasterState)
		{
			case NORMAL_OPERATION_STATE :
				DuringNormalOperation(EV_EXIT);
				break;
		  case DAMPING_ADJUSTMENT_STATE:
			  DuringDampingAdjustment(EV_EXIT);
			break;
      case DEAD_MAN_WALKING_STATE :
        DuringDeadManWalking(EV_EXIT);
      break;
			case SLEEP_STATE :
				DuringSleep(EV_EXIT);
				break;
			case CALIBRATION_STATE :
				DuringCalibration(EV_EXIT);
				break;
			case LOW_BATTERY_STATE :
				DuringLowBattery(EV_EXIT);
				break;
			case FUNCTIONAL_TEST_STATE :
				DuringFunctionalTest(EV_EXIT);
				break;
			case USER_CALIBRATION_STATE :
				DuringUserCalibration(EV_EXIT);
				break;
			case VALIDATION_STATE :
				DuringValidation(EV_EXIT);
				break;
			case SHIP_STATE :
				DuringShip(EV_EXIT);
				break;
		}

		currentMasterState = nextState;

		switch (currentMasterState)
		{
			case NORMAL_OPERATION_STATE :
				DuringNormalOperation(EV_ENTRY);
				break;
			case DAMPING_ADJUSTMENT_STATE:
				DuringDampingAdjustment(EV_ENTRY);
			break;
      case DEAD_MAN_WALKING_STATE :
        DuringDeadManWalking(EV_ENTRY);
      break;
			case SLEEP_STATE :
				DuringSleep(EV_ENTRY);
				break;
			case CALIBRATION_STATE :
				DuringCalibration(EV_ENTRY);
				break;
			case LOW_BATTERY_STATE :
				DuringLowBattery(EV_ENTRY);
				break;
			case FUNCTIONAL_TEST_STATE :
				DuringFunctionalTest(EV_ENTRY);
				break;
			case USER_CALIBRATION_STATE :
				DuringUserCalibration(EV_ENTRY);
				break;
			case VALIDATION_STATE :
				DuringValidation(EV_ENTRY);
				break;
			case SHIP_STATE :
				DuringShip(EV_ENTRY);
				break;
		}
	}
	return(currentEvent);
}

void StartMasterSM(void)
{
	currentMasterState = NORMAL_OPERATION_STATE;
	RunMasterSM(EV_ENTRY);
}

uint8_t QueryMasterSM(void)
{
	return currentMasterState;
}

/***************************************************************************
 private During functions
 ***************************************************************************/
static void DuringNormalOperation(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		ClearLcdAll();
		InitializeCountdownTimer();
		
		StartMahonySM();
		StartModesSM();
		StartBatterySM();
		StartAutoShutoffSM(SHUTOFF_CONDITION_STABLE_HEADING);
	}
	else if (currentEvent == EV_EXIT)
	{
		RunMahonySM(currentEvent);
		RunModesSM(currentEvent);
		RunBatterySM(currentEvent);
		RunAutoShutoffSM(SHUTOFF_CONDITION_STABLE_HEADING,currentEvent);
		
		DisableCountdownTimer();
	}
	else
	{
		RunMahonySM(currentEvent);
		RunModesSM(currentEvent);
		RunBatterySM(currentEvent);
		RunAutoShutoffSM(SHUTOFF_CONDITION_STABLE_HEADING,currentEvent);
	}
}

#define DAMPING_CHANGE_THRESHOLD 10.0
static void DuringDampingAdjustment(uint8_t currentEvent)
{
	static float old_damping_adjust_angle;
  float new_damping_adjust_angle;

  if (currentEvent == EV_ENTRY)
	{
		ClearLcdAll();
    UpdateDampingDisplay();

    old_damping_adjust_angle = GetDampingAdjustAngle();
	}
	else if (currentEvent == EV_EXIT)
	{
		RunMahonySM(currentEvent);
	}
	else
	{
		RunMahonySM(currentEvent);

    if(currentEvent == EV_UPDATE_DISPLAY)
    {
      
      if (IsQuaternionReady())
      {
        new_damping_adjust_angle = GetDampingAdjustAngle();

        if (new_damping_adjust_angle >= old_damping_adjust_angle + DAMPING_CHANGE_THRESHOLD)
        {
          IncrementDampingLevel();
          old_damping_adjust_angle = new_damping_adjust_angle;
        }      
        else if(new_damping_adjust_angle <= old_damping_adjust_angle - DAMPING_CHANGE_THRESHOLD)
        {
          DecrementDampingLevel();
          old_damping_adjust_angle = new_damping_adjust_angle;
        }
        
        // SetHeadingOnBottom(new_damping_adjust_angle);
        UpdateLcd();
        
      }


    }
	}
}

static void DuringDeadManWalking(uint8_t currentEvent)
{
  if (currentEvent == EV_ENTRY)
  {
    ClearLcdAll();
    UpdateLcd();
  }
  else if (currentEvent == EV_EXIT)
  {
    
  }
  else
  {
   
  }
}

static void DuringSleep(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		SleepMpu6050();
		DisableLcdDriver();
		DisableBlinkGenerator();  //Stop Timer2 after Timer2 interrupt registers are set, otherwise the register changes are ignored
		DisableAccessoryPower();

		EnableExternalInterruptTwo();
		EnterDeepSleep();
		DisableExternalInterruptTwo();

		EnableAccessoryPower();
		InitializeBlinkGenerator();
		InitializeLcdDriver();
	}
	else if (currentEvent == EV_EXIT)
	{
	}
	else
	{
	}
}

static void DuringCalibration(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		InitializeUartZeroAndOpenLogInReleaseMode();

		StartProcessMpu6050SM();
		StartCalibrationSM();
	}
	else if (currentEvent == EV_EXIT)
	{
		RunProcessMpu6050SM(currentEvent);
		RunCalibrationSM(currentEvent);
		
		ClearLeastSquares();  //Frees any allocated memory used during calibration
		PrintAllEepromValuesToOpenLogAndUartZero();
		DisableUartZeroAndOpenLogInReleaseMode();
	}
	else
	{
		RunProcessMpu6050SM(currentEvent);
		RunCalibrationSM(currentEvent);
	}
}

static void DuringLowBattery(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		ClearLcdAll();
		UpdateLcd();

		SleepMpu6050();
		StartBatterySM();
		StartAutoShutoffSM(SHUTOFF_CONDITION_TIME);
	}
	else if (currentEvent == EV_EXIT)
	{
		RunBatterySM(currentEvent);
		RunAutoShutoffSM(SHUTOFF_CONDITION_TIME,currentEvent);
	}
	else
	{
		RunBatterySM(currentEvent);
		RunAutoShutoffSM(SHUTOFF_CONDITION_TIME,currentEvent);
	}
}

static void DuringFunctionalTest(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		ClearLcdAll();
		UpdateLcd();
		SleepMpu6050();
		
		StartFunctionalTestSM();
	}
	else if (currentEvent == EV_EXIT)
	{
		RunFunctionalTestSM(currentEvent);
	}
	else
	{
		RunFunctionalTestSM(currentEvent);
	}	
}

static void DuringUserCalibration(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		InitializeUartZeroAndOpenLogInReleaseMode();
		
		StartProcessMpu6050SM();
		StartCompassOffsetOnlyCalibrationSM();
	}
	else if (currentEvent == EV_EXIT)
	{
		RunProcessMpu6050SM(currentEvent);
		RunCompassOffsetOnlyCalibrationSM(currentEvent);
		
		ClearLeastSquares();  //Frees any allocated memory used during calibration
		PrintAllEepromValuesToOpenLogAndUartZero();
		DisableUartZeroAndOpenLogInReleaseMode();
	}
	else
	{
		RunProcessMpu6050SM(currentEvent);
		RunCompassOffsetOnlyCalibrationSM(currentEvent);
	}
}

static void DuringValidation(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		ClearLcdAll();
		UpdateLcd();
		SetHysteresisBandwidth(VALIDATION_HYSTERESIS_BANDWIDTH);
		
		StartMahonySM();
		StartHeadingSM(HEADING_DISPLAY_VALIDATION);
	}
	else if (currentEvent == EV_EXIT)
	{
		RunMahonySM(currentEvent);
		RunHeadingSM(HEADING_DISPLAY_VALIDATION,currentEvent);
		
		SetHysteresisBandwidth(DEFAULT_HYSTERESIS_BANDWIDTH);
	}
	else
	{
		RunMahonySM(currentEvent);
		RunHeadingSM(HEADING_DISPLAY_VALIDATION,currentEvent);
	}
}

static void DuringShip(uint8_t currentEvent)
{
	if (currentEvent == EV_ENTRY)
	{
		ClearLcdAll();
		SetMessage(LCD_MESSAGE_SHP);
		SetFirmwareVersionNumber(FIRMWARE_VERSION_NUMBER);
		UpdateLcd();

		WriteDoUseLocalOffsetToEeprom(false);
		SetHysteresisBandwidth(DEFAULT_HYSTERESIS_BANDWIDTH);
		SetCountdownMinuteReset(DEFAULT_COUNTDOWN_RESET_MINUTES);
	}
	else if (currentEvent == EV_EXIT)
	{
	}
	else
	{
	}
}

/***************************************************************************
 private functions
 ***************************************************************************/
static void InitializeUartZeroAndOpenLogInReleaseMode(void)
{
	#ifndef DEBUG
	InitializeUartZero();
	InitializeUartOne();
	SetStdoutToUartZeroStream();
	InitializeOpenLog();
	#endif
}

static void DisableUartZeroAndOpenLogInReleaseMode(void)
{
	#ifndef DEBUG
	DisableUartZero();
	DisableUartOne();
	#endif	
}

static void PrintAllEepromValuesToOpenLogAndUartZero(void)
{
	PrintAllEepromValuesToOpenLog();
	PrintAllEepromValuesToUartZero();
}

static void PrintAllEepromValuesToOpenLog(void)
{
	float vectorBuffer[NUM_ELEMENTS_IN_SO3_VECTOR];
	float matrixBuffer[NUM_ELEMENTS_IN_SO3_SQUARE_MATRIX];
	
	ReadAccelBiasFromEeprom(vectorBuffer);
	DataBufferWriteSensorData(vectorBuffer);
	ReadAccelTransformationMatrixFromEeprom(matrixBuffer);
	DataBufferWriteMatrixData(matrixBuffer);
	
	ReadGyroBiasFromEeprom(vectorBuffer);
	DataBufferWriteSensorData(vectorBuffer);
	ReadGyroTransformationMatrixFromEeprom(matrixBuffer);
	DataBufferWriteMatrixData(matrixBuffer);

	ReadCompassOffsetsFromEeprom(vectorBuffer);
	DataBufferWriteSensorData(vectorBuffer);
	ReadCompassTransformationMatrixFromEeprom(matrixBuffer);
	DataBufferWriteMatrixData(matrixBuffer);
	ReadCompassAlignmentMatrixFromEeprom(matrixBuffer);
	DataBufferWriteMatrixData(matrixBuffer);

	ReadLocalCompassOffsetsFromEeprom(vectorBuffer);
	DataBufferWriteSensorData(vectorBuffer);
	
	SendAvailableDataAndClearDataBuffer();
}

static void PrintAllEepromValuesToUartZero(void)
{
	float vectorBuffer[NUM_ELEMENTS_IN_SO3_VECTOR];
	float matrixBuffer[NUM_ELEMENTS_IN_SO3_SQUARE_MATRIX];
	
	ReadAccelBiasFromEeprom(vectorBuffer);
	PrintVector(vectorBuffer);
	ReadAccelTransformationMatrixFromEeprom(matrixBuffer);
	PrintMatrix(matrixBuffer);
	
	ReadGyroBiasFromEeprom(vectorBuffer);
	PrintVector(vectorBuffer);
	ReadGyroTransformationMatrixFromEeprom(matrixBuffer);
	PrintMatrix(matrixBuffer);
	
	ReadCompassOffsetsFromEeprom(vectorBuffer);
	PrintVector(vectorBuffer);
	ReadCompassTransformationMatrixFromEeprom(matrixBuffer);
	PrintMatrix(matrixBuffer);
	ReadCompassAlignmentMatrixFromEeprom(matrixBuffer);
	PrintMatrix(matrixBuffer);

	ReadLocalCompassOffsetsFromEeprom(vectorBuffer);
	PrintVector(vectorBuffer);
	
	printf("\n\r");	
}

static void PrintVector(float *v)
{
	printf("%3.7f, %3.7f, %3.7f\n\r",(double)v[0],(double)v[1],(double)v[2]);
}

static void PrintMatrix(float *m)
{
	printf("%3.7f, %3.7f, %3.7f\n\r",(double)m[0],(double)m[1],(double)m[2]);
	printf("%3.7f, %3.7f, %3.7f\n\r",(double)m[3],(double)m[4],(double)m[5]);
	printf("%3.7f, %3.7f, %3.7f\n\r",(double)m[6],(double)m[7],(double)m[8]);
}