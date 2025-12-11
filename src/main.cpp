#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <cstdio>
#include <cmath>

// difficulty levels
enum GameMode
{
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
    STORY = 3
};

// keeps track of everything happening in the game
struct GameState
{
    int stateofgame = 0; // 0 = menu, 2 = playing
    int menuOption = 1;

    GameMode currentMode = NORMAL;
    std::string theme = "Classic";
    int storyLevel = 1;

    bool gameOver = false;
    int score = 0;
    int highscore = 0;

    // snake properties
    int snakeLength = 4;
    int snakePosition[1024][2] = {0};
    int snakeX, snakeY;
    char key = 'R';

    // food pos
    int foodX = 0;
    int foodY = 0;

    // speed control
    float moveTimer = 0.0f;
    float moveInterval = 0.1f;
    bool allowMove = false;

    // level switching
    bool isLevelTransitioning = false;
    float transitionTimer = 0.0f;
    const float transitionDuration = 3.0f;

    // obstacles
    int hurdles[100][2];
    int hurdleCount = 0;

    // save system
    bool hasSaveFile = false;
};

// globals (calculated later)
int screenWidth;
int screenHeight;
const int cellSize = 40;
int boardWidth, boardHeight;
int gridCountX, gridCountY;
int boardOffsetX, boardOffsetY;

// definitions
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

int main()
{
    InitWindow(0, 0, "Snake Game - Ultimate Version");
    SetTargetFPS(60);

    InitGameGrid(); // setup the board dimensions

    // setup state
    GameState game;
    game.snakeX = gridCountX / 2;
    game.snakeY = gridCountY / 2;

    // load assets and data
    InitHurdles(game);
    LoadHighscore(game);
    CheckSaveFile(game);
    ResetGame(game, true);

    // main loop
    while (true)
    {
        // update loop
        switch (game.stateofgame)
        {
        case 0:
            UpdateMenu(game);
            break;
        case 2:
            if (IsKeyPressed(KEY_ESCAPE))
                game.stateofgame = 0;
            UpdateGameplay(game);
            break;
        }

        // render loop
        BeginDrawing();
        switch (game.stateofgame)
        {
        case 0:
            DrawMenu(game);
            break;
        case 2:
            DrawGameplay(game);
            break;
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

//
// IMPLEMENTATION
//

void InitGameGrid()
{
    // getting the screen dimensions
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    // adding padding for better UX
    int rawBoardWidth = screenWidth - 120;
    int rawBoardHeight = screenHeight - 120;

    // phir check kia ke whether it's divisible by cellSize or not.
    // if not then we minus the remainder to make it divisible
    boardWidth = rawBoardWidth - (rawBoardWidth % cellSize);
    boardHeight = rawBoardHeight - (rawBoardHeight % cellSize);

    gridCountX = boardWidth / cellSize;
    gridCountY = boardHeight / cellSize;

    // center the board
    boardOffsetX = (screenWidth - boardWidth) / 2;
    boardOffsetY = (screenHeight - boardHeight) / 2;
}

// checks if a coordinate hits the snake or a wall
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

// resets game state
void ResetGame(GameState &game, bool fullReset)
{
    if (fullReset)
    {
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

    // reset snake to middle
    int cx = gridCountX / 2;
    int cy = gridCountY / 2;
    for (int i = 0; i < game.snakeLength; i++)
    {
        game.snakePosition[i][0] = cx;
        game.snakePosition[i][1] = cy;
    }

    // spawn food somewhere safe
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

    // set up corner walls
    game.hurdles[idx][0] = 0;
    game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = 1;
    game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = 2;
    game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = 0;
    game.hurdles[idx++][1] = 1;
    game.hurdles[idx][0] = 0;
    game.hurdles[idx++][1] = 2;

    game.hurdles[idx][0] = lastX;
    game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = lastX - 1;
    game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = lastX - 2;
    game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = lastX;
    game.hurdles[idx++][1] = lastY - 1;
    game.hurdles[idx][0] = lastX;
    game.hurdles[idx++][1] = lastY - 2;

    game.hurdles[idx][0] = lastX;
    game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = lastX - 1;
    game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = lastX - 2;
    game.hurdles[idx++][1] = 0;
    game.hurdles[idx][0] = lastX;
    game.hurdles[idx++][1] = 1;
    game.hurdles[idx][0] = lastX;
    game.hurdles[idx++][1] = 2;

    game.hurdles[idx][0] = 0;
    game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = 1;
    game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = 2;
    game.hurdles[idx++][1] = lastY;
    game.hurdles[idx][0] = 0;
    game.hurdles[idx++][1] = lastY - 1;
    game.hurdles[idx][0] = 0;
    game.hurdles[idx++][1] = lastY - 2;

    // barrier in the middle
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

void LoadHighscore(GameState &game)
{
    std::ifstream hsFileIn("highscore.txt");
    if (hsFileIn.is_open())
    {
        hsFileIn >> game.highscore;
        hsFileIn.close();
    }
}

void CheckSaveFile(GameState &game)
{
    std::ifstream savecheck("savefile.txt");
    if (savecheck.is_open())
    {
        game.hasSaveFile = true;
        savecheck.close();
    }
}

void LoadGame(GameState &game)
{
    if (game.hasSaveFile)
    {
        std::ifstream load("savefile.txt");
        if (load.is_open())
        {
            int modeInt;
            load >> game.snakeLength >> game.score >> game.key >> game.foodX >> game.foodY >> modeInt;
            game.currentMode = (GameMode)modeInt;
            for (int i = 0; i < game.snakeLength; i++)
            {
                load >> game.snakePosition[i][0] >> game.snakePosition[i][1];
            }
            load.close();
            game.allowMove = true;
            game.stateofgame = 2;
            game.isLevelTransitioning = false;
        }
    }
}

void SaveGame(GameState &game)
{
    std::ofstream save("savefile.txt");
    if (save.is_open())
    {
        save << game.snakeLength << "\n"
             << game.score << "\n"
             << game.key << "\n"
             << game.foodX << "\n"
             << game.foodY << "\n"
             << (int)game.currentMode;
        for (int i = 0; i < game.snakeLength; i++)
        {
            save << "\n"
                 << game.snakePosition[i][0] << " " << game.snakePosition[i][1];
        }
        save.close();
        game.hasSaveFile = true;
    }
}

//
// UPDATES
//

void UpdateMenu(GameState &game)
{
    if (IsKeyPressed(KEY_DOWN))
        game.menuOption++;
    if (IsKeyPressed(KEY_UP))
        game.menuOption--;
    if (game.menuOption > 5)
        game.menuOption = 1;
    if (game.menuOption < 1)
        game.menuOption = 5;

    // handle mode switching
    if (game.menuOption == 4)
    {
        if (IsKeyPressed(KEY_RIGHT))
        {
            int next = (int)game.currentMode + 1;
            if (next > 3)
                next = 0;
            game.currentMode = (GameMode)next;
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            int prev = (int)game.currentMode - 1;
            if (prev < 0)
                prev = 3;
            game.currentMode = (GameMode)prev;
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        switch (game.menuOption)
        {
        case 1: // continue
            LoadGame(game);
            break;
        case 2: // new game
            game.stateofgame = 2;
            ResetGame(game, true);
            break;
        case 3: // toggle theme
            if (game.theme == "Classic")
                game.theme = "Desert";
            else
                game.theme = "Classic";
            break;
        case 5: // exit
            exit(0);
            break;
        default:
            break;
        }
    }
}

void UpdateGameplay(GameState &game)
{
    if (game.gameOver)
    {
        if (game.hasSaveFile)
        {
            remove("savefile.txt");
            game.hasSaveFile = false;
        }
        if (IsKeyPressed(KEY_R))
            ResetGame(game, true);
        return;
    }

    bool wallsActive = (game.currentMode == NORMAL || game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 2));
    bool hurdlesActive = (game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 3));

    // handle story progression
    if (game.currentMode == STORY)
    {
        int nextLevel = 1;
        if (game.score >= 50 && game.score < 100)
            nextLevel = 2;
        else if (game.score >= 100)
            nextLevel = 3;

        if (nextLevel > game.storyLevel)
        {
            game.storyLevel = nextLevel;
            game.isLevelTransitioning = true;
            game.transitionTimer = game.transitionDuration;
            SaveGame(game);

            // respawn logic to prevent glitches on level change
            game.key = 'R';
            int cx = gridCountX / 2;
            int cy = gridCountY / 2;
            for (int i = 0; i < game.snakeLength; i++)
            {
                game.snakePosition[i][0] = cx - i;
                game.snakePosition[i][1] = cy;
            }

            // check hurdles before spawning food
            bool nextHurdles = (game.storyLevel >= 3);
            do
            {
                game.foodX = GetRandomValue(0, gridCountX - 1);
                game.foodY = GetRandomValue(0, gridCountY - 1);
            } while (IsTileBlocked(game.foodX, game.foodY, game, nextHurdles));
        }
    }

    // save highscore if beat
    if (game.score > game.highscore)
    {
        game.highscore = game.score;
        std::ofstream hsOut("highscore.txt");
        if (hsOut.is_open())
        {
            hsOut << game.highscore;
            hsOut.close();
        }
    }

    // handle transition timer
    if (game.isLevelTransitioning)
    {
        game.transitionTimer -= GetFrameTime();
        if (game.transitionTimer <= 0)
        {
            game.isLevelTransitioning = false;
            game.transitionTimer = 0;
            game.allowMove = true;
        }
        return;
    }

    // input check
    if (game.allowMove)
    {
        // prevent 180 degree turns
        if (IsKeyPressed(KEY_RIGHT) && game.key != 'L')
        {
            game.key = 'R';
            game.allowMove = false;
        }
        else if (IsKeyPressed(KEY_LEFT) && game.key != 'R')
        {
            game.key = 'L';
            game.allowMove = false;
        }
        else if (IsKeyPressed(KEY_UP) && game.key != 'D')
        {
            game.key = 'U';
            game.allowMove = false;
        }
        else if (IsKeyPressed(KEY_DOWN) && game.key != 'U')
        {
            game.key = 'D';
            game.allowMove = false;
        }
    }

    // move timer loop
    game.moveTimer += GetFrameTime();
    if (game.moveTimer >= game.moveInterval)
    {
        game.moveTimer = 0.0f;
        game.allowMove = true;

        int nextX = game.snakePosition[0][0];
        int nextY = game.snakePosition[0][1];

        switch (game.key)
        {
        case 'R':
            nextX++;
            break;
        case 'L':
            nextX--;
            break;
        case 'U':
            nextY--;
            break;
        case 'D':
            nextY++;
            break;
        }

        // wall collision
        if (nextX < 0 || nextX >= gridCountX || nextY < 0 || nextY >= gridCountY)
        {
            if (wallsActive)
            {
                game.gameOver = true;
                return;
            }
            else
            {
                // wrap around logic
                if (nextX < 0)
                    nextX = gridCountX - 1;
                if (nextX >= gridCountX)
                    nextX = 0;
                if (nextY < 0)
                    nextY = gridCountY - 1;
                if (nextY >= gridCountY)
                    nextY = 0;
            }
        }

        // hurdle collision
        if (hurdlesActive)
        {
            for (int i = 0; i < game.hurdleCount; i++)
            {
                if (nextX == game.hurdles[i][0] && nextY == game.hurdles[i][1])
                {
                    game.gameOver = true;
                    return;
                }
            }
        }

        // move body segments
        for (int i = game.snakeLength; i > 0; i--)
        {
            game.snakePosition[i][0] = game.snakePosition[i - 1][0];
            game.snakePosition[i][1] = game.snakePosition[i - 1][1];
        }
        game.snakePosition[0][0] = nextX;
        game.snakePosition[0][1] = nextY;

        // food collision
        if (nextX == game.foodX && nextY == game.foodY)
        {
            game.snakeLength++;
            game.score += 10;
            if (game.moveInterval > 0.05f)
                game.moveInterval -= 0.001f; // slight speed up
            do
            {
                game.foodX = GetRandomValue(0, gridCountX - 1);
                game.foodY = GetRandomValue(0, gridCountY - 1);
            } while (IsTileBlocked(game.foodX, game.foodY, game, hurdlesActive));
            SaveGame(game);
        }

        // self collision
        if (!game.gameOver)
        {
            for (int i = 1; i < game.snakeLength; i++)
            {
                if (game.snakePosition[0][0] == game.snakePosition[i][0] && game.snakePosition[0][1] == game.snakePosition[i][1])
                {
                    game.gameOver = true;
                }
            }
        }
    }
}

// DRAWING

void DrawMenu(GameState &game)
{
    // colors based on theme
    Color bg, box, sel, text, textSel;
    if (game.theme == "Classic")
    {
        bg = {185, 205, 160, 255};
        box = {160, 190, 140, 255};
        sel = {120, 150, 110, 255};
        text = {60, 80, 50, 255};
        textSel = WHITE;
    }
    else
    {
        bg = {240, 225, 185, 255};
        box = {225, 210, 178, 255};
        sel = {25, 40, 75, 255};
        text = {60, 45, 25, 255};
        textSel = WHITE;
    }

    ClearBackground(bg);
    DrawText("SNAKE ULTIMATE", screenWidth / 2 - 200, 200, 50, text);
    std::string hsText = "High Score: " + std::to_string(game.highscore);
    DrawText(hsText.c_str(), screenWidth / 2 - 100, 260, 30, text);

    int startY = 250, gap = 70, boxW = 300, boxH = 50;

    const char *titles[] = {"Continue", "New Game", "Theme", "Mode", "Exit"};
    for (int i = 1; i <= 5; i++)
    {
        int yPos = startY + (gap * (i - 1)) + 100;
        std::string display = titles[i - 1];

        // custom labels
        if (i == 3)
            display = "Theme: " + game.theme;
        if (i == 4)
        {
            display = "Mode: ";
            switch (game.currentMode)
            {
            case EASY:
                display += "< Easy >";
                break;
            case NORMAL:
                display += "< Normal >";
                break;
            case HARD:
                display += "< Hard >";
                break;
            case STORY:
                display += "< Story >";
                break;
            }
        }

        if (game.menuOption == i)
        {
            if (i == 1 && !game.hasSaveFile)
            {
                DrawRectangle(screenWidth / 2 - 150, yPos, boxW, boxH, RED);
                DrawText(display.c_str(), screenWidth / 2 - 40, yPos + 15, 20, WHITE);
            }
            else
            {
                DrawRectangle(screenWidth / 2 - 150, yPos, boxW, boxH, sel);
                // aligning text
                int offset = (i == 3) ? 60 : (i == 4 ? 70 : 40);
                if (i == 2)
                    offset = 45;
                if (i == 5)
                    offset = 20;
                DrawText(display.c_str(), screenWidth / 2 - offset, yPos + 15, 20, textSel);
            }
        }
        else
        {
            DrawRectangleLines(screenWidth / 2 - 150, yPos, boxW, boxH, box);
            int offset = (i == 3) ? 60 : (i == 4 ? 70 : 40);
            if (i == 2)
                offset = 45;
            if (i == 5)
                offset = 20;
            DrawText(display.c_str(), screenWidth / 2 - offset, yPos + 15, 20, text);
        }
    }
}

void DrawGameplay(GameState &game)
{
    // local colors
    Color cBg, cGrid, cSnake, cFood, cMenuBg;
    if (game.theme == "Classic")
    {
        cBg = {150, 180, 110, 255};
        cGrid = {100, 130, 90, 255};
        cSnake = {50, 70, 40, 255};
        cFood = {200, 0, 0, 255};
        cMenuBg = {185, 205, 160, 255};
    }
    else
    {
        cBg = {255, 246, 199, 255};
        cGrid = {232, 223, 194, 255};
        cSnake = {15, 26, 51, 255};
        cFood = {255, 79, 163, 255};
        cMenuBg = {240, 225, 185, 255};
    }

    ClearBackground(cMenuBg);
    DrawRectangle(boardOffsetX, boardOffsetY, boardWidth, boardHeight, cBg);

    // grid
    for (int i = 0; i <= gridCountX; i++)
        DrawLine(boardOffsetX + i * cellSize, boardOffsetY, boardOffsetX + i * cellSize, boardOffsetY + boardHeight, cGrid);
    for (int i = 0; i <= gridCountY; i++)
        DrawLine(boardOffsetX, boardOffsetY + i * cellSize, boardOffsetX + boardWidth, boardOffsetY + i * cellSize, cGrid);

    bool wallsActive = (game.currentMode == NORMAL || game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 2));
    bool hurdlesActive = (game.currentMode == HARD || (game.currentMode == STORY && game.storyLevel >= 3));

    // draw hurdles
    if (hurdlesActive)
    {
        for (int i = 0; i < game.hurdleCount; i++)
            DrawRectangle(boardOffsetX + game.hurdles[i][0] * cellSize, boardOffsetY + game.hurdles[i][1] * cellSize, cellSize, cellSize, DARKGRAY);
    }

    // draw food
    float fruitPixelX = boardOffsetX + game.foodX * cellSize + cellSize / 2.0f;
    float fruitPixelY = boardOffsetY + game.foodY * cellSize + cellSize / 2.0f;
    float fruitRadius = cellSize / 2.0f - 4;

    // apple parts
    DrawLineEx((Vector2){fruitPixelX, fruitPixelY - fruitRadius}, (Vector2){fruitPixelX, fruitPixelY - fruitRadius - 10}, 3, BROWN);
    DrawEllipse(fruitPixelX + 6, fruitPixelY - fruitRadius - 5, 6, 3, GREEN);
    DrawCircleV((Vector2){fruitPixelX, fruitPixelY}, fruitRadius, cFood);

    // draw snake
    for (int i = game.snakeLength - 1; i >= 0; i--)
    {
        float snakePixelX = boardOffsetX + game.snakePosition[i][0] * cellSize + cellSize / 2.0f;
        float snakePixelY = boardOffsetY + game.snakePosition[i][1] * cellSize + cellSize / 2.0f;
        float segmentRadius = cellSize / 2.0f;

        if (i == 0) // head
        {
            DrawCircleV((Vector2){snakePixelX, snakePixelY}, segmentRadius, cSnake);

            // eye calc
            float eyeOffsetX = 0;
            float eyeOffsetY = 0;
            float eyeSep = 8;

            switch (game.key)
            {
            case 'R':
                eyeOffsetX = eyeSep;
                break;
            case 'L':
                eyeOffsetX = -eyeSep;
                break;
            case 'U':
                eyeOffsetY = -eyeSep;
                break;
            case 'D':
                eyeOffsetY = eyeSep;
                break;
            }

            Vector2 leftEye, rightEye;
            if (game.key == 'U' || game.key == 'D')
            {
                leftEye = {snakePixelX - 6, snakePixelY + eyeOffsetY};
                rightEye = {snakePixelX + 6, snakePixelY + eyeOffsetY};
            }
            else
            {
                leftEye = {snakePixelX + eyeOffsetX, snakePixelY - 6};
                rightEye = {snakePixelX + eyeOffsetX, snakePixelY + 6};
            }

            // draw eyes
            DrawCircleV(leftEye, 5, WHITE);
            DrawCircleV(rightEye, 5, WHITE);
            DrawCircleV(leftEye, 2, BLACK);
            DrawCircleV(rightEye, 2, BLACK);
        }
        else // body
        {
            DrawCircleV((Vector2){snakePixelX, snakePixelY}, segmentRadius - 1, cSnake);
        }
    }

    // walls
    if (wallsActive)
    {
        DrawRectangleLinesEx((Rectangle){(float)boardOffsetX, (float)boardOffsetY, (float)boardWidth, (float)boardHeight}, 4, RED);
    }

    // UI text
    DrawText(TextFormat("Score: %i", game.score), 20, 20, 30, WHITE);

    std::string mText;
    Color mColor;

    switch (game.currentMode)
    {
    case EASY:
        mText = "EASY";
        mColor = GREEN;
        break;
    case NORMAL:
        mText = "NORMAL";
        mColor = ORANGE;
        break;
    case HARD:
        mText = "HARD";
        mColor = RED;
        break;
    case STORY:
        mText = "STORY - LVL " + std::to_string(game.storyLevel);
        mColor = SKYBLUE;
        break;
    }

    DrawText(mText.c_str(), screenWidth / 2 - MeasureText(mText.c_str(), 30) / 2, 20, 30, mColor);

    // level transition
    if (game.isLevelTransitioning)
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 100});
        std::string levelMsg = "LEVEL " + std::to_string(game.storyLevel);
        DrawText(levelMsg.c_str(), screenWidth / 2 - MeasureText(levelMsg.c_str(), 60) / 2, screenHeight / 2 - 100, 60, GOLD);
        std::string countStr = std::to_string((int)ceil(game.transitionTimer));
        DrawText(countStr.c_str(), screenWidth / 2 - MeasureText(countStr.c_str(), 80) / 2, screenHeight / 2, 80, WHITE);
        DrawText("Get Ready!", screenWidth / 2 - MeasureText("Get Ready!", 30) / 2, screenHeight / 2 + 80, 30, LIGHTGRAY);
    }

    // game over screen
    if (game.gameOver)
    {
        DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 60) / 2, screenHeight / 2 - 60, 60, RED);
        DrawText("Press ESC for Menu", screenWidth / 2 - MeasureText("Press ESC for Menu", 20) / 2, screenHeight / 2 + 10, 20, LIGHTGRAY);
        DrawText("Press 'R' to RESTART", screenWidth / 2 - MeasureText("Press 'R' to RESTART", 25) / 2, screenHeight / 2 + 40, 25, GOLD);
    }
}