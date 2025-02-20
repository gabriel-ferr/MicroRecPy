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
//          Tensor header
//      -------------------------------------------------------------------------------------------------------
#ifndef TENSOR_H
#define TENSOR_H
//      -------------------------------------------------------------------------------------------------------
//              * Include the libraries that we will use.
#include <vector>
#include <numeric>
//      -------------------------------------------------------------------------------------------------------
//              * Namespace RecurrenceMicrostates
namespace RecurrenceMicrostates {
    //      -------------------------------------------------------------------------------------------------------
    //              * Our tensor class.
    template <typename T> class Tensor {
        std::vector<size_t> shape;
        std::vector<T> body;

        [[nodiscard]] size_t get_index(const std::vector<size_t> &shape) const;

    public:
        //      Class proprieties.
        [[nodiscard]] std::vector<size_t> dimensions() const { return shape; }
        [[nodiscard]] size_t dimension(size_t d) const { return shape[d]; }

        //      Get a vector projection of our tensor as a "column" for the first dimension.
        std::vector<T> vector(const std::vector<size_t> &indexes) const;

        //      Constructor
        explicit Tensor(const std::vector<size_t> &shape) : shape(shape) {
            body.resize(std::accumulate(shape.begin(), shape.end(), size_t{1}, std::multiplies()));
        }

        //      Operator [] to access / modify elements.
        template <typename... Args> T& operator[] (Args... args) {
            return body[get_index({static_cast<size_t>(args)...})];
        }

        template <typename... Args> const T& operator[] (Args... args) const {
            return body[get_index({static_cast<size_t>(args)...})];
        }
    };
    //      -------------------------------------------------------------------------------------------------------
}
#endif