file(GLOB INCLUDE_FILES ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/include/*.hpp)

set(SOURCES
        include/hal.h
        include/hal_except.h
        include/hal_actions.h
        include/hal_types.h
        include/backend/hal_control.h
        include/backend/hal_backend.h

        src/hal.cpp
        src/hal_config.cpp
        src/hal_value.cpp
        src/hal_create.cpp
        src/handles.h
        src/global.h
        src/handles.cpp
        src/backend.h
        src/backend.cpp
        src/control.cpp
        src/global.cpp
        src/hal_serial.cpp
        src/hal_obsr.h
        src/hal_obsr.cpp
)

add_library(autobot_hal STATIC ${SOURCES})
target_include_directories(autobot_hal
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(autobot_hal obsr::obsr)
