# Smart-Home-Automation

**INTRODUCTION**

This project involves creating a home automation system using an ESP32 microcontroller. The system is designed to control a bulb and a fan based on environmental conditions detected by various sensors. It operates in two modes: automatic and manual. 

In automatic mode, the system uses an HC-SR04 ultrasonic sensor to detect the presence of a person within 10 cm. If a person is detected, it checks the light level with an LDR and the temperature with a DHT11 sensor to decide whether to turn the bulb and fan on or off. The user can control the bulb and fan manually through a web server interface. If there is no internet connection, the system defaults to automatic mode. 

**FUNCTIONAL REQUIREMENTS**

* The project controls two home appliances, a light bulb and a fan connected to a 230v supply through relays. 
* The controller board is connected to the home WiFi network. 
* The fan and bulb are controlled by a mobile connected to a home WiFi network. 
* Auto or mobile control mode can be selected from the mobile phone

**NON FUNCTIONAL REQUIREMENTS**

* Automatic Mode: The bulb should switch on after the entry of a human into the room 
within 1 minute and the fan should turn on as soon as the threshold temperature is 
reached. 
* Manual Mode: The bulb should switch on/off within 1 minute of pressing on the 
mobile phone 
* The system should work in the temperature range of 0 degrees centigrade to 60 degrees 
centigrade.

**SCHEMATIC DIAGRAM**

![image](https://github.com/user-attachments/assets/61533e53-7c25-4adf-bbcd-ac1b824f22ed)

**LIST OF COMPONENTS**

1. ESP32
2. 230V Bulb
3. 230V Fan
4. 2 Channel Relay
5. LDR
6. HC-SR04 Ultrasonic Sensor
7. DHT11 - Temperature/Humidity Sensor
8. BC-547 NPN Transistor

**IMPLEMENTATION**

![image](https://github.com/user-attachments/assets/c71bae98-c9fd-43a4-b9ef-c3d4f6813992)

![image](https://github.com/user-attachments/assets/0fdc1e15-f792-4402-a15d-bc02f711eb08)

