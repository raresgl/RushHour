#pragma once
#ifndef MENU_H
#define MENU_H
#include "variables.h"

void printStartUp()
{
  lcd.setCursor(4, 0);
  lcd.print("Rush Hour");
  lcd.setCursor(0, 1);
  lcd.print("by Rares Mateiu");
}

void printLine(int line, String text)
{
  printLineTimer = millis();
  lcd.setCursor(0, line);
  lcd.print(text);
  lcd.print("               ");
}

void displaySettings()
{
  int joystick = readJoystick();

  if (joystick != lastJoyRead && !inSecondSubmenu)
  {
    lastJoyRead = joystick;

    if (joystick == up)
    {
      if (submenuScrollPosition > 0)
      {
        tone(buzzerPin, 100, 200);
        submenuScrollPosition--;
      }
    }
    if (joystick == down)
    {
      if (submenuScrollPosition < 7)
      {
        tone(buzzerPin, 300, 200);
        submenuScrollPosition++;
      }
    }
    printLine(0, "Settings Menu");
    switch (submenuScrollPosition)
    {
    case 0:
      printLine(1, "Show name");
      break;
    case 1:
      printLine(1, "New name");
      break;
    case 2:
      printLine(1, "Set difficulty");
      break;
    case 3:
    {
      printLine(1, "Contrast");
      break;
    }
    case 4:
    {
      printLine(1, "Screen Light");
      break;
    }
    case 5:
    {
      printLine(1, "Matrix Light");
      break;
    }
    case 6:
      printLine(1, "Reset memory");
      break;
    case 7:
      printLine(1, "Quit submenu");
      break;
    }

    if (joystick == enter)
    {
      lcd.clear();
      inSecondSubmenu = !inSecondSubmenu;
    }
  }
  if (inSecondSubmenu)
  {

    switch (submenuScrollPosition)
    {
    case 0:
    { // show name
      printLine(0, "Your name");
      readStringFromEEPROM(eepromPlayerNameStartAddress, &playerName);
      printLine(1, playerName);

      joystick = readJoystick();
      Serial.println(joystick);
      if (joystick == enter)
      {
        lastJoyRead = joystick;
        inSecondSubmenu = !inSecondSubmenu; // !inSecondSubmenu;
        lcd.clear();
      }
      break;
    }
    case 1: // enter name
    {
      printLine(0, "Enter your name");
      printLine(1, "In serial");

      joystick = readJoystick();
      if (joystick == enter)
      {
        while (Serial.available() > 0)
        {
          incomingByte = (char)Serial.read();
          inputString += incomingByte;
          Serial.print("I received: ");
          Serial.println(incomingByte);

          if (incomingByte == '\n')
          {
            stringComplete = true;
            Serial.println(inputString);
            writeStringToEEPROM(eepromPlayerNameStartAddress, inputString);
            break;
          }
        }
        inSecondSubmenu = !inSecondSubmenu;
        lcd.clear();
        submenuScrollPosition = 1;
      }
      break;
    }
    case 2: // difficulty
    {
      printLine(1, "              ");
      printLine(0, "Level" + String(chosenLevel));
      joystick = readJoystick();
      if (joystick != lastJoyRead)
      {
        lastJoyRead = joystick;
        if (joystick == left)
        {
          if (chosenLevel > 1)
          {
            chosenLevel--;
          }
        }
        if (joystick == right)
        {
          if (chosenLevel < 7)
          {
            chosenLevel++;
          }
        }
        if (joystick == enter)
        {
          inSecondSubmenu = !inSecondSubmenu;
          submenuScrollPosition = 2;
          writeIntIntoEEPROM(eepromChosenLevelAddress, chosenLevel);
          lcd.clear();
        }
      }
      break;
    }
    case 3: // contrast settings
    {
      int xValue = analogRead(xPin);

      if (xValue > maxThreshold)
      {
        if (LCDContrast < 255)
        {

          LCDContrast++;
          delay(15);
        }
      }

      if (xValue < minThreshold)
      {
        if (LCDContrast > 0)
        {
          LCDContrast--;
          delay(15);
        }
      }
      analogWrite(contrastPin, LCDContrast);
      lcd.setCursor(0, 0);
      lcd.print("Contrast: ");
      lcd.print(LCDContrast);
      lcd.print("      ");

      joystick = readJoystick();
      if (joystick == enter)
      {
        inSecondSubmenu = !inSecondSubmenu;
        submenuScrollPosition = 3;
        writeIntIntoEEPROM(eepromContrastAddress, LCDContrast);
        lcd.clear();
      }

      break;
    }
    case 4: // screen light
    {
      int xValue = analogRead(xPin);

      if (xValue > maxThreshold)
      {
        if (screenLight < 1400)
        {

          screenLight++;
          delay(15);
        }
      }

      if (xValue < minThreshold)
      {
        if (screenLight > 700)
        {
          screenLight--;
          delay(15);
        }
      }

      analogWrite(screenLightPin, screenLight);
      lcd.setCursor(0, 0);
      lcd.print("BRIGHTNESS: ");
      lcd.print(screenLight);
      lcd.print("      ");
      lcd.setCursor(0, 1);
      lcd.print("Screen");

      joystick = readJoystick();
      if (joystick == enter)
      {
        inSecondSubmenu = !inSecondSubmenu;
        submenuScrollPosition = 4;

        writeIntIntoEEPROM(eepromScreenBrightnessAddress, screenLight);
        lcd.clear();
      }

      break;
    }
    case 5: // matrix light
    {

      for (int i = 0; i < matrixSize; i++)
      {
        for (int j = 0; j < matrixSize; j++)
        {
          lc.setLed(0, i, j, true);
        }
      }

      int xValue = analogRead(xPin);
      if (xValue > maxThreshold)
      {
        if (matrixLight < 15)
        {
          matrixLight++;
          delay(100);
        }
      }

      if (xValue < minThreshold)
      {
        if (matrixLight > 0)
        {
          matrixLight--;
          delay(100);
        }
      }
      lc.setIntensity(0, matrixLight);

      lcd.setCursor(0, 0);
      lcd.print("BRIGHTNESS: ");
      lcd.print(matrixLight);
      lcd.print("      ");
      lcd.setCursor(0, 1);
      lcd.print("Matrix");

      joystick = readJoystick();
      if (joystick == enter)
      {
        inSecondSubmenu = !inSecondSubmenu;
        submenuScrollPosition = 5;
        lcd.clear();
        writeIntIntoEEPROM(eepromMatrixBrightnessAddress, matrixLight);
        lc.clearDisplay(0); // clear screen
      }

      break;
    }
    case 6: // reset memory
    {
      printLine(0, "RESETTING ALL");
      lcd.setCursor(0, 1);
      lcd.write("Loading.");
      for (int i = 1; i <= 6; i++)
      {
        for (int j = (eepromSize / 6) * (i - 1); j <= (eepromSize / 6) * i; j++)
        {
          EEPROM.write(j, 0);
        }
        lcd.write(".");
        delay(100);
      }
      appInitSuccessfully = 0;
      writeIntIntoEEPROM(eepromAppInitSuccessfullyAddress, appInitSuccessfully);
      resetFunc();
    }
    case 7: // exit
    {
      inSubmenu = false;
      currentMenuItem = 0;
      submenuScrollPosition = 0;
      break;
    }
    }
  }
}

void displayHighScore()
{
  highScoreRecords = readIntFromEEPROM(eepromHighScoreRecordsAddress);
  if (highScoreRecords > 3 || highScoreRecords < 0)
  {
    highScoreRecords = 0;
  }

  if (highScoreRecords == 0)
  {
    printLine(0, "NO HIGHSCORES");
    lcd.setCursor(0, 1);
    lcd.write("Returning.");
    for (int i = 0; i < 6; i++)
    {
      lcd.write(".");
      delay(500);
    }
    inSubmenu = !inSubmenu;
    currentMenuItem = 1;
    submenuScrollPosition = 0;
    currentHighScorePosition = 0;
  }
  else
  {
    int joystick = readJoystick();
    if (joystick != lastJoyRead)
    {
      lastJoyRead = joystick;

      if (joystick == up)
      {
        if (currentHighScorePosition > 0)
        {
          currentHighScorePosition--;
        }
        else
        {
          currentHighScorePosition = highScoreRecords - 1;
        }
      }
      if (joystick == down)
      {
        if (currentHighScorePosition < highScoreRecords - 1)
        {
          currentHighScorePosition++;
        }
        else
        {
          currentHighScorePosition = 0;
        }
      }
      else if (joystick == enter)
      {
        inSubmenu = !inSubmenu;
        currentMenuItem = 1;
        submenuScrollPosition = 0;
        currentHighScorePosition = 0;
      }
    }
  }
  String name1, name2, currentName;
  int currentScore;
  currentScore = readIntFromEEPROM(eepromHighScoreStartAddress + currentHighScorePosition * sizeof(int));
  byte length1, length2;

  if (currentHighScorePosition == 0)
  {
    readStringFromEEPROM(eepromPlayerNameStartAddress, &currentName);
  }
  else if (currentHighScorePosition == 1)
  {
    length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, &name1);
    readStringFromEEPROM(eepromPlayerNameStartAddress + length1, &currentName);
  }
  else if (currentHighScorePosition == 2)
  {
    length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, &name1);
    length2 = readStringFromEEPROM(eepromPlayerNameStartAddress + length1, &name2);
    readStringFromEEPROM(eepromPlayerNameStartAddress + length1 + length2, &currentName);
  }

  lcd.setCursor(0, 0);
  lcd.print("High scores ");
  lcd.setCursor(13, 0);
  if (currentHighScorePosition == 0)
  {
    lcd.print("1st");
  }
  else if (currentHighScorePosition == 1)
  {
    lcd.print("2nd");
  }
  else
  {
    lcd.print("3rd");
  }
  lcd.setCursor(0, 1);
  lcd.print(currentName);
  lcd.print(" Score");
  lcd.print(currentScore);
  lcd.print("          ");
}

void displayAbout()
{
  int joystick = readJoystick();
  if (joystick != lastJoyRead)
  {
    lastJoyRead = joystick;
    if (joystick == down)
    {
      if (submenuScrollPosition > 0)
      {
        submenuScrollPosition--;
      }
      else
      {
        submenuScrollPosition = 2;
      }
    }
    if (joystick == up)
    {
      if (submenuScrollPosition < 2)
      {
        submenuScrollPosition++;
      }
      else
      {
        submenuScrollPosition = 0;
      }
    }
    switch (submenuScrollPosition)
    {
    case 0:
      printLine(0, "Game");
      printLine(1, "Rush Hour");
      break;
    case 1:
      printLine(0, "Created By");
      printLine(1, "Mateiu Rares");
      break;
    case 2:
      printLine(0, "Link");
      printLine(1, "zzz");
      break;
    }
    if (joystick == enter)
    {
      inSubmenu = !inSubmenu;
      currentMenuItem = 0;
      submenuScrollPosition = 0;
    }
  }
}

void resetGame()
{
  numberOfLives = 3;
  numberOfStars = 3;
  lastLevelChange = 0;
  scoreIncrement = 1;
  carSpeed = 600;
  botSpawnRate = 12.5 * carSpeed;
  isGameOver = false;
  didCheckLevel = false;
  clearLastPlayerPosition();
  clearLastBotPosition();
  xCar = 8;
  yCar = 2;
  xPlayer = 2;
  yPlayer = 5;
  levelReset = millis();
  score = 0;
  currentLevel = chosenLevel;
  lcd.clear();
}

void showMenu()
{

  if (!inSubmenu)
  {
    printLine(0, "Main Menu");
    switch (currentMenuItem)
    {
    case 0:
      printLine(1, "Start Game");
      break;
    case 1:
      printLine(1, "High Score");
      break;
    case 2:
      printLine(1, "Settings");
      break;
    case 3:
      printLine(1, "About");
      break;
    }
    int joystick = readJoystick();

    if (joystick != lastJoyRead)
    {
      lastJoyRead = joystick;
      if (joystick == right)
      {

        tone(buzzerPin, 300, 200);
        if (currentMenuItem < 3)
        {
          currentMenuItem++;
        }
        else
        {
          currentMenuItem = 0;
        }
      }
      if (joystick == left)
      {

        tone(buzzerPin, 100, 200);
        if (currentMenuItem > 0)
        {
          currentMenuItem--;
        }
        else
        {
          currentMenuItem = 3;
        }
      }
      if (joystick == enter)
      {
        inSubmenu = true;
      }
    }
  }
  else if (inSubmenu)
  {
    switch (currentMenuItem)
    {
    case 0:
      resetGame();
      gameStarted = true;
      break;
    case 1:
      displayHighScore();
      break;
    case 2:
      displaySettings();
      break;
    case 3:
      displayAbout();
      break;
    }
  }
}

void lcdGame()
{
  lcd.createChar(0, heartShape);
  lcd.createChar(1, starShape);
  if (lastNumberOfLives != numberOfLives)
    lcd.clear();
  if (lastNumberOfStars != numberOfStars)
    lcd.clear();
  for (int i = 0; i < numberOfLives; i++)
  {
    lcd.home();
    lcd.setCursor(i, 0);
    lcd.write(byte(0));
  }
  for (int i = 0; i < numberOfStars; i++)
  {
    lcd.home();
    lcd.setCursor(i, 1);
    lcd.write(byte(1));
  }
  lcd.setCursor(6, 0);
  lcd.print("Level:");
  lcd.print(currentLevel);
  lcd.setCursor(6, 1);
  lcd.print("Score:");
  lcd.print(score);
}

void endMenu()
{
  lcd.setCursor(0, 0);
  lcd.print("SCORE:");
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("TIME:");
  lcd.print(elapsedTime - (levelReset / 1000));
  lcd.setCursor(11, 1);
  lcd.print("RETRY");
  lcd.setCursor(11, 0);
  lcd.print("MENU");
  lcd.setCursor(10, endMenuOption);
  lcd.print(">");

  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  int joystick = readJoystick();
  if (joystick == down)
  {
    if (endMenuOption < 1)
    {
      endMenuOption += 1;
      lcd.clear();
    }
  }
  if (joystick == up)
  {
    if (endMenuOption > 0)
    {
      endMenuOption -= 1;
      lcd.clear();
    }
  }

  if (joystick == enter)
  {
    if (endMenuOption == 1) // retry
    {
      resetGame();
      gameStarted = true;
    }
    if (endMenuOption == 0) // MENU
    {
      numberOfLives = 3;
      numberOfStars = 3;
      lastLevelChange = 0;
      scoreIncrement = 1;
      EEPROM.get(0, highScore);
      didCheckLevel = false;
      // inMainMenu = true;
      inSubmenu = false;

      carSpeed = 600;
      botSpawnRate = 12.5 * carSpeed;
      gameStarted = false;
      isGameOver = false;
      clearLastPlayerPosition();
      clearLastBotPosition();
      xCar = 8;
      yCar = 2;
      xPlayer = 2;
      yPlayer = 5;
      levelReset = millis();
      score = 0;
      currentLevel = 1;
      lc.clearDisplay(0);
      lcd.clear();
    }
  }
}

#endif
