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
//          Settings header
//      -------------------------------------------------------------------------------------------------------
#ifndef SETTINGS_H
#define SETTINGS_H
//      -------------------------------------------------------------------------------------------------------
//              * Include the libraries that we will use.
#include <thread>
#include <vector>
//      -------------------------------------------------------------------------------------------------------
//              * Defines
#define MODE_DEFAULT 0
#define MODE_FORCE_VECTOR 1
#define MODE_FORCE_DICTIONARY 2

#define DEFAULT_THREADS 1
#define DEFAULT_HYPERVOLUME_TO_DICTIONARY 26
//      -------------------------------------------------------------------------------------------------------
//              * Namespace RecurrenceMicrostates
namespace RecurrenceMicrostates {
    //      -------------------------------------------------------------------------------------------------------
    //              * Our Settings class structure.
    class Settings {

        std::vector<size_t> vect;
        std::vector<size_t> shape;

        size_t hypervolume;
        unsigned int threads;
        bool use_dictionary;

    public:
        [[nodiscard]] unsigned int available_threads() const { return threads; }
        [[nodiscard]] size_t dimensions() const { return shape.size(); }
        [[nodiscard]] size_t structure(const size_t dim) const { return shape[dim]; }
        [[nodiscard]] size_t get_hypervolume() const { return hypervolume; }
        [[nodiscard]] size_t power(const size_t dim) const { return vect[dim]; }
        [[nodiscard]] size_t possibilities() const { return static_cast<size_t>(std::pow(2, hypervolume)); }
        [[nodiscard]] bool dictionary() const { return use_dictionary; }

        explicit Settings(const std::vector<size_t> &structure, unsigned int threads = std::thread::hardware_concurrency(), unsigned short mode = MODE_DEFAULT);
    };
    //      -------------------------------------------------------------------------------------------------------
}
#endif
