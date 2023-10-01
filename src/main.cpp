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
LockInstance *lockInstance;

void setup()
{
  // Configure timezone for time


  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(GPIO_NUM_15, OUTPUT);
  pinMode(GPIO_NUM_2, OUTPUT);

  lockInstance = new LockInstance();
  lockInstance->currentGPIO = GPIO_NUM_15;
  lockInstance->serverStatusGPIO = GPIO_NUM_2;
  lockInstance->deviceSerialNumber = deviceSerialNumber;

  wifiConnector = new WiFiConnector(wifiSSID, wifiPassword);
  wifiConnector->connect();

  webClient = new WebClient();
  webClient->lockInstance = lockInstance;
  const char *rootCACert = readRootCACertificateFromSPIFFS();
  webClient->setWebsocketConnection(host, websocketPort, "/", rootCACert);
}

void loop()
{
  if (wifiConnector->status != WL_CONNECTED)
  {
    return;
  }

  if (webClient->ipAddress == NULL || lockInstance->deviceSerialNumber == NULL)
  {
      webClient->ipAddress = wifiConnector->ipStr;
      lockInstance->deviceSerialNumber = deviceSerialNumber;
  }

  webClient->loop();
  lockInstance->loop();
}

char *readRootCACertificateFromSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("[Error] has occurred whule mounting SPIFFS");
    return NULL;
  }

  File file = SPIFFS.open(filePathToRootCACert);
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