from ..utils import *
from ..cytnx.cytnx_extension_c import *  


"""
@add_method(Storage)
def astype(self, new_type):
    if(self.dtype() == new_type):
        return self

    else:
        return self.astype_different_type(new_type)
"""
""
@add_method(CyTensor)
def to(self, device):
    if(self.device() == device):
        return self

    else:
        return self.to_different_device(device)
""
