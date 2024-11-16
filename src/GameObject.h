#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include <wchar.h>
#include <stdio.h>

#include <windows.h>

#include "WorldObject.h"
#include "PlayerController.h"

#define TARGET_TPS 60
#define TARGET_TICK_TIME (1.0 / TARGET_TPS)

#define SCREEN_WIDTH 180
#define SCREEN_HEIGHT 45
#define SCREEN_BORDER_WIDTH 1
#define GAME_WIDTH SCREEN_WIDTH // 180
#define GAME_HEIGHT 34 - 2 * SCREEN_BORDER_WIDTH // 32
#define BUFFER_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

typedef enum {
    FPS,
    TPS
} FpsOrTps;

typedef struct GameObject
{
    // Game information
    int level;
    float gravity;
    WorldObject world;
    PlayerObject player;


    // Game internal states 
    float fps;
    float tps;
    WCHAR fpsString[30];
    WCHAR tpsString[30];
    LARGE_INTEGER fpsLastTime; 
    LARGE_INTEGER fpsFrequency;
    LARGE_INTEGER tpsLastTime; 
    LARGE_INTEGER tpsFrequency;

    // Rendering 
    HANDLE hConsoleOutput;
    CHAR_INFO screenBuffer[BUFFER_SIZE+5];
} GameObject;

void game_init(GameObject *game);
void game_handle_input(GameObject *game);
void game_update(GameObject *game);
void game_render(GameObject *game);
void fps_tps_format(GameObject *game, int fpsOrTps);
void fps_update(GameObject *game);
void tps_update(GameObject *game);
int not_yet_to_tick(GameObject *game);

#endif