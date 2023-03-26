
// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"


// Global variables
trie_node *root;
int count_words = 0;

// Dictionary file
FILE *dicfile;


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
	// Open dictionary file
	dicfile = fopen(dictionary, "r");
	if (dicfile == NULL)
	{
		return false;
	}

	// Initialize root trie node
	root = get_node();

	// Read each word from dictionary
	char word[LENGTH + 1];
	while (fgets(word, LENGTH + 1, dicfile))
	{
		// Delete '\n' character at end of word
		word[strcspn(word, "\n")] = '\0';

		// skip empty lines
		if (word[0] != '\0')
		{

		// increase number of words
		count_words++;

		// add word to trie node
		add_word(root, word);
		}
	}

	return true;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
	if (word[0] == '\'')
	{
		return 26;
	}
	return toupper(word[0]) - 'A';
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
	return count_words;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
	trie_node *ptr = root;
	for (int i = 0, len = strlen(word); i < len; i++)
	{
		// I'm not allowed to change hash function so I'm creating a stupid string
		// that contains word[i] to pass as arg to hash()
		char stupidstr[2];
		stupidstr[0] = word[i];
		stupidstr[1] = '\0';
		int index = hash(stupidstr);
		if (ptr->children[index] == NULL)
		{
			return false;
		}
		ptr = ptr->children[index];
	}
	bool result = ptr->is_last_letter;
	return result;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
	if (root != NULL)
	{
		unload_trie(root);
		return true;
	}
	else
	{
		return false;
	}
}

// Returns a new trie node initialized to NULLs
trie_node *get_node(void)
{
	trie_node *node = malloc(sizeof(trie_node));
	node->is_last_letter = false;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		node->children[i] = NULL;
	}
	return node;
}


// Adds word to trie
void add_word(trie_node *node, char *word)
{
	trie_node *p = node;
	for (int i = 0, len = strlen(word); i < len; i++)
	{
		// I'm not allowed to change hash function so I'm creating a stupid string
		// that contains word[i] to pass as arg to hash()
		char stupidstr[2];
		stupidstr[0] = word[i];
		stupidstr[1] = '\0';
		int index = hash(stupidstr);
		if (p->children[index] == NULL)
		{
			p->children[index] = get_node();
		}
		p = p->children[index];
	}
	p->is_last_letter = true;
}

// Unload all trie nodes from memory
void unload_trie(trie_node *node)
{
	// Handle base case
	if (node == NULL)
	{
		return;
	}

	// Free memory for all nodes in trie struct recursively
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		unload_trie(node->children[i]);
	}
	free(node);
}
