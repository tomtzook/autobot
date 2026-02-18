
add_library(beagleboneblack_defs STATIC pins.h pins.cpp)
target_include_directories(beagleboneblack_defs
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
