[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/cTM0vFuq)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-7f7980b617ed060a017424585567c406b6ee15c891e84e1186181d67ecf80aa0.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=12812059)
# README.md for Coursework: Game of Life in C

## Overview
This coursework involves implementing John Conway's Game of Life in C with a twist – the program will read the starting arrangement of cells from a file and also custom rules for cell survival and birth. This simulation is an excellent way to practice file I/O, dynamic memory management, and algorithmic thinking.

## Description
The Game of Life is a cellular automaton devised by mathematician John Conway. The game is a zero-player game, meaning its evolution is determined by its initial state, with no further input from human players. It consists of a grid of cells which, based on a few mathematical rules, can live, die or multiply. The standard version has a simple set of rules:
1. Any live cell with two or three live neighbours survives.
2. Any dead cell with three live neighbours becomes a live cell.
3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.

In this coursework, students will implement the Game of Life with the ability to:
- Read an initial grid state from a file.
- Apply custom rules for cell survival and creation.

## Custom Rules
The twist in this coursework is the custom rules feature. Students will program the Game of Life such that it can read and apply rules specified in a configuration file. For example:
- A live cell surrounded by 5 or more cells dies due to overpopulation.
- An empty cell surrounded by 4 cells, instead of the usual 3, becomes alive.

## Requirements
Students must create a program in C that:
- Reads and parses an initial state from a file.
- Reads and applies custom rules for the evolution of the game.
- Processes the grid to apply the rules for each generation.
- Outputs each subsequent generation to the console or a file.
- Handles edge cases and errors gracefully.

## File Format
- The initial configuration file should contain the grid size and the initial state of the grid.
- The rules file should specify the number of neighbours that cause a live cell to die and a dead cell to become live.

## Example Files
`initial_state.txt`:
```
5 5
0 0 1 0 0
0 1 1 1 0
1 1 0 1 1
0 0 1 0 0
0 0 0 0 0
```

`rules.txt`:
```
Survival: 2 3
Birth: 4
Death: 5+
```

## Compilation and Execution
Students should include a Makefile for compiling the program and a .gitignore file to exclude binary and temporary files from their git repository.

## Evaluation
The program will be evaluated based on:
- Correctness of the implementation.
- Ability to read and apply custom rules.
- Code organization and readability.
- Documentation and commenting.

## Resources
- [Game of Life Explanation](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
- [C Programming Language](https://en.cppreference.com/w/c/language)
- [File I/O in C](https://en.cppreference.com/w/c/io)
.

---



# Jekabs information

## How to setup the code
    Use 
    - gcc game.c -o game.o -lm
    command to compile the code
    And then use ./game to start the game