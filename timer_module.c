#include "timer_module.h"
#include <stdio.h>

// Erstellt und startet einen Timer mit der angegebenen Callback-Funktion und Intervall
void setup_timer(timer_t *timerID, void (*func)(timer_t,int), int interval_ms) {
    struct itimerspec ts;
    int arg = 0;
    if (timer_create(CLOCK_REALTIME, NULL, timerID) == ERROR) {
        printf("timer_create FEHLGESCHLAGEN\n"); return;
    }
    if (timer_connect(*timerID, (VOIDFUNCPTR)func, arg) == ERROR) {
        printf("timer_connect FEHLGESCHLAGEN\n"); return;
    }
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = interval_ms * 1000000;
    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = interval_ms * 1000000;
    if (timer_settime(*timerID, TIMER_RELTIME, &ts, NULL) == ERROR) {
        printf("timer_settime FEHLGESCHLAGEN\n"); return;
    }
}
