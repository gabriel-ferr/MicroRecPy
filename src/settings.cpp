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
//          Settings .cpp body
//      -------------------------------------------------------------------------------------------------------
//                * Include the file header.
#include "settings.h"
//                * Include the used libraries.
#include <vector>
#include <thread>
#include <numeric>
#include <iostream>
//      -------------------------------------------------------------------------------------------------------
RecurrenceMicrostates::Settings::Settings(const std::vector<size_t> &structure, const unsigned int threads, const unsigned short mode) {
    //      Check the input before to do anything.
    if (structure.size() < 2) throw std::invalid_argument("[ERROR] Recurrence Microstates - Settings: the microstate structure required at least two dimensions.");

    //      Save the information.
    this->threads = threads;
    this->shape = structure;
    this->hypervolume = std::accumulate(structure.begin(), structure.end(), 1, std::multiplies());

    //      Check the number of threads.
    if (threads < 1) {
        std::cout << "[WARNING] Recurrence Microstates - Settings: it was not possible to determinate the number of available threads, value set to: " << DEFAULT_THREADS << std::endl;
        this->threads = DEFAULT_THREADS;
    } else if (threads > std::thread::hardware_concurrency())
        std::cout << "[WARNING] Recurrence Microstates - Settings: the number of configured threads exceeds the number available on the hardware. This configuration may affect library performance." << std::endl;

    //      Since we are using Python, the hypervolume of our microstate cannot exceed 64.
    if (this->hypervolume > 64) throw std::invalid_argument("[ERROR] Recurrence Microstates - Settings: the hypervolume of the microstates exceeds 64, and this library does not support it.");

    //      Check the use of dictionaries.
    this->use_dictionary = mode == MODE_FORCE_DICTIONARY;
    this->use_dictionary = hypervolume > DEFAULT_HYPERVOLUME_TO_DICTIONARY ? true : this->use_dictionary;
    this->use_dictionary = mode == MODE_FORCE_VECTOR ? false : this->use_dictionary;

    //      Compute the power vector.
    for (size_t i = 0; i < hypervolume; i++) vect.push_back(static_cast<size_t>(pow(2, i)));
}