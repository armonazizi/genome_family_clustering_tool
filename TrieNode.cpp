/*
Armon Azizi
A91037300

CSE 100
PA4 Extension

TrieNode.cpp

This class represents a node in the GenomeTrie class.
Each node is a multiway trie node that can point to 4 
different nucleotide nodes.
*/

#include "TrieNode.h"

#include <vector>

using namespace std;

TrieNode::TrieNode() {

	//declare the nucleotide vector
	nucleotides = vector<TrieNode *>(4);

	//instantiate all pointers to null
	for (int i = 0; i < 4; ++i)
		nucleotides[i] = nullptr;
}

//deconstructor
TrieNode::~TrieNode() {
	for (int i = 0; i < 4; ++i)
		delete nucleotides[i];
}