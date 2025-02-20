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
//          ** Acknowledgment to JetBrains for providing the Student License of CLion and to CNPq - Brazil
//  for the financial support.
//
//      -------------------------------------------------------------------------------------------------------
//            This is the C++ module that communicates with the Python interpreter using the PyBind11 library.
//      -------------------------------------------------------------------------------------------------------
//                * Include PyBind11
#include <pybind11/pybind11.h>
//                * Include the internal headers
#include "tensor.h"
#include "settings.h"
#include "probabilities.h"
//      -------------------------------------------------------------------------------------------------------
inline pybind11::capsule settings(const pybind11::tuple &structure, const unsigned int threads = DEFAULT_THREADS, const bool force_dictionaries = false, const bool force_vectors = false) {
    const unsigned short mode = force_dictionaries ? MODE_FORCE_DICTIONARY : (force_vectors ? MODE_FORCE_VECTOR : MODE_DEFAULT);
    const auto conf = new RecurrenceMicrostates::Settings(structure.cast<std::vector<size_t>>(), threads, mode);

    return {conf, "settings_ptr", [](void *ptr) {
        delete static_cast<RecurrenceMicrostates::Settings *>(ptr);
    }};
}
//      -------------------------------------------------------------------------------------------------------
//                * PyBind11 Module Settings
PYBIND11_MODULE(microrecpy, m) {
    m.def("settings", &settings,
        pybind11::arg("structure"),
        pybind11::arg("threads") = DEFAULT_THREADS,
        pybind11::arg("force_dictionaries") = false,
        pybind11::arg("force_vectors") = false,
        "Create a setting structure that control the process.");
}