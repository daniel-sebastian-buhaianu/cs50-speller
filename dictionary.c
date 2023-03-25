// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"


trie_node *root;

// Dictionary file
FILE *dicfile;

// Returns a new trie node initialized to NULLs
trie_node *get_node(void)
{
	trie_node *n = malloc(sizeof(trie_node));
	n->is_last_letter = false;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		n->children[i] = NULL;
	}
	return n;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
	trie_node *ptr = root;
	for (int i = 0, len = strlen(word) - 1; i < len; i++)
	{
		// I'm not allowed to change hash function so I'm creating a stupid string
		// that contains word[i] to pass as arg to hash()
		char stupidstr[2];
		stupidstr[0] = word[i];
		stupidstr[1] = '\0';
		int index = hash(stupidstr);
		if (!ptr->children[index])
		{
			return false;
		}
		ptr = ptr->children[index];
	}
	bool result = ptr->is_last_letter;
	return result;
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

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
	dicfile = fopen(dictionary, "r");
	if (dicfile == NULL)
	{
		return false;
	}
	// Initialize root trie node
	root = get_node();
	return true;
}

// Adds word to trie structure
void add_word(trie_node *ptr, char *word)
{
	trie_node *p = ptr;
	for (int i = 0, len = strlen(word) - 1; i < len; i++)
	{
		char stupidstr[2];
		stupidstr[0] = word[i];
		stupidstr[1] = '\0';
		int index = hash(stupidstr);
		if (!p->children[index])
		{
			p->children[index] = get_node();
		}
		p = p->children[index];
	}
	p->is_last_letter = true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
	int count = 0;
	char word[LENGTH + 1];
	if (dicfile == NULL)
	{
		return 0;
	}
	while (fgets(word, LENGTH + 1, dicfile))
	{
		// increase number of words
		count++;

		// add word to trie
		add_word(root, word);
	}
	return count;
}

void unload_trie(trie_node *ptr)
{
	// Handle base case
	if (ptr == NULL)
	{
		return;
	}

	// Free memory for all nodes in trie struct recursively
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		unload_trie(ptr->children[i]);
	}
	free(ptr);
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
