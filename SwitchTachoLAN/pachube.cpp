#include "Arduino.h"
#include "pachube.h"

Pachube::Pachube(char* apiKey)
{
  this->_apiKey = apiKey;
}

void Pachube::sendData(char* feed, char* datastream, int data) {
  Serial.print("Sending data to Pachube: ");
  Serial.println(data);

  // if there's a successful connection:
  if (this->_client.connect("api.pachube.com", 80)) {
    Serial.print("sending...");
    
    // calculate the length of the sensor reading in bytes:
    int dataLength = this->getLength(data);

    // send the HTTP PUT request. 
    this->_client.print("PUT /v2/feeds/");
    this->_client.print(feed);
    this->_client.print("/datastreams/");
    this->_client.print(datastream);
    this->_client.print(".csv HTTP/1.1\n");
    this->_client.print("Host: api.pachube.com\n");
    this->_client.print("X-PachubeApiKey: ");
    this->_client.print(this->_apiKey);
    this->_client.print("\n");
    this->_client.print("Content-Length: ");
    this->_client.println(dataLength, DEC);
    this->_client.print("Content-Type: text/csv\n");
    this->_client.println("Connection: close\n");

    // here's the actual content of the PUT request:
    this->_client.println(data, DEC);

    // note the time that the connection was made:
    this->_lastConnection = millis();
    
    Serial.println("sent");
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
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

