#ifndef POSITIONBEST_H
#define POSITIONBEST_H

#include "Busdata.h"

// Struktur zur Speicherung der Positionsdaten
typedef struct {
    int x; // 1-10
    int y; // 1-5
    int z; // 0: Mitte, 1: im Fach, 2: I/O
} Position;

// Bestimmt die aktuelle Position anhand der Sensordaten
void positionBestimmen(const sbusdata* sdata, Position* pos);

#endif
