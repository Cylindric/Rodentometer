#include <SoftwareSerial.h>

const int LCD_PIN = 3;   // Pin connected to LCD
const int REED_PIN = 4;   // Pin connected to reed
const int REVS_TO_MM = 540; // Circumference of wheel (mm)
const float REVS_TO_M = 0.54; // Circumference of wheel (m)
const int SCREEN_DELAY = 1000; // Max screen update (ms)
const int SCREEN_REDRAW = 5000; // Screen redraw period (ms)
const int DEBOUNCE = 100; // Debounce filter time (ms)
const int HEIGHT = 64; // Height of LCD (pixels)
const int WIDTH = 128; // Width of LCD (pixels)

int revolutions = 0;
int reedState = HIGH;
int reedPreviousState = LOW;

long time = 0;
long lastScreenUpdate = 0;
long lastScreenRedraw = 0;

SoftwareSerial glcd(0, LCD_PIN);

void setup() {
  pinMode(REED_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Hello");
  glcd.begin(115200);
  clearLCD();
}


void loop() {
  //TestLoop();
  MainLoop();
}

void TestLoop() {
  for (int j = 0; j < 1024; j++) {
    updateDisplay(j);
    delay(10);
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


void updateDisplay(int revs) {
  
  if (millis() > (lastScreenRedraw + SCREEN_REDRAW)) {
    clearLCD();
    lastScreenRedraw = millis();
  }
  
  if (millis() > (lastScreenUpdate + SCREEN_DELAY)) {
    setXY(0, 8);
    glcd.print(revs);
    glcd.print(" revolutions    ");
    
    setXY(0, 20);
    glcd.print(REVS_TO_M * revs);
    glcd.print("m     ");
    lastScreenUpdate = millis();
  }
}


/* LCD Functions */
void setXY(byte x, byte y) {
  setX(x);
  setY(y);
}


void setX(byte x) {
  glcd.write(0x7C); // Command
  glcd.write(0x18); // Set X
  glcd.write(x); // X-coord
}


void setY(byte y) {
  glcd.write(0x7C); // Command
  glcd.write(0x19); // Set Y
  glcd.write(y); // Y-coord
}


void drawBox(byte x1, byte y1, byte x2, byte y2) {
  glcd.write(0x7C); // Command
  glcd.write(0x0F); // Box
  glcd.write(x1); // X-coord 1
  glcd.write(y1); // Y-coord 1
  glcd.write(x2); // X-coord 2
  glcd.write(y2); // Y-coord 2
  glcd.write(0x01); // Set
}


void clearLCD() {
  glcd.write(0x7C); // Command
  glcd.write((byte)0); // Clear
}


void demo() {
  glcd.write(0x7C); // Command
  glcd.write(0x04); // Demo
}


void reverse() {
  glcd.write(0x7C); // Command
  glcd.write(0x12); // Demo
}
