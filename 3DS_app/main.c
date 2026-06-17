#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Allocate 128KB buffer for the 3DS sockets service
#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x20000

static u32 *soc_buffer = NULL;

void initNetwork() {
    // Allocate memory alignment for network operations
    soc_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
    if(soc_buffer == NULL) {
        printf("Failed to allocate SOC buffer memory.\n");
        return;
    }

    // Initialize the 3DS SOC service
    Result ret = socInit(soc_buffer, SOC_BUFFERSIZE);
    if(R_FAILED(ret)) {
        printf("socInit failed: 0x%08X\n", (unsigned int)ret);
        free(soc_buffer);
        soc_buffer = NULL;
    } else {
        printf("3DS Network System: ONLINE\n");
    }
}

void exitNetwork() {
    if(soc_buffer != NULL) {
        socExit();
        free(soc_buffer);
        soc_buffer = NULL;
    }
}

int main(int argc, char **argv) {
    gfxInitDefault();
    
    // Direct standard console output to the TOP screen for system logs
    consoleInit(GFX_TOP, NULL);
    
    printf("uShop Functional Boot Target...\n");
    printf("Press START to exit.\n\n");

    // Start the network service
    initNetwork();

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break;

        // PLACEHOLDER: This is where we will map a button press
        // (like pressing 'A') to trigger a message to the Pi server
        if (kDown & KEY_A) {
            printf("A pressed: Sending ping request...\n");
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    // Clean up network allocation safely before quitting
    exitNetwork();
    gfxExit();
    return 0;
}
