#include <raylib.h>
#include <stdlib.h>
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
    InitWindow(0, 0, "Snake Game - Wide View");

    ToggleFullscreen(); // fullscreen mode for better immersion ig
    SetTargetFPS(10);

    // getting the screen dimensions
    const int windowWidth = GetScreenWidth();
    const int windowHeight = GetScreenHeight();

    const int cellSize = 60;

    // --- CHANGED LOGIC START ---
    // Instead of finding minDimension, we calculate width and height separately
    // We still remove 120px for padding
    int rawBoardWidth = windowWidth - 120;
    int rawBoardHeight = windowHeight - 120;

    // Make sure both dimensions are divisible by cellSize
    const int boardWidth = rawBoardWidth - (rawBoardWidth % cellSize);
    const int boardHeight = rawBoardHeight - (rawBoardHeight % cellSize);

    // Calculate grid counts for X (width) and Y (height) separately
    const int gridCountX = boardWidth / cellSize;
    const int gridCountY = boardHeight / cellSize;

    // Recalculate offsets to center the rectangular board
    const int boardOffsetX = (windowWidth - boardWidth) / 2;
    const int boardOffsetY = (windowHeight - boardHeight) / 2;
    // --- CHANGED LOGIC END ---

    bool gameOver = false;
    int score = 0;

    int snakeLength = 4;

    // snake will spawn in middle of the GRID
    int snakeX = gridCountX / 2;
    int snakeY = gridCountY / 2;

    // Increased array size because a wider map allows for a longer snake
    int snakePosition[1024][2] = {0};

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
        // Updated to use specific X and Y grid counts
        foodX = GetRandomValue(0, gridCountX - 1);
        foodY = GetRandomValue(0, gridCountY - 1);
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
            // Updated to check against gridCountX for width
            if (snakePosition[0][0] >= gridCountX)
                snakePosition[0][0] = 0;
            if (snakePosition[0][0] < 0)
                snakePosition[0][0] = gridCountX - 1;

            // Updated to check against gridCountY for height
            if (snakePosition[0][1] >= gridCountY)
                snakePosition[0][1] = 0;
            if (snakePosition[0][1] < 0)
                snakePosition[0][1] = gridCountY - 1;

            // 4. Food Collision Logic
            if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
            {
                snakeLength++;
                score += 10;

                do
                {
                    isOnSnake = false;
                    // Updated Random limits
                    foodX = GetRandomValue(0, gridCountX - 1);
                    foodY = GetRandomValue(0, gridCountY - 1);

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

        // Draw the Rectangular Game Board Background
        DrawRectangle(boardOffsetX, boardOffsetY, boardWidth, boardHeight, boardBgColor);

        // Draw Grid (Updated loops to use gridCountX and gridCountY)
        for (int i = 0; i < gridCountX; i++)
        {
            for (int j = 0; j < gridCountY; j++)
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