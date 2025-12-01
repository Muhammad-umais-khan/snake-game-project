#include <raylib.h>
#include <stdlib.h>
// #include <math.h> commented this out as it's not used
#include <string>

void DrawingFood(int foodX, int foodY, int cellSize, int boardOffsetX, int boardOffsetY, Color foodColor)
{
    // Draw Food (Offset added)
    DrawRectangle(
        boardOffsetX + (foodX * cellSize),
        boardOffsetY + (foodY * cellSize),
        cellSize, cellSize, foodColor);
}

int main()
{
    // this makes the game window use the current screen resolution
    InitWindow(0, 0, "Snake Game - Square View");

    ToggleFullscreen(); // fullscreen mode for better immersion ig
    SetTargetFPS(10);

    // getting the screen dimensions
    const int windowWidth = GetScreenWidth();
    const int windowHeight = GetScreenHeight();

    const int cellSize = 60;

    // checking which is smaller
    int minDimension = (windowWidth < windowHeight) ? windowWidth : windowHeight;

    // screen ke size ko pehla 120 se minus karaya for some padding
    int rawBoardSize = minDimension - 120;

    // phir check kia ke whether it's divisible by cellSize or not.
    // if not then we minus the remainder to make it divisible
    const int boardSize = rawBoardSize - (rawBoardSize % cellSize);

    const int gridCount = boardSize / cellSize;

    const int boardOffsetX = (windowWidth - boardSize) / 2;
    const int boardOffsetY = (windowHeight - boardSize) / 2;

    bool gameOver = false;
    int score = 0;

    int snakeLength = 4;

    // snake will spawn in middle of the GRID
    int snakeX = gridCount / 2;
    int snakeY = gridCount / 2;
    int snakePosition[255][2] = {0};

    // entire snake body
    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }

    char key = 'R';

    // Color Palette
    Color windowBgColor = {30, 30, 30, 255}; // Dark grey
    Color boardBgColor = {150, 180, 110, 255};
    Color gridColor = {100, 130, 90, 255};
    Color foodColor = {220, 0, 0, 255};
    Color snakebodyColor = {50, 70, 40, 255};
    Color textColor = WHITE;

    // Food Initialization
    int foodX = 0;
    int foodY = 0;
    bool isOnSnake;

    // Initial random food placement
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
    } while (isOnSnake == true);

    // Main Game Loop
    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            // Input Handling
            if (IsKeyDown(KEY_RIGHT) && key != 'L')
                key = 'R';
            if (IsKeyDown(KEY_LEFT) && key != 'R')
                key = 'L';
            if (IsKeyDown(KEY_UP) && key != 'D')
                key = 'U';
            if (IsKeyDown(KEY_DOWN) && key != 'U')
                key = 'D';

            // 1. Move Body Segments
            for (int i = snakeLength; i > 0; i--)
            {
                snakePosition[i][0] = snakePosition[i - 1][0];
                snakePosition[i][1] = snakePosition[i - 1][1];
            }

            // 2. Move Head
            if (key == 'R')
                snakePosition[0][0] += 1;
            if (key == 'L')
                snakePosition[0][0] -= 1;
            if (key == 'U')
                snakePosition[0][1] -= 1;
            if (key == 'D')
                snakePosition[0][1] += 1;

            // 3. Boundary Logic (Screen Wrapping)
            if (snakePosition[0][0] >= gridCount)
                snakePosition[0][0] = 0;
            if (snakePosition[0][0] < 0)
                snakePosition[0][0] = gridCount - 1;

            if (snakePosition[0][1] >= gridCount)
                snakePosition[0][1] = 0;
            if (snakePosition[0][1] < 0)
                snakePosition[0][1] = gridCount - 1;

            // 4. Food Collision Logic
            if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
            {
                snakeLength++;
                score += 10;

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

                } while (isOnSnake == true);
            }

            // 5. Self Collision Logic
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
        ClearBackground(windowBgColor); // Clear the whole window with dark grey

        // Draw the Square Game Board Background
        DrawRectangle(boardOffsetX, boardOffsetY, boardSize, boardSize, boardBgColor);

        // Draw Grid (Offset by boardOffsetX/Y)
        for (int i = 0; i < gridCount; i++)
        {
            for (int j = 0; j < gridCount; j++)
            {
                DrawRectangleLines(
                    boardOffsetX + (i * cellSize),
                    boardOffsetY + (j * cellSize),
                    cellSize, cellSize, gridColor);
            }
        }

        // Draw Food (Offset added)
        DrawingFood(foodX, foodY, cellSize, boardOffsetX, boardOffsetY, foodColor);

        // Draw Snake (Offset added)
        for (int i = 0; i < snakeLength; i++)
        {
            DrawRectangle(
                boardOffsetX + (snakePosition[i][0] * cellSize),
                boardOffsetY + (snakePosition[i][1] * cellSize),
                cellSize, cellSize, snakebodyColor);
        }

        // Draw Score
        std::string scoretext = "Score: " + std::to_string(score);
        DrawText(scoretext.c_str(), windowWidth - 250, 50, 40, WHITE);

        // Draw Title
        DrawText("SNAKE", 50, 50, 40, WHITE);

        if (gameOver)
        {
            const char *text = "GAME OVER!";
            int textWidth = MeasureText(text, 60);
            DrawText(text, (windowWidth - textWidth) / 2, windowHeight / 2 - 50, 60, RED);

            const char *subtext = "Press ESC to exit";
            int subWidth = MeasureText(subtext, 20);
            DrawText(subtext, (windowWidth - subWidth) / 2, windowHeight / 2 + 20, 20, LIGHTGRAY);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}