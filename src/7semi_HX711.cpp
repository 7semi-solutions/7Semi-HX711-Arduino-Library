/**
 * @file 7semi_HX711.cpp
 * @brief Lightweight HX711 load cell ADC library using manual bit-banging.
 * @author 7semi
 * @version 1.0.0
 * 
 * This library communicates with the HX711 24-bit ADC without using any third-party dependencies.
 * It supports tare calibration, averaging, and scale factor conversion to grams.
 */

#include "7semi_HX711.h"

// Constructor: set data and clock pins
HX711_7semi::HX711_7semi(int dataPin, int clockPin) {
  data_pin = dataPin;
  clock_pin = clockPin;
}

// Initialize pins
void HX711_7semi::begin() {
  pinMode(data_pin, INPUT);
  pinMode(clock_pin, OUTPUT);
  digitalWrite(clock_pin, LOW);
}

// Read one 24-bit raw sample from HX711
long HX711_7semi::readRaw() {
  // Wait until HX711 is ready (DOUT goes LOW)
  while (digitalRead(data_pin) == HIGH);

  long value = 0;
  for (int i = 0; i < 24; i++) {
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(1);
    value <<= 1;
    if (digitalRead(data_pin)) value++;
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(1);
  }

  // Send 25th clock pulse to set gain = 128
  digitalWrite(clock_pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(clock_pin, LOW);
  delayMicroseconds(1);

  // Sign-extend 24-bit to 32-bit
  if (value & 0x800000) {
    value |= 0xFF000000;
  }

  return value;
}

// Average multiple raw readings
long HX711_7semi::readAverage(int samples) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += readRaw();
  }
  return sum / samples;
}

// Store tare (zero) offset using average
void HX711_7semi::tare(int samples) {
  _tare = readAverage(samples);
}

// Set user-defined scale factor (raw units per gram)
void HX711_7semi::setScale(int scale) {
  scale_factor = scale;
}

// Return calculated weight in grams
float HX711_7semi::getWeight() {
  long raw = readAverage(5);
  return (raw - _tare) / scale_factor;
}
