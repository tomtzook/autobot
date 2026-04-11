#pragma once

#include <cstdint>

namespace autobot::dashboard {

using color = int32_t; // todo: uint

static constexpr color rgba(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) noexcept {
    return (static_cast<color>(r) << 0) | (static_cast<color>(g) << 8) | (static_cast<color>(b) << 16) | (static_cast<color>(a) << 24);
}

}