#ifndef ARENA_H

#include <stdlib.h>
#include <strings.h>

typedef uint8_t* Arena;

typedef struct {
    uint8_t* end;
    uint8_t* current;
} _ArenaHeader;

typedef struct {
    size_t used_bytes;
    size_t capacity_bytes;

    size_t total_malloced_bytes;
} ArenaUsage;

Arena arena_create(size_t size);
void* arena_alloc(Arena arena, size_t size);
void* arena_alloc_zeroed(Arena arena, size_t size);
void arena_clear(Arena arena);
void arena_clear_zeroed(Arena arena);
ArenaUsage arena_usage(Arena arena);
void arena_free(Arena arena);

#define ARENA_H
#endif
