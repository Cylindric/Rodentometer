#include "Arduino.h"
#include "SerialGLCD.h"
#include <SoftwareSerial.h>
#include "gui.h"

GUI::GUI(uint8_t receivePin, uint8_t transmitPin) : _glcd(SerialGLCD(receivePin, transmitPin))
{
}

void GUI::begin()
{
  this->_glcd.begin();
  this->_glcd.setDutyCycle(30);
  this->splash();
  this->resetStatistics();
  this->setupScreen();
  this->_topButtonState = 0;
  this->_bottomButtonState = 0;
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
  this->_glcd.drawAscii(2, 38, "Max RPM:");
  this->_glcd.drawAscii(2, 47, "Max kmH:");

  this->_glcd.drawCircle(this->BUTTON_X, this->TOP_BUTTON_Y, 2, 1);
  this->_glcd.drawCircle(this->BUTTON_X, this->BOTTOM_BUTTON_Y, 2, 1);
  this->setButton(GUI::TOP_BUTTON, 0);
  this->setButton(GUI::BOTTOM_BUTTON, 0);
}

void GUI::update(uint16_t revs)
{
  uint16_t now = millis();
  bool revsHaveChanged = !(this->_revs[this->_revPtr] == revs);
  bool speedHasChanged = false;
  
  if ((now - this->_lastRevLog) > 1000) 
  {
    this->_revs[this->_revPtr] = revs;
    this->_lastRevLog = now;
    
    // calculate the speed
    this->_delta = this->_revs[this->_revPtr] - this->_revs[(this->_revPtr + 1) % 5];
    speedHasChanged = !(this->_deltas[this->_revPtr] == this->_delta);

    this->_revPtr = (this->_revPtr + 1) % 5;
  }
  

  // update the numerical values
  int metres = 0.54 * revs;
  int rpm = this->_delta * 12;
  bool peakRpmHasChanged = false;
  if (rpm > this->_peakRPM) {
    this->_peakRPM = rpm;
    peakRpmHasChanged = true;
  }
  int rph = rpm * 60;
  int metres_per_hour = 0.54 * rph;
  float km_per_hour = 0.001 * metres_per_hour;
  float peak_kph = 0.001 * 0.54 * 60 * this->_peakRPM;
  
  int col = 50;
  if (revs == 0) {
    this->_glcd.drawAscii(col,  2, "0        ");
    this->_glcd.drawAscii(col, 11, "0        ");
    this->_glcd.drawAscii(col, 20, "0        ");
    this->_glcd.drawAscii(col, 29, "0        ");
    this->_glcd.drawAscii(col, 38, "0        ");
    this->_glcd.drawAscii(col, 47, "0        ");
  } else {
    if (revsHaveChanged) {
      this->_glcd.drawAscii(col,  2, revs);
      this->_glcd.drawAscii(col, 11, metres);
    }
    if (speedHasChanged) {
      this->_glcd.drawAscii(col, 20, rpm);
      this->_glcd.drawAscii(col, 29, km_per_hour, 2);
    }
    if (peakRpmHasChanged) {
      this->_glcd.drawAscii(col, 38, this->_peakRPM);
      this->_glcd.drawAscii(col, 47, peak_kph, 2);
    }
  }
}


void GUI::resetStatistics() {
  this->_revPtr = 0;  
  this->_lastRevLog = 0;
  this->_peakRPM = 0;
  for (uint8_t i = 0; i < 5; i++) {
    this->_revs[i] = 0;
    this->_deltas[i] = 0;
  }
}


void GUI::setButton(uint8_t button, uint8_t state)
{
  bool stateChanged = false;
  uint8_t x = this->BUTTON_X;
  uint8_t y = 0;

  switch (button)
  {
    case (GUI::TOP_BUTTON):
      y = this->TOP_BUTTON_Y;
      stateChanged = (this->_topButtonState != state);
      this->_topButtonState = state;
      break;

    case GUI::BOTTOM_BUTTON:
      y = this->BOTTOM_BUTTON_Y;
      stateChanged = (this->_bottomButtonState != state);
      this->_bottomButtonState = state;
      break;
  }

  if (stateChanged)
  {
    this->_glcd.drawRectangle(x-1, y-1, x+1, y+1, state);
    this->_glcd.drawPixel(x, y, state);
  }
}


void GUI::splash()
{
  delay(1000);
  this->_glcd.drawRectangle(0, 0, 127, 63);
  this->_glcd.drawAscii(25, 20, "Rodentometer!");
  delay(2000);
  this->_glcd.drawFilledRectangle(1, 1, 126, 62, 0x00);
}
