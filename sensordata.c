#include "sensordata.h"
#include <stdio.h>
#include <string.h>
#include "Busdata.h"
//#include <fcntl.h>    // open(), O_RDONLY
//#include <unistd.h>   // read(), close(), ssize_t
#include <ioLib.h>

// Simuliert das Auslesen der Sensordaten
// Hier kann die Belegung beliebig angepasst werden
void readSensorData(sbusdata* data) {
	//sdata_init(data);

    int fd = open("/pipe/sensordata", O_RDONLY, 0);
    if (fd < 0) { sdata_init(data); return; }
    ssize_t n = read(fd, data, sizeof(sbusdata));
    if (n != sizeof(sbusdata)) sdata_init(data);
    close(fd);
   
}
