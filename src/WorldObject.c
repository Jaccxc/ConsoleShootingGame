#include "WorldObject.h"
#include <stdio.h>
#include <stdlib.h>

void load_seed_world(WorldObject *world) {
    FILE *file = fopen(SEED_MAP_FILE_PATH, "r");
    if (!file) {
        perror("Failed to open seed_map.txt");
        exit(1);
    }

    // Read the seed map data from the file
    for (int i = 0; i < SEED_WORLD_HEIGHT; i++) {
        for (int j = 0; j < SEED_WORLD_WIDTH; j++) {
            char c = fgetc(file);
            while(c == EOF || c == '\n') {
                c = fgetc(file);
            }
            world->seedWorldMap[i][j] = c;
        }
    }

    world->isSeedWorldLoaded = 1;
    fclose(file);
}

void fill_ground_chunk(TileObject world[WORLD_HEIGHT][WORLD_WIDTH], int x, int y, FillType fillType) {
    for(int i = 0; i < CHUNK_HEIGHT; i++) {
        for(int j = 0; j < CHUNK_WIDTH; j++) {
            if (fillType == FILL_BLOCK) {
                world[SKY_Y_BIAS + y*CHUNK_HEIGHT + i][x*CHUNK_WIDTH + j].type = GROUND;
            } 
            // Place ramps on the edges of the chunk, steps every 5 units
            else if (fillType == FILL_RAMP_UP && CHUNK_HEIGHT - 1 - i == j / 5 && j % 5 == 0) {
                world[SKY_Y_BIAS + y*CHUNK_HEIGHT + i][x*CHUNK_WIDTH + j].type = RAMP_UP;
            }
            // Place ground under the ramps
            else if (fillType == FILL_RAMP_UP && j / 5 >= CHUNK_HEIGHT - 1 - i) { 
                world[SKY_Y_BIAS + y*CHUNK_HEIGHT + i][x*CHUNK_WIDTH + j].type = GROUND;
            }
            else if (fillType == FILL_RAMP_DOWN && i == j / 5 && j % 5 == 4) {
                world[SKY_Y_BIAS + y*CHUNK_HEIGHT + i][x*CHUNK_WIDTH + j].type = RAMP_DOWN;
            }
            else if (fillType == FILL_RAMP_DOWN && j / 5 <= i) {
                world[SKY_Y_BIAS + y*CHUNK_HEIGHT + i][x*CHUNK_WIDTH + j].type = GROUND;
            }
        }    
    }

}

void generate_world(WorldObject *world) {
    if (!world->isSeedWorldLoaded) {
        printf("ERROR - Seed world not loaded!\n");
        exit(1);
    }
    
    // Fill the world with air
    for (int i = 0; i < WORLD_HEIGHT; i++) {
        for (int j = 0; j < WORLD_WIDTH; j++) {
            world->worldMap[i][j].type = AIR;
        }
    }

    // Fill the world with seed map in chunks
    for(int i = 0; i < SEED_WORLD_HEIGHT; i++) {
        for(int j = 0; j < SEED_WORLD_WIDTH; j++) {
            if(world->seedWorldMap[i][j] == 'G') {
                // printf("Filling chunk at %d, %d\n", j, i);
                fill_ground_chunk(world->worldMap, j, i, FILL_BLOCK);
            }
            else if(world->seedWorldMap[i][j] == 'U') {
                fill_ground_chunk(world->worldMap, j, i, FILL_RAMP_UP);
            }
            else if(world->seedWorldMap[i][j] == 'D') {
                fill_ground_chunk(world->worldMap, j, i, FILL_RAMP_DOWN);
            }

        }
    }
    // system("pause");
}

int getGroundLevel(WorldObject *world, int x) {
    int groundLevel = 0;
    while(world->worldMap[groundLevel][x].type != GROUND) {
        groundLevel++;
        if (groundLevel >= WORLD_HEIGHT) {
            return 99999;
        }
    }
    return groundLevel;
}