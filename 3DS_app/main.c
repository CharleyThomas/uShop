#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// Application States
typedef enum {
    STATE_IP_ENTRY,
    STATE_MAIN_GRID
} AppState;

int main(int argc, char **argv) {
    gfxInitDefault();
    
    // Initialize standard console text print engines for both screens
    PrintConsole topConsole, bottomConsole;
    consoleInit(GFX_TOP, &topConsole);
    consoleInit(GFX_BOTTOM, &bottomConsole);

    AppState currentState = STATE_IP_ENTRY;

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        
        if (kDown & KEY_START) break; // Exit application safely

        // Read touch screen inputs
        touchPosition touch;
        hidTouchRead(&touch);

        // ----------------------------------------------------
        // LOGIC & RENDERING ENGINE BY STATE
        // ----------------------------------------------------
        if (currentState == STATE_IP_ENTRY) {
            // --- TOP SCREEN: IP ENTRY MODE ---
            consoleSelect(&topConsole);
            consoleClear();
            printf("\n\n\n\n\n");
            printf("          [ uShop Logo Placeholder ]\n");
            printf("               (logo.png target)\n");

            // --- BOTTOM SCREEN: IP ENTRY MODE ---
            consoleSelect(&bottomConsole);
            consoleClear();
            printf("\n\n\n");
            printf("  Enter server IP address:\n");
            printf("  _____________________________________\n");
            printf("  |                                   |\n");
            printf("  |___________________________________|\n");
            printf("\n\n\n\n\n");
            printf("            <skip for testing>\n");

            // Touch collision detection for "<skip for testing>" zone
            if (kDown & KEY_TOUCH) {
                // Bounds matching roughly where the text renders on screen
                if (touch.px > 60 && touch.px < 260 && touch.py > 140 && touch.py < 180) {
                    currentState = STATE_MAIN_GRID; // Swap menus!
                }
            }
        } 
        else if (currentState == STATE_MAIN_GRID) {
            // --- TOP SCREEN: SHOP DISPLAY ---
            consoleSelect(&topConsole);
            consoleClear();
            printf("\n\n\n\n\n");
            printf("          [ uShop Logo Placeholder ]\n");

            // --- BOTTOM SCREEN: PRODUCT GRID ---
            consoleSelect(&bottomConsole);
            consoleClear();
            printf("\n");
            // Render a clean functional grid array using text layouts
            printf("  [ App 1 ]   [ App 2 ]   [ App 3 ]\n\n\n");
            printf("  [ App 4 ]   [ App 5 ]   [ App 6 ]\n\n\n");
            printf("  [ App 7 ]   [ App 8 ]   [ App 9 ]\n");
            printf("\n\n\n Press B to return to setup");

            if (kDown & KEY_B) {
                currentState = STATE_IP_ENTRY;
            }
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
