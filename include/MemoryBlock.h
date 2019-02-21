//
// Created by ben on 19-2-21.
//

#ifndef MEM_POOL_MEMORYBLOCK_H
#define MEM_POOL_MEMORYBLOCK_H

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include "MemoryCell.h"

// MemoryBlock中存储的内容的布局是 cell_mem_cell_mem_cell_mem_cell_mem
class AbstractMemoryBlock
{
private:

protected:
    char *_buf;             // block的地址
    MemoryCell *_header;    // block中第一个cell
    size_t _cell_size;      // block中cell的单元大小
    size_t _cell_count;     // block中cell的数量

    AbstractMemoryBlock() : AbstractMemoryBlock(0, 0) {
    }

    void init_memory_cell(MemoryCell *cell) {
        cell->_is_in_pool = false;
        cell->_id = -1;
        cell->_ref_count = 0;
        cell->_block = this;
        cell->_next = nullptr;
    }

public:

    AbstractMemoryBlock(size_t cell_size, size_t cell_count) {
        _buf = nullptr;
        _header = nullptr;
        _cell_size = cell_size;
        _cell_count = cell_count;
    }

    virtual ~AbstractMemoryBlock() = default;

    // 从Block中获取Cell
    void *allocMemory(size_t nSize) {
        if (_buf == nullptr) {
//            printf("DEBUG: 逻辑错误\n");
            initMemory();
        }

        MemoryCell *cell = nullptr;
        if (_header == nullptr) {
            cell = (MemoryCell *) malloc(_cell_size + sizeof(MemoryCell));
            init_memory_cell(cell);

            cell->_is_in_pool = false;  // 该Cell不是Block中的Cell
            cell->_id = -1;             // 它不是block中的cell，因此也就没有id
            cell->_ref_count = 1;
            cell->_block = nullptr;
            cell->_next = nullptr;      // 它不是block中的cell，因此也就没有所谓的下一个cell
        }
        else {
            cell = _header;             // 头部的cell被“用掉”，第二个cell将成为新的头部
            _header = _header->_next;
            cell->_ref_count = 1;       // 标识该cell被人使用着
        }

        printf("DEBUG: allocMem: %p, id=%d, size=%ld\n", cell, cell->_id, _cell_size);
        return (char *) cell + sizeof(MemoryCell);
    }

    void freeMemory(void *mem) {
        auto cell = (MemoryCell *) ((char *) mem - sizeof(MemoryCell));
        if (cell->_ref_count != 0) {
            return;
        }

        // 该cell有可能不是block中的cell。对应allocMemory中的if情况
        if (cell->_is_in_pool) {
            cell->_next = _header;
            _header = cell;
        }
        else {
            free(mem);
        }
    }

    // 分配Block并初始化其中的Cell
    void initMemory() {
        if (_buf != nullptr) {
            printf("DEBUG: Block已经过初始化过\n");
            return;
        }

        // 计算内存block的大小
        // 每个Cell结构体紧挨着的就是它的关联内存空间
        size_t real_size = _cell_size + sizeof(MemoryCell);
        size_t buf_size = real_size * _cell_count;

        // 向系统申请池的内存
        _buf = (char *) malloc(buf_size);

        // 初始化Block中的第一个Cell
        _header = (MemoryCell *) _buf;
        _header->_is_in_pool = true;
        _header->_id = 0;
        _header->_ref_count = 0;
        _header->_block = this;
        _header->_next = nullptr;

        // 遍历内存Block，初始化其它Cell
        MemoryCell *pTemp1 = _header;
        for (size_t n = 1; n < _cell_count; n++) {
            auto pTemp2 = (MemoryCell *) (_buf + (n * real_size));
            pTemp2->_is_in_pool = true;
            pTemp2->_id = (int) n;
            pTemp2->_ref_count = 0;
            pTemp2->_block = this;
            pTemp2->_next = nullptr;

            pTemp1->_next = pTemp2;
            pTemp1 = pTemp2;
        }
    }
};

template<size_t CELL_SIZE, size_t CELL_COUNT>
class MemoryBlock : public AbstractMemoryBlock
{
public:
    MemoryBlock() {
        printf("DEBUG: 创建cell_size为%ld的Block\n", CELL_SIZE);
        // 8 4   61/8=7  61%8=5
        const size_t n = sizeof(void *);
        // (7*8) + 8
        _cell_size = (CELL_SIZE / n) * n + (CELL_SIZE % n ? n : 0);
        _cell_count = CELL_COUNT;
    }

};


#endif //MEM_POOL_MEMORYBLOCK_H
