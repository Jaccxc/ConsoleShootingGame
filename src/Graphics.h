#ifndef _GRAPHICS_H
#define _GRAPHICS_H


#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_BLACK 0
#define BACKGROUND_WHITE (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)
#define BACKGROUND_CYAN (BACKGROUND_BLUE | BACKGROUND_GREEN)
#define BG_COLOR 0

typedef struct GameObject GameObject;

typedef enum {
    BORDER_LIGHT, 
    BORDER_MEDIUM, 
    BORDER_HEAVY
} BorderThickness;

typedef enum {
    LEFT,
    CENTER,
    RIGHT
} TextAlignment;

void set_console_size(GameObject*);
void write_to_console(GameObject*);
void clear_screen_buffer(GameObject*);
void draw_char_at(GameObject*, int, int, WCHAR , int, int);
void draw_horizontal_line(GameObject*, int, int, int, WCHAR , int, int);
void draw_vertical_line(GameObject*, int, int, int, WCHAR , int, int);
void draw_rect(GameObject*, int, int, int, int, int, int, BorderThickness);
void draw_text(GameObject*, int, int, int, WCHAR *, int, int, TextAlignment);
void draw_ui(GameObject*);
void draw_game_world(GameObject*);

#endif