#include "linalg.hpp"
#include "linalg_internal_interface.hpp"


namespace cytnx {
    namespace linalg {
        Tensor Mul(const Tensor &Lt, const Tensor &Rt) {

            cytnx_error_msg(Lt.shape() != Rt.shape(), "[Mul] error, the two tensor does not have the same type.%s",
                            "\n");
            cytnx_error_msg(Lt.device() != Rt.device(), "[Mul] error, two tensor cannot on different devices.%s", "\n");
            //cytnx_error_msg(!(Lt.is_contiguous() && Rt.is_contiguous()), "[Mul] error two tensors must be contiguous. Call Contiguous_() or Contiguous() first%s","\n");

            Tensor out(Lt.shape(), Lt.dtype() < Rt.dtype() ? Lt.dtype() : Rt.dtype(), Lt.device());


            if (Lt.is_contiguous() && Rt.is_contiguous()) {
                // contiguous section
                if (Lt.device() == Device.cpu) {
                    cytnx::linalg_internal::lii.Ari_ii[Lt.dtype()][Rt.dtype()](out._impl->storage()._impl,
                                                                               Lt._impl->storage()._impl,
                                                                               Rt._impl->storage()._impl,
                                                                               Lt._impl->storage()._impl->size(), {},
                                                                               {}, {}, 1);
                } else {
#ifdef UNI_GPU
                    checkCudaErrors(cudaSetDevice(Rt.device()));
                    cytnx::linalg_internal::lii.cuAri_ii[Lt.dtype()][Rt.dtype()](out._impl->storage()._impl,Lt._impl->storage()._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                    cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
                }
            } else {
                // non-contiguous section
                if (Lt.device() == Device.cpu) {
                    cytnx::linalg_internal::lii.Ari_ii[Lt.dtype()][Rt.dtype()](out._impl->storage()._impl,
                                                                               Lt._impl->storage()._impl,
                                                                               Rt._impl->storage()._impl,
                                                                               Lt._impl->storage()._impl->size(),
                                                                               Lt._impl->shape(), Lt._impl->invmapper(),
                                                                               Rt._impl->invmapper(), 1);
                } else {
#ifdef UNI_GPU
                    cytnx_error_msg(true, "[Mul][on GPU/CUDA] error two tensors must be contiguous. Call Contiguous_() or Contiguous() first%s","\n");
#else
                    cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
                }

            }
            return out;

        }

        //-----------------------------------------------------------------------------------
        template<>
        Tensor Mul<cytnx_complex128>(const cytnx_complex128 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.ComplexDouble);
            Cnst.at<cytnx_complex128>(0) = lc;

            Tensor out(Rt.shape(), Type.ComplexDouble, Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.ComplexDouble][Rt.dtype()](out._impl->storage()._impl,
                                                                                   Cnst._impl,
                                                                                   Rt._impl->storage()._impl,
                                                                                   Rt._impl->storage()._impl->size(),
                                                                                   {}, {}, {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.ComplexDouble][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_complex64>(const cytnx_complex64 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.ComplexFloat);
            Cnst.at<cytnx_complex64>(0) = lc;

            Tensor out(Rt.shape(), Type.ComplexFloat < Rt.dtype() ? Type.ComplexFloat : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.ComplexFloat][Rt.dtype()](out._impl->storage()._impl,
                                                                                  Cnst._impl, Rt._impl->storage()._impl,
                                                                                  Rt._impl->storage()._impl->size(), {},
                                                                                  {}, {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.ComplexFloat][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_double>(const cytnx_double &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Double);
            Cnst.at<cytnx_double>(0) = lc;

            Tensor out(Rt.shape(), Type.Double < Rt.dtype() ? Type.Double : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Double][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                            Rt._impl->storage()._impl,
                                                                            Rt._impl->storage()._impl->size(), {}, {},
                                                                            {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Double][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_float>(const cytnx_float &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Float);
            Cnst.at<cytnx_float>(0) = lc;

            Tensor out(Rt.shape(), Type.Float < Rt.dtype() ? Type.Float : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Float][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                           Rt._impl->storage()._impl,
                                                                           Rt._impl->storage()._impl->size(), {}, {},
                                                                           {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Float][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_int64>(const cytnx_int64 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Int64);
            Cnst.at<cytnx_int64>(0) = lc;

            Tensor out(Rt.shape(), Type.Int64 < Rt.dtype() ? Type.Int64 : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Int64][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                           Rt._impl->storage()._impl,
                                                                           Rt._impl->storage()._impl->size(), {}, {},
                                                                           {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Int64][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_uint64>(const cytnx_uint64 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Uint64);
            Cnst.at<cytnx_uint64>(0) = lc;

            Tensor out(Rt.shape(), Type.Uint64 < Rt.dtype() ? Type.Uint64 : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Uint64][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                            Rt._impl->storage()._impl,
                                                                            Rt._impl->storage()._impl->size(), {}, {},
                                                                            {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Uint64][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_int32>(const cytnx_int32 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Int32);
            Cnst.at<cytnx_int32>(0) = lc;

            Tensor out(Rt.shape(), Type.Int32 < Rt.dtype() ? Type.Int32 : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Int32][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                           Rt._impl->storage()._impl,
                                                                           Rt._impl->storage()._impl->size(), {}, {},
                                                                           {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Int32][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_uint32>(const cytnx_uint32 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Uint32);
            Cnst.at<cytnx_uint32>(0) = lc;

            Tensor out(Rt.shape(), Type.Uint32 < Rt.dtype() ? Type.Uint32 : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Uint32][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                            Rt._impl->storage()._impl,
                                                                            Rt._impl->storage()._impl->size(), {}, {},
                                                                            {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Uint32][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_int16>(const cytnx_int16 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Int16);
            Cnst.at<cytnx_int16>(0) = lc;

            Tensor out(Rt.shape(), Type.Int16 < Rt.dtype() ? Type.Int16 : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Int16][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                           Rt._impl->storage()._impl,
                                                                           Rt._impl->storage()._impl->size(), {}, {},
                                                                           {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Int16][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_uint16>(const cytnx_uint16 &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Uint16);
            Cnst.at<cytnx_uint16>(0) = lc;

            Tensor out(Rt.shape(), Type.Uint16 < Rt.dtype() ? Type.Uint16 : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Uint16][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                            Rt._impl->storage()._impl,
                                                                            Rt._impl->storage()._impl->size(), {}, {},
                                                                            {}, 1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Uint16][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        template<>
        Tensor Mul<cytnx_bool>(const cytnx_bool &lc, const Tensor &Rt) {
            Storage Cnst(1, Type.Bool);
            Cnst.at<cytnx_bool>(0) = lc;

            Tensor out(Rt.shape(), Type.Bool < Rt.dtype() ? Type.Bool : Rt.dtype(), Rt.device());

            if (Rt.device() == Device.cpu) {
                cytnx::linalg_internal::lii.Ari_ii[Type.Bool][Rt.dtype()](out._impl->storage()._impl, Cnst._impl,
                                                                          Rt._impl->storage()._impl,
                                                                          Rt._impl->storage()._impl->size(), {}, {}, {},
                                                                          1);
            } else {
#ifdef UNI_GPU
                checkCudaErrors(cudaSetDevice(Rt.device()));
                cytnx::linalg_internal::lii.cuAri_ii[Type.Bool][Rt.dtype()](out._impl->storage()._impl,Cnst._impl,Rt._impl->storage()._impl,Rt._impl->storage()._impl->size(),{},{},{},1);
#else
                cytnx_error_msg(true, "[Mul] fatal error, the tensor is on GPU without CUDA support.%s", "\n");
#endif
            }

            return out;
        }

        //-----------------------------------------------------------------------------------
        template<>
        Tensor Mul<cytnx_complex128>(const Tensor &Lc, const cytnx_complex128 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_complex64>(const Tensor &Lc, const cytnx_complex64 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_double>(const Tensor &Lc, const cytnx_double &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_float>(const Tensor &Lc, const cytnx_float &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_int64>(const Tensor &Lc, const cytnx_int64 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_uint64>(const Tensor &Lc, const cytnx_uint64 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_int32>(const Tensor &Lc, const cytnx_int32 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_uint32>(const Tensor &Lc, const cytnx_uint32 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_int16>(const Tensor &Lc, const cytnx_int16 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_uint16>(const Tensor &Lc, const cytnx_uint16 &rc) {
            return Mul(rc, Lc);
        }

        template<>
        Tensor Mul<cytnx_bool>(const Tensor &Lc, const cytnx_bool &rc) {
            return Mul(rc, Lc);
        }


        //============================================
        // CyTensor
        //============================================

        CyTensor Mul(const CyTensor &Lt, const CyTensor &Rt){
            cytnx_error_msg(true,"[Developing!]%s","\n");
            return CyTensor();
        }

        //-----------------------------------------------------------------------------------

        template<class T>
        CyTensor Mul(const T &lc, const CyTensor &Rt){
            CyTensor out = Rt.clone();
            if(Rt.is_blockform()){
                //developing
                cytnx_error_msg(true,"[Developing][Mul][SparseCyTensor]%s","\n");
            }else{
                out.get_block_() = Mul(lc , out.get_block_());
            }
            return out;
        }

        template CyTensor Mul<cytnx_complex128>(const cytnx_complex128 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_complex64>(const cytnx_complex64 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_double>(const cytnx_double &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_float>(const cytnx_float &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_int64>(const cytnx_int64 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_uint64>(const cytnx_uint64 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_int32>(const cytnx_int32 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_uint32>(const cytnx_uint32 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_int16>(const cytnx_int16 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_uint16>(const cytnx_uint16 &lc, const CyTensor &Rt);
        template CyTensor Mul<cytnx_bool>(const cytnx_bool &lc, const CyTensor &Rt);

        //-----------------------------------------------------------------------------------
        template<>
        CyTensor Mul<cytnx_complex128>(const CyTensor &Lt, const cytnx_complex128 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_complex64>(const CyTensor &Lt, const cytnx_complex64 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_double>(const CyTensor &Lt, const cytnx_double &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_float>(const CyTensor &Lt, const cytnx_float &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_int64>(const CyTensor &Lt, const cytnx_int64 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_uint64>(const CyTensor &Lt, const cytnx_uint64 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_int32>(const CyTensor &Lt, const cytnx_int32 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_uint32>(const CyTensor &Lt, const cytnx_uint32 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_int16>(const CyTensor &Lt, const cytnx_int16 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_uint16>(const CyTensor &Lt, const cytnx_uint16 &rc){
            return Mul(rc,Lt);
        }
        template<>
        CyTensor Mul<cytnx_bool>(const CyTensor &Lt, const cytnx_bool &rc){
            return Mul(rc,Lt);
        }



    }//linalg
Tensor operator*(const Tensor &Lt, const Tensor &Rt){
  return cytnx::linalg::Mul(Lt,Rt);
}
template<>
Tensor operator*<cytnx_complex128>(const cytnx_complex128 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_complex64>(const cytnx_complex64 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_double>(const cytnx_double &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_float>(const cytnx_float &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_int64>(const cytnx_int64 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_uint64>(const cytnx_uint64 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_int32>(const cytnx_int32 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_uint32>(const cytnx_uint32 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_uint16>(const cytnx_uint16 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_int16>(const cytnx_int16 &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}
template<>
Tensor operator*<cytnx_bool>(const cytnx_bool &lc, const Tensor &Rt){
  return cytnx::linalg::Mul(lc,Rt);
}

template<>
Tensor operator*<cytnx_complex128>(const Tensor &Lt, const cytnx_complex128 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_complex64>(const Tensor &Lt, const cytnx_complex64 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_double>(const Tensor &Lt, const cytnx_double &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_float>(const Tensor &Lt, const cytnx_float &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_int64>(const Tensor &Lt, const cytnx_int64 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_uint64>(const Tensor &Lt, const cytnx_uint64 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_int32>(const Tensor &Lt, const cytnx_int32 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_uint32>(const Tensor &Lt, const cytnx_uint32 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_uint16>(const Tensor &Lt, const cytnx_uint16 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_int16>(const Tensor &Lt, const cytnx_int16 &rc){
  return cytnx::linalg::Mul(Lt,rc);
}
template<>
Tensor operator*<cytnx_bool>(const Tensor &Lt, const cytnx_bool &rc){
  return cytnx::linalg::Mul(Lt,rc);
}


template<> Tensor operator*<cytnx_complex128>(const Tensor &, const cytnx_complex128&);
template<> Tensor operator*<cytnx_complex64>(const Tensor &, const cytnx_complex64&);
template<> Tensor operator*<cytnx_double>(const Tensor &, const cytnx_double&);
template<> Tensor operator*<cytnx_float>(const Tensor &, const cytnx_float&);
template<> Tensor operator*<cytnx_int64>(const Tensor &, const cytnx_int64&);
template<> Tensor operator*<cytnx_uint64>(const Tensor &, const cytnx_uint64&);
template<> Tensor operator*<cytnx_int32>(const Tensor &, const cytnx_int32&);
template<> Tensor operator*<cytnx_uint32>(const Tensor &, const cytnx_uint32&);
template<> Tensor operator*<cytnx_uint16>(const Tensor &, const cytnx_uint16&);
template<> Tensor operator*<cytnx_int16>(const Tensor &, const cytnx_int16&);
template<> Tensor operator*<cytnx_bool>(const Tensor &, const cytnx_bool&);


template<> Tensor operator*<cytnx_complex128>( const cytnx_complex128&,const Tensor &);
template<> Tensor operator*<cytnx_complex64>( const cytnx_complex64&,const Tensor &);
template<> Tensor operator*<cytnx_double>( const cytnx_double&,const Tensor &);
template<> Tensor operator*<cytnx_float>( const cytnx_float&,const Tensor &);
template<> Tensor operator*<cytnx_int64>( const cytnx_int64&,const Tensor &);
template<> Tensor operator*<cytnx_uint64>( const cytnx_uint64&,const Tensor &);
template<> Tensor operator*<cytnx_int32>( const cytnx_int32&,const Tensor &);
template<> Tensor operator*<cytnx_uint32>( const cytnx_uint32&,const Tensor &);
template<> Tensor operator*<cytnx_uint16>( const cytnx_uint16&,const Tensor &);
template<> Tensor operator*<cytnx_int16>( const cytnx_int16&,const Tensor &);
template<> Tensor operator*<cytnx_bool>( const cytnx_bool&,const Tensor &);


    //-------------------------------------------
    CyTensor operator*(const CyTensor &Lt, const CyTensor &Rt){
        return cytnx::linalg::Mul(Lt,Rt);
    }

    template<class T>
    CyTensor operator*(const T &lc, const CyTensor &Rt){
        return cytnx::linalg::Mul(lc,Rt);
    }

    template CyTensor operator*<cytnx_complex128>(const cytnx_complex128 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_complex64>(const cytnx_complex64 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_double>(const cytnx_double &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_float>(const cytnx_float &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_int64>(const cytnx_int64 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_uint64>(const cytnx_uint64 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_int32>(const cytnx_int32 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_uint32>(const cytnx_uint32 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_int16>(const cytnx_int16 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_uint16>(const cytnx_uint16 &lc, const CyTensor &Rt);
    template CyTensor operator*<cytnx_bool>(const cytnx_bool &lc, const CyTensor &Rt);

    template<>
    CyTensor operator*<cytnx_complex128>(const CyTensor &Lt, const cytnx_complex128 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_complex64>(const CyTensor &Lt, const cytnx_complex64 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_double>(const CyTensor &Lt, const cytnx_double &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_float>(const CyTensor &Lt, const cytnx_float &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_int64>(const CyTensor &Lt, const cytnx_int64 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_uint64>(const CyTensor &Lt, const cytnx_uint64 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_int32>(const CyTensor &Lt, const cytnx_int32 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_uint32>(const CyTensor &Lt, const cytnx_uint32 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_int16>(const CyTensor &Lt, const cytnx_int16 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_uint16>(const CyTensor &Lt, const cytnx_uint16 &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }
    template<>
    CyTensor operator*<cytnx_bool>(const CyTensor &Lt, const cytnx_bool &rc){
       return cytnx::linalg::Mul(Lt,rc);
    }



}
