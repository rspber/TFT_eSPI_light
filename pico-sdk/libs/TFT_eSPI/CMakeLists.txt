add_library(TFT_eSPI INTERFACE)

target_include_directories(TFT_eSPI INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../TFT_eSPI)

target_sources(TFT_eSPI INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../TFT_eSPI/TFT_eSPI.cpp
)

target_link_libraries(TFT_eSPI INTERFACE
  env
  snakes
)
