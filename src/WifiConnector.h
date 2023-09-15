#ifndef WIFICONNECTOR_H_
#define WIFICONNECTOR_H_

//#include <Arduino.h>
#include <WiFi.h>
#include <WifiMulti.h>
#include <WiFiClient.h>

class WiFiConnector
{
private:
    const char *wifiSSID;
    const char *wifiPassword;
    WiFiMulti *wiFiMulti;
    WiFiConnector(const char *wifiSSID, const char *wifiPassword, WiFiMulti *wifiMulti);
    ~WiFiConnector();
    void setup();

public:
    char *ipStr;
    uint8_t status = WL_DISCONNECTED;
    
    WiFiConnector(const char *wifiSSID, const char *wifiPassword);
    void connect();
};

#endif
