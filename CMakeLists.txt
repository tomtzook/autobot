cmake_minimum_required(VERSION 3.28)
project(autobot
        LANGUAGES C CXX
        VERSION "1.0.0")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

include(GNUInstallDirs)

file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

set(SOURCES
        include/math/common.h
        include/math/plant/plant.h
        include/math/plant/dc_motor.h
        include/math/control/pid.h
        include/sim/dc_motor.h
        include/units.h

        src/math/plant/dc_motor.cpp
        src/sim/dc_motor.cpp
        include/math/control/trapezoid_profile.h
)

add_library(autobot STATIC ${SOURCES})
target_include_directories(autobot
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/autobot>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autobot Eigen3::Eigen)

install(TARGETS autobot EXPORT autobot
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES ${INCLUDE_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/autobot)
install(EXPORT autobot
        FILE autobotConfig.cmake
        NAMESPACE autobot::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/autobot")

add_executable(test test.cpp)
target_link_libraries(test autobot)
