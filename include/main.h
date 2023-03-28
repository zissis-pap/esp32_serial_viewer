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

// System setup
void OLEDSetUp(void);
void LoRaSetUp(void);

// Misc
void display_ok(void);
void display_fail(void);

#endif /* __MAIN_H */