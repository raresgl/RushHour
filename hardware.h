#pragma once
#ifndef HARDWARE_H
#define HARDWARE_H

#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

// joystick values
int xValue = 0;
int yValue = 0;
const int minThreshold = 300;
const int maxThreshold = 700;
int swState = HIGH;
int lastSwState = HIGH;
bool selector = false;


//defining the lcd pins
const int contrastPin = 3; 
const int lcdRs = 8; 
const int lcdE = 9; 
const int lcdD4 = 4; 
const int lcdD5 = 5; 
const int lcdD6 = 6;
const int lcdD7 = 7; 

const int screenLightPin = 2; // TO DO

//defining matrix pins
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10; 
const int matrixSize = 8; 

//defining joystick pins
const int xPin = A1; 
const int yPin = A0; 
const int swPin = 2; 

unsigned long lastPress = 0;
const int debounceDelay = 250;

//lcd values
int contrastValue = 100;
const int lcdWidth = 16;
const int lcdHeight = 2;
int matrixLight = 2;
int screenLight = 1000; 




LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
LiquidCrystal lcd = LiquidCrystal(lcdRs, lcdE, lcdD4, lcdD5, lcdD6, lcdD7);

void hardware_setup()
{

  pinMode(swPin, INPUT_PULLUP);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(contrastPin, OUTPUT);
  analogWrite(contrastPin, contrastValue);
  // TO INSTALL hardware
    /*analogWrite(screenLightPin,screenLight)*/ 
  lcd.begin(lcdWidth, lcdHeight);

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixLight); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);    // clear screen

  // EEPROM.get(0, highScore);
  // pinMode(pinBacklight, OUTPUT);
}

//Joystick values
#define up 0
#define right 1
#define down 2
#define left 3
#define enter 4
#define none 5

int readJoystick()
{
  int output = none;
  // read all joystick values
  int xValue = analogRead(xPin); // read the x axis value
  int yValue = analogRead(yPin); // read the y axis value
  // Y_Axis = map(Y_Axis, 0, 1023, 1023, 0);      // invert the input from the y axis so that pressing the stick forward gives larger values
  int SwitchValue = digitalRead(swPin);       // read the state of the switch
  SwitchValue = map(SwitchValue, 0, 1, 1, 0); // invert the input from the switch to be high when pressed


  if (SwitchValue == 1 && millis() - lastPress > debounceDelay)
  {
    lastPress = millis();
    output = enter;
    
  }


  else if (xValue >= maxThreshold)
  {
    output = right;
  }
  else if (xValue <= minThreshold)
  {
    output = left;
  }
  else if (yValue >= maxThreshold)
  {
    output = up;
  }
  else if (yValue <= minThreshold)
  {
    output = down;
  }
  return output;
}

#endif
