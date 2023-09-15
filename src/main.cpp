#include <Arduino.h>
#include <ArduinoJson.h>
#include "WebClient.h"
#include "WiFiConnector.h"
#include "Config.h"

// const char *wifiSSID = "STRONG_0ff0_2.4GHz";
// const char *wifiPassword = "FnMI6mG5Fm";
// const char *websocketIP = "192.168.1.17";
// const unsigned int websocketPort = 81;

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
