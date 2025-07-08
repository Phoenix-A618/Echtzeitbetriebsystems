#include "sensordata.h"
#include "positionsbest.h"
#include <vxWorks.h>
#include <ioLib.h>
#include <stdio.h>
#include <string.h>
#include <taskLib.h>

// Empfängerprozess: Liest Sensordaten aus der Pipe und berechnet die Position
void empfaenger_task(void) {
    int fd = open("/pipe/sensordata", O_RDONLY, 0);
    if (fd == ERROR) {
        printf("Empfaenger: Fehler beim Öffnen der Pipe.\n");
        return;
    }

    while (1) {
        sbusdata sdata;
        int r = read(fd, &sdata, sizeof(sbusdata));
        if (r == sizeof(sbusdata)) {
            Position pos;
            positionBestimmen(&sdata, &pos);
            printf("Empfaenger: Position berechnet: X=%d Y=%d Z=%d\n", pos.x, pos.y, pos.z);
        } else {
            printf("Empfaenger: Keine neuen Sensordaten.\n");
        }
        taskDelay(10);
    }
    close(fd);
}
