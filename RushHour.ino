#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

//defining the lcd pins
const int contrastPin = 3;
const int lcdRs = 8;
const int lcdE = 9;
const int lcdD4 = 4;
const int lcdD5 = 5;
const int lcdD6 = 6;
const int lcdD7 = 7;

//defining matrix pins
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int matrixSize = 8;

//defining joystick pins
const int xPin = A0;
const int yPin = A1;
const int swPin = 2;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
LiquidCrystal lcd = LiquidCrystal(lcdRs, lcdE, lcdD4, lcdD5, lcdD6, lcdD7);

bool matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};
byte starShape[] = {
  B00100,
  B00100,
  B01110,
  B11111,
  B01110,
  B00100,
  B00100,
  B00000
};

byte heartShape[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

// joystick values
int xValue = 0;
int yValue = 0;
int counter = 0;
int minThreshold = 300;
int maxThreshold = 700;
int swState = HIGH;
int lastSwState = HIGH;
bool joyMoved = false;
bool selector = false;


//lcd values
const int contrastValue = 100;
const int lcdWidth = 16;
const int lcdHeight = 2;

//values for the game menu
bool startUp = true;
int startUpTime = 3000;
int timer = 0;
int sectionNum = 0; //what section will be selected(highscore, play, etc)
bool selected = false;
bool dpState = false;


//game values
unsigned int highscore = 0;
bool checkDifficulty = false;
bool shouldChangeLevel = false;
bool spawnCar = false; //should spawn a new car
bool isInvincible = false; //the player cant be hit for a couple of seconds after he takes damage
bool isGameOver = false;
bool takenDmg = false; //verifies if the collision happened in order to make the player invincible
const int defaultSpeedValue = 600;
const int multiplier = 12;
const int decreaseValue = 70;
const int defaultTimeValue = 15000; //you will need to resist more time in the higher levels
const int invincibilityEnd = 2500;
int startingLevel = 1;
int currentLevel = 1;
int points = 1; //the harder the game gets the more points you get
int carSpeed = 600; //how fast the car moves (it goes backwards)
int carSpawnRate = 0;
int score = 0;
int lastBlink = 0;
int numberOfStars = 3; //the player has 3 invincibility stars so that he is imune to collision after he takes dmg
int numberOfLives = 3; //the player has 3 lives
int lastNumberOfLives = 3;
int lastNumberOfStars = 3;
unsigned int pointsTimer = 0; //you get points every second survived
unsigned int resetLevelTimer = 0; 
unsigned int lastLevelChange = 0;
unsigned int levelTimer = 15000;
unsigned int botUpdateTimer = 0;
unsigned int lastSpawnTime = 0;
unsigned int startInvincibility = 0;
unsigned int dmgTimer = 0;
unsigned int secondsPlayed = 0;

//end of the game values
unsigned int endTime = 0;

//matrix position values
int xCar = 0;
int yCar = 0;
int defaultXCar = 9;
int xPlayer = 0;
int yPlayer = 0;

//values for EEPROM
String inputString = "";
char incomingByte = 0;
bool stringComplete = false;

void setup()
{
  pinMode(swPin, INPUT_PULLUP);
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(contrastPin, OUTPUT);
  analogWrite(contrastPin, contrastValue);
  lcd.begin(lcdWidth, lcdHeight);
  
  Serial.begin(9600);
}
void loop() {
  if(startUp == true){
    lcd.setCursor(4, 0);
    lcd.print("Rush Hour");
    lcd.setCursor(0,1);
    lcd.print("by Rares Mateiu");
  }
  if(millis() - timer > startUpTime){
    startUp = false;
  }
  if(startUp == false){
    lcd.setCursor(0, 0);
    lcd.print("Main Menu");
  
    xValue = analogRead(xPin);
    //Serial.println(xValue);   
    counter = joystickSelector(counter);
    swState = digitalRead(swPin);
  
    if(swState != lastSwState && swState == LOW){
        selected = !selected;
        selector = 0;
        delay(10);
      }
      lcdDisplay(counter);
      delay(100);
      lastSwState = swState; 
  }
}
//function to scroll trough the menu
int joystickSelector(int counter) {      
  if(xValue < minThreshold && joyMoved == false && !selected) {
    if(counter < 3) {
      counter++;
    }
    else {
      counter = 0;
    }
    joyMoved = true;
  }
  if(xValue > maxThreshold && joyMoved == false && !selected) {
    if(counter > 0) {
      counter--;
    }
    else {
      counter = 3;
    }
    joyMoved = true;
  }
  if(xValue >= minThreshold && xValue <= maxThreshold && !selected) {
    joyMoved = false;
  }
  return counter;
}
//function on what to display when selected
void lcdDisplay(int option){
  if(!selected) {
    gameOptions(sectionNum);
  }
  if(selected) {
    switch(counter) {
    case 0:
      lcdGame();
      RunGame();
      break;
    case 1:
      displayHighscore();
      break;
    case 2:
      delay(100);
      selector=0;
      displaySettings(selector);
      break;
    case 3:
      delay(100);
      selector=0;
      displayAbout(selector);
      break;
    }
  }
}

//function on what to display if not selected
void gameOptions(int option) {
  switch(counter) {
    case 0: 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Main Menu");
      lcd.setCursor(0,1);
      lcd.print("Start Game");
      break;
    case 1: 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Main Menu");
      lcd.setCursor(0,1);
      lcd.print("Highscore");
      break;
    case 2: 
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Main Menu");
      lcd.setCursor(0,1);
      lcd.print("Settings");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Main Menu");
      lcd.setCursor(0,1);
      lcd.print("About");
      break;  
  }
}
//settings menu
void displaySettings(int selector)
{
  while(true)
   {
    yValue = analogRead(yPin);
    swState = digitalRead(swPin);
    if(yValue > maxThreshold && joyMoved == false) {
        if(selector > 0){
          selector--;
        }
        else {
          selector = 2;
        }
        joyMoved = true;
      }
      if(yValue < minThreshold && joyMoved == false) {
        if(selector < 2) {
         selector++;
        }
        else {
          selector = 0;
        }
        joyMoved = true;
      }
      if(yValue >= minThreshold && yValue <= maxThreshold ) {
        joyMoved = false;
      }
      switch(selector){
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Show name");
          break;
        case 1: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("New name");
          break;
        case 2: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Settings Menu");
          lcd.setCursor(0,1);
          lcd.print("Set difficulty");
          break;
      }
    delay(100);
    if( swState != lastSwState && swState == LOW) {
      dpState = !dpState;
      break;
      } 
    lastSwState = swState;
      
    switch(selector) {
      case 0:
      while(true) {
        delay(100);
        swState = digitalRead(swPin);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Your name");
        lcd.setCursor(0,1);
        String nume = readStringFromEEPROM(0); 
        Serial.println(nume);
        lcd.print(nume);
        if( swState != lastSwState && swState == LOW){
          dpState = !dpState;
          selected = !selected;
          selector = 0;
          Serial.println("pushed");
          delay(10);
          lcd.clear();
          break;
        }
        delay(100);
        lastSwState = swState;
        break;
      }
      case 1: 
      while(true)
      {
        swState = digitalRead(swPin);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter your name");
        lcd.setCursor(0,1);
        lcd.print("in serial");
        
        if( swState != lastSwState && swState == LOW){
          while (Serial.available() > 0) {
          incomingByte = (char)Serial.read();
          inputString += incomingByte;
          Serial.print("I received: ");
          Serial.println(incomingByte);

          if (incomingByte == '\n') {
            stringComplete = true;
            Serial.println(inputString);
            writeStringToEEPROM(1,inputString);
            break;
            }
            
          }
          writeStringToEEPROM(1,inputString);
          dpState = !dpState;
          selected = !selected;
          selector = 0;
          delay(10);
          lcd.clear();
          break;
          
          delay(100);
          lastSwState = swState;
        }
      
      case 2: 
      while(true) {
        delay(100);
        yValue = analogRead(yPin);
        swState = digitalRead(swPin);
        if(yValue > maxThreshold && joyMoved == false){
          startingLevel--;
          joyMoved = true;
        }
        if(yValue < minThreshold && joyMoved ==false){
          startingLevel++;
          joyMoved = true;
        }
        if(yValue >= minThreshold && yValue <= maxThreshold ){
          joyMoved = false;
        }
        
         lcd.clear();
         lcd.setCursor(0, 0);
         lcd.print("Level ");
         lcd.print(startingLevel);
         
         if( swState != lastSwState && swState == LOW){
          dpState = !dpState;
          selected = !selected;
          selector = 0;
          delay(10);
          lcd.clear();
          break;
        }
        delay(100);
        lastSwState = swState;
        break;
      }
  }
  }
   }
}
void displayHighscore()
{

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Highscore ");
    lcd.setCursor(0,1);
    String nume = readStringFromEEPROM(1);
    lcd.print(nume);
    lcd.print(": ");
    lcd.print(highscore);
    //Serial.println("okay");

}
void displayAbout(int option)
{
  Serial.println("okay");
  while(true)
   {
    delay(100);
    yValue = analogRead(yPin);
    swState = digitalRead(swPin);
    if(yValue > maxThreshold && joyMoved == false){
        if(option > 0){
          option--;
        }else{
          option = 2;
        }
        joyMoved = true;
      }
      if(yValue < minThreshold && joyMoved == false){
        if(option < 2){
         option++;
        }else{
          option = 0;
        }
        joyMoved = true;
      }
      if(yValue >= minThreshold && yValue <= maxThreshold ){
        joyMoved = false;
      }
      switch(option){
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Name");
          lcd.setCursor(0,1);
          lcd.print("Rush Hour");
          break;
        case 1: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Created By");
          lcd.setCursor(0,1);
          lcd.print("Mateiu Rares");
          break;
        case 2: 
          lcd.clear();   
          lcd.setCursor(0, 0);
          lcd.print("Link");
          lcd.setCursor(0,1);
          lcd.print("zzz");
          break;
      }
     if( swState != lastSwState && swState == LOW){
          selected = !selected;
          option = 0;
          delay(10);
          lcd.clear();
          break;
        }
        lastSwState = swState;
    }
}

void RunGame()            
{
  SpawnPlayer();
  //CreateWalls();
  if(checkDifficulty == false)
    if(startingLevel != currentLevel){
      shouldChangeLevel = true;
      currentLevel--;
      checkDifficulty = true;
    }
  else
    checkDifficulty = true;
  //the dificulty of the game increases with the seconds the player survives
  if(((millis() - resetLevelTimer - lastLevelChange) >= levelTimer && currentLevel < 7) || shouldChangeLevel == true){      
    shouldChangeLevel = false;
    lastLevelChange = millis() - resetLevelTimer;
    switch (currentLevel) {
      case 0:
      break;
      case 1:   
        carSpeed = defaultSpeedValue;
        levelTimer = defaultTimeValue;
        carSpawnRate = multiplier * carSpeed;
        break;
      case 2:    
        carSpeed = carSpeed - decreaseValue;
        points++;
        carSpawnRate = multiplier * carSpeed;
        break;
      case 3:    
        carSpeed = carSpeed - decreaseValue;     
        levelTimer = levelTimer + defaultTimeValue;        
        points++;
        carSpawnRate = multiplier * carSpeed;
        break;
      case 4:    
        carSpeed = carSpeed - decreaseValue;
        levelTimer = levelTimer + defaultTimeValue;
        points++;
        carSpawnRate = multiplier * carSpeed;        
        break;
      case 5:
        carSpeed = carSpeed - decreaseValue;
        points++;
        carSpawnRate = multiplier * carSpeed;
        break;
      case 6:
        carSpeed = carSpeed - decreaseValue;
        points++;
        carSpawnRate = multiplier * carSpeed;
        break;
      case 7:
        carSpeed = carSpeed - decreaseValue;
        points++;
        carSpawnRate = multiplier * carSpeed;
        break;
    }
  }
  //the player gets points with every second passed
  if( millis() - pointsTimer >= 1000){          
    score += points;
    pointsTimer = millis();
  }
  
  if(millis() - botUpdateTimer >= carSpeed){        
    ClearLastBotPosition();
    xCar -= 1;
    botUpdateTimer = millis();
  }
  //checks to see if there should be another spawn
  if(millis() - lastSpawnTime >= carSpawnRate && spawnCar == true){    
    carSpawner();
    spawnCar = false;
    lastSpawnTime = millis();
  }
  SpawnBot();
  swState = digitalRead(swPin);
  
  if(isInvincible == false){                
    swState = digitalRead(swPin);
    if (swState !=  lastSwState) {
      if (swState == HIGH && numberOfStars > 0) {
        isInvincible = true;
        numberOfStars--;
        startInvincibility = millis();
      }
    }
    CheckCollision();
  }
  else 
  {
    if(millis() - startInvincibility >= invincibilityEnd)
      isInvincible = false;
    ClearLastPlayerPosition();
    if(millis() - lastBlink >= 500){
      SpawnPlayer();
      lastBlink = millis();
    }
  }
  lastSwState = swState;
  if(isGameOver == false)
    lcdGame();                                   
  lastNumberOfLives = numberOfLives;
  if(xCar <= -3)
    spawnCar = true;
  //player movement
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  if (xValue < minThreshold && joyMoved == false) {           
      if (xPlayer > 2) {
        ClearLastPlayerPosition();
        xPlayer -= 1;
      } else {
        xPlayer = xPlayer;
      }
      joyMoved = true;
    }
  if (xValue > maxThreshold && joyMoved == false) {
      if (xPlayer < 6) {
        ClearLastPlayerPosition();
        xPlayer += 1;
      } else {
        xPlayer = xPlayer;
      }
      joyMoved = true;
  }
  if (yValue < minThreshold && joyMoved == false) {
      if (yPlayer < 3) {
        ClearLastPlayerPosition();
        yPlayer += 3;
      } else {
        yPlayer = yPlayer;
      }
      joyMoved = true;
    }
  if (yValue > maxThreshold && joyMoved == false) {
      if (yPlayer > 3) {
        ClearLastPlayerPosition();
        yPlayer -= 3;
      } else {
        yPlayer = yPlayer;
      }
      joyMoved = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
      joyMoved = false;
  }
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
        lcdGame();       // i want to show the update game UI ( with no lives)
        endGame();
      }
  if(takenDmg == true){            // the player will blink so he knows he is invincible after taking damage
    ClearLastPlayerPosition();
    if(millis() - lastBlink >= 500){
      SpawnPlayer();
      lastBlink = millis();
    }
  }
  if(millis() - dmgTimer >= 2000)
    takenDmg = false;
    
}
void SpawnBot()                         //this function will spawn a bot at the designated coordinates, modifying the collision matrix
{
  lc.setLed(0,xCar, yCar, true);
  lc.setLed(0,xCar - 1, yCar, true);
  lc.setLed(0,xCar, yCar + 1, true);
  lc.setLed(0,xCar, yCar - 1, true);
  lc.setLed(0,xCar + 1, yCar, true);
  lc.setLed(0,xCar + 2, yCar, true);
  lc.setLed(0,xCar + 2, yCar + 1, true);
  lc.setLed(0,xCar + 2, yCar - 1, true);
  //since i want the bot to fade out, i have to check if i have to modify the collision matrix, otherwise the game will get stuck trying to acces a nonexistent position
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
void carSpawner()         //it creates random coordinates for the bot cars
{
  xCar = defaultXCar;
  int randomNumber = random(0,11);
  if(randomNumber % 2 == 0)
    yCar = 5;
  else
    yCar = 2;
  SpawnBot();
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

void ClearLastPlayerPosition()              //every time the player changes position i use this function to turn off the led's thus avoiding the matrix blinking all the time
{
  lc.setLed(0,xPlayer, yPlayer, false);
  lc.setLed(0,xPlayer + 1, yPlayer, false);
  lc.setLed(0,xPlayer, yPlayer + 1, false);
  lc.setLed(0,xPlayer, yPlayer - 1, false);
  lc.setLed(0,xPlayer - 1, yPlayer, false);
  lc.setLed(0,xPlayer - 2, yPlayer, false);
  lc.setLed(0,xPlayer - 2, yPlayer + 1, false);
  lc.setLed(0,xPlayer - 2, yPlayer - 1, false);
}
void ClearLastBotPosition()               //same principle as for the player
{
  lc.setLed(0,xCar, yCar, false);
  lc.setLed(0,xCar - 1, yCar, false);
  lc.setLed(0,xCar, yCar + 1, false);
  lc.setLed(0,xCar, yCar - 1, false);
  lc.setLed(0,xCar + 1, yCar, false);
  lc.setLed(0,xCar + 2, yCar, false);
  lc.setLed(0,xCar + 2, yCar + 1, false);
  lc.setLed(0,xCar + 2, yCar - 1, false);
  //since i want the bot to fade out, i have to check if i have to modify the collision matrix, otherwise the game will get stuck trying to acces a nonexistent position
  if(7-xCar <= 7 && 7- xCar >= 0){
    matrix[7-xCar][7-yCar] = 0;
    matrix[7-xCar][7-(yCar + 1)] = 0;
    matrix[7-xCar][7-(yCar - 1)] = 0;
  }
  if(7-(xCar - 1) <= 7 && 7- xCar >= 0)
    matrix[7-(xCar - 1)][7-yCar] = 0;
  if(7-(xCar + 1) <= 7 && 7 - (xCar + 1) >= 0)
    matrix[7-(xCar + 1)][7-yCar] = 0;
  if(7-(xCar + 2) <= 7 && 7 - (xCar + 2) >= 0){
    matrix[7-(xCar + 2)][7-yCar] = 0;
    matrix[7-(xCar + 2)][7-(yCar + 1)] = 0;
    matrix[7-(xCar + 2)][7-(yCar - 1)] = 0;
  }
}

void endGame()        //when the game is over i make sure i print to the lcd the important information
{
  isGameOver = true;
  secondsPlayed = millis()/1000;
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("GAME OVER");
  endTime = millis();
  while(millis() - endTime < 1000)
    ;
  lcd.clear();
  if(highscore < score){        // saving the highscore if needed
    EEPROM.put(0, score);
    lcd.setCursor(0,0);
    lcd.print("You got a ");
    lcd.setCursor(2,1);
    lcd.print("high score!");
  }
  else{
    lcd.setCursor(0,0);
    lcd.print("Not a highscore ");
  }
  endTime = millis();
  while(millis() - endTime < 3000)
    ;
  lcd.clear();
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
String readStringFromEEPROM(int addrOffset)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  } 
  data[newStrLen] = '\0';
  return String(data);
}

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  int len = strToWrite.length();
  Serial.println(len);
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}
