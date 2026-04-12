file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

set(SOURCES
        include/autobot/math/common.h
        include/autobot/math/plant/plant.h
        include/autobot/math/plant/dc_motor.h
        include/autobot/units.h
        include/autobot/math/ops.h
        include/autobot/math/control/pid.h
        include/autobot/math/control/trapezoid_profile.h
        include/autobot/math/control/feed_forward.h
        include/autobot/sim/dc_motor.h
        include/autobot/dashboard/object.h
        include/autobot/dashboard/toggle.h
        include/autobot/dashboard/world.h
        include/autobot/dashboard/color.h
        include/autobot/dashboard/canvas.h
        include/autobot/devices/hcsr04.h

        src/math/plant/dc_motor.cpp
        src/sim/dc_motor.cpp
        src/dashboard/object.cpp
        src/dashboard/toggle.cpp
        src/dashboard/canvas.cpp
        src/dashboard/world.cpp
        include/autobot/math/transform.h
)

add_library(autobot STATIC ${SOURCES})
target_include_directories(autobot
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autobot PUBLIC autobot_hal obsr::obsr Eigen3::Eigen PRIVATE looper::looper)
