#pragma once

#include "obsr/storage.h"

namespace ui {

class obsr_window {
public:
    obsr_window();

    void update();
    void draw();

private:
    obsr_storage m_storage;
};

}
