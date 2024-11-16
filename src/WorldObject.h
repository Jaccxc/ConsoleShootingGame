#ifndef _WORLDOBJECT_H
#define _WORLDOBJECT_H

#define SEED_MAP_FILE_PATH "seed_map.txt"

#define SEED_WORLD_WIDTH 30
#define SEED_WORLD_HEIGHT 4
#define WORLD_WIDTH 900
#define WORLD_HEIGHT 32
#define SKY_Y_BIAS 8
#define CHUNK_WIDTH WORLD_WIDTH / SEED_WORLD_WIDTH // 30
#define CHUNK_HEIGHT (WORLD_HEIGHT - SKY_Y_BIAS) / SEED_WORLD_HEIGHT // 6

typedef struct GameObject GameObject;

typedef enum {
    GROUND,
    RAMP_UP,
    RAMP_DOWN, 
    HEALING,
    TRAP,  
    AIR,
    STAR
} TileType;

typedef enum {
    FILL_BLOCK,
    FILL_RAMP_UP,
    FILL_RAMP_DOWN
} FillType;

typedef struct TileObject
{
    TileType type;
} TileObject;

typedef struct WorldObject
{
    int isSeedWorldLoaded;
    char seedWorldMap[SEED_WORLD_HEIGHT][SEED_WORLD_WIDTH];
    TileObject worldMap[WORLD_HEIGHT][WORLD_WIDTH];
} WorldObject;

void load_seed_world(WorldObject*);
void fill_ground_chunk(TileObject world[WORLD_HEIGHT][WORLD_WIDTH], int x, int y, FillType fillType);
void generate_world(WorldObject*);
int getGroundLevel(WorldObject*, int);

#endif
