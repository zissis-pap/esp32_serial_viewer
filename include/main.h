/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  * */

#ifndef __MAIN_H
#define __MAIN_H

/* Variables */
typedef uint16_t SYS_STATUS;	/* Stores the current system error
				 	 	 	 	 * System errors are of greater priority than system state */
typedef uint8_t SYS_STATE;		/* Stores the current system state - What routine should the device run */
/* Functions */


// Error functions
void PrintSystemStatus(void);
uint16_t CheckSystemError(void);
uint8_t CheckErrorID(uint16_t error);
void SetErrorID(uint16_t error);
void ClearErrorID(uint16_t error);

// System setup
void SerialSetUp(void);
void OLEDSetUp(void);
void BluetoothSetUP(void);
void LoRaSetUp(void);
void SDSetup(void);

// Misc
void display_ok(void);
void display_fail(void);

#endif /* __MAIN_H */