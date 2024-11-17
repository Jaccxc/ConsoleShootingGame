#ifndef _WEAPON_H
#define _WEAPON_H

typedef struct GameObject GameObject;

typedef enum BulletSource {
    PLAYER,
    ENEMY
} BulletSource;

typedef struct {
    float x, y;           // Current position
    float velocityX, velocityY;  // Velocity in each direction
    int damage;           // Damage dealt by the bullet
    BulletSource source;  // Source of the bullet (player or enemy)
} Bullet;

typedef struct {
    int damage;                 // Damage per bullet
    float fireRate;             // Fire rate in shots per second
    float spread;               // Bullet spread (larger values mean more spread)
    float bulletSpeedMultiplier;  // Multiplier for bullet speed
    int bulletCount;            // Number of bullets fired per shot (relevant for shotgun, etc.)
    int fireCooldownTime;       // Time in ticks left until next shot
    int fireCooldownCounter; // Time counter for fire cooldown
} Weapon;

void weapon_update(GameObject*);
void bullet_update(GameObject*);

void fire(GameObject*, Weapon, int, int, int, int, BulletSource);

#endif