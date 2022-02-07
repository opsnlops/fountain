#if !defined(ESP32)
#error This code is intended to run only on the ESP32 board
#endif

#include <Arduino.h>

#include <WiFi.h>
#include <ESPmDNS.h>

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
}

#include "creature.h"
#include "network/connection.h"
#include "fountain/jet.h"
#include "mdns/creature-mdns.h"
#include "time/time.h"

using namespace creatures;

static const char *TAG = "Main";

// Function Prototypes
void jet1TaskRunner(void *pvParamenters);
void messageSenderTask(void *pvParamenters);

Jet jet1 = Jet(1);

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(19200);
    while (!Serial)
        ;

    // Nice long delay to let the terminal app start
    delay(2500);
    ESP_LOGI(TAG, "Helllllo! I'm up and running on on %s!", ARDUINO_VARIANT);

    NetworkConnection network = NetworkConnection();
    network.connectToWiFi();

    // Register ourselves in mDNS
    CreatureMDNS creatureMDNS = CreatureMDNS(std::string(CREATURE_NAME));
    creatureMDNS.registerService(666);
    creatureMDNS.addStandardTags();

    ESP_LOGD(TAG, "There are %d ticks in 100ms", pdMS_TO_TICKS(100));

    Time time = Time();
    time.init();
    time.obtainTime();

    jet1.setQueue(xQueueCreate(50, sizeof(struct JetCommand)));
    TaskHandle_t jet1Task;
    xTaskCreate(jet1TaskRunner,
                "jet1Task",
                2048,
                NULL,
                2,
                &jet1Task);

    // Fire off a task just for sending messages
    TaskHandle_t messageSender;
    xTaskCreate(messageSenderTask,
                "messageSender",
                2048,
                NULL,
                2,
                &messageSender);
}

void loop()
{
    // Kill this task
    vTaskDelete(NULL);
}

void messageSenderTask(void *pvParamenters)
{
    for (;;)
    {
        delay(500);

        struct JetCommand testCommand;
        testCommand.command = debug;

        ESP_LOGD(TAG, "sending message");
        xQueueSendToBack(jet1.getQueue(), &testCommand, pdMS_TO_TICKS(500));

        delay(1234);
        struct JetCommand testCommand2;
        testCommand2.command = paws;

        ESP_LOGD(TAG, "sending message");
        xQueueSendToBack(jet1.getQueue(), &testCommand2, pdMS_TO_TICKS(500));
    }
}

void jet1TaskRunner(void *pvParamenters)
{
    struct JetCommand currentCommand;

    for (;;)
    {
        if (xQueueReceive(jet1.getQueue(), &currentCommand, (TickType_t)pdMS_TO_TICKS(1000)) == pdPASS)
        {
            ESP_LOGD(TAG, "got a command");

            switch (currentCommand.command)
            {
            case debug:
                ESP_LOGD(TAG, "DEBUG MESSAGE");
                break;
            case paws:
                ESP_LOGD(TAG, "PAUSE MESSAGE");
                break;
            }
        }
    }
}
