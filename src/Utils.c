#include <windows.h>
#include <stdio.h>

#include "Utils.h"
#include "GameObject.h"

void fps_tps_format(GameObject *game, int fpsOrTps) {
    // Calculate the FPS (assuming game->fps is already updated)
    float data = (float)game->fps;
    WCHAR result[30];

    if(fpsOrTps == FPS) {
        data = game->fps;
    } else {
        data = game->tps;
    }

    // Calculate the integer part and the fractional part separately
    int integerPart = (int)data;
    int fractionalPart = (int)((data - integerPart) * 100);  // Take 2 decimal places

    int index = 0;

    // Add "FPS: " or "TPS: "
    if (fpsOrTps == FPS) {
        result[index++] = L'F';
    } else {
        result[index++] = L'T';
    }
    result[index++] = L'P';
    result[index++] = L'S';
    result[index++] = L':';
    result[index++] = L' ';

    // Add the integer part
    if (integerPart > 999) {
        result[index++] = L'0' + (integerPart / 1000);  // Thousands digit
        integerPart %= 1000;
    }
    if (integerPart > 99) {
        result[index++] = L'0' + (integerPart / 100);  // Hundreds digit
        integerPart %= 100;
    }
    if (integerPart > 9) {
        result[index++] = L'0' + (integerPart / 10);  // Tens digit
        integerPart %= 10;
    }
    result[index++] = L'0' + (integerPart % 10);  // Ones digit

    // Add the decimal point
    result[index++] = L'.';

    // Add the fractional part (always 2 digits)
    result[index++] = L'0' + (fractionalPart / 10);  // Tens digit of fractional part
    result[index++] = L'0' + (fractionalPart % 10);  // Ones digit of fractional part

    // Null-terminate the string
    result[index] = L'\0';

    if (fpsOrTps == FPS) {
        wcscpy(game->fpsString, result);
    } else {
        wcscpy(game->tpsString, result);
    }
}

void fps_update(GameObject *game) {
    // Calculate FPS
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double elapsedTime = (double)(currentTime.QuadPart - game->fpsLastTime.QuadPart) / game->fpsFrequency.QuadPart;

    if (elapsedTime > 0) 
        game->fps = 1.0f / (float)elapsedTime;

    game->fpsLastTime = currentTime;

    // Update FPS string
    fps_tps_format(game, FPS);
}

void tps_update(GameObject *game) {
    // Update tps
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);

    double elapsedTime = (double)(currentTime.QuadPart - game->tpsLastTime.QuadPart) / game->tpsFrequency.QuadPart;

    if (elapsedTime > 0) 
        game->tps = 1.0f / (float)elapsedTime;

    game->tpsLastTime = currentTime;

    // Update TPS string
    fps_tps_format(game, TPS);
}

