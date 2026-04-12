add_library(autobot_sim STATIC
        include/autobot_sim/devices/hcsr04.h
        include/autobot_sim/dynamics/data.h

        src/devices/hcsr04.cpp
        include/autobot_sim/dynamics/body.h
        src/dynamics/body.cpp
        include/autobot_sim/dynamics/engine/base.h
        include/autobot_sim/dynamics/engine/skeleton.h
        include/autobot_sim/dynamics/engine/raycast.h
        include/autobot_sim/dynamics/engine/world.h
        src/dynamics/engine/world.cpp
        src/dynamics/engine/raycast.cpp
        src/dynamics/engine/skeleton.cpp
        src/dynamics/engine/info.h
        src/dynamics/engine/holders.h
)
target_include_directories(autobot_sim
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/autobot_sim>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${BULLET_INCLUDE_DIRS})
target_link_libraries(autobot_sim PUBLIC autobot autobot_hal autobot_hal_sim dart dart-collision-bullet ${BULLET_LIBRARIES})

add_executable(autobot_sim_main temp_main.cpp)
target_include_directories(autobot_sim_main
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${BULLET_INCLUDE_DIRS})
target_link_libraries(autobot_sim_main autobot_sim autobot autobot_hal_sim Eigen3::Eigen glui)
