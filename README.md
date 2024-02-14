# Maze Generator and Solver

## Overview

This repository contains a C++ project that demonstrates the generation and solving of mazes. The maze generation is implemented using the Randomized Depth-First-Search algorithm, and maze solving is achieved using the A* algorithm. The project utilizes the [SFML](https://github.com/SFML/SFML) (Simple and Fast Multimedia Library) graphic library for visualizing the maze generation and solving process.

## Features

- **Maze Generation:** Randomized Depth-First-Search algorithm is employed to generate mazes. This algorithm creates complex and interesting mazes by carving paths through the maze randomly.

- **Maze Solving:** The A* algorithm is utilized to find the optimal path from the start to the end of the generated maze. A* is an efficient pathfinding algorithm that guarantees the shortest path.

- **Visualization:** SFML is used for graphical representation, providing a visual demonstration of maze generation and solving.

- **C++ Environment:** The project is designed to run in a C++ environment, making it platform-independent and easily compilable on different systems.

## Demo

For a quick demo, check out the [demo video](./demo/demo.mp4) to see the project in action.

## Requirements

- C++ compiler
- SFML library
- MSBuild

## Getting Started

1. **Clone the Repository:**
   ```
   git clone https://github.com/duongve13112002/Maze-Generator-and-Solver.git
   cd Maze-Generator-and-Solver/source/Mazegamemakeforfun
   ```

2. **Compile and Build:**
   Ensure that you have a C++ compiler, MSBuild, and SFML library installed. Adjust the Makefile or build script according to your compiler and library paths.
   ```
   msbuild Mazegamemakeforfun.sln
   ```

3. **Run the compiled program:**
   - After a successful compilation, you will find the executable files (e.g., .exe on Windows) in the output directory.

     On Windows:
     ```bash
     Mazegamemakeforfun.exe
     ```

     On Linux/Mac:
     ```bash
     ./Mazegamemakeforfun
     ```

## Usage
- In the program, to navigate up and down, use the up and down arrow keys, and press the Enter key to enter a specific option.
- When adjusting the number of cells in the maze in the options, after entering the desired number, press the "Escape" key to save the entered value.

To run the program, follow these steps:

Step 1: You can skip to step 2 or follow these instructions:
   - Press the 'P' key to change the starting cell and generate a random maze.
   - Left-click on any cell in the window to change it (the one with a green color is the changed cell).
   - Press 'Enter' to complete step 1.

Step 2: Press the 'M' key to generate a random maze.

Step 3: You can skip to step 4 or 5, if not, follow these instructions:
   - Press the 'C' key to start creating walls for the maze.
   - Create walls by left-clicking on any cell you want to wall off.
   - To go back to the previous cell, press the "Backspace" key.
   - Once done, press 'Enter' to complete step 3.

Step 4: You can skip to step 5, if not, follow these instructions:
   - Press the 'F' key to start changing the starting and ending cells for maze solving.
   - Left-click on any cell in the window to change it to the starting cell.
   - Right-click on any cell in the window to change it to the ending cell.
   - Once done, press 'Enter' to complete step 4.

Step 5: Press the 'S' key to view the program finding the path.

## Contributing

Feel free to contribute to the project by opening issues, proposing new features, or submitting pull requests. Your feedback and contributions are highly appreciated.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- The Randomized Depth-First-Search algorithm and A* algorithm are classic computer science algorithms widely used in maze generation and solving.
- [SFML](https://github.com/SFML/SFML) provides a simple and efficient multimedia library for C++.

Enjoy exploring and solving mazes with this project!
