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

const char* ResetReason(RESET_REASON reason);
const char* ResetReasonVerbose(RESET_REASON reason);
