#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <string> // Needed for std::to_string for the score

int main()
{
    // --- Initialization & Configuration ---
    const int screenWidth = 1200;
    const int screenHeight = 600;
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
    Color textColor = WHITE; // For score

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
        ClearBackground(backgroundColor); // Green background

        // Draw Grid
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                DrawRectangleLines(i * cellSize, j * cellSize, cellSize, cellSize, gridColor);
            }
        }

        // Draw Food
        DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, foodColor);

        // Draw Snake
        for (int i = 0; i < snakeLength; i++)
        {
            DrawRectangle((snakePosition[i][0]) * cellSize, (snakePosition[i][1]) * cellSize, cellSize, cellSize, snakebodyColor);
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

    CloseWindow();
    return 0;
}