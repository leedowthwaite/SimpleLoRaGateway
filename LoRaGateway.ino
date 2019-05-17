/*
 * LoRaGateway.ino 
 * 
 * Implements a minimal, single-channel LoRa to MQTT gateway
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include "Arduino.h"
#include <PubSubClient.h>

#include "HAL.h"
#include "LoRaInterface.h"

// checkAndForwardPackets()
// This is the core function that checks for received LoRa packets and forwards the contents on to MQTT
//
static void checkAndForwardPackets() {
  // check for received data
  struct LoRaPacket *rxPacket = checkRxBuffer();
  if (rxPacket) {
    // forward packet content to MQTT
    const char *msg = rxPacket->payload.c_str();
    publishMQTT(msg);

    Serial.print("rx packet: msg: ");
    Serial.println(msg);

    clearDisplay();
    displayString(0, 0, "received packet ");
    displayString(10, 0, msg);
    displayRssi(rxPacket->rssi);
  }
}

// Arduino main hooks

void setup() {
  // initialise the board
  configureBoard();
  
  Serial.begin(115200);
  Serial.println("setup()");

  pinMode(LED_BUILTIN, OUTPUT);

  clearDisplay();
  displayString(0, 0, "Initialising GATEWAY...");

  // Initialise wifi connection
  initWiFi();
  
  // Configure LoRa interface
  configureLoRa();

  if (isWiFiConnected()) {
    connectMQTT();
  }
  Serial.println("setup() done");
}


void loop() {

  // ensure WiFi stays connected
  checkWiFiStatus();

  // Perform packet forwarding
  checkAndForwardPackets();
  
  // MQTT housekeeping
  updateMQTT();

  delay(100);
}
