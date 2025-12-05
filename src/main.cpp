#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cmath>

// --- Enum ---
enum GameMode { EASY = 0, NORMAL = 1, HARD = 2, STORY = 3 };

// --- Struct to hold the entire Game State ---
// This keeps main() clean and allows us to pass "game" to functions easily.
struct GameState {
    int stateofgame = 0; // 0 = Menu, 2 = Gameplay
    int menuOption = 1;
    
    GameMode currentMode = NORMAL;
    std::string theme = "Classic";
    int storyLevel = 1;

    bool gameOver = false;
    int score = 0;
    int highscore = 0;

    // Snake Data
    int snakeLength = 4;
    int snakePosition[1024][2] = {0};
    int snakeX, snakeY; // Initial positions
    char key = 'R';

    // Food Data
    int foodX = 0;
    int foodY = 0;

    // Movement & Timing
    float moveTimer = 0.0f;
    float moveInterval = 0.1f;
    bool allowMove = false;

    // Story Transition
    bool isLevelTransitioning = false;
    float transitionTimer = 0.0f;
    const float transitionDuration = 3.0f;

    // Hurdles
    int hurdles[100][2];
    int hurdleCount = 0;

    // File State
    bool hasSaveFile = false;
};

// --- Global Constants (Calculated in Main) ---
int screenWidth;
int screenHeight;
const int cellSize = 40;
int boardWidth, boardHeight;
int gridCountX, gridCountY;
int boardOffsetX, boardOffsetY;

// --- Function Prototypes ---
// (This tells the compiler these functions exist below main)
void InitGameGrid();
void InitHurdles(GameState &game);
void LoadHighscore(GameState &game);
void CheckSaveFile(GameState &game);
bool IsTileBlocked(int x, int y, GameState &game, bool hurdlesActive);
void ResetGame(GameState &game, bool fullReset);
void LoadGame(GameState &game);
void SaveGame(GameState &game);
void UpdateMenu(GameState &game);
void UpdateGameplay(GameState &game);
void DrawMenu(GameState &game);
void DrawGameplay(GameState &game);

// ==========================================
//              MAIN FUNCTION
// ==========================================
int main()
{
    InitWindow(0, 0, "Snake Game - Ultimate Version");
    SetTargetFPS(60);

    InitGameGrid(); // Calculate board size/offsets

    // Initialize State
    GameState game;
    game.snakeX = gridCountX / 2;
    game.snakeY = gridCountY / 2;
    
    // Setup initial Environment
    InitHurdles(game);
    LoadHighscore(game);
    CheckSaveFile(game);
    ResetGame(game, true); // Initial reset to set safe defaults

    // --- Main Loop ---
    while (true)
    {
        
        // 1. UPDATE LOGIC
        if (game.stateofgame == 0)
        {
            UpdateMenu(game);
        }
        if (game.stateofgame == 2)
        {
            if (IsKeyPressed(KEY_ESCAPE)) game.stateofgame = 0;
            UpdateGameplay(game);
        }

        // 2. DRAWING
        BeginDrawing();
        if (game.stateofgame == 0)
        {
            DrawMenu(game);
        }
        else if (game.stateofgame == 2)
        {
            DrawGameplay(game);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// ==========================================
//          FUNCTION DEFINITIONS
// ==========================================

// Helper: Calculates grid dimensions based on screen size
void InitGameGrid() {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    
    int rawBoardWidth = screenWidth - 120;
    int rawBoardHeight = screenHeight - 120;
    boardWidth = rawBoardWidth - (rawBoardWidth % cellSize);
    boardHeight = rawBoardHeight - (rawBoardHeight % cellSize);

    gridCountX = boardWidth / cellSize;
    gridCountY = boardHeight / cellSize;

    boardOffsetX = (screenWidth - boardWidth) / 2;
    boardOffsetY = (screenHeight - boardHeight) / 2;
}

// Helper: The block checking logic from original code
bool IsTileBlocked(int x, int y, GameState &game, bool hurdlesActive)
{
    for (int i = 0; i < game.snakeLength; i++)
    {
        if (game.snakePosition[i][0] == x && game.snakePosition[i][1] == y)
            return true;
    }

    if (hurdlesActive)
    {
        for (int i = 0; i < game.hurdleCount; i++)
        {
            if (game.hurdles[i][0] == x && game.hurdles[i][1] == y)
                return true;
        }
    }
    return false;
}

// Resets variables for a new game or restart
void ResetGame(GameState &game, bool fullReset)
{
    if (fullReset) {
        game.gameOver = false;
        game.score = 0;
        game.storyLevel = 1;
        game.snakeLength = 4;
        game.key = 'R';
        game.moveTimer = 0.0f;
        game.moveInterval = 0.1f;
        game.allowMove = true;
        game.isLevelTransitioning = false;
        game.transitionTimer = 0.0f;
    }

    // Reset Position to Center
    int cx = gridCountX / 2;
    int cy = gridCountY / 2;
    for (int i = 0; i < game.snakeLength; i++)
    {
        game.snakePosition[i][0] = cx;
        game.snakePosition[i][1] = cy;
    }

    // Spawn Food
    bool hurdlesActive = (game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 3));
    do
    {
        game.foodX = GetRandomValue(0, gridCountX - 1);
        game.foodY = GetRandomValue(0, gridCountY - 1);
    } while (IsTileBlocked(game.foodX, game.foodY, game, hurdlesActive));
}

void InitHurdles(GameState &game)
{
    int lastX = gridCountX - 1;
    int lastY = gridCountY - 1;
    int idx = 0;

    // Corners
    game.hurdles[idx][0] = 0; game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = 1; game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = 2; game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = 0; game.hurdles[idx++][1] = 1;
    game.hurdles[idx][0] = 0; game.hurdles[idx++][1] = 2;

    game.hurdles[idx][0] = lastX; game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = lastX - 1; game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = lastX - 2; game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = lastX; game.hurdles[idx++][1] = lastY - 1;
    game.hurdles[idx][0] = lastX; game.hurdles[idx++][1] = lastY - 2;

    game.hurdles[idx][0] = lastX; game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = lastX - 1; game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = lastX - 2; game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = lastX; game.hurdles[idx++][1] = 1;
    game.hurdles[idx][0] = lastX; game.hurdles[idx++][1] = 2;

    game.hurdles[idx][0] = 0; game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = 1; game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = 2; game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = 0; game.hurdles[idx++][1] = lastY - 1;
    game.hurdles[idx][0] = 0; game.hurdles[idx++][1] = lastY - 2;

    // Middle Barrier
    int gap = 4;
    int startY = (gridCountY - gap) / 2 - 1;
    for (int i = 0; i < 7; i++)
    {
        game.hurdles[idx][0] = i + (gridCountX - 7) / 2;
        game.hurdles[idx++][1] = startY;
        game.hurdles[idx][0] = i + (gridCountX - 7) / 2;
        game.hurdles[idx++][1] = startY + gap + 1;
    }
    game.hurdleCount = idx;
}

void LoadHighscore(GameState &game) {
    std::ifstream hsFileIn("highscore.txt");
    if (hsFileIn.is_open()) {
        hsFileIn >> game.highscore;
        hsFileIn.close();
    }
}

void CheckSaveFile(GameState &game) {
    std::ifstream savecheck("savefile.txt");
    if (savecheck.is_open()) {
        game.hasSaveFile = true;
        savecheck.close();
    }
}

void LoadGame(GameState &game) {
    if (game.hasSaveFile) {
        std::ifstream load("savefile.txt");
        if (load.is_open()) {
            int modeInt;
            load >> game.snakeLength >> game.score >> game.key >> game.foodX >> game.foodY >> modeInt;
            game.currentMode = (GameMode)modeInt;
            for (int i = 0; i < game.snakeLength; i++) {
                load >> game.snakePosition[i][0] >> game.snakePosition[i][1];
            }
            load.close();
            game.allowMove = true;
            game.stateofgame = 2;
            game.isLevelTransitioning = false;
        }
    }
}

void SaveGame(GameState &game) {
    std::ofstream save("savefile.txt");
    if (save.is_open()) {
        save << game.snakeLength << "\n"
             << game.score << "\n"
             << game.key << "\n"
             << game.foodX << "\n"
             << game.foodY << "\n"
             << (int)game.currentMode;
        for (int i = 0; i < game.snakeLength; i++) {
            save << "\n" << game.snakePosition[i][0] << " " << game.snakePosition[i][1];
        }
        save.close();
        game.hasSaveFile = true;
    }
}

// --- UPDATE LOGIC FUNCTIONS ---

void UpdateMenu(GameState &game)
{
    if (IsKeyPressed(KEY_DOWN)) game.menuOption++;
    if (IsKeyPressed(KEY_UP)) game.menuOption--;
    if (game.menuOption > 5) game.menuOption = 1;
    if (game.menuOption < 1) game.menuOption = 5;

    if (game.menuOption == 4) // Mode selection
    {
        if (IsKeyPressed(KEY_RIGHT)) {
            int next = (int)game.currentMode + 1;
            if (next > 3) next = 0;
            game.currentMode = (GameMode)next;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            int prev = (int)game.currentMode - 1;
            if (prev < 0) prev = 3;
            game.currentMode = (GameMode)prev;
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        if (game.menuOption == 5) exit(0); // Exit
        if (game.menuOption == 3) // Theme
        {
            if (game.theme == "Classic") game.theme = "Desert";
            else game.theme = "Classic";
        }
        if (game.menuOption == 2) // New Game
        {
            game.stateofgame = 2;
            ResetGame(game, true);
        }
        if (game.menuOption == 1) // Continue
        {
            LoadGame(game);
        }
    }
}

void UpdateGameplay(GameState &game)
{
    if (game.gameOver) {
        if (game.hasSaveFile) { remove("savefile.txt"); game.hasSaveFile = false; }
        if (IsKeyPressed(KEY_R)) ResetGame(game, true);
        return;
    }

    bool wallsActive = (game.currentMode == NORMAL || game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 2));
    bool hurdlesActive = (game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 3));

    // Story Mode Logic
    if (game.currentMode == STORY)
    {
        int nextLevel = 1;
        if (game.score >= 50 && game.score < 100) nextLevel = 2;
        else if (game.score >= 100) nextLevel = 3;

        if (nextLevel > game.storyLevel)
        {
            game.storyLevel = nextLevel;
            game.isLevelTransitioning = true;
            game.transitionTimer = game.transitionDuration;
            
            // Safe Spawn Pattern logic
            game.key = 'R';
            int cx = gridCountX / 2;
            int cy = gridCountY / 2;
            for (int i = 0; i < game.snakeLength; i++) {
                game.snakePosition[i][0] = cx - i;
                game.snakePosition[i][1] = cy;
            }
            
            // Respawn Food
            bool nextHurdles = (game.storyLevel >= 3);
            do {
                game.foodX = GetRandomValue(0, gridCountX - 1);
                game.foodY = GetRandomValue(0, gridCountY - 1);
            } while (IsTileBlocked(game.foodX, game.foodY, game, nextHurdles));
        }
    }

    // Highscore Update
    if (game.score > game.highscore) {
        game.highscore = game.score;
        std::ofstream hsOut("highscore.txt");
        if (hsOut.is_open()) { hsOut << game.highscore; hsOut.close(); }
    }

    // Transition Timer
    if (game.isLevelTransitioning) {
        game.transitionTimer -= GetFrameTime();
        if (game.transitionTimer <= 0) {
            game.isLevelTransitioning = false;
            game.transitionTimer = 0;
            game.allowMove = true;
        }
        return; // Skip movement
    }

    // Input Handling
    if (game.allowMove) {
        if (IsKeyPressed(KEY_RIGHT) && game.key != 'L') { game.key = 'R'; game.allowMove = false; }
        else if (IsKeyPressed(KEY_LEFT) && game.key != 'R') { game.key = 'L'; game.allowMove = false; }
        else if (IsKeyPressed(KEY_UP) && game.key != 'D') { game.key = 'U'; game.allowMove = false; }
        else if (IsKeyPressed(KEY_DOWN) && game.key != 'U') { game.key = 'D'; game.allowMove = false; }
    }

    // Movement Loop
    game.moveTimer += GetFrameTime();
    if (game.moveTimer >= game.moveInterval)
    {
        game.moveTimer = 0.0f;
        game.allowMove = true;

        int nextX = game.snakePosition[0][0];
        int nextY = game.snakePosition[0][1];

        if (game.key == 'R') nextX++;
        if (game.key == 'L') nextX--;
        if (game.key == 'U') nextY--;
        if (game.key == 'D') nextY++;

        // Collision: Walls
        if (nextX < 0 || nextX >= gridCountX || nextY < 0 || nextY >= gridCountY) {
            if (wallsActive) {
                game.gameOver = true;
                return;
            } else {
                if (nextX < 0) nextX = gridCountX - 1;
                if (nextX >= gridCountX) nextX = 0;
                if (nextY < 0) nextY = gridCountY - 1;
                if (nextY >= gridCountY) nextY = 0;
            }
        }

        // Collision: Hurdles
        if (hurdlesActive) {
            for (int i = 0; i < game.hurdleCount; i++) {
                if (nextX == game.hurdles[i][0] && nextY == game.hurdles[i][1]) {
                    game.gameOver = true;
                    return;
                }
            }
        }

        // Move Body
        for (int i = game.snakeLength; i > 0; i--) {
            game.snakePosition[i][0] = game.snakePosition[i - 1][0];
            game.snakePosition[i][1] = game.snakePosition[i - 1][1];
        }
        game.snakePosition[0][0] = nextX;
        game.snakePosition[0][1] = nextY;

        // Collision: Food
        if (nextX == game.foodX && nextY == game.foodY) {
            game.snakeLength++;
            game.score += 10;
            if (game.moveInterval > 0.05f) game.moveInterval -= 0.001f;
            do {
                game.foodX = GetRandomValue(0, gridCountX - 1);
                game.foodY = GetRandomValue(0, gridCountY - 1);
            } while (IsTileBlocked(game.foodX, game.foodY, game, hurdlesActive));
        }

        // Collision: Self
        if (!game.gameOver) {
            for (int i = 1; i < game.snakeLength; i++) {
                if (game.snakePosition[0][0] == game.snakePosition[i][0] && game.snakePosition[0][1] == game.snakePosition[i][1]) {
                    game.gameOver = true;
                }
            }
        }

        if (!game.gameOver) SaveGame(game);
    }
}

// --- DRAWING FUNCTIONS ---

void DrawMenu(GameState &game)
{
    // Define Colors Locally based on theme selection
    Color bg, box, sel, text, textSel;
    if (game.theme == "Classic") {
        bg = {185, 205, 160, 255}; box = {160, 190, 140, 255};
        sel = {120, 150, 110, 255}; text = {60, 80, 50, 255}; textSel = WHITE;
    } else {
        bg = {240, 225, 185, 255}; box = {225, 210, 178, 255};
        sel = {25, 40, 75, 255}; text = {60, 45, 25, 255}; textSel = WHITE;
    }

    ClearBackground(bg);
    DrawText("SNAKE ULTIMATE", screenWidth / 2 - 200, 200, 50, text);
    std::string hsText = "High Score: " + std::to_string(game.highscore);
    DrawText(hsText.c_str(), screenWidth / 2 - 100, 260, 30, text);

    int startY = 250, gap = 70, boxW = 300, boxH = 50;

    // --- Options Loop for cleaner code ---
    const char* titles[] = {"Continue", "New Game", "Theme", "Mode", "Exit"};
    for (int i = 1; i <= 5; i++)
    {
        int yPos = startY + (gap * (i - 1))+100;
        std::string display = titles[i - 1];

        // Custom Labels
        if (i == 3) display = "Theme: " + game.theme;
        if (i == 4) {
            display = "Mode: ";
            if (game.currentMode == EASY) display += "< Easy >";
            else if (game.currentMode == NORMAL) display += "< Normal >";
            else if (game.currentMode == HARD) display += "< Hard >";
            else display += "< Story >";
        }

        if (game.menuOption == i)
        {
            if (i == 1 && !game.hasSaveFile) {
                DrawRectangle(screenWidth / 2 - 150, yPos, boxW, boxH, RED);
                DrawText(display.c_str(), screenWidth / 2 - 40, yPos + 15, 20, WHITE);
            } else {
                DrawRectangle(screenWidth / 2 - 150, yPos, boxW, boxH, sel);
                // Adjust text offset for Theme/Mode to match original look
                int offset = (i==3) ? 60 : (i==4 ? 70 : 40);
                if (i==2) offset = 45; if(i==5) offset = 20;
                DrawText(display.c_str(), screenWidth / 2 - offset, yPos + 15, 20, textSel);
            }
        }
        else
        {
            DrawRectangleLines(screenWidth / 2 - 150, yPos, boxW, boxH, box);
            int offset = (i==3) ? 60 : (i==4 ? 70 : 40);
             if (i==2) offset = 45; if(i==5) offset = 20;
            DrawText(display.c_str(), screenWidth / 2 - offset, yPos + 15, 20, text);
        }
    }
}

void DrawGameplay(GameState &game)
{
    // Define Colors Locally
    Color cBg, cGrid, cSnake, cFood, cMenuBg;
    if (game.theme == "Classic") {
        cBg = {150, 180, 110, 255}; cGrid = {100, 130, 90, 255};
        cSnake = {50, 70, 40, 255}; cFood = {220, 0, 0, 255}; cMenuBg = {185, 205, 160, 255};
    } else {
        cBg = {255, 246, 199, 255}; cGrid = {232, 223, 194, 255};
        cSnake = {15, 26, 51, 255}; cFood = {255, 79, 163, 255}; cMenuBg = {240, 225, 185, 255};
    }

    ClearBackground(cMenuBg);
    DrawRectangle(boardOffsetX, boardOffsetY, boardWidth, boardHeight, cBg);

    // Grid
    for (int i = 0; i <= gridCountX; i++)
        DrawLine(boardOffsetX + i * cellSize, boardOffsetY, boardOffsetX + i * cellSize, boardOffsetY + boardHeight, cGrid);
    for (int i = 0; i <= gridCountY; i++)
        DrawLine(boardOffsetX, boardOffsetY + i * cellSize, boardOffsetX + boardWidth, boardOffsetY + i * cellSize, cGrid);

    bool wallsActive = (game.currentMode == NORMAL || game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 2));
    bool hurdlesActive = (game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 3));

    // Hurdles
    if (hurdlesActive) {
        for (int i = 0; i < game.hurdleCount; i++)
            DrawRectangle(boardOffsetX + game.hurdles[i][0] * cellSize, boardOffsetY + game.hurdles[i][1] * cellSize, cellSize, cellSize, DARKGRAY);
    }

    // Food
    DrawRectangle(boardOffsetX + game.foodX * cellSize, boardOffsetY + game.foodY * cellSize, cellSize, cellSize, cFood);

    // Snake
    for (int i = 0; i < game.snakeLength; i++)
        DrawRectangle(boardOffsetX + game.snakePosition[i][0] * cellSize, boardOffsetY + game.snakePosition[i][1] * cellSize, cellSize, cellSize, cSnake);

    // Walls
    if (wallsActive) {
        DrawRectangleLinesEx((Rectangle){(float)boardOffsetX, (float)boardOffsetY, (float)boardWidth, (float)boardHeight}, 4, RED);
    }

    // UI
    DrawText(TextFormat("Score: %i", game.score), 20, 20, 30, WHITE);
    
    std::string mText; Color mColor;
    if (game.currentMode == EASY) { mText = "EASY"; mColor = GREEN; }
    else if (game.currentMode == NORMAL) { mText = "NORMAL"; mColor = ORANGE; }
    else if (game.currentMode == HARD) { mText = "HARD"; mColor = RED; }
    else { mText = "STORY - LVL " + std::to_string(game.storyLevel); mColor = SKYBLUE; }
    DrawText(mText.c_str(), screenWidth / 2 - MeasureText(mText.c_str(), 30) / 2, 20, 30, mColor);

    // Transition Overlay
    if (game.isLevelTransitioning) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 100});
        std::string levelMsg = "LEVEL " + std::to_string(game.storyLevel);
        DrawText(levelMsg.c_str(), screenWidth / 2 - MeasureText(levelMsg.c_str(), 60) / 2, screenHeight / 2 - 100, 60, GOLD);
        std::string countStr = std::to_string((int)ceil(game.transitionTimer));
        DrawText(countStr.c_str(), screenWidth / 2 - MeasureText(countStr.c_str(), 80) / 2, screenHeight / 2, 80, WHITE);
        DrawText("Get Ready!", screenWidth / 2 - MeasureText("Get Ready!", 30) / 2, screenHeight / 2 + 80, 30, LIGHTGRAY);
    }

    // Game Over Overlay
    if (game.gameOver) {
        DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 60) / 2, screenHeight / 2 - 60, 60, RED);
        DrawText("Press ESC for Menu", screenWidth / 2 - MeasureText("Press ESC for Menu", 20) / 2, screenHeight / 2 + 10, 20, LIGHTGRAY);
        DrawText("Press 'R' to RESTART", screenWidth / 2 - MeasureText("Press 'R' to RESTART", 25) / 2, screenHeight / 2 + 40, 25, GOLD);
    }
}
