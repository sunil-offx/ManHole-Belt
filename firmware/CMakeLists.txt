cmake_minimum_required(VERSION 3.16)

# Register extra component directories
set(EXTRA_COMPONENT_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/components/hal
    ${CMAKE_CURRENT_LIST_DIR}/components/sensors
    ${CMAKE_CURRENT_LIST_DIR}/components/processing
    ${CMAKE_CURRENT_LIST_DIR}/components/risk_engine
    ${CMAKE_CURRENT_LIST_DIR}/components/motion
    ${CMAKE_CURRENT_LIST_DIR}/components/ai_inference
    ${CMAKE_CURRENT_LIST_DIR}/components/alerts
    ${CMAKE_CURRENT_LIST_DIR}/components/comms
    ${CMAKE_CURRENT_LIST_DIR}/components/power
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(sentinel_belt)
