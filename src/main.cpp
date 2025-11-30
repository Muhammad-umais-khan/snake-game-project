#include <raylib.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    bool gameOver = false;

    const int screenWidth = 1000;
    const int screenHeight = 960;
    const int cellSize = 40;
    const int gridWidth = screenWidth / cellSize;
    const int gridHeight = screenHeight / cellSize;

    int snakeLength = 4;
    int snakeX = screenWidth / 20;
    int snakeY = screenHeight / 20;
    int snakePosition[255][2] = {{snakeX, snakeY}};

    char key = 'R';
    Color black = {0, 0, 0, 255};
    Color white = {255, 255, 255, 255};
    Color red = {255, 73, 92, 255};
    int foodX = 0;
    int foodY = 0;
    InitWindow(screenWidth, screenHeight, "first game window by umais using raylib");
    SetTargetFPS(10);

    do
    {
        foodX = GetRandomValue(0, gridWidth - 1);
        foodY = GetRandomValue(0, gridHeight - 1);
    } while (foodX == snakeX && foodY == snakeY);
    // Add this before the main loop
    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY; // or snakeY + i to start vertically
    }
    while (!WindowShouldClose())
    {
        if (!gameOver)
        {

            if (IsKeyDown(KEY_RIGHT) && key != 'L')
                key = 'R';
            if (IsKeyDown(KEY_LEFT) && key != 'R')
                key = 'L';
            if (IsKeyDown(KEY_UP) && key != 'D')
                key = 'U';
            if (IsKeyDown(KEY_DOWN) && key != 'U')
                key = 'D';

            for (int i = snakeLength - 1; i > 0; i--)
            { // move body segments
                snakePosition[i][0] = snakePosition[i - 1][0];
                snakePosition[i][1] = snakePosition[i - 1][1];
            }
            // Change this: for (int i = snakeLength; i > 0; i--)

            if (key == 'R')
                snakePosition[0][0] += 1; // move head right

            if (key == 'L')
                snakePosition[0][0] -= 1; // move head left

            if (key == 'U')
                snakePosition[0][1] -= 1; // move head up

            if (key == 'D')
                snakePosition[0][1] += 1; // move head Down

            if (snakePosition[0][0] >= gridWidth)
                snakePosition[0][0] = 0;
            if (snakePosition[0][0] < 0)
                snakePosition[0][0] = gridWidth - 1; // Wrap to last valid index

            if (snakePosition[0][1] >= gridHeight)
                snakePosition[0][1] = 0;
            if (snakePosition[0][1] < 0)
                snakePosition[0][1] = gridHeight - 1;

            // float dx = snakeX - foodX;
            // float dy = snakeY - foodY;
            // float distance = sqrt(dx*dx + dy*dy);

            if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
            {
                snakeLength++;

                // distance <= playerRadius + cellSize/2
                do
                {
                    foodX = GetRandomValue(0, gridWidth - 1);
                    foodY = GetRandomValue(0, gridHeight - 1);
                } while (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY);
            }
            for (int i = 1; i < snakeLength; i++)
            {
                if (snakePosition[0][0] == snakePosition[i][0] && snakePosition[0][1] == snakePosition[i][1] && i > 0)
                {
                    gameOver = true;
                }
            }
        }

        BeginDrawing();
        ClearBackground(black);

        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                DrawRectangleLines(i * cellSize, j * cellSize, cellSize, cellSize, GRAY);
            }
        }
        DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, red);

        for (int i = 0; i < snakeLength; i++)
        {

            DrawRectangle((snakePosition[i][0]) * cellSize, (snakePosition[i][1]) * cellSize, cellSize, cellSize, white);
        }
        if (gameOver)
        {
            DrawText("Game Over!", screenWidth / 2 - 50, screenHeight / 2 - 50, 50, RED);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
