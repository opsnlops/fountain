
#pragma once

#include <Arduino.h>

#define MAX_COMMAND_LENGTH 20

namespace fountain
{
    



enum CommandType
{
    on,
    await,
    color,
    debug
};

// One command in the queue
struct JetCommand
{
    CommandType command;
    char data[MAX_COMMAND_LENGTH + 1];
} __attribute__((packed));

class Jet
{

public:
    Jet();
    void run(TaskHandle_t ourTask);

    //QueueHandle_t getQueue();

    static void queueReaderTask(void *pvParamenters);

    static QueueHandle_t commandQueue;


};


} // namespace fountain
