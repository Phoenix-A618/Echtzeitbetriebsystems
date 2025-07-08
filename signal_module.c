#include "signal_module.h"
#include <stdio.h>
#include "vxWorks.h"
#include "sigLib.h"

// Signal-Handler-Funktion für SIGUSR1
static void catchSIGUSR1(int sig) {
    printf("[Positionsbestimmung] Signal Handler: SIGUSR1 empfangen!\n");
}

// Registriert den Signal Handler für SIGUSR1
void setupSignalHandler(void) {
    struct sigaction sa;
    sa.sa_handler = catchSIGUSR1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
        printf("Signal Handler konnte nicht installiert werden\n");
    else
        printf("SIGUSR1 Handler installiert (Positionsbestimmung).\n");
}
