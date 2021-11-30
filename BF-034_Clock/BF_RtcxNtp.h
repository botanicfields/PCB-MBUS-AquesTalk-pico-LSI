// Copyright 2021 BotanicFields, Inc.
// for RTCx(PCf8563) and NTP

#pragma once

#include <sys/time.h>  // for struct timeval
#include <sntp.h>      // for sntp_sync_status, https://github.com/espressif/arduino-esp32 1.0.6-

void NtpBegin();
void SntpTimeSyncNotificationCallback(struct timeval *tv);
void RtcxUpdate();
void PrintSntpStatus(const char* header, sntp_sync_status_t sntp_sync_status);
void SetTimeFromRtcx();
