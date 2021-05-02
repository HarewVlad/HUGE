#pragma once

#include <memory>
#include <assert.h>

#define MAX(x, y) ((x) >= (y) ? (x) : (y))

#define ALIGN_DOWN(n, a) ((n) & ~((a) - 1))
#define ALIGN_UP(n, a) ALIGN_DOWN((n) + (a) - 1, (a))
#define ALIGN_DOWN_PTR(p, a) ((void *)ALIGN_DOWN((uintptr_t)(p), (a)))
#define ALIGN_UP_PTR(p, a) ((void *)ALIGN_UP((uintptr_t)(p), (a)))

#define ALIGN 8
#define BLOCK_SIZE 256

struct MemoryBlock {
  unsigned char *base = nullptr;
  unsigned char *ptr = nullptr;
  unsigned char *end = nullptr;
  unsigned int size = 0;
  MemoryBlock *prev = nullptr;
  MemoryBlock *next = nullptr;
};

struct Arena {
  MemoryBlock *current_block = nullptr;
  int block_count = 0;
  unsigned int block_size = BLOCK_SIZE;
};

void ArenaGrow(Arena *arena, size_t size);
unsigned char *ArenaAlloc(Arena *arena, size_t size); // TODO: Add mutex.
void ArenaFree(Arena *arena);
void ArenaFreeLastBlock(Arena *arena);
void ArenaFreeBlockAt(Arena *arena, int index);