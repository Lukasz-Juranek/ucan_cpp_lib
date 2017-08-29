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

      printf("test1");
    if (ucan_can_interface::interface_id == -1) {
      struct sockaddr_can addr;
      struct ifreq ifr;

      printf("test2");
      if ((ucan_can_interface::interface_id =
               socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        printf("Error while opening socket");
        return;
      }

      strcpy(ifr.ifr_name, ifname);
      ioctl(ucan_can_interface::interface_id, SIOCGIFINDEX, &ifr);

      addr.can_family = AF_CAN;
      addr.can_ifindex = ifr.ifr_ifindex;

      printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

      if (bind(ucan_can_interface::interface_id, (struct sockaddr *)&addr,
               sizeof(addr)) < 0) {
        printf("Error in socket bind");
        return;
      }
    }
  }

  uint8_t can_rx(can_frame *frame_rd) {

    int recvbytes = 0;
    struct timeval timeout = {1, 0};
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(ucan_can_interface::interface_id, &readSet);

    if (select((ucan_can_interface::interface_id + 1), &readSet, NULL, NULL, &timeout) >= 0) {
      if (FD_ISSET(ucan_can_interface::interface_id, &readSet)) {
        recvbytes = read(ucan_can_interface::interface_id, frame_rd, sizeof(struct can_frame));
        if (recvbytes) {
          return 1;
        }
      }
    }
    return -1;
  }


  void set_filter(canid_t id, canid_t mask)
  {
      //Add new filter on can-id 0x123
        struct can_filter rfilter[1];
      //   rfilter[0].can_id   = 0x80000123;
         rfilter[0].can_id   = id;
         rfilter[0].can_mask = mask;
         setsockopt(ucan_can_interface::interface_id, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
  }


  void can_send(can_frame frame) {
    std::lock_guard<std::mutex> lock(ucan_can_interface::socket_mutex);
    int nbytes;

    frame.can_id |= CAN_EFF_FLAG;
    nbytes = write(ucan_can_interface::interface_id, &frame,
                   sizeof(struct can_frame));
    //    printf("Wrote %d bytes\n", nbytes);
    //    return nbytes;
  }
};

int ucan_can_interface::interface_id = -1;
std::mutex ucan_can_interface::socket_mutex;

#endif // UCAN_CAN_INTERFACE_H
