# UMBRA

## Overview

UMBRA is a bullet hell game where you move the moon to dodge the bullets.

The game is built using Raylib.

---

## Movement

  * Control the Earth using the left stick of a gamepad.
  * Control the Moon using the right stick.
  
---

## Requirements

* **Raylib**: The game uses the [Raylib](https://www.raylib.com/) library for graphics and input handling.

  * Make sure to install Raylib before compiling this game.
* **C Compiler**: This project is written in C. You will need a C compiler (such as GCC or Clang) to compile the source code.

---

## Installation

1. **Install Raylib:**

   * For **Linux**:

     ```bash
     sudo apt-get install libraylib-dev
     ```
   * For **Windows**:

     * Download the Raylib installer from [here](https://github.com/raysan5/raylib/releases).
   * For **macOS**:

     ```bash
     brew install raylib
     ```

2. **Clone the repository (or copy the source code):**

   ```bash
   git clone https://github.com/your-username/umbra.git
   cd umbra
   ```

3. **Compile the code:**
   Use the following command to compile the game (ensure Raylib is properly linked):

   ```bash
   gcc -o umbra main.c -lraylib -lm -lpthread -ldl -lrt -lX11
   ```

4. **Run the game:**
   Once compiled, you can run the game:

   ```bash
   ./umbra
   ```

---

## TODO

* **Bullet Type Variations**: Implement different bullet types with unique behaviors.
* **Game Objectives**: Introduce a goal or scoring system to the game.
* **Menu and UI**: Add a start menu, pause menu, and game over screen.
* **Sound Effects**: Implement sound effects for bullet shots, collisions, and movement.

---

## License

This project is open-source and available under the MIT License.

---

## Contact

* [https://github.com/Zimodo](https://github.com/Zimodo)
