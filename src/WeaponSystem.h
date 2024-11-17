#ifndef _WEAPON_H
#define _WEAPON_H

typedef struct GameObject GameObject;

typedef struct {
    float x, y;           // Current position
    float velocityX, velocityY;  // Velocity in each direction
} Bullet;

typedef struct {
    int damage;                 // Damage per bullet
    float fireRate;             // Fire rate in shots per second
    float spread;               // Bullet spread (larger values mean more spread)
    int bulletCount;            // Number of bullets fired per shot (relevant for shotgun, etc.)
    int (*fire)(int x, int y);  // Function pointer for fire behavior
    void (*cooldown)();
    int fireCooldownTime;       // Time in ticks left until next shot
} Weapon;


void weapon_update(GameObject*);
void bullet_update(GameObject*);

#endif