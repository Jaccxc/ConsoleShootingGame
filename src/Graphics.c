#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "Graphics.h"
#include "GameObject.h"
#include "WorldObject.h"

// Set the console size to 180x45
void set_console_size(GameObject *game) {
    system("mode con cols=180 lines=45");
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(game->hConsoleOutput, &cursorInfo);
    cursorInfo.bVisible = FALSE;  // Hide the cursor
    SetConsoleCursorInfo(game->hConsoleOutput, &cursorInfo);
}

// Function to write the screen buffer to the console
void write_to_console(GameObject *game) {
    COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
    COORD dwBufferCoord = {0, 0};
    SMALL_RECT lpWriteRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};

    if (!WriteConsoleOutputW(game->hConsoleOutput, game->screenBuffer, dwBufferSize, dwBufferCoord, &lpWriteRegion)) {
        printf("Error writing to console output\n");
    }
}

// Clear the screen buffer by setting all to blank spaces
void clear_screen_buffer(GameObject *game) {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        game->screenBuffer[i].Char.UnicodeChar = L' ';  // Blank space
        game->screenBuffer[i].Attributes = BG_COLOR;  // Default background color
    }
}

// Function to draw a character at a specific position in the buffer
void draw_char_at(GameObject *game, int x, int y, WCHAR ch, int fg, int bg) {
    int index = y * SCREEN_WIDTH + x;
    if (index >= 0 && index < BUFFER_SIZE) {
        game->screenBuffer[index].Char.UnicodeChar = ch;  // Character to display
        game->screenBuffer[index].Attributes = (fg | (bg << 4));  // Foreground and background color
    }
}

// Function to draw a horizontal line
void draw_horizontal_line(GameObject *game, int y, int startX, int endX, WCHAR ch, int fg, int bg) {
    for (int x = startX; x < endX; x++) {
        draw_char_at(game, x, y, ch, fg, bg);
    }
}

// Function to draw a vertical line
void draw_vertical_line(GameObject *game, int x, int startY, int endY, WCHAR ch, int fg, int bg) {
    for (int y = startY; y < endY; y++) {
        draw_char_at(game, x, y, ch, fg, bg);
    }
}

void draw_text(GameObject *game, int x, int endX, int y, WCHAR *text, int fg, int bg, TextAlignment alignment) {
    int maxLineLength = endX - x + 1; // Maximum characters allowed in the line
    int textLength = wcslen(text);

    // Ensure text doesn't exceed the maximum allowed length
    if (textLength > maxLineLength) {
        textLength = maxLineLength;
    }

    // Determine the starting x position based on alignment
    int startX = x;
    switch (alignment) {
        case LEFT:
            startX = x;
            break;
        case CENTER:
            startX = x + (maxLineLength - textLength) / 2;
            break;
        case RIGHT:
            startX = endX - textLength + 1;
            break;
    }

    // Draw the text character by character
    for (int i = 0; i < textLength; i++) {
        if (startX + i >= x && startX + i <= endX) {
            draw_char_at(game, startX + i, y, text[i], fg, bg);
        }
    }
}


void draw_rect(GameObject *game, int x, int y, int width, int height, int fg, int bg, BorderThickness thickness) {
    // Determine characters to use based on thickness
    wchar_t horizontalChar, verticalChar, topLeftCorner, topRightCorner, bottomLeftCorner, bottomRightCorner;
    
    switch (thickness) {
        case BORDER_LIGHT:
            horizontalChar = L'─'; 
            verticalChar = L'│';   
            topLeftCorner = L'┌'; 
            topRightCorner = L'┐'; 
            bottomLeftCorner = L'└'; 
            bottomRightCorner = L'┘'; 
            break;
        case BORDER_MEDIUM:
            horizontalChar = L'═'; 
            verticalChar = L'║';  
            topLeftCorner = L'╔';  
            topRightCorner = L'╗';
            bottomLeftCorner = L'╚'; 
            bottomRightCorner = L'╝';
            break;
        case BORDER_HEAVY:
            horizontalChar = L'█'; 
            verticalChar = L'█';  
            topLeftCorner = L'█';  
            topRightCorner = L'█';
            bottomLeftCorner = L'█'; 
            bottomRightCorner = L'█';
            break;
        default:
            horizontalChar = L'═'; 
            verticalChar = L'║';  
            topLeftCorner = L'╔';  
            topRightCorner = L'╗';
            bottomLeftCorner = L'╚'; 
            bottomRightCorner = L'╝';
            break;
    }

    // Draw the top border
    draw_horizontal_line(game, y, x, x+width, horizontalChar, fg, bg);

    // Draw the bottom border
    draw_horizontal_line(game, y + height - 1, x, x+width, horizontalChar, fg, bg);

    // Draw the left and right borders
    draw_vertical_line(game, x, y, y + height, verticalChar, fg, bg);
    draw_vertical_line(game, x + width - 1, y, y + height, verticalChar, fg, bg);

    // Draw the corners
    draw_char_at(game, x, y, topLeftCorner, fg, bg);                       // Top-left corner
    draw_char_at(game, x + width - 1, y, topRightCorner, fg, bg);           // Top-right corner
    draw_char_at(game, x, y + height - 1, bottomLeftCorner, fg, bg);        // Bottom-left corner
    draw_char_at(game, x + width - 1, y + height - 1, bottomRightCorner, fg, bg);  // Bottom-right corner
}


// Function to draw the UI (bottom panel, weapon area, etc.)
void draw_ui(GameObject *game) {
    // Draw some UI text
    draw_char_at(game, 2, 34, L'S', FOREGROUND_RED, BG_COLOR); // Select weapon: "S"
    draw_char_at(game, 2, 35, L'W', FOREGROUND_RED, BG_COLOR); // Weapon: Sword
    draw_char_at(game, 2, 36, L'H', FOREGROUND_YELLOW, BG_COLOR); // Health: 100%

    // Draw the info panel (bottom 12 lines)
    for (int y = 34; y < SCREEN_HEIGHT; y++) {
        draw_horizontal_line(game, y, 0, SCREEN_WIDTH, L' ', FOREGROUND_WHITE, BG_COLOR); // Blank info section
    }

    // Draw the frame
    draw_rect(game, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FOREGROUND_WHITE, BG_COLOR, BORDER_MEDIUM);
    
    // Draw the separation line between top and bottom (34 line)
    draw_horizontal_line(game, 33, 0, SCREEN_WIDTH, L'═', FOREGROUND_WHITE, BG_COLOR);

    // Draw the corners
    draw_char_at(game, 0, 33, L'╠', FOREGROUND_WHITE, BG_COLOR);
    draw_char_at(game, SCREEN_WIDTH-1, 33, L'╣', FOREGROUND_WHITE, BG_COLOR);

    // Draw Weapon Icon
    draw_rect(game, 3, 34, 25, 10, FOREGROUND_WHITE, BG_COLOR, BORDER_LIGHT);
    draw_rect(game, 3+25+2, 34, 25, 10, FOREGROUND_WHITE, BG_COLOR, BORDER_LIGHT);
    draw_rect(game, 3+25+2+25+2, 34, 25, 10, FOREGROUND_WHITE, BG_COLOR, BORDER_LIGHT);
    switch (game->weaponSelected) {
        case 0:
            draw_rect(game, 3, 34, 25, 10, FOREGROUND_WHITE, BG_COLOR, BORDER_HEAVY);
            break;
        case 1:
            draw_rect(game, 3+25+2, 34, 25, 10, FOREGROUND_WHITE, BG_COLOR, BORDER_HEAVY);
            break;
        case 2:
            draw_rect(game, 3+25+2+25+2, 34, 25, 10, FOREGROUND_WHITE, BG_COLOR, BORDER_HEAVY);
            break;
    }


    draw_text(game, 3, 3+25, 34+5-1, L"Weapon #1", FOREGROUND_WHITE, BG_COLOR, CENTER);
    draw_text(game, 3, 3+25, 34+5, L"Pistol", FOREGROUND_WHITE, BG_COLOR, CENTER);
    draw_text(game, 3+25+2, 3+25+2+25, 34+5-1, L"Weapon #2", FOREGROUND_WHITE, BG_COLOR, CENTER);
    draw_text(game, 3+25+2, 3+25+2+25, 34+5, L"Shotgun", FOREGROUND_WHITE, BG_COLOR, CENTER);
    draw_text(game, 3+25+2+25+2, 3+25+2+25+2+25, 34+5-1, L"Weapon #3", FOREGROUND_WHITE, BG_COLOR, CENTER);
    draw_text(game, 3+25+2+25+2, 3+25+2+25+2+25, 34+5, L"Minigun", FOREGROUND_WHITE, BG_COLOR, CENTER);

    // FPS Counter
    draw_text(game, SCREEN_WIDTH-15, SCREEN_WIDTH-3, 1, game->fpsString, FOREGROUND_WHITE, BG_COLOR, RIGHT);

    // TPS Counter
    draw_text(game, SCREEN_WIDTH-15, SCREEN_WIDTH-3, 2, game->tpsString, FOREGROUND_WHITE, BG_COLOR, RIGHT);

    if (game->isHintBoxVisible) {
        for(int i = 0; i < 12; i++) {
            for(int j = 0; j < 48; j++) {
                draw_char_at(game, SCREEN_WIDTH/2-24+j, SCREEN_HEIGHT/2-6+i, L' ', FOREGROUND_WHITE, BG_COLOR);
            }
        }
        draw_rect(game, SCREEN_WIDTH/2-24, SCREEN_HEIGHT/2-6, 48, 12, FOREGROUND_WHITE, BG_COLOR, BORDER_MEDIUM);
        draw_text(game, SCREEN_WIDTH/2-24, SCREEN_WIDTH/2+24, SCREEN_HEIGHT/2-6+4, game->hintBoxText, FOREGROUND_WHITE, BG_COLOR, CENTER);
        draw_text(game, SCREEN_WIDTH/2-24, SCREEN_WIDTH/2+24, SCREEN_HEIGHT/2-6+7, L" Restart ", BACKGROUND_CYAN , FOREGROUND_BLACK, CENTER);
    }
}

// Function to draw the game world (e.g., player, enemies, etc.)
void draw_game_world(GameObject *game) {
    // Calculate the camera's viewport position (the area of the world that will be visible on screen)
    int cameraX = game->player.x - GAME_WIDTH / 3;  // Center the camera on the player

    // Prevent camera from going out of world boundaries
    if (cameraX < 0) cameraX = 0;
    if (cameraX + GAME_WIDTH > WORLD_WIDTH) cameraX = WORLD_WIDTH - SCREEN_WIDTH;

    // Loop over the screen dimensions and render the visible portion of the world map
    for (int i = 0; i < GAME_HEIGHT; i++) { 
        for (int j = 0; j < GAME_WIDTH; j++) {
            int worldX = cameraX + j;  // World position in x (adjusted by camera offset)

            // Ensure world coordinates are within bounds
            if (worldX >= 0 && worldX < WORLD_WIDTH) {
                switch (game->world.worldMap[i][worldX].type) {
                    case AIR:
                        draw_char_at(game, j, i+1, L' ', FOREGROUND_WHITE, BG_COLOR);
                        break;
                    case GROUND:
                        draw_char_at(game, j, i+1, L'█', FOREGROUND_WHITE, BG_COLOR);
                        break;
                    case RAMP_UP:
                        draw_char_at(game, j, i+1, L'╱', FOREGROUND_WHITE, BG_COLOR);
                        break;
                    case RAMP_DOWN:
                        draw_char_at(game, j, i+1, L'╲', FOREGROUND_WHITE, BG_COLOR);
                        break;
                    default:
                        draw_char_at(game, j, i+1, L'█', FOREGROUND_WHITE, BG_COLOR);
                        break;
                }
            }
        }
    }

    // Draw the player at the screen coordinates (center the player)
    draw_char_at(game, (int)(game->player.x - cameraX), (int)(game->player.y), L'█', FOREGROUND_RED, BG_COLOR);
    draw_char_at(game, (int)(game->player.x - cameraX), (int)(game->player.y+1), L'█', FOREGROUND_RED, BG_COLOR);

    // Draw the bullets
    for (int i = 0; i < game->bulletCount; i++) {
        // Skip bullets that are out of bounds
        if (game->bullet[i].x - cameraX > SCREEN_WIDTH || game->bullet[i].x - cameraX < 0){
            continue;
        }
        draw_char_at(game, (int)(game->bullet[i].x - cameraX), (int)(game->bullet[i].y), L'⏺', FOREGROUND_YELLOW, BG_COLOR);
    }
}


