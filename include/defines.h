/**
  ******************************************************************************
  * @file           : defines.h
  * @brief          : This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  * */

#ifndef __DEFINES_H
#define __DEFINES_H

/* LoRa Parameters */
#define LORA_BAND    868E6

/* OLED Parameters */
#define OLED_ADDRESS  0x3C
#define SCREEN_WIDTH  128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define LINE_HEIGHT   8   // each line is 8 pixels high

/* SD Card Parameters */
#define SD_CS 13
#define SD_SCK 14
#define SD_MOSI 15
#define SD_MISO 2

#endif