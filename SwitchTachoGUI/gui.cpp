#include "Arduino.h"
#include "SerialGLCD.h"
#include <SoftwareSerial.h>
#include "gui.h"

GUI::GUI(uint8_t transmitPin) : _glcd(SerialGLCD(transmitPin))
{  
}

void GUI::begin()
{
  this->_revPtr = 0;
  this->_glcd.begin();
  this->_glcd.setDutyCycle(30);
  this->splash();
  this->setupScreen();
}

void GUI::setupScreen()
{
  // screen border
  this->_glcd.drawRectangle(0, 0, 127, 63);
  
  // data labels
  this->_glcd.drawAscii(2,  2, "Revs:");
  this->_glcd.drawAscii(2, 11, "Meters:");
  this->_glcd.drawAscii(2, 20, "RPM:");
  this->_glcd.drawAscii(2, 29, "kmH:");
}

void GUI::update(uint16_t revs)
{
  uint16_t now = millis();
  if ((now - this->_lastRevLog) > 1000) 
  {
    this->_revs[this->_revPtr] = revs;
    this->_lastRevLog = now;
    
    // calculate the speed
    this->_delta = this->_revs[this->_revPtr] - this->_revs[(this->_revPtr + 1) % 5];
    //speed = 

    this->_revPtr = (this->_revPtr + 1) % 5;
  }
  
  // update the numerical values
  int metres = 0.54 * revs;
  int rpm = this->_delta * 12;
  int rph = rpm * 60;
  int metres_per_hour = 0.54 * rph;
  float km_per_hour = 0.001 * metres_per_hour;
  
  int col = 45;
  this->_glcd.drawAscii(col,  2, revs);
  this->_glcd.drawAscii(col, 11, metres);
  this->_glcd.drawAscii(col, 20, rpm);
  this->_glcd.drawAscii(col, 29, km_per_hour, 2);  
}

void GUI::splash()
{
  delay(1000);
  this->_glcd.drawRectangle(0, 0, 127, 63);
  this->_glcd.drawAscii(25, 20, "Rodentometer!     ");
  delay(2000);
  this->_glcd.drawFilledRectangle(1, 1, 126, 62, 0x00);
}