# Snake Game in C using Raylib — Detailed Line-by-Line Explanation

This README explains **every line** of the provided Snake Game code in a clear, structured, and beginner-friendly way. It breaks the program into sections: window setup, variables, game logic, movement, food system, collision, and drawing.

---

## ⭐ Overview
This project is a simple **Snake Game** built using the **Raylib** graphics library. The game creates a window, draws a snake on a grid, allows it to move, spawn food, grow, and wrap around the screen.

---

## 📌 Full Line-by-Line Explanation

### **1. Header Files**
```c
#include <raylib.h>
#include <stdlib.h>
#include <math.h>
```
- `raylib.h` — for graphics, input, window, drawing.
- `stdlib.h` — for functions like `rand()` (used inside Raylib's random functions).
- `math.h` — used earlier but not required now; still included harmlessly.

---

## 📌 2. Main Function Begins
```c
int main() {
```
This starts the program. Everything happens inside `main()`.

---

## 🖥️ 3. Window and Grid Setup
```c
const int screenWidth = 840;
const int screenHeight = 960;
const int cellSize = 40;
const int gridWidth = screenWidth / cellSize;
const int gridHeight = screenHeight / cellSize;
```
- Screen resolution is **840×960**.
- Each grid cell is **40×40 pixels**.
- `gridWidth` and `gridHeight` represent number of cells horizontally/vertically.

---

## 🐍 4. Snake Initialization
```c
int snakeLength = 4;
int snakeX = screenWidth/20;
int snakeY = screenHeight / 20;
int snakePosition[255][2] = {{snakeX, snakeY}};
```
- Snake starts with a length of **4 segments**.
- Initial head coordinates calculated based on screen size.
- `snakePosition` stores (x, y) for **each segment**.
- First segment = head (`snakeX, snakeY`).

---

## 🎮 5. Movement Key
```c
char key = 'R';
```
- `'R'` means the snake will move to the **right** initially.

---

## 🎨 6. Color Definitions
```c
Color black = {0, 0, 0, 255};
Color white = {255, 255, 255, 255};
Color red = {255, 73, 92, 255};
```
Custom colors for background, snake, and food.

---

## 🍎 7. Food Variables
```c
int foodX=0;
int foodY=0;
```
Stores grid coordinates of food.

---

## 🪟 8. Initialize Window
```c
InitWindow(screenWidth, screenHeight, "first game window by umais using raylib");
SetTargetFPS(10);
```
- Creates the window.
- Sets game speed to **10 FPS** (snake moves 10 steps per second).

---

## 🍏 9. Generate First Food
```c
do {
    foodX = GetRandomValue(0, gridWidth - 1);
    foodY = GetRandomValue(0, gridHeight - 1);
} while (foodX == snakeX && foodY == snakeY);
```
- Randomly place food **inside the grid**.
- Loop ensures food does **not spawn on the snake’s head**.

---

## 🔄 10. Start Game Loop
```c
while (!WindowShouldClose()) {
```
Runs continuously until user closes the window.

---

## 🎮 11. Detect Keyboard Input
```c
if (IsKeyDown(KEY_RIGHT)) key = 'R';
if (IsKeyDown(KEY_LEFT)) key = 'L';
if (IsKeyDown(KEY_UP)) key = 'U';
if (IsKeyDown(KEY_DOWN)) key = 'D';
```
Updates movement direction based on arrow keys.

---

## 🧩 12. Move Snake Body
```c
for(int i=snakeLength; i>0; i--) {
    snakePosition[i][0] = snakePosition[i-1][0];
    snakePosition[i][1] = snakePosition[i-1][1];
}
```
- Each segment moves into the position of the one before it.
- This creates the snake “chain movement”.

---

## 🐍 13. Move Snake Head
```c
if (key == 'R') snakePosition[0][0] +=1;
if (key == 'L') snakePosition[0][0] -=1;
if (key == 'U') snakePosition[0][1] -=1;
if (key == 'D') snakePosition[0][1] +=1;
```
Moves head one cell in chosen direction.

---

## 🔁 14. Screen Wrap Logic
```c
if (snakePosition[0][0] > gridWidth) snakePosition[0][0] = 0;
if (snakePosition[0][0] < 0) snakePosition[0][0] = gridWidth;
if (snakePosition[0][1] > gridHeight) snakePosition[0][1] = 0;
if (snakePosition[0][1] < 0) snakePosition[0][1] = gridHeight;
```
Makes snake appear from the opposite side if it crosses the boundary.

---

## 🍎 15. Check Food Collision
```c
if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY) {
    do {
        foodX = GetRandomValue(0, gridWidth - 1);
        foodY = GetRandomValue(0, gridHeight - 1);
        snakeLength++;
    } while (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY);
}
```
- If head touches food → snake grows.
- New food is placed randomly.
- Ensures food doesn’t spawn on the head.

---

## 🎨 16. Start Drawing
```c
BeginDrawing();
ClearBackground(black);
```
- Starts a new frame.
- Fills screen with black.

---

## 🧱 17. Draw Grid
```c
for(int i=0; i<gridWidth; i++) {
    for(int j=0; j<gridHeight; j++) {
        DrawRectangleLines(i*cellSize, j*cellSize, cellSize, cellSize, GRAY);
    }
}
```
Draws grid lines for visual clarity.

---

## 🍏 18. Draw Food
```c
DrawRectangle(foodX*cellSize, foodY*cellSize, cellSize, cellSize, red);
```
Draws food block.

---

## 🐍 19. Draw Snake
```c
for(int i=0; i<snakeLength; i++) {
    DrawRectangle(snakePosition[i][0]*cellSize,
                  snakePosition[i][1]*cellSize,
                  cellSize, cellSize, white);
}
```
Each snake segment is drawn as a white square.

---

## 🏁 20. Finish Drawing
```c
EndDrawing();
```
Renders everything on the screen.

---

## 🚪 21. End of Game
```c
}
CloseWindow();
return 0;
```
- Exits loop.
- Closes game window.
- Program ends.

---

# 🎉 Final Notes
- The snake wraps across edges.
- Collision with itself is **not implemented yet**.
- Movement uses a simple grid + array shifting logic.

If you want, I can also create:
✔ A more advanced README
✔ A version with images & diagrams
✔ A version that explains memory, arrays, and loops in depth
✔ Add self-collision, score, sounds, menus, etc.

Just tell me, Muneeb!

