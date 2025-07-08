#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "vxWorks.h"
#include "semLib.h"

// Globale Semaphore für den Zugriff auf Lagerdaten
extern SEM_ID lager_sem;

// Initialisiert die Semaphore
void initSemaphore(void);

#endif
