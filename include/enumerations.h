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
enum {SYSTEM_RESET = 0};
enum {FALSE = 0, TRUE = 1};

#endif
