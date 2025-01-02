#include "CanCommunication.h"
#include <cstring>

CanCommunication::CanCommunication(gpio_num_t tx_io, gpio_num_t rx_io, int baudrate)
    : tx_io(tx_io), rx_io(rx_io), baudrate(baudrate) {}

bool CanCommunication::begin()
{
    twai_general_config_t g_config = {
        .mode = TWAI_MODE_NORMAL,
        .tx_io = tx_io,
        .rx_io = rx_io,
        .clkout_io = ((gpio_num_t)-1),
        .bus_off_io = ((gpio_num_t)-1),
        .tx_queue_len = 5,
        .rx_queue_len = 5,
        .alerts_enabled = TWAI_ALERT_NONE,
        .clkout_divider = 0};
    twai_timing_config_t t_config = twai_timing_config_t();
    switch (baudrate)
    {
    case 1000000:
        t_config = TWAI_TIMING_CONFIG_1MBITS();
        break;
    case 500000:
        t_config = TWAI_TIMING_CONFIG_500KBITS();
        break;
    case 250000:
        t_config = TWAI_TIMING_CONFIG_250KBITS();
        break;
    case 100000:
        t_config = TWAI_TIMING_CONFIG_100KBITS();
        break;
    default:
        return false; // Unsupported baudrate
    }
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    return (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK);
}
bool CanCommunication::start()
{
    return (twai_start() == ESP_OK);
}

bool CanCommunication::transmit(uint32_t identifier, uint8_t *data, size_t len, bool isExtended)
{
    twai_message_t message;
    message.extd = isExtended;
    message.identifier = identifier;
    message.data_length_code = len;
    for (size_t i = 0; i < len; ++i)
    {
        message.data[i] = data[i];
    }

    return (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK);
}

bool CanCommunication::receive(twai_message_t *message, uint32_t waiting_time)
{
    return (twai_receive(message, pdMS_TO_TICKS(waiting_time)) == ESP_OK);
}
