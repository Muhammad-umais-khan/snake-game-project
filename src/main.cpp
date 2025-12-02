#include <raylib.h>
#include <stdlib.h>
#include <string>

// --------------------------------------------
// FUNCTION: Draw Food
// --------------------------------------------
void DrawingFood(int foodX, int foodY, int cellSize, int boardOffsetX, int boardOffsetY, Color foodColor)
{
    DrawRectangle(
        boardOffsetX + (foodX * cellSize),
        boardOffsetY + (foodY * cellSize),
        cellSize, cellSize, foodColor);
}

// --------------------------------------------
// FUNCTION: Check if tile is blocked by snake or hurdles
// --------------------------------------------
bool IsTileBlocked(int x, int y,
                   int snakePosition[][2], int snakeLength,
                   int hurdles[][2], int hurdleCount)
{
    // Check snake
    for (int i = 0; i < snakeLength; i++)
        if (snakePosition[i][0] == x && snakePosition[i][1] == y)
            return true;

    // Check hurdles
    for (int i = 0; i < hurdleCount; i++)
        if (hurdles[i][0] == x && hurdles[i][1] == y)
            return true;

    return false;
}

int main()
{
    InitWindow(0, 0, "Snake Game - Corner & Extra Hurdles");
    ToggleFullscreen();
    SetTargetFPS(10);

    const int windowWidth = GetScreenWidth();
    const int windowHeight = GetScreenHeight();
    const int cellSize = 40;

    // BOARD SIZE
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

    // Snake
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
    Color windowBgColor  = {30, 30, 30, 255};
    Color boardBgColor   = {150, 180, 110, 255};
    Color gridColor      = {100, 130, 90, 255};
    Color foodColor      = {220, 0, 0, 255};
    Color snakeBodyColor = {50, 70, 40, 255};

    // --------------------------------------------
    // HURDLES: Corner + Extra + Parallel lines
    // --------------------------------------------
    const int hurdleCount = 34; // previous 20 + 14 new tiles (2x7)
    int hurdles[hurdleCount][2];

    int lastX = gridCountX - 1;
    int lastY = gridCountY - 1;

    int idx = 0;

    // --- TOP-LEFT corner (┏ shape) ---
    hurdles[idx][0] = 0; hurdles[idx++][1] = 0;
    hurdles[idx][0] = 1; hurdles[idx++][1] = 0;
    hurdles[idx][0] = 2; hurdles[idx++][1] = 0; // horizontal
    hurdles[idx][0] = 0; hurdles[idx++][1] = 1;
    hurdles[idx][0] = 0; hurdles[idx++][1] = 2; // vertical

    // --- BOTTOM-RIGHT corner (┛ shape) ---
    hurdles[idx][0] = lastX;     hurdles[idx++][1] = lastY;
    hurdles[idx][0] = lastX - 1; hurdles[idx++][1] = lastY;
    hurdles[idx][0] = lastX - 2; hurdles[idx++][1] = lastY; // horizontal
    hurdles[idx][0] = lastX;     hurdles[idx++][1] = lastY - 1;
    hurdles[idx][0] = lastX;     hurdles[idx++][1] = lastY - 2; // vertical

    // --- EXTRA: UPPER-RIGHT L-SHAPE ---
    hurdles[idx][0] = lastX;     hurdles[idx++][1] = 0;
    hurdles[idx][0] = lastX - 1; hurdles[idx++][1] = 0;
    hurdles[idx][0] = lastX - 2; hurdles[idx++][1] = 0; // horizontal
    hurdles[idx][0] = lastX;     hurdles[idx++][1] = 1;
    hurdles[idx][0] = lastX;     hurdles[idx++][1] = 2; // vertical

    // --- EXTRA: LOWER-LEFT L-SHAPE ---
    hurdles[idx][0] = 0; hurdles[idx++][1] = lastY;
    hurdles[idx][0] = 1; hurdles[idx++][1] = lastY;
    hurdles[idx][0] = 2; hurdles[idx++][1] = lastY; // horizontal
    hurdles[idx][0] = 0; hurdles[idx++][1] = lastY - 1;
    hurdles[idx][0] = 0; hurdles[idx++][1] = lastY - 2; // vertical

    // --- TWO PARALLEL HORIZONTAL HURDLES ---
    int gap = 4; // 4 rows gap
    int startY = (gridCountY - gap) / 2 - 1; // above the gap
    for (int i = 0; i < 7; i++)
    {
        // Upper hurdle
        hurdles[idx][0] = i + (gridCountX - 7) / 2; 
        hurdles[idx++][1] = startY;

        // Lower hurdle
        hurdles[idx][0] = i + (gridCountX - 7) / 2; 
        hurdles[idx++][1] = startY + gap + 1;
    }

    // ---------------------------------------------
    // FOOD GENERATION (not on snake or hurdle)
    // ---------------------------------------------
    int foodX, foodY;
    do
    {
        foodX = GetRandomValue(0, gridCountX - 1);
        foodY = GetRandomValue(0, gridCountY - 1);
    } while (IsTileBlocked(foodX, foodY, snakePosition, snakeLength, hurdles, hurdleCount));

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

            // NEXT HEAD POSITION
            int nextX = snakePosition[0][0];
            int nextY = snakePosition[0][1];
            if (key == 'R') nextX++;
            if (key == 'L') nextX--;
            if (key == 'U') nextY--;
            if (key == 'D') nextY++;

            // WRAP AROUND EDGES
            if (nextX < 0) nextX = gridCountX - 1;
            if (nextX >= gridCountX) nextX = 0;
            if (nextY < 0) nextY = gridCountY - 1;
            if (nextY >= gridCountY) nextY = 0;

            // COLLISION WITH HURDLES
            for (int i = 0; i < hurdleCount; i++)
                if (nextX == hurdles[i][0] && nextY == hurdles[i][1])
                    gameOver = true;

            if (!gameOver)
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
            if (!gameOver && snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
            {
                snakeLength++;
                score += 10;
                do
                {
                    foodX = GetRandomValue(0, gridCountX - 1);
                    foodY = GetRandomValue(0, gridCountY - 1);
                } while (IsTileBlocked(foodX, foodY, snakePosition, snakeLength, hurdles, hurdleCount));
            }

            // SELF COLLISION
            for (int i = 1; i < snakeLength; i++)
                if (snakePosition[0][0] == snakePosition[i][0] &&
                    snakePosition[0][1] == snakePosition[i][1])
                    gameOver = true;
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

        // Hurdles
        for (int i = 0; i < hurdleCount; i++)
        {
            DrawRectangle(
                boardOffsetX + hurdles[i][0] * cellSize,
                boardOffsetY + hurdles[i][1] * cellSize,
                cellSize, cellSize,
                DARKGRAY
            );
        }

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
