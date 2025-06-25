#include "../include/trie.h"
#include <stdlib.h>
#include <string.h>

TrieNode* command_trie = NULL;

static void collectSuggestions(TrieNode* node, char*** matches, int* match_count) {
    if (node->isEndOfWord) {
        (*match_count)++;
        *matches = realloc(*matches, (*match_count) * sizeof(char*));
        (*matches)[*match_count - 1] = strdup(node->fullPath);
    }
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            collectSuggestions(node->children[i], matches, match_count);
        }
    }
}


TrieNode* createNode(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->isEndOfWord = false;
    node->fullPath = NULL;
    node->description = NULL;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}




void insert(TrieNode* root, const char* path, const char* desc) {
    TrieNode* current = root;
    for (int i = 0; path[i] != '\0'; i++) {
        int index = (int)path[i];
        if (!current->children[index]) {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = true;
    current->fullPath = strdup(path);
    if (desc) current->description = strdup(desc);
}

void freeTrie(TrieNode* root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }
    free(root->fullPath);
    free(root->description);
    free(root);
}

void findSuggestions(TrieNode* root, const char* prefix, char*** matches, int* match_count) {
    if (!root) return;
    
    TrieNode* current = root;
    for (int i = 0; prefix[i] != '\0'; i++) {
        int index = (int)prefix[i];
        if (!current->children[index]) return;
        current = current->children[index];
    }
    
    *matches = NULL;
    *match_count = 0;
    collectSuggestions(current, matches, match_count);
}

