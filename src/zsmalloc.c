typedef long atomic_long_t;
#include "zsmalloc.h"
#include <stdlib.h>
#include <stddef.h>


static struct zs_pool *POOL = NULL;


void *zsmalloc_alloc(unsigned long size) {
  // Simple type
  #pragma message("Compiling zsmalloc_alloc")

  // if (POOL == NULL) {
  //   POOL = zs_create_pool("Adam's ZSwap Pool");
  // }

  // 1. Allocate memory
  void *ptr = malloc(size);
  // int result = zs_malloc(size);

  return ptr;
}