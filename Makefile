# I've left this to show what I was doing, but it needs converting to build a CPython extension
# (probably use a setup.py instead, or CMake-like scikit-build stuff)

CFLAGS := -I${CONDA_PREFIX}/include/python3.11 -I${CONDA_PREFIX}/lib/python3.11/site-packages/numpy/core/include

all:
	gcc -c -fPIC $(CFLAGS) patched_array_adaptor.c
