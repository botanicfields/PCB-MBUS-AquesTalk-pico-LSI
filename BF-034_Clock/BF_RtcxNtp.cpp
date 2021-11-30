// Copyright 2021 BotanicFields, Inc.
// for RTCx(PCf8563) and NTP

#include <Arduino.h>
#include "BF_Pcf8563.h"
#include "BF_RtcxNtp.h"

const char* time_zone  = "JST-9";
const char* ntp_server = "pool.ntp.org";
bool sntp_sync_status_complete(false);

void NtpBegin()
{
  configTzTime(time_zone, ntp_server);
  Serial.print("NtpBegin: Config TZ time\n");
  Serial.printf("NtpBegin: SNTP sync mode = %d (0:IMMED 1:SMOOTH)\n", sntp_get_sync_mode());
  Serial.printf("NtpBegin: SNTP sync interval = %dms\n", sntp_get_sync_interval());
  sntp_sync_status_complete = false;
  sntp_set_time_sync_notification_cb(SntpTimeSyncNotificationCallback);
}

void SntpTimeSyncNotificationCallback(struct timeval *tv)
{
  sntp_sync_status_t sntp_sync_status = sntp_get_sync_status();
  PrintSntpStatus("SNTP callback:", sntp_sync_status);

  if (sntp_sync_status == SNTP_SYNC_STATUS_COMPLETED)
    sntp_sync_status_complete = true;
}

void RtcxUpdate()
{
  if (sntp_sync_status_complete) {
    sntp_sync_status_complete = false;

    struct tm tm_sync;
    getLocalTime(&tm_sync);
    Serial.print(&tm_sync, "RTCX Update: %A, %B %d %Y %H:%M:%S\n");
    // print sample: must be < 64
    //....:....1....:....2....:....3....:....4....:....5....:....6....
    //RTCX Update: Wednesday, September 11 2021 11:10:46

    if (rtcx.WriteTime(&tm_sync) == 0)
      Serial.print("RTCx Update: RTCx time updated\n");
    else
      Serial.print("RTCx Update: RTCx update failed\n");
  }
}

void PrintSntpStatus(const char* header, sntp_sync_status_t sntp_sync_status)
{
  static const char* sntp_sync_status_str[] = {
    "SNTP_SYNC_STATUS_RESET       ",  // 0
    "SNTP_SYNC_STATUS_COMPLETED   ",  // 1
    "SNTP_SYNC_STATUS_IN_PROGRESS ",  // 2
    "sntp_sync_status invalid     ",  // 3
  };
  int sntp_sync_status_index = 3;
  if (sntp_sync_status >= 0 && sntp_sync_status <= 2)
    sntp_sync_status_index = sntp_sync_status;
  Serial.printf("%s SNTP sync status = %d: %s\n", header, sntp_sync_status, sntp_sync_status_str[sntp_sync_status_index]);
}

void SetTimeFromRtcx()
{
  struct tm tm_init;

  setenv("TZ", time_zone, 1);
  tzset();  // Assign the local timezone from setenv for mktime()
  if (rtcx.ReadTime(&tm_init) == 0) {
    Serial.print("Set time from RTCx: RTCx valid\n");
    struct timeval tv = { mktime(&tm_init), 0 };
    settimeofday(&tv, NULL);
  }
  else {
    Serial.print("Set time from RTCx: ERROR RTCx invalid\n");
    tm_init.tm_year = 120;  // 2020
    tm_init.tm_mon  = 0;    // January
    tm_init.tm_mday = 1;
    tm_init.tm_hour = 0;
    tm_init.tm_min  = 0;
    tm_init.tm_sec  = 0;
    struct timeval tv = { mktime(&tm_init), 0 };
    settimeofday(&tv, NULL);
  }

  getLocalTime(&tm_init);
  Serial.print(&tm_init, "Set time from RTCx: %A, %B %d %Y %H:%M:%S\n");
  // print sample: must be < 64
  //....:....1....:....2....:....3....:....4....:....5....:....6....
  //Set time from RTCx: Wednesday, September 11 2021 11:10:46
}
