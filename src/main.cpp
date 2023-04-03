#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BluetoothSerial.h>
#include <SD.h>
#include <FS.h>
#include "main.h"
#include "defines.h"
#include "enumerations.h"
#include "init_variables.h"
#include "dialogs.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST); // Initialize SSD1306 display
BluetoothSerial SerialBT;                                               // Initialize bluetooth
SPIClass sd_spi(HSPI);                                                  // Initialize SPI for SD card

SYS_STATUS 		SYSTEM_STATUS = SYSTEM_OK;
SYS_STATE  		SYSTEM_STATE  = SYSTEM_RESET;

void setup(void) 
{
  // Initialize LED
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize serial
  SerialSetUp();
  // Initialize OLED
  OLEDSetUp(); 
  // Initialize LoRa
  LoRaSetUp(); 
  // Initialize SD Card
  SDSetup();
   // Initialize Bluetooth
  BluetoothSetUP();

  PrintSystemStatus();
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW); // while OLED is running, must set GPIO25 in high
  display.clearDisplay();
  display.setCursor(0, 0);  // Set the cursor to the top-left corner of the display
  display.display();  
}

void loop(void) 
{
  while(1)
  {
    StateExecution();
  }
}

/* MAIN FLOW FUNCTIONS */
void ErrorHandler(void)
{

}

void StateExecution(void)
{
  static char* sys_control;
  switch(SYSTEM_STATE)
  {
    case SYSTEM_RESET:
      SystemPowerUp();
      break;
    case AWAIT_BLE_CLIENT:
      WaitBLEClientConnection();
      break;
    case BLE_CLIENT_CONNECTED:
      sys_control = BLEClientConnected();
      break;
    case COMMAND:
      ExecuteCommand(sys_control);
      break;
    default:
      break;
  }
}

void SystemPowerUp(void)
{
  OLEDDisplayStatus("SYSTEM BOOT");
  SYSTEM_STATE = AWAIT_BLE_CLIENT;
}

void WaitBLEClientConnection(void)
{
  OLEDDisplayStatus("WAIT FOR BLE CLIENT");
  while(!SerialBT.connected())
  {
    SYSTEM_STATE = BLE_CLIENT_CONNECTED;
  }
}

char* BLEClientConnected(void)
{
  char* command;
  static uint8_t greeting_displayed = FALSE;
  OLEDDisplayStatus("BLE CLIENT CONNECTED");
  if(greeting_displayed == FALSE) 
  {
    SerialBT.write(bluetooth_greeting, sizeof(bluetooth_greeting)/sizeof(uint8_t) - 1);
    greeting_displayed = TRUE;
  }
  while(SerialBT.connected())
  {
    command = CheckForCommand();
    if(command != NULL)
    {
      SYSTEM_STATE = COMMAND;
      return command;
    }
  }
  if(!SerialBT.connected())
  {
    greeting_displayed = FALSE;
  }
  SYSTEM_STATE = AWAIT_BLE_CLIENT;
  return NULL;
}

void ReadUART(void)
{
  const uint8_t end[1] = {10};
  int horizontal_chars = floor(SCREEN_WIDTH/6);                           // each char is about 6 pixels wide
  OLEDDisplayStatus("UART READ MODE");
  SerialBT.write((uint8_t*)"ESP READ MODE INITIATED\n", 24);
  while(SerialBT.connected())
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
      
      if (display.getCursorY() >= SCREEN_HEIGHT - lines*LINE_HEIGHT + 1) 
      {
        for (int y = 0; y < SCREEN_HEIGHT; y++) 
        {
          for (int x = 0; x < SCREEN_WIDTH; x++) 
          {
            display.drawPixel(x, y, display.getPixel(x, y + lines*LINE_HEIGHT));
          }
        }
        display.setCursor(0, SCREEN_HEIGHT - lines*LINE_HEIGHT);
      }
      display.setCursor(0, display.getCursorY());
      display.println(data);
      display.display();

      SerialBT.write((uint8_t*)data.c_str(), data_len);
      SerialBT.write(end, 1);

      digitalWrite(LED_BUILTIN, LOW); // while OLED is running, must set GPIO25 in high
    }
  }
  SYSTEM_STATE = AWAIT_BLE_CLIENT;
}

/* SETUP FUNCTIONS */
void SerialSetUp(void)
{
  Serial.begin(115200);
  Serial.println(F("\nESP32 Serial Viewer"));
  Serial.println(firmware_version);
}

void OLEDSetUp(void)
{
  Wire.begin(OLED_SDA, OLED_SCL); // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS, false, false)) // Address 0x3C for 128x32
  { 
    SetErrorID(OLED_ERROR);
    display_fail();
  }
  else
  {
    ClearErrorID(OLED_ERROR);
    display.clearDisplay(); // Clear display
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);  // Set the cursor to the top-left corner of the display
    display.println("ESP32 SERIAL VIEWER");
    display.println(firmware_version);
    display.println("---------------------");
    display_ok(); // print OK message
  }
}

void BluetoothSetUP(void)
{
  if(SerialBT.begin(bluetooth_name)) //Bluetooth device name
  {
    ClearErrorID(BLUETOOTH_ERROR);
    display_ok();
  }
  else
  {
    SetErrorID(BLUETOOTH_ERROR);
    display_fail();
  }
}

void LoRaSetUp(void)
{
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS); // Initialize SPI for LoRa tranceiver
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(LORA_BAND)) 
  {
    SetErrorID(LORA_ERROR);
    display_fail();
  }
  else
  {
    ClearErrorID(LORA_ERROR);
    display_ok();
  }
}

void SDSetup(void)
{
  sd_spi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, sd_spi))
  {
    SetErrorID(SD_CARD_ERROR);
    display_fail();
  }
  else
  { 
    ClearErrorID(SD_CARD_ERROR);
    display_ok();
  }
}

/* BLUETOOTH FUNCTIONS */
char* BluetoothReceive(void)
{
  uint8_t index = 0;
  char c;
  static char buffer[64];
  while(SerialBT.available()) 
  {
    c = SerialBT.read();
    if(c == '\n')
    {
      buffer[index] = '\0';
      return buffer;
    }
    buffer[index] = c;
    index++;
    delay(1); // wait for whatever reason
  }
  return NULL;
}

/* ERROR FUNCTIONS */
/**
  * @brief  Prints the SYSTEM_STATUS variable
  * @retval none
  */
void PrintSystemStatus(void)
{
	char data[32] = "";
	sprintf(data, "ERROR STATUS: %d", SYSTEM_STATUS);
	Serial.println(data);
  display.println(data);
  display.display();
}

/**
  * @brief  Reads the SYSTEM_STATUS variable
  * @retval the error ID
  */
uint16_t CheckSystemError(void)
{
	for(uint8_t i = 0; i < 16; i++)
	{
		if(SYSTEM_STATUS&(1<<i))
		{
			return(1 << i);
		}
	}
	return 0;
}

/**
  * @brief  Checks the SYSTEM_STATUS for a specific ID
  * @retval TRUE if the error is found or FALSE if not found
  */
uint8_t CheckErrorID(uint16_t error)
{
	if((SYSTEM_STATUS&error) == error)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
  * @brief  Adds a specific Error ID to the SYSTEM_STATUS variable
  * @retval none
  */
void SetErrorID(uint16_t error)
{
	SYSTEM_STATUS = SYSTEM_STATUS | error;
}

/**
  * @brief  Removes a specific Error ID to the SYSTEM_STATUS variable
  * @retval none
  */
void ClearErrorID(uint16_t error)
{
	SYSTEM_STATUS &= ~error;
}

/* COMMAND FUNCTIONS */
char* CheckForCommand(void)
{
  char* data = BluetoothReceive();
  if(data != NULL)
  {
    if(strncmp(data, "esp", 3) == 0 && strlen(data) >= 8)
    {
      if(strncmp(&data[4], "set", 3) == 0) {}
      else if(strncmp(&data[4], "exe", 3) == 0)
      {
        if(strncmp(&data[8], "read", 4) == 0) data[0] = READ_UART; 
        if(strncmp(&data[8], "list", 4) == 0) data[0] = LIST_SD; 
        else if(strncmp(&data[9], "stop", 4) == 0) {}
      }
      else if(strncmp(&data[4], "get", 3) == 0) {}
      else if(strncmp(&data[4], "help", 4) == 0)  // OK
      {
        SerialBT.write(help_message, sizeof(help_message)/sizeof(uint8_t) - 1);
        return NULL;
      }
      return data;
    }
  }
  return NULL;
}

void ExecuteCommand(char* command)
{
  uint8_t command_id;
  if(command != NULL) 
  {
    command_id = command[0];
    switch(command_id)
    {
      case READ_UART:
        ReadUART();
        break;
      case LIST_SD:
        SDListDir();
        break;
      default:
        Serial.println("Default");
        break;
    }
  }
  if(SerialBT.connected())
  {
    SYSTEM_STATE = BLE_CLIENT_CONNECTED;
  }
  else
  {
    SYSTEM_STATE = AWAIT_BLE_CLIENT;
  }
}

/* OLED functions */
void OLEDDisplayStatus(String data)
{
  for(uint16_t i = 0; i < SCREEN_WIDTH; i++)
  {
    for(uint8_t k = 0; k < LINE_HEIGHT; k++)
    {
      display.drawPixel(i, k, SSD1306_BLACK);
    }
    display.drawPixel(i, LINE_HEIGHT, SSD1306_WHITE);
  }
  display.setCursor(0, 0);
  display.print(data);
  display.setCursor(0, 2*LINE_HEIGHT);
  display.display();
}

/* SD functions */
void SDListDir(void)
{
  listDir(SD, "/", 0);

}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
  char buffer[2048] = "";
  sprintf(buffer, "Listing directory: %s\n", dirname);
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file)
  {
    if(file.isDirectory())
    {
      strcat(buffer, "  DIR : ");
      strcat(buffer, file.name());
      strcat(buffer, "\n");

      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels)
      {
        listDir(fs, file.name(), levels -1);
      }
    } 
    else 
    {
      strcat(buffer, "  FILE: ");
      strcat(buffer, file.name());
      strcat(buffer, "  SIZE: ");
      strcat(buffer, (char*)file.size());
      strcat(buffer, "\n");

      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
  SerialBT.write((uint8_t*)buffer, sizeof(buffer)/sizeof(char) - 1);
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
