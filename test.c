typedef long atomic_long_t;
#include "linux/zsmalloc.h"

int main() {
    // std::cout << zsmalloc_alloc(5) << std::endl;
    void *pool = zs_create_pool("Adam's ZSwap Pool");
}