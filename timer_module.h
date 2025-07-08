#ifndef TIMER_MODULE_H
#define TIMER_MODULE_H

#include "vxWorks.h"
#include "time.h"

// Initialisiert und startet einen Timer mit Callback-Funktion
void setup_timer(timer_t *timerID, void (*func)(timer_t,int), int interval_ms);

#endif
