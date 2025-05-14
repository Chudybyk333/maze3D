//
// Created by bramv on 28/03/2023.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "Game.h"

int main()
{
    srand(unsigned(time(NULL)));
    Game game = Game(1920, 1080);
    game.mainloop();
}
