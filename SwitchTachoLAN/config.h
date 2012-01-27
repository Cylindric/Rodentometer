#ifndef Config_h
#define Config_h

#include <Ethernet.h>

/******************************************
 *                PINS                    *
 ******************************************/
// Ethernet shield
static const int ETHERNET_SCK_PIN = 13; // reserved for Ethernet shield
static const int ETHERNET_MISO_PIN = 12; // reserved for Ethernet shield
static const int ETHERNET_MOSI_PIN = 11; // reserved for Ethernet shield
static const int ETHERNET_SS = 10; // reserved for Ethernet shield
static const int SDCARD_SS = 4; // reserved for Ethernet shield

// User interface
static const int LCD_RX_PIN = 2; // Pin connected to LCD TX
static const int LCD_TX_PIN = 3; // Pin connected to LCD RX
static const int TOP_BUTTON_PIN = 7; // Pin connected to the top button
static const int BOTTOM_BUTTON_PIN = 8; // Pin connected to the bottom button

// Sensors
static const int REED_PIN = 9; // Pin connected to reed

/******************************************
 *              NETWORK SETUP             *
 ******************************************/
static byte ETHERNET_MAC[] = {0x90, 0xA2, 0xDA, 0x00, 0xFB, 0xA6};
static const IPAddress ETHERNET_IP(192,168,1,55); // manual IP fallback address

/******************************************
 *                PACHUBE                 *
 ******************************************/
char* PA_API_KEY = "vMZ2_PPD04LpatS9cohU0y_Exu5EwTXV-nTAyG5wx2fJJ81IcnlfJj0aLs2Q2xQeOqHTNjwiQuojQg49xAp2iA615kYR_rpkgQczOBldzpHYmt5PyDp643iHOMVc_VIKQ6JCsaaf1D-KSvBhp8Qg5Q";
char* PA_FEED = "45482";
char* PA_DATASTREAM = "test";

/******************************************
 *             OTHER SETTINGS             *
 ******************************************/
static const int SENSOR_DEBOUNCE = 100; // Debounce filter time (ms)
static const int BUTTON_DEBOUNCE = 5; // Debounce filter time (ms)

#define DEBUG
//#define TESTING

#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif


#endif
