#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../objects/based/Cube.h"
#include "../core/Maze.h"
#include "MazeGenerator.h"

class MazeLoader
{
public:
    static const char WALLCHAR = '#';
    static const char EMPTYCHAR = ' ';
    MazeLoader();
    ~MazeLoader();

    Maze *loadMazeFromFile(std::string fileName);

private:
    void loadMaze();
    void loadLine(const std::string &line);

    Maze::MazeBuilder mazeBuilder;

    std::ifstream *mazeFile;

    int colOffset = 0;
    int rowOffset = 0;
};