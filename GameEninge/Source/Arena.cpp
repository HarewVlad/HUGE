#include "Arena.h"
#include "Utils.h"

MakeInternal void ArenaGrow(Arena *arena, size_t size) {
  size_t new_size = ALIGN_UP(MAX(arena->block_size, size), ALIGN);
  MemoryBlock *memory_block = (MemoryBlock *)malloc(sizeof(MemoryBlock));
  memory_block->base = (unsigned char *)malloc(new_size);
  memory_block->ptr = memory_block->base;
  memset(memory_block->ptr, NULL, new_size);
  memory_block->end = memory_block->ptr + new_size;
  memory_block->prev = arena->current_block;
  memory_block->next = nullptr;
  memory_block->size = size;
  if (arena->current_block != nullptr) {
    arena->current_block->next = memory_block;
  }
  arena->current_block = memory_block;
  arena->block_count++;
}

MakeInternal unsigned char *ArenaAlloc(Arena *arena, size_t size) {
  if (arena->current_block == nullptr || size > (size_t)(arena->current_block->end - arena->current_block->ptr)) {
    ArenaGrow(arena, size);
  }
  unsigned char *ptr = arena->current_block->ptr;
  arena->current_block->ptr = (unsigned char *)ALIGN_UP_PTR(arena->current_block->ptr + size, ALIGN);
  return ptr;
}

MakeInternal void ArenaFree(Arena *arena) {
  while (arena->current_block) {
    MemoryBlock *memory_block = arena->current_block;
    free(memory_block->base);
    arena->current_block = memory_block->prev;
    free(memory_block);
  }
}

MakeInternal void ArenaFreeLastBlock(Arena *arena) {
  MemoryBlock *block_to_delete = arena->current_block;
  
  arena->current_block = arena->current_block->prev;

  free(block_to_delete->base);
  free(block_to_delete);
  
  arena->block_count--;
}

MakeInternal void ArenaFreeBlockAt(Arena *arena, int index) {
  ASSERT(arena->block_count > index);

  MemoryBlock *block_to_delete = arena->current_block;

  for (int i = index; i < arena->block_count - 1; i++) {
    block_to_delete = block_to_delete->prev;
  }

  if (block_to_delete->prev != nullptr) {
    block_to_delete->prev->next = block_to_delete->next;
    if (block_to_delete->next != nullptr) {
      block_to_delete->next->prev = block_to_delete->prev;
    }
  }

  free(block_to_delete->base);
  free(block_to_delete);

  arena->block_count--;
}