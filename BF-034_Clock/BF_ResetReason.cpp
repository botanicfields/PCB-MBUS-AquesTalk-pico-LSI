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

#include <M5Stack.h>
#include "BF_ResetReason.h"

String ResetReasonStr(RESET_REASON reason)
{
  switch (reason) {
    case  1 : return "POWERON_RESET";           break;  // <1,  Vbat power on reset
    case  3 : return "SW_RESET";                break;  // <3,  Software reset digital core
    case  4 : return "OWDT_RESET";              break;  // <4,  Legacy watch dog reset digital core
    case  5 : return "DEEPSLEEP_RESET";         break;  // <5,  Deep Sleep reset digital core
    case  6 : return "SDIO_RESET";              break;  // <6,  Reset by SLC module, reset digital core
    case  7 : return "TG0WDT_SYS_RESET";        break;  // <7,  Timer Group0 Watch dog reset digital core
    case  8 : return "TG1WDT_SYS_RESET";        break;  // <8,  Timer Group1 Watch dog reset digital core
    case  9 : return "RTCWDT_SYS_RESET";        break;  // <9,  RTC Watch dog Reset digital core
    case 10 : return "INTRUSION_RESET";         break;  // <10, Instrusion tested to reset CPU
    case 11 : return "TGWDT_CPU_RESET";         break;  // <11, Time Group reset CPU
    case 12 : return "SW_CPU_RESET";            break;  // <12, Software reset CPU
    case 13 : return "RTCWDT_CPU_RESET";        break;  // <13, RTC Watch dog Reset CPU
    case 14 : return "EXT_CPU_RESET";           break;  // <14, for APP CPU, resetted by PRO CPU
    case 15 : return "RTCWDT_BROWN_OUT_RESET";  break;  // <15, Reset when the vdd voltage is not stable
    case 16 : return "RTCWDT_RTC_RESET";        break;  // <16, RTC Watch dog reset digital core and rtc module
    default : return "NO_MEAN";                 break;
  }
}

String VerboseResetReasonStr(RESET_REASON reason)
{
  switch (reason) {
    case  1 : return "Vbat power on reset";                              break;
    case  3 : return "Software reset digital core";                      break;
    case  4 : return "Legacy watch dog reset digital core";              break;
    case  5 : return "Deep Sleep reset digital core";                    break;
    case  6 : return "Reset by SLC module, reset digital core";          break;
    case  7 : return "Timer Group0 Watch dog reset digital core";        break;
    case  8 : return "Timer Group1 Watch dog reset digital core";        break;
    case  9 : return "RTC Watch dog Reset digital core";                 break;
    case 10 : return "Instrusion tested to reset CPU";                   break;
    case 11 : return "Time Group reset CPU";                             break;
    case 12 : return "Software reset CPU";                               break;
    case 13 : return "RTC Watch dog Reset CPU";                          break;
    case 14 : return "for APP CPU, resetted by PRO CPU";                 break;
    case 15 : return "Reset when the vdd voltage is not stable";         break;
    case 16 : return "RTC Watch dog reset digital core and rtc module";  break;
    default : return "NO_MEAN";  break;
  }
}

String ResetReasonCpu0() { return ResetReasonStr(rtc_get_reset_reason(0)); }
String ResetReasonCpu1() { return ResetReasonStr(rtc_get_reset_reason(1)); }
String VerboseRestReasonCpu0() { return VerboseResetReasonStr(rtc_get_reset_reason(0)); }
String VerboseRestReasonCpu1() { return VerboseResetReasonStr(rtc_get_reset_reason(1)); }
