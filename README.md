
# Smart Home Firmware

  - Arduino C
  - Version 2.0
  - /023

## How to create a project on the Angaza Elimu Platform
Head over to the Angaza Elimu platform and register for an account. The registration page allows you to create a user profile for accessing the platform's features. [Angaza Elimu Platform](https://smart-home-iot.angazaelimu.com/)

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/Signup.png)


## Login 
Once registered, use your credentials to log in to the Angaza Elimu platform.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/login.png)

## Create Project

Once you’re logged in, you will be taken to the homepage. Click the Create Project button to create a new project. Clicking the Create Project button on the left side menu bar will also take you to the page for creating a new project. 

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/Create%20project.png)

In the Create Project page, you will need to create a name for your project and select the hardware for your project. For this project we will name it temperature and humidity. Also, give it a meaningful project description as done below and click on create.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/c32de850a9d1b5c0fb2c7ad2f03270d0d181f3fa/Images/temp_project.png)

After Successful project creation you will get this popup.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/Successful%20project%20creation.png)

## Hardware components

Connect the 4 pin Female-Female jumper cable to the OLED screen. 
Connect the SDA pin of the OLED screen to the SDA pin of the ESP32, SCL pin to SCL pin, GND pin to GND pin and VCC pin  to 3.3V pin
Connect the 3 pin Female - Female jumper cable to the DHT22 sensor. 
Connect the OUT pin of the DHT22 sensor to pin 7 of the ESP32, VCC (+) pin to 3.3V pin, and the GND (-) pin to GND pin, as shown in the diagram below.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/DHT_components.png)

## Api Key
Inside your project control page, you will see the API key. This API key is what will allow your IoT kit to communicate with the IoT platform. 

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/apikey.png)

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/data-1.png)

## Data Collection
On the screen we can be able to see the data in real time. The table below will show the temperature,humidity and the time it was collected.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/aad044f2a2269e7618c54b3a0069805f034542a1/Images/data_numbers.png)

## Threshhold
The threshold can be automatically set. Users can be able to set various thresholds to achieve different functionalities.

![image alt](https://github.com/ellie-ochieno/AE-Firmware/blob/1dd7b8d7e0a040d8338a19ee5dae03cc6f9f2b2b/Images/set-threshold.png)



# buni-smarthome.v2-firmware

**Angaza Elimu Smarthome v2 project - firmware**

Welcome to the firmware repository for the Angaza Elimu Smarthome version 2 project. This repository contains firmware and Arduino libraries designed to control the various components of the Smarthome system. The target platform is ESP32 family devices(tested on ESP32-S2), utilizing board library versions 2.17 and above. The firmware is written in C++ and developed specifically for the Arduino IDE.

## Description

This repository holds the firmware for the Angaza Elimu Smarthome v2 project. The Smarthome project is built around the ESP32 platform, using various sensors and actuators to automate and control different elements of a Smarthome kit. Version 2 of the firmware introduces more robust handling of sensor data, control mechanisms, and enhanced communication features.

### Target Platform

- **Device**: ESP32-S2
- **Arduino Core Version**: 1.18 and above
- **Firmware Language**: C++

## Features

- **Modular Sensor Libraries**: Every sensor library is self-contained in a zip file with all necessary support files.
- **Basic Sensor Example Code**: Each sensor library includes a basic `.ino` example to help users get started quickly.
- **EEPROM-Based Configuration**: Persistent settings using ESP32’s EEPROM.
- **Secure HTTP Client**: Built-in support for secure communication with cloud services.
- **Servos, LEDs, Fans and Sensors Control**: Simple functions for events actuation, notification and monitoring environmental data.

## How to Use This Repository

### Installation Instructions

To use the firmware files and sensor libraries from this repository, follow the instructions below:

1. **Clone the Repository**  
   Clone the `iot-smarthome.v2-firmware` repository from GitHub:
   ```bash
   git clone https://github.com/Angaza-Elimu/buni-smarthome-firmware-v2.git
   ```
   - With this approach, the whole repository files will be available on your local machine.

2. **Download and Install Individual Sensor Libraries**
   - Each sensor library is packaged as a zip file, containing all the support files for controlling the specific sensor.
   - Download the zipped file from the repository under desired sensor directory/folder.
   - Open Arduino IDE and follow these steps:
     - Go to `Sketch` -> `Include Library` -> `Add .ZIP Library...`
     - Select the downloaded sensor zip file from your local machine and click **Ok**.
     - The library will now be available in the Arduino IDE.

3. **Access Example Files**
   - After the sensor library is installed, a basic `.ino` example file will be available for testing and getting started.
   - To access the example file:
     - Go to `File` -> `Examples` -> `Installed_Library_Name`
     - Open the example `.ino` file.
   - Alternatively, you can navigate directly to the `examples` folder in each sensor library directory within the GitHub repository.

### Quick Start Guide for Using Sensor Libraries

1. **Download Sensor Firmware**  
   Download the sensor firmware zip file from the corresponding folder in the repo.

2. **Install Sensor Library**  
   In Arduino IDE, navigate to:
   - `Sketch` -> `Include Library` -> `Add .ZIP Library...`
   - Select the downloaded zip file.

3. **Load Example Sketch**  
   Go to:
   - `File` -> `Examples` -> `Installed_Library_Name`
   - Load the example sketch, modify the necessary parameters:
      - Sensor `pins` in `pin_configuration.h` file,
      - `WiFi credentials` and `API Key` under `secrets.h` file, and upload it to your ESP32 board.


### Example Sensor Libraries

- **SmartDoorControl**  
   Control and automate door opening&locking mechanisms using servos, keypad and remote set server details.

   - Example location:  
     `examples/SmartDoorControl/SmartDoorControlExample.ino`

- **MQ2SmokeMonitor**  
   Monitor, detect smoke levels and invoke safety controls using a buzzer alarm for notification.

   - Example location:  
     `examples/MQ2SmokeMonitor/MQ2SmokeMonitorExample.ino`

- **SmartWindowControl**  
   Open and close windows using servos for ventilation control.

   - Example location:  
     `examples/SmartWindowControl/SmartWindowControlExample.ino`

### Additional Notes

- **WiFi Configuration**: Ensure that you have configured the correct `ssid` and `password` values in your firmware for WiFi connectivity.
- **Secure Communication**: This firmware supports secure HTTPS communication.
- **Platform connection**: For successful platform connection, ensure to provide a valid project `API Key`(generated after new project creation).

### Supported Sensors and Devices

- Door and Window Control with Servo and Keypad
- Smoke Detection (MQ2 sensor and Buzzer)
- Temperature and Humidity Monitoring (DHT sensor and Fan)
- Intrusion detection (PIR sensor and Buzzer)
- Rain detection (moisture sensor)
- Light intensity monitoring (LDR sensor)


---

Feel free to open an issue or report problems with the **Buni - Smarthome** library.

Happy Coding! 😊
