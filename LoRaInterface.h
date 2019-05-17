/*
 * LoRaInterface.ino 
 * 
 * Implements all the LoRa communication functions.
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */
#ifndef __LORA_INTERFACE
#define __LORA_INTERFACE

#define MAX_LORA_PAYLOAD  256

#define MIN_SPREAD_FACTOR (7)   // technically it's 6 but in EU band all channels use 7-12
#define MAX_SPREAD_FACTOR (12)

#define MAX_TX_POWER      (20)

struct LoRaPacket {
  //const char payload[MAX_LORA_PAYLOAD];
  String  payload;
  int     index;
  int     rssi;
  LoRaPacket(): payload(""), index(0), rssi(0) {};
};

#endif //  __LORA_INTERFACE
