# INTRODUCTION

### Hello, my name is Rares and this is a game made by me during the Introduction to Robotics course at University of Bucharest. This repository will include both the coding & setup part as well as the logic behind the game and each steps that were made in the process of creating this project.

## <a href="https://github.com/raresgl/Introduction-to-Robotics-2021---2022-" target="_blank">Related stuff</a>

# Game description

This game is inspired by my recent found passion for f1. Basically you are the main driver of your team and you have to avoid all the cars that come in your way (I know it's not like that in f1). There are only 2 lanes, but even the games seems simple you will need full focus on the higher levels. You can move both front& back and left& right. You have 3 lives and 3 *Invincibility* stars, which can be activated by pressing the button of your joystick. The *Invincibility* lasts 2 seconds. 
Also, after a collision you become immune to any possible crashes for a short period of time. Have fun!

# Menu

The menu has 2 components:

The Main Menu, which includes
* Start Game
* About
* Settings
* High Score

The Settings Menu, which lets you costumise your game & UI however you want and includes:
* Show Name (shows current player's name)
* New Name (lets the user input his name through Terminal serial)
* Set Difficulty (you can start from whatever level you want!)
* Contrast (sets the contrast of the LCD)
* Screen Light (sets the brightness of the LCD)
* Matrix Light (sets the brightness of the matrix leds)
* Quit submenu (you return to the main menu)

# Components

* Arduino UNO
* 16x2 LCD
* Joystick
* 8x8 led matrix
* Breadboards
* Wires
* Electrolytic capacitor
* Ceramic capacitor
* Resistors
* Button

# Setup
* Game 
![WhatsApp Image 2021-12-29 at 16 33 45](https://user-images.githubusercontent.com/40142811/147676199-ac88ca83-4c3f-429b-a171-29ec0c35a8e8.jpeg)
* Gameover interface
![WhatsApp Image 2021-12-29 at 16 33 48](https://user-images.githubusercontent.com/40142811/147676251-987914f9-78fc-44cd-94ba-659c8bf560c5.jpeg)
* Normal score
![WhatsApp Image 2021-12-29 at 16 33 48 (1)](https://user-images.githubusercontent.com/40142811/147676279-ac7e3da4-2215-43cc-a724-5048b2923e1e.jpeg)
* Highscore
![WhatsApp Image 2021-12-29 at 16 33 50](https://user-images.githubusercontent.com/40142811/147676333-21227395-c9cb-47c2-a749-3d30a92a19e9.jpeg)
* New name serial
<img width="135" alt="Captură de ecran din 2021-12-29 la 17 08 45" src="https://user-images.githubusercontent.com/40142811/147676378-efe49aef-a98a-4f74-b3d8-821b333f4841.png">

# Demo 

# Master Comment about Core Architecture 

The thought behind the code architecture was to have a division in modules. That's why `setup()` and `loop()` are short functions. The project is divided mainly in 2 parts, `menu.h` and `variables.h` . 

I am going through each file of the project:

* hardware.h
  In this file I included pin & pin related declarations. Also `readJoystick()` is used for interpreting read values of the joystick and in order to make the code more clear.
 
* variables.h
  It includes all the variables that are not pin related and the position clear functions , which turn off the matrix leds that correspund to the previous position of both the player & the bot car (for `clearLastBotPosition()` it also turns from 1 to 0 the values of the prev position in the matrix)
 
* game.h
  Includes game related functions. ` spawnBot()` and `carSpawner()` are bot related functions which randomise the spawn of the bot and lights up the led matrix & the collision matrix. `spawnPlayer()` lights up the matrix leds corresponding to the player. `checkCollision()` is the collision function. `runGame()` is the main game function. `endGame` checks if the game is over and checks for a highscore.

* menu.h
  Includes LCD related functions, mainly for the menu. 



