#pragma once

#include <unordered_map>
#include <string_view>
#include <string>
#include <implot.h>

namespace ui::plots {

struct xy_plot_data {
    xy_plot_data();

    void add(float x, float y);
    void show_plot_line(const char* label, float weight) const;

private:
    static constexpr int measurement_count = 20000;

    float x_data[measurement_count];
    float y_data[measurement_count];
    int current_index;
};

class xy_plot {
public:
    explicit xy_plot(std::string_view label);

    [[nodiscard]] const char* name() const;

    void setup_x_axis(const char* label, float min_initial, float max_initial);
    void setup_y_axis(const char* label, float min_initial, float max_initial);

    int create_plot(std::string_view label, float line_weight);
    void add_data(int plot_id, float x, float y);

    void update(float x);
    void draw() const;

private:
    struct sub_plot {
        sub_plot();
        sub_plot(std::string_view label, float line_weight);

        void add(float x, float y);

        void draw() const;

    private:
        std::string m_label;
        float m_line_weight;
        xy_plot_data m_data;
    };

    std::unordered_map<int, sub_plot> m_data;
    std::string m_label;
    const char* m_x_label;
    const char* m_y_label;
    float m_x_min;
    float m_x_max;
    float m_y_min;
    float m_y_max;
    int m_next_plot_id;
};

}
