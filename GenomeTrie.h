/*
Armon Azizi

GenomeTrie.h

This class is a multiway trie implementation
that can store dna sequences consisting of
'A' 'G' 'C' and 'T'.

The implementation allows for O(1) insert and search
*/

#ifndef GENOMETRIE_H
#define GENOMETRIE_H

#include "TrieNode.h"

#include <string>
#include <vector>

using namespace std;

class GenomeTrie {

public:

	GenomeTrie();

	~GenomeTrie();

	//Store the root node
	TrieNode * root;

	/*
	Add the given sequence to the trie.

	precondition: the string must only consist of valid characters
	*/
	void addSequence(string &sequence);

	//Return an integer representation of the character
	//Return -1 if cahracter is not valid.
	int charVal(char c);

	//Return true if the trie contains the given sequence.
	bool containsSequence(string &sequence);

};


#endif // GENOMETRIE_H