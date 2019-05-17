/*
 * MQTT.ino 
 * 
 * Implements all MQTT-related functions.
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include "WiFi.h"

// MQTT Configuration
// if you have a hostname set for the MQTT broker, you can use it here
//const char *serverHostname = "mqtt-server";
// otherwise you can use an IP address like this
static const IPAddress serverIPAddress(192, 168, 0, 2);

// the topic we want to use
static const char *_topic = "env/node1/temp";

// Instantiate the MQTT PubSubClient (requires a WiFiClient which is not used anywhere else)
static WiFiClient _wifiClient;
static PubSubClient _mqttClient(_wifiClient);


// MQTT Setup

// connect to MQTT server
void connectMQTT() {

  // connect to MQTT server  
  //_mqttClient.setServer(serverHostname, 1883);
  _mqttClient.setServer(serverIPAddress, 1883);

  // Wait until we're connected
  while (!_mqttClient.connected()) {
    // Create a unique MQTT client ID of form LoRa-Gateway-XXXX, where the XXXX is the Heltec chip ID.
    String clientId = "LoRa-Gateway-";
    clientId += String((uint32_t)(_chipID>>32));
    clientId += String((uint32_t)_chipID);
    Serial.printf("MQTT connecting as client %s...\n", clientId.c_str());
    // Attempt to connect
    if (_mqttClient.connect(clientId.c_str())) {
      Serial.println("MQTT connected");
      // Once connected, publish an announcement...
      _mqttClient.publish(_topic, String(clientId+" connected").c_str());
    } else {
      Serial.printf("MQTT failed, state %s, retrying...\n", _mqttClient.state());
      // Wait before retrying
      delay(2500);
    }
  }
}

// MQTT housekeeping
//
void updateMQTT() {
  if (!_mqttClient.connected()) {
    connectMQTT();
  }
  // this is ESSENTIAL!
  _mqttClient.loop();
}

// Publish a message on a topic
//
void publishMQTT(const char *topic, const char *msg) {
    _mqttClient.publish(topic, msg);
}

void publishMQTT(const char *msg) {
    _mqttClient.publish(_topic, msg);
}
