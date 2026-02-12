
#include "global.h"

#include "ports.h"

namespace autobot::hal {

std::optional<const port*> lookup_port(const port_id id) {
    auto& global_data = get_global_data();
    if (const auto it = global_data.ports.find(id); it != global_data.ports.end()) {
        return &it->second;
    }

    return std::nullopt;
}

}
