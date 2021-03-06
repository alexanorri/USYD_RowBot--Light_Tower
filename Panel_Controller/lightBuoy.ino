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

#include "lightBuoy.h"

#define DEBUGGING 1

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);

uint16_t curState;

void setup() {
  // configure the panel interface, all panels are cloned off the one line set
  matrix.begin();

  // Configures the data capture pins to digital inputs
  pinMode(RED_PIN, INPUT);
  pinMode(GREEN_PIN, INPUT);
  pinMode(BLUE_PIN, INPUT);

  // Blanks the panel before we begin
  matrix.fillScreen(COL_BLACK);

  // sets the initial state
  curState = COL_BLACK;

  if(DEBUGGING){
    // Serial for debugging the software
    Serial.begin(9600);
  }
}

void loop() {
  // updates the panel with data from the input pins
  updatePanel(*curState);

  // delay for the specified time
  delay(SAMPLE_DELAY);
}

/*!
   \brief Updates the panel with data from the input pins
   \param the colour value that represents the current state of the panel
   \pre matrix, pins and current state must all be declared and configured
   \return void
*/
void updatePanel(uint16_t *state) {
  // Checks the input to determine if a new colour command has been received
  uint16_t newColour = determineColour(readInput(), colourSet);

  // checks if there's been any change in signal
  if(*curState != newColour) {
    // write the colour to the panel
    matrix.fillScreen(newColour);

    if(DEBUGGING){
      Serial.println(readInput);
    }

    *curState = newColour;
  }
}

/*!
   \brief Reads the data from the input pins and outputs it as an unigned byte
   \pre Pins must be configured as digital inputs in setup
   \return integer displaying bit field of input
*/
uint8_t readInput(void) {
  uint8_t colourSet readVal = 0;
  // reads input and bit-shifts it to the correct positions
  readVal |= (digitalRead(RED_PIN) << 2);
  readVal |= (digitalRead(GREEN_PIN) << 1);
  readVal |= digitalRead(BLUE_PIN);

  return readVal;
}

/*!
   \brief Determines the specific colour value specified by the 3-bit value given
   \param index - unsigned int from 0-7 specifying the colour to be written
   \param colSet - array of colour values that correspond to the 8 allowed colours
   \return uint16_t colour value specifying the exact colour to write to the panel
*/
uint16_t determineColour(uint8_t index, uint16_t* colSet) {
  return colSet[index];
}
