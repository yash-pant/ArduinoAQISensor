# Air Quality Monitoring System 🌫️

An Arduino-based air quality monitoring project that integrates the Plantower PMS5003 sensor to measure real-time particulate matter levels (PM1.0, PM2.5, and PM10). The system is designed to be compatible across multiple Plantower sensor models and supports environmental health analysis through live air pollution data.

## Overview
This project uses an Arduino microcontroller and a Plantower PMS-series particulate sensor to collect and analyze airborne particle concentrations. The firmware supports PMS1003–PMS7003 sensors, enabling flexible deployment with different hardware configurations.

## Features
- Real-time measurement of **PM1.0, PM2.5, and PM10**
- Compatible with **Plantower PMS1003–PMS7003** sensors
- Serial output for live data monitoring
- Lightweight, modular firmware for easy extension
- Suitable for environmental health and air quality analysis

## Hardware Requirements
- Arduino (Uno, Nano, or compatible)
- Plantower PMS5003 (or PMS1003–PMS7003)
- Jumper wires
- USB cable for programming and power

## Software Requirements
- Arduino IDE
- Plantower PMS sensor library (or custom serial parser)

## How It Works
1. The PMS sensor continuously samples airborne particles.
2. Sensor data is transmitted via UART to the Arduino.
3. Firmware parses the data packet and extracts PM1.0, PM2.5, and PM10 values.
4. Measurements are output in real time through the Arduino Serial Monitor for analysis.

## Setup & Usage
1. Connect the PMS sensor to the Arduino using UART (TX/RX).
2. Open the project in the Arduino IDE.
3. Select the correct board and port.
4. Upload the firmware.
5. Open the Serial Monitor to view live particulate measurements.

## Applications
- Indoor air quality monitoring
- Environmental health risk assessment
- Pollution trend analysis
- IoT-based environmental sensing projects

## Future Improvements
- LCD or OLED display support
- Data logging to SD card or cloud
- Threshold-based alerts
- Integration with IoT platforms

## License
This project is open-source and I modified it to fit my needs
