#!/usr/bin/env python

"""
setup file for Perceptron extension
"""

from distutils.core import setup, Extension


Perceptron_module = Extension('_Perceptron',
                           sources=['Perceptron_wrap.cxx', 'Perceptron.cpp'],
                           )

setup (name = 'Perceptron',
       version = '0.1',
       author      = "bslawski",
       description = """Multi-layer Perception""",
       ext_modules = [Perceptron_module],
       py_modules = ["Perceptron"],
       )
