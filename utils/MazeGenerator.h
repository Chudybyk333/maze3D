#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include "../core/Maze.h"

struct Direction
{
    int bit;
    int dx;
    int dy;
    int oppositeBit;
};

class MazeGenerator
{
private:
    Maze::MazeBuilder mazeBuilder;
    std::vector<std::vector<int>> directionMaze; // contains bits that represent the open/closed directions
    int width = 0;
    int height = 0;
    std::vector<Direction> directions = {
        Direction{1, 0, -1, 2}, // North
        Direction{2, 0, 1, 1},  // South
        Direction{4, 1, 0, 8},  // East
        Direction{8, -1, 0, 4}  // West
    };

    void generateMaze(int cx, int cy);
    std::vector<Direction> getShuffledDirections();
    bool between(int v, int upper);

public:
    MazeGenerator(int width = 10, int height = 10);
    Maze *getMaze();
};