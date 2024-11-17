#ifndef _PLAYERCONTROLLER_H
#define _PLAYERCONTROLLER_H

#include <windows.h>
#include <stdio.h>

// Forward declaration 
typedef struct GameObject GameObject;

typedef struct {
    float x, y;           // Player position
    float verticalVelocity; // Vertical velocity (used for jumping and gravity)
    float horizontalVelocity; // Horizontal velocity (used for movement)
    float jumpHeight;      // Height to which the player can jump
    float speedLimit;      // Maximum speed
    int isAirborne;      // Whether the player is in the air or not
    int facingDirection; // 1 for right, -1 for left 
} PlayerObject;

void process_player_input(GameObject*);
void player_update(GameObject*);

#endif
