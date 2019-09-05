/*
 * Kinetic Robotics WS2811 LED Strip Control
 * Written By Alex
 *
 * NOTE: Will be upgraded to WS2812B once we get the new LEDs.
 */

//Include LED Library
#include <FastLED.h>

//Define variables for the number of leds on the LED Strip and the data pin for the Arduino to communicate with the strip.
#define NUM_LEDS 26 //its a strange number because that is how many of the leds weren't utterly obliterated by the robot! :D
#define DATA_PIN 10

//Create integer variables for various things...
int ROBORIO = 4; //for the robot to communicate, sort of... (not implemented) [plan to use Serial (I2C) communication]
int COMMANDRECEIVED = 2; //for receiving a signal and doing a quick user feedback whenever a control for the robot itself is pressed (not implemented) [plan to use Serial (I2C) communication]
int scene1phase = 0; //incrementing variable for the first pattern
int timer = 25; //timer that controls how fast the scenes progress
int del = 20; //variable to store the time between each scene of the startup sequence (del is short for delay, but delay is a keyword)
bool startupleds = false; //to trigger the startup sequence

//Creates an array for all the individual leds on the strip
CRGB leds[NUM_LEDS];

void setup() {
  //RUNS ON STARTUP
      FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS); //Starts the LED library with the corresponding led strip type, data pin, and led amount.
      pinMode(ROBORIO, INPUT); //changes the pinMode to INPUT
      pinMode(COMMANDRECEIVED, INPUT); //changes the pinMode to INPUT
}

void STARTUP() {
  //INTRO SEQUENCE
      for (int x = 0; x < 26; x++) { //red chase sequence
        leds[x] = CRGB::Red;
        delay(del);
        FastLED.show();
      }
      for (int x = 26; x > 0; x--) { //orange chase sequence
        leds[x] = CRGB::Orange;
        delay(del);
        FastLED.show();
      }
      for (int x = 0; x < 26; x++) { //yellow chase sequence
        leds[x] = CRGB::Yellow;
        delay(del);
        FastLED.show();
      }
      for (int x = 26; x > 0; x--) { //green chase sequence
        leds[x] = CRGB::Green;
        delay(del);
        FastLED.show();
      }
      for (int x = 0; x < 26; x++) { //blue chase sequence
        leds[x] = CRGB::Blue;
        delay(del);
        FastLED.show();
      }
      for (int x = 26; x > 0; x--) { //purple chase sequence
        leds[x] = CRGB::Purple;
        delay(del);
        FastLED.show();
      }
      for (int x = 0; x < 26; x++) { //white chase sequence
        leds[x] = CRGB::White;
        delay(del);
        FastLED.show();
      }
    startupleds = true; //prevents the sequence from going again
}

void PATTERNS() { //function to handle patterns besides the main knightrider lights
  int newleds[18] = {0,1,2,3,4,5,6,7,8,17,18,19,20,21,22,23,24,25}; //an array of all leds besides the middle ones which are used for the knightrider sequence
  for (int x = 0; x < 9; x++) { //resets leds
    leds[x] = CRGB::Black;
  }
  for (int x = 18; x < 26; x++) { //resets leds
    leds[x] = CRGB::Black;
  }
  if (digitalRead(COMMANDRECEIVED) == LOW) { //rainbow pattern plays unless a command is received, in which case the pattern switches to the user feedback for a second
    leds[newleds[scene1phase + 5]] = CRGB::Red;
    leds[newleds[scene1phase + 4]] = CRGB::Orange;
    leds[newleds[scene1phase + 3]] = CRGB::Yellow;
    leds[newleds[scene1phase + 2]] = CRGB::Green;
    leds[newleds[scene1phase + 1]] = CRGB::Blue;
    leds[newleds[scene1phase]] = CRGB::Purple;
    scene1phase++;
    if (scene1phase >= 18) {
      scene1phase = 0;
    }
  }
  if (digitalRead(COMMANDRECEIVED) == HIGH) { //special feedback scene plays if a control has been pressed on the robot, to indicate the operation has gone through successfully
    //maybe all LEDs go green or something like that?
    for (int x = 0; x < 18; x++) { //sets all LEDs green
      leds[newleds[x]] = CRGB::Green;
    }
  }
}

void loop() {
    if (startupleds == false) {
      STARTUP(); //runs start-up sequence if it has not been run yet
    }
    for (int x = 0; x < 8; x++) { //knightrider red dot moves in one direction
      leds[x + 9] = CRGB::Red;
      PATTERNS(); //TRIGGERS THE OTHER PATTERN WITHIN THE SAME FRAME (see PATTERNS() above)
      FastLED.show();
      delay(timer);
      leds[x + 9] = CRGB::Black;
    }
    for (int x = 8; x > 0; x--) { //knightrider red dot moves in the other direction
      leds[x + 9] = CRGB::Red;
      PATTERNS(); //TRIGGERS THE OTHER PATTERN WITHIN THE SAME FRAME (see PATTERNS() above)
      FastLED.show();
      delay(timer);
      leds[x + 9] = CRGB::Black;
    }
}
