/**
 * @file 7semi_HX711.h
 * @brief Lightweight HX711 ADC library using manual bit-banging (no external dependencies).
 * @author 7semi
 * @version 1.0.0
 * 
 * This library provides basic functionality for the HX711 24-bit load cell amplifier:
 * - Raw data reading
 * - Averaging
 * - Tare (zero offset)
 * - Weight calculation with a custom scale factor
 */

#ifndef _7SEMI_HX711_H_
#define _7SEMI_HX711_H_

#include <Arduino.h>

class HX711_7semi {
public:
  /**
   Construct a new HX711_7semi object with specified pins
    dataPin  Pin connected to HX711 DOUT
    clockPin Pin connected to HX711 SCK
   */
  HX711_7semi(int dataPin, int clockPin);

  /**
   Initializes the HX711 pins
   */
  void begin();

  /**
   Read one raw 24-bit sample from HX711
  Signed 32-bit value
   */
  long readRaw();

  /**
   Average multiple raw samples
    samples Number of samples to average (default: 5)
  Averaged signed 32-bit value
   */
  long readAverage(int samples = 5);

  /**
   Set tare offset by averaging current readings
    samples Number of samples to average for tare (default: 10)
   */
  void tare(int samples = 10);

  /**
   Set the scale factor (raw units per gram)
    scale Calibration factor
   */
  void setScale(int scale);

  /**
   Get weight in grams using current tare and scale factor
  Weight in grams
   */
  float getWeight();

private:
  int data_pin, clock_pin;
  long _tare = 0;
  long scale_factor = 1.00;
};

#endif
