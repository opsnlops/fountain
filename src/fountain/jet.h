
#pragma once

#include <Arduino.h>

#define MAX_COMMAND_LENGTH 20

namespace fountain
{

    enum CommandType
    {
        on,
        paws,
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
        void setQueue(QueueHandle_t ourQueue);
        QueueHandle_t getQueue();

    private:
        QueueHandle_t commandQueue;
    };

} // namespace fountain
