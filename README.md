# 🐍 Snake Game - Ultimate Version

## 👥 Group Information

**Group Name:** 404 Not Found
**Members:**

* Muneeb ur Rehman
* Muhammad Umais
* Sadia Sahar

---

## 🎮 Overview

Welcome to the **Snake Game - Ultimate Version**.
This is a robust implementation of the classic arcade game built from scratch using **C++** and the **Raylib** library.
It features:

* Multiple difficulty modes
* Story mode with progression
* Persistent save files
* Dynamic themes

---

## 🛠️ Requirements & Setup

### **1. The Only Dependency: Raylib**

Raylib is the only external library required.
No complex engines or multiple libraries are needed.

### **2. Project Files**

Your C++ compiler (Visual Studio / VS Code / Xcode) must be linked to Raylib.

Steps:

1. Link Raylib
2. Compile `main.cpp`
3. Run the executable

---

## 💻 Code Walkthrough & Explanation

### **📚 1. Libraries and Headers**

* `#include <raylib.h>` — Graphics, window, input
* `#include <stdlib.h>` — Random functions
* `#include <string>` — Text and string handling
* `#include <fstream>` — Save/Load file system
* `#include <vector>` — Dynamic arrays
* `#include <cstdio>` — For deleting save files
* `#include <cmath>` — Used for `ceil()` in Story Mode

---

## ⚙️ 2. Global Structures and Enums

### **`enum GameMode`**

Used to make code readable:

```
EASY = 0
NORMAL = 1
HARD = 2
STORY = 3
```

### **`struct GameState`**

A container holding all game variables:

* `stateofgame` (Menu vs Gameplay)
* `snakePosition[1024][2]` (Snake body coordinates)
* Story Logic:

  * `isLevelTransitioning`
  * `storyLevel`
* File logic:

  * `hasSaveFile`

---

## 📏 3. Global Constants

* `screenWidth` / `screenHeight`
* `cellSize = 40` (Each grid cell is 40×40 pixels)
* `boardOffsetX` — Centers the board on screen

---

## 🧩 4. Function Prototypes

Functions like `ResetGame()` and `DrawMenu()` are declared before `main()` to keep code organized.

---

## 🚀 5. The main() Function

### **Initialization**

* Starts the window using `InitWindow()`
* Sets FPS to 60
* Calls `InitGameGrid()`
* Creates `GameState game`
* Initializes hurdles and checks for save files

### **Game Loop**

Runs until window closes:

* **Update Phase:**

  * Menus → `UpdateMenu()`
  * Gameplay → `UpdateGameplay()`

* **Draw Phase:**
  Draws menu or gameplay screen based on state

---

## 🧠 6. Logic Functions (The Brains)

### **`InitGameGrid()`**

Ensures the grid fits perfectly on the screen.

### **`IsTileBlocked()`**

Checks if a tile is:

* Occupied by snake
* Occupied by a wall

Prevents food spawning inside obstacles.

### **`ResetGame()`**

* Score → 0
* Snake length → 4
* Snake resets to center
* Food spawns safely (checked with loop)

### **`InitHurdles()`**

Draws:

* L-shaped corners
* Middle barrier

### **`SaveGame()` / `LoadGame()`**

Uses file stream to write/read `savefile.txt`.

---

## 🎮 7. Update Functions

### **`UpdateMenu(GameState &game)`**

Handles:

* Arrow key navigation
* Theme switching
* New Game
* Continue

### **`UpdateGameplay(GameState &game)`**

#### Story Mode

* Level up when score > 50 or > 100
* Snake resets safely
* Countdown timer appears

#### Movement & Timer

* `moveTimer` increases with frame time
* When it exceeds `moveInterval`, snake moves

#### Collision Detection

* Wall collision → Game Over
* Hurdle collision → Game Over
* Self collision → Game Over

#### Auto-Saving

Game progress is saved on every move.

---

## 🎨 8. Drawing Functions

### **`DrawMenu()`**

* Updates based on theme (Classic / Desert)
* Draws UI elements

### **`DrawGameplay()`**

* Draws grid
* Draws snake
* Draws food
* Shows overlays:

  * Level-up countdown
  * Game Over screen

---

## 🕹️ Controls

| Action                     | Key        |
| -------------------------- | ---------- |
| Move Snake / Navigate Menu | Arrow Keys |
| Select Menu Option         | Enter      |
| Pause / Go Back            | ESC        |
| Restart After Game Over    | R          |

---
