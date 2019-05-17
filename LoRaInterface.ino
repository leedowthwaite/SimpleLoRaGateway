/*
 * LoRaInterface.ino 
 * 
 * Implements all the LoRa communication functions.
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include "LoRaInterface.h"

// LoRa Settings

//#define ENA_TRANSMIT

#define USE_SPREAD_FACTOR 
static int _spreadFactor = MIN_SPREAD_FACTOR;


#ifdef ENA_TRANSMIT
// transmission parameters
#define SEND_INTERVAL (1000)
static long _lastSendTime = 0;
static uint32_t _txCounter = 0;
#endif

// LoRa receive buffers
static bool _receivedFlag = false;
static String _payloadBuffer = "";
static struct LoRaPacket _rxPacket;

// LoRa Handling

void configureLoRa() {
  Serial.println("configureLoRa()");
#ifdef ENA_TRANSMIT
  LoRa.setTxPowerMax(MAX_TX_POWER);
#endif
#ifdef USE_SPREAD_FACTOR  
  LoRa.setSpreadingFactor(_spreadFactor);
#endif
  LoRa.onReceive(onReceive);
  LoRa.receive();
}

// LoRa receiver

// receiver ISR
//
void onReceive(int packetSize)
{
  // Keep this short and sweet - it's an interrupt service routine
  digitalWrite(LED_BUILTIN, HIGH);
  _payloadBuffer = "";
  while (LoRa.available())
  {
    _payloadBuffer += (char) LoRa.read();
  }
  _receivedFlag = true;
  digitalWrite(LED_BUILTIN, LOW);
}

void setDefaultSpread() {
  LoRa.setSpreadingFactor(MIN_SPREAD_FACTOR);
}

// LoRa transmitter

#ifdef ENA_TRANSMIT

int sendTestPacket() {
  Serial.println("Sending packet "+String(_txCounter));
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(_txCounter++);
  LoRa.endPacket();
}

void sendIfReady() {
  if(millis() - _lastSendTime > SEND_INTERVAL)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    sendTestPacket();
    _lastSendTime = millis();

    clearDisplay();
    displayString(0, 0, "sent packet ");
    displayString(8, 0, (const char *)String(_txCounter-1).c_str());
    displaySpreadFactor(_spreadFactor);
    
    digitalWrite(LED_BUILTIN, LOW);
  }
}

#endif

struct LoRaPacket *checkRxBuffer() {
  if (_receivedFlag && _payloadBuffer.length() > 0) {
    _receivedFlag = false;
    // ensure length does not exceed maximum
    int len = min((int)_payloadBuffer.length(), MAX_LORA_PAYLOAD-1);
    // copy String from rx buffer to packet struct
    _rxPacket.payload = _payloadBuffer;
    ++_rxPacket.index;
    _rxPacket.rssi = LoRa.packetRssi();
    // return ptr to caller
    return &_rxPacket;
  } else {
    return NULL;
  }
}
