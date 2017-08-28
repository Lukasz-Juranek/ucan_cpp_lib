#ifndef UCAN_TOOLS_H
#define UCAN_TOOLS_H

#include "ucan_net.h"
#include "ucan_commands_manager.h"
#include <chrono>
#include <deque>
#include <string>
#include <linux/can.h>
#include <vector>
#include <ctime>

class ucan_tools{

public:
    struct device_table_entry {
        uCANnetID id;
        time_t activity_time;
    };

    static std::map <uint32_t,device_table_entry> active_devices;

    static void scan_for_devices(int scantime_ms){
        can_frame buffer;
        ucan_can_interface can_sock = ucan_can_interface("vcan0");
        while ( scantime_ms --)
        {
           if (can_sock.can_rx(&buffer) == 1)
           {

               uCANnetID map_id;
               map_id.whole =  buffer.can_id;
               map_id.frame_type = 0;
               map_id.group = 0;
               map_id.mcast = 0;
               map_id.unused = 0;

               ucan_tools::active_devices[map_id.whole] = {buffer.can_id, time(0)};
           }
           std::this_thread::sleep_for(1ms);
        }
    }
};

std::map <uint32_t,ucan_tools::device_table_entry> ucan_tools::active_devices;

#endif // UCAN_TOOLS_H

