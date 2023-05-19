#ifndef _bler_ble_h
#define _bler_ble_h

#include <Arduino.h>

void start_adv(String manufacturerData,String manufacturerId = "9d22");
void stop_adv();

#endif