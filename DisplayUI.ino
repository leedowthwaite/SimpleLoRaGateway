/*
 * DisplayUI.ino 
 * 
 * Helper functions for the display UI.
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include "HAL.h"

void displaySpreadFactor(int sf) {
    displayString(0, 1, "SF:            ");
    displayString(3, 1, String(sf).c_str());    
}

void displayRssi(int rssi) {
  displayString(0, 2, "RSSI:         ");
  displayString(6, 2, String(rssi, DEC).c_str());
}
