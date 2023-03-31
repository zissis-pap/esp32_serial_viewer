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

// Main flow functions
void ErrorHandler(void);
void StateExecution(void);
void SystemPowerUp(void);
void WaitBLEClientConnection(void);
char* BLEClientConnected(void);
void ReadUART(void);

// Bluetooth functions
char* BluetoothReceive(void);

// Command functions
char* CheckForCommand(void);
void ExecuteCommand(char* command);

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

// OLED functions
void OLEDDisplayStatus(String data);

// SD functions
void SDListDir(void);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

// Misc
void display_ok(void);
void display_fail(void);

#endif /* __MAIN_H */