add_library(tft INTERFACE)

target_include_directories(tft INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../tft  ${CMAKE_CURRENT_LIST_DIR}/../../../drivers)

target_sources(tft INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../tft/TFT_Write.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tft/TFT_Read.cpp
  ${CMAKE_CURRENT_LIST_DIR}/../../../tft/TFT_Reader.cpp
)

target_link_libraries(tft INTERFACE
  gfx
)
