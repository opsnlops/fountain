
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
        height,
        debug
    };

    // One command in the queue
    struct JetCommand
    {
        uint8_t jet;
        CommandType command;
        char data[MAX_COMMAND_LENGTH + 1];
    } __attribute__((packed));

    class Jet
    {

    public:
        Jet(uint8_t number);

        uint8_t getNumber();

        void setPin(uint8_t pin);
        uint8_t getPin();

        void setHeight(uint8_t height);
        uint8_t getHeight();

        void setQueue(QueueHandle_t ourQueue);
        QueueHandle_t getQueue();

    private:
        uint8_t number;
        uint8_t pin;
        uint8_t height;

        QueueHandle_t commandQueue;
    };

} // namespace fountain
