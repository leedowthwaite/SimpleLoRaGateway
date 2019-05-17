/*
 * HAL.h
 * 
 * Generic HAL interface
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#define HELTEC

#ifdef HELTEC
// LoRa Settings
// From Heltec docs: Replace BAND to fit your hardware and location. e.g. 433E6, 470E6, 868E6, 915E6 
//                   (it’s very important! Wrong working band will damage hardware and may illegal for your location law)
#define LORA_BAND 868e6    // 868MHz

// ESP32 I/O config
// schematic: https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/blob/master/SchematicDiagram/WiFi_LoRa_32(V2)/WIFI_LoRa_32_V2(868-915).PDF
# define LED_BUILTIN (25)
# define BTN_PROG    (0)
#else
# define LED_BUILTIN (2)   // standard ESP32
# define BTN_PROG    (0)
#endif
// Board support interface
void configureBoard();

// Display driver interface
void clearDisplay();
void displayString(int x, int y, const char *str);



 
