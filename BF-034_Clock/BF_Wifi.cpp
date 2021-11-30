// Copyright 2021 BotanicFields, Inc.
// routines for Wi-Fi

#include <Arduino.h>
#include "BF_Wifi.h"

WiFiManager wm;
const char* wifi_country = "JP";
const int config_potal_timeout_sec(60);  // 60sec
wl_status_t wifi_status_old(WL_NO_SHIELD);

void WifiBegin()
{
//  wm.resetSettings();  // for testing
//  wm.setCountry(wifi_country);
  wm.setConfigPortalBlocking(true);  // blocking mode (default)
  wm.setConfigPortalTimeout(config_potal_timeout_sec);
  wm.setAPCallback(ConfigModeCallback);
  if (wm.autoConnect())
    Serial.print("WifiBegin: WiFi connected...yeey :)\n");
  else
    Serial.print("WifiBegin: Failed to connect\n");
  wm.setConfigPortalBlocking(false);  // non-blocking mode for loop
  wifi_status_old = WiFi.status();
}

void WifiProcess()
{
  wm.process();  // for non-blocking process of WiFiManager

  // print if WiFi status changed
  wl_status_t wifi_status_new = WiFi.status();
  if (wifi_status_old != wifi_status_new) {
    PrintWiFiStatus("WifiProcess:", wifi_status_new);
    wifi_status_old = wifi_status_new;
  }
}

void ConfigModeCallback(WiFiManager *wm)
{
  Serial.print("WiFiManager callback: Enter config mode\n");
}

void PrintWiFiStatus(const char* header, wl_status_t wl_status)
{
  static const char* wl_status_str[] = {
    "WL_IDLE_STATUS     ",  // 0
    "WL_NO_SSID_AVAIL   ",  // 1
    "WL_SCAN_COMPLETED  ",  // 2
    "WL_CONNECTED       ",  // 3
    "WL_CONNECT_FAILED  ",  // 4
    "WL_CONNECTION_LOST ",  // 5
    "WL_DISCONNECTED    ",  // 6
    "WL_NO_SHIELD       ",  // 7 <-- 255
    "wl_status invalid  ",  // 8
  };
  int wl_status_index = 8;
  if (wl_status >= 0 && wl_status <= 6)
    wl_status_index = wl_status;
  if (wl_status == 255)
    wl_status_index = 7;
  Serial.printf("%s WiFi status = %d: %s\n", header, wl_status, wl_status_str[wl_status_index]);
}
