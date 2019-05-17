/*
 * HeltecSupport.ino 
 * 
 * HAL for the Heltec ESP32 LoRa board
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include "heltec.h"
#include "HAL.h"

// Board configuration
// Configure specific vendor boards here
uint64_t _chipID;

void configureBoard() {
#ifdef HELTEC
  Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/, true /*LoRa use PABOOST*/, LORA_BAND /*LoRa RF working band*/);

  _chipID = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(_chipID>>32));  //print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)_chipID);                      //print Low 4bytes.
#endif
}

// Display support

void clearDisplay() {
#ifdef HELTEC
  Heltec.display->clear();
#endif
}

void displayString(int x, int y, const char *str) {
#ifdef HELTEC
  Heltec.display -> drawString(x*9, y*9, str);
  Heltec.display -> display();
#endif
  Serial.println(str);
}


 
