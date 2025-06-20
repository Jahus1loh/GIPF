# GIPF Board Game

## Overview

This C++ project implements a hexagonal board game with unique movement mechanics and capture rules. The game features a dynamic board where players can push pieces in six directions, with special chain capture mechanics when certain conditions are met.

## Game Features

- Hexagonal game board with configurable size
- Two players (White and Black) with pawns
- Unique "pushing" movement mechanics in 6 directions
- Chain capture system when pawns form specific patterns
- Board state validation and error checking
- Move generation and game state analysis

## Game Rules

1. Players alternate turns placing or moving pawns
2. Pawns can be pushed in one of six directions:
   - Left/Right
   - Top-Left/Top-Right
   - Down-Left/Down-Right
3. When a chain of K pawns is formed (where K is configurable), all pawns in that row are captured
4. The game ends when a player runs out of reserve pawns

## Code Structure

```
Game.hpp/cpp      - Main game logic and state management
Main.cpp          - Program entry point and command processing
```

## Key Components

- Hexagonal board representation
- Movement in 6 directions
- Chain detection and capture mechanics
- Board state validation
- Move generation
- Input/output handling

## Building and Running

Compile with:
```bash
g++ -std=c++17 -o hexgame Game.cpp main.cpp
```

Run with:
```bash
./hexgame
```

## Input Commands

The program accepts several commands:

- `LOAD_GAME_BOARD` - Load a new game state
- `PRINT_GAME_BOARD` - Display the current board
- `DO_MOVE <from>-<to>` - Execute a move
- `GEN_ALL_POS_MOV_NUM` - Generate all possible moves
- `IS_GAME_OVER` - Check if game has ended
- `BOARD` - Debug board display

## Board Format

The board is represented as a diamond-shaped grid where:
- `W` represents white pawns
- `B` represents black pawns
- `_` represents empty spaces
- Other positions are out of bounds

## Example Game State

```
5 5 10 10
3 3 W
    _ _ _ _ _
   _ W W W _
  _ W B W _
   _ W W _
    _ _ _
```

## Error Handling

The game includes comprehensive error checking for:
- Invalid board states
- Incorrect pawn counts
- Illegal moves
- Full rows when attempting to push

## License

This project is for educational purposes. Redistribution requires attribution.
