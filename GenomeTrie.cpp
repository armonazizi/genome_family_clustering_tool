/*
Armon Azizi
A91037300

CSE 100
PA4 Extension

GenomeTrie.cpp

This class is a multiway trie implementation
that can store dna sequences consisting of
'A' 'G' 'C' and 'T'.

The implementation allows for O(1) insert and search
*/

#include "GenomeTrie.h"
#include "TrieNode.h"

#include <string>
#include <iostream>

using namespace std;

GenomeTrie::GenomeTrie() {
	root = new TrieNode();
}

GenomeTrie::~GenomeTrie() {
	delete root;
}

/*
Add the given sequence to the trie.

precondition: the string must only consist of valid characters
*/
void GenomeTrie::addSequence(string &sequence) {

	//Find root
	TrieNode * currNode = root;

	//Iterate through trie, if nodes for sequence don't exist,
	//add nodes.
	for (char c : sequence) {

		int val = charVal(c);

		if (val == -1) return;
		
		if (currNode->nucleotides[val]) {
			
			currNode = currNode->nucleotides[val];
		}
		else {
			
			currNode->nucleotides[val] = new TrieNode();
			
			currNode = currNode->nucleotides[val];
		}
		
	}
}

//Return true if the trie contains the given sequence.
bool GenomeTrie::containsSequence(string &sequence) {

	TrieNode * currNode = root;

	//Iterate through trie, if next node isn't found, return false
	for (char c : sequence) {

		int val = charVal(c);

		if (val == -1) return false;

		if (currNode->nucleotides[val]) {
			currNode = currNode->nucleotides[val];
		}
		else {
			return false;
		}

	}

	return true;

}

//Assigns integer representaion (0-4) to each nucleotide.
//If character is not a nucleotide, return -1.
int GenomeTrie::charVal(char c) {

	if (c == 'A')
		return 0;
	else if (c == 'G')
		return 1;
	else if (c == 'C')
		return 2;
	else if (c == 'T')
		return 3;

	return -1;
}