#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 900;
    int BallX = screenWidth / 2;
    int BallY = screenHeight / 2;
    char key = 'Z';
    Color black = {0, 0, 0, 255};
    Color white = {255, 255, 255, 255};
    Color red = {255, 73, 92, 255};
    int playerRadius = 20;
    int foodX;
    int foodY;
    int foodRadius = 10;

    InitWindow(screenWidth, screenHeight, "first game window by umais using raylib");
    SetTargetFPS(59);
    srand(time(NULL));

    do {
        foodX = rand() % screenWidth;
        foodY = rand() % screenHeight;
    } while (foodX == BallX && foodY == BallY);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) key = 'R';
        if (IsKeyDown(KEY_LEFT)) key = 'L';
        if (IsKeyDown(KEY_UP)) key = 'U';
        if (IsKeyDown(KEY_DOWN)) key = 'D';

        if (key == 'R') BallX += 5;
        if (key == 'L') BallX -= 5;
        if (key == 'U') BallY -= 5;
        if (key == 'D') BallY += 5;

        if (BallX > screenWidth) BallX = 0;
        if (BallX < 0) BallX = screenWidth;
        if (BallY > screenHeight) BallY = 0;
        if (BallY < 0) BallY = screenHeight;

        float dx = BallX - foodX;
        float dy = BallY - foodY;
        float distance = sqrt(dx*dx + dy*dy);

        if (distance <= playerRadius + foodRadius) {
            do {
                foodX = rand() % screenWidth;
                foodY = rand() % screenHeight;
            } while (foodX == BallX && foodY == BallY);
        }

        BeginDrawing();
        ClearBackground(black);
        DrawCircle(foodX, foodY, foodRadius, red);
        DrawCircle(BallX, BallY, playerRadius, white);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
