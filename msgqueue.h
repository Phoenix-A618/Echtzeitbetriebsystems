#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include "positionsbest.h"
#include "Busdata.h"
#include "vxWorks.h"
#include "msgQLib.h"

// Initialisiert alle verwendeten Message Queues
void initMsgQueues(void);

// Message Queues für die Kommunikation zwischen Prozessen
extern MSG_Q_ID mq_sensor_to_pos;
extern MSG_Q_ID mq_licht_to_lager;
extern MSG_Q_ID mq_pos_to_lager;

#endif
