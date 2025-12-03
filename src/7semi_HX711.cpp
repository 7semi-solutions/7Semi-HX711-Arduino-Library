/**
 * 7semi_HX711.cpp
 * Lightweight HX711 load cell ADC library using manual bit-banging
 *
 * - Uses Arduino core only
 * - 24-bit signed raw reads with proper sign extension
 * - 64-bit safe averaging
 * - Configurable gain and timeout
 */

#include "7semi_HX711.h"

/**
 * Constructor
 * - Stores data and clock pin numbers
 * - Does not touch pin modes, call begin() first
 */
HX711_7semi::HX711_7semi(int dataPin, int clockPin) {
  data_pin  = dataPin;
  clock_pin = clockPin;
}

/**
 * Initialize HX711 interface
 * - Configures data as input and clock as output
 * - Ensures a clean wake-up pulse on the clock line
 */
void HX711_7semi::begin() {
  pinMode(data_pin, INPUT);
  pinMode(clock_pin, OUTPUT);

  digitalWrite(clock_pin, LOW);
  delayMicroseconds(1);

  digitalWrite(clock_pin, HIGH);
  delayMicroseconds(1);
  digitalWrite(clock_pin, LOW);
  delayMicroseconds(1);
}

/**
 * Configure gain mode
 * - Updates internal gain setting used after each 24-bit read
 * - Gain affects extra clock pulses for channel/gain selection
 */
void HX711_7semi::setGain(Gain gain) {
  _gain = gain;
}

/**
 * Configure ready-wait timeout
 * - timeoutMs is the maximum wait time in milliseconds
 * - 0 disables timeout and waits indefinitely for data ready
 */
void HX711_7semi::setTimeout(unsigned long timeoutMs) {
  _timeout_ms = timeoutMs;
}

/**
 * Wait until HX711 is ready
 * - Waits for DOUT to go LOW
 * - Uses internal timeout in milliseconds
 * - Returns true when ready, false on timeout
 */
bool HX711_7semi::waitForReady(unsigned long timeoutMs) {
  unsigned long start = millis();

  while (digitalRead(data_pin) == HIGH) {
    if (timeoutMs > 0 && (millis() - start) >= timeoutMs) {
      return false;
    }
  }

  return true;
}

/**
 * Read single raw 24-bit value
 * - Waits for device ready with timeout
 * - Reads 24 bits MSB first
 * - Applies sign extension to 32-bit signed
 * - Returns 0 on timeout as a safe fallback
 */
long HX711_7semi::readRaw() {
  if (!waitForReady(_timeout_ms)) {
    return 0;
  }

  uint32_t value = 0;

  for (uint8_t i = 0; i < 24; i++) {
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(1);

    value <<= 1;
    if (digitalRead(data_pin)) {
      value |= 0x01;
    }

    digitalWrite(clock_pin, LOW);
    delayMicroseconds(1);
  }

  uint8_t extraPulses = static_cast<uint8_t>(_gain);
  for (uint8_t i = 0; i < extraPulses; i++) {
    digitalWrite(clock_pin, HIGH);
    delayMicroseconds(1);
    digitalWrite(clock_pin, LOW);
    delayMicroseconds(1);
  }

  if (value & 0x800000UL) {
    value |= 0xFF000000UL;
  }

  return static_cast<long>(value);
}

/**
 * Average multiple raw readings
 * - Uses 64-bit accumulator for safety
 * - Clamps sample count to a reasonable range
 * - Returns averaged 32-bit signed value
 */
long HX711_7semi::readAverage(int samples) {
  if (samples < 1) {
    samples = 1;
  }
  if (samples > 50) {
    samples = 50;
  }

  int64_t sum = 0;

  for (int i = 0; i < samples; i++) {
    sum += static_cast<int64_t>(readRaw());
  }

  int64_t avg = sum / samples;
  return static_cast<long>(avg);
}

/**
 * Capture tare offset
 * - Uses averaged readings as zero reference
 * - Stored value is subtracted in getWeight()
 */
void HX711_7semi::tare(int samples) {
  _tare = readAverage(samples);
}

/**
 * Configure scale factor
 * - Scale is raw units per gram
 * - If 0 is passed, scale is forced to 1.0f to avoid division by zero
 */
void HX711_7semi::setScale(float scale) {
  if (scale == 0.0f) {
    scale_factor = 1.0f;
  } else {
    scale_factor = scale;
  }
}

/**
 * Compute weight in grams
 * - Uses averaged raw reading minus tare
 * - Divides by float scale factor
 * - Returns 0.0 if scale factor is zero
 */
float HX711_7semi::getWeight(int samples) {
  if (scale_factor == 0.0f) {
    return 0.0f;
  }

  long raw = readAverage(samples);
  long net = raw - _tare;

  return static_cast<float>(net) / scale_factor;
}

/**
 * Put HX711 into power down mode
 * - Drives clock line high for at least 60 microseconds
 * - Reduces current consumption when not measuring
 */
void HX711_7semi::powerDown() {
  digitalWrite(clock_pin, LOW);
  delayMicroseconds(1);

  digitalWrite(clock_pin, HIGH);
  delayMicroseconds(70);
}

/**
 * Wake HX711 from power down mode
 * - Brings clock line low again
 * - Device needs a short time before delivering valid data
 */
void HX711_7semi::powerUp() {
  digitalWrite(clock_pin, LOW);
  delayMicroseconds(1);
}
