# ESP32 Environmental Monitoring with BLE

This code is designed for the ESP32 microcontroller to perform environmental monitoring using a BMP280 sensor for temperature, pressure, and altitude readings, along with battery level monitoring. The data is broadcasted over Bluetooth Low Energy (BLE) for remote monitoring. This README provides an overview of the code, prerequisites, hardware setup, and how to use it with PlatformIO.

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Hardware Setup](#hardware-setup)
- [Software Setup](#software-setup)
- [Libraries](#libraries)
- [Usage](#usage)

## Overview

This code reads temperature, pressure, and altitude data from a BMP280 sensor and battery level from an analog pin. It then broadcasts this data over Bluetooth Low Energy (BLE) using the ArduinoBLE library.

## Prerequisites

### Hardware

- ESP32 microcontroller
- BMP280 sensor
- Battery connected to an analog pin

### Software

- PlatformIO development environment

## Hardware Setup

1. Connect the hardware as follows:

   - VCC: Connect to a power source.
   - GND: Connect to the ground.
   - SCL: Connect to the I2C clock pin (usually labeled SCL).
   - SDA: Connect to the I2C data pin (usually labeled SDA).
   - Battery: Connect to an analog pin for battery level monitoring.

## Software Setup

Ensure you have the PlatformIO development environment set up and configured for your ESP32 board.

## Libraries

To use this code, you'll need the following libraries:

- **Wire.h**: Required for I2C communication.
- **Adafruit_Sensor.h**: Sensor abstraction library.
- **Adafruit_BMP280.h**: Library for interfacing with the BMP280 sensor.
- **ArduinoBLE.h**: Library for Bluetooth Low Energy (BLE) communication.

## Usage

1. Clone or download the code to your local machine.

2. Open the project in PlatformIO.

3. Compile and upload the code to your ESP32 microcontroller using PlatformIO.

4. Monitor the serial output to see the temperature, pressure, altitude, and battery level readings.

- The code reads the BMP280 sensor and battery level and prints the readings to the serial monitor.

5. The code broadcasts the sensor readings and battery level over Bluetooth Low Energy (BLE) at regular intervals.

### Note:

- Adjust the I2C address in the code if your BMP280 sensor has a different address (default is usually 0x76 or 0x77).
- Make sure to install the required libraries through PlatformIO's library manager before compiling the code.

