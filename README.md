# # 7Semi-HX711-Arduino-Library

This Arduino library provides support for the **7Semi HX711 Load Cell Amplifier Module**, a 24-bit ADC designed specifically for weigh-scale and pressure sensor applications. It enables accurate and high-resolution measurement from load cells using a digital interface.

![Arduino](https://img.shields.io/badge/platform-arduino-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg)

---

## Hardware Required

- 7Semi HX711 Load Cell Amplifier Module  
- Load cell (strain gauge-based)  
- Arduino-compatible board  
- Two GPIO pins for DOUT and SCK  

---

## Getting Started

### 1. Installation via Arduino Library Manager

1. Open the **Arduino IDE**
2. Go to:
   - `Sketch > Include Library > Manage Libraries…` (IDE 1.x)  
   - or use the 📚 icon in Arduino IDE 2.x sidebar
3. Search for:
   -7Semi HX711
4. Click **Install**

Then include in your sketch:

#include <7semi_hx711.h>

### 2. Wiring

| HX711 Pin | Arduino Pin |
| --------- | ----------- |
| VCC       | 5V          |
| GND       | GND         |
| DT (DOUT) | D2          |
| SCK       | D3          |


 
