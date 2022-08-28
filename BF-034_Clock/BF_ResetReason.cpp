// Reset Reason print to LCD of M5Stack

/*
*  Print last reset reason of ESP32
*  =================================
*
*  Use either of the methods print_reset_reason
*  or verbose_print_reset_reason to display the
*  cause for the last reset of this device.
*
*  Public Domain License.
*
*  Author:
*  Evandro Luis Copercini - 2017
*/

#include <Arduino.h>
#include "BF_ResetReason.h"

const char* ResetReason(RESET_REASON reason)
{
  const char* reset_reason[] = {
    "NO_MEAN",                 //  0,
    "POWERON_RESET",           //  1, Vbat power on reset
    "2",                       //  2,
    "SW_RESET",                //  3, Software reset digital core
    "OWDT_RESET",              //  4, Legacy watch dog reset digital core
    "DEEPSLEEP_RESET",         //  5, Deep Sleep reset digital core
    "SDIO_RESET",              //  6, Reset by SLC module, reset digital core
    "TG0WDT_SYS_RESET",        //  7, Timer Group0 Watch dog reset digital core
    "TG1WDT_SYS_RESET",        //  8, Timer Group1 Watch dog reset digital core
    "RTCWDT_SYS_RESET",        //  9, RTC Watch dog Reset digital core
    "INTRUSION_RESET",         // 10, Intrusion tested to reset CPU
    "TGWDT_CPU_RESET",         // 11, Time Group reset CPU
    "SW_CPU_RESET",            // 12, Software reset CPU
    "RTCWDT_CPU_RESET",        // 13, RTC Watch dog Reset CPU
    "EXT_CPU_RESET",           // 14, for APP CPU, reset by PRO CPU
    "RTCWDT_BROWN_OUT_RESET",  // 15, Reset when the vdd voltage is not stable
    "RTCWDT_RTC_RESET",        // 16, RTC Watch dog reset digital core and rtc module
  };
  return reset_reason[reason];
}

const char* ResetReasonVerbose(RESET_REASON reason)
{
  const char* reset_reason_verbose[] = {
    " 0, No mean",
    " 1, Vbat power on reset",
    " 2, ",
    " 3, Software reset digital core",
    " 4, Legacy watch dog reset digital core",
    " 5, Deep Sleep reset digital core",
    " 6, Reset by SLC module, reset digital core",
    " 7, Timer Group0 Watch dog reset digital core",
    " 8, Timer Group1 Watch dog reset digital core",
    " 9, RTC Watch dog Reset digital core",
    "10, Intrusion tested to reset CPU",
    "11, Time Group reset CPU",
    "12, Software reset CPU",
    "13, RTC Watch dog Reset CPU",
    "14, for APP CPU, reset by PRO CPU",
    "15, Reset when the vdd voltage is not stable",
    "16, RTC Watch dog reset digital core and rtc module",
  };
  return reset_reason_verbose[reason];
}
