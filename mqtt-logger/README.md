# Project description
The objective of this project is to create a remote monitoring system for tracking the temperature and brightness levels within a living space. 
The implemented solution utilizes the RP2040 Launchpad, the WiFiTick add-on board and the Educational Boosterpack MK II to provide real-time data access through a web interface. 
The measurements are visualised on a dashboard using https://io.adafruit.com/.

# Features
- **Remote Monitoring:**
  The project enables the user to remotely monitor the temperature and brightness of their living space in real-time with Adafruit.
- **Offline Temperature Display:**
  In addition to the online dashboard, the temperature and brightness values are also displayed on the LDC Display of the Educational Boosterpack MK II. 
This feature allows users to check the environment's status even when not connected to the internet.
- **Feedback Mechanism:**
To transfer sensor data of the Educational Boosterpack MK II from the RP2040 to the ESP8266 on the WiFiTick add-on Board, Backchannel UART is utilized.
The ESP8266 sends the received messages with the measurement data back and publishes the sensor data to Adafruit.io feeds.
It also sends a response message via Backchannel UART telling whether the data publication was successful or not.

# Project Structure
- In [Room.cpp](RoomMeasurements%2FRoom.cpp)
the sensor values from the temperature sensor and light sensor are measured and rounded to two decimal places. 
Additionally, the class contains a doubleToString method, which converts the sensor readings into a yahal_String object for the communication via UART.

- The Communication with the ESP8266 takes place in the [main.cpp](RoomMeasurements%2Fmain.cpp) file. 
Here, the measured sensor values are displayed on the Educational MK II's display and sent to the ESP8266 via UART. 
Furthermore, there is a Message Receive Handler to receive and output the messages sent back by the ESP8266. 

- In [Communication.cpp](esp8266%2FCommunication.cpp) file the measurement values are read and published to the corresponding MQTT topics.
Additionally, a message is sent back to confirm whether the received message was correctly interpreted. 
Another message is provided about whether the publishing was successful or not.

# Setup
## Adafruit Setup
- Create an account on https://io.adafruit.com/
- go to feeds and create a new feed group called "Room"
- create a feed called "temperature" and a feed called "brightness" within that group
![Feeds.png](img%2FFeeds.png)

- go to dashboards and create a new dashboard called "Room"
![Dashboard.png](img%2FDashboard.png)

- open the dashboard and add blocks for the "temperature" and the "brightness" feed to show the values on the dashboard
![Block.png](img%2FBlock.png)

- open "./esp8266/config.h" and to enter your Adafruit.io username and your Adafruit.io API Key

## WifiTick Setup
- open "./esp8266/config.h" and to enter your SSID and your Password to connect the ESP8266 to your Access Point
- download and install Arduino IDE 1.8.19
- start Arduino IDE
- go to File/Preferences 
- fill out the "Boards Manager URLs" textbox with http://arduino.esp8266.com/stable/package_esp8266com_index.json
- navigate to Tools/Board an open the board manager in the submenu
![Board Manager.png](img%2FBoard%20Manager.png)

- search for ESP8266 and click the installation button
![ESP8266 Board.png](img%2FESP8266%20Board.png)

- navigate to Tools/Manage Libraries...
- install the Adafruit MQTT Library and all library dependencies (see https://learn.adafruit.com/mqtt-adafruit-io-and-you/arduino-plus-library-setup)
![AdafruitMQTTLibrary.png](img%2FAdafruitMQTTLibrary.png)

- connect the WiFiTick add-on board to the RP2040 launchpad
![Board.png](img%2FBoard.png)

- upload the [WifiTick-Programmer](WifiTick-Programmer) project on the RP2040 launchpad
- switch the toggle of the RP2040 launchpad from USB PC to USB Target
- connect the RP2040 board with your computer using the USB Target port
- open the device manager to find out the COM port
- open a terminal and navigate to "esp8266"
- replace the following command with the correct COM port and the correct path to arduino.exe and enter it in your terminal
```
mkdir build
cd build
cmake -G"MinGW Makefiles" -DARDUINO_INSTALL_PATH="C:\Program Files (x86)\Arduino" ..
mingw32-make
mingw32-make upload SERIAL_PORT=COM4
```
- wait for the output in your terminal until the code is uploaded
- open the serial port monitor and check the output
- in case that the blue LED of the Educational Booster Pack starts shining press the S1 button to reset the board
- wait for the output to verify that the connection to MQTT was successful
- switch the toggle of the RP2040 launchpad from USB Target back to USB PC
- connect the RP2040 board to your PC using the USB PC port of the RP2040 launchpad
- upload the [RoomMeasurements](RoomMeasurements) project on the RP2040 launchpad