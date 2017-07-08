#ifndef UCAN_CAN_INTERFACE_H
#define UCAN_CAN_INTERFACE_H

#include <linux/can.h>
#include <linux/can/raw.h>
#include <mutex>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "ucan_commands_manager.h"

class ucan_can_interface {

private:
  static int interface_id;
  static std::mutex socket_mutex; // protects socket write

public:
  ucan_can_interface(const char *ifname) {

    if (ucan_can_interface::interface_id == -1) {
      struct sockaddr_can addr;
      struct ifreq ifr;

      if ((ucan_can_interface::interface_id =
               socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket");
        return;
      }

      strcpy(ifr.ifr_name, ifname);
      ioctl(ucan_can_interface::interface_id, SIOCGIFINDEX, &ifr);

      addr.can_family = AF_CAN;
      addr.can_ifindex = ifr.ifr_ifindex;

      printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

      if (bind(ucan_can_interface::interface_id, (struct sockaddr *)&addr,
               sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return;
      }
    }
  }

  void can_send(can_frame frame) {
    std::lock_guard<std::mutex> lock(ucan_can_interface::socket_mutex);
    int nbytes;

    frame.can_id |= CAN_EFF_FLAG;
    nbytes = write(ucan_can_interface::interface_id, &frame,
                   sizeof(struct can_frame));
    printf("Wrote %d bytes\n", nbytes);
    //    return nbytes;
  }
};

int ucan_can_interface::interface_id = -1;
std::mutex ucan_can_interface::socket_mutex;

#endif // UCAN_CAN_INTERFACE_H
