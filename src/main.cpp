#include <raylib.h>
#include <stdlib.h>
#include <string>

// Draw Food with board offsets
void DrawingFood(int foodX, int foodY, int cellSize, int boardOffsetX, int boardOffsetY, Color foodColor)
{
    DrawRectangle(
        boardOffsetX + (foodX * cellSize),
        boardOffsetY + (foodY * cellSize),
        cellSize, cellSize,
        foodColor);
}

int main()
{
    InitWindow(0, 0, "Snake Game - Boundary Collision");
    ToggleFullscreen();
    SetTargetFPS(10);

    const int windowWidth = GetScreenWidth();
    const int windowHeight = GetScreenHeight();

    const int cellSize = 60;

    int minDimension = (windowWidth < windowHeight) ? windowWidth : windowHeight;
    int rawBoardSize = minDimension - 120;
    const int boardSize = rawBoardSize - (rawBoardSize % cellSize);

    const int gridCount = boardSize / cellSize;

    const int boardOffsetX = (windowWidth - boardSize) / 2;
    const int boardOffsetY = (windowHeight - boardSize) / 2;

    bool gameOver = false;
    int score = 0;

    int snakeLength = 4;

    // Snake start in center of grid
    int snakeX = gridCount / 2;
    int snakeY = gridCount / 2;
    int snakePosition[255][2] = { 0 };

    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }

    char key = 'R';

    // Colors
    Color windowBgColor = { 30, 30, 30, 255 };
    Color boardBgColor = { 150, 180, 110, 255 };
    Color gridColor = { 100, 130, 90, 255 };
    Color foodColor = { 220, 0, 0, 255 };
    Color snakeBodyColor = { 50, 70, 40, 255 };

    // Food placement
    int foodX, foodY;
    bool isOnSnake;

    // Initial food placement
    do
    {
        isOnSnake = false;
        foodX = GetRandomValue(0, gridCount - 1);
        foodY = GetRandomValue(0, gridCount - 1);

        for (int i = 0; i < snakeLength; i++)
        {
            if (snakePosition[i][0] == foodX && snakePosition[i][1] == foodY)
            {
                isOnSnake = true;
                break;
            }
        }

    } while (isOnSnake);

    // Main Game Loop
    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            // --------------------------
            // INPUT
            // --------------------------
            if (IsKeyDown(KEY_RIGHT) && key != 'L') key = 'R';
            if (IsKeyDown(KEY_LEFT)  && key != 'R') key = 'L';
            if (IsKeyDown(KEY_UP)    && key != 'D') key = 'U';
            if (IsKeyDown(KEY_DOWN)  && key != 'U') key = 'D';

            // --------------------------
            // NEXT HEAD POSITION (before moving)
            // --------------------------
            int nextX = snakePosition[0][0];
            int nextY = snakePosition[0][1];

            if (key == 'R') nextX++;
            if (key == 'L') nextX--;
            if (key == 'U') nextY--;
            if (key == 'D') nextY++;

            // --------------------------
            // BOUNDARY COLLISION CHECK
            // --------------------------
            if (nextX < 0 || nextX >= gridCount || nextY < 0 || nextY >= gridCount)
            {
                gameOver = true;
            }
            else
            {
                // --------------------------
                // MOVE SNAKE BODY
                // --------------------------
                for (int i = snakeLength; i > 0; i--)
                {
                    snakePosition[i][0] = snakePosition[i - 1][0];
                    snakePosition[i][1] = snakePosition[i - 1][1];
                }

                // --------------------------
                // MOVE HEAD
                // --------------------------
                snakePosition[0][0] = nextX;
                snakePosition[0][1] = nextY;
            }

            // --------------------------
            // FOOD COLLISION
            // --------------------------
            if (!gameOver &&
                snakePosition[0][0] == foodX &&
                snakePosition[0][1] == foodY)
            {
                snakeLength++;
                score += 10;

                // Re-spawn food, not on snake
                do
                {
                    isOnSnake = false;
                    foodX = GetRandomValue(0, gridCount - 1);
                    foodY = GetRandomValue(0, gridCount - 1);

                    for (int i = 0; i < snakeLength; i++)
                    {
                        if (snakePosition[i][0] == foodX &&
                            snakePosition[i][1] == foodY)
                        {
                            isOnSnake = true;
                            break;
                        }
                    }

                } while (isOnSnake);
            }

            // --------------------------
            // SELF COLLISION
            // --------------------------
            for (int i = 1; i < snakeLength; i++)
            {
                if (snakePosition[0][0] == snakePosition[i][0] &&
                    snakePosition[0][1] == snakePosition[i][1])
                {
                    gameOver = true;
                }
            }
        }

        // --------------------------
        // DRAWING
        // --------------------------
        BeginDrawing();
        ClearBackground(windowBgColor);

        // Board Background
        DrawRectangle(boardOffsetX, boardOffsetY, boardSize, boardSize, boardBgColor);

        // Grid
        for (int i = 0; i < gridCount; i++)
        {
            for (int j = 0; j < gridCount; j++)
            {
                DrawRectangleLines(
                    boardOffsetX + (i * cellSize),
                    boardOffsetY + (j * cellSize),
                    cellSize, cellSize,
                    gridColor);
            }
        }

        // Food
        DrawingFood(foodX, foodY, cellSize, boardOffsetX, boardOffsetY, foodColor);

        // Snake
        for (int i = 0; i < snakeLength; i++)
        {
            DrawRectangle(
                boardOffsetX + snakePosition[i][0] * cellSize,
                boardOffsetY + snakePosition[i][1] * cellSize,
                cellSize, cellSize,
                snakeBodyColor);
        }

        // Score
        DrawText(("Score: " + std::to_string(score)).c_str(), windowWidth - 250, 50, 40, RAYWHITE);

        // Title
        DrawText("SNAKE", 50, 50, 40, RAYWHITE);

        // GAME OVER SCREEN
        if (gameOver)
        {
            const char* text = "GAME OVER!";
            int textWidth = MeasureText(text, 60);
            DrawText(text, (windowWidth - textWidth) / 2, windowHeight / 2 - 50, 60, RED);

            const char* sub = "Press ESC to Exit";
            int subWidth = MeasureText(sub, 20);
            DrawText(sub, (windowWidth - subWidth) / 2, windowHeight / 2 + 20, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
