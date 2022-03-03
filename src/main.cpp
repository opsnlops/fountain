#if !defined(ESP32)
#error This code is intended to run only on the ESP32 board
#endif

#include <Arduino.h>

extern "C"
{
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/queue.h"
}

#include "fountain/jet.h"
#include "ota.h"

#include "mqtt/mqtt.h"
#include "network/connection.h"
#include "creatures/creatures.h"
#include "time/time.h"
#include "mdns/creature-mdns.h"
#include "mdns/magicbroker.h"
#include "logging/logging.h"

#include "creature.h"

using namespace creatures;

// Keep a link to our logger
static Logger l;
static MQTT mqtt = MQTT(String(CREATURE_NAME));

// Function Prototypes
void jet1TaskRunner(void *pvParamenters);
void messageSenderTask(void *pvParamenters);

Jet jet1 = Jet(1);

void setup()
{
    l.init();
    l.info("Helllllo! I'm up and running on on %s!", ARDUINO_VARIANT);

    NetworkConnection network = NetworkConnection();
    network.connectToWiFi();

    // Register ourselves in mDNS
    CreatureMDNS creatureMDNS = CreatureMDNS(CREATURE_NAME, CREATURE_POWER);
    creatureMDNS.registerService(666);
    creatureMDNS.addStandardTags();

    digitalWrite(LED_BUILTIN, LOW);

    Time time = Time();
    time.init();
    time.obtainTime();

    // Get the location of the magic broker
    MagicBroker magicBroker;
    magicBroker.find();

    // Connect to MQTT
    mqtt.connect(magicBroker.ipAddress, magicBroker.port);
    mqtt.subscribe(String("cmd"), 0);

    // Enable OTA
    setup_ota(String(CREATURE_NAME));
    start_ota();

    mqtt.publish(String("status"), String("I'm alive!!"), 0, false);

    mqtt.startHeartbeat();

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
        vTaskDelay(TickType_t pdMS_TO_TICKS(500));

        struct JetCommand testCommand;
        testCommand.command = debug;

        l.verbose("sending message");
        xQueueSendToBack(jet1.getQueue(), &testCommand, pdMS_TO_TICKS(500));

        vTaskDelay(TickType_t pdMS_TO_TICKS(1234));
        struct JetCommand testCommand2;
        testCommand2.command = paws;

        l.verbose("sending message");
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
            l.verbose("got a command");

            switch (currentCommand.command)
            {
            case debug:
                l.verbose("DEBUG MESSAGE");
                break;
            case paws:
                l.verbose("PAUSE MESSAGE");
                break;
            }
        }
    }
}
