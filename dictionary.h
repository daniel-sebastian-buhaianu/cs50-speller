// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45
#define ALPHABET_SIZE 27 // A-Z + apostroph

typedef struct trie_node {
	struct trie_node *children[ALPHABET_SIZE + 1];
	bool is_last_letter;
}
trie_node;

// Prototypes
trie_node *get_node(void);
bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
void unload_trie(trie_node *node);
bool unload(void);
void add_word(trie_node *node, char *word);

#endif // DICTIONARY_H
