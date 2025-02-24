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
//          Tensor .cpp body
//      -------------------------------------------------------------------------------------------------------
//                * Include the file header.
#include "tensor.h"
//                * Include the used libraries.
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
//      -------------------------------------------------------------------------------------------------------
template<typename T>
size_t RecurrenceMicrostates::Tensor<T>::get_index(const std::vector<size_t> &shape) const {
    //      Check if the number of dimensions is equals.
    if (shape.size() != this->shape.size()) throw std::invalid_argument("[ERROR] Recurrence Microstates - Tensor: shapes must have the same size.");

    //      Compute the index.
    auto index = shape[0];
    for (size_t i = 1; i < shape.size(); i++)
        index += shape[i] * std::accumulate(this->shape.begin(), static_cast<std::ptrdiff_t>(i + this->shape.begin()), size_t{1}, std::multiplies());

    return index;
}
//      -------------------------------------------------------------------------------------------------------
template<typename T>
std::vector<T> RecurrenceMicrostates::Tensor<T>::vector(const std::vector<size_t> &indexes) const {
    //      Check the input.
    if (indexes.size() != this->shape.size() - 1) throw std::invalid_argument("[ERROR] Recurrence Microstates - Tensor: the vector index must have the size of the tensor shape minus one.");

    //      Make the shape array to get the index.
    std::vector<size_t> shape;
    shape.resize(this->shape.size());

    shape[0] = indexes[0];
    std::ranges::copy(indexes, shape.begin() + 1);

    //      Make the result vector.
    std::vector<T> result;
    for (size_t i = 0; i < this->shape[0]; i++) {
        result.push_back(body[get_index(shape)]);
        ++shape[0];
    }

    return result;
}
//      -------------------------------------------------------------------------------------------------------
template<typename T>
RecurrenceMicrostates::Tensor<T>::Tensor(const pybind11::array_t<T> &array) {
}
//      -------------------------------------------------------------------------------------------------------