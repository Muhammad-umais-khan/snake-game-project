#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cmath> // Added: Required for the ceil() function

// --- Enum from Main Code ---
enum GameMode
{
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
    STORY = 3
};

// --- Helper Function from Main Code ---
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

int main()
{
    // --- STEP 1: Full Screen Initialization ---
    InitWindow(0, 0, "Snake Game - Ultimate Version");
    SetTargetFPS(60);

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int cellSize = 40;

    // --- STEP 2: Calculate Board Size ---
    int rawBoardWidth = screenWidth - 120;
    int rawBoardHeight = screenHeight - 120;
    const int boardWidth = rawBoardWidth - (rawBoardWidth % cellSize);
    const int boardHeight = rawBoardHeight - (rawBoardHeight % cellSize);

    const int gridCountX = boardWidth / cellSize;
    const int gridCountY = boardHeight / cellSize;

    const int boardOffsetX = (screenWidth - boardWidth) / 2;
    const int boardOffsetY = (screenHeight - boardHeight) / 2;

    // --- STEP 3: Game Variables ---
    int stateofgame = 0;
    int menuOption = 1;

    GameMode currentMode = NORMAL;
    std::string theme = "Classic";
    int storyLevel = 1;

    bool gameOver = false;
    int score = 0;
    int snakeLength = 4;
    int snakePosition[1024][2] = {0};

    // New Variables for Story Transition
    bool isLevelTransitioning = false;
    float transitionTimer = 0.0f;
    const float transitionDuration = 3.0f;

    int snakeX = gridCountX / 2;
    int snakeY = gridCountY / 2;
    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }

    char key = 'R';
    int foodX = 0;
    int foodY = 0;

    float moveTimer = 0.0f;
    float moveInterval = 0.1f;

    bool allowMove = false;

    const int maxHurdles = 100;
    int hurdles[maxHurdles][2];
    int hurdleCount = 0;

    // --- STEP 4: Initialize Hurdles ---
    {
        int lastX = gridCountX - 1;
        int lastY = gridCountY - 1;
        int idx = 0;

        // Corners
        hurdles[idx][0] = 0;
        hurdles[idx++][1] = 0;
        hurdles[idx][0] = 1;
        hurdles[idx++][1] = 0;
        hurdles[idx][0] = 2;
        hurdles[idx++][1] = 0;
        hurdles[idx][0] = 0;
        hurdles[idx++][1] = 1;
        hurdles[idx][0] = 0;
        hurdles[idx++][1] = 2;

        hurdles[idx][0] = lastX;
        hurdles[idx++][1] = lastY;
        hurdles[idx][0] = lastX - 1;
        hurdles[idx++][1] = lastY;
        hurdles[idx][0] = lastX - 2;
        hurdles[idx++][1] = lastY;
        hurdles[idx][0] = lastX;
        hurdles[idx++][1] = lastY - 1;
        hurdles[idx][0] = lastX;
        hurdles[idx++][1] = lastY - 2;

        hurdles[idx][0] = lastX;
        hurdles[idx++][1] = 0;
        hurdles[idx][0] = lastX - 1;
        hurdles[idx++][1] = 0;
        hurdles[idx][0] = lastX - 2;
        hurdles[idx++][1] = 0;
        hurdles[idx][0] = lastX;
        hurdles[idx++][1] = 1;
        hurdles[idx][0] = lastX;
        hurdles[idx++][1] = 2;

        hurdles[idx][0] = 0;
        hurdles[idx++][1] = lastY;
        hurdles[idx][0] = 1;
        hurdles[idx++][1] = lastY;
        hurdles[idx][0] = 2;
        hurdles[idx++][1] = lastY;
        hurdles[idx][0] = 0;
        hurdles[idx++][1] = lastY - 1;
        hurdles[idx][0] = 0;
        hurdles[idx++][1] = lastY - 2;

        // Middle Barrier
        int gap = 4;
        int startY = (gridCountY - gap) / 2 - 1;
        for (int i = 0; i < 7; i++)
        {
            hurdles[idx][0] = i + (gridCountX - 7) / 2;
            hurdles[idx++][1] = startY;
            hurdles[idx][0] = i + (gridCountX - 7) / 2;
            hurdles[idx++][1] = startY + gap + 1;
        }
        hurdleCount = idx;
    }

    // --- Game Colors ---
    Color bgClassic = {150, 180, 110, 255};
    Color gridClassic = {100, 130, 90, 255};
    Color snakeClassic = {50, 70, 40, 255};
    Color foodClassic = {220, 0, 0, 255};

    Color bgDesert = {255, 246, 199, 255};
    Color gridDesert = {232, 223, 194, 255};
    Color snakeDesert = {15, 26, 51, 255};
    Color foodDesert = {255, 79, 163, 255};

    // --- Menu Colors Definition ---
    Color classicMenuBg = {185, 205, 160, 255};
    Color classicMenuBox = {160, 190, 140, 255};
    Color classicMenuSelected = {120, 150, 110, 255};
    Color classicMenuText = {60, 80, 50, 255};
    Color classicMenuTextSel = {255, 255, 255, 255};

    Color desertMenuBg = {240, 225, 185, 255};
    Color desertMenuBox = {225, 210, 178, 255};
    Color desertMenuSelected = {25, 40, 75, 255};
    Color desertMenuText = {60, 45, 25, 255};
    Color desertMenuTextSel = {255, 255, 255, 255};

    // Current Menu State
    Color currentMenuBg = classicMenuBg;
    Color currentMenuBox = classicMenuBox;
    Color currentMenuSelected = classicMenuSelected;
    Color currentMenuText = classicMenuText;
    Color currentMenuTextSel = classicMenuTextSel;

    int highscore = 0;
    std::ifstream hsFileIn("highscore.txt");
    if (hsFileIn.is_open())
    {
        hsFileIn >> highscore;
        hsFileIn.close();
    }

    bool hasSaveFile = false;
    {
        std::ifstream savecheck("savefile.txt");
        if (savecheck.is_open())
        {
            hasSaveFile = true;
            savecheck.close();
        }
    }

    // --- Main Game Loop ---
    while (!WindowShouldClose())
    {
        if (stateofgame == 0)
        {
            BeginDrawing();
            ClearBackground(currentMenuBg);

            if (IsKeyPressed(KEY_DOWN))
                menuOption++;
            if (IsKeyPressed(KEY_UP))
                menuOption--;
            if (menuOption > 5)
                menuOption = 1;
            if (menuOption < 1)
                menuOption = 5;

            if (menuOption == 4)
            {
                if (IsKeyPressed(KEY_RIGHT))
                {
                    int next = (int)currentMode + 1;
                    if (next > 3)
                        next = 0;
                    currentMode = (GameMode)next;
                }
                if (IsKeyPressed(KEY_LEFT))
                {
                    int prev = (int)currentMode - 1;
                    if (prev < 0)
                        prev = 3;
                    currentMode = (GameMode)prev;
                }
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                if (menuOption == 5)
                    break;

                if (menuOption == 3)
                {
                    // Toggle Theme & Update Colors
                    if (theme == "Classic")
                    {
                        theme = "Desert";
                        currentMenuBg = desertMenuBg;
                        currentMenuBox = desertMenuBox;
                        currentMenuSelected = desertMenuSelected;
                        currentMenuText = desertMenuText;
                        currentMenuTextSel = desertMenuTextSel;
                    }
                    else
                    {
                        theme = "Classic";
                        currentMenuBg = classicMenuBg;
                        currentMenuBox = classicMenuBox;
                        currentMenuSelected = classicMenuSelected;
                        currentMenuText = classicMenuText;
                        currentMenuTextSel = classicMenuTextSel;
                    }
                }

                if (menuOption == 2)
                {
                    stateofgame = 2;
                    gameOver = false;
                    score = 0;
                    storyLevel = 1;
                    snakeLength = 4;
                    key = 'R';
                    moveTimer = 0.0f;
                    moveInterval = 0.1f;
                    allowMove = true;
                    isLevelTransitioning = false;
                    transitionTimer = 0.0f;

                    for (int i = 0; i < snakeLength; i++)
                    {
                        snakePosition[i][0] = gridCountX / 2;
                        snakePosition[i][1] = gridCountY / 2;
                    }

                    bool hurdlesActive = (currentMode == HARD);
                    do
                    {
                        foodX = GetRandomValue(0, gridCountX - 1);
                        foodY = GetRandomValue(0, gridCountY - 1);
                    } while (IsTileBlocked(foodX, foodY, snakePosition, snakeLength, hurdles, hurdleCount, hurdlesActive));
                }

                if (menuOption == 1)
                {
                    if (hasSaveFile)
                    {
                        std::ifstream load("savefile.txt");
                        if (load.is_open())
                        {
                            int modeInt;
                            load >> snakeLength >> score >> key >> foodX >> foodY >> modeInt;
                            currentMode = (GameMode)modeInt;
                            for (int i = 0; i < snakeLength; i++)
                            {
                                load >> snakePosition[i][0] >> snakePosition[i][1];
                            }
                            load.close();
                            allowMove = true;
                            stateofgame = 2;
                            isLevelTransitioning = false;
                        }
                    }
                }
            }

            // Draw Menu Title & Highscore
            DrawText("SNAKE ULTIMATE", screenWidth / 2 - 200, 100, 50, currentMenuText);
            std::string hsText = "High Score: " + std::to_string(highscore);
            DrawText(hsText.c_str(), screenWidth / 2 - 100, 160, 30, currentMenuText);

            int startY = 250;
            int gap = 70;
            int boxW = 300;
            int boxH = 50;

            // --- Option 1: Continue ---
            if (menuOption == 1)
            {
                if (!hasSaveFile)
                {
                    // Invalid/Unavailable State
                    DrawRectangle(screenWidth / 2 - 150, startY, boxW, boxH, RED);
                    DrawText("Continue", screenWidth / 2 - 40, startY + 15, 20, WHITE);
                }
                else
                {
                    // Selected State
                    DrawRectangle(screenWidth / 2 - 150, startY, boxW, boxH, currentMenuSelected);
                    DrawText("Continue", screenWidth / 2 - 40, startY + 15, 20, currentMenuTextSel);
                }
            }
            else
            {
                // Unselected State
                DrawRectangleLines(screenWidth / 2 - 150, startY, boxW, boxH, currentMenuBox);
                DrawText("Continue", screenWidth / 2 - 40, startY + 15, 20, currentMenuText);
            }

            // --- Option 2: New Game ---
            if (menuOption == 2)
            {
                DrawRectangle(screenWidth / 2 - 150, startY + gap, boxW, boxH, currentMenuSelected);
                DrawText("New Game", screenWidth / 2 - 45, startY + gap + 15, 20, currentMenuTextSel);
            }
            else
            {
                DrawRectangleLines(screenWidth / 2 - 150, startY + gap, boxW, boxH, currentMenuBox);
                DrawText("New Game", screenWidth / 2 - 45, startY + gap + 15, 20, currentMenuText);
            }

            // --- Option 3: Theme ---
            std::string themeText = "Theme: " + theme;
            if (menuOption == 3)
            {
                DrawRectangle(screenWidth / 2 - 150, startY + gap * 2, boxW, boxH, currentMenuSelected);
                DrawText(themeText.c_str(), screenWidth / 2 - 60, startY + gap * 2 + 15, 20, currentMenuTextSel);
            }
            else
            {
                DrawRectangleLines(screenWidth / 2 - 150, startY + gap * 2, boxW, boxH, currentMenuBox);
                DrawText(themeText.c_str(), screenWidth / 2 - 60, startY + gap * 2 + 15, 20, currentMenuText);
            }

            // --- Option 4: Mode ---
            std::string modeStr = "Mode: ";
            if (currentMode == EASY)
                modeStr += "< Easy >";
            else if (currentMode == NORMAL)
                modeStr += "< Normal >";
            else if (currentMode == HARD)
                modeStr += "< Hard >";
            else
                modeStr += "< Story >";

            if (menuOption == 4)
            {
                DrawRectangle(screenWidth / 2 - 150, startY + gap * 3, boxW, boxH, currentMenuSelected);
                DrawText(modeStr.c_str(), screenWidth / 2 - 70, startY + gap * 3 + 15, 20, currentMenuTextSel);
            }
            else
            {
                DrawRectangleLines(screenWidth / 2 - 150, startY + gap * 3, boxW, boxH, currentMenuBox);
                DrawText(modeStr.c_str(), screenWidth / 2 - 70, startY + gap * 3 + 15, 20, currentMenuText);
            }

            // --- Option 5: Exit ---
            if (menuOption == 5)
            {
                DrawRectangle(screenWidth / 2 - 150, startY + gap * 4, boxW, boxH, currentMenuSelected);
                DrawText("Exit", screenWidth / 2 - 20, startY + gap * 4 + 15, 20, currentMenuTextSel);
            }
            else
            {
                DrawRectangleLines(screenWidth / 2 - 150, startY + gap * 4, boxW, boxH, currentMenuBox);
                DrawText("Exit", screenWidth / 2 - 20, startY + gap * 4 + 15, 20, currentMenuText);
            }

            EndDrawing();
        }
        else if (stateofgame == 2)
        {
            if (!gameOver)
            {
                // --- Story Mode Logic ---
                if (currentMode == STORY)
                {
                    int nextLevel = 1;
                    if (score >= 50 && score < 100)
                        nextLevel = 2;
                    else if (score >= 100)
                        nextLevel = 3;

                    // Detect Level Change
                    if (nextLevel > storyLevel)
                    {
                        storyLevel = nextLevel;
                        isLevelTransitioning = true;
                        transitionTimer = transitionDuration;

                        // --- SAFE SPAWN PATTERN ---
                        // 1. Reset Direction to RIGHT so you don't run into your own body immediately
                        key = 'R';

                        // 2. Center Head
                        int cx = gridCountX / 2;
                        int cy = gridCountY / 2;

                        // 3. Arrange body in a straight horizontal line to the left of center
                        // This ensures you are not "inside" a hurdle or a wall
                        for (int i = 0; i < snakeLength; i++)
                        {
                            snakePosition[i][0] = cx - i; // Body trails to the left
                            snakePosition[i][1] = cy;
                        }

                        // 4. Respawn Food in a valid place for the NEW level
                        bool wallsActive = (storyLevel >= 2);
                        bool hurdlesActive = (storyLevel >= 3);
                        do
                        {
                            foodX = GetRandomValue(0, gridCountX - 1);
                            foodY = GetRandomValue(0, gridCountY - 1);
                        } while (IsTileBlocked(foodX, foodY, snakePosition, snakeLength, hurdles, hurdleCount, hurdlesActive));
                    }
                }

                bool wallsActive = (currentMode == NORMAL || currentMode == HARD || (currentMode == STORY && storyLevel >= 2));
                bool hurdlesActive = (currentMode == HARD || (currentMode == STORY && storyLevel >= 3));

                if (score > highscore)
                {
                    highscore = score;
                    std::ofstream hsOut("highscore.txt");
                    if (hsOut.is_open())
                    {
                        hsOut << highscore;
                        hsOut.close();
                    }
                }

                // --- TRANSITION TIMER LOGIC ---
                if (isLevelTransitioning)
                {
                    transitionTimer -= GetFrameTime();
                    if (transitionTimer <= 0)
                    {
                        isLevelTransitioning = false;
                        transitionTimer = 0;
                        allowMove = true; // Unlock controls
                    }
                    // Skip the rest of the movement logic while transitioning
                }
                else
                {
                    // --- NORMAL GAMEPLAY LOOP ---
                    if (allowMove)
                    {
                        if (IsKeyPressed(KEY_RIGHT) && key != 'L')
                        {
                            key = 'R';
                            allowMove = false;
                        }
                        else if (IsKeyPressed(KEY_LEFT) && key != 'R')
                        {
                            key = 'L';
                            allowMove = false;
                        }
                        else if (IsKeyPressed(KEY_UP) && key != 'D')
                        {
                            key = 'U';
                            allowMove = false;
                        }
                        else if (IsKeyPressed(KEY_DOWN) && key != 'U')
                        {
                            key = 'D';
                            allowMove = false;
                        }
                    }

                    moveTimer += GetFrameTime();
                    if (moveTimer >= moveInterval)
                    {
                        moveTimer = 0.0f;
                        allowMove = true;

                        int nextX = snakePosition[0][0];
                        int nextY = snakePosition[0][1];

                        if (key == 'R')
                            nextX++;
                        if (key == 'L')
                            nextX--;
                        if (key == 'U')
                            nextY--;
                        if (key == 'D')
                            nextY++;

                        bool wallHit = false;

                        if (nextX < 0 || nextX >= gridCountX || nextY < 0 || nextY >= gridCountY)
                        {
                            if (wallsActive)
                            {
                                gameOver = true;
                                wallHit = true;
                            }
                            else
                            {
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

                        if (!wallHit && hurdlesActive)
                        {
                            for (int i = 0; i < hurdleCount; i++)
                            {
                                if (nextX == hurdles[i][0] && nextY == hurdles[i][1])
                                {
                                    gameOver = true;
                                }
                            }
                        }

                        if (!gameOver)
                        {
                            for (int i = snakeLength; i > 0; i--)
                            {
                                snakePosition[i][0] = snakePosition[i - 1][0];
                                snakePosition[i][1] = snakePosition[i - 1][1];
                            }
                            snakePosition[0][0] = nextX;
                            snakePosition[0][1] = nextY;
                        }

                        if (!gameOver && nextX == foodX && nextY == foodY)
                        {
                            snakeLength++;
                            score += 10;
                            if (moveInterval > 0.05f)
                                moveInterval -= 0.001f;

                            do
                            {
                                foodX = GetRandomValue(0, gridCountX - 1);
                                foodY = GetRandomValue(0, gridCountY - 1);
                            } while (IsTileBlocked(foodX, foodY, snakePosition, snakeLength, hurdles, hurdleCount, hurdlesActive));
                        }

                        if (!gameOver)
                        {
                            for (int i = 1; i < snakeLength; i++)
                            {
                                if (snakePosition[0][0] == snakePosition[i][0] && snakePosition[0][1] == snakePosition[i][1])
                                {
                                    gameOver = true;
                                }
                            }
                        }

                        if (!gameOver)
                        {
                            std::ofstream save("savefile.txt");
                            if (save.is_open())
                            {
                                save << snakeLength << "\n"
                                     << score << "\n"
                                     << key << "\n"
                                     << foodX << "\n"
                                     << foodY << "\n"
                                     << (int)currentMode;
                                for (int i = 0; i < snakeLength; i++)
                                {
                                    save << "\n"
                                         << snakePosition[i][0] << " " << snakePosition[i][1];
                                }
                                save.close();
                                hasSaveFile = true;
                            }
                        }
                    }
                } // End else (not transitioning)
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                stateofgame = 0;
            }

            BeginDrawing();

            Color cBg = (theme == "Classic") ? bgClassic : bgDesert;
            Color cGrid = (theme == "Classic") ? gridClassic : gridDesert;
            Color cSnake = (theme == "Classic") ? snakeClassic : snakeDesert;
            Color cFood = (theme == "Classic") ? foodClassic : foodDesert;
            Color cMenuBg = (theme == "Classic") ? classicMenuBg : desertMenuBg;

            ClearBackground(cMenuBg);
            DrawRectangle(boardOffsetX, boardOffsetY, boardWidth, boardHeight, cBg);

            for (int i = 0; i <= gridCountX; i++)
                DrawLine(boardOffsetX + i * cellSize, boardOffsetY, boardOffsetX + i * cellSize, boardOffsetY + boardHeight, cGrid);
            for (int i = 0; i <= gridCountY; i++)
                DrawLine(boardOffsetX, boardOffsetY + i * cellSize, boardOffsetX + boardWidth, boardOffsetY + i * cellSize, cGrid);

            bool wallsActive = (currentMode == NORMAL || currentMode == HARD || (currentMode == STORY && storyLevel >= 2));
            bool hurdlesActive = (currentMode == HARD || (currentMode == STORY && storyLevel >= 3));

            if (hurdlesActive)
            {
                for (int i = 0; i < hurdleCount; i++)
                    DrawRectangle(boardOffsetX + hurdles[i][0] * cellSize, boardOffsetY + hurdles[i][1] * cellSize, cellSize, cellSize, DARKGRAY);
            }

            DrawRectangle(boardOffsetX + foodX * cellSize, boardOffsetY + foodY * cellSize, cellSize, cellSize, cFood);

            for (int i = 0; i < snakeLength; i++)
                DrawRectangle(boardOffsetX + snakePosition[i][0] * cellSize, boardOffsetY + snakePosition[i][1] * cellSize, cellSize, cellSize, cSnake);

            if (wallsActive)
            {
                DrawRectangleLinesEx((Rectangle){(float)boardOffsetX, (float)boardOffsetY, (float)boardWidth, (float)boardHeight}, 4, RED);
            }

            DrawText(TextFormat("Score: %i", score), 20, 20, 30, WHITE);

            std::string mText;
            Color mColor;
            if (currentMode == EASY)
            {
                mText = "EASY";
                mColor = GREEN;
            }
            else if (currentMode == NORMAL)
            {
                mText = "NORMAL";
                mColor = ORANGE;
            }
            else if (currentMode == HARD)
            {
                mText = "HARD";
                mColor = RED;
            }
            else
            {
                mText = "STORY - LVL " + std::to_string(storyLevel);
                mColor = SKYBLUE;
            }

            DrawText(mText.c_str(), screenWidth / 2 - MeasureText(mText.c_str(), 30) / 2, 20, 30, mColor);

            // --- DRAW TRANSITION TIMER ---
            if (isLevelTransitioning)
            {
                // Draw a semi-transparent overlay
                DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 100});

                std::string levelMsg = "LEVEL " + std::to_string(storyLevel);
                DrawText(levelMsg.c_str(), screenWidth / 2 - MeasureText(levelMsg.c_str(), 60) / 2, screenHeight / 2 - 100, 60, GOLD);

                std::string countStr = std::to_string((int)ceil(transitionTimer));
                DrawText(countStr.c_str(), screenWidth / 2 - MeasureText(countStr.c_str(), 80) / 2, screenHeight / 2, 80, WHITE);

                DrawText("Get Ready!", screenWidth / 2 - MeasureText("Get Ready!", 30) / 2, screenHeight / 2 + 80, 30, LIGHTGRAY);
            }

            if (gameOver)
            {
                DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 60) / 2, screenHeight / 2 - 60, 60, RED);
                DrawText("Press ESC for Menu", screenWidth / 2 - MeasureText("Press ESC for Menu", 20) / 2, screenHeight / 2 + 10, 20, LIGHTGRAY);
                DrawText("Press 'R' to RESTART", screenWidth / 2 - MeasureText("Press 'R' to RESTART", 25) / 2, screenHeight / 2 + 40, 25, GOLD);

                if (hasSaveFile)
                {
                    remove("savefile.txt");
                    hasSaveFile = false;
                }

                if (IsKeyPressed(KEY_R))
                {
                    gameOver = false;
                    score = 0;
                    storyLevel = 1;
                    snakeLength = 4;
                    key = 'R';
                    moveTimer = 0.0f;
                    moveInterval = 0.1f;
                    allowMove = true;
                    isLevelTransitioning = false; // Reset transition

                    for (int i = 0; i < snakeLength; i++)
                    {
                        snakePosition[i][0] = gridCountX / 2;
                        snakePosition[i][1] = gridCountY / 2;
                    }

                    bool restartHurdles = (currentMode == HARD);
                    do
                    {
                        foodX = GetRandomValue(0, gridCountX - 1);
                        foodY = GetRandomValue(0, gridCountY - 1);
                    } while (IsTileBlocked(foodX, foodY, snakePosition, snakeLength, hurdles, hurdleCount, restartHurdles));
                }
            }

            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}