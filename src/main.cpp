#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include <SD.h>
#include "main.h"
#include "logo.h"
#include "defines.h"
#include "enumerations.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST); // Initialize SSD1306 display

BluetoothSerial SerialBT; // Initialize bluetooth

SPIClass sd_spi(HSPI);  // Initialize SPI for SD card

SYS_STATUS 		SYSTEM_STATUS = SYSTEM_OK;
SYS_STATE  		SYSTEM_STATE  = SYSTEM_RESET;

String IncominData = "";
int horizontal_chars = floor(SCREEN_WIDTH/6); // each char is about 6 pixels wide
int cursor = 0;
const int LINE = 8;                           // each line is 8 pixels high
const uint8_t end[1] = {10};

void setup(void) 
{
  // Initialize serial
  Serial.begin(115200);
  // Initialize LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize OLED
  OLEDSetUp(); 
  // Initialize LoRa
  LoRaSetUp(); 
  
  // Initialize Bluetooth
  SerialBT.begin("ESP32 Portable UART Reader"); //Bluetooth device name

  // Initialize SD card
  sd_spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, sd_spi))
  {
    Serial.println(F("SD Card: mounting failed"));
    display_fail();
  }
  else
  { 
    Serial.println(F("SD Card: mounted"));
    display_ok();
  }

  display_ok();
  Serial.println(F("SYSTEM OK"));
  digitalWrite(LED_BUILTIN, LOW); // while OLED is running, must set GPIO25 in high
  display.clearDisplay();
  display.setCursor(0, 0);  // Set the cursor to the top-left corner of the display
  display.display();  
}


void loop(void) 
{
  if (Serial.available()) 
  {
    digitalWrite(LED_BUILTIN, HIGH); // while OLED is running, must set GPIO25 in high
    String data = Serial.readStringUntil('\n');
    int lines = 0;
    int data_len = data.length();
    if(data_len % horizontal_chars)
    {
      lines = (data_len / horizontal_chars) + 1;
    }
    else
    {
      lines = data_len / horizontal_chars;
    }
    if(lines == 0) lines = 1;
    
    if (display.getCursorY() >= SCREEN_HEIGHT - lines*LINE + 1) 
    {
      for (int y = 0; y < SCREEN_HEIGHT; y++) 
      {
        for (int x = 0; x < SCREEN_WIDTH; x++) 
        {
          display.drawPixel(x, y, display.getPixel(x, y + lines*LINE));
        }
      }
      display.setCursor(0, SCREEN_HEIGHT - lines*LINE);
    }
    display.setCursor(0, display.getCursorY());
    display.println(data);
    display.display();

    SerialBT.write((uint8_t*)data.c_str(), data_len);
    SerialBT.write(end, 1);

    digitalWrite(LED_BUILTIN, LOW); // while OLED is running, must set GPIO25 in high
  }
}

/* SETUP FUNCTIONS */
void OLEDSetUp(void)
{
  Wire.begin(OLED_SDA, OLED_SCL); // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS, false, false)) // Address 0x3C for 128x32
  { 
    Serial.println(F("SSD1306 allocation failed"));
    display_fail();
  }
  else
  {
    display.clearDisplay(); // Clear display
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Set the cursor to the top-left corner of the display
    display_ok(); // print OK message
  }
}

void LoRaSetUp(void)
{
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS); // Initialize SPI for LoRa tranceiver
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(LORA_BAND)) 
  {
    Serial.println(F("Starting LoRa failed!"));
    display_fail();
  }
  else
  {
    display_ok();
  }
}

/* MISC FUNCTIONS */
void display_ok(void)
{
  display.println("OK");
  display.display();
  Serial.println(F("OK"));
}

void display_fail(void)
{
  display.println("ERROR");
  display.display();
  Serial.println(F("ERROR"));
}
