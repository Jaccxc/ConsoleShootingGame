#include <wchar.h>
#include <stdio.h>

#include "GameObject.h"
#include "Graphics.h"
#include "PlayerController.h"
#include "WorldObject.h"

void game_init(GameObject *game)
{
    // Initialize game variables
    game->gravity = 0.1;

    // Initialize World
    load_seed_world(&game->world);
    generate_world(&game->world);

    // Initialize Player
    game->player.x = 5;
    game->player.y = 5;
    game->player.verticalVelocity = 0;
    game->player.horizontalVelocity = 0;
    game->player.jumpHeight = 10;
    game->player.isAirborne = 1;

    // Initialize Rendering
    game->hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    set_console_size(game);

    // Initialize high-resolution timer
    QueryPerformanceFrequency(&game->fpsFrequency);  // Get the frequency of the high-resolution counter
    QueryPerformanceCounter(&game->fpsLastTime);     // Get the initial timestamp

    QueryPerformanceFrequency(&game->tpsFrequency);  // Get the frequency of the high-resolution counter
    QueryPerformanceCounter(&game->tpsLastTime);     // Get the initial timestamp
}

void game_handle_input(GameObject *game)
{
    // Handle input 
    process_player_input(game);
}

void game_update(GameObject *game)
{
    fps_update(game);
    
    if(not_yet_to_tick(game))
        return;
    
    tps_update(game);

    // Limit horizontal velocity to -1 to 1
    if (game->player.horizontalVelocity > 1) {
        game->player.horizontalVelocity = 1;
    } else if (game->player.horizontalVelocity < -1) {
        game->player.horizontalVelocity = -1;
    }
    

    // Check if the player has hit a wall
    if (game->player.x == 0 || game->world.worldMap[(int)game->player.y][(int)(game->player.x+game->player.horizontalVelocity)].type == GROUND) {  
        game->player.horizontalVelocity = 0;
    }

    // Apply horizontal movement
    game->player.x += game->player.horizontalVelocity;

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

void game_render(GameObject *game)
{
    // Render the game object
    clear_screen_buffer(game);
    draw_game_world(game);
    draw_ui(game);
    write_to_console(game);
}

void fps_update(GameObject *game) {
    // Calculate FPS
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double elapsedTime = (double)(currentTime.QuadPart - game->fpsLastTime.QuadPart) / game->fpsFrequency.QuadPart;

    if (elapsedTime > 0) 
        game->fps = 1.0f / (float)elapsedTime;

    game->fpsLastTime = currentTime;

    // Update FPS string
    fps_tps_format(game, FPS);
}

void tps_update(GameObject *game) {
    // Update tps
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double elapsedTime = (double)(currentTime.QuadPart - game->tpsLastTime.QuadPart) / game->tpsFrequency.QuadPart;

    if (elapsedTime > 0) 
        game->tps = 1.0f / (float)elapsedTime;

    game->tpsLastTime = currentTime;

    // Update TPS string
    fps_tps_format(game, TPS);
}

void fps_tps_format(GameObject *game, int fpsOrTps) {
    // Calculate the FPS (assuming game->fps is already updated)
    float data = (float)game->fps;
    WCHAR result[30];

    if(fpsOrTps == FPS) {
        data = game->fps;
    } else {
        data = game->tps;
    }

    // Calculate the integer part and the fractional part separately
    int integerPart = (int)data;
    int fractionalPart = (int)((data - integerPart) * 100);  // Take 2 decimal places

    int index = 0;

    // Add "FPS: " or "TPS: "
    if (fpsOrTps == FPS) {
        result[index++] = L'F';
    } else {
        result[index++] = L'T';
    }
    result[index++] = L'P';
    result[index++] = L'S';
    result[index++] = L':';
    result[index++] = L' ';

    // Add the integer part
    if (integerPart > 999) {
        result[index++] = L'0' + (integerPart / 1000);  // Thousands digit
        integerPart %= 1000;
    }
    if (integerPart > 99) {
        result[index++] = L'0' + (integerPart / 100);  // Hundreds digit
        integerPart %= 100;
    }
    if (integerPart > 9) {
        result[index++] = L'0' + (integerPart / 10);  // Tens digit
        integerPart %= 10;
    }
    result[index++] = L'0' + (integerPart % 10);  // Ones digit

    // Add the decimal point
    result[index++] = L'.';

    // Add the fractional part (always 2 digits)
    result[index++] = L'0' + (fractionalPart / 10);  // Tens digit of fractional part
    result[index++] = L'0' + (fractionalPart % 10);  // Ones digit of fractional part

    // Null-terminate the string
    result[index] = L'\0';

    if (fpsOrTps == FPS) {
        wcscpy(game->fpsString, result);
    } else {
        wcscpy(game->tpsString, result);
    }
}

int not_yet_to_tick(GameObject *game) {
    // Check if it's time to tick the game
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double elapsedTime = (double)(currentTime.QuadPart - game->tpsLastTime.QuadPart) / game->tpsFrequency.QuadPart;

    if (elapsedTime < TARGET_TICK_TIME) {
        return 1;
    } else {
        return 0;
    }
}


