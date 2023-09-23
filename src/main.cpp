#include <Arduino.h>
#include <ArduinoJson.h>
#include "Services/WebClient.h"
#include "Services/WiFiConnector.h"
#include "Services/LockInstance.h"
#include "Config.h"
#include <esp_log.h>
#include <SPIFFS.h>

char *readRootCACertificateFromSPIFFS();

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
  const char *caCert = readRootCACertificateFromSPIFFS();
  webClient->setWebsocketConnection(host, websocketPort, "/", caCert);
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

char *readRootCACertificateFromSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("[Error] has occurred whule mounting SPIFFS");
    return NULL;
  }

  File file = SPIFFS.open("/RootCACertificate.cer");
  if (!file)
  {
    Serial.println("[WARNING] RootCACertificate.cer is missing!");
    return NULL;
  }

  String caCertString = file.readString();
  file.close();
  char *returnedValue = new char[caCertString.length() + 1];
  strcpy(returnedValue, caCertString.c_str());
  return returnedValue;
}