from ctypes import CFUNCTYPE, c_void_p
from numba import njit
import numpy as np

import llvmlite.binding as llvm


# Njit something first to make sure everything is initialized before we patch -
# not 100% sure this is necesssary (check later) but it'll take out a potential
# issue while we're still getting this to work

@njit
def f(x, y):
    return x + y


f(1, 2)


addr = 0x12345678  # You need to get the address of the patched function here
llvm.add_symbol("NRT_adapt_ndarray_from_python", addr)


class MyArray:
    def __init__(self, array):
        self.xyz = array

    def __array__(self):
        return self.xyz


N = 5
x = MyArray(np.arange(N))
y = MyArray(np.ones(N))

print(f(x, y))
