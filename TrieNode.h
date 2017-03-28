/*
Armon Azizi

TrieNode.h

This class represents a node in the GenomeTrie class.
Each node is a multiway trie node that can point to 4
different nucleotide nodes.
*/

#ifndef TRIENODE_H
#define TRIENODE_H

#include <string>
#include <vector>

using namespace std;

class TrieNode {

public:

	TrieNode();

	~TrieNode();

	//Stores 4 children, one fore each nucleotide.
	vector<TrieNode *> nucleotides;

};


#endif // TRIENODE_H