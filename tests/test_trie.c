#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "trie.h"

#define ASCII_GREEN "\e[32m"
#define ASCII_DEFAULT "\e[0m"

static void test_trie_create() {
    Arena arena = arena_create(4096);

    Trie trie = trie_create(arena);
    assert(trie.head);
    assert(trie.arena == arena);

    arena_free(arena);

    printf("%s%s%s - trie_create\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

static void test_trie_insert() {
    Arena arena = arena_create(4096);

    Trie trie = trie_create(arena);
    assert(trie.head);

    TrieKey key = { "test", 4 };
    _Bool result = trie_insert(trie, key);
    assert(result);

    arena_free(arena);

    printf("%s%s%s - trie_insert\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

static void test_trie_contains() {
    Arena arena = arena_create(8192);

    Trie trie = trie_create(arena);
    assert(trie.head);

    TrieKey key1 = { "www.example.co", 14 };
    TrieKey key2 = { "www.example.com", 15 };
    TrieKey key3 = { "www.example.org", 15 };

    _Bool result1 = trie_insert(trie, key1);
    _Bool result2 = trie_insert(trie, key2);
    _Bool result3 = trie_insert(trie, key3);

    assert(result1);
    assert(result2);
    assert(result3);

    TrieKey nonexistant_key1 = { "www.example.c", 13 };
    TrieKey nonexistant_key2 = { "www.example", 11 };
    TrieKey nonexistant_key3 = { "www", 3 };
    TrieKey nonexistant_key4 = { "www.example.comm", 16 };

    assert(trie_contains(trie, key1));
    assert(trie_contains(trie, key2));
    assert(trie_contains(trie, key3));

    assert(!trie_contains(trie, nonexistant_key1));
    assert(!trie_contains(trie, nonexistant_key2));
    assert(!trie_contains(trie, nonexistant_key3));
    assert(!trie_contains(trie, nonexistant_key4));

    assert(!trie_contains(trie, (TrieKey){ 0, 0 }));
    assert(!trie_contains(trie, (TrieKey){ "www.example.com", 0 }));

    arena_free(arena);

    printf("%s%s%s - trie_contains\n", ASCII_GREEN, "PASS", ASCII_DEFAULT);
}

int main() {
    test_trie_create();
    test_trie_insert();
    test_trie_contains();
    return 0;
}
