# Smart-Home-Automation

**Introduction**
This project involves creating a home automation system using an ESP32 microcontroller. The system is designed to control a bulb and a fan based on environmental conditions detected by various sensors. It operates in two modes: automatic and manual. 

In automatic mode, the system uses an HC-SR04 ultrasonic sensor to detect the presence of a person within 10 cm. If a person is detected, it checks the light level with an LDR and the temperature with a DHT11 sensor to decide whether to turn the bulb and fan on or off. In manual mode, the user can control the bulb and fan through a web server interface. If there is no internet connection, the system defaults to automatic mode. 

**Functional Requirements**
* The project controls two home appliances, a light bulb and a fan connected to 230v supply through relays. 
* The controller board is connected to the home WiFi network. 
* The fan and bulb are controlled by a mobile connected to home WiFi network. 
* Auto or mobile control mode can be selected from mobile phone

**Non Functional Requirements**
* Automatic Mode: The bulb should switch on after the entry of human in the room 
within 1 minute and fan should turn on as soon as the threshold temperature is 
reached. 
* Manual Mode: The bulb and should switch on/off within 1 minute of pressing on the 
mobile phone 
* The system should work in the temperature range 0 degree centigrade to 60 degrees 
centigrade.

![image](https://github.com/user-attachments/assets/61533e53-7c25-4adf-bbcd-ac1b824f22ed)

**List of Components**
1. ESP32
2. 230V Bulb
3. 230V Fan
4. 2 Channel Relay
5. LDR
6. HC-SR04 Ultrasonic Sensor
7. DHT11 - Temperature/Humidity Sensor
8. BC-547 NPN Transistor




