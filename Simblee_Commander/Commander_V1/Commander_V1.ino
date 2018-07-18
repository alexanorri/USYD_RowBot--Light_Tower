/*
 * Copyright (c) 2015 RF Digital Corp. All Rights Reserved.
 *
 * The source code contained in this file and all intellectual property embodied in
 * or covering the source code is the property of RF Digital Corp. or its licensors.
 * Your right to use this source code and intellectual property is non-transferable,
 * non-sub licensable, revocable, and subject to terms and conditions of the
 * SIMBLEE SOFTWARE LICENSE AGREEMENT.
 * http://www.simblee.com/licenses/SimbleeSoftwareLicenseAgreement.txt
 *
 * THE SOURCE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
 *
 * This heading must NOT be removed from this file.
 */
  
#include <SimbleeForMobile.h>

#define DEBUGGING 1 // controls debugging functionality

// Digital IO pins used to send colour commands to the Panel Commander
#define REDPIN 1
#define GREENPIN 2
#define BLUEPIN 3

#define NUM_PINS 3
char pinSet[] = {REDPIN,GREENPIN,BLUEPIN}; // used to simplify writing outputs

// Timing information so we can get nice even flashes
#define FLASH_TIME 1000
#define FLASH_DELTA 10

// Timing Envelope
int deltaTMin = FLASH_TIME - FLASH_DELTA;
int deltaTMax = FLASH_TIME + FLASH_DELTA;

// Bit Masks for each colour to output
char blackMask[] = {0,0,0};
char blueMask[] = {0,0,1};
char greenMask[] = {0,1,0};
char aquaMask[] = {0,1,1};
char redMask[] = {1,0,0};
char purpleMask[] = {1,0,1};
char yellowMask[] = {1,1,0};
char whiteMask[] = {1,1,1};

// Light code general information
#define NUM_COLOURS 3 // The number of coloured flashes in a given code
const int numSegs = 2*NUM_COLOURS + 1; // the number of total segments we will need to run through

// State information
int curSeg = 0; // the current segment we're working with
char colSequence[] = {'K','K','K','K','K','K','K'}; // the full colour sequence we're working with
char curPins[] = {0,0,0}; // the current output pattern in use
unsigned long delayStart = 0; // the time when we started delaying from

// Text Input
char inText[] = "APY"; // initial debug state with aqua-purple-yellow scheme
boolean textChanged = false; // change flag
char* defaultTextField; // Stores raw data from the input text field

// UI Objects
uint8_t textField, updateButton; // interactive objects
uint8_t ui_rect; // the default object
uint8_t legendText; // the text space we'll use to display debugging info if in use

const char *defaultLegend = "Allowed Characters Legend\n K - Black \n W- White \n R - Red \n G - Green \n B - Blue \n A - Aqua \n P - Purple \n Y - Yellow";

/*
 * Traditional Arduino setup routine
 * 
 * Initialize the SimbleeForMobile environment.
 */
void setup() {
  // put your setup code here, to run once:
  SimbleeForMobile.deviceName = "Name";
  SimbleeForMobile.advertisementData = "Data";
  SimbleeForMobile.domain = "template.simblee.com";
  // Begin Simblee UI
  SimbleeForMobile.begin();

  // Pin Configuration
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  

  // Initial State Configuration
  defaultTextField = (char*)malloc(10); // Allocate memory to store the text from the UI
  delayStart = millis(); // note the initial delay time
  curSeg = 0; // start at the first segment
  textChanged = true; // marks that we need to initialise the display out
}


/*
 * The traditional Arduino loop method
 * 
 * Enable SimbleeForMobile functionality by calling the process method
 * each time through the loop. This method must be called regularly for
 * functionality to work.
 */
void loop() {
  // put your main code here, to run repeatedly:
  // process must be called in the loop for Simblee UI
  SimbleeForMobile.process();  

  // If new text has arrived
  if(textChanged) {
    // iterate across the given string
    for(int i = 0; i < NUM_COLOURS; i++) {
      char curChar = inText[i]; // extract the current character
      colSequence[2*i + 1] = curChar; // set the right character in the sequence string to the new value
    }
    curSeg = 0; // resets position in the colour sequence
    
    // mark the change as being handled
    textChanged = false;
  }
  
  // checks current time and input state
  unsigned long curTime = millis();
  unsigned long deltaTime = curTime - delayStart;

  // If we need to change to a new state
  if(deltaTime >= deltaTMin && deltaTime <= deltaTMax) {
    
    // extract the colour that's next in line to display
    char curChar = colSequence[curSeg];
    
    // determine the pin mask to apply
    char* toCopy = blackMask;
    switch(curChar) {
      case 'K':
        toCopy = blackMask;
        break;
      case 'B':
        toCopy = blueMask;
        break;
      case 'G':
        toCopy = greenMask;
        break;
      case 'A':
        toCopy = aquaMask;
        break;
      case 'R':
        toCopy = redMask;
        break;
      case 'P':
        toCopy = purpleMask;
        break;
      case 'Y':
        toCopy = yellowMask;
        break;
      case 'W':
        toCopy = whiteMask;
        break;
      default:
        toCopy = blackMask;
        break; 
    }

    // copies the pin sequence across to curPins
    memcpy(curPins,toCopy,NUM_COLOURS);

    // iterate across pin set to output the pattern in curPins
    for(int j = 0; j < NUM_PINS; j++){
      char pinNum = pinSet[j]; // get the current pin number
      char state = (curPins[j] == 1) ? HIGH : LOW; // get the state variable
      digitalWrite(pinNum,state); // write this data out
    }

    delayStart = millis();

    // Iterate the current segment, wrapping around
    curSeg = (curSeg + 1)%numSegs ; 
  }

}





/*
 * SimbleeForMobile UI callback requesting the user interface
 */
void ui()
{  
  SimbleeForMobile.beginScreen(WHITE);

  //Create your own UI here!

  // determine screen dimensions
  int screenH = SimbleeForMobile.screenHeight;
  int screenW = SimbleeForMobile.screenWidth;

  // determine layout positions for the different items
  int fieldWidth = screenW / 2; // take up half the screen in width
  int fieldStart = screenW / 4; // start 25% of the way across the screen
  int descHeight = screenH/10; // start the descriptor 10% of the way down the screen
  int inpHeight = screenH/4; // start the input block 25% of the way down the screen
  int btnHeight = screenH/3; // start the update button half way down the screen
  int legendHeight = screenH/2; // start the legend half-way down

  // Start Drawing Items

  // Draw the Explanation Text
  SimbleeForMobile.drawText(fieldStart,descHeight,"Enter 3 Capitalised Letters for Colour Sequence\n e.g. \"RGB\"");

  // Draw the Input Field
  textField = SimbleeForMobile.drawTextField(fieldStart,inpHeight,fieldWidth,"Sequence String");

  // Draw the Update Button
  updateButton = SimbleeForMobile.drawButton(fieldStart,btnHeight,fieldWidth,"Update");

  // Configure Events for the Update Button
  SimbleeForMobile.setEvents(updateButton, EVENT_PRESS);

  // Draw the Legend Text OR Debug Text, depending
  legendText = SimbleeForMobile.drawText(fieldStart,legendHeight,defaultLegend);

  // default state
  ui_rect = SimbleeForMobile.drawRect(122, 402, 76, 76, WHITE);
  
  SimbleeForMobile.endScreen();
}





/*
 * SimbleeForMobile event callback method
 */
void ui_event(event_t &event)
{
  // Unsure about this operation, will need to check the function of this so we can make something happen
  if(event.id = textField) {
    // copy the recieved text to our more manageable field
    for(int i = 0; i < NUM_COLOURS; i++){ // limited to three characters because I'm not a monster
      inText[i] = defaultTextField[i];
    }

    if(DEBUGGING) {
      SimbleeForMobile.updateText(legendText,defaultTextField); // echo the received text to the legend field
    }
  }

  if(event.id = updateButton) {
    // mark the text as updated
    textChanged = true;
  }
}


