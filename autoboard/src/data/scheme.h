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

}
