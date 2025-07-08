#include "positionsbest.h"
#include <stdio.h>

// Merkt sich die letzte gültige Y-Position (wird bei Sensor-Ausfall weiterverwendet)
static int y_last = 1; // Anfangswert kann angepasst werden

void positionBestimmen(const sbusdata* sdata, Position* pos) {
    pos->x = 0;
    int found = 0;
    pos->z = sdata->sgroups.sZ;

    // Bestimme X-Position
    for (int i = 0; i < 10; ++i) {
        if ((sdata->sgroups.sX >> i) & 1) {
            pos->x = i + 1;
            break;
        }
    }

    // Prüfe zuerst auf Y-oben
    for (int i = 0; i < 5; ++i) {
        if ((sdata->sgroups.sYo >> i) & 1) {
            pos->y = (i + 1) * 2;
            y_last = pos->y;
            found = 1;
            break;
        }
    }
    // Wenn kein Y-oben gefunden, prüfe Y-unten
    if (!found) {
        for (int i = 0; i < 5; ++i) {
            if ((sdata->sgroups.sYu >> i) & 1) {
                pos->y = (i + 1) * 2 - 1;
                y_last = pos->y;
                found = 1;
                break;
            }
        }
    }
    // Wenn weder Y-oben noch Y-unten gefunden: nutze die letzte gültige Y-Position
    if (!found) {
        pos->y = y_last;
    }

    printf("[Positionsbestimmung] X=%d, Y=%d, Z=%d\n", pos->x, pos->y, pos->z);
}
