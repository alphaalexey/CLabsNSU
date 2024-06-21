#pragma once

typedef struct Trie Trie;

Trie *InitTrie(void);
void FreeTrie(Trie *trie);
int GetTrieCount(const Trie *trie);
void TrieInsert(Trie *trie, const char *key);
void TrieSearchAndPrint(Trie *trie, const char *prefix);
