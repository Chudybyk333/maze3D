
#include "MazeLoader.h"

MazeLoader::MazeLoader()
    : mazeBuilder{Maze::MazeBuilder(21, 31)}
{
}

MazeLoader::~MazeLoader()
{
    delete this->mazeFile;
}
Maze *MazeLoader::loadMazeFromFile(std::string fileName)
{
    this->mazeFile = new std::ifstream(fileName);
    loadMaze();
    this->mazeFile->close();

    return mazeBuilder.build();
}

void MazeLoader::loadMaze()
{
    if (this->mazeFile->is_open())
    {
        std::string line;
        while (this->mazeFile->good())
        {
            std::getline(*(this->mazeFile), line);
            loadLine(line);
        }
    }
    else
    {
        std::cout << "Couldn't open file" << std::endl;
    }
}

void MazeLoader::loadLine(const std::string &line)
{
    for (const char &el : line)
    {
        if (WALLCHAR == el)
        {
            mazeBuilder.addWall(this->rowOffset, this->colOffset);
        }
        else if (EMPTYCHAR == el)
        {
        }
        this->colOffset++;
    }
    this->colOffset = 0;
    this->rowOffset++;
}