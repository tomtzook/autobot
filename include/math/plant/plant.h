#pragma once

#include "math/common.h"

namespace autobot::math {

template<numeric type_, size_t properties_, size_t inputs_, size_t states_, size_t outputs_>
struct plant_data {
    using type = type_;
    using property_mat = Eigen::Matrix<type, properties_, 1>;
    using input_mat = Eigen::Matrix<type, inputs_, 1>;
    using state_mat = Eigen::Matrix<type, states_, 1>;
    using output_mat = Eigen::Matrix<type, outputs_, 1>;

    plant_data() = default;
    plant_data(const plant_data&) = default;
    plant_data(plant_data&&) = default;

    plant_data& operator=(const plant_data&) = default;
    plant_data& operator=(plant_data&&) = default;

    property_mat m_properties;
    input_mat m_input;
    state_mat m_state;
    output_mat m_output;
};

template<typename t_, typename type_, size_t properties_, size_t inputs_, size_t states_, size_t outputs_>
concept plant_system = requires(
    t_ t,
    plant_data<type_, properties_, inputs_, states_, outputs_>& func1_data,
    typename plant_data<type_, properties_, inputs_, states_, outputs_>::type func1_dt) {
    { t.update(func1_data, func1_dt) } -> std::same_as<void>;
};

template<
    numeric type_,
    size_t properties_, size_t inputs_, size_t states_, size_t outputs_,
    plant_system<type_, properties_, inputs_, states_, outputs_> system_>
class plant final {
public:
    using data = plant_data<type_, properties_, inputs_, states_, outputs_>;
    using type = data::type;
    using property_mat = data::property_mat;
    using input_mat = data::input_mat;
    using state_mat = data::state_mat;
    using output_mat = data::output_mat;

    plant() requires(std::is_default_constructible_v<system_>) : m_data() , m_system() {}
    explicit plant(system_&& system) : m_data(), m_system(system) {}

    plant(const plant&) requires(std::is_copy_constructible_v<system_>) = default;
    plant(plant&&) requires(std::is_move_constructible_v<system_>) = default;

    plant& operator=(const plant&) requires(std::is_copy_assignable_v<system_>) = default;
    plant& operator=(plant&&) requires(std::is_move_assignable_v<system_>) = default;

    [[nodiscard]] type property(const size_t index) const { return m_data.m_properties(index, 0); }
    [[nodiscard]] type& property(const size_t index) { return m_data.m_properties(index, 0); }
    [[nodiscard]] type input(const size_t index) const { return m_data.m_input(index, 0); }
    [[nodiscard]] type& input(const size_t index) { return m_data.m_input(index, 0); }
    [[nodiscard]] type state(const size_t index) const { return m_data.m_state(index, 0); }
    [[nodiscard]] type& state(const size_t index) { return m_data.m_state(index, 0); }
    [[nodiscard]] type output(const size_t index) const { return m_data.m_output(index, 0); }

    void update(const type dt) { m_system.update(m_data, dt); }

private:
    data m_data;
    system_ m_system;
};

template<numeric type_, size_t properties_, size_t inputs_, size_t states_, size_t outputs_>
class state_spaced_system {
public:
    using data = plant_data<type_, properties_, inputs_, states_, outputs_>;
    using type = data::type;
    using mat_A = Eigen::Matrix<type, states_, states_>;
    using mat_B = Eigen::Matrix<type, states_, inputs_>;
    using mat_C = Eigen::Matrix<type, outputs_, states_>;
    using mat_D = Eigen::Matrix<type, outputs_, inputs_>;

    state_spaced_system(mat_A&& A, mat_B&& B, mat_C&& C, mat_D&& D)
        : m_A(A)
        , m_B(B)
        , m_C(C)
        , m_D(D)
    {}

    void update(data& data, const type dt) {
        const auto d_state = m_A * data.m_state + m_B * data.m_input;
        data.m_output = m_C * data.m_state + m_D * data.m_input;

        data.m_state += d_state * dt;
    }

private:
    mat_A m_A;
    mat_B m_B;
    mat_C m_C;
    mat_D m_D;
};

}
