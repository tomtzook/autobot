
#include <imgui.h>

#include "obsr.h"

namespace ui::widgets {

static const char* get_type_name(const obsr::value_type type) {
    switch (type) {
        case obsr::value_type::empty: return "empty";
        case obsr::value_type::raw: return "raw";
        case obsr::value_type::string: return "string";
        case obsr::value_type::boolean: return "boolean";
        case obsr::value_type::integer32: return "int32";
        case obsr::value_type::integer64: return "int64";
        case obsr::value_type::floating_point32: return "float";
        case obsr::value_type::floating_point64: return "double";
        case obsr::value_type::integer32_array: return "int32 array";
        case obsr::value_type::integer64_array: return "int64 array";
        case obsr::value_type::floating_point32_array: return "float array";
        case obsr::value_type::floating_point64_array: return "double array";
        default: return "unknown";
    }
}

static bool inputs32(int32_t* value) {
    return ImGui::InputScalar("",
                              ImGuiDataType_S32,
                              value,
                              nullptr, nullptr,
                              "%d",
                              0);
}

static bool inputs64(int64_t* value) {
    return ImGui::InputScalar("",
                              ImGuiDataType_S64,
                              value,
                              nullptr, nullptr,
                              "%ld",
                              0);
}

static bool inputf32(float* value) {
    return ImGui::InputScalar("",
                              ImGuiDataType_Float,
                              value,
                              nullptr, nullptr,
                              "%.3f",
                              0);
}

static bool inputf64(double* value) {
    return ImGui::InputScalar("",
                              ImGuiDataType_Double,
                              value,
                              nullptr, nullptr,
                              "%.3f",
                              0);
}

template<typename t_>
static void draw_arr_readonly(std::span<const t_> arr, const char* pattern) {
    ImGui::Text("[");
    ImGui::SameLine();

    for (int i = 0; i < arr.size(); i++) {
        if (i > 0) {
            ImGui::Text(", ");
            ImGui::SameLine();
        }

        const auto data = arr.data()[i];
        ImGui::Text(pattern, data);
        ImGui::SameLine();
    }

    ImGui::Text("]");
}

static void draw_value_string(const obsr::entry entry, const obsr::value& value) {
    const auto inner_value = value.get_string();
    char value_buffer[256];
    assert(inner_value.size() <= sizeof(value_buffer));

    memcpy(value_buffer, inner_value.data(), inner_value.size());

    if (ImGui::InputText("", value_buffer, sizeof(value_buffer), 0)) {
        std::string new_str(value_buffer);
        obsr::set_value(entry, obsr::value::make_string(new_str));
        printf("value changed string %s\n",value_buffer);
    }
}

static void draw_value_boolean(const obsr::entry entry, const obsr::value& value) {
    static const char* items[] = {"True", "False"};

    const auto inner_value = value.get_boolean();
    int selected_index = inner_value ? 0 : 1;
    if (ImGui::Combo("", &selected_index, items, 2)) {
        const auto new_val = selected_index == 0;
        if (inner_value != new_val) {
            obsr::set_value(entry, obsr::value::make_boolean(new_val));
            printf("value changed boolean %d\n", new_val);
        }
    }
}

static void draw_value_int32(const obsr::entry entry, const obsr::value& value) {
    const auto inner_value = value.get_int32();
    auto current_val = inner_value;
    if (inputs32(&current_val)) {
        if (current_val != inner_value) {
            obsr::set_value(entry, obsr::value::make_int32(current_val));
            printf("value changed int32 %d\n", current_val);
        }
    }
}

static void draw_value_int64(const obsr::entry entry, const obsr::value& value) {
    const auto inner_value = value.get_int64();
    int64_t current_val = inner_value;
    if (inputs64(&current_val)) {
        if (current_val != inner_value) {
            obsr::set_value(entry, obsr::value::make_int64(current_val));
            printf("value changed int64 %ld\n", current_val);
        }
    }
}

static void draw_value_float(const obsr::entry entry, const obsr::value& value) {
    const auto inner_value = value.get_float();
    float current_val = inner_value;
    if (inputf32(&current_val)) {
        if (current_val != inner_value) {
            obsr::set_value(entry, obsr::value::make_float(current_val));
            printf("value changed float %f\n", current_val);
        }
    }
}

static void draw_value_double(const obsr::entry entry, const obsr::value& value) {
    const auto inner_value = value.get_double();
    double current_val = inner_value;
    if (inputf64(&current_val)) {
        if (current_val != inner_value) {
            obsr::set_value(entry, obsr::value::make_double(current_val));
            printf("value changed double %f\n", current_val);
        }
    }
}

obsr_tree::obsr_tree(data::obsr_storage& storage)
    : m_storage(storage)
{}

void obsr_tree::draw() const {
    auto [lock, root] = m_storage.use();
    draw_tree(root);
}

void obsr_tree::draw_tree(const data::obsr_object* root) {
    if (ImGui::BeginTable("obsr_table", 3, ImGuiTableFlags_Borders)) {
        ImGui::TableSetupColumn("name", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("type", ImGuiTableColumnFlags_IndentDisable);
        ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        ImGui::PushID("obsr");
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            draw_content(root);
        }
        ImGui::PopID();

        ImGui::EndTable();
    }
}

void obsr_tree::draw(const data::obsr_object* object) {
    const auto name = obsr::get_name_for_object(object->get_handle());

    ImGui::PushID(name.data());
    {
        if (ImGui::TreeNode(name.data())) {
            set_drag_drop(object);

            draw_content(object);
            ImGui::TreePop();
        } else {
            set_drag_drop(object);
        }
    }
    ImGui::PopID();
}

void obsr_tree::draw_content(const data::obsr_object* object) {
    object->foreach_child([](const auto& child)->void {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);

        draw(child);
    });
    object->foreach_entry([](const auto& entry)->void {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);

        draw(entry);
    });
}

void obsr_tree::draw(const data::obsr_entry* entry) {
    const auto name = obsr::get_name_for_entry(entry->get_handle());

    ImGui::PushID(name.data());
    {
        ImGui::TableSetColumnIndex(0);
        ImGui::Selectable(name.data());
        set_drag_drop(entry);

        const auto& value = entry->get_value();
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", get_type_name(value.get_type()));

        ImGui::TableSetColumnIndex(2);
        draw(entry->get_handle(), value);
    }
    ImGui::PopID();
}

void obsr_tree::draw(const obsr::entry handle, const obsr::value& value) {
    ImGui::PushID("value");
    switch (value.get_type()) {
        case obsr::value_type::string:
            draw_value_string(handle, value);
            break;
        case obsr::value_type::boolean:
            draw_value_boolean(handle, value);
            break;
        case obsr::value_type::integer32:
            draw_value_int32(handle, value);
            break;
        case obsr::value_type::integer64:
            draw_value_int64(handle, value);
            break;
        case obsr::value_type::floating_point32:
            draw_value_float(handle, value);
            break;
        case obsr::value_type::floating_point64:
            draw_value_double(handle, value);
            break;
        case obsr::value_type::integer32_array:
            draw_arr_readonly(value.get_int32_array(), "%d");
            break;
        case obsr::value_type::integer64_array:
            draw_arr_readonly(value.get_int64_array(), "%ld");
            break;
        case obsr::value_type::floating_point32_array:
            draw_arr_readonly(value.get_float_array(), "%.3f");
            break;
        case obsr::value_type::floating_point64_array:
            draw_arr_readonly(value.get_double_array(), "%.3f");
            break;
        case obsr::value_type::raw:
        case obsr::value_type::empty:
        default:
            break;
    }

    ImGui::PopID();
}

void obsr_tree::set_drag_drop(const data::obsr_object* object) {
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const auto handle = object->get_id();
        ImGui::SetDragDropPayload("DATA_SOURCE", &handle, sizeof(handle));
        ImGui::Text("%s", object->get_name().data());
        ImGui::EndDragDropSource();
    }
}

void obsr_tree::set_drag_drop(const data::obsr_entry* entry) {
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        const auto handle = entry->get_id();
        ImGui::SetDragDropPayload("DATA_SOURCE", &handle, sizeof(handle));
        ImGui::Text("%s", entry->get_name().data());
        ImGui::EndDragDropSource();
    }
}

}
