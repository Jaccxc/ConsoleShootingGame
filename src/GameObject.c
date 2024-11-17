#include <wchar.h>
#include <stdio.h>

#include "GameObject.h"
#include "Graphics.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "WorldObject.h"
#include "WeaponSystem.h"
#include "Utils.h"


void game_init(GameObject *game)
{
    // Initialize game variables
    game->gravity = 0.1;
    game->weaponSelected = 0;
    game->weaponFired = 0;
    game->weaponUpwards = 0;

    // Initialize World
    load_seed_world(&game->world);
    generate_world(&game->world);

    // Initialize Player
    game->player.x = 5;
    game->player.y = 5;
    game->player.verticalVelocity = 0;
    game->player.horizontalVelocity = 0;
    game->player.jumpHeight = 10;
    game->player.speedLimit = 1.0;
    game->player.isAirborne = 1;
    game->player.facingDirection = 1;

    // Initialize Rendering
    game->isHintBoxVisible = 0;
    wcscpy(game->hintBoxText, L"You have died!");
    game->hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    set_console_size(game);

    // Initialize high-resolution timer
    QueryPerformanceFrequency(&game->fpsFrequency);  // Get the frequency of the high-resolution counter
    QueryPerformanceCounter(&game->fpsLastTime);     // Get the initial timestamp

    QueryPerformanceFrequency(&game->tpsFrequency);  // Get the frequency of the high-resolution counter
    QueryPerformanceCounter(&game->tpsLastTime);     // Get the initial timestamp
}

void game_reset(GameObject *game) {
    game->player.x = 5;
    game->player.y = 5;
    game->player.verticalVelocity = 0;
    game->player.horizontalVelocity = 0;
    game->player.jumpHeight = 10;
    game->player.isAirborne = 1;

    // Initialize Rendering
    game->isHintBoxVisible = 0;
    wcscpy(game->hintBoxText, L"You have died!");
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
    
    // Check if it's time to tick the game
    if(not_yet_to_tick(game))
        return;
    
    tps_update(game);

    player_update(game);

    weapon_update(game);

    bullet_update(game);

    enemy_update(game);
}

void game_render(GameObject *game)
{
    // Render the game object
    clear_screen_buffer(game);
    draw_game_world(game);
    draw_ui(game);
    write_to_console(game);
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



