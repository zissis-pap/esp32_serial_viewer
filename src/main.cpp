#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"
#include "logo.h"
#include "defines.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

BluetoothSerial SerialBT;

String IncominData = "";
int cursor = 0;
const int LINE = 8;
const uint8_t end[1] = {10};

void setup(void) 
{
  // Initialize serial
  Serial.begin(115200);

  // Initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) 
  { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    while(1); // Don't proceed, loop forever
  }
  display.display();     // the library initializes this with an Adafruit splash screen.
  Serial.println(F("OK"));

  // Initialize LoRa
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(LORA_BAND)) 
  {
    Serial.println(F("Starting LoRa failed!"));
    while (1);
  }
  Serial.println(F("OK"));

  // Initialize Bluetooth
  SerialBT.begin("ESP32 Portable UART Reader"); //Bluetooth device name

  // Initialize LED
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(F("OK"));
  Serial.printf("SYSTEM OK\n");
  digitalWrite(LED_BUILTIN, LOW); // while OLED is running, must set GPIO25 in high

  // Clear display
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);  // Set the cursor to the top-left corner of the display
}


void loop(void) 
{
  if (Serial.available()) 
  {
    digitalWrite(LED_BUILTIN, HIGH); // while OLED is running, must set GPIO25 in high
    String data = Serial.readStringUntil('\n');
    int lines = 0;
    int data_len = data.length();
    if(data_len % 21)
    {
      lines = (data_len / 21) + 1;
    }
    else
    {
      lines = data_len / 21;
    }
    if(lines == 0) lines = 1;
    
    if (display.getCursorY() >= SCREEN_HEIGHT - lines*LINE) 
    {
      for (int y = 0; y < 64; y++) 
      {
        for (int x = 0; x < 128; x++) 
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
