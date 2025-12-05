# README — Snake Game (Raylib, C++)

**Goal:** Teach a beginner every important thing the program does. This README explains the code in plain language and walks through each section and important lines so you understand how the game works and why each part is there.

---

## Quick overview

This is a Snake game written in C++ using [raylib](https://www.raylib.com/) for graphics and input. The program:

* Opens a fullscreen window
* Builds a rectangular grid board
* Lets the player control a snake to eat food
* Supports modes: Easy, Normal, Hard and Story
* Saves/loads high score and a momentary save file
* Adds obstacles (hurdles) and world-wrapping or wall collisions depending on mode
* Has a level transition mechanic for Story mode

---

## How to compile & run (short)

1. Install raylib for your platform.
2. Save the `.cpp` file.
3. Compile with a C++ compiler linking raylib, for example:

   ```
   g++ -o snake game.cpp -lraylib -lGL -lm -lpthread -ldl -lrt
   ```

   (Exact flags depend on your OS; see raylib docs.)
4. Run `./snake`.

---

## File-level explanation (top → bottom)

Below the code is explained in logical sections. For each small group of lines you'll find: the code snippet and a simple explanation.

---

### 1) Includes and headers

```cpp
#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cmath> // Added: Required for the ceil() function
```

**Explanation (line-by-line):**

* `#include <raylib.h>` — imports Raylib functions (window creation, drawing, input, etc.).
* `#include <stdlib.h>` — standard C library utilities (e.g., `rand`/`srand`, but here used for general utilities).
* `#include <string>` — C++ string type `std::string`.
* `#include <fstream>` — file stream classes (`std::ifstream`, `std::ofstream`) for reading/writing files.
* `#include <vector>` — C++ dynamic array container (not heavily used in this file but included).
* `#include <cstdio>` — C-style I/O functions (e.g., `remove()` is used later).
* `#include <cmath>` — math functions; `ceil()` is used to show countdown numbers.

**Why these matter:** They provide the building blocks: drawing, input, text handling, file saving, and some math.

---

### 2) Enum: Game Mode

```cpp
enum GameMode
{
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
    STORY = 3
};
```

**Explanation:**

* `enum GameMode` defines named integer constants to represent the game difficulty/mode.
* Using `EASY`, `NORMAL`, `HARD`, `STORY` makes code easier to read than raw numbers.

---

### 3) Helper function: IsTileBlocked

```cpp
bool IsTileBlocked(int x, int y, int snakePosition[][2], int snakeLength, int hurdles[][2], int hurdleCount, bool hurdlesActive)
{
    for (int i = 0; i < snakeLength; i++)
    {
        if (snakePosition[i][0] == x && snakePosition[i][1] == y)
            return true;
    }

    if (hurdlesActive)
    {
        for (int i = 0; i < hurdleCount; i++)
        {
            if (hurdles[i][0] == x && hurdles[i][1] == y)
                return true;
        }
    }

    return false;
}
```

**Explanation:**

* This function checks whether a given board cell `(x,y)` is unusable (blocked).
* It checks two things:

  * If the coordinates match any cell of the snake's body (`snakePosition`) → blocked.
  * If `hurdlesActive` is true, it checks if the coordinates match any hurdle → blocked.
* Returns `true` if blocked, otherwise `false`.
* Used when placing food or respawning to avoid placing objects inside the snake or obstacles.

---

### 4) `main()` — window & basic setup

```cpp
int main()
{
    // --- STEP 1: Full Screen Initialization ---
    InitWindow(0, 0, "Snake Game - Ultimate Version");
    SetTargetFPS(60);
```

**Explanation:**

* `int main()` is the program entry point.
* `InitWindow(0, 0, "…")` creates a window. Passing `0, 0` requests a fullscreen window (in raylib this uses the display resolution).
* `SetTargetFPS(60)` caps the game loop to approximately 60 frames per second for smooth and consistent timing.

---

### 5) Screen & cell size, board calculation

```cpp
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int cellSize = 40;
```

* `GetScreenWidth()` / `GetScreenHeight()` get the actual window (fullscreen) size.
* `cellSize = 40` sets the pixel size of each grid square. Larger cell → fewer grid tiles.

```cpp
    int rawBoardWidth = screenWidth - 120;
    int rawBoardHeight = screenHeight - 120;
    const int boardWidth = rawBoardWidth - (rawBoardWidth % cellSize);
    const int boardHeight = rawBoardHeight - (rawBoardHeight % cellSize);

    const int gridCountX = boardWidth / cellSize;
    const int gridCountY = boardHeight / cellSize;

    const int boardOffsetX = (screenWidth - boardWidth) / 2;
    const int boardOffsetY = (screenHeight - boardHeight) / 2;
```

**Explanation:**

* The board is made slightly smaller than the full screen by subtracting 120 pixels on each axis (space for UI / margins).
* `boardWidth` and `boardHeight` are rounded down to multiples of `cellSize` using `rawBoard % cellSize`. This ensures the grid fits perfectly with no half-cells.
* `gridCountX` / `gridCountY` count how many cells horizontally and vertically.
* `boardOffsetX` / `boardOffsetY` center the board in the screen by computing the margins on each side.

---

### 6) Game variables and state

```cpp
    int stateofgame = 0;
    int menuOption = 1;

    GameMode currentMode = NORMAL;
    std::string theme = "Classic";
    int storyLevel = 1;
```

**Explanation:**

* `stateofgame` tracks the screen/state: `0` means menu, `2` is gameplay (as used later). (The code uses `0` and `2` — `1` might be reserved for other screens in other versions.)
* `menuOption` stores which menu item is selected (1–5).
* `currentMode` starts at `NORMAL`.
* `theme` chooses color theme ("Classic" or "Desert").
* `storyLevel` for Story mode progression.

```cpp
    bool gameOver = false;
    int score = 0;
    int snakeLength = 4;
    int snakePosition[1024][2] = {0};
```

* `gameOver` flag ends a play session.
* `score` starts at 0.
* `snakeLength` initial body length: 4 blocks.
* `snakePosition` stores up to 1024 body segments as `[index][0]=x, [index][1]=y`. Initialised to zero.

```cpp
    // New Variables for Story Transition
    bool isLevelTransitioning = false;
    float transitionTimer = 0.0f;
    const float transitionDuration = 3.0f;
```

* These handle a short countdown/overlay when Story level changes (3 seconds by default).

```cpp
    int snakeX = gridCountX / 2;
    int snakeY = gridCountY / 2;
    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }
```

* Places the initial snake head (and body) at the grid center. Initially all segments are at the same position; later movement array-shifting will separate them.

```cpp
    char key = 'R';
    int foodX = 0;
    int foodY = 0;

    float moveTimer = 0.0f;
    float moveInterval = 0.1f;

    bool allowMove = false;
```

* `key` uses characters `'R','L','U','D'` to represent the current direction (Right, Left, Up, Down).
* `foodX`/`foodY` store current food coordinates.
* `moveTimer` increments with frame time; when it exceeds `moveInterval`, the snake moves one tile. This creates time-based movement independent of frame rate.
* `moveInterval` controls speed (lower = faster).
* `allowMove` prevents multiple direction changes between actual moves (to avoid immediate reversing causing a crash).

---

### 7) Hurdles (obstacles) arrays & initialization

```cpp
    const int maxHurdles = 100;
    int hurdles[maxHurdles][2];
    int hurdleCount = 0;
```

* Fixed-size array for up to `maxHurdles`. Each hurdle has x and y.

The next block (braced) sets up a pattern of hurdles:

* Small clusters at corners and edges.
* A middle barrier with a configurable `gap` so player can pass through.
* `idx` increments as hurdles are placed and finally `hurdleCount` stores how many were added.

**Why this approach:** Using an array and index is low-level but clear. The pattern is hard-coded so obstacles are consistent across runs.

---

### 8) Colors and menu color themes

```cpp
    Color bgClassic = {150, 180, 110, 255};
    Color gridClassic = {100, 130, 90, 255};
    Color snakeClassic = {50, 70, 40, 255};
    Color foodClassic = {220, 0, 0, 255};

    Color bgDesert = {255, 246, 199, 255};
    ...
```

* `Color` is a `raylib` struct with RGBA integers.
* Two themes (`Classic`, `Desert`) each define colors for background, grid, snake, and food.
* Additional colors for menu background, boxes, selected items, and text are defined separately.

```cpp
    Color currentMenuBg = classicMenuBg;
    ...
```

* `currentMenu*` variables hold whichever theme's menu colors are active.

---

### 9) High score loading

```cpp
    int highscore = 0;
    std::ifstream hsFileIn("highscore.txt");
    if (hsFileIn.is_open())
    {
        hsFileIn >> highscore;
        hsFileIn.close();
    }
```

* Tries to open `highscore.txt`. If present, reads the number inside and uses it as `highscore`.
* If file doesn't exist or can't open, `highscore` remains `0`.

---

### 10) Save-file presence check

```cpp
    bool hasSaveFile = false;
    {
        std::ifstream savecheck("savefile.txt");
        if (savecheck.is_open())
        {
            hasSaveFile = true;
            savecheck.close();
        }
    }
```

* Checks whether a temporary `savefile.txt` exists. This determines whether the menu's "Continue" option should be enabled.

---

### 11) Main game loop

```cpp
    while (true)
    {
        if(IsKeyPressed(KEY_ESCAPE) && stateofgame == 2) stateofgame = 0;
```

* `while(true)` forms the main loop. It runs until the program `break`s (menu Exit).
* If the user presses ESC during gameplay (`stateofgame == 2`), it sets the state back to menu (`0`).

---

### 12) Menu state (`stateofgame == 0`)

When `stateofgame == 0` the program:

* Begins drawing (raylib `BeginDrawing()` / `EndDrawing()` bracket).
* Clears the background to `currentMenuBg`.
* Processes up/down keys to move `menuOption`.
* Wraps menu selection between 1 and 5.
* If `menuOption == 4` (Mode row), Left/Right changes `currentMode` cycling through `EASY,NORMAL,HARD,STORY`.
* `KEY_ENTER` triggers actions depending on which menu option is selected:

Menu options mapping:

1. Continue — loads `savefile.txt` and sets `stateofgame = 2` if `hasSaveFile` is true.
2. New Game — resets variables and places food avoiding blocked tiles.
3. Theme — toggles theme strings and menu color variables.
4. Mode — changed via left/right keys (see above).
5. Exit — `break` out of loop and close the program.

It also draws the menu title, high score, and the five menu boxes (Continue, New Game, Theme, Mode, Exit). Visual state (selected, unselected, disabled) is drawn as different rectangles and text colors.

**Important details:**

* When selecting "New Game", it sets:

  * `stateofgame = 2` (gameplay)
  * `gameOver = false`, `score=0`, `storyLevel=1`, `snakeLength=4`, `key='R'`, `moveTimer=0`, `moveInterval=0.1f`, and `allowMove=true`.
  * Repositions snake segments to center.
  * Uses `IsTileBlocked()` in a `do-while` loop to place `foodX, foodY` in a free tile (avoid snake and hurdles if active).

* When selecting "Continue", it reads values from `savefile.txt`:

  * `snakeLength, score, key, foodX, foodY, modeInt`, and then each snake segment position.
  * Restores state and sets `stateofgame = 2`.

---

### 13) Gameplay state (`stateofgame == 2`) — top-level

When `stateofgame == 2` and `gameOver` is `false`, the code:

* Handles Story mode transitions: determines `nextLevel` by score thresholds (`>=50 → lvl2`, `>=100 → lvl3`). If `nextLevel > storyLevel`, it sets `isLevelTransitioning=true`, resets direction and body layout in a safe spawn pattern, respawns food away from blocked tiles, and starts `transitionTimer`.
* Determines `wallsActive` and `hurdlesActive` based on current mode and storyLevel:

  * `wallsActive` = true for NORMAL, HARD, or STORY levels ≥ 2.
  * `hurdlesActive` = true for HARD or STORY levels ≥ 3.
* Updates `highscore` file if `score > highscore`.

---

### 14) Transition timer block

```cpp
if (isLevelTransitioning)
{
    transitionTimer -= GetFrameTime();
    if (transitionTimer <= 0)
    {
        isLevelTransitioning = false;
        transitionTimer = 0;
        allowMove = true;
    }
    // Skip normal gameplay during transition
}
```

* While transitioning: countdown decreases by the frame time. When it reaches zero:

  * Stops transitioning, allows the player to move.
* While transitioning, the code intentionally skips movement logic to prevent the snake from moving during the overlay.

---

### 15) Movement & input handling (normal gameplay)

When not transitioning:

* If `allowMove` is true, key presses set `key` to `'R','L','U','D'` provided the new direction isn't directly opposite the current one (prevents immediate self-collision).
* `allowMove` is set false right after direction change to avoid multiple direction changes before the snake moves one tile.

Movement timing:

```cpp
moveTimer += GetFrameTime();
if (moveTimer >= moveInterval)
{
    moveTimer = 0.0f;
    allowMove = true;
    // compute nextX, nextY based on key
    // handle wall collisions or wrapping
    // check hurdles collision
    // update snakePosition array by shifting segments
    // detect food collision and grow snake
    // detect self-collision
    // save to savefile.txt (auto-save)
}
```

**Details:**

* `GetFrameTime()` returns the seconds since last frame — used so movement speed depends on real time, not frames.
* `nextX`,`nextY` are calculated by incrementing/decrementing the head coordinates based on `key`.
* Wall behavior:

  * If `wallsActive` and the head moves outside the grid → `gameOver = true`.
  * If walls are **not** active, the code wraps the position to the opposite side (toroidal world).
* Hurdles check iterates over hurdles and sets `gameOver` if head hits one (only if `hurdlesActive`).
* After computing `nextX,nextY` and validating collisions, the code updates the array `snakePosition` by shifting from end to start, then sets `snakePosition[0]` (head) to the new position.
* If the head matches the food position:

  * `snakeLength++` (grows by 1).
  * `score += 10`.
  * If `moveInterval > 0.05f`, it slightly speeds the snake by subtracting `0.001f`.
  * Food is respawned in an unblocked tile via `IsTileBlocked()`.
* Self-collision: checks if head equals any other segment → `gameOver = true`.
* Auto-save: writes the current essential game state to `savefile.txt` including `snakeLength`, `score`, `key`, `foodX`, `foodY`, `currentMode` and all snake segment positions. This is used by "Continue".

---

### 16) Handling ESC in gameplay

```cpp
if (IsKeyPressed(KEY_ESCAPE))
{
    stateofgame = 0;
}
```

* If the user presses ESC while in gameplay drawing code (this check appears twice in places), they are returned to the menu.

---

### 17) Drawing the gameplay screen

```cpp
BeginDrawing();

Color cBg = (theme == "Classic") ? bgClassic : bgDesert;
Color cGrid = (theme == "Classic") ? gridClassic : gridDesert;
Color cSnake = (theme == "Classic") ? snakeClassic : snakeDesert;
Color cFood = (theme == "Classic") ? foodClassic : foodDesert;
Color cMenuBg = (theme == "Classic") ? classicMenuBg : desertMenuBg;

ClearBackground(cMenuBg);
DrawRectangle(boardOffsetX, boardOffsetY, boardWidth, boardHeight, cBg);
```

* Chooses colors based on the selected theme and clears/draws the board rectangle.

Grid drawing:

```cpp
for (int i = 0; i <= gridCountX; i++)
    DrawLine(boardOffsetX + i * cellSize, boardOffsetY, boardOffsetX + i * cellSize, boardOffsetY + boardHeight, cGrid);
for (int i = 0; i <= gridCountY; i++)
    DrawLine(boardOffsetX, boardOffsetY + i * cellSize, boardOffsetX + boardWidth, boardOffsetY + i * cellSize, cGrid);
```

* Draws vertical and horizontal grid lines across the board.

Hurdles, food and snake:

```cpp
if (hurdlesActive)
{
    for (int i = 0; i < hurdleCount; i++)
        DrawRectangle(boardOffsetX + hurdles[i][0] * cellSize, boardOffsetY + hurdles[i][1] * cellSize, cellSize, cellSize, DARKGRAY);
}

DrawRectangle(boardOffsetX + foodX * cellSize, boardOffsetY + foodY * cellSize, cellSize, cellSize, cFood);

for (int i = 0; i < snakeLength; i++)
    DrawRectangle(boardOffsetX + snakePosition[i][0] * cellSize, boardOffsetY + snakePosition[i][1] * cellSize, cellSize, cellSize, cSnake);
```

* Draws hurdles (if active), then draws food tile, then draws each snake segment as a rectangle.

Walls indicator:

```cpp
if (wallsActive)
{
    DrawRectangleLinesEx((Rectangle){(float)boardOffsetX, (float)boardOffsetY, (float)boardWidth, (float)boardHeight}, 4, RED);
}
```

* If walls are active, draws a red border around the game board to visually show that hitting edges will kill the snake.

Score & mode text:

```cpp
DrawText(TextFormat("Score: %i", score), 20, 20, 30, WHITE);

// Mode label chooses text and color (GREEN, ORANGE, RED, SKYBLUE) depending on mode
DrawText(mText.c_str(), screenWidth / 2 - MeasureText(mText.c_str(), 30) / 2, 20, 30, mColor);
```

* Displays current score and a centered label showing mode (EASY, NORMAL, HARD, or STORY - LVL N).

---

### 18) Transition overlay drawing

When `isLevelTransitioning` is true:

```cpp
DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 100});
std::string levelMsg = "LEVEL " + std::to_string(storyLevel);
DrawText(levelMsg.c_str(), screenWidth / 2 - MeasureText(levelMsg.c_str(), 60) / 2, screenHeight / 2 - 100, 60, GOLD);

std::string countStr = std::to_string((int)ceil(transitionTimer));
DrawText(countStr.c_str(), screenWidth / 2 - MeasureText(countStr.c_str(), 80) / 2, screenHeight / 2, 80, WHITE);

DrawText("Get Ready!", screenWidth / 2 - MeasureText("Get Ready!", 30) / 2, screenHeight / 2 + 80, 30, LIGHTGRAY);
```

* Draws a semi-transparent overlay across the screen to dim the gameplay.
* Shows "LEVEL X" large at top, shows a countdown number `ceil(transitionTimer)` in the middle and a "Get Ready!" message below.
* Uses `ceil()` so a timer of 2.1 shows "3", giving a full-second feel.

---

### 19) Game Over screen & restart

When `gameOver` is true:

```cpp
DrawText("GAME OVER", ... RED);
DrawText("Press ESC for Menu", ... LIGHTGRAY);
DrawText("Press 'R' to RESTART", ... GOLD);

if (hasSaveFile)
{
    remove("savefile.txt");
    hasSaveFile = false;
}

if (IsKeyPressed(KEY_R))
{
    // reset many game variables as in New Game
}
```

* Displays "GAME OVER" and hints.
* If there was an autosave file, deletes it (makes Continue unavailable after a final death).
* If user presses 'R', the game resets initial state and places food safely (similar to starting a new game).

---

### 20) Program exit & cleanup

```cpp
    CloseWindow();
    return 0;
}
```

* `CloseWindow()` properly shuts down the window and frees raylib resources.
* `return 0;` signals program ended successfully.

---

## Extra notes, tips & assumptions

* **Coordinate system:** The code uses grid coordinates where `(0,0)` is the top-left cell inside the board. Graphics drawing converts grid coordinates to pixels using `boardOffsetX + x*cellSize` and `boardOffsetY + y*cellSize`.
* **Movement timing:** `moveInterval` controls speed. Lower values → snake moves more often. `moveTimer` + `GetFrameTime()` produces smooth, frame-independent timing.
* **Saving:** The program auto-saves after each successful move to `savefile.txt`. If you die, that file is removed. `highscore.txt` keeps the best score across runs.
* **Themes:** Switching themes only changes colors — the gameplay is identical.
* **Story mode:** Story has staging — early levels have wrapping (no walls) and later levels enable walls and hurdles progressively.
* **Collision checks:** The code uses multiple explicit loops to check collisions. `IsTileBlocked` is used for safe placement of food and respects `hurdlesActive`.

---

## Common beginner questions

* **Q: Why is `snakePosition` large (1024)?**
  A: It's a safe upper bound for maximum snake length. The game never grows beyond available array space. Using a fixed large array is simple and avoids dynamic allocation.

* **Q: Why `allowMove` and direction characters?**
  A: To prevent the player from flipping direction twice between moves (which could cause the snake's head to reverse into the neck and instantly die). `allowMove` is toggled so only one direction change is processed per movement interval.

* **Q: Why use `ceil(transitionTimer)` for the countdown?**
  A: To show whole seconds as a friendly countdown. `ceil(2.1) = 3` so users see "3", "2", "1", "0" each displayed approximately for a second.

* **Q: Can this code be cleaned or refactored?**
  A: Yes — many improvements are possible: use `std::vector` for snake and hurdles, introduce meaningful state enum values instead of `0/2`, factor drawing and logic into functions or classes, add proper initialization/shutdown, and handle file I/O errors more robustly.

---

## Where to go next (improvements & experiments)

* Convert `snakePosition` to `std::vector<std::pair<int,int>>` so the snake can dynamically grow without a fixed maximum.
* Move repeated code (like placing food safely) into small helper functions.
* Add sound effects (raylib supports simple audio).
* Add pause screen and a proper `PAUSE` state.
* Add levels with different obstacle patterns loaded from files.
* Add touch controls (for mobile) or joystick support.

---

If you'd like, I can produce:

* A **line-by-line** literal mapping that comments every single code line (even more verbose), or
* A **refactored** version of this program using `std::vector` and clearer states, or
* A shorter **cheat-sheet** summarizing only the most important lines to remember.

Tell me which follow-up you'd prefer and I'll produce it. Enjoy exploring the code! 🐍
