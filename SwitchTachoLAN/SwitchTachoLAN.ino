#include "config.h"
#include "gui.h"
#include "pachube.h"
#include <Bounce.h>
#include <SerialGLCD.h>

// Dependency libraries
#include <SoftwareSerial.h> // Required for the SerialGLCD library
#include <SPI.h> // For Pachube
#include <Ethernet.h> // For Pachube

// Pachube
long paLastConnection;
long paPostingInterval = 60000;

// initialise objects
GUI Gui(LCD_RX_PIN, LCD_TX_PIN);
Pachube pachube(PA_API_KEY);
Bounce TopButton(TOP_BUTTON_PIN, BUTTON_DEBOUNCE);
Bounce BottomButton(BOTTOM_BUTTON_PIN, BUTTON_DEBOUNCE);

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
  DEBUG_PRINT("IP Address: ");
  DEBUG_PRINTLN(Ethernet.localIP());
  
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
    delay(SENSOR_DEBOUNCE);
  }

  pachube.stop();
  if (!pachube.connected() && (millis() - paLastConnection > paPostingInterval)) {
    pachube.sendData(PA_FEED, PA_DATASTREAM, revolutions);
  }
  
  reedPreviousState = reading;
}

