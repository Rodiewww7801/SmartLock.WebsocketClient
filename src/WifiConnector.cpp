#include "WiFiConnector.h"

WiFiConnector ::WiFiConnector(const char *wifiSSID, const char *wifiPassword) : WiFiConnector(wifiSSID, wifiPassword, new WiFiMulti()) {}

WiFiConnector ::WiFiConnector(const char *wifiSSID, const char *wifiPassword, WiFiMulti *wifiMultiRef)
{
    this->wifiSSID = wifiSSID;
    this->wifiPassword = wifiPassword;
    this->wiFiMulti = wifiMultiRef;

    this->setup();
}

WiFiConnector ::~WiFiConnector()
{
    delete wifiSSID;
    delete wifiPassword;
    delete wiFiMulti;
}

void WiFiConnector ::setup()
{
    WiFi.mode(WIFI_STA);
    wiFiMulti->addAP(wifiSSID, wifiPassword);
}

void WiFiConnector ::connect()
{
    int failedAttemptsNum = 0;
    Serial.print("[WiFiConnector]: Connecting...");

    while (status != WL_CONNECTED && ++failedAttemptsNum <= 10)
    {
        status = wiFiMulti->run();
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    if (failedAttemptsNum >= 10)
    {
        Serial.println("[WiFiConnector]: Failed to connect to WiFi");
        status = WL_CONNECT_FAILED;
        return;
    }

    Serial.println("[WiFiConnector]: Connected to WiFi");

    String ipStr = WiFi.localIP().toString();
    this->ipStr = new char[ipStr.length() + 1];
    strcpy(this->ipStr, ipStr.c_str());
    Serial.printf("[WiFiConnector]: Local IP: %s\n", this->ipStr);
    return;
}
