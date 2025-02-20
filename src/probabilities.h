//
//          Python - Recurrence Microstates Library (MicroRecPy)
//          Created by Gabriel Ferreira on February 2025.
//          Advisors: Thiago de Lima Prado and Sérgio Roberto Lopes
//          Federal University of Paraná - Physics Department
//
//      Julia version: https://github.com/gabriel-ferr/RecurrenceMicrostates.jl
//      Python version: https://github.com/gabriel-ferr/MicroRecPy
//      C++ version: https://github.com/gabriel-ferr/RecurrenceMicrostates (base for the MicroRecPy)
//
//      -------------------------------------------------------------------------------------------------------
//          Probabilities header
//      -------------------------------------------------------------------------------------------------------
#ifndef PROBABILITIES_H
#define PROBABILITIES_H
//      -------------------------------------------------------------------------------------------------------
//              * Include the libraries that we will use.
#include <tuple>
#include <vector>
#include <pybind11/pybind11.h>

#include "tensor.h"
#include "settings.h"
//      -------------------------------------------------------------------------------------------------------
//              * Namespace RecurrenceMicrostates
namespace RecurrenceMicrostates {
    //      -------------------------------------------------------------------------------------------------------
    //              * Default recurrence function
    inline bool std_recurrence(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &params);
    //      -------------------------------------------------------------------------------------------------------
    //              * Our Probabilities class structure.
    class Probabilities {
        std::vector<std::vector<std::vector<size_t>>> samples;

        const Settings settings;
        const double sample_rate;
        const Tensor<double> data_x;
        const Tensor<double> data_y;
        const pybind11::function function;

        std::vector<double> vect_result;

        void compute_to_vector(const std::vector<double> &params);
        void compute_to_dict(const std::vector<double> &params);

        static std::tuple<std::vector<size_t>, size_t> task_compute_using_vector(const std::vector<std::vector<size_t>> &samples,
            const Settings &settings, const Tensor<double> &data_x, const Tensor<double> &data_y, const std::vector<double> &params,
            const pybind11::function &function);

        static std::vector<double> get_data(const Tensor<double> tensor, const std::vector<size_t> &fixed_indexes,
            const std::vector<size_t> &recursive_indexes);

        static bool call_user_function(const std::vector<double> &x, const std::vector<double> &y, const std::vector<double> &params,
            const pybind11::function &function);

    public:
          explicit Probabilities(const Settings &settings, const Tensor<double> &data_x, const Tensor<double> &data_y,
                             const std::vector<double> &params, double sample_rate = 0.2, const pybind11::function &func = pybind11::none());
    };
    //      -------------------------------------------------------------------------------------------------------
}
#endif