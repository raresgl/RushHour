#pragma once
#ifndef GAME_H
#define GAME_H
#include "menu.h"

//this function will spawn a bot at the designated coordinates, modifying the collision matrix
void SpawnBot()     
{                    
  lc.setLed(0,xCar, yCar, true);
  lc.setLed(0,xCar - 1, yCar, true);
  lc.setLed(0,xCar, yCar + 1, true);
  lc.setLed(0,xCar, yCar - 1, true);
  lc.setLed(0,xCar + 1, yCar, true);
  lc.setLed(0,xCar + 2, yCar, true);
  lc.setLed(0,xCar + 2, yCar + 1, true);
  lc.setLed(0,xCar + 2, yCar - 1, true);
//i want to check if the bot is in the matrix 
  if(7-xCar <= 7 && 7- xCar >= 0){
    matrix[7-xCar][7-yCar] = 1;
    matrix[7-xCar][7-(yCar + 1)] = 1;
    matrix[7-xCar][7-(yCar - 1)] = 1;
  }
  if(7-(xCar - 1) <= 7 && 7- xCar >= 0)
    matrix[7-(xCar - 1)][7-yCar] = 1;
  if(7-(xCar + 1) <= 7 && 7 - (xCar + 1) >= 0)
    matrix[7-(xCar + 1)][7-yCar] = 1;
  if(7-(xCar + 2) <= 7 && 7 - (xCar + 2) >= 0){
    matrix[7-(xCar + 2)][7-yCar] = 1;
    matrix[7-(xCar + 2)][7-(yCar + 1)] = 1;
    matrix[7-(xCar + 2)][7-(yCar - 1)] = 1;
  }

}

//chooses randomly if the bot should spawn from the right or the left side
void CarSpawner()         
{
  xCar = defaultXCar;
  int randomNumber = random(0,3);
  if(randomNumber % 2 == 0)
    yCar = 5;
  else
    yCar = 2;
  SpawnBot();
}

void CreateWalls()
{
  int row = 0;
  for (int col = 0; col < maxColumns; col++)
  {
    lc.setLed(0, col, row, true); // turns on LED at col, row
  }
  row = 7;
  for (int col = 0; col < maxColumns; col++)
  {
    lc.setLed(0, col, row, true); // turns on LED at col, row
  }
}

void SpawnPlayer()
{
  lc.setLed(0,xPlayer, yPlayer, true);
  lc.setLed(0,xPlayer + 1, yPlayer, true);
  lc.setLed(0,xPlayer, yPlayer + 1, true);
  lc.setLed(0,xPlayer, yPlayer - 1, true);
  lc.setLed(0,xPlayer - 1, yPlayer, true);
  lc.setLed(0,xPlayer - 2, yPlayer, true);
  lc.setLed(0,xPlayer - 2, yPlayer + 1, true);
  lc.setLed(0,xPlayer - 2, yPlayer - 1, true);
}

//printing on lcd the after game credits ()
void endGame() 
{
  isGameOver = true;
  elapsedTime = millis() / 1000;
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER");
  endTime = millis();
  while (millis() - endTime < 1000)
  lcd.clear();
  if (highScore < score)
  { 
  // saving the highscore if needed
    EEPROM.put(0, score);
    lcd.setCursor(0, 0);
    lcd.print("You got a ");
    lcd.setCursor(2, 1);
    lcd.print("high score!");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("You were ");
    lcd.print(highScore - score);
    lcd.setCursor(2, 1);
    lcd.print("points behind");
  }
  endTime = millis();
  while (millis() - endTime < 3000)
  lcd.clear();
}

void CheckCollision()               
{
  if(takenDmg == false)
    if(matrix[7-xPlayer][7-yPlayer] == 1 || matrix[7-(xPlayer+1)][7-yPlayer] == 1 || matrix[7-(xPlayer-1)][7-yPlayer] == 1 || matrix[7-(xPlayer-2)][7-yPlayer] == 1)
      if(numberOfLives > 1){
        numberOfLives -= 1;
        takenDmg = true;
        dmgTimer = millis();
      }
      else{
        numberOfLives = 0;
        lcdGame();       
        endGame();
      }
  if(takenDmg == true){ 
  // the player will blink so he knows he is invincible after taking damage           
    ClearLastPlayerPosition();
    if(millis() - lastBlink >= 500){
      SpawnPlayer();
      lastBlink = millis();
    }
  }
  if(millis() - dmgTimer >= 2000)
    takenDmg = false;

}

void RunGame() // the function that is running the game
{
  SpawnPlayer();
  CreateWalls();
  if (didCheckLevel == false)
    if (defaultLevel != currentLevel)
    {
      shouldChangeLevel = true;
      currentLevel--;
      didCheckLevel = true;
    }
    else
      didCheckLevel = true;
  if (((millis() - levelReset - lastLevelChange) >= levelTimer && currentLevel < 7) || shouldChangeLevel == true)
  { // i want the level to change based on seconds survived
    currentLevel++;
    shouldChangeLevel = false;
    lastLevelChange = millis() - levelReset;
    switch (currentLevel)
    {
    case 0:
      break;
    case 1:
      carSpeed = case1CarSpeedVal;
      botSpawnRate = multiplier * carSpeed;
      levelTimer = defaultLevelTimer;
      scoreIncrement = 1;
      break;
    case 2:
      carSpeed = case2CarSpeedVal;
      botSpawnRate = multiplier * carSpeed;
      scoreIncrement++ ;
      break;
    case 3:
      carSpeed = case3CarSpeedVal;
      levelTimer += timerIncrementValue;
      botSpawnRate = multiplier * carSpeed;
      scoreIncrement++ ;
      break;
    case 4:
      carSpeed = case4CarSpeedVal;
      levelTimer += timerIncrementValue;
      botSpawnRate = multiplier * carSpeed;
      scoreIncrement++ ;
      break;
    case 5:
      carSpeed = case5CarSpeedVal;
      levelTimer += timerIncrementValue;
      botSpawnRate = multiplier * carSpeed;
      scoreIncrement++ ;
      break;
    case 6:
      carSpeed = case6CarSpeedVal;
      botSpawnRate = multiplier * carSpeed;
      scoreIncrement++ ;
      levelTimer = defaultLevelTimer;
      break;
    case 7:
      carSpeed = case7CarSpeedVal;
      botSpawnRate = multiplier * carSpeed;
      scoreIncrement++ ;
      break;
    }
  }
  if (millis() - lastIncrementTime >= 1000)
  { //incrementing the score every second
    score += scoreIncrement;
    lastIncrementTime = millis();
  }
  if (millis() - lastRefreshTime >= carSpeed)
  { //moving the bot cars based on their speed
    ClearLastBotPosition();
    xCar -= 1;
    lastRefreshTime = millis();
  }
  if (millis() - lastSpawnTime >= botSpawnRate && shouldCreateBot == true)
  { //checking to see if i need to create another car
    CarSpawner();
    shouldCreateBot = false;
    lastSpawnTime = millis();
  }
  SpawnBot();
  swState = digitalRead(swPin);
  if (isInvincible == false)
  { //code for the invincibility
    swState = digitalRead(swPin);
    if (swState != lastSwState)
    {
      if (swState == HIGH && numberOfStars > 0)
      {
        isInvincible = true;
        numberOfStars--;
        startInvincibility = millis();
      }
    }
    CheckCollision();
  }
  else
  {
    if (millis() - startInvincibility >= 2000)
      isInvincible = false;
    ClearLastPlayerPosition();
    if (millis() - lastBlink >= 500)
    {
      SpawnPlayer();
      lastBlink = millis();
    }
  }
  lastSwState = swState;
  if (isGameOver == false)
    lcdGame(); //updating the UI everytime
  lastNumberOfLives = numberOfLives;
  lastNumberOfStars = numberOfStars;
  if (xCar <= -3)
    shouldCreateBot = true;
  int joystick = readJoystick();

  if(joystick == up)
  { //code for moving the player
    if (xPlayer > 2)
    {
      ClearLastPlayerPosition();
      xPlayer -= 1;
    }
    else
    {
      xPlayer = xPlayer;
    }
  }
  if(joystick == down)
  {
    if (xPlayer < 6)
    {
      ClearLastPlayerPosition();
      xPlayer += 1;
    }
    else
    {
      xPlayer = xPlayer;
    }
  }
   if(joystick == left)
  {
    if (yPlayer < 3)
    {
      ClearLastPlayerPosition();
      yPlayer += 3;
    }
    else
    {
      yPlayer = yPlayer;
    }
  }
   if(joystick == right)
  {
    if (yPlayer > 3)
    {
      ClearLastPlayerPosition();
      yPlayer -= 3;
    }
    else
    {
      yPlayer = yPlayer;
    }
  }
}

#endif
