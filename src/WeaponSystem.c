#include "WeaponSystem.h"
#include "GameObject.h"

void weapon_update(GameObject *game) {
    // TODO: Fire weapon
    // Fire weapon
    if (game->weaponFired) {
        game->weaponFired = 0;
        // game->weapon[game->weaponSelected].fire(game->weaponUpwards);
    }
}