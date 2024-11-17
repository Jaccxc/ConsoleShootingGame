#ifndef _UTILS_H
#define _UTILS_H

typedef struct GameObject GameObject;

void fps_tps_format(GameObject *game, int fpsOrTps);
void fps_update(GameObject *game);
void tps_update(GameObject *game);

#endif
