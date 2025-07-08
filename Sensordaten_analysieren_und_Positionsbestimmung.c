#include "timer_module.h"
#include "msgqueue.h"
#include "semaphore.h"
#include "positionsbest.h"
#include "sensordata.h"
#include "vxWorks.h"
#include "Busdata.h"
#include "signal_module.h"
#include <stdio.h>
#include "taskLib.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>

// Struktur für die Lagerzustandsdaten
typedef struct {
    Position pos;
    int belegungsdaten;
} LagerData;

// Globale Variablen für Lagerdaten und Lichtschrankenstatus
LagerData g_lagerdata;
int g_lichtschranke = 0;

// Globale Timer-IDs
timer_t sensordata_timer;
timer_t positionsbest_timer;

// Prozess: Sensordaten analysieren (Timer-gesteuert)
void sensordata_task(timer_t timerID, int arg) {
    sbusdata sensor_raw;
    readSensorData(&sensor_raw);

    // Senden der Sensordaten an Positionsbestimmung
    msgQSend(mq_sensor_to_pos, (char *)&sensor_raw, sizeof(sensor_raw), WAIT_FOREVER, MSG_PRI_NORMAL);

    // Lichtschrankenstatus extrahieren und an Lagerzustand senden
    int licht = sensor_raw.sgroups.sL;
    msgQSend(mq_licht_to_lager, (char *)&licht, sizeof(int), WAIT_FOREVER, MSG_PRI_NORMAL);
}

// Prozess: Positionsbestimmung (Timer-gesteuert, Signal Handler)
void positionsbest_task(timer_t timerID, int arg) {
    sbusdata sensor_raw;
    Position pos;

    // Empfangen der Sensordaten von der Queue (non-blocking)
    if (msgQReceive(mq_sensor_to_pos, (char *)&sensor_raw, sizeof(sensor_raw), NO_WAIT) != ERROR) {
        positionBestimmen(&sensor_raw, &pos);
        // Senden der Positionsdaten an Lagerzustand
        msgQSend(mq_pos_to_lager, (char *)&pos, sizeof(Position), WAIT_FOREVER, MSG_PRI_NORMAL);
        // Signal an eigenen Task (SIGUSR1) senden
        kill(taskIdSelf(), SIGUSR1);
    }
}

// Prozess: Lagerzustand (Task, verwendet Semaphore)
void lagerzustand_task() {
    int licht;
    Position pos;
    while (1) {
        // Empfangen des Lichtschrankenstatus (non-blocking)
        if (msgQReceive(mq_licht_to_lager, (char *)&licht, sizeof(int), NO_WAIT) != ERROR) {
            semTake(lager_sem, WAIT_FOREVER);
            g_lichtschranke = licht;
            semGive(lager_sem);
            printf("[Lagerzustand] Lichtschranke aktualisiert: %d\n", licht);
        }

        // Empfangen der Positionsdaten (non-blocking)
        if (msgQReceive(mq_pos_to_lager, (char *)&pos, sizeof(Position), NO_WAIT) != ERROR) {
            semTake(lager_sem, WAIT_FOREVER);
            g_lagerdata.pos = pos;
            g_lagerdata.belegungsdaten = (pos.x + pos.y + pos.z) % 2; // Beispielhafte Logik
            // Senden der Daten an Aktorsteuerung (hier als printf simuliert)
            printf("[Lagerzustand] Sende an Aktorsteuerung: Position[X=%d Y=%d Z=%d], Belegungsdaten=%d\n",
                pos.x, pos.y, pos.z, g_lagerdata.belegungsdaten);
            semGive(lager_sem);
        }
        taskDelay(10); // CPU-Last reduzieren
    }
}

// Startet den Task für den Lagerzustand
void start_lagerzustand_task() {
    if (taskSpawn("lagerzustand", 90, 0, 4096, (FUNCPTR)lagerzustand_task, 0,0,0,0,0,0,0,0,0,0) == ERROR) {
        printf("taskSpawn lagerzustand fehlgeschlagen!\n"); exit(1);
    }
}

// Hauptfunktion: Initialisierung aller Komponenten und Start der Prozesse
int main() {
    initMsgQueues();
    initSemaphore();

    // Sensordaten analysieren: Timer alle 50 ms
    setup_timer(&sensordata_timer, sensordata_task, 50);

    // Positionsbestimmung: Timer alle 60 ms, mit Signal Handler
    //setupSignalHandler();
    setup_timer(&positionsbest_timer, positionsbest_task, 60);

    // Lagerzustand: Task starten
    start_lagerzustand_task();

    printf("System gestartet.\n");

    while(1) taskDelay(100); // Hauptschleife läuft endlos
    return 0;
}
