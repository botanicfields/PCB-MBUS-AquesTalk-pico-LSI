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
#ifndef INCLUDED_RESET_REASON_H
#define INCLUDED_RESET_REASON_H

#include <rom/rtc.h>

void print_reset_reason(RESET_REASON reason);
void verbose_print_reset_reason(RESET_REASON reason);
void PrintResetReason();

#endif  // INCLUDED_RESET_REASON_H
