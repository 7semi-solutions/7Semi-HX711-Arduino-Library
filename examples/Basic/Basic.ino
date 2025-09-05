/*
 * HX711 Load Cell Scale Factor Calibration Note
 *
 * The scale factor is used to convert raw ADC values from the HX711
 * into grams.
 *
 * Calibration Steps:
 * 1. Tare the scale (set it to zero with no weight).
 * 2. Place a known weight on the load cell (e.g., 1000g).
 * 3. Observe the raw reading from the HX711.
 * 4. Calculate the scale factor using:
 *      scale_factor = raw_reading / known_weight
 * 5. Replace this value eith scale_factor:
 *      scale.set_scale(scale_factor);
 *
 * Tips:
 * - Use accurate, known weights for calibration.
 * - Make sure the load cell is mounted securely during calibration.
 * - Recalibrate if the physical setup changes.
 */
#include <7semi_HX711.h>

// HX711 pin configuration
const int dataPin = 6;   // DOUT
const int clockPin = 7;  // SCK
float scale = 230.00;    // Calibration factor (raw units per gram)

HX711_7semi loadcell(dataPin, clockPin);

void setup() {
  Serial.begin(9600);
  loadcell.begin();

  loadcell.setScale(scale);  // Set scale factor
  loadcell.tare();           // Calibrate zero (tare)
  Serial.println("Tare done. Starting weight measurement...");
}

void loop() {
  float weight = loadcell.getWeight();  // Get weight in grams
  Serial.print("Weight: ");
  Serial.print(weight, 2);
  Serial.println(" g");

  delay(500);
}
