
#include "global.h"
#include "hal_obsr.h"

namespace autobot::hal {

template<typename t>
std::string mask_str(const t mask, const char*(*bit_to_str)(t)) {
    std::stringstream ss;
    bool has_one = false;
    for (int i = 0; i < sizeof(mask); i++) {
        if (const auto bit = mask & (1 << i)) {
            const auto str = bit_to_str(bit);

            if (has_one) {
                ss << '|';
            } else {
                has_one = true;
            }

            ss << str;
        }
    }

    return ss.str();
}

obsr::object show_in_obsr(const obsr::object root, const device& device) {
    const auto object = obsr::get_child(root, device.name);

    const auto id_entry = obsr::get_entry(object, "id");
    const auto type_entry = obsr::get_entry(object, "types");

    obsr::set_value(id_entry, obsr::value::make_int32(device.id));
    obsr::set_value(type_entry, obsr::value::make_string(mask_str(device.supported_types, port_type_str)));

    return object;
}

void show_in_obsr(const obsr::object root, const config_def& def, const config_key key) {
    const auto object = obsr::get_child(root, def.name);

    const auto key_entry = obsr::get_entry(object, "key");
    const auto type_entry = obsr::get_entry(object, "type");
    const auto permission_entry = obsr::get_entry(object, "permission");

    obsr::set_value(key_entry, obsr::value::make_int32(key));
    obsr::set_value(type_entry, obsr::value::make_string(data_type_str(def.type)));
    obsr::set_value(permission_entry, obsr::value::make_string(data_permission_str((def.permission))));
}

void show_in_obsr(const obsr::object root, const value_def& def, const value_key key) {
    const auto object = obsr::get_child(root, def.name);

    const auto key_entry = obsr::get_entry(object, "key");
    const auto type_entry = obsr::get_entry(object, "type");
    const auto permission_entry = obsr::get_entry(object, "permission");

    obsr::set_value(key_entry, obsr::value::make_int32(key));
    obsr::set_value(type_entry, obsr::value::make_string(data_type_str(def.type)));
    obsr::set_value(permission_entry, obsr::value::make_string(data_permission_str((def.permission))));
}

}
