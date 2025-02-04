

set(RF_SRCS "")
set(RF_LIBS "")

if(ENABLE_UHD)
  list(APPEND RF_LIBS ${UHD_LIBRARIES})
  list(APPEND RF_SRCS "rf_uhd.cpp")
endif()

if(ENABLE_ZMQ)
  list(APPEND RF_LIBS ${ZeroMQ_LIBRARIES})
  list(APPEND RF_SRCS "rf_zmq.cpp")
endif()

add_library(uuagent_rf SHARED ${RF_SRCS})

target_link_libraries(uuagent_rf ${RF_LIBS} ${YAML_CPP_LIBRARIES})

