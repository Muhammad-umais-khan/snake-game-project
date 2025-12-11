# 🐍 Snake Game — Ultimate Version

A feature-rich implementation of the classic **Snake** game written in **C++** using the **Raylib** library. Includes multiple game modes (Easy / Normal / Hard / Story), persistent save & high-score, dynamic themes, and polished visuals.

---

## Table of Contents

* [Demo / Screenshots](#demo--screenshots)
* [Features](#features)
* [Controls](#controls)
* [Prerequisites](#prerequisites)
* [Folder Structure](#folder-structure)
* [Build & Run](#build--run)
* [Game Modes Explained](#game-modes-explained)
* [Save Files](#save-files)
* [Project Info & Credits](#project-info--credits)
* [License](#license)

---

## Demo / Screenshots

*(Add screenshots/gifs of your game here for a nicer GitHub README.)*

---

## Features

* **4 Game Modes**

  * Easy (wrap-around, no walls)
  * Normal (walls active)
  * Hard (walls + static obstacles)
  * Story (progressive difficulty with autosave)
* **Save & Load** for Story mode (auto-save + continue)
* **High Score Tracking**
* **Dynamic Themes** (e.g., Classic / Desert)
* **Polished visuals** (eye mechanics, transitions, animations)

---

## Controls

| Key        | Action                                |
| ---------- | ------------------------------------- |
| Arrow Keys | Move snake (Up / Down / Left / Right) |
| Enter      | Select menu option                    |
| ESC        | Pause / Return to menu                |
| R          | Restart (on Game Over screen)         |

---

## Prerequisites

### Core Tools

* **C++ compiler:** `g++` (MinGW on Windows), `clang`, or MSVC.
* **Code editor (recommended):** Visual Studio Code, or Visual Studio Community on Windows.

### Required Library

* **Raylib** — games use Raylib for graphics/input/audio.

  * Download Raylib releases from the official repo: `https://github.com/raysan5/raylib/releases`
  * On Windows the **w64devkit** installer or prebuilt packages are the easiest route.

---

## Folder Structure (recommended)

```
Project Folder/
├── main.cpp          # Source code entry point
├── lib/              # raylib library files (e.g., libraylib.a)
├── include/          # raylib.h and other headers
├── assets/           # images, fonts, audio (if any)
├── savefile.txt      # (auto-created) story progress
├── highscore.txt     # (auto-created) persistent high score
└── .vscode/          # optional VS Code tasks / launch configs
```

---

## Build & Run

### Command-Line (Windows with g++)

If Raylib is installed and available to the linker:

```bash
g++ main.cpp -o SnakeGame -lraylib -lgdi32 -lwinmm
```

> The `-lgdi32 -lwinmm` flags are needed on Windows. Linux/macOS generally require only `-lraylib` (platform-specific audio/window libs may vary).

### Example (Linux/macOS)

```bash
g++ main.cpp -o SnakeGame -lraylib
```

### Using Visual Studio Code

1. Place `raylib` headers in `include/` and library files in `lib/` (or install raylib system-wide).
2. Create a `.vscode/tasks.json` entry that compiles with the proper flags:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "Build SnakeGame",
      "type": "shell",
      "command": "g++",
      "args": [
        "main.cpp",
        "-o",
        "SnakeGame",
        "-lraylib",
        "-lgdi32",
        "-lwinmm"
      ],
      "group": "build"
    }
  ]
}
```

3. Run the build task (`Terminal → Run Build Task`) and execute the produced binary.

---

## Game Modes Explained

* **Easy**

  * No walls: the snake wraps around screen edges.
  * Good for learning controls and mechanics.
* **Normal**

  * Standard rules: walls are fatal.
* **Hard**

  * Walls + static obstacles to navigate around.
* **Story**

  * Progressive difficulty that changes with score:

    * Level 1 (0–50 pts): beginner-friendly.
    * Level 2 (50–100 pts): walls active.
    * Level 3 (100+ pts): obstacles spawn.
  * Story mode **auto-saves** progress so you can continue later.

---

## Save Files

* `savefile.txt` — automatically created (Story mode progress).
* `highscore.txt` — stores the persisted high score across sessions.

> Ensure your game has write permission in the project directory so these files can be created.

---

## Notes & Tips

* If compilation fails with linker errors, double-check:

  * Raylib library path (e.g., `-L/path/to/lib`) and include path (e.g., `-I/path/to/include`).
  * On Windows, ensure MinGW/MSYS environments are consistent with the compiler used.
* For smooth development in VS Code, install the C/C++ extension and configure your includePath and compilerPath in `c_cpp_properties.json`.

---

## Contributing

If you want to improve visuals, add new themes, or expand the Story mode:

1. Fork the repo
2. Create a feature branch
3. Open a PR with a clear description of changes

Include a short changelog entry and verify the game still builds on at least one platform.

---

## Project Info & Credits

* **Engine / Library:** Raylib ([https://www.raylib.com](https://www.raylib.com) / [https://github.com/raysan5/raylib](https://github.com/raysan5/raylib))
* **Language:** C++ (std library: `<string>`, `<fstream>`, `<cmath>`, etc.)
* **Created by:** *Your Name / Team Name Here* — update this before publishing.

---

## License

Specify a license for your repository. Example:

```
MIT License
```

(Replace with the license you prefer and add a `LICENSE` file.)

---

If you want, I can:

* generate a ready-to-commit `README.md` file with badges and optional screenshots placeholders, or
* produce a `.vscode/tasks.json` and `c_cpp_properties.json` tailored for your environment (MinGW / Windows or Linux).
