#include "AR488_Ethernet.h"

#include <ETH.h>

namespace ethernet
{
    Stream& begin(int port)
    {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
        // The argument order changed in esp32-arduino v3+
        ETH.begin(ETH_PHY_LAN8720, 1, 23, 18, 16, ETH_CLOCK_GPIO0_IN);
#else
        ETH.begin(1, 16, 23, 18, ETH_PHY_LAN8720, ETH_CLOCK_GPIO0_IN);
#endif

        
    }
} // namespace ethernet
