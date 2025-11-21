#include "AR488_Ethernet.h"

#include <ETH.h>
#include "src/esp_eth_console/eth_console_vfs.h"

EthernetStream::EthernetStream(FILE *input, FILE *output) : input_file(input), output_file(output)
{
    setTimeout(20);
    fcntl(fileno(input_file), F_SETFL, O_NONBLOCK);
}

int EthernetStream::available()
{
    return eth_console_vfs::get_available();
};

int EthernetStream::peek()
{
    int ret = fgetc(input_file);
    ungetc(ret, input_file);
    return ret;
};

int EthernetStream::read()
{
    return fgetc(input_file);
};

void EthernetStream::flush()
{
    fflush(output_file);
};

size_t EthernetStream::write(const uint8_t data)
{
    return (fputc(data, output_file) != EOF) ? 1 : 0;
}

size_t EthernetStream::write(const uint8_t *buffer, size_t size)
{
    return fwrite(buffer, sizeof(uint8_t), size, output_file);
}

namespace ethernet
{
    EthernetStream* command_stream;

    Stream &begin()
    {
        assert(!command_stream);

#if ESP_ARDUINO_VERSION_MAJOR >= 3
        // The argument order changed in esp32-arduino v3+
        ETH.begin(ETH_PHY_LAN8720, 1, 23, 18, 16, ETH_CLOCK_GPIO0_IN);
#else
        ETH.begin(1, 16, 23, 18, ETH_PHY_LAN8720, ETH_CLOCK_GPIO0_IN);
#endif

        eth_console_vfs::init_console();
        eth_console_vfs::set_rx_line_endings(ESP_LINE_ENDINGS_LF);
        eth_console_vfs::set_tx_line_endings(ESP_LINE_ENDINGS_LF);
        FILE *input, *output;
        eth_console_vfs::get_streams(&input, &output);
        command_stream = new EthernetStream(input, output);
        return *command_stream;
    }
} // namespace ethernet
