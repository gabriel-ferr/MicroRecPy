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
//          Probabilities .cpp body
//      -------------------------------------------------------------------------------------------------------
//                * Include the file header.
#include "probabilities.h"
//                * Include the used libraries.
#include <tuple>
#include <vector>
#include <random>
#include <future>
#include <numeric>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
//      -------------------------------------------------------------------------------------------------------
bool RecurrenceMicrostates::std_recurrence(const std::vector<double> &x, const std::vector<double> &y,
    const std::vector<double> &params) {

    if (params.empty()) throw std::invalid_argument("[ERROR] Recurrence Microstates - Probabilities: the standard recurrence function requires a threshold parameter.");

    auto distance = 0.0;
    for (size_t i = 0; i < x.size(); i++) distance += std::pow(x[i] - y[i], 2);
    return params[0] - std::sqrt(distance) >= 0;
}
//      -------------------------------------------------------------------------------------------------------
void RecurrenceMicrostates::Probabilities::compute_to_vector(const std::vector<double> &params) {
    //      Alloc some memory =3
    size_t counter = 0;

    //      Create the async tasks...
    std::vector<std::future<std::tuple<std::vector<size_t>, size_t>>> tasks;
    tasks.reserve(settings.available_threads());
    for (unsigned int i = 0; i < settings.available_threads(); i++)
        tasks.push_back(std::async(std::launch::async, task_compute_using_vector, this->samples[i], this->settings,
            this->data_x, this->data_y, params, this->function));

    std::vector<std::tuple<std::vector<size_t>, size_t>> results;
    results.reserve(settings.available_threads());
    for (auto &f : tasks)
        results.push_back(f.get());

    for (auto &r : results)
        counter += std::get<1>(r);

    this->vect_result.resize(settings.possibilities());
    for (auto &r : results) {
        auto probs = std::get<0>(r);
        for (size_t i = 0; i < probs.size(); i++)
            this->vect_result[i] = probs[i] / static_cast<double>(counter);
    }
}
//      -------------------------------------------------------------------------------------------------------
std::tuple<std::vector<size_t>, size_t> RecurrenceMicrostates::Probabilities::task_compute_using_vector(
    const std::vector<std::vector<size_t>> &samples, const Settings &settings, const Tensor<double> &data_x,
    const Tensor<double> &data_y, const std::vector<double> &params, const pybind11::function &function) {

    std::vector<size_t> result(settings.possibilities(), 0);
    size_t counter = 0;

    std::vector<size_t> indexes(settings.dimensions(), 0);

    for (auto &sample : samples) {
        size_t add = 0;
        std::ranges::fill(indexes, 0);

        const std::vector index_x(sample.begin(), sample.begin() + settings.dimensions() / 2);
        const std::vector index_y(sample.begin() + settings.dimensions() / 2, sample.end());

        for (size_t m = 0; m < settings.get_hypervolume(); m++) {
            const std::vector recursive_x(indexes.begin(), indexes.begin() + settings.dimensions() / 2);
            const std::vector recursive_y(indexes.begin() + settings.dimensions() / 2, indexes.end());

            const auto x = get_data(data_x, index_x, recursive_x);
            const auto y = get_data(data_y, index_y, recursive_y);

            add += settings.power(m) * (function.is_none() ? std_recurrence(x, y, params) : call_user_function(x, y, params, function));

            //      Increment the recursive index.
            indexes[0]++;
            for (size_t k = 0; k < settings.dimensions() - 1; k++) {
                if (indexes[k] > settings.structure(k)) {
                    indexes[k+1]++;
                    indexes[k] = 0;
                } else break;
            }
        }

        result[add]++;
        counter++;
    }

    return std::make_tuple(result, counter);
}
//      -------------------------------------------------------------------------------------------------------
std::vector<double> RecurrenceMicrostates::Probabilities::get_data(const Tensor<double> tensor,
    const std::vector<size_t> &fixed_indexes, const std::vector<size_t> &recursive_indexes) {
    std::vector<size_t> indexes;
    for (size_t i = 0; i < tensor.dimensions().size() - 1; i++)
        indexes.push_back(fixed_indexes[i] + recursive_indexes[i]);

    return tensor.vector(indexes);
}
//      -------------------------------------------------------------------------------------------------------
bool RecurrenceMicrostates::Probabilities::call_user_function(const std::vector<double> &x,
    const std::vector<double> &y, const std::vector<double> &params, const pybind11::function &function) {

    return function(
        pybind11::array_t(x.size(), x.data()),
        pybind11::array_t(y.size(), y.data()),
        pybind11::array_t(params.size(), params.data())
    ).cast<bool>();
}
//      -------------------------------------------------------------------------------------------------------
RecurrenceMicrostates::Probabilities::Probabilities(const Settings &settings, const Tensor<double> &data_x,
    const Tensor<double> &data_y, const std::vector<double> &params, double sample_rate, const pybind11::function &func) :
    settings(settings), sample_rate(sample_rate), data_x(data_x), data_y(data_y), function(func) {
    //      Check the input arguments.
    if (data_x.dimensions() != data_y.dimensions())
        throw std::invalid_argument(
            "[ERROR] Recurrence Microstates - Probabilities: data x and data y must have the same number of dimensions.");
    if (data_x.dimension(0) != data_y.dimension(0))
        throw std::invalid_argument(
            "[ERROR] Recurrence Microstates - Probabilities: data x and data y first dimension must have the same size.");

    const auto dims = data_x.dimensions().size() - 1;
    const auto D = 2 * dims;

    if (settings.dimensions() != D)
        throw std::invalid_argument(
            "[ERROR] Recurrence Microstates - Settings: the configured microstate structure and the given data are not compatible.");

    //      Get the dimensions.
    auto dims_x = data_x.dimensions();
    auto dims_y = data_y.dimensions();

    //      Compute the recurrence space hypervolume.
    const auto hypervolume = std::accumulate(dims_x.begin() + 1, dims_x.end(), size_t{1}, std::multiplies()) *
                             std::accumulate(dims_y.begin() + 1, dims_y.end(), size_t{1}, std::multiplies());

    //      Number of samples to take.
    const auto numb_samples = static_cast<size_t>(std::floor(static_cast<double>(hypervolume) * sample_rate));
    //      Get the samples.
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector samples(numb_samples, std::vector<size_t>(D, 0));
    for (size_t dim = 0; dim < dims; dim++) {
        const auto max_x = dims_x[dim + 1] - settings.structure(dim);
        const auto max_y = dims_y[dim + 1] - settings.structure(dim);

        std::uniform_int_distribution<size_t> dist_x(0, max_x);
        std::uniform_int_distribution<size_t> dist_y(0, max_y);

        for (size_t i = 0; i < numb_samples; i++) {
            samples[i][dim] = dist_x(gen);
            samples[i][dims + dim] = dist_y(gen);
        }
    }

    //      Now, we divide the work between each available thread =D
    const auto int_number = numb_samples / settings.available_threads();
    auto rest_number = numb_samples % settings.available_threads();
    size_t start_index = 0;

    this->samples.resize(settings.available_threads());
    for (size_t i = 0; i < settings.available_threads(); i++) {
        const auto numb = int_number + (rest_number > 0 ? 1 : 0);
        this->samples[i].resize(numb);

        std::vector<size_t> indices(numb);
        std::iota(indices.begin(), indices.end(), start_index);

        for (size_t j = 0; j < numb; j++) this->samples[i][j] = samples[indices[j]];

        start_index += numb;
        if (rest_number > 0) --rest_number;
    }

    if (settings.dictionary()) this->compute_to_dict(params);
    else this->compute_to_vector(params);
}
