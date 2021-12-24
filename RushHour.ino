#include "hardware.h"
#include "variables.h"
#include "menu.h"
#include "game.h"

void setup()
{
  appInitSuccessfully = readIntFromEEPROM(eepromAppInitSuccessfullyAddress);
  // Init only when asked
  if (!appInitSuccessfully)
  {
    initEEPROM();
  }
  LCDContrast = readIntFromEEPROM(eepromContrastAddress);
  screenLight = readIntFromEEPROM(eepromScreenBrightnessAddress);
  matrixLight = readIntFromEEPROM(eepromMatrixBrightnessAddress);
  chosenLevel = readIntFromEEPROM(eepromChosenLevelAddress);
  readStringFromEEPROM(eepromPlayerNameStartAddress, &playerName);
  
  hardware_setup();
  Serial.begin(9600);
}

void loop()
{

  if (startUp == true)
  {
    printStartUp();
  }
  if (millis() > startUpTime)
  {
    startUp = false;
  }
  if (startUp == false)
  {
    // analogWrite(pinBacklight, brightness);
    if (gameStarted == true)
    {
      if (isGameOver == false)
        runGame();
      else
        endMenu();
    }
    else
      showMenu();
  }
}
