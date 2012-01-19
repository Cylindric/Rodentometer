#include <SoftwareSerial.h>
#include <SerialGLCD.h>

static const int MODE = 0; // Operation mode: 0=test, 1=sensor
static const int LCD_PIN = 3;   // Pin connected to LCD
static const int REED_PIN = 4;   // Pin connected to reed
static const int REVS_TO_MM = 540; // Circumference of wheel (mm)
static const float REVS_TO_M = 0.54; // Circumference of wheel (m)
static const int SCREEN_DELAY = 200; // Max screen update (ms)
static const int SCREEN_REDRAW = 5000; // Screen redraw period (ms)
static const int DEBOUNCE = 100; // Debounce filter time (ms)
static const int HEIGHT = 64; // Height of LCD (pixels)
static const int WIDTH = 128; // Width of LCD (pixels)
static const int TXT_ROW_0 = 0;
static const int TXT_ROW_1 = 9;

int revolutions = 0;
int reedState = HIGH;
int reedPreviousState = LOW;
bool displayNeedsReset = true;

long time = 0;
long lastScreenUpdate = 0;
long startTime = 0;
long nextGraphPointTime = 0;
int graphPointPtr = 0;
int graphDelta = 0;
int graphLastValue = 0;

SerialGLCD glcd(LCD_PIN, 128, 64);

void setup() {
  pinMode(REED_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Hello");
  glcd.begin();
  glcd.clear();
}

void loop() {
  if (MODE == 0) {
    TestLoop();
  } else {
    MainLoop();
  }
}

void TestLoop() {
  int i = 0;
  while (true) {
    updateDisplay(i);
    delay(200);
    i+=random(0, 6);
  }
}

void MainLoop() {
  int reading = digitalRead(REED_PIN);
  
  if (reading == HIGH && reedPreviousState == LOW) {
    revolutions++;
    Serial.print(millis());
    Serial.print(": ");
    Serial.print(revolutions);
    Serial.print(" = ");
    Serial.print(REVS_TO_M * revolutions);
    Serial.println("m");
    delay(DEBOUNCE);
  }
  
  updateDisplay(revolutions);
  reedPreviousState = reading;
}


void setupDisplay() {
  glcd.clear();
  glcd.gotoXY(0, TXT_ROW_0);
  glcd.drawAscii("Revolutions: ");
  glcd.gotoXY(0, TXT_ROW_1);
  glcd.drawAscii("Meters:      ");
}


void updateDisplay(int revs) {
  
  if (displayNeedsReset) {
    setupDisplay();
    displayNeedsReset = false;
  }
  
  if (startTime == 0) { 
    startTime = millis();
    nextGraphPointTime = startTime + 10000;
    graphPointPtr = 0;
  }
  
  if (millis() >= nextGraphPointTime) {
    graphDelta = revs - graphLastValue;
    glcd.drawLine(graphPointPtr, 64, graphPointPtr, 64-graphDelta);
    graphPointPtr++;
    nextGraphPointTime = millis() + 10000;
    graphLastValue = revs;
  }
  
  if (millis() > (lastScreenUpdate + SCREEN_DELAY)) {
    glcd.gotoXY(80, TXT_ROW_0);
    glcd.drawAscii(revs);
    
    glcd.gotoXY(80, TXT_ROW_1);
    glcd.drawAscii(REVS_TO_M * revs);
    lastScreenUpdate = millis();
  }
}

