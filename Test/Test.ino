int PIN_HALL = 8;
int PIN_LED = 13;

int hallState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_HALL, INPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.println("Hello");
  delay(1000);
}

void loop() {
  hallState = digitalRead(PIN_HALL);
  digitalWrite(PIN_LED, hallState);
  Serial.print(millis());
  Serial.print(": ");
  Serial.println(hallState);
}
