; uchar tshr_saddr2py(void *saddr)

SECTION code_clib
SECTION code_arch

PUBLIC _tshr_saddr2py_fastcall

EXTERN _zx_saddr2py_fastcall

defc _tshr_saddr2py_fastcall = _zx_saddr2py_fastcall
