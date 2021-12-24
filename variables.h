#ifndef VARIABLES_H
#define VARIABLES_H

void( * resetFunc)(void) = 0;
byte appInitSuccessfully;

bool matrix[matrixSize][matrixSize] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

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
    B00000};

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
int LCDContrast;
int endMenuOption = 0;
int eepromSize = 1024;

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
bool inSecondSubmenu = false;

int currentMenuItem = 0;
int lastJoyRead = 0;
int submenuScrollPosition = 0;

//values for Serial Reading
String inputString = "";
char incomingByte = 0;
bool stringComplete = false;

String playerName;
const String defaultName = "PLAYER";

const int invicinbleTime = 2000;

int highScoreRecords = 0;
int currentHighScorePosition = 0;
String highScorePlayersArray[3];
unsigned long highScoresArray[3];


//eeprom values
const byte eepromHighScoreRecordsAddress = 8;
const byte eepromScreenBrightnessAddress = 10;
const byte eepromContrastAddress = 12;
const byte eepromMatrixBrightnessAddress = 14;
const byte eepromAppInitSuccessfullyAddress = 20;
const byte eepromChosenLevelAddress = 22;

//eeprom arrays
const byte eepromHighScoreStartAddress = 0;
const byte eepromPlayerNameStartAddress = 80;
const byte eepromCurrentPlayerNameStartAddress = 220;


//if the players position is updated the function will turn of the prev position in order to avoid blink
void clearLastPlayerPosition()
{
  // 0 represents the address of the matrix and its 0 bc i have only one matrix
  lc.setLed(0, xPlayer, yPlayer, false);
  lc.setLed(0, xPlayer + 1, yPlayer, false);
  lc.setLed(0, xPlayer, yPlayer + 1, false);
  lc.setLed(0, xPlayer, yPlayer - 1, false);
  lc.setLed(0, xPlayer - 1, yPlayer, false);
  lc.setLed(0, xPlayer - 2, yPlayer, false);
  lc.setLed(0, xPlayer - 2, yPlayer + 1, false);
  lc.setLed(0, xPlayer - 2, yPlayer - 1, false);
}

void clearLastBotPosition()
{
  lc.setLed(0, xCar, yCar, false);
  lc.setLed(0, xCar - 1, yCar, false);
  lc.setLed(0, xCar, yCar + 1, false);
  lc.setLed(0, xCar, yCar - 1, false);
  lc.setLed(0, xCar + 1, yCar, false);
  lc.setLed(0, xCar + 2, yCar, false);
  lc.setLed(0, xCar + 2, yCar + 1, false);
  lc.setLed(0, xCar + 2, yCar - 1, false);
  //in order to avoid clearing nonexistent positions i check if the bot is still in the matrix
  if (7 - xCar <= 7 && 7 - xCar >= 0)
  {
    matrix[7 - xCar][7 - yCar] = 0;
    matrix[7 - xCar][7 - (yCar + 1)] = 0;
    matrix[7 - xCar][7 - (yCar - 1)] = 0;
  }
  if (7 - (xCar - 1) <= 7 && 7 - xCar >= 0)
    matrix[7 - (xCar - 1)][7 - yCar] = 0;
  if (7 - (xCar + 1) <= 7 && 7 - (xCar + 1) >= 0)
    matrix[7 - (xCar + 1)][7 - yCar] = 0;
  if (7 - (xCar + 2) <= 7 && 7 - (xCar + 2) >= 0)
  {
    matrix[7 - (xCar + 2)][7 - yCar] = 0;
    matrix[7 - (xCar + 2)][7 - (yCar + 1)] = 0;
    matrix[7 - (xCar + 2)][7 - (yCar - 1)] = 0;
  }
}


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
void writeIntIntoEEPROM(int address, int number) {
    EEPROM.write(address, number >> 8);
    EEPROM.write(address + 1, number & 0xFF);
}
int readIntFromEEPROM(int address) {
    return (EEPROM.read(address) << 8) + EEPROM.read(address + 1);
}


void initEEPROM() {
    writeIntIntoEEPROM(eepromMatrixBrightnessAddress, 2);
    writeIntIntoEEPROM(eepromContrastAddress, 100);
    writeIntIntoEEPROM(eepromScreenBrightnessAddress, 950);
    writeIntIntoEEPROM(eepromAppInitSuccessfullyAddress, 1);
    writeStringToEEPROM(eepromPlayerNameStartAddress, defaultName);
    writeIntIntoEEPROM(eepromChosenLevelAddress, defaultLevel);
}



int saveHighScore(const String & playerName, const unsigned int & score) {
    int place = 0;
    int score1, score2, score3;
    String name1, name2, name3;
    int length1, length2, length3, length;
    highScoreRecords = readIntFromEEPROM(eepromHighScoreRecordsAddress);
    if (highScoreRecords > 3 || highScoreRecords < 0) {
        highScoreRecords = 0;
    }

    switch (highScoreRecords) {
    case 0: {
        writeIntIntoEEPROM(eepromHighScoreStartAddress, score);
        writeStringToEEPROM(eepromPlayerNameStartAddress, playerName);
        place = 1;
        String d;
        readStringFromEEPROM(eepromPlayerNameStartAddress, & d);
        break;
    }
    case 1: {
        score1 = readIntFromEEPROM(eepromHighScoreStartAddress);
        if (score > score1) {
            writeIntIntoEEPROM(eepromHighScoreStartAddress, score);
            writeIntIntoEEPROM(eepromHighScoreStartAddress + sizeof(int), score1);
            readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            length1 = writeStringToEEPROM(eepromPlayerNameStartAddress, playerName);
            writeStringToEEPROM((eepromPlayerNameStartAddress + length1), name1);
            place = 1;
        } else {
            writeIntIntoEEPROM(eepromHighScoreStartAddress + sizeof(int), score);
            length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            writeStringToEEPROM(eepromPlayerNameStartAddress + length1, playerName);
            place = 2;
        }
        break;
    }
    case 2: {
        score1 = readIntFromEEPROM(eepromHighScoreStartAddress);
        score2 = readIntFromEEPROM(eepromHighScoreStartAddress + sizeof(int));
        if (score > score1) {
            writeIntIntoEEPROM(eepromHighScoreStartAddress, score);
            writeIntIntoEEPROM(eepromHighScoreStartAddress + sizeof(int), score1);
            writeIntIntoEEPROM(eepromHighScoreStartAddress + 2 * sizeof(int), score2);
            length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            readStringFromEEPROM(eepromPlayerNameStartAddress + length1, & name2);
            length = writeStringToEEPROM(eepromPlayerNameStartAddress, playerName);
            writeStringToEEPROM(eepromPlayerNameStartAddress + length, name1);
            writeStringToEEPROM(eepromPlayerNameStartAddress + length + length1, name2);
            place = 1;
        } else if (score > score2) {
            writeIntIntoEEPROM(eepromHighScoreStartAddress + sizeof(int), score);
            writeIntIntoEEPROM(eepromHighScoreStartAddress + 2 * sizeof(int), score2);
            length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            readStringFromEEPROM(eepromPlayerNameStartAddress + length1, & name2);
            length = writeStringToEEPROM(eepromPlayerNameStartAddress + length1, playerName);
            writeStringToEEPROM(eepromPlayerNameStartAddress + length1 + length, name2);
            place = 2;
        } else {
            writeIntIntoEEPROM(eepromHighScoreStartAddress + 2 * sizeof(int), score);
            length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            length2 = readStringFromEEPROM(eepromPlayerNameStartAddress + length1, & name2);
            writeStringToEEPROM(eepromPlayerNameStartAddress + length1 + length2, playerName);
            place = 3;

        }
        break;
    }
    case 3: {
        score1 = readIntFromEEPROM(eepromHighScoreStartAddress);
        score2 = readIntFromEEPROM(eepromHighScoreStartAddress + sizeof(int));
        score3 = readIntFromEEPROM(eepromHighScoreStartAddress + 2 * sizeof(int));
        if (score > score1) {
            writeIntIntoEEPROM(eepromHighScoreStartAddress, score);
            writeIntIntoEEPROM(eepromHighScoreStartAddress + sizeof(int), score1);
            writeIntIntoEEPROM(eepromHighScoreStartAddress + 2 * sizeof(int), score2);
            length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            length2 = readStringFromEEPROM(eepromPlayerNameStartAddress + length1, & name2);
            length = writeStringToEEPROM(eepromPlayerNameStartAddress, playerName);
            writeStringToEEPROM(eepromHighScoreStartAddress + length, name1);
            writeStringToEEPROM(eepromHighScoreStartAddress + length + length1, name2);
            place = 1;
        } else if (score > score2) {
            writeIntIntoEEPROM(eepromHighScoreStartAddress + sizeof(int), score);
            writeIntIntoEEPROM(eepromHighScoreStartAddress + 2 * sizeof(int), score2);
            length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            readStringFromEEPROM(eepromPlayerNameStartAddress + length1, & name2);
            length = writeStringToEEPROM(eepromPlayerNameStartAddress + length1, playerName);
            writeStringToEEPROM(eepromHighScoreStartAddress + length1 + length, name2);
            place = 2;
        } else if (score > score3) {
            writeIntIntoEEPROM(eepromHighScoreStartAddress + 2 * sizeof(int), score);
            length1 = readStringFromEEPROM(eepromPlayerNameStartAddress, & name1);
            length2 = readStringFromEEPROM(eepromPlayerNameStartAddress + length1, & name2);
            writeStringToEEPROM(eepromPlayerNameStartAddress + length1 + length2, playerName);
            place = 3;
        } else {
            // Report if we didn't save
            return -1;
        }
        break;
    }
    }
    // Report that we have saved a new high score
    if (highScoreRecords < 3) {
        highScoreRecords++;
    }
    writeIntIntoEEPROM(eepromHighScoreRecordsAddress, highScoreRecords);
    return place;
}




#endif
