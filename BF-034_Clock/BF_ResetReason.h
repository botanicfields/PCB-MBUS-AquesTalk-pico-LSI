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
#pragma once

#include <rom/rtc.h>

String ResetReasonStr(RESET_REASON reason);
String VerboseResetReasonStr(RESET_REASON reason);
String ResetReasonCpu0();
String ResetReasonCpu1();
String VerboseRestReasonCpu0();
String VerboseRestReasonCpu1();
