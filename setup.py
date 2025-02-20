#
#           Python - Recurrence Microstates Library (MicroRecPy)
#           Created by Gabriel Ferreira on February 2025.
#           Advisors: Thiago de Lima Prado and Sérgio Roberto Lopes
#           Federal University of Paraná - Physics Department
#
#       Julia version: https://github.com/gabriel-ferr/RecurrenceMicrostates.jl
#       Python version: https://github.com/gabriel-ferr/MicroRecPy
#       C++ version: https://github.com/gabriel-ferr/RecurrenceMicrostates (base for the MicroRecPy)
#
#       -------------------------------------------------------------------------------------------------------
#           This is the library setup code.
#       -------------------------------------------------------------------------------------------------------
import pybind11
from setuptools import setup, Extension

ext_modules = [
    Extension(
        "microrecpy",
        ["src/module.cpp", "src/settings.cpp", "src/tensor.cpp", "src/probabilities.cpp"],
        include_dirs=[pybind11.get_include()],
        language="c++",
        extra_compile_args=["-std=c++23"]
    ),
]

setup(
    name="microrecpy",
    version="0.0.1",
    author="Gabriel Ferreira",
    description="A library to compute the recurrence microstates probabilities from some generic data.",
    ext_modules=ext_modules,
    zip_safe=False
)