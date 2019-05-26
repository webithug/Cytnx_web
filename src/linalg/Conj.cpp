#include "linalg/linalg.hpp"
#include <iostream>

namespace cytnx{
    namespace linalg{
        Tensor Conj(const Tensor &Tin){
            
            //cytnx_error_msg(Tin.shape().size() != 2,"[Inv] error, Inv can only operate on rank-2 Tensor.%s","\n");
            //cytnx_error_msg(!Tin.is_contiguous(), "[Inv] error tensor must be contiguous. Call Contiguous_() or Contiguous() first%s","\n");
           
            //cytnx_error_msg(Tin.shape()[0] != Tin.shape()[1], "[Inv] error, the size of last two rank should be the same.%s","\n"); 

            Tensor out;
            out = Tin.clone();


            if(Tin.device()==cytnxdevice.cpu){

                if(out.dtype()<3) cytnx::linalg_internal::lii.Inv_inplace_ii[out.dtype()](out._impl->_get_storage(),out.shape().back());
                
                return out;

            }else{
                #ifdef UNI_GPU
                    if(out.dtype()<3) cytnx::linalg_internal::lii.cuInv_inplace_ii[out.dtype()](out._impl->_get_storage(),
                                                                             out.shape().back()); 
                    return out;
                #else
                    cytnx_error_msg(true,"[Inv] fatal error,%s","try to call the gpu section without CUDA support.\n");
                #endif
            }    
            

        }

    }// linalg
}//cytnx


