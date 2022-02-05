# import torch
from torch._C import *
import torch._C as _C

from torch._Tensor import Tensor
# import torch._Tensor

__all__ = ['test']

for name in dir(_C._VariableFunctions):
    globals()[name] = getattr(_C._VariableFunctions, name)
    __all__.append(name)

# class Tensor(_C._TensorBase):
#     pass

print(__all__)
test()
# a = Tensor()
# print(a)