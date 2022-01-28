

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
}

#include "jet.h"

namespace fountain
{

    Jet::Jet()
    {
        log_d("jet created");
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