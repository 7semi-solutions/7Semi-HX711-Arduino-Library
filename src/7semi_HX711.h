/**
 * 7semi_HX711.h
 * Lightweight HX711 ADC library using manual bit-banging
 * 
 * - Raw data reading (24-bit)
 * - Averaging with 64-bit accumulator
 * - Tare (zero offset)
 * - Weight calculation with a float scale factor
 * - Gain selection and basic timeout handling
 */

#ifndef _7SEMI_HX711_H_
#define _7SEMI_HX711_H_

#include <Arduino.h>
  /**
   * HX711 gain selection
   * - GAIN_128: Channel A, gain 128 (typical)
   * - GAIN_32 : Channel B, gain 32
   * - GAIN_64 : Channel A, gain 64
   */
  enum Gain {
    GAIN_128 = 1,  // 25 total clock pulses
    GAIN_32  = 2,  // 26 total clock pulses
    GAIN_64  = 3   // 27 total clock pulses
  };

  
/**
 * HX711_7semi
 * Simple HX711 driver for load cell measurements
 */
class HX711_7semi {
public:
  /**
   * Construct a new HX711_7semi object with specified pins
   * - dataPin  Pin connected to HX711 DOUT
   * - clockPin Pin connected to HX711 SCK
   */
  HX711_7semi(int dataPin, int clockPin);

  /**
   * Initialize HX711 pins and send a clean wake-up pulse
   * - Call once in setup()
   */
  void begin();

  /**
   * Set the HX711 gain mode
   * - gain One of GAIN_128, GAIN_32, GAIN_64
   * - Default after construction is GAIN_128
   */
  void setGain(Gain gain);

  /**
   * Configure maximum wait time for HX711 ready signal
   * - timeoutMs Timeout in milliseconds
   * - 0 disables timeout (waits forever)
   */
  void setTimeout(unsigned long timeoutMs);

  /**
   * Read one raw 24-bit sample from HX711
   * - Blocking until data ready or timeout expires
   * - Signed 32-bit value (after sign-extension)
   */
  long readRaw();

  /**
   * Average multiple raw samples
   * - samples Number of samples to average (1..50, default 5)
   * - Uses 64-bit accumulator internally for safety
   * - Returns averaged signed 32-bit value
   */
  long readAverage(int samples = 5);

  /**
   * Set tare offset by averaging current readings
   * - samples Number of samples to average for tare (default 10)
   * - Stored offset is used by getWeight()
   */
  void tare(int samples = 10);

  /**
   * Set the scale factor in raw units per gram
   * - scale Calibration factor (must be non-zero)
   * - If 0 is passed, scale is forced to 1.0 internally
   */
  void setScale(float scale);

  /**
   * Get weight in grams using current tare and scale factor
   * - samples Number of raw readings to average (default 5)
   * - Returns 0.0 if scale factor is zero
   */
  float getWeight(int samples = 5);

  /**
   * Put HX711 into power down mode
   * - Drives SCK high for at least 60 µs
   */
  void powerDown();

  /**
   * Wake HX711 from power down mode
   * - Drives SCK low
   */
  void powerUp();

private:
  int data_pin;
  int clock_pin;

  long  _tare         = 0;
  float scale_factor  = 1.0f;
  Gain  _gain         = GAIN_128;
  unsigned long _timeout_ms = 1000;  // default 1 second

  /**
   * Wait until HX711 is ready (DOUT goes LOW)
   * - timeoutMs Maximum time to wait in milliseconds
   * - 0 waits forever
   * - Returns true if ready, false on timeout
   */
  bool waitForReady(unsigned long timeoutMs);
};

#endif
