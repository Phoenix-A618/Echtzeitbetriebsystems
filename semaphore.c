#include "semaphore.h"
#include <stdio.h>

// Globale Semaphore-Variable
SEM_ID lager_sem = NULL;

// Erstellt eine bin�re Semaphore f�r den Schutz der Lagerdaten
void initSemaphore(void) {
    lager_sem = semBCreate(SEM_Q_FIFO, SEM_FULL);
    if (!lager_sem) {
        printf("semBCreate fehlgeschlagen!\n");
    }
}
