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

// transmission parameters
#define SEND_INTERVAL (1000)
static long _lastSendTime = 0;
static int _txSpreadFactor = 7;
static uint32_t _txCounter = 0;

// LoRa receive buffers
static bool _receivedFlag = false;
static String _payloadBuffer = "";
static struct LoRaPacket _rxPacket;

// LoRa Handling

void configureLoRa() {
  Serial.println("configureLoRa()");
  LoRa.setTxPowerMax(20);
  LoRa.setSpreadingFactor(_txSpreadFactor);
  LoRa.onReceive(onReceive);
  LoRa.receive();
}

// LoRa receiver ISR
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



#define MIN_SPREAD_FACTOR (7)   // technically it's 6 but in EU band all channels use 7-12
#define MAX_SPREAD_FACTOR (12)

void setDefaultSpread() {
  LoRa.setSpreadingFactor(MIN_SPREAD_FACTOR);
}
/*
void updateTxMode() {
  if (++_txSpreadFactor > MAX_SPREAD_FACTOR) {
    _txSpreadFactor = MIN_SPREAD_FACTOR;
  }
  LoRa.setSpreadingFactor(_txSpreadFactor);

  Serial.print("set spread factor to ");
  Serial.println(String(_txSpreadFactor));

  clearDisplay();
  displaySpreadFactor(_txSpreadFactor);
}
*/

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
    //setTestSpread();
    sendTestPacket();
    //setDefaultSpread();
    _lastSendTime = millis();

    clearDisplay();
    displayString(0, 0, "sent packet ");
    displayString(8, 0, (const char *)String(_txCounter-1).c_str());
    displaySpreadFactor(_txSpreadFactor);
    
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

void updateLoRa() {
  // ensure LoRa radio is receiving
  LoRa.receive();
}
