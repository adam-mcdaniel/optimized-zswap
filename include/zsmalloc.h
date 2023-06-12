

// #ifdef __cplusplus
// #endif

// #ifdef __cplusplus
// }
// #endif

#ifdef __cplusplus
extern "C" {
    void *zsmalloc_alloc(unsigned long size);
}
#else
void *zsmalloc_alloc(unsigned long size);
#endif


