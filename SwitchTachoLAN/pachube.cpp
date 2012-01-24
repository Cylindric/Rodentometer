#include "Arduino.h"
#include "pachube.h"

Pachube::Pachube(char* apiKey)
{
  this->_apiKey = apiKey;
}

void Pachube::sendDataMany(char* feed, String data) {
  Serial.print("Sending data to Pachube");

  // if there's a successful connection:
  if (this->_client.connect("api.pachube.com", 80)) {
    Serial.print("sending...");

    // send the HTTP PUT request.
    String request = String("PUT /v2/feeds/");
    request += feed;
    request += ".csv HTTP/1.1\nHost: api.pachube.com\nX-PachubeApiKey: ";
    request += this->_apiKey;
    request += "\nContent-Length: ";
    request += data.length();
    request += "Content-Type: text/csv\nConnection: close\n";
    request += data;

    Serial.println(request);
    this->_client.println(request);
    
    // note the time that the connection was made:
    this->_lastConnection = millis();
    
    Serial.println("sent");
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

void Pachube::sendData(char* feed, char* datastream, int data) {
  String dataString = datastream;
  dataString += ",";
  dataString += data;  
  dataString += "\n";
  this->sendDataMany(feed, dataString);
}

bool Pachube::connected() {
  return this->_client.connected();
}

void Pachube::stop() {
  if(this->_client.connected()) {
    this->_client.stop();
  }
}

// This method calculates the number of digits in the
// sensor reading.  Since each digit of the ASCII decimal
// representation is a byte, the number of digits equals
// the number of bytes:
int Pachube::getLength(int data) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten, 
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = data /10;
  while (dividend > 0) {
    dividend = dividend /10;
    digits++;
  }
  // return the number of digits:
  return digits;
}

