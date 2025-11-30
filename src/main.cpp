#include <raylib.h>
#include <stdlib.h>
#include <math.h>

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 600;
    const int cellSize = 40;
    const int gridWidth = screenWidth / cellSize;
    const int gridHeight = screenHeight / cellSize;

    int snakeLength = 4;
    int snakeX = screenWidth/20;
    int snakeY = screenHeight / 20;
    int snakePosition[255][2] = {{snakeX, snakeY}};
    
    char key = 'R';
    Color backgroundColor = {150, 180, 110, 255};
    Color gridColor = {100, 130, 90, 255};
    Color foodColor = {220, 0, 0, 255};
    Color snakebodyColor = {50, 70, 40, 255};
    int foodX=0;
    int foodY=0;
    InitWindow(screenWidth, screenHeight, "first game window by umais using raylib");
    SetTargetFPS(10);

    do {
        foodX = GetRandomValue(0, gridWidth - 1);
        foodY = GetRandomValue(0, gridHeight - 1);
    } while (foodX == snakeX && foodY == snakeY);

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_RIGHT)) key = 'R';
        if (IsKeyDown(KEY_LEFT)) key = 'L';
        if (IsKeyDown(KEY_UP)) key = 'U';
        if (IsKeyDown(KEY_DOWN)) key = 'D';

        for(int i=snakeLength; i>0; i--) { //move body segments 
                snakePosition[i][0] = snakePosition[i-1][0];
                snakePosition[i][1] = snakePosition[i-1][1];
            }

        if (key == 'R') snakePosition[0][0] +=1; //move head right

        if (key == 'L') snakePosition[0][0] -= 1; //move head left
        
        if (key == 'U') snakePosition[0][1] -= 1; //move head up
        
        if (key == 'D') snakePosition[0][1] += 1; //move head Down
        

        if (snakePosition[0][0] > gridWidth) snakePosition[0][0] = 0;
        if (snakePosition[0][0] < 0) snakePosition[0][0] = gridWidth;
        if (snakePosition[0][1] > gridHeight) snakePosition[0][1] = 0;
        if (snakePosition[0][1] < 0) snakePosition[0][1] = gridHeight;

        // float dx = snakeX - foodX;
        // float dy = snakeY - foodY;
        // float distance = sqrt(dx*dx + dy*dy);

        if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY ) { 
            //distance <= playerRadius + cellSize/2
            do {
                foodX = GetRandomValue(0, gridWidth - 1);
                foodY = GetRandomValue(0, gridHeight - 1);
                snakeLength++;
            } while (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY);
        }

        BeginDrawing();
        ClearBackground( backgroundColor );
        for(int i=0; i<gridWidth; i++) {
            for(int j=0; j<gridHeight; j++) {
                DrawRectangleLines(i*cellSize, j*cellSize, cellSize, cellSize,  gridColor);
            }
        }
        DrawRectangle(foodX*cellSize, foodY*cellSize, cellSize,cellSize ,foodColor);
        for(int i=0; i<snakeLength; i++) {
            DrawRectangle((snakePosition[i][0])*cellSize,(snakePosition[i][1])*cellSize, cellSize,cellSize, snakebodyColor );
        }
        EndDrawing();
    
    }
    CloseWindow();
    return 0;
}
