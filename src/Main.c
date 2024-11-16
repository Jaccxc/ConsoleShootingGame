#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "GameObject.h"

int main() {
    GameObject game;

    // Initialize the game object
    game_init(&game);

    // Main game loop
    while (1) {
        // Handle input (key presses, etc.)
        game_handle_input(&game);

        // Game logic
        game_update(&game);

        // Render the game
        game_render(&game);
    }

    return 0;
}
