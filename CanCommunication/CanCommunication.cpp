#include "CanCommunication.h"
#include <cstring>

CanCommunication::CanCommunication(gpio_num_t tx_io, gpio_num_t rx_io, int baudrate)
    : tx_io(tx_io), rx_io(rx_io), baudrate(baudrate) {}

bool CanCommunication::begin()
{
    can_general_config_t g_config = {
        .mode = CAN_MODE_NORMAL,
        .tx_io = tx_io,
        .rx_io = rx_io,
        .clkout_io = ((gpio_num_t)-1),
        .bus_off_io = ((gpio_num_t)-1),
        .tx_queue_len = 5,
        .rx_queue_len = 5,
        .alerts_enabled = CAN_ALERT_NONE,
        .clkout_divider = 0};
    can_timing_config_t t_config = can_timing_config_t();
    switch (baudrate)
    {
    case 1000000:
        t_config = CAN_TIMING_CONFIG_1MBITS();
        break;
    case 500000:
        t_config = CAN_TIMING_CONFIG_500KBITS();
        break;
    case 250000:
        t_config = CAN_TIMING_CONFIG_250KBITS();
        break;
    case 100000:
        t_config = CAN_TIMING_CONFIG_100KBITS();
        break;
    default:
        return false; // Unsupported baudrate
    }
    can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
    return (can_driver_install(&g_config, &t_config, &f_config) == ESP_OK);
}
bool CanCommunication::start()
{
    return (can_start() == ESP_OK);
}

bool CanCommunication::transmit(uint32_t identifier, uint8_t *data, size_t len)
{
    can_message_t message;
    message.identifier = identifier;
    message.flags = CAN_MSG_FLAG_NONE;
    message.data_length_code = len;
    for (size_t i = 0; i < len; ++i)
    {
        message.data[i] = data[i];
    }
    return (can_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK);
}

bool CanCommunication::receive(can_message_t *message)
{
    return (can_receive(message, pdMS_TO_TICKS(100)) == ESP_OK);
}
