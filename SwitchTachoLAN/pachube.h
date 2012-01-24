#ifndef Pachube_h
#define Pachube_h

#include <inttypes.h>
#include <SPI.h>
#include <Ethernet.h>

class Pachube
{
  private:
    EthernetClient _client;
    char* _apiKey;
    long _lastConnection;
    int getLength(int data);
    
  public:
    Pachube(char* apiKey);
    void sendData(char* feed, char* datastream, int data);
    bool connected();
    void stop();
};

#endif
