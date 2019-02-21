//
// Created by ben on 19-2-21.
//

#ifndef MEM_POOL_MEMORYPOOL_H
#define MEM_POOL_MEMORYPOOL_H

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <cassert>
#include <cstdio>
#include "MemoryBlock.h"

#define MAX_MEMORY_SZIE 1024

class MemoryPool
{
private:
    MemoryBlock<64, 10> _mem64;          // 10000 个大小为 64 字节的Cell 组成的一个 Block
    MemoryBlock<128, 10> _mem128;        // 10000 个大小为 128 字节的Cell 组成的一个 Block
    MemoryBlock<256, 10> _mem256;        // 10000 个大小为 256 字节的Cell 组成的一个 Block
    MemoryBlock<512, 10> _mem512;        // 10000 个大小为 512 字节的Cell 组成的一个 Block
    MemoryBlock<1024, 10> _mem1024;      // 10000 个大小为 1024 字节的Cell 组成的一个 Block
    AbstractMemoryBlock *_block_array[MAX_MEMORY_SZIE + 1] = {};

public:
    MemoryPool() {
        printf("DEBUG: MemoryPool()\n");
        /*
        _mem64.initMemory();
        _mem128.initMemory();
        _mem256.initMemory();
        _mem512.initMemory();
        _mem1024.initMemory();
         */

        init_block_array(0, 64, &_mem64);       // 0 ... 64 都指向同一个 block
        init_block_array(65, 128, &_mem128);    // 65 ... 128 都指向同一个 block
        init_block_array(129, 256, &_mem256);   // 129 ... 256 都指向同一个 block
        init_block_array(257, 512, &_mem512);   // 257 ... 512 都指向同一个 block
        init_block_array(513, 1024, &_mem1024); // 513 ... 1024 都指向同一个 block
    }

    static MemoryPool &Instance() { // 单例模式 静态
        static MemoryPool pool;
        return pool;
    }

    // 申请内存
    void *allocMem(size_t nSize) {

        // 所申请的内存空间大小超过了限制，则直接分配，不从内存池中取
        if (nSize > MAX_MEMORY_SZIE) {
            auto cell = (MemoryCell *) malloc(nSize + sizeof(MemoryCell));
            cell->_is_in_pool = false;
            cell->_id = -1;
            cell->_ref_count = 1;
            cell->_block = nullptr;
            cell->_next = nullptr;
            printf("DEBUG: allocMem: %p, id=%d, size=%ld\n", cell, cell->_id, nSize);
            return ((char *) cell + sizeof(MemoryCell));
        }

        // nSize <= MAX_MEMORY_SZIE，从内存池中获取内存
        return _block_array[nSize]->allocMemory(nSize);
    }

    // 释放内存
    void freeMem(void *mem) {
        auto cell = (MemoryCell *) ((char *) mem - sizeof(MemoryCell));
        printf("DEBUG: freeMem: %p, id=%d\n", cell, cell->_id);

        if (cell->_is_in_pool) {
            cell->_block->freeMemory(mem);
        }
        else {
            if (--cell->_ref_count == 0) {
                free(cell);
            }
        }
    }

    // 增加内存块的引用计数
    void addRef(void *mem) {
        auto cell = (MemoryCell *) ((char *) mem - sizeof(MemoryCell));
        ++cell->_ref_count;
    }

private:
    // 初始化内存池映射数组
    void init_block_array(int begin, int end, AbstractMemoryBlock *block) {
        for (int n = begin; n <= end; n++) {
            _block_array[n] = block;
        }
    }
};

#endif //MEM_POOL_MEMORYPOOL_H
