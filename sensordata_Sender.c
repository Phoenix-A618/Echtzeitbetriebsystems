#include "sensordata.h"
#include <vxWorks.h>
#include <pipeDrv.h>
#include <ioLib.h>
#include <stdio.h>
#include <string.h>
#include <taskLib.h>

// Sendeprozess: Liest Sensordaten und schreibt sie in die Pipe
void sender_task(void) {
    pipeDrv(10); // Pipe-Treiber laden (nur einmal nötig)
    pipeDevCreate("/pipe/sensordata", 10, sizeof(sbusdata)); // Pipe anlegen

    int fd = open("/pipe/sensordata", O_WRONLY, 0);
    if (fd == ERROR) {
        printf("Sender: Fehler beim Oeffnen der Pipe.\n");
        return;
    }

    while (1) {
        sbusdata sdata;
        readSensorData(&sdata);  // Sensordaten simuliert lesen
        write(fd, &sdata, sizeof(sbusdata));
        printf("Sender: Sensordaten gesendet.\n");
        taskDelay(50); // 50ms warten
    }
    close(fd);
}
