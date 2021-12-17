#pragma once
#ifndef MENU_H
#define MENU_H
#include "variables.h"

int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0';
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}

int writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.update(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}

void highscoreMemoryInit()
{
  String player1 = "RARES";
  String player2 = "ANDREI";

  byte addrOffsetPlayer1 = writeStringToEEPROM(currentEepromOffset, player1);
  highScoreRecords++;
  currentEepromOffset = addrOffsetPlayer1;

  byte addrOffsetPlayer2 = writeStringToEEPROM(currentEepromOffset, player2);
  highScoreRecords++;
  currentEepromOffset = addrOffsetPlayer1;
}

void printStartUp()
{
  lcd.setCursor(4, 0);
  lcd.print("Rush Hour");
  lcd.setCursor(0, 1);
  lcd.print("by Rares Mateiu");
}

void print_line(int line, String text)
{
  printLineTimer = millis();
  lcd.setCursor(0, line);
  lcd.print(text);
  lcd.print("               ");
  // lcd.setCursor(0, line);
}

void displaySettings()
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
        submenuScrollPosition = 3;
      }
    }
    if (joystick == up)
    {
      if (submenuScrollPosition < 3)
      {
        submenuScrollPosition++;
      }
      else
      {
        submenuScrollPosition = 0;
      }
    }

    String nume;
    if (joystick == enter)
    {
      switch (submenuScrollPosition)
      {
      case 0:
        print_line(0, "Your name");
        currentEepromOffset = 0; // ?
        int newOffset;
        newOffset = readStringFromEEPROM(currentEepromOffset, &nume);
        currentEepromOffset = newOffset;
        Serial.println(nume);
        print_line(1, nume);

        if (joystick == enter)
        {
          inSubmenu = !inSubmenu;
          currentMenuItem = 0;
          submenuScrollPosition = 0;
        }

        break;
      case 1:
        print_line(0, "Enter your name");
        print_line(1, "In serial");
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
              int newOffset;
              newOffset = writeStringToEEPROM(currentEepromOffset, inputString);
              currentEepromOffset = newOffset;
              break;
            }
          }
          inSubmenu = !inSubmenu;
          currentMenuItem = 0;
          submenuScrollPosition = 0;
        }
        break;
      case 2:
        print_line(0, "Level" + String(chosenLevel));
        joystick = readJoystick();
        if (joystick == right)
        {
          if (chosenLevel > 1)
          {
            chosenLevel--;
          }
        }
        if (joystick == left)
        {
          chosenLevel++;
        }
        break;
      case 3:
        inSubmenu = false;
        currentMenuItem = 0;
        submenuScrollPosition = 0 ;
      }
    }
    else
    {
      print_line(0, "Settings Menu");
      switch (submenuScrollPosition)
      {
      case 0:
        print_line(1, "Show name");
        break;
      case 1:
        print_line(1, "New name");
        break;
      case 2:
        print_line(1, "Set difficulty");
        break;
      case 3:
        print_line(1, "Quit submenu");
        break;
      }
    }
  }
}

void updateHighscoreArrays(int eepromStartingOffset)
{
  int lastOffset = eepromStartingOffset;
  String newStr;
  for (int i = 0; i < highScoreRecords; i++)
  {
    int newOffset;
    newOffset = readStringFromEEPROM(lastOffset, &newStr);
    lastOffset = newOffset;
    highScorePlayersArray[i] = newStr;
    highScoresArray[i] = 500;
  }
}

void displayHighscore()
{

  int highscoreEepromOffset = 0;
  updateHighscoreArrays(highscoreEepromOffset);
  int joystick = readJoystick();
  if (joystick != lastJoyRead)
  {
    lastJoyRead = joystick;
    if (joystick == up)
    {
      if (submenuScrollPosition > 0)
      {
        submenuScrollPosition--;
      }
      else
      {
        submenuScrollPosition = highScoreRecords - 1;
      }
    }
    if (joystick == down)
    {
      if (submenuScrollPosition < highScoreRecords)
      {
        submenuScrollPosition++;
      }
      else
      {
        submenuScrollPosition = 0;
      }
    }

    print_line(0, "Highscore");
    String nume = highScorePlayersArray[submenuScrollPosition];
    int highscore = highScoresArray[submenuScrollPosition];
    print_line(1, nume + ":" + String(highscore));

    if (joystick == enter)
    {
      inSubmenu = !inSubmenu;
      currentMenuItem = 1;
      submenuScrollPosition = 0;
    }
  }
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
      print_line(0, "Game");
      print_line(1, "Rush Hour");
      break;
    case 1:
      print_line(0, "Created By");
      print_line(1, "Mateiu Rares");
      break;
    case 2:
      print_line(0, "Link");
      print_line(1, "zzz");
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

void showMenu()
{
  
  if (!inSubmenu)
  {
    print_line(0, "Main Menu");
    switch (currentMenuItem)
    {
    case 0:
      print_line(1, "Start Game");
      break;
    case 1:
      print_line(1, "High Score");
      break;
    case 2:
      print_line(1, "Settings");
      break;
    case 3:
      print_line(1, "About");
      break;
    }
    int joystick = readJoystick();

    if (joystick != lastJoyRead)
    {
      lastJoyRead = joystick;
      if (joystick == right)
      {
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
  if (inSubmenu)
  {
    switch (currentMenuItem)
    {
    case 0:
      gameStarted = true;
      lastSpawnTime = millis();
      break;
    case 1:
      displayHighscore();
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
  if(lastNumberOfLives != numberOfLives)
    lcd.clear();
  if(lastNumberOfStars != numberOfStars)
    lcd.clear();
  for(int i=0; i < numberOfLives; i++){
    lcd.home();
    lcd.setCursor(i,0);
    lcd.write(byte(0));
  }
  for(int i=0; i < numberOfStars; i++){
    lcd.home();
    lcd.setCursor(i,1);
    lcd.write(byte(1));
  }
  lcd.setCursor(6,0);
  lcd.print("Level:");
  lcd.print(currentLevel);
  lcd.setCursor(6,1);
  lcd.print("Score:");
  lcd.print(score);
}

void EndMenu()
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
      if (endMenuOption == 1)
      {
        numberOfLives = 3;
        numberOfStars = 3;
        lastLevelChange = 0;
        scoreIncrement = 1;
        carSpeed = 600;
        botSpawnRate = 12.5 * carSpeed;
        isGameOver = false;
        gameStarted = true;
        didCheckLevel = false;
        ClearLastPlayerPosition();
        ClearLastBotPosition();
        xCar = 8;
        yCar = 2;
        xPlayer = 2;
        yPlayer = 5;
        levelReset = millis();
        score = 0;
        currentLevel = chosenLevel;
        lcd.clear();
      }
      if (endMenuOption == 0)
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
        ClearLastPlayerPosition();
        ClearLastBotPosition();
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
