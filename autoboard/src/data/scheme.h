#pragma once

#include <cstdint>

namespace data::scheme {

enum class type {
    unknown,
    boolean,
    number,
    string,
    array,
    canvas_line,
    canvas_rect,
    canvas_circle,
    canvas_group,
    d3_body,
    d3_box
};

struct number {
    float value;
};

struct canvas_line {
    float x;
    float y;
    float length;
    float angle;
    uint32_t color;
};

struct canvas_rect {
    float x;
    float y;
    float length;
    float width;
    uint32_t color;
};

struct canvas_circle {
    float x;
    float y;
    float radius;
    uint32_t color;
};

struct canvas_group {
    float x;
    float y;
};

struct d3_body {
    float x;
    float y;
    float z;
    float pitch;
    float yaw;
    float roll;
};

struct d3_box {
    float x;
    float y;
    float z;
    float pitch;
    float yaw;
    float roll;
    float length;
    float width;
    float height;
    uint32_t color;
};

}
