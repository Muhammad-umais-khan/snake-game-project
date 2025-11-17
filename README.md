# Simple Raylib Ball Game
A small C++ game made using Raylib.
You control a white ball that moves around the screen, wraps around the edges, and eats a red food circle that respawns randomly.

This project is perfect for beginners learning:
1. window creation
2.drawing shapes
3. movement handling
4. collisions
5. randomness

How to Use This Project

Open the project by double-clicking main.code-workspace.

From VS Code’s Explorer, open src/main.cpp.

Press F5 to build and run the game.

Watch the explanation video included in the project folder.

Features

Smooth 59 FPS rendering

Arrow-key movement

Keeps moving until new direction chosen

Screen wrap-around (Pac-Man style)

Random food generation

Collision detection using distance formula

How to Compile Manually (Optional)

Windows + MinGW + Raylib:

g++ main.cpp -o game -lraylib -lopengl32 -lgdi32 -lwinmm


Then run:

./game

# Code Explanation

Everything below explains how the program works, part by part.

1. Includes
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


raylib.h → graphics, window, drawing, input

stdlib.h → randomness functions

time.h → seeding the random generator

math.h → square root for detecting collision distances

2. Window + Player Setup
const int screenWidth = 810;
const int screenHeight = 900;
int BallX = screenWidth / 2;
int BallY = screenHeight / 2;
char key = 'Z';


Creates the window size, centers the player, and stores movement direction.

Colors:

Color black = {0, 0, 0, 255};
Color white = {255, 255, 255, 255};
Color red   = {255, 73, 92, 255};


Player and food sizes:

int playerRadius = 20;
int foodRadius = 10;

3. Window Initialization
InitWindow(screenWidth, screenHeight, "first game window by umais using raylib");
SetTargetFPS(59);
srand(time(NULL));


Creates the game window

Sets smooth frame rate

Seeds random numbers

4. Food Spawn
do {
    foodX = rand() % screenWidth;
    foodY = rand() % screenHeight;
} while (foodX == BallX && foodY == BallY);


Makes sure food never spawns on top of the player.

Main Game Loop
while (!WindowShouldClose()) {


Runs until the user closes the window.

5. Movement Input
if (IsKeyDown(KEY_RIGHT)) key = 'R';
if (IsKeyDown(KEY_LEFT))  key = 'L';
if (IsKeyDown(KEY_UP))    key = 'U';
if (IsKeyDown(KEY_DOWN))  key = 'D';


Direction updates only when a new key is pressed.

6. Move Player
if (key == 'R') BallX += 5;
if (key == 'L') BallX -= 5;
if (key == 'U') BallY -= 5;
if (key == 'D') BallY += 5;


Moves the ball 5 pixels each frame.

7. Screen Wrap-Around
if (BallX > screenWidth)  BallX = 0;
if (BallX < 0)            BallX = screenWidth;
if (BallY > screenHeight) BallY = 0;
if (BallY < 0)            BallY = screenHeight;


If the ball leaves one edge, it appears on the opposite side.

8. Collision Detection
float dx = BallX - foodX;
float dy = BallY - foodY;
float distance = sqrt(dx*dx + dy*dy);


Standard distance formula from geometry.

If touching:

if (distance <= playerRadius + foodRadius) {
    // respawn food
}


Food respawns randomly.

9. Drawing
BeginDrawing();
ClearBackground(black);
DrawCircle(foodX, foodY, foodRadius, red);
DrawCircle(BallX, BallY, playerRadius, white);
EndDrawing();


Clears screen

Draws food

Draws player

Runs every frame.
Why EndDrawing() is necessary

In Raylib, drawing works like a double-buffer system:

BeginDrawing() → tells Raylib: “I’m about to start drawing this frame.”

You draw all objects (background, player, food, text, etc.)

EndDrawing() → tells Raylib: “Done drawing this frame, show it on screen now.”

Without EndDrawing(), nothing appears because the drawn frame isn’t sent to the screen yet. It essentially flushes the drawing commands
10. Cleanup
CloseWindow();
return 0;


Shuts down the window and ends the program.
