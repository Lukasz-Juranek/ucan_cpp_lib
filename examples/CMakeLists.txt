set(ucan_discover_SRCS
   ucan_discover.cpp
)
add_executable(ucan_discover
  ${ucan_discover_SRCS}
)


set (ucan_receiver_SRCS
    ucan_receiver.cpp
)
add_executable(ucan_receiver
  ${ucan_receiver_SRCS}
)

