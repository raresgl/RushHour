#include "hardware.h"
#include "variables.h"
#include "menu.h"
#include "game.h"

void setup()
{
  hardware_setup();
  Serial.begin(9600);
  highscoreMemoryInit();
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
    Serial.println(gameStarted);
    // analogWrite(pinBacklight, brightness);
    if (gameStarted == true)
    {
      if (isGameOver == false)
        RunGame();
      else
        EndMenu();
    }
    else
      showMenu();
  }
}
