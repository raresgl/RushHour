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

# Demo 

# Master Comment about Core Architecture 

The thought behind the code architecture was to have a division in modules. That's why "setup()" and loop() are short functions.

