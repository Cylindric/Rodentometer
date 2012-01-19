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
    void setupScreen();

  public:
    GUI(uint8_t transmitPin);
    void begin();
    void splash();
    void update(uint16_t revs);
};

#endif
