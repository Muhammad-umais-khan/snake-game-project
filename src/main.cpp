#include <raylib.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 600;

    int stateofgame = 0; // 0 = Menu, 1 = Continue, 2 = New Game, 3 = Change Theme, , 4 = Mode 5 = Exit
    int option = 1;

    const int cellSize = 40;
    const int gridWidth = screenWidth / cellSize;
    const int gridHeight = screenHeight / cellSize;
    std::string theme = "Classic";
    std::string gamemode = "Easy";
    int mode = 0; // 1 = Easy, 2 = Medium, 3 = Hard
    bool gameOver = false;
    int score = 0;
    int snakeLength = 4;
    int snakeX = screenWidth / 20;
    int snakeY = screenHeight / 20;
    int snakePosition[255][2] = {0};

    for (int i = 0; i < snakeLength; i++)
    {
        snakePosition[i][0] = snakeX;
        snakePosition[i][1] = snakeY;
    }

    char key = 'R';

    Color backgroundColor = {150, 180, 110, 255};
    Color gridColor = {100, 130, 90, 255};
    Color foodColor = {220, 0, 0, 255};
    Color snakebodyColor = {50, 70, 40, 255};

    Color backgroundColorSpecial = {255, 246, 199, 255};
    Color gridColorSpecial = {232, 223, 194, 255};
    Color snakebodyColorSpecial = {15, 26, 51, 255};
    Color foodColorSpecial = {255, 79, 163, 255};
    Color backgroundMenu = {185, 205, 160, 255};
    Color boxesMenu = {160, 190, 140, 255};
    Color selectedOption = {120, 150, 110, 255};
    Color menuText = {60, 80, 50, 255};
    Color menuTextSelected = {255, 255, 255, 255};

    int foodX = 0;
    int foodY = 0;

    // ✔ Correct place: Check save file here ONLY
    {
        std::ifstream savecheck("savefile.txt");
        if (savecheck.is_open()) savecheck.close();
        
    }

    int highscore = 0;
    std::ifstream highscorefile("highscore.txt");
    if (highscorefile.is_open())
    {
        highscorefile >> highscore;
        highscorefile.close();
    }

    InitWindow(screenWidth, screenHeight, "Snake Game - Combined Version");
    SetTargetFPS(10);

    //  Removed wrong loading here
    //  Removed food spawn (should only happen on New Game)

    while (!WindowShouldClose())
    {
        // Highscore update
        if (score > highscore)
        {
            highscore = score;
            std::ofstream highscorefile("highscore.txt");
            if (highscorefile.is_open())
            {
                highscorefile << highscore;
                highscorefile.close();
            }
        }

        // Movement (unchanged)
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

            for (int i = snakeLength; i > 0; i--)
            {
                snakePosition[i][0] = snakePosition[i - 1][0];
                snakePosition[i][1] = snakePosition[i - 1][1];
            }

            if (key == 'R')
                snakePosition[0][0] += 1;
            if (key == 'L')
                snakePosition[0][0] -= 1;
            if (key == 'U')
                snakePosition[0][1] -= 1;
            if (key == 'D')
                snakePosition[0][1] += 1;

            if (snakePosition[0][0] >= gridWidth)
                snakePosition[0][0] = 0;
            if (snakePosition[0][0] < 0)
                snakePosition[0][0] = gridWidth - 1;

            if (snakePosition[0][1] >= gridHeight)
                snakePosition[0][1] = 0;
            if (snakePosition[0][1] < 0)
                snakePosition[0][1] = gridHeight - 1;

            if (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY)
            {
                snakeLength++;
                score += 10;

                do
                {
                    foodX = GetRandomValue(0, gridWidth - 1);
                    foodY = GetRandomValue(0, gridHeight - 1);
                } while (snakePosition[0][0] == foodX && snakePosition[0][1] == foodY);
            }

            for (int i = 1; i < snakeLength; i++)
            {
                if (snakePosition[0][0] == snakePosition[i][0] &&
                    snakePosition[0][1] == snakePosition[i][1])
                {
                    gameOver = true;
                }
            }
        }
        // -------- SAVE GAME ---------
        if (!gameOver && (stateofgame == 1 || stateofgame == 2))
        {
            std::ofstream save("savefile.txt");
            if (save.is_open())
            {
                save << snakeLength << "\n";
                save << score << "\n";
                save << key << "\n";
                save << foodX << "\n";
                save << foodY << "\n";

                for (int i = 0; i < snakeLength; i++)
                {
                    save << snakePosition[i][0] << " " << snakePosition[i][1] << "\n";
                }

                save.close();
            }
        }

        BeginDrawing();

        // -------------------- MENU --------------------
        if (stateofgame == 0){
            ClearBackground(backgroundMenu);

            if (IsKeyPressed(KEY_DOWN))
                option++;
            if (IsKeyPressed(KEY_UP))
                option--;
            if(IsKeyPressed(KEY_RIGHT) && (option ==4) && (mode !=0))
                mode++;
            if(IsKeyPressed(KEY_LEFT) && (option ==4) && (mode !=0))
                mode--;
            if (option > 5)
                option = 1;
            if (option < 1)
                option = 5;

            if (IsKeyPressed(KEY_ENTER)){
                stateofgame = option;

                // change theme
                if ((stateofgame == 3) && (theme == "Classic")){
                    theme = "Desert";
                    backgroundMenu = {240, 225, 185, 255};
                    boxesMenu = {225, 210, 178, 255};
                    selectedOption = { 25,  40,  75, 255}; 
                    menuText = { 60,  45,  25, 255};
                    menuTextSelected = {255, 255, 255, 255};
                    stateofgame = 0; // stay in menu

                }
                else if (stateofgame == 3 && theme == "Desert"){
                    theme = "Classic";
                    backgroundMenu = {185, 205, 160, 255};
                    boxesMenu = {160, 190, 140, 255};
                    selectedOption = {120, 150, 110, 255};
                    menuText = {60, 80, 50, 255};
                    menuTextSelected = {255, 255, 255, 255};
                    stateofgame = 0; // stay in menu

                    }

                // ✔ NEW GAME: Reset + spawn food here ONLY
                if (stateofgame == 2){
                    snakeLength = 4;
                    score = 0;
                    key = 'R';

                    for (int i = 0; i < snakeLength; i++){
                        snakePosition[i][0] = snakeX;
                        snakePosition[i][1] = snakeY;
                    }

                    do{
                        foodX = GetRandomValue(0, gridWidth - 1);
                        foodY = GetRandomValue(0, gridHeight - 1);
                    } while (foodX == snakeX && foodY == snakeY);
                    }

                // ✔ CONTINUE: Load AFTER pressing Enter
                if (stateofgame == 1){
                std::ifstream load("savefile.txt");
                    if (load.is_open()){
                        load >> snakeLength;
                        load >> score;
                        load >> key;
                        load >> foodX;
                        load >> foodY;

                        for (int i = 0; i < snakeLength; i++){
                            load >> snakePosition[i][0];
                            load >> snakePosition[i][1];
                            }

                        load.close();
                        }
                    }
                if(stateofgame == 4){
                    // Adding mode toggle functionality
                    mode = 1; 
                    stateofgame = 0; // return to menu after selecting mode
                    }
                }

            // Draw menu UI (unchanged)
            DrawText("SNAKE GAME", screenWidth / 2 - 150, screenHeight / 2 - 180, 50, menuText);

            if (option == 1){
                DrawRectangle(screenWidth / 2 - 160, screenHeight / 2 - 100, 320, 50, selectedOption);
                DrawText("Continue", screenWidth / 2 - 60, screenHeight / 2 - 85, 25, menuTextSelected);
            }
            else{
                DrawRectangleLines(screenWidth / 2 - 160, screenHeight / 2 - 100, 320, 50, boxesMenu);
                DrawText("Continue", screenWidth / 2 - 60, screenHeight / 2 - 85, 25, menuText);
            }

            if (option == 2){
                DrawRectangle(screenWidth / 2 - 160, screenHeight / 2 - 30, 320, 50, selectedOption);
                DrawText("New Game", screenWidth / 2 - 70, screenHeight / 2 - 15, 25, menuTextSelected);
            }
            else{
                DrawRectangleLines(screenWidth / 2 - 160, screenHeight / 2 - 30, 320, 50, boxesMenu);
                DrawText("New Game", screenWidth / 2 - 70, screenHeight / 2 - 15, 25, menuText);
            }

            if (option == 3){
                DrawRectangle(screenWidth / 2 - 160, screenHeight / 2 + 40, 320, 50, selectedOption);
                DrawText("Desert Theme", screenWidth / 2 - 90, screenHeight / 2 + 55, 25, menuTextSelected);
            }
            else{
                DrawRectangleLines(screenWidth / 2 - 160, screenHeight / 2 + 40, 320, 50, boxesMenu);
                DrawText("Desert Theme", screenWidth / 2 - 90, screenHeight / 2 + 55, 25, menuText);
            }

            if (option == 4){
                DrawRectangle(screenWidth / 2 - 160, screenHeight / 2 + 110, 320, 50, selectedOption);
                DrawText("Mode", screenWidth / 2 - 45, screenHeight / 2 + 125, 25, menuTextSelected);
            }
            else{
                DrawRectangleLines(screenWidth / 2 - 160, screenHeight / 2 + 110, 320, 50, boxesMenu);
                DrawText("Mode", screenWidth / 2 - 45, screenHeight / 2 + 125, 25, menuText);
            }
            if (option == 5){
                DrawRectangle(screenWidth / 2 - 160, screenHeight / 2 + 180, 320, 50, selectedOption);
                DrawText("Exit", screenWidth / 2 - 45, screenHeight / 2 + 195, 25, menuTextSelected);
            }
            else{
                DrawRectangleLines(screenWidth / 2 - 160, screenHeight / 2 + 180, 320, 50, boxesMenu);
                DrawText("Exit", screenWidth / 2 - 45, screenHeight / 2 + 195, 25, menuText);
            }
            if((mode != 0) && (option ==4)){
                if(mode>3) mode =0; // just in case
                if(mode == 1){
                    DrawRectangle(screenWidth / 2+ 180, screenHeight / 2 + 40, 320, 50, selectedOption);
                    DrawText("Easy", screenWidth / 2 + 295, screenHeight / 2 + 55, 25,  menuTextSelected);
                }
                else {
                    DrawRectangleLines(screenWidth / 2+ 180, screenHeight / 2 + 40, 320, 50, boxesMenu);
                    DrawText("Easy", screenWidth / 2 + 295, screenHeight / 2 + 55, 25, menuText);
                }
                if(mode == 2){
                    DrawRectangle(screenWidth / 2+ 180, screenHeight / 2 + 110, 320, 50, selectedOption);
                    DrawText("Medium", screenWidth / 2 + 295, screenHeight / 2 + 125, 25, menuTextSelected);
                }
                else {
                    DrawRectangleLines(screenWidth / 2+ 180, screenHeight / 2 + 110, 320, 50, boxesMenu);
                    DrawText("Medium", screenWidth / 2 + 295, screenHeight / 2 + 125, 25, menuText);
                }
                if(mode ==3){
                    DrawRectangle(screenWidth / 2+ 180, screenHeight / 2 + 180, 320, 50, selectedOption);
                    DrawText("Hard", screenWidth / 2 + 295, screenHeight / 2 + 195, 25, menuTextSelected);
                }
                else {
                    DrawRectangleLines(screenWidth / 2+ 180, screenHeight / 2 + 180, 320, 50, boxesMenu);
                    DrawText("Hard", screenWidth / 2 + 295, screenHeight / 2 + 195, 25, menuText);
                }
            }
            EndDrawing();
            continue;
        }

        // -------------------- GAME SCREEN --------------------
        if (stateofgame == 1 || stateofgame == 2)
        {
            if (theme == "Desert")
                ClearBackground(backgroundColorSpecial);
            else
                ClearBackground(backgroundColor);

            for (int i = 0; i < gridWidth; i++)
            {
                for (int j = 0; j < gridHeight; j++)
                {
                    if (theme == "Desert")
                        DrawRectangleLinesEx((Rectangle){i * cellSize, j * cellSize, cellSize, cellSize}, 2, gridColorSpecial);
                    else
                        DrawRectangleLinesEx((Rectangle){i * cellSize, j * cellSize, cellSize, cellSize}, 2, gridColor);
                }
            }

            if (theme == "Desert")
                DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, foodColorSpecial);
            else
                DrawRectangle(foodX * cellSize, foodY * cellSize, cellSize, cellSize, foodColor);

            for (int i = 0; i < snakeLength; i++)
            {
                if (theme == "Desert")
                    DrawRectangle(snakePosition[i][0] * cellSize, snakePosition[i][1] * cellSize, cellSize, cellSize, snakebodyColorSpecial);
                else
                    DrawRectangle(snakePosition[i][0] * cellSize, snakePosition[i][1] * cellSize, cellSize, cellSize, snakebodyColor);
            }

            std::string scoretext = "Score: " + std::to_string(score);
            DrawText(scoretext.c_str(), screenWidth - 180, 8, 30, DARKGRAY);

            if (gameOver)
            {
                DrawText("GAME OVER!", screenWidth / 2 - 150, screenHeight / 2 - 50, 60, RED);
                DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight / 2 + 20, 20, DARKGRAY);

                // ✔ Delete save file ONLY on death
                std::ifstream savefile("savefile.txt");
                if (savefile.is_open())
                {
                    savefile.close();
                    remove("savefile.txt");
                }
            }

            EndDrawing();
        }

        if (stateofgame == 5)
            break;
    }

    CloseWindow();
    return 0;
}
