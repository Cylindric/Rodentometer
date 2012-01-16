int latchPin = 8;  // Pin connected to ST_CP of 74HC595 (12)
int clockPin = 12; // Pin connected to SH_CP of 74HC595 (11)
int dataPin = 11;  // Pin connected to DS of 74HC595 (14)
int reedPin = 7;   // Pin connected to reed

int revolutions = 0;
int reedState = HIGH;
int reedPreviousState = LOW;

long time = 0;
long debounce = 100;

const int REVS_TO_MM = 540;
const float REVS_TO_M = 0.54;
const int COUNTER_STYLE = 1; // 0=revs, 1=m

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(reedPin, INPUT);
  Serial.println("Hello");
}


void loop() {
  TestLoop();
}

void TestLoop() {
  for (int j = 0; j < 512; j++) {
    updateDisplay(j);
    delay(10);
  }
}

void MainLoop() {
  int reading = digitalRead(reedPin);
  
  if (reading == HIGH && reedPreviousState == LOW) {
    revolutions++;
    Serial.print(millis());
    Serial.print(": ");
    Serial.print(revolutions);
    Serial.print(" = ");
    Serial.print(REVS_TO_M * revolutions);
    Serial.println("m");
    delay(debounce);
  }
  
  updateDisplay(revolutions);
  reedPreviousState = reading;
}


void updateDisplay(int revs) {
  int numberToDisplay = revs;
  switch(COUNTER_STYLE) {
    case 1:
      numberToDisplay = REVS_TO_M * revs;
      break;
  }

  myShiftOut(numberToDisplay);
}


void myShiftOut(int data) {
  int i = 0;
  int pinState;
  
  digitalWrite(latchPin, LOW);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  
  for (i=7; i>=0; i--) {
    digitalWrite(clockPin, LOW);
    
    if (data & (1<<i)) {
      pinState = HIGH;
    }
    else {
      pinState = LOW
    }
    
    digitalWrite(dataPin, pinState);
    
    digitalWrite(clockPin, HIGH);
    digitalWrite(dataPin, LOW);
  }
  
  digitalWrite(latchPin, HIGH);
}
