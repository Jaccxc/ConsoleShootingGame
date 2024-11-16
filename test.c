#include <windows.h>
#include <wchar.h>  // For wide characters
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

void print_unicode_array(HANDLE hConsole, const WCHAR *unicodeArray, int width, int height, int fg, int bg) {
    // Create a CHAR_INFO buffer to store the characters and their attributes
    CHAR_INFO *buffer = (CHAR_INFO *)malloc(width * height * sizeof(CHAR_INFO));

    // Fill the CHAR_INFO buffer with Unicode characters and attributes
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            buffer[index].Char.UnicodeChar = unicodeArray[index];  // Set the Unicode character
            buffer[index].Attributes = (WORD)((fg & 0x0F) | ((bg & 0x0F) << 4));  // Set the colors
        }
    }

    // Set up the region to write to
    COORD bufferSize = { (SHORT)width, (SHORT)height };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };

    // Write the buffer to the console
    WriteConsoleOutputW(hConsole, buffer, bufferSize, bufferCoord, &writeRegion);

    // Clean up
    free(buffer);
}

int main() {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Example Unicode characters to display
    WCHAR unicodeArray[] = {
        L'╔', L'═', L'═', L'═', L'╗',
        L'║', L' ', L' ', L' ', L'║',
        L'║', L' ', L'█', L' ', L'║',
        L'║', L' ', L' ', L' ', L'║',
        L'╚', L'═', L'═', L'═', L'╝'
    };

    // Array dimensions (width x height)
    int width = 5;
    int height = 5;

    // Print the Unicode array to the console with white text on a black background
    print_unicode_array(hConsole, unicodeArray, width, height, FOREGROUND_GREEN, 0);

    // Wait for user input before closing
    system("pause");

    return 0;
}

