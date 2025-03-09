#include "trie.h"

static inline size_t charidx(char c) {
    // Using a massive switch statement to avoid branching (assuming the compiler is smart enough)
    switch (c) {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'a':
    case 'A':
        return 10;
    case 'b':
    case 'B':
        return 11;
    case 'c':
    case 'C':
        return 12;
    case 'd':
    case 'D':
        return 13;
    case 'e':
    case 'E':
        return 14;
    case 'f':
    case 'F':
        return 15;
    case 'g':
    case 'G':
        return 16;
    case 'h':
    case 'H':
        return 17;
    case 'i':
    case 'I':
        return 18;
    case 'j':
    case 'J':
        return 19;
    case 'k':
    case 'K':
        return 20;
    case 'l':
    case 'L':
        return 21;
    case 'm':
    case 'M':
        return 22;
    case 'n':
    case 'N':
        return 23;
    case 'o':
    case 'O':
        return 24;
    case 'p':
    case 'P':
        return 25;
    case 'q':
    case 'Q':
        return 26;
    case 'r':
    case 'R':
        return 27;
    case 's':
    case 'S':
        return 28;
    case 't':
    case 'T':
        return 29;
    case 'u':
    case 'U':
        return 30;
    case 'v':
    case 'V':
        return 31;
    case 'w':
    case 'W':
        return 32;
    case 'x':
    case 'X':
        return 33;
    case 'y':
    case 'Y':
        return 34;
    case 'z':
    case 'Z':
        return 35;
    case '-':
        return 36;
    case '.':
        return 37;
    }

    return CHAR_INVALID_ERRNO;
}

Trie trie_create(Arena arena) {
    Trie trie = {0};

    _TrieNode* head = arena_alloc_zeroed(arena, sizeof(_TrieNode));
    if (!head) {
        return trie;
    }

    trie.arena = arena;
    trie.head = head;

    return trie;
}

_Bool trie_insert(Trie trie, TrieKey key) {
    _TrieNode* prev_node = trie.head;
    _TrieNode* curr_node = 0;

    for (size_t pos = 0; pos < key.length; ++pos, prev_node = curr_node) {
        size_t cidx = charidx(key.key[pos]);
        if (cidx == CHAR_INVALID_ERRNO) {
            return 0;
        }

        curr_node = prev_node->next_nodes[cidx];

        if (!curr_node) {
            curr_node = arena_alloc_zeroed(trie.arena, sizeof(_TrieNode));
            if (!curr_node) {
                return 0;
            }

            prev_node->next_nodes[cidx] = curr_node;
        }

        if (pos == key.length - 1) {
            curr_node->is_end = 1;
        }
    }

    curr_node->is_end = 1;

    return 1;
}

_Bool trie_contains(Trie trie, TrieKey key) {
    _TrieNode* curr_node = trie.head;
    for (size_t pos = 0; pos < key.length; ++pos) {
        size_t cidx = charidx(key.key[pos]);
        if (cidx == CHAR_INVALID_ERRNO) {
            return 0;
        }

        _TrieNode* next_node = curr_node->next_nodes[cidx];
        if (!next_node) {
            return 0;
        }

        curr_node = next_node;
    }

    return curr_node->is_end;
}
