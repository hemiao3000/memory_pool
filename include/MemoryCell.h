//
// Created by ben on 19-2-21.
//

#ifndef MEM_POOL_MEMORYCELL_H
#define MEM_POOL_MEMORYCELL_H

#include "MemoryBlock.h"

class AbstractMemoryBlock;

// 内存块 最小单元
struct MemoryCell
{
    AbstractMemoryBlock *_block = nullptr;  // 所属大内存块
    MemoryCell *_next = nullptr;    // 下一个Cell位置
    int _id = -1;                   // 内存块编号
    int _ref_count = 0;             // 引用次数（理论上说，非0即1）
    bool _is_in_pool = false;       // 是否是Block中的Cell
};

#endif //MEM_POOL_MEMORYCELL_H
