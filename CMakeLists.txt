cmake_minimum_required(VERSION 3.28)
project(autobot
        LANGUAGES C CXX
        VERSION "1.0.0")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(looper REQUIRED HINTS /home/tomtzook/.local)
find_package(obsr REQUIRED HINTS /home/tomtzook/.local)

include(GNUInstallDirs)

add_subdirectory(autobot)
add_subdirectory(autoboard)

add_executable(test_robot robot.cpp)
target_link_libraries(test_robot autobot)
