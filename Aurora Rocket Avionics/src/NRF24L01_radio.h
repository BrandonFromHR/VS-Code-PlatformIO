#include "RF24.h"

struct Packet
{
    uint8_t id;
    uint8_t data_uint8[3];
    float data_float[7];
};

class NRF24L01_radio : public RF24
{
private:
    Packet packet;
public:
    void setupTransmitter();
    void setupReceiver();
    void transmit();
    void receive();
    void sortOutgoingData(uint8_t id);
    void sortIncomingData();
};