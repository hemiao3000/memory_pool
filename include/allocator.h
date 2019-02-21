//
// Created by ben on 19-2-21.
//

#ifndef MEM_POOL_ALLOCATOR_H
#define MEM_POOL_ALLOCATOR_H

#include <cstdio>
#include <cstdlib>
#include "MemoryPool.h"


void *operator new(size_t nSize) {
    return MemoryPool::Instance().allocMem(nSize);
}

void operator delete(void *p) {
    MemoryPool::Instance().freeMem(p);
}

void *operator new[](size_t nSize) {
    return MemoryPool::Instance().allocMem(nSize);
}

void operator delete[](void *p) {
    MemoryPool::Instance().freeMem(p);
}

void *mem_alloc(size_t size) {
    return malloc(size);
}

void mem_free(void *p) {
    free(p);
}

#endif //MEM_POOL_ALLOCATOR_H
