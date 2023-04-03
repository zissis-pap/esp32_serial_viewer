/**
  ******************************************************************************
  * @file           : enumerations.h
  * @brief          : This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  * */

#ifndef __ENUMERATIONS_H
#define __ENUMERATIONS_H

/* ERROR CODES enumerations */
#define SYSTEM_OK           0UL
#define HIGH_TEMPERATURE    1UL << 1
#define OLED_ERROR          1UL << 2
#define SD_CARD_ERROR       1UL << 3
#define NO_SD_CARD          1UL << 4
#define LORA_ERROR          1UL << 5
#define BLUETOOTH_ERROR     1UL << 6
#define NO_BLUETOOTH_CONN   1UL << 7

/* SYSTEM STATE enumerations */
enum {SYSTEM_RESET = 0, AWAIT_BLE_CLIENT = 1, BLE_CLIENT_CONNECTED = 2, COMMAND = 3};
/* COMMANDS enumerations */
enum {READ_UART = 0, LIST_SD = 1, OPEN_FILE = 2, SET_BAUD_RATE = 3, SAVE_TO_SD = 4};
/* MISC */
enum {FALSE = 0, TRUE = 1};


#endif
