/*
testScreenFill

Tests the screens by filling them with a R,G,B one at a time.
If this works, then your panels are good to go for use with the light tower.

This code is written to run on an Arduino Uno, but most code is lifted straight from the examples.

Requires:
- Adafruit RGB-matrix-panel library

Author: Alexander Norris
*/

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

//#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

void setup() {
  matrix.begin(); // configures the matrix
}

void loop() {
  // Fills the screen with black i.e. clears it
  matrix.fillScreen(matrix.Color333(0,0,0));
  delay(500);

  // Fills screen with red for 500ms
  matrix.fillScreen(matrix.Color333(7,0,0));
  delay(500);

  // Fills screen with green for 500ms
  matrix.fillScreen(matrix.Color333(0,7,0));
  delay(500);

  // Fills screen with blue for 500ms
  matrix.fillScreen(matrix.Color333(0,0,7));
  delay(500);
}
