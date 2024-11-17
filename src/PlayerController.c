#include <windows.h>
#include <stdio.h>

#include "GameObject.h"
#include "PlayerController.h"

void process_player_input(GameObject *game) {
    // Handle movement
    if (GetAsyncKeyState(VK_LCONTROL) & 0x8000) { // Check if 'W' is pressed (move up)
        if (!game->player.isAirborne) {  // Only jump if not already jumping
            game->player.isAirborne = 1;
            game->player.verticalVelocity = -1.5;  // Initial jump velocity
        }
    }
    if (GetAsyncKeyState('W') & 0x8000) { // Check if 'W' is pressed (gun up)
        game->weaponUpwards = 1;
    } else {
        game->weaponUpwards = 0;
    }
    if (GetAsyncKeyState(VK_UP) & 0x8000) { // Check if '^' is pressed (gun up)
        game->weaponUpwards = 1;
    }  else {
        game->weaponUpwards = 0;
    }
    if (GetAsyncKeyState('S') & 0x8000) { // Check if 'S' is pressed (move down)
        // game->player.y += game->player.velocity;
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) { // Check if '<-' is pressed (move left)
        game->player.horizontalVelocity -= 0.01;
    }
    if (GetAsyncKeyState('A') & 0x8000) { // Check if 'A' is pressed (move left)
        game->player.horizontalVelocity -= 0.01;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) { // Check if '->' is pressed (move right)
        game->player.horizontalVelocity += 0.01;
    }
    if (GetAsyncKeyState('D') & 0x8000) { // Check if 'D' is pressed (move right)
        game->player.horizontalVelocity += 0.01;
    }
    if (GetAsyncKeyState('1') & 0x8000) { // Check if '1' is pressed (select weapon 1)
        game->weaponSelected = 0;
    }
    if (GetAsyncKeyState('2') & 0x8000) { // Check if '2' is pressed (select weapon 2)
        game->weaponSelected = 1;
    }
    if (GetAsyncKeyState('3') & 0x8000) { // Check if '3' is pressed (select weapon 3)
        game->weaponSelected = 2;
    }
    // Handle jumping (spacebar)
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if(game->isHintBoxVisible) {
            game->isHintBoxVisible = 0;
            game_reset(game);
        } else {
            game->weaponFired = 1;
        }
    }
}

void player_update(GameObject *game) {
    // Check if the player has died
    if (game->player.y >= WORLD_HEIGHT - 1) {
        game->isHintBoxVisible = 1;
        wcscpy(game->hintBoxText, L"You have died!");
        return;
    }

    // Limit horizontal velocity to -SpeedLimit to SpeedLimit
    if (game->player.horizontalVelocity > game->player.speedLimit) {
        game->player.horizontalVelocity = game->player.speedLimit;
    } else if (game->player.horizontalVelocity < -game->player.speedLimit) {
        game->player.horizontalVelocity = -game->player.speedLimit;
    }    

    // Check if the player has hit a wall
    if (game->player.x == 0 || game->world.worldMap[(int)game->player.y][(int)(game->player.x+game->player.horizontalVelocity)].type == GROUND) {  
        game->player.horizontalVelocity = 0;
    }

    // Apply horizontal movement
    game->player.x += game->player.horizontalVelocity;

    // Check for player movement to update facing direction
    if (game->player.horizontalVelocity > 0) {
        game->player.facingDirection = 1;  // Facing right
    } else if (game->player.horizontalVelocity < 0) {
        game->player.facingDirection = -1; // Facing left
    }

    // Bound the player's x position to the world width
    if (game->player.x > WORLD_WIDTH - 2) { 
        game->player.x = WORLD_WIDTH - 2;
    } else if (game->player.x < 1) {
        game->player.x = 1;
    }

    // Check if the player has stepped on a ramp
    if (game->world.worldMap[(int)game->player.y][(int)game->player.x].type == RAMP_UP || 
        game->world.worldMap[(int)game->player.y][(int)game->player.x].type == RAMP_DOWN) {
        game->player.y -= 1;
    }

    // Apply friction to the horizontal velocity
    game->player.horizontalVelocity *= 0.5;

    // Check if the player is in the air due to movement
    if (game->world.worldMap[(int)game->player.y+1][(int)game->player.x].type == AIR) {
        game->player.isAirborne = 1;
    }

    // Apply gravity and update the vertical position
    if (game->player.isAirborne) {
        // Apply gravity (negative acceleration)
        game->player.verticalVelocity += game->gravity;  // Gravity, can be adjusted for strength

        // Update the player's y position based on vertical velocity
        game->player.y += game->player.verticalVelocity;

        // Check if the player has hit the ground
        int groundLevel = getGroundLevel(&game->world, game->player.x) - 1;
        if (game->player.y >= groundLevel) {
            game->player.y = groundLevel;  // Reset to ground level
            game->player.isAirborne = 0;  // Stop falling
            game->player.verticalVelocity = 0;  // Reset vertical velocity
        }
    }
}