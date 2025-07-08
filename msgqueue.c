#include "msgqueue.h"
#include <stdio.h>

// Globale Message Queue-IDs
MSG_Q_ID mq_sensor_to_pos = NULL;
MSG_Q_ID mq_licht_to_lager = NULL;
MSG_Q_ID mq_pos_to_lager = NULL;

// Initialisiert die Message Queues für die Interprozesskommunikation
void initMsgQueues(void) {
    mq_sensor_to_pos = msgQCreate(10, sizeof(sbusdata), MSG_Q_FIFO);
    mq_licht_to_lager = msgQCreate(10, sizeof(int), MSG_Q_FIFO);
    mq_pos_to_lager = msgQCreate(10, sizeof(Position), MSG_Q_FIFO);

    if (!mq_sensor_to_pos || !mq_licht_to_lager || !mq_pos_to_lager) {
        printf("msgQCreate fehlgeschlagen!\n");
    }
}
