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

static const int MODE = 1; // Operation mode: 0=test, 1=sensor

static const int LCD_RX_PIN = 7; // Pin connected to LCD
static const int LCD_TX_PIN = 6; // Pin connected to LCD
static const int TOP_BUTTON_PIN = 9; // Pin connected to the top button
static const int BOTTOM_BUTTON_PIN = 8; // Pin connected to the bottom button
static const int SENSOR_PIN = 3; // Pin connected to the Hall sensor (interrupt pin)
static const int DEBOUNCE = 20; // Debounce filter time (ms)

static const int GUI_DELAY_MS = 40; // Delay between frames (ms)

GUI Gui(LCD_RX_PIN, LCD_TX_PIN);
Bounce TopButton(TOP_BUTTON_PIN, 5);
Bounce BottomButton(BOTTOM_BUTTON_PIN, 5);

volatile int revolutions = 0;
volatile long lastRevolutionTrigger = 0;
long lastFrameUpdate = 0;

void setup() 
{
  Serial.begin(115200);
  DEBUG_PRINTLN("Hello");
  delay(500);
  Gui.begin();
  attachInterrupt(1, SensorInterrupt, FALLING);
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
  int reading = digitalRead(SENSOR_PIN);
#endif

  if (TopButton.read()) {
    revolutions = 0;
    Gui.resetStatistics();
  }

  Gui.setButton(GUI::TOP_BUTTON, TopButton.read());
  Gui.setButton(GUI::BOTTOM_BUTTON, BottomButton.read());
  if (millis() - lastFrameUpdate > GUI_DELAY_MS) {
    Gui.update(revolutions);
    lastFrameUpdate = millis();
  }
}


void SensorInterrupt() {
  if (millis()-lastRevolutionTrigger > DEBOUNCE) {
    revolutions++;
    lastRevolutionTrigger = millis();
    digitalWrite(13, revolutions%2);
  }
}

