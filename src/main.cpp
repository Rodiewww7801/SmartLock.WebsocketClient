#include <Arduino.h>
#include <ArduinoJson.h>
#include "Services/WebClient.h"
#include "Services/WiFiConnector.h"
#include "Services/LockInstance.h"
#include "Config.h"
#include <esp_log.h>

unsigned long previousMillis;
unsigned long interval = 1000;

WiFiConnector *wifiConnector;
WebClient *webClient;
LockInstance *lockInstace;

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(GPIO_NUM_15, OUTPUT);
  pinMode(GPIO_NUM_2, OUTPUT);

  lockInstace = new LockInstance();
  lockInstace->currentGPIO = GPIO_NUM_15;
  lockInstace->serverStatusGPIO = GPIO_NUM_2;
  wifiConnector = new WiFiConnector(wifiSSID, wifiPassword);
  wifiConnector->connect();
  webClient = new WebClient();
  webClient->lockInstance = lockInstace;
  webClient->setWebsocketConnection(websocketIP, websocketPort);
}

void loop()
{
  if (wifiConnector->status != WL_CONNECTED)
  {
    return;
  }

  webClient->loop();
  lockInstace->loop();
}
