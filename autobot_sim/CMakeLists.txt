add_library(autobot_sim STATIC src/empty.cpp
        include/devices/hcsr04.h
        include/transform.h
        include/dynamics/engine.h
        include/dynamics/data.h
        include/dynamics/body.h

        src/devices/hcsr04.cpp
        src/dynamics/data.cpp
        src/dynamics/body.cpp
)
target_include_directories(autobot_sim
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/autobot_sim>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autobot_sim PUBLIC autobot autobot_hal autobot_hal_sim dart)

add_executable(autobot_sim_main temp_main.cpp)
target_include_directories(autobot_sim_main
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autobot_sim_main autobot_sim autobot autobot_hal_sim Eigen3::Eigen glui)
