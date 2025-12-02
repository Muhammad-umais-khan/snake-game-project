#include <raylib.h>
#include <stdlib.h>
#include <string>
#include <fstream> // Required for file handling

// Function from your main code to draw food with offsets
void DrawingFood(int foodX, int foodY, int cellSize, int boardOffsetX, int boardOffsetY, Color foodColor)
{
    DrawRectangle(
        boardOffsetX + (foodX * cellSize),
        boardOffsetY + (foodY * cellSize),
        cellSize, cellSize, foodColor);
}

int main()
{
    // --- INITIALIZATION ---
    InitWindow(0, 0, "Snake Game - Ultimate Version");

    // CHANGE 1: Set FPS to 60 for smooth input and drawing
    SetTargetFPS(60);

    // Get dynamic screen dimensions (From Main Code)
    const int windowWidth = GetScreenWidth();
    const int windowHeight = GetScreenHeight();
    const int cellSize = 40;

    // --- BOARD CALCULATION (From Main Code) ---
    int rawBoardWidth = windowWidth - 120;
    int rawBoardHeight = windowHeight - 120;

    // Ensure dimensions divisible by cellSize
    const int boardWidth = rawBoardWidth - (rawBoardWidth % cellSize);
    const int boardHeight = rawBoardHeight - (rawBoardHeight % cellSize);

    // Calculate Grid Counts
    const int gridCountX = boardWidth / cellSize;
    const int gridCountY = boardHeight / cellSize;

    // Calculate Offsets
    const int boardOffsetX = (windowWidth - boardWidth) / 2;
    const int boardOffsetY = (windowHeight - boardHeight) / 2;

    // --- GAME VARIABLES ---
    // State: 0=Menu, 1=Continue, 2=New Game, 3=Desert Mode, 4=Exit
    int stateofgame = 0;
    int option = 1;
    bool gameOver = false;
    int score = 0;
    int snakeLength = 4;

    int snakeX = gridCountX / 2;
    int snakeY = gridCountY / 2;

    int snakePosition[1024][2] = {0};

    // Initialize Snake Array
    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }

    char key = 'R';
    int foodX = 0;
    int foodY = 0;
    bool isOnSnake;

    // --- CHANGE 2: MOVEMENT TIMER VARIABLES ---
    // These control the speed of the snake independently of the FPS
    float moveTimer = 0.0f;
    float moveInterval = 0.1f; // 0.1 seconds = 10 movements per second (Original Speed)

    // --- COLORS (Standard) ---
    Color windowBgColor = {30, 30, 30, 255};
    Color boardBgColor = {150, 180, 110, 255};
    Color gridColor = {100, 130, 90, 255};
    Color foodColor = {220, 0, 0, 255};
    Color snakebodyColor = {50, 70, 40, 255};

    // --- COLORS (Desert Theme) ---
    Color bgSpecial = {255, 246, 199, 255};
    Color gridSpecial = {232, 223, 194, 255};
    Color snakeSpecial = {15, 26, 51, 255};
    Color foodSpecial = {255, 79, 163, 255};

    // --- FILE HANDLING CHECK (From Friend's Code) ---
    bool hasSaveFile = false;
    {
        std::ifstream savecheck("savefile.txt");
        if (savecheck.is_open())
        {
            hasSaveFile = true;
            savecheck.close();
        }
    }

    int highscore = 0;
    std::ifstream highscorefile("highscore.txt");
    if (highscorefile.is_open())
    {
        highscorefile >> highscore;
        highscorefile.close();
    }

    // --- MAIN LOOP ---
    while (true)
    {
        if (WindowShouldClose())
        {
            stateofgame = 0;
        }

        // ---------------- MENU LOGIC (State 0) ----------------
        if (stateofgame == 0)
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Menu Input
            if (IsKeyPressed(KEY_DOWN))
                option++;
            if (IsKeyPressed(KEY_UP))
                option--;
            if (option > 4)
                option = 1;
            if (option < 1)
                option = 4;

            if (IsKeyPressed(KEY_ENTER))
            {
                stateofgame = option;

                // LOGIC: NEW GAME (Standard or Desert)
                if (stateofgame == 2 || stateofgame == 3)
                {
                    // Reset variables
                    gameOver = false;
                    snakeLength = 4;
                    score = 190;
                    key = 'R';
                    moveTimer = 0.0f; // Reset timer

                    // Reset Snake Position
                    for (int i = 0; i < snakeLength; i++)
                    {
                        snakePosition[i][0] = gridCountX / 2;
                        snakePosition[i][1] = gridCountY / 2;
                    }

                    // Initial Food Spawn
                    do
                    {
                        isOnSnake = false;
                        foodX = GetRandomValue(0, gridCountX - 1);
                        foodY = GetRandomValue(0, gridCountY - 1);
                    } while (false);
                }

                // LOGIC: CONTINUE
                if (stateofgame == 1)
                {
                    if (hasSaveFile)
                    {
                        std::ifstream load("savefile.txt");
                        if (load.is_open())
                        {
                            load >> snakeLength;
                            load >> score;
                            load >> key;
                            load >> foodX;
                            load >> foodY;
                            for (int i = 0; i < snakeLength; i++)
                            {
                                load >> snakePosition[i][0];
                                load >> snakePosition[i][1];
                            }
                            load.close();
                        }
                    }
                    else
                    {
                        // If no save file, stay in menu
                        stateofgame = 0;
                    }
                }
            }

            // Draw Menu UI
            DrawText("SNAKE GAME", windowWidth / 2 - 150, windowHeight / 2 - 200, 50, DARKGRAY);

            std::string hsText = "High Score: " + std::to_string(highscore);
            DrawText(hsText.c_str(), windowWidth / 2 - 100, windowHeight / 2 - 140, 30, GOLD);

            // Option 1: Continue
            Color btnColor = (option == 1) ? DARKGRAY : LIGHTGRAY;
            if (!hasSaveFile && option == 1)
                btnColor = RED; // Visual cue if no save
            DrawRectangle(windowWidth / 2 - 160, windowHeight / 2 - 100, 320, 50, btnColor);
            DrawText("Continue", windowWidth / 2 - 60, windowHeight / 2 - 85, 25, WHITE);

            // Option 2: New Game
            DrawRectangle(windowWidth / 2 - 160, windowHeight / 2 - 20, 320, 50, (option == 2) ? DARKGRAY : LIGHTGRAY);
            DrawText("New Game", windowWidth / 2 - 70, windowHeight / 2 - 5, 25, WHITE);

            // Option 3: Desert Theme
            DrawRectangle(windowWidth / 2 - 160, windowHeight / 2 + 60, 320, 50, (option == 3) ? ORANGE : LIGHTGRAY);
            DrawText("Desert Theme", windowWidth / 2 - 90, windowHeight / 2 + 75, 25, WHITE);

            // Option 4: Exit
            DrawRectangle(windowWidth / 2 - 160, windowHeight / 2 + 140, 320, 50, (option == 4) ? DARKGRAY : LIGHTGRAY);
            DrawText("Exit", windowWidth / 2 - 45, windowHeight / 2 + 155, 25, WHITE);

            EndDrawing();
        }

        // ---------------- GAME LOGIC (States 1, 2, 3) ----------------
        else if (stateofgame == 1 || stateofgame == 2 || stateofgame == 3)
        {
            if (!gameOver)
            {
                // Update Highscore
                if (score > highscore)
                {
                    highscore = score;
                    std::ofstream hsFile("highscore.txt");
                    if (hsFile.is_open())
                    {
                        hsFile << highscore;
                        hsFile.close();
                    }
                }

                // --- CHANGE 3: INPUT HANDLING ---
                // We check Input EVERY FRAME (60 times a second) for responsiveness
                if (IsKeyDown(KEY_RIGHT) && key != 'L')
                    key = 'R';
                if (IsKeyDown(KEY_LEFT) && key != 'R')
                    key = 'L';
                if (IsKeyDown(KEY_UP) && key != 'D')
                    key = 'U';
                if (IsKeyDown(KEY_DOWN) && key != 'U')
                    key = 'D';
                if (IsKeyDown(KEY_R) && key != 'U')
                    key = 'r';

                if (key == 'r')
                {
                    stateofgame = 0;
                }

                // --- CHANGE 4: MOVEMENT LOGIC WRAPPED IN TIMER ---
                // We only move the snake when the timer hits the interval
                moveTimer += GetFrameTime(); // Add time passed since last frame

                if (moveTimer >= moveInterval)
                {
                    moveTimer = 0.0f; // Reset timer

                    // Calculate Next Position
                    int nextX = snakePosition[0][0];
                    int nextY = snakePosition[0][1];

                    if (key == 'R')
                        nextX++;
                    else if (key == 'L')
                        nextX--;
                    else if (key == 'U')
                        nextY--;
                    else if (key == 'D')
                        nextY++;

                    // Difficulty: Medium (Walls)
                    if (score >= 200)
                    {
                        if (nextX < 0 || nextX >= gridCountX || nextY < 0 || nextY >= gridCountY)
                        {
                            gameOver = true;
                        }
                    }

                    if (!gameOver)
                    {
                        // Move Body
                        for (int i = snakeLength; i > 0; i--)
                        {
                            snakePosition[i][0] = snakePosition[i - 1][0];
                            snakePosition[i][1] = snakePosition[i - 1][1];
                        }

                        // Move Head
                        snakePosition[0][0] = nextX;
                        snakePosition[0][1] = nextY;

                        // Difficulty: Easy (Wrap)
                        if (score < 200)
                        {
                            if (snakePosition[0][0] >= gridCountX)
                                snakePosition[0][0] = 0;
                            if (snakePosition[0][0] < 0)
                                snakePosition[0][0] = gridCountX - 1;
                            if (snakePosition[0][1] >= gridCountY)
                                snakePosition[0][1] = 0;
                            if (snakePosition[0][1] < 0)
                                snakePosition[0][1] = gridCountY - 1;
                        }
                    }

                    // Food Collision
                    if (!gameOver && snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
                    {
                        snakeLength++;
                        score += 10;
                        moveInterval -= 0.001f; // Increase speed by reducing the interval

                        do
                        {
                            isOnSnake = false;
                            foodX = GetRandomValue(0, gridCountX - 1);
                            foodY = GetRandomValue(0, gridCountY - 1);

                            for (int i = 0; i < snakeLength; i++)
                            {
                                if (snakePosition[i][0] == foodX && snakePosition[i][1] == foodY)
                                {
                                    isOnSnake = true;
                                    break;
                                }
                            }

                        } while (isOnSnake);
                    }

                    // Self Collision
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

                    // --- SAVE GAME LOGIC (Auto-save) ---
                    // Only save when the snake actually moves
                    if (!gameOver)
                    {
                        std::ofstream save("savefile.txt");
                        if (save.is_open())
                        {
                            save << snakeLength << "\n"
                                 << score << "\n"
                                 << key << "\n"
                                 << foodX << "\n"
                                 << foodY << "\n";
                            for (int i = 0; i < snakeLength; i++)
                            {
                                save << snakePosition[i][0] << " " << snakePosition[i][1] << "\n";
                            }
                            save.close();
                            hasSaveFile = true; // Update flag
                        }
                    }

                } // --- END OF TIMER BLOCK ---

            } // End of !gameOver logic

            // ---------------- DRAWING PHASE ----------------
            // This happens 60 times a second now (smooth!)
            BeginDrawing();
            ClearBackground(windowBgColor);

            // Determine Theme Colors
            Color currentBoard = (stateofgame == 3) ? bgSpecial : boardBgColor;
            Color currentGrid = (stateofgame == 3) ? gridSpecial : gridColor;
            Color currentSnake = (stateofgame == 3) ? snakeSpecial : snakebodyColor;
            Color currentFood = (stateofgame == 3) ? foodSpecial : foodColor;

            // Draw Board
            DrawRectangle(boardOffsetX, boardOffsetY, boardWidth, boardHeight, currentBoard);

            // Draw Grid
            // 1. Draw Vertical Lines
            for (int i = 0; i <= gridCountX; i++)
            {
                DrawLine(
                    boardOffsetX + (i * cellSize), // Start X
                    boardOffsetY,                  // Start Y (Top of board)
                    boardOffsetX + (i * cellSize), // End X (Same as Start X)
                    boardOffsetY + boardHeight,    // End Y (Bottom of board)
                    currentGrid);
            }

            // 2. Draw Horizontal Lines
            for (int i = 0; i <= gridCountY; i++)
            {
                DrawLine(
                    boardOffsetX,                  // Start X (Left of board)
                    boardOffsetY + (i * cellSize), // Start Y
                    boardOffsetX + boardWidth,     // End X (Right of board)
                    boardOffsetY + (i * cellSize), // End Y (Same as Start Y)
                    currentGrid);
            }

            // --- VISUAL INDICATOR FOR MEDIUM MODE (WALLS) ---
            if (score >= 200)
            {
                // Draw a thick red border around the board to indicate walls are active
                DrawRectangleLinesEx((Rectangle){(float)boardOffsetX, (float)boardOffsetY, (float)boardWidth, (float)boardHeight}, 5.0f, RED);
            }

            // Draw Food
            DrawingFood(foodX, foodY, cellSize, boardOffsetX, boardOffsetY, currentFood);

            // Draw Snake
            for (int i = 0; i < snakeLength; i++)
            {
                DrawRectangle(
                    boardOffsetX + (snakePosition[i][0] * cellSize),
                    boardOffsetY + (snakePosition[i][1] * cellSize),
                    cellSize, cellSize, currentSnake);
            }

            // UI Text
            std::string scoretext = "Score: " + std::to_string(score);
            DrawText(scoretext.c_str(), windowWidth - 250, 50, 40, WHITE);

            if (score >= 200)
                DrawText("MEDIUM", (windowWidth / 2) - 50, 50, 40, ORANGE);
            else
                DrawText("EASY", (windowWidth / 2) - 30, 50, 40, GREEN);

            DrawText("SNAKE", 50, 50, 40, WHITE);

            if (gameOver)
            {
                const char *text = "GAME OVER!";
                int textWidth = MeasureText(text, 60);
                DrawText(text, (windowWidth - textWidth) / 2, windowHeight / 2 - 50, 60, RED);

                const char *subtext = "Press ESC for Menu";
                int subWidth = MeasureText(subtext, 20);
                DrawText(subtext, (windowWidth - subWidth) / 2, windowHeight / 2 + 20, 20, LIGHTGRAY);

                // --- NEW CODE: RESTART INSTRUCTION ---
                const char *restartText = "Press 'R' to RESTART";
                int restartWidth = MeasureText(restartText, 20);
                DrawText(restartText, (windowWidth - restartWidth) / 2, windowHeight / 2 + 50, 20, GOLD);

                // --- DELETE SAVE FILE ON DEATH ---
                std::ifstream savefile("savefile.txt");
                if (savefile.is_open())
                {
                    savefile.close();
                    remove("savefile.txt");
                    hasSaveFile = false;
                }

                // --- NEW CODE: RESTART LOGIC ---
                if (IsKeyPressed(KEY_R))
                {
                    // 1. Reset Game Variables
                    gameOver = false;
                    score = 0;
                    snakeLength = 4;
                    key = 'R';        // Reset direction to Right
                    moveTimer = 0.0f; // Reset timer

                    // 2. Reset Snake Position to Center of Grid
                    for (int i = 0; i < snakeLength; i++)
                    {
                        snakePosition[i][0] = gridCountX / 2;
                        snakePosition[i][1] = gridCountY / 2;
                    }

                    // 3. Respawn Food to ensure it's not under the new snake
                    bool validSpawn = false;
                    while (!validSpawn)
                    {
                        foodX = GetRandomValue(0, gridCountX - 1);
                        foodY = GetRandomValue(0, gridCountY - 1);
                        validSpawn = true;
                        // Check against snake body
                        for (int i = 0; i < snakeLength; i++)
                        {
                            if (snakePosition[i][0] == foodX && snakePosition[i][1] == foodY)
                            {
                                validSpawn = false;
                                break;
                            }
                        }
                    }
                }

                // Return to menu logic (Existing)
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    stateofgame = 0;
                }
            }
            EndDrawing();
        }

        // EXIT
        if (stateofgame == 4)
            break;
    }

    CloseWindow();
    return 0;
}