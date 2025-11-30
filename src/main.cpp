#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <string> // Needed for std::to_string for the score

int main()
{
    // --- Initialization & Configuration ---
    const int screenWidth = 1200;
    const int screenHeight = 600;

    int stateofgame = 0; // 0 = Menu, 1 = NewGame, 2 = Continue, 3 = Exit
    int option = 1; 

    const int cellSize = 40;
    const int gridWidth = screenWidth / cellSize;
    const int gridHeight = screenHeight / cellSize;

    // Game State variables
    bool gameOver = false;
    int score = 0;

    // Snake Initialization
    int snakeLength = 4;
    int snakeX = screenWidth / 20; // roughly center
    int snakeY = screenHeight / 20;
    int snakePosition[255][2] = {0}; // Initialize array to 0

    // Initialize entire snake body at start position to prevent "stretching" glitch
    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }

    // Input state
    char key = 'R';

    // --- Color Palette (From the "Green" version) ---
    Color backgroundColor = {150, 180, 110, 255};
    Color gridColor = {100, 130, 90, 255};
    Color foodColor = {220, 0, 0, 255};
    Color snakebodyColor = {50, 70, 40, 255};
    Color  backgroundColorSpecial       = {255, 246, 199, 255};   // #FFF6C7
    Color  gridColorSpecial      = {232, 223, 194, 255};   // #E8DFC2
    Color  snakebodyColorSpecial    = {15, 26, 51, 255};      // #0F1A33
    Color  foodColorSpecial = {255, 79, 163, 255};    // #FF4FA3  

    // Food Initialization
    int foodX = 0;
    int foodY = 0;

    InitWindow(screenWidth, screenHeight, "Snake Game - Combined Version");
    SetTargetFPS(10); // Snake speed

    // Initial random food placement
    do
    {
        foodX = GetRandomValue(0, gridWidth - 1);
        foodY = GetRandomValue(0, gridHeight - 1);
    } while (foodX == snakeX && foodY == snakeY);

    // --- Main Game Loop ---
    while (!WindowShouldClose())
    {
        // Only update movement if the game is not over
        if (!gameOver)
        {
            // Input Handling (Added checks to prevent 180-degree turns into self)
            if (IsKeyDown(KEY_RIGHT) && key != 'L')
                key = 'R';
            if (IsKeyDown(KEY_LEFT) && key != 'R')
                key = 'L';
            if (IsKeyDown(KEY_UP) && key != 'D')
                key = 'U';
            if (IsKeyDown(KEY_DOWN) && key != 'U')
                key = 'D';

            // 1. Move Body Segments (Shift from tail to head)
            for (int i = snakeLength; i > 0; i--)
            {
                snakePosition[i][0] = snakePosition[i - 1][0];
                snakePosition[i][1] = snakePosition[i - 1][1];
            }

            // 2. Move Head based on current direction
            if (key == 'R')
                snakePosition[0][0] += 1;
            if (key == 'L')
                snakePosition[0][0] -= 1;
            if (key == 'U')
                snakePosition[0][1] -= 1;
            if (key == 'D')
                snakePosition[0][1] += 1;

            // 3. Boundary Logic (Screen Wrapping)
            // If snake goes off right, spawn on left (and vice versa)
            if (snakePosition[0][0] >= gridWidth)
                snakePosition[0][0] = 0;
            if (snakePosition[0][0] < 0)
                snakePosition[0][0] = gridWidth - 1;

            // If snake goes off bottom, spawn on top (and vice versa)
            if (snakePosition[0][1] >= gridHeight)
                snakePosition[0][1] = 0;
            if (snakePosition[0][1] < 0)
                snakePosition[0][1] = gridHeight - 1;

            // 4. Food Collision Logic
            if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
            {
                // Increase length and score
                snakeLength++;
                score += 10;

                // Respawn food ensuring it doesn't spawn on the head
                do
                {
                    foodX = GetRandomValue(0, gridWidth - 1);
                    foodY = GetRandomValue(0, gridHeight - 1);
                } while (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY);
            }

            // 5. Self Collision Logic
            // Check if head coordinates match any body segment coordinates
            for (int i = 1; i < snakeLength; i++)
            {
                if (snakePosition[0][0] == snakePosition[i][0] &&
                    snakePosition[0][1] == snakePosition[i][1])
                {
                    gameOver = true;
                }
            }
        }

        // --- Drawing Phase ---
        BeginDrawing();
        if(stateofgame == 0){
            ClearBackground(RAYWHITE);
            if(IsKeyPressed(KEY_DOWN)){
                option += 1;
            }
            if(IsKeyPressed(KEY_UP)){
                option -= 1;
            }
            if(option > 4){
                option = 1;
            }
            if(option < 1){
                option = 4;
            }
            if(IsKeyPressed(KEY_ENTER)){
                stateofgame = option;
            }
            DrawText("SNAKE GAME", screenWidth/2 - 150, screenHeight/2 - 180, 50, DARKGRAY);
            
            if(option == 1){
                DrawRectangle(screenWidth/2 - 160, screenHeight/2 - 100, 320, 50, DARKGRAY);
                DrawText("Continue", screenWidth/2 - 60, screenHeight/2 - 85, 25, WHITE);
            }
            else{
                DrawRectangleLines(screenWidth/2 - 160, screenHeight/2 - 100, 320, 50, WHITE);
                DrawText("Continue", screenWidth/2 - 60, screenHeight/2 - 85, 25, DARKGRAY);
            }
            if(option == 2) {
                DrawRectangle(screenWidth/2 - 160, screenHeight/2 - 20, 320, 50, DARKGRAY);
                DrawText("New Game", screenWidth/2 - 70, screenHeight/2 - 5, 25, WHITE);
            }
            else {
                DrawRectangleLines(screenWidth/2 - 160, screenHeight/2 - 20, 320, 50, WHITE);
                DrawText("New Game", screenWidth/2 - 70, screenHeight/2 - 5, 25, DARKGRAY);
            }
            if(option == 3) {
                DrawRectangle(screenWidth/2 - 160, screenHeight/2 + 60, 320, 50, DARKGRAY);
                DrawText("Desert Theme", screenWidth/2 - 90, screenHeight/2 + 75, 25, WHITE);
            }
            else  {
                DrawRectangleLines(screenWidth/2 - 160, screenHeight/2 + 60, 320, 50, WHITE);
                DrawText("Desert Theme", screenWidth/2 - 90, screenHeight/2 + 75, 25, DARKGRAY);
            }
            if(option == 4) {
                DrawRectangle(screenWidth/2 - 160, screenHeight/2 + 140, 320, 50, DARKGRAY);
                DrawText("Exit", screenWidth/2 - 45, screenHeight/2 + 155, 25, WHITE);
            }
            else  {
                DrawRectangleLines(screenWidth/2 - 160, screenHeight/2 + 140, 320, 50, WHITE);
                DrawText("Exit", screenWidth/2 - 45, screenHeight/2 + 155, 25, DARKGRAY);
            }
            EndDrawing();
            continue;
        }
        if(stateofgame == 2 || stateofgame == 1 || stateofgame == 3){
            if(stateofgame == 3) ClearBackground(backgroundColorSpecial); // Special Background
            else ClearBackground(backgroundColor); // Green background

            // Draw Grid
            for (int i = 0; i < gridWidth; i++)
            {
                for (int j = 0; j < gridHeight; j++)
                {   
                    if(stateofgame == 3){
                        DrawRectangleLinesEx(
                            (struct Rectangle){i * cellSize, j * cellSize, cellSize, cellSize},
                            2, gridColorSpecial);
                    }
                    else
                    DrawRectangleLinesEx(
                        (struct Rectangle){i * cellSize, j * cellSize, cellSize, cellSize},
                        2, gridColor);
                }
            }

            // Draw Food
            if(stateofgame == 3) DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, foodColorSpecial);
            else DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, foodColor);

            // Draw Snake
            for (int i = 0; i < snakeLength; i++)
            {
                if(stateofgame == 3) DrawRectangle((snakePosition[i][0]) * cellSize, (snakePosition[i][1]) * cellSize, cellSize, cellSize, snakebodyColorSpecial);
                else DrawRectangle((snakePosition[i][0]) * cellSize, (snakePosition[i][1]) * cellSize, cellSize, cellSize, snakebodyColor);
            }

            // Draw Score
            std::string scoretext = "Score: " + std::to_string(score);
            DrawText(scoretext.c_str(), screenWidth - 180, 8, 30, DARKGRAY);

            // Draw Game Over Screen
            if (gameOver)
            {
                DrawText("GAME OVER!", screenWidth / 2 - 150, screenHeight / 2 - 50, 60, RED);
                DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight / 2 + 20, 20, DARKGRAY);
            }

            EndDrawing();
        }
        if(stateofgame == 4) break;
    }
        CloseWindow();
    
    return 0;
}
