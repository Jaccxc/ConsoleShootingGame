#include <math.h>
#include <stdlib.h>

#include "WeaponSystem.h"
#include "GameObject.h"

#define PI 3.14159265

void weapon_update(GameObject *game) {
    if (game->weaponFired) {
        game->weaponFired = 0;
        fire(game, game->weapon[game->weaponSelected], game->player.x, game->player.y, game->player.facingDirection, game->weaponUpwards, PLAYER);
    }
    game->weapon[game->weaponSelected].fireCooldownCounter = max(0, game->weapon[game->weaponSelected].fireCooldownCounter - 1);
}

void bullet_update(GameObject *game) {
    // Iterate over all active bullets in the game
    for (int i = 0; i < game->bulletCount; i++) {
        game->bullet[i].x += game->bullet[i].velocityX;
        game->bullet[i].y += game->bullet[i].velocityY;

        // Check if the bullet goes out of the screen bounds and should be removed
        if (game->bullet[i].x < 0 || game->bullet[i].x > WORLD_WIDTH ||
            game->bullet[i].y < 0 || game->bullet[i].y > WORLD_HEIGHT) {
            // Remove the bullet by shifting all subsequent bullets up in the array
            for (int j = i; j < game->bulletCount - 1; j++) {
                game->bullet[j] = game->bullet[j + 1];
            }
            game->bulletCount--; // Decrease bullet count
            i--; // Adjust loop index to account for removed bullet
        }

        // Collision detection
        
    }
}


void fire(GameObject *game, Weapon weapon, int x, int y, int facingDirection, int weaponUpwards, BulletSource source) {
    // Check if cooldown is finished
    if (weapon.fireCooldownCounter > 0) {
        return;
    }

    game->weapon[game->weaponSelected].fireCooldownCounter = weapon.fireCooldownTime;
    
    float spread = weapon.spread; // Range between 0 to 10 (or a chosen maximum)
    float baseSpeed = 5.0; // Basic speed for all bullets
    float randomSpeedRange = 0.1; // Small range for speed variation (e.g., 0.1 means ±10%)

    // Create new bullets
    for (int i = 0; i < weapon.bulletCount; i++) {
        Bullet bullet;
        bullet.x = x;
        bullet.y = y;

        // Calculate the angle deviation for each bullet
        float angleOffset = ((float)rand() / RAND_MAX) * spread - (spread / 2.0); // Random value between -spread/2 and +spread/2

        // Convert angleOffset to radians for trigonometric functions
        float angleInRadians = angleOffset * (PI / 180.0);

        // Base angle for velocity calculation
        float baseAngle = atan2(weaponUpwards, facingDirection);

        // Calculate final bullet velocity with spread applied
        bullet.velocityX = cos(baseAngle + angleInRadians);
        bullet.velocityY = sin(baseAngle + angleInRadians);

        // Apply basic speed, weapon type multiplier, and random speed multiplier
        float randomSpeedMultiplier = 1.0 + ((float)rand() / RAND_MAX) * randomSpeedRange - (randomSpeedRange / 2.0); // Between 1 - range/2 and 1 + range/2
        float finalSpeed = baseSpeed * weapon.bulletSpeedMultiplier * randomSpeedMultiplier;

        bullet.velocityX *= finalSpeed;
        bullet.velocityY *= finalSpeed;

        bullet.damage = weapon.damage;
        bullet.source = source;
        game->bullet[game->bulletCount] = bullet;
        game->bulletCount++;
    }
}