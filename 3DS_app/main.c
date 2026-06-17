#include <3ds.h>
#include <stdio.h>

// Helper function to fill a rectangle on a 3D video buffer
void drawRect(u8* fb, int x, int y, int w, int h, u8 r, u8 g, u8 b, int screenWidth) {
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            // 3DS screens are rotated physically; this math aligns it to standard (x, y)
            int index = 3 * (i * screenWidth + (screenWidth - 1 - j));
            fb[index]     = b; // Blue
            fb[index + 1] = g; // Green
            fb[index + 2] = r; // Red
        }
    }
}

int main(int argc, char **argv) {
    gfxInitDefault();

    // Main application loop
    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; // Press START to exit back to Homebrew Menu

        // Get pointers to the raw screen drawing memory buffers
        u8* topFB    = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
        u8* bottomFB = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);

        // ----------------------------------------------------
        // 1. DRAW TOP SCREEN (400x240)
        // ----------------------------------------------------
        // Soft Orange/Beige Canvas Background
        drawRect(topFB, 0, 0, 400, 240, 255, 242, 229, 240);
        
        // Pure White Status Bar (Top 20 pixels)
        drawRect(topFB, 0, 0, 400, 20, 255, 255, 255, 240);
        
        // Light Gray Footer Bar (Bottom 40 pixels)
        drawRect(topFB, 0, 200, 400, 40, 235, 235, 235, 240);

        // ----------------------------------------------------
        // 2. DRAW BOTTOM SCREEN (320x240)
        // ----------------------------------------------------
        // Soft Orange/Beige Canvas Background
        drawRect(bottomFB, 0, 0, 320, 240, 255, 242, 229, 240);
        
        // Light Gray Static Bottom Menu Bar (Bottom 50 pixels)
        drawRect(bottomFB, 0, 190, 320, 50, 235, 235, 235, 240);

        // PLACEHOLDER: Draw an eShop Item Grid Button (60x60 square)
        // White button background
        drawRect(bottomFB, 20, 30, 60, 60, 255, 255, 255, 240);

        // Flush and swap video frames
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
