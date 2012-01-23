#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>

#include <SoftwareSerial.h>
#include <SerialGLCD.h>
#include <Bounce.h>
#include "gui.h"

#define DEBUG
//#define TESTING

#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

static const int MODE = 0; // Operation mode: 0=test, 1=sensor

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
static const char* PA_API_KEY = "vMZ2_PPD04LpatS9cohU0y_Exu5EwTXV-nTAyG5wx2fJJ81IcnlfJj0aLs2Q2xQeOqHTNjwiQuojQg49xAp2iA615kYR_rpkgQczOBldzpHYmt5PyDp643iHOMVc_VIKQ6JCsaaf1D-KSvBhp8Qg5Q";
static const char* PA_FEED = "45482";
static const char* PA_DATASTREAM = "test";
long paLastConnection;
long paPostingInterval = 15000;

// initialise objects
EthernetClient Network;
GUI Gui(LCD_RX_PIN, LCD_TX_PIN);
Bounce TopButton(TOP_BUTTON_PIN, 5);
Bounce BottomButton(BOTTOM_BUTTON_PIN, 5);

int revolutions = 0;
int reedPreviousState = LOW;


void setup() 
{
  Serial.begin(115200);
  DEBUG_PRINTLN("Hello");
  delay(500);

  if (Ethernet.begin(ETHERNET_MAC) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection:
  if (Ethernet.begin(ETHERNET_MAC) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Configure manually:
    Ethernet.begin(ETHERNET_MAC, ETHERNET_IP);
  }

  Gui.begin();
}


void loop() {
  if (MODE == 0) {
    TestLoop();
  } else {
    MainLoop();
  }
}


void TestLoop() 
{
  Gui.update(revolutions);
  revolutions = revolutions + random(0, 5);
}


void MainLoop() {
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

  if (!Network.connected() && paLastConnection) {
    Serial.println();
    Serial.println("Disconnecting");
    Network.stop();
  }
  
  if (!Network.connected() && (millis() - paLastConnection > paPostingInterval)) {
    sendData(revolutions);
  }
  
  reedPreviousState = reading;
}


void sendData(int thisData) {
  Serial.print("Sending data to Pachube... ");
  Serial.println(thisData);

  // if there's a successful connection:
  if (Network.connect("api.pachube.com", 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request. 
    Network.print("PUT /v2/feeds/");
    Network.print(PA_FEED);
    Network.print("/datastreams/");
    Network.print(PA_DATASTREAM);
    Network.print(".csv HTTP/1.1\n");
    Network.print("Host: www.pachube.com\n");
    Network.print("X-PachubeApiKey: ");
    Network.print(PA_API_KEY);
    Network.print("\n");
    Network.print("Content-Length: ");

    // calculate the length of the sensor reading in bytes:
    int thisLength = getLength(thisData);
    Network.println(thisLength, DEC);

    // last pieces of the HTTP PUT request:
    Network.print("Content-Type: text/csv\n");
    Network.println("Connection: close\n");

    // here's the actual content of the PUT request:
    Network.println(thisData, DEC);

    // note the time that the connection was made:
    paLastConnection = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


// This method calculates the number of digits in the
// sensor reading.  Since each digit of the ASCII decimal
// representation is a byte, the number of digits equals
// the number of bytes:
int getLength(int someValue) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten, 
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue /10;
  while (dividend > 0) {
    dividend = dividend /10;
    digits++;
  }
  // return the number of digits:
  return digits;
}

