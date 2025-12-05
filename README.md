🐍 Snake Game - Ultimate Version

Group Name: 404 Not Found

Members: Muneeb ur Rehman, Muhammad Umais, Sadia Sahar

Welcome to the Snake Game - Ultimate Version. This is a robust implementation of the classic arcade game built from scratch using C++ and the Raylib library. It features multiple difficulties, a story mode with progression, persistent save files, and dynamic theming.

🛠️ Requirements & Setup

This project was designed to be simple to set up.

1. The Only Dependency: Raylib

You do not need to install complex engines or multiple libraries. The only requirement for this project is:

Raylib (A simple and easy-to-use library for videogames programming).

2. Project Files

While our project folder contains various source and header files, the external requirement is strictly Raylib.

Ensure your C++ compiler (Visual Studio, VS Code, or Xcode) is linked to Raylib.

Compile and run main.cpp.

💻 Code Walkthrough & Explanation

Here is a detailed breakdown of how our code works, explained section by section.

📚 1. Libraries and Headers

At the very top of the file, we include the necessary tools:

#include <raylib.h>: The core library used for graphics, window management, and keyboard input.

#include <stdlib.h>: Used for rand() and GetRandomValue() (to spawn food in random spots).

#include <string>: Allows us to use text variables (like "Score: 100" or player names).

#include <fstream>: Stands for "File Stream." This is crucial for our Save/Load and High Score systems.

#include <vector>: Included for dynamic array support.

#include <cstdio>: Used for the remove() command to delete save files when the player dies.

#include <cmath>: Used for the ceil() function to round numbers (used in the Story Mode countdown timer).

⚙️ 2. Global Structures and Enums

To make the code clean and professional, we grouped data together instead of using hundreds of loose variables.

enum GameMode:

This creates custom labels for numbers. Instead of remembering "0 is Easy" and "3 is Story", we can write game.currentMode = STORY.

struct GameState:

What is it? This is a custom container that holds all the variables for the game.

Why use it? It allows us to pass the entire game's data (Score, Snake Position, Mode, etc.) into functions using a single variable (game).

It contains:

State: stateofgame (Menu vs Gameplay).

Snake: snakePosition[1024][2] (An array holding X and Y coordinates for every body part).

Story Logic: isLevelTransitioning and storyLevel (to handle level-ups).

File Logic: hasSaveFile (to enable the "Continue" button).

📏 3. Global Constants

These are calculated once to handle the game grid:

screenWidth / screenHeight: The size of the application window.

cellSize: Set to 40 pixels. Every grid square (snake part, food, wall) is 40x40.

boardOffsetX: Calculates padding to ensure the game board is perfectly centered in the window.

🧩 4. Function Prototypes

These are "declarations" listed before main(). They tell the C++ compiler: "Trust us, these functions (like ResetGame or DrawMenu) exist further down in the file." This allows us to organize the code neatly.

🚀 5. The main() Function

This is the entry point of the application.

Initialization (InitWindow): Starts the graphics engine and sets the target FPS to 60.

Grid Setup (InitGameGrid): Calculates how many 40px cells fit on your specific screen.

State Initialization:

Creates the GameState game object.

Calls InitHurdles to build the map.

Calls LoadHighscore and CheckSaveFile to read from the hard drive.

The Game Loop (while):

Update Phase: If in Menu, run UpdateMenu(). If Playing, run UpdateGameplay().

Draw Phase: Clears the screen, draws the appropriate scene, and displays the result.

🧠 6. Logic Functions (The Brains)

InitGameGrid(): Performs math (Modulus %) to ensure the grid fits on the screen without cutting off any squares.

IsTileBlocked():

Crucial Function: Before spawning food or moving, this checks if a specific coordinate (x, y) is occupied by the Snake's body OR a Wall.

It prevents bugs where food spawns inside walls.

ResetGame():

Resets Score to 0 and Snake Length to 4.

Places the snake back in the center.

Uses a do-while loop to find a valid spawn point for the food.

InitHurdles():

Contains the hard-coded coordinates for the map. It draws "L" shapes in the corners and the barrier in the middle.

SaveGame() & LoadGame():

Uses std::ofstream (output) and std::ifstream (input) to write/read the game variables to a text file named savefile.txt.

🎮 7. Update Functions (Input & Math)

UpdateMenu(GameState &game):

Handles Navigation (Up/Down keys).

Handles Logic: If you press "Theme", it swaps the colors. If you press "New Game", it calls ResetGame.

UpdateGameplay(GameState &game):

Story Mode Check: Checks if score > 50 or > 100. If so, it triggers a Level Up, resets the snake position (Safe Spawn), and starts a countdown.

Timer: Increases moveTimer by GetFrameTime(). When it exceeds moveInterval, the snake moves.

Movement Logic:

Shifts every body segment to the position of the segment in front of it.

Updates the Head position based on the key (Direction).

Collision Detection:

Walls: If x < 0 or x > width, TRIGGER Game Over.

Hurdles: Loops through walls; if Head == Wall, TRIGGER Game Over.

Self: Loops through body; if Head == Body, TRIGGER Game Over.

Auto-Save: Calls SaveGame() every time the snake moves so progress is never lost.

🎨 8. Drawing Functions (The Visuals)

DrawMenu():

Checks the game.theme variable.

If "Classic", uses Green colors. If "Desert", uses Sand/Blue colors.

Draws the rectangles and text for the UI.

DrawGameplay():

Grid: Uses for loops to draw the grid lines.

Snake & Food: Loops through the arrays and draws DrawRectangle for every part.

Overlays:

If isLevelTransitioning is true, it draws a semi-transparent black screen with a countdown.

If gameOver is true, it displays the Game Over text and instructions to restart.

🕹️ Controls

Arrow Keys: Move Snake / Navigate Menu.

Enter: Select Menu Option.

ESC: Pause Game / Go Back.

R: Restart (on Game Over screen).
