# esp32_serial_viewer
Turns a TTGO LoRa 32 board into an affordable and portable UART viewer.

### Features
* Prints UART data to the OLED display
* Transmits UART data through BLE
* Includes a python script that feeds a.txt file to the TTGO board to display it on the OLED display

### Roadmap
* Enable commands through Bluetooth that set baud rate and other parameters of the UART
* Enable saving received data to the SD card with custom name
* Enable sending data through the UART
* Enable sending a txt file from the SD to the UART
* Display system state to the screen