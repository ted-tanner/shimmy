#ifndef TRIE_H

#include <stdint.h>
#include <stdlib.h>

#include "arena.h"

// [a-z0-9-.] allowed in domain
#define TRIE_NODE_SIZE 38
#define CHAR_INVALID_ERRNO 38

typedef struct _TrieNode {
    struct _TrieNode* next_nodes[TRIE_NODE_SIZE];
    _Bool is_end;
} _TrieNode;

typedef struct {
    Arena arena;
    _TrieNode* head;
} Trie;

typedef struct {
    char* key;
    size_t length;
} TrieKey;

Trie trie_create(Arena arena);
_Bool trie_insert(Trie trie, TrieKey key);
_Bool trie_contains(Trie trie, TrieKey key);

#define TRIE_H
#endif
