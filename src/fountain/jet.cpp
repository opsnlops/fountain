

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
    }

    // QueueHandle_t Jet::getQueue()
    //{
    //     return commandQueue;
    // }

    void Jet::queueReaderTask(void *pvParamenters)
    {
        struct JetCommand currentCommand;

        for (;;)
        {
            if (xQueueReceive(commandQueue, &currentCommand, (TickType_t)pdMS_TO_TICKS(1000)) == pdPASS)
            {
                log_d("got a command");

                switch (currentCommand.command)
                {
                case debug:
                    log_d("DEBUG MESSAGE");
                    break;
                }
            }
        }
    }

} // namespace fountain