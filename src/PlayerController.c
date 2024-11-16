#include <windows.h>
#include <stdio.h>

#include "GameObject.h"
#include "PlayerController.h"

void process_player_input(GameObject *game) {
    // Handle movement
    if (GetAsyncKeyState('W') & 0x8000) { // Check if 'W' is pressed (move up)
        // game->player.y -= game->player.velocity;
    }
    if (GetAsyncKeyState('S') & 0x8000) { // Check if 'S' is pressed (move down)
        // game->player.y += game->player.velocity;
    }
    if (GetAsyncKeyState('A') & 0x8000) { // Check if 'A' is pressed (move left)
        game->player.horizontalVelocity -= 1;
    }
    if (GetAsyncKeyState('D') & 0x8000) { // Check if 'D' is pressed (move right)
        game->player.horizontalVelocity += 1;
    }

    // Handle jumping (spacebar)
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if (!game->player.isAirborne) {  // Only jump if not already jumping
            game->player.isAirborne = 1;
            game->player.verticalVelocity = -1.5;  // Initial jump velocity
        }
    }
}