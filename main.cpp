#include "game.h"

int main() {
    Game game;
    game.Init();

    while (!game.ShouldClose()) {
        game.ProcessInput();
        game.Update();
        game.Render();
    }

    game.Cleanup();
    return 0;
}
