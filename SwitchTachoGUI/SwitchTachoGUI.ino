#include <SoftwareSerial.h>
#include <SerialGLCD.h>
#include "gui.h"

static const int MODE = 0; // Operation mode: 0=test, 1=sensor
static const int LCD_PIN = 3;   // Pin connected to LCD
static const int REED_PIN = 4;   // Pin connected to reed
static const int DEBOUNCE = 100; // Debounce filter time (ms)

GUI gui(LCD_PIN);
int revolutions = 0;
int reedPreviousState = LOW;

void setup() 
{
  delay(500);
  gui.begin();
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
  gui.update(revolutions);
  revolutions = revolutions + random(0, 5);
}

void MainLoop() {
  int reading = digitalRead(REED_PIN);
  
  if (reading == HIGH && reedPreviousState == LOW) {
    revolutions++;
    delay(DEBOUNCE);
  }

  gui.update(revolutions);
  reedPreviousState = reading;
  delay(1000);
}
