#pragma once

#include <cstdint>

namespace data {

enum class scheme {
    unknown,
    boolean,
    number,
    string,
    array,
    canvas_line
};

struct number_scheme {
    float value;
};

struct canvas_line_scheme {
    float x;
    float y;
    float length;
    float angle;
};

}
