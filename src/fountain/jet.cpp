
#include <Arduino.h>

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
}

#include "jet.h"

namespace creatures
{
    namespace fountain
    {

        Jet::Jet(uint8_t number)
        {
            this->number = number;
            log_d("jet %d created", number);
        }
        uint8_t Jet::getNumber()
        {
            return number;
        }

        void Jet::setPin(uint8_t pin)
        {
            this->pin = pin;
        }
        uint8_t Jet::getPin()
        {
            return pin;
        }

        void Jet::setHeight(uint8_t height)
        {
            this->height = height;
        }
        uint8_t Jet::getHeight()
        {
            return height;
        }

        void Jet::setQueue(QueueHandle_t ourQueue)
        {
            this->commandQueue = ourQueue;
        }
        QueueHandle_t Jet::getQueue()
        {
            return commandQueue;
        }

    } // namespace fountain
} // namespace creatures