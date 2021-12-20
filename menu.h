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

  if (joystick != lastJoyRead && !inSecondSubmenu)
  {
    lastJoyRead = joystick;

    if (joystick == up)
    {
      if (submenuScrollPosition > 0)
      {
        submenuScrollPosition--;
      }
    }
    if (joystick == down)
    {
      if (submenuScrollPosition < 6)
      {
        submenuScrollPosition++;
      }
    }
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
    {
      print_line(1, "Contrast");
      break;
    }
    case 4:
    {
      print_line(1, "Screen Light");
      break;
    }
    case 5:
    {
      print_line(1, "Matrix Light");
      break;
    }
    case 6:
      print_line(1, "Quit submenu");
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

    String nume;

    switch (submenuScrollPosition)
    {
      case 0:
      { // show name
        print_line(0, "Your name");
        currentEepromOffset = 0; // ?
        int newOffset;
        newOffset = readStringFromEEPROM(currentEepromOffset, &nume);
        currentEepromOffset = newOffset;
        print_line(1, nume);

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
        print_line(0, "Enter your name");
        print_line(1, "In serial");

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
              int newOffset;
              newOffset = writeStringToEEPROM(currentEepromOffset, inputString);
              currentEepromOffset = newOffset;
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
        print_line(1, "              ");
        print_line(0, "Level" + String(chosenLevel));
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


        // TO INSTALL hardware
          /* analogWrite(screenLightPin, screenLight); */
        lcd.setCursor(0, 0);
        lcd.print("BRIGHTNESS: ");
        lcd.print(screenLight);
        lcd.print("      ");
        lcd.setCursor(0,1);
        lcd.print("Screen");

        joystick = readJoystick();
        if (joystick == enter)
        {
          inSecondSubmenu = !inSecondSubmenu;
          submenuScrollPosition = 4;
          lcd.clear();
        }
        
        break;
      }
      case 5: // matrix light
      {
       
        for(int i=0;i<matrixSize;i++) {
          for(int j=0;j<matrixSize;j++) {
            lc.setLed(0,i,j,true);
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
        lcd.setCursor(0,1);
        lcd.print("Matrix");

        joystick = readJoystick();
        if (joystick == enter)
        {
          inSecondSubmenu = !inSecondSubmenu;
          submenuScrollPosition = 5;
          lcd.clear();
          lc.clearDisplay(0);    // clear screen
        }
        
        break;
      }
      case 6: // exit
      {
        inSubmenu = false;
        currentMenuItem = 0;
        submenuScrollPosition = 0;
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
  else if (inSubmenu)
  {
    switch (currentMenuItem)
    {
    case 0:
      resetGame();
      gameStarted = true;
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
