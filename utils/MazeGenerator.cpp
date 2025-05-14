#include "MazeGenerator.h"

MazeGenerator::MazeGenerator(int width, int height)
    :   directionMaze{std::vector(height, std::vector(width, 0))},
        width{width},
        height{height},
        mazeBuilder{Maze::MazeBuilder(2*width+1, 2*height+1)} // 2*width+1, 2*height+1
{
    generateMaze(0, 0);

    Direction north = directions[0];
    Direction south = directions[1];
    Direction east = directions[2];
    Direction west = directions[3];

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int bit = directionMaze[y][x];
            // std::cout << ((bit & 1) != 0) << ((bit & 2) != 0) << ((bit & 4) != 0) << ((bit & 8) != 0) << " ";
            int xWorld = x*2 + 1;
            int yWorld = y*2 + 1;
            // add default walls
            mazeBuilder.addWall(xWorld-1, yWorld+1); // north-west
            mazeBuilder.addWall(xWorld+1, yWorld+1); // north-east
            mazeBuilder.addWall(xWorld-1, yWorld-1); // south-west
            mazeBuilder.addWall(xWorld+1, yWorld-1); // south-east
            // add walls to blocked paths
            if ((bit & 1) == 0){ // north wall
                mazeBuilder.addWall(xWorld, yWorld - 1);
            }
            if ((bit & 2) == 0){ // south wall
                mazeBuilder.addWall(xWorld, yWorld + 1);
            }
            if ((bit & 4) == 0){ // east wall
                mazeBuilder.addWall(xWorld + 1, yWorld);
            }
            if ((bit & 8) == 0){ // west wall
                mazeBuilder.addWall(xWorld - 1, yWorld);
            }
        }
    }
}    

/**
 * @brief maze generation with Depth-first search
 * 
 * @param cx : current x
 * @param cy : current y
 */
void MazeGenerator::generateMaze(int cx, int cy)
{
    std::vector<Direction> directions = getShuffledDirections();
    for (Direction direction : directions){
        int nx = cx + direction.dx; // new x
        int ny = cy + direction.dy; // new y
        if (between(nx, width) && between(ny, height) && (directionMaze[ny][nx] == 0)){
            directionMaze[cy][cx] |= direction.bit; // add direction bit to current position, represents an open path to this direction
            directionMaze[ny][nx] |= direction.oppositeBit; // add direction bit to new position (opposite direction)
            generateMaze(nx, ny);
        }
    }
}

Maze* MazeGenerator::getMaze(){
    return mazeBuilder.build();
}

std::vector<Direction> MazeGenerator::getShuffledDirections()
{
    std::vector<Direction> directionsCopy = directions;
    std::random_shuffle(std::begin(directions), std::end(directions));
    return directions;
}

bool MazeGenerator::between(int v, int upper){
    return (v >= 0) && (v < upper);
}