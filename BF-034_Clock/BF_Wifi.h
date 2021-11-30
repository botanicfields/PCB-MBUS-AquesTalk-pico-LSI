// Copyright 2021 BotanicFields, Inc.
// for WiFiManager

#pragma once

#include <WiFi.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

void WifiBegin();
void WifiProcess();
void ConfigModeCallback(WiFiManager *wm);
void PrintWiFiStatus(const char* header, wl_status_t wl_status);
