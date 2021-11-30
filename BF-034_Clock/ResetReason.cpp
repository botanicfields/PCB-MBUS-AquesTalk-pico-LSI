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
#include "ResetReason.h"

void print_reset_reason(RESET_REASON reason)
{
  switch (reason) {
    case  1 : Serial.println("[ResetReason]POWERON_RESET");           break;  // <1,  Vbat power on reset
    case  3 : Serial.println("[ResetReason]SW_RESET");                break;  // <3,  Software reset digital core
    case  4 : Serial.println("[ResetReason]OWDT_RESET");              break;  // <4,  Legacy watch dog reset digital core
    case  5 : Serial.println("[ResetReason]DEEPSLEEP_RESET");         break;  // <5,  Deep Sleep reset digital core
    case  6 : Serial.println("[ResetReason]SDIO_RESET");              break;  // <6,  Reset by SLC module, reset digital core
    case  7 : Serial.println("[ResetReason]TG0WDT_SYS_RESET");        break;  // <7,  Timer Group0 Watch dog reset digital core
    case  8 : Serial.println("[ResetReason]TG1WDT_SYS_RESET");        break;  // <8,  Timer Group1 Watch dog reset digital core
    case  9 : Serial.println("[ResetReason]RTCWDT_SYS_RESET");        break;  // <9,  RTC Watch dog Reset digital core
    case 10 : Serial.println("[ResetReason]INTRUSION_RESET");         break;  // <10, Instrusion tested to reset CPU
    case 11 : Serial.println("[ResetReason]TGWDT_CPU_RESET");         break;  // <11, Time Group reset CPU
    case 12 : Serial.println("[ResetReason]SW_CPU_RESET");            break;  // <12, Software reset CPU
    case 13 : Serial.println("[ResetReason]RTCWDT_CPU_RESET");        break;  // <13, RTC Watch dog Reset CPU
    case 14 : Serial.println("[ResetReason]EXT_CPU_RESET");           break;  // <14, for APP CPU, resetted by PRO CPU
    case 15 : Serial.println("[ResetReason]RTCWDT_BROWN_OUT_RESET");  break;  // <15, Reset when the vdd voltage is not stable
    case 16 : Serial.println("[ResetReason]RTCWDT_RTC_RESET");        break;  // <16, RTC Watch dog reset digital core and rtc module
    default : Serial.println("[ResetReason]NO_MEAN");
  }
}

void verbose_print_reset_reason(RESET_REASON reason)
{
  switch (reason) {
    case  1 : Serial.println("[ResetReason]Vbat power on reset");                              break;
    case  3 : Serial.println("[ResetReason]Software reset digital core");                      break;
    case  4 : Serial.println("[ResetReason]Legacy watch dog reset digital core");              break;
    case  5 : Serial.println("[ResetReason]Deep Sleep reset digital core");                    break;
    case  6 : Serial.println("[ResetReason]Reset by SLC module, reset digital core");          break;
    case  7 : Serial.println("[ResetReason]Timer Group0 Watch dog reset digital core");        break;
    case  8 : Serial.println("[ResetReason]Timer Group1 Watch dog reset digital core");        break;
    case  9 : Serial.println("[ResetReason]RTC Watch dog Reset digital core");                 break;
    case 10 : Serial.println("[ResetReason]Instrusion tested to reset CPU");                   break;
    case 11 : Serial.println("[ResetReason]Time Group reset CPU");                             break;
    case 12 : Serial.println("[ResetReason]Software reset CPU");                               break;
    case 13 : Serial.println("[ResetReason]RTC Watch dog Reset CPU");                          break;
    case 14 : Serial.println("[ResetReason]for APP CPU, resetted by PRO CPU");                 break;
    case 15 : Serial.println("[ResetReason]Reset when the vdd voltage is not stable");         break;
    case 16 : Serial.println("[ResetReason]RTC Watch dog reset digital core and rtc module");  break;
    default : Serial.println("[ResetReason]NO_MEAN");
  }
}

void PrintResetReason()
{
  Serial.println("[ResetReason]CPU0 reset reason:");
  print_reset_reason(rtc_get_reset_reason(0));
  verbose_print_reset_reason(rtc_get_reset_reason(0));

  Serial.println("[ResetReason]CPU1 reset reason:");
  print_reset_reason(rtc_get_reset_reason(1));
  verbose_print_reset_reason(rtc_get_reset_reason(1));
}
