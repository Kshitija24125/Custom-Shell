#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 128 // For ASCII characters

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    char* fullPath; // Stores complete path for leaf nodes
    char* description; // Optional: for command descriptions
} TrieNode;

extern TrieNode* command_trie;  

TrieNode* createNode();
void insert(TrieNode* root, const char* path, const char* desc);
void freeTrie(TrieNode* root);
void findSuggestions(TrieNode* root, const char* prefix, char*** matches, int* match_count);

#endif