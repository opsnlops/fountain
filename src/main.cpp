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

#include "secrets.h"
#include "network/connection.h"
#include "fountain/jet.h"

using namespace fountain;



// XXX: Remove
Jet aJet;

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(19200);
    while (!Serial)
        ;

    // Nice long delay to let the terminal app start
    delay(2500);
    log_i("Helllllo! I'm up and running on on %s!", ARDUINO_VARIANT);

    NetworkConnection network = NetworkConnection();
    network.connectToWiFi();

    log_d("There are %d ticks in 100ms", pdMS_TO_TICKS(100));


    aJet = Jet();
    Jet::commandQueue = xQueueCreate(50, sizeof(struct JetCommand));
    TaskHandle_t jet1Task;
    xTaskCreate(Jet::queueReaderTask,
              "queueReaderTask",
              2048,
              NULL,
              2,
              &jet1Task);
   

}

void loop()
{
    // Kill this task
    // vTaskDelete(NULL);

    delay(500);

    struct JetCommand testCommand;
    testCommand.command = debug;

    log_d("sending message");
    xQueueSendToBack(Jet::commandQueue, &testCommand, pdMS_TO_TICKS(500));
}
