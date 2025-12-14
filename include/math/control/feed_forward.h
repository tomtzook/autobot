#pragma once

#include "math/common.h"
#include "units.h"
#include "math/ops.h"

namespace autobot::math {

template<units::unit_or_measure_type unit_>
class simple_feed_forward {
public:
    using unit = units::underlying_unit<unit_>::type;
    using velocity_unit = units::compound_unit<unit, units::unit_inverse<units::units::seconds>>;
    using raw_type = unit::type;
    using type = units::measure<unit>;
    using velocity_type = units::measure<velocity_unit>;
};

}
