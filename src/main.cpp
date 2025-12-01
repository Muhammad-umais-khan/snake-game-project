#include <raylib.h>
#include <stdlib.h>
#include <string> // For std::to_string

int main()
{
    // --- Configuration ---
    const int screenWidth = 1200;
    const int screenHeight = 600;
    const int cellSize = 40;
    const int gridWidth = screenWidth / cellSize;
    const int gridHeight = screenHeight / cellSize;

    // Game state
    bool gameOver = false;
    int score = 0;

    // --- Snake Initialization ---
    int snakeLength = 4;
    int snakeX = gridWidth / 2;  // center in grid
    int snakeY = gridHeight / 2;
    int snakePosition[255][2] = {0};

    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX - i;  // horizontal snake to the left
        snakePosition[i][1] = snakeY;
    }

    char key = 'R'; // initial direction

    // --- Colors ---
    Color backgroundColor = {150, 180, 110, 255};
    Color gridColor = {100, 130, 90, 255};
    Color foodColor = {220, 0, 0, 255};
    Color snakeBodyColor = {50, 70, 40, 255};
    Color textColor = WHITE;

    // --- Food Initialization ---
    int foodX = 0;
    int foodY = 0;

    InitWindow(screenWidth, screenHeight, "Snake Game - Boundary Collision");
    SetTargetFPS(10);

    // Spawn initial food
    do
    {
        foodX = GetRandomValue(0, gridWidth - 1);
        foodY = GetRandomValue(0, gridHeight - 1);
    } while (foodX == snakeX && foodY == snakeY);

    // --- Main Game Loop ---
    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            // --- Input ---
            if (IsKeyDown(KEY_RIGHT) && key != 'L') key = 'R';
            if (IsKeyDown(KEY_LEFT) && key != 'R') key = 'L';
            if (IsKeyDown(KEY_UP) && key != 'D') key = 'U';
            if (IsKeyDown(KEY_DOWN) && key != 'U') key = 'D';

            // --- Move snake body ---
            for (int i = snakeLength; i > 0; i--)
            {
                snakePosition[i][0] = snakePosition[i - 1][0];
                snakePosition[i][1] = snakePosition[i - 1][1];
            }

            // --- Move snake head ---
            if (key == 'R') snakePosition[0][0] += 1;
            if (key == 'L') snakePosition[0][0] -= 1;
            if (key == 'U') snakePosition[0][1] -= 1;
            if (key == 'D') snakePosition[0][1] += 1;

            // --- Boundary Collision ---
            if (snakePosition[0][0] >= gridWidth || snakePosition[0][0] < 0 ||
                snakePosition[0][1] >= gridHeight || snakePosition[0][1] < 0)
            {
                gameOver = true;
            }

            // --- Food Collision ---
            if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
            {
                snakeLength++;
                score += 10;

                // Spawn new food (ensure not on snake head)
                do
                {
                    foodX = GetRandomValue(0, gridWidth - 1);
                    foodY = GetRandomValue(0, gridHeight - 1);
                } while (foodX == snakePosition[0][0] && foodY == snakePosition[0][1]);
            }

            // --- Self Collision ---
            for (int i = 1; i < snakeLength; i++)
            {
                if (snakePosition[0][0] == snakePosition[i][0] &&
                    snakePosition[0][1] == snakePosition[i][1])
                {
                    gameOver = true;
                }
            }
        }

        // --- Drawing ---
        BeginDrawing();
        ClearBackground(backgroundColor);

        // Draw grid
        for (int i = 0; i < gridWidth; i++)
            for (int j = 0; j < gridHeight; j++)
                DrawRectangleLines(i * cellSize, j * cellSize, cellSize, cellSize, gridColor);

        // Draw food
        DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, foodColor);

        // Draw snake
        for (int i = 0; i < snakeLength; i++)
            DrawRectangle(snakePosition[i][0] * cellSize, snakePosition[i][1] * cellSize, cellSize, cellSize, snakeBodyColor);

        // Draw score
        DrawText(("Score: " + std::to_string(score)).c_str(), screenWidth - 180, 8, 30, DARKGRAY);

        // Draw game over
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

