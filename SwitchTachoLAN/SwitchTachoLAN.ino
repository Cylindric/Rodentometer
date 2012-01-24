#include "pachube.h"
#include <SPI.h>
#include <Ethernet.h>

#include <SoftwareSerial.h>
#include <SerialGLCD.h>

#include <Bounce.h>
#include "gui.h"

#define DEBUG
#define TESTING

#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

static const int ETHERNET_SCK_PIN = 13; // reserved for Ethernet shield
static const int ETHERNET_MISO_PIN = 12; // reserved for Ethernet shield
static const int ETHERNET_MOSI_PIN = 11; // reserved for Ethernet shield
static const int ETHERNET_SS = 10; // reserved for Ethernet shield
static byte ETHERNET_MAC[] = {0x90, 0xA2, 0xDA, 0x00, 0xFB, 0xA6};
static const IPAddress ETHERNET_IP(192,168,1,55); // manual IP

static const int SDCARD_SS = 4; // reserved for Ethernet shield

static const int LCD_RX_PIN = 3; // Pin connected to LCD
static const int LCD_TX_PIN = 2; // Pin connected to LCD
static const int TOP_BUTTON_PIN = 7; // Pin connected to the top button
static const int BOTTOM_BUTTON_PIN = 8; // Pin connected to the bottom button
static const int REED_PIN = 9; // Pin connected to reed
static const int DEBOUNCE = 100; // Debounce filter time (ms)

// Pachube
char* paApiKey = "vMZ2_PPD04LpatS9cohU0y_Exu5EwTXV-nTAyG5wx2fJJ81IcnlfJj0aLs2Q2xQeOqHTNjwiQuojQg49xAp2iA615kYR_rpkgQczOBldzpHYmt5PyDp643iHOMVc_VIKQ6JCsaaf1D-KSvBhp8Qg5Q";
char* paFeed = "45482";
char* paDatastream = "test";
long paLastConnection;
long paPostingInterval = 60000;

// initialise objects
GUI Gui(LCD_RX_PIN, LCD_TX_PIN);
Pachube pachube(paApiKey);
Bounce TopButton(TOP_BUTTON_PIN, 5);
Bounce BottomButton(BOTTOM_BUTTON_PIN, 5);

int revolutions = 0;
int reedPreviousState = LOW;


void setup() 
{
  Serial.begin(115200);
  DEBUG_PRINTLN("Hello");
  delay(500);

  // attempt a DHCP connection:
  if (!Ethernet.begin(ETHERNET_MAC)) {
    // if DHCP fails, start with a hard-coded address:
    Ethernet.begin(ETHERNET_MAC, ETHERNET_IP);
  }  
  Serial.print("IP Address: ");
  Serial.println(Ethernet.localIP());
  
  Gui.begin();
}


void loop() {
  TopButton.update();
  BottomButton.update();

#ifdef TESTING
  int reading = (random(100)>90);
#else
  int reading = digitalRead(REED_PIN);
#endif

  if (TopButton.read()) {
    revolutions = 0;
    Gui.resetStatistics();
  }

  Gui.setButton(GUI::TOP_BUTTON, TopButton.read());
  Gui.setButton(GUI::BOTTOM_BUTTON, BottomButton.read());

  if (reading == HIGH && reedPreviousState == LOW) {
    revolutions++;
    Gui.update(revolutions);
    delay(DEBOUNCE);
  }

  pachube.stop();
  if (!pachube.connected() && (millis() - paLastConnection > paPostingInterval)) {
    pachube.sendData(paFeed, paDatastream, revolutions);
  }
  
  reedPreviousState = reading;
}

