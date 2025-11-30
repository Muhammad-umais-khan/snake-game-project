#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <string>
int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 640;
    const int cellSize = 40;
    const int gridWidth = screenWidth / cellSize;   // find the number of grid call horizantally
    const int gridHeight = screenHeight / cellSize; // find the number of grid call verically
    int score = 0;

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

    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT) && key != 'L')
            key = 'R';
        if (IsKeyDown(KEY_LEFT) && key != 'R')
            key = 'L';
        if (IsKeyDown(KEY_UP) && key != 'D')
            key = 'U';
        if (IsKeyDown(KEY_DOWN) && key != 'U')
            key = 'D';

        for (int i = snakeLength; i > 0; i--)
        { // move body segments
            snakePosition[i][0] = snakePosition[i - 1][0];
            snakePosition[i][1] = snakePosition[i - 1][1];
        }

        if (key == 'R')
            snakePosition[0][0] += 1; // move head right

        if (key == 'L')
            snakePosition[0][0] -= 1; // move head left

        if (key == 'U')
            snakePosition[0][1] -= 1; // move head up

        if (key == 'D')
            snakePosition[0][1] += 1; // move head Down

        if (snakePosition[0][0] > gridWidth - 1)
            snakePosition[0][0] = 0;
        if (snakePosition[0][0] < 0)
            snakePosition[0][0] = gridWidth;
        if (snakePosition[0][1] > gridHeight - 1)
            snakePosition[0][1] = 0;
        if (snakePosition[0][1] < 0)
            snakePosition[0][1] = gridHeight;

        // float dx = snakeX - foodX;
        // float dy = snakeY - foodY;
        // float distance = sqrt(dx*dx + dy*dy);

        if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
        {
            // distance <= playerRadius + cellSize/2
            do
            {
                foodX = GetRandomValue(0, gridWidth - 1);
                foodY = GetRandomValue(0, gridHeight - 1);
                snakeLength++;
                score += 10;
            } while (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY);
        }
        std::string scoretext = "Score: " + std::to_string(score);
        BeginDrawing();
        ClearBackground(black);
        for (int i = 0; i < gridWidth; i++)
        {
            for (int j = 0; j < gridHeight; j++)
            {
                DrawRectangleLines(i * cellSize, j * cellSize, cellSize, cellSize, GRAY);
            }
        }
        DrawText(scoretext.c_str(), screenWidth - 120, 20, 20, WHITE);
        DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, red);
        for (int i = 0; i < snakeLength; i++)
        {
            DrawRectangle((snakePosition[i][0]) * cellSize, (snakePosition[i][1]) * cellSize, cellSize, cellSize, white);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
