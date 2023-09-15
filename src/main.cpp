#include <Arduino.h>
#include <ArduinoJson.h>
#include "WebClient.h"
#include "WiFiConnector.h"
#include "Config.h"

unsigned long previousMillis;
unsigned long interval = 1000;

WiFiConnector *wifiConnector;
WebClient *webClient;

void setup()
{
  Serial.begin(115200);

  wifiConnector = new WiFiConnector(wifiSSID, wifiPassword);
  wifiConnector->connect();
  webClient = new WebClient();
  webClient->setWebsocketConnection(websocketIP, websocketPort, "/ws");
}

void loop()
{
  if (wifiConnector->status != WL_CONNECTED)
  {
    return;
  }

  webClient->loop();
}
