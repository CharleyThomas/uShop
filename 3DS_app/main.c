#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IP_ENTRY,
    STATE_MAIN_GRID
} AppState;

int main(int argc, char **argv) {
    gfxInitDefault();
    
    // Initialize text consoles for both screens
    PrintConsole topConsole, bottomConsole;
    consoleInit(GFX_TOP, &topConsole);
    consoleInit(GFX_BOTTOM, &bottomConsole);

    AppState currentState = STATE_IP_ENTRY;
    
    // Buffer memory to store the typed IP address
    char serverIP[16] = "192.168.1.100"; // Default placeholder text

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) break; // Exit application safely

        touchPosition touch;
        hidTouchRead(&touch);

        if (currentState == STATE_IP_ENTRY) {
            // --- TOP SCREEN ---
            consoleSelect(&topConsole);
            consoleClear();
            printf("\n\n\n\n\n");
            printf("          [ uShop Logo Placeholder ]\n");
            printf("               (logo.png target)\n");

            // --- BOTTOM SCREEN ---
            consoleSelect(&bottomConsole);
            consoleClear();
            printf("\n\n\n");
            printf("  Enter server IP address:\n");
            printf("  _____________________________________\n");
            printf("  |                                   |\n");
            printf("    Current: %s\n", serverIP); // Display what was typed
            printf("  |___________________________________|\n");
            printf("\n\n\n\n\n");
            printf("            <skip for testing>\n");

            if (kDown & KEY_TOUCH) {
                // Click 1: Did they tap inside the box area to change the IP?
                if (touch.px > 20 && touch.px < 300 && touch.py > 40 && touch.py < 90) {
                    
                    // Initialize the native 3DS OS Software Keyboard
                    SwkbdState swkbd;
                    swkbdInit(&swkbd, SWKBD_TYPE_NUMPAD, 2, 15); // Numpad layout for IPs
                    swkbdSetValidation(&swkbd, SWKBD_ANYTHING, 0, 0);
                    swkbdSetFeatures(&swkbd, SWKBD_FIXED_WIDTH);
                    swkbdSetHintText(&swkbd, "Enter RPi IP Address");
                    
                    // Open the keyboard overlay and store the output string
                    swkbdInputText(&swkbd, serverIP, sizeof(serverIP));
                }

                // Click 2: Did they tap the "<skip for testing>" region?
                if (touch.px > 60 && touch.px < 260 && touch.py > 140 && touch.py < 180) {
                    currentState = STATE_MAIN_GRID;
                }
            }
        } 
        else if (currentState == STATE_MAIN_GRID) {
            // --- TOP SCREEN ---
            consoleSelect(&topConsole);
            consoleClear();
            printf("\n\n\n\n\n");
            printf("          [ uShop Logo Placeholder ]\n");

            // --- BOTTOM SCREEN ---
            consoleSelect(&bottomConsole);
            consoleClear();
            printf("\n");
            printf("  Target IP Configured: %s\n\n", serverIP);
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
