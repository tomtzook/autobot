file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

set(SOURCES
        include/hal_sim.h

        src/backend.cpp
        src/sim.cpp
        src/global.h
        src/global.cpp
)

add_library(autobot_hal_sim STATIC ${SOURCES})
target_include_directories(autobot_hal_sim
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autobot_hal_sim autobot_hal obsr::obsr)
