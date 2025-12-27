#pragma once

#include "data/obsr_storage.h"

namespace ui::widgets {

class obsr_tree {
public:
    explicit obsr_tree(data::obsr_storage& storage);

    void draw() const;

private:
    static void draw_tree(const data::obsr_object* root);
    static void draw(const data::obsr_object* object);
    static void draw_content(const data::obsr_object* object);
    static void draw(const data::obsr_entry* entry);
    static void draw(obsr::entry handle, const obsr::value& value);
    static void set_drag_drop(const data::obsr_object* object);
    static void set_drag_drop(const data::obsr_entry* entry);

    data::obsr_storage& m_storage;
};

}
