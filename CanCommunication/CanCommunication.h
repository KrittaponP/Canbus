#ifndef CAN_COMMUNICATION_H
#define CAN_COMMUNICATION_H

#include "driver/gpio.h"
#include "driver/twai.h"

class CanCommunication
{
public:
    CanCommunication(gpio_num_t tx_io, gpio_num_t rx_io, int baudrate = 500000);
    bool begin();
    bool start();
    bool transmit(uint32_t identifier, uint8_t *data, size_t len, bool isExtended);
    bool receive(twai_message_t *message, uint32_t waiting_time);

private:
    gpio_num_t tx_io;
    gpio_num_t rx_io;
    int baudrate;
};

#endif
