file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

set(SOURCES
        include/math/common.h
        include/math/plant/plant.h
        include/math/plant/dc_motor.h
        include/units.h
        include/math/ops.h
        include/math/control/pid.h
        include/math/control/trapezoid_profile.h
        include/math/control/feed_forward.h
        include/sim/dc_motor.h
        include/dashboard/object.h
        include/dashboard/toggle.h
        include/devices/hcsr04.h

        src/math/plant/dc_motor.cpp
        src/sim/dc_motor.cpp
        src/dashboard/object.cpp
        src/dashboard/toggle.cpp
        include/dashboard/canvas.h
        src/dashboard/canvas.cpp
        include/dashboard/world.h
        include/dashboard/color.h
        src/dashboard/world.cpp
)

add_library(autobot STATIC ${SOURCES})
target_include_directories(autobot
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/autobot>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autobot PUBLIC autobot_hal obsr::obsr Eigen3::Eigen PRIVATE looper::looper)
