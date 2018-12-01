/*
The USYD RowBot Implementation of the RobotX Light Buoy

This implementation is for the Arduino MEGA, an uno can't handle the required memory

Regularly reads 3 digital pins for the desired state and displays it to the panels.
Configurable delay time and read pins.

This implementation uses 3 Adafruit 32*64 LED Matrix Panels https://www.adafruit.com/product/2276
The panels are all driven simultaneously by linking the IDC cables to clone the signal.

This implementation is cheap and dirty, so don't come crying to me when you
build it and you can't make a Nyan Cat gif circle around your light tower.

Author: Alexander Norris
Version: 0.1
*/

// The Adafruit Library that does all the magic
#include <RGBmatrixPanel.h>

// -------------- Definitions for the pin constants ---------------------------

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro)

//#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11 // USE THIS ON ARDUINO MEGA
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

// Pins for Reading Colour Commands, will be used to respond to external commands
#define RED_PIN 3
#define GREEN_PIN 4
#define BLUE_PIN 5

// ------------------------- Sampling Delay ------------------------------------
// #define SAMPLE_DELAY 50
#define SAMPLE_DELAY 1

// ---------------------- Definitions of Colours ------------------------------

// All Black and White
#define COL_BLACK matrix.Color333(0,0,0)
#define COL_WHITE matrix.Color333(7,7,7)

// Principle Colours
#define COL_RED matrix.Color333(7,0,0)
#define COL_GREEN matrix.Color333(0,7,0)
#define COL_BLUE matrix.Color333(0,0,7)

// Colours defined for fun
#define COL_YELLOW matrix.Color333(7,7,0)
#define COL_PURPLE matrix.Color333(7,0,7)
#define COL_AQUA matrix.Color333(0,7,7)

const uint16_t colourSet[] = [COL_BLACK,COL_BLUE,COL_GREEN,COL_AQUA,COL_RED,COL_PURPLE,COL_YELLOW,COL_WHITE];


// ----------------------------- Functions -------------------------------------
/*!
   \brief Updates the panel with data from the input pins
   \param the colour value that represents the current state of the panel
   \pre matrix, pins and current state must all be declared and configured
   \return void
*/
void updatePanel(uint16_t *state);

/*!
   \brief Reads the data from the input pins and outputs it as an unigned byte
   \pre Pins must be configured as digital inputs in setup
   \return integer displaying bit field of input
*/
uint8_t readInput(void);

/*!
   \brief Determines the specific colour value specified by the 3-bit value given
   \param index - unsigned int from 0-7 specifying the colour to be written
   \param colSet - array of colour values that correspond to the 8 allowed colours
   \return uint16_t colour value specifying the exact colour to write to the panel
*/
uint16_t determineColour(uint8_t index, uint16_t* colSet);
