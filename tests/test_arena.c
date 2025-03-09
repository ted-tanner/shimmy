#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "arena.h"

#define ASCII_GREEN "\e[32m"
#define ASCII_DEFAULT "\e[0m"

static void test_arena_create_free() {
    Arena arena = arena_create(8192);
    assert(arena);

    _ArenaHeader* header = (_ArenaHeader*) arena;
    assert(header->end == arena + 8192 + sizeof(_ArenaHeader));
    assert(header->current == arena + sizeof(_ArenaHeader));

    arena_free(arena);

    printf("%s%s%s - arena_create_free\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

static void test_arena_alloc() {
    Arena arena = arena_create(8192);
    assert(arena);

    _ArenaHeader* header = (_ArenaHeader*) arena;
    assert(header->current == arena + sizeof(_ArenaHeader));

    void* ptr = arena_alloc(arena, 1024);
    assert(ptr == arena + sizeof(_ArenaHeader));

    assert(header->current == arena + sizeof(_ArenaHeader) + 1024);

    ptr = arena_alloc(arena, 512);
    assert(ptr == arena + sizeof(_ArenaHeader) + 1024);

    assert(header->current == arena + sizeof(_ArenaHeader) + 1024 + 512);

    arena_free(arena);

    printf("%s%s%s - arena_alloc\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

static void test_arena_alloc_zeroed() {
    Arena arena = arena_create(8192);
    assert(arena);

    _ArenaHeader* header = (_ArenaHeader*) arena;
    assert(header->current == arena + sizeof(_ArenaHeader));

    uint64_t* arena_start = (uint64_t*) header->current;

    for (uint64_t* curr = arena_start; (uint8_t*) curr < header->end; ++curr) {
        *curr = 0xFFFFFFFF;
    }

    void* ptr = arena_alloc_zeroed(arena, 1024);
    assert(ptr);

    assert(header->current == arena + sizeof(_ArenaHeader) + 1024);

    for (uint64_t* curr = (uint64_t*) ptr; (uint8_t*) curr < (uint8_t*) ptr + 1024; ++curr) {
        assert(*curr == 0);
    }

    arena_free(arena);

    printf("%s%s%s - arena_alloc_zeroed\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

static void test_arena_clear() {
    Arena arena = arena_create(8192);
    assert(arena);

    _ArenaHeader* header = (_ArenaHeader*) arena;
    assert(header->current == arena + sizeof(_ArenaHeader));

    void* ptr = arena_alloc(arena, 1024);
    assert(ptr);

    assert(header->current == arena + sizeof(_ArenaHeader) + 1024);

    arena_clear(arena);

    assert(header->current == arena + sizeof(_ArenaHeader));

    ptr = arena_alloc(arena, 1024);
    assert(ptr == arena + sizeof(_ArenaHeader));
    assert(header->current == arena + sizeof(_ArenaHeader) + 1024);

    arena_free(arena);

    printf("%s%s%s - arena_clear\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

static void test_arena_clear_zeroed() {
    Arena arena = arena_create(8192);
    assert(arena);

    _ArenaHeader* header = (_ArenaHeader*) arena;
    assert(header->current == arena + sizeof(_ArenaHeader));

    void* ptr = arena_alloc(arena, 1024);
    assert(ptr);

    assert(header->current == arena + sizeof(_ArenaHeader) + 1024);

    for (uint64_t* curr = (uint64_t*) header->current; (uint8_t*) curr < header->end; ++curr) {
        *curr = 0xFFFFFFFF;
    }

    arena_clear_zeroed(arena);

    assert(header->current == arena + sizeof(_ArenaHeader));

    for (uint64_t* curr = (uint64_t*) header->current; (uint8_t*) curr < header->end; ++curr) {
        assert(*curr == 0);
    }

    ptr = arena_alloc(arena, 1024);
    assert(ptr == arena + sizeof(_ArenaHeader));
    assert(header->current == arena + sizeof(_ArenaHeader) + 1024);

    arena_free(arena);

    printf("%s%s%s - arena_clear_zeroed\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

int main() {
    test_arena_create_free();
    test_arena_alloc();
    test_arena_alloc_zeroed();
    test_arena_clear();
    test_arena_clear_zeroed();
    return 0;
}
