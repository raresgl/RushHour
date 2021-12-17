#ifndef VARIABLES_H
#define VARIABLES_H
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

byte heartShape[] = {
    B00000,
    B01010,
    B11111,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000};
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

//game vars
int defaultXCar = 9;
int xPlayer = 2;
int yPlayer = 5;
int xCar = 8;
int yCar = 2;
int score = 0;
int scoreIncrement = 1;

int maxRows = 8;
int maxColumns = 8;

bool joyMoved = false;
bool gameStarted = false;
bool isGameOver = false;
bool shouldCreateBot = true;
bool takenDmg = false;
bool isInvincible = false;
bool didCheckLevel = false;

int timerIncrementValue = 15000;
float multiplier = 12.5;
int carSpeed = 600;
int botSpawnRate = 12.5 * carSpeed;
int currentLevel = 1;
int numberOfLives = 3;
int lastNumberOfLives = 3;
int numberOfStars = 3;
int lastNumberOfStars = 3;
int defaultLevel = 1;
int highScore;
int chosenLevel = 1;

int case1CarSpeedVal = 600;
int case2CarSpeedVal = 450;
int case3CarSpeedVal = 250;
int case4CarSpeedVal = 150;
int case5CarSpeedVal = 100;
int case6CarSpeedVal = 75;
int case7CarSpeedVal = 50;
//time vars
unsigned long lastRefreshTime = 0; //for the cars to update
unsigned long lastSpawnTime = 0;   //to spawn a new car
unsigned long lastLevelChange = 0;
unsigned long levelTimer = 15000;
unsigned long defaultLevelTimer = 15000;
unsigned long lastIncrementTime = 0;
unsigned long lastDamageTaken = 0;
unsigned long lastBlink = 0;
unsigned long elapsedTime = 0;
unsigned long endTime = 0;
unsigned long levelReset = 0; //a variable that i use to "reset" the millis
unsigned long startInvincibility = 0;
unsigned long dmgTimer = 0;

//variables for the menu
int menuLevel = 0;
int menuOption = 0;
int linkPart = 0;
int infoMenuOption = 0;
int secondaryMenu = 0;
int brightness = 128;
int endMenuOption = 0;

bool gameJustOpened = true;
bool inMainMenu = true;
bool isScrolled = false;
bool lockedLevel = false;
bool shouldChangeLevel = false;
bool lockedOnLink = false;
bool lockedBrightness = false;

///NEW
//values for the game menu
bool startUp = true;
int startUpTime = 500;
unsigned long printLineTimer = 0;
const int printDebounceTime = 100;
int sectionNum = 0; //what section will be inSubmenu(highscore, play, etc)
bool inSubmenu = false;
int currentMenuItem = 0;
int lastJoyRead = 0;
int submenuScrollPosition = 0;

//values for EEPROM
String inputString = "";
char incomingByte = 0;
bool stringComplete = false;

int currentEepromOffset = 0;
int highScoreRecords = 0;
String highScorePlayersArray[15];
unsigned long highScoresArray[15];
//if the players position is updated the function will turn of the prev position in order to avoid blink
void ClearLastPlayerPosition() 
{
// 0 represents the address of the matrix and its 0 bc i have only one matrix
  lc.setLed(0,xPlayer, yPlayer, false);
  lc.setLed(0,xPlayer + 1, yPlayer, false);
  lc.setLed(0,xPlayer, yPlayer + 1, false);
  lc.setLed(0,xPlayer, yPlayer - 1, false);
  lc.setLed(0,xPlayer - 1, yPlayer, false);
  lc.setLed(0,xPlayer - 2, yPlayer, false);
  lc.setLed(0,xPlayer - 2, yPlayer + 1, false);
  lc.setLed(0,xPlayer - 2, yPlayer - 1, false);
}

void ClearLastBotPosition()               
{
  lc.setLed(0,xCar, yCar, false);
  lc.setLed(0,xCar - 1, yCar, false);
  lc.setLed(0,xCar, yCar + 1, false);
  lc.setLed(0,xCar, yCar - 1, false);
  lc.setLed(0,xCar + 1, yCar, false);
  lc.setLed(0,xCar + 2, yCar, false);
  lc.setLed(0,xCar + 2, yCar + 1, false);
  lc.setLed(0,xCar + 2, yCar - 1, false);
//in order to avoid clearing nonexistent positions i check if the bot is still in the matrix
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
#endif
