// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <RGBmatrixPanel.h>

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

#define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
//#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3


RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);

// For reading and following commands
#define REDPIN 11
#define GREENPIN 12
#define BLUEPIN 13

// Function Declarations
int pollPins(char** response);
int updateDisplay(void);


void setup() {

  matrix.begin();
}

void loop() {
  updateDisplay();
  delay(500);
//   // Do nothing -- image doesn't change
//   matrix.fillScreen(matrix.Color333(0,0,0));
//   delay(2000);
//   matrix.fillScreen(matrix.Color333(7,0,0));
//   delay(1000);
// //  matrix.fillScreen(matrix.Color333(0,0,0));
// //  delay(500);
//   matrix.fillScreen(matrix.Color333(0,7,0));
//   delay(1000);
// //  matrix.fillScreen(matrix.Color333(0,0,0));
// //  delay(500);
//   matrix.fillScreen(matrix.Color333(0,0,7));
//   delay(1000);
// //  matrix.fillScreen(matrix.Color333(0,0,0));
// //  delay(500);

}

int pollPins(char** response) {
  *response[0] = (digitalRead(REDPIN) == HIGH) ? 7 : 0;
  *response[1] = (digitalRead(GREENPIN) == HIGH) ? 7 : 0;
  *response[2] = (digitalRead(BLUEPIN) == HIGH) ? 7 : 0;

  return 0;
}

int updateDisplay(void) {
  char redState, greenState, blueState;
  char* stateSet[] = {&redState,&greenState,&blueState};

  pollPins(stateSet); // reads the pin information

  // update the display
  matrix.fillScreen(matrix.Color333(redState,greenState,blueState));
  return 0;
}
