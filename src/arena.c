#include "arena.h"

Arena arena_create(size_t size) {
    size_t total_size = size + sizeof(_ArenaHeader);
    Arena arena = (uint8_t*) malloc(total_size);
    if (!arena) {
        return 0;
    }

    _ArenaHeader* header = (_ArenaHeader*) arena;
    header->end = arena + total_size;
    header->current = arena + sizeof(_ArenaHeader);

    return arena;
}

void* arena_alloc(Arena arena, size_t size) {
    _ArenaHeader* header = (_ArenaHeader*) arena;
    if (header->current + size > header->end) {
        return 0;
    }

    void* temp = header->current;
    header->current += size;
    return temp;
}

void* arena_alloc_zeroed(Arena arena, size_t size) {
    _ArenaHeader* header = (_ArenaHeader*) arena;
    if (header->current + size > header->end) {
        return 0;
    }

    void* temp = header->current;
    header->current += size;
    memset(temp, 0, size);
    return temp;
}

void arena_clear(Arena arena) {
    _ArenaHeader* header = (_ArenaHeader*) arena;
    header->current = arena + sizeof(_ArenaHeader);
}

void arena_clear_zeroed(Arena arena) {
    _ArenaHeader* header = (_ArenaHeader*) arena;
    header->current = arena + sizeof(_ArenaHeader);
    memset(header->current, 0, header->end - header->current);
}

ArenaUsage arena_usage(Arena arena) {
    ArenaUsage usage = {0};

    _ArenaHeader* header = (_ArenaHeader*) arena;
    usage.used_bytes = header->current - (arena + sizeof(_ArenaHeader));
    usage.capacity_bytes = header->end - (arena + sizeof(_ArenaHeader));
    usage.total_malloced_bytes = header->end - arena;

    return usage;
}

void arena_free(Arena arena) {
    free(arena);
}
