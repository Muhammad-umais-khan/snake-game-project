#include <raylib.h>
#include <stdlib.h>
#include <string>

void DrawingFood(int foodX, int foodY, int cellSize, int boardOffsetX, int boardOffsetY, Color foodColor)
{
    DrawRectangle(
        boardOffsetX + (foodX * cellSize),
        boardOffsetY + (foodY * cellSize),
        cellSize, cellSize, foodColor);
}

int main()
{
    InitWindow(0, 0, "Snake Game - Wide View");

    ToggleFullscreen();
    SetTargetFPS(10);

    const int windowWidth = GetScreenWidth();
    const int windowHeight = GetScreenHeight();

    const int cellSize = 60;

    // BOARD SIZE (rectangle)
    int rawBoardWidth = windowWidth - 120;
    int rawBoardHeight = windowHeight - 120;

    const int boardWidth  = rawBoardWidth  - (rawBoardWidth  % cellSize);
    const int boardHeight = rawBoardHeight - (rawBoardHeight % cellSize);

    const int gridCountX = boardWidth  / cellSize;
    const int gridCountY = boardHeight / cellSize;

    const int boardOffsetX = (windowWidth  - boardWidth)  / 2;
    const int boardOffsetY = (windowHeight - boardHeight) / 2;

    bool gameOver = false;
    int score = 0;

    int snakeLength = 4;

    int snakeX = gridCountX / 2;
    int snakeY = gridCountY / 2;

    int snakePosition[1024][2] = {0};

    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }

    char key = 'R';

    // Colors
    Color windowBgColor = {30, 30, 30, 255};
    Color boardBgColor  = {150, 180, 110, 255};
    Color gridColor     = {100, 130, 90, 255};
    Color foodColor     = {220, 0, 0, 255};
    Color snakeBodyColor = {50, 70, 40, 255};

    // Food
    int foodX, foodY;
    bool isOnSnake;

    // Initial food placement
    do
    {
        isOnSnake = false;
        foodX = GetRandomValue(0, gridCountX - 1);
        foodY = GetRandomValue(0, gridCountY - 1);

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

    // ------------------------
    // MAIN GAME LOOP
    // ------------------------
    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            // INPUT
            if (IsKeyDown(KEY_RIGHT) && key != 'L') key = 'R';
            if (IsKeyDown(KEY_LEFT)  && key != 'R') key = 'L';
            if (IsKeyDown(KEY_UP)    && key != 'D') key = 'U';
            if (IsKeyDown(KEY_DOWN)  && key != 'U') key = 'D';

            // NEXT HEAD POSITION BEFORE MOVING
            int nextX = snakePosition[0][0];
            int nextY = snakePosition[0][1];

            if (key == 'R') nextX++;
            if (key == 'L') nextX--;
            if (key == 'U') nextY--;
            if (key == 'D') nextY++;

            // --------- ⭐ BOUNDARY COLLISION ⭐ ----------
            if (nextX < 0 || nextX >= gridCountX ||
                nextY < 0 || nextY >= gridCountY)
            {
                gameOver = true;
            }
            else
            {
                // MOVE BODY
                for (int i = snakeLength; i > 0; i--)
                {
                    snakePosition[i][0] = snakePosition[i - 1][0];
                    snakePosition[i][1] = snakePosition[i - 1][1];
                }

                // MOVE HEAD
                snakePosition[0][0] = nextX;
                snakePosition[0][1] = nextY;
            }

            // FOOD COLLISION
            if (!gameOver &&
                snakePosition[0][0] == foodX &&
                snakePosition[0][1] == foodY)
            {
                snakeLength++;
                score += 10;

                // NEW FOOD
                do
                {
                    isOnSnake = false;
                    foodX = GetRandomValue(0, gridCountX - 1);
                    foodY = GetRandomValue(0, gridCountY - 1);

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

            // SELF COLLISION
            for (int i = 1; i < snakeLength; i++)
            {
                if (snakePosition[0][0] == snakePosition[i][0] &&
                    snakePosition[0][1] == snakePosition[i][1])
                {
                    gameOver = true;
                }
            }
        }

        // ------------------------
        // DRAWING
        // ------------------------
        BeginDrawing();
        ClearBackground(windowBgColor);

        DrawRectangle(boardOffsetX, boardOffsetY, boardWidth, boardHeight, boardBgColor);

        // Grid lines
        for (int i = 0; i < gridCountX; i++)
            for (int j = 0; j < gridCountY; j++)
                DrawRectangleLines(
                    boardOffsetX + (i * cellSize),
                    boardOffsetY + (j * cellSize),
                    cellSize, cellSize,
                    gridColor);

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

        DrawText(("Score: " + std::to_string(score)).c_str(), windowWidth - 250, 50, 40, WHITE);
        DrawText("SNAKE", 50, 50, 40, WHITE);

        if (gameOver)
        {
            const char* text = "GAME OVER!";
            int w = MeasureText(text, 60);
            DrawText(text, (windowWidth - w) / 2, windowHeight / 2 - 50, 60, RED);

            const char* sub = "Press ESC to exit";
            int sw = MeasureText(sub, 20);
            DrawText(sub, (windowWidth - sw) / 2, windowHeight / 2 + 20, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
