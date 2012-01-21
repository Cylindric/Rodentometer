#ifndef GUI_h
#define GUI_h

#include <inttypes.h>
#include <SoftwareSerial.h>
#include <SerialGLCD.h>

class GUI
{
  private:
    SerialGLCD _glcd;
    uint16_t _lastRevLog;
    uint16_t _revs[5];
    uint8_t _revPtr;
    uint16_t _delta;
    uint8_t _topButtonState;
    uint8_t _bottomButtonState;
    static const uint8_t BUTTON_X = 123;
    static const uint8_t TOP_BUTTON_Y = 15;
    static const uint8_t BOTTOM_BUTTON_Y = 50;
    
    void setupScreen();

  public:
    static const uint8_t TOP_BUTTON = 0;
    static const uint8_t BOTTOM_BUTTON = 1;
    GUI(uint8_t receivePin, uint8_t transmitPin);
    void begin();
    void splash();
    void update(uint16_t revs);
    void setButton(uint8_t button, uint8_t state);
};

#endif
