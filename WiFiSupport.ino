/*
 * WiFiSupport.ino
 * 
 * Support for ESPxxx on-board WiFi
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.  
 */

#include "WiFi.h"

// WiFi Credentials
static const char *kSSID = "******";
static const char *kPASSWORD = "******";

// WiFi status
static bool _wifiConnected = false;

// connect to wifi – returns true if successful or false if not
//
static boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.disconnect(true);
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(kSSID, kPASSWORD);
  displayString(0, 0, "Connecting to WiFi...");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.print("Connected to ");
    Serial.println(kSSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }

  return state;
}

static void displayConnectionStatus() {
  clearDisplay();
  if (_wifiConnected) {
    displayString(0, 0, "Connected to:");
    displayString(0, 1, kSSID);
    displayString(0, 2, "IP address:");
    displayString(0,3, String(WiFi.localIP()).c_str());
  } else {
    displayString(0, 0, "Connection failed.");
  }
}


void initWiFi() {
  _wifiConnected = connectWifi();
  displayConnectionStatus();
  delay(1000);
}

bool isWiFiConnected() {
  return _wifiConnected;
}

void checkWiFiStatus() {
  if (!_wifiConnected) {
    _wifiConnected = connectWifi();
    displayConnectionStatus();
    delay(1000);
  }
}
