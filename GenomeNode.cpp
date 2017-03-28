/*
Armon Azizi

GenomeNode.cpp

This class represents a node that is stored in the GenomeNetwork
class. It represents a single genome and points to all
of the genomeNodes that this genome is homologous to.
*/

#include "GenomeNode.h"

#include <vector>
#include <iostream>
#include <tuple>

using namespace std;

GenomeNode::GenomeNode(string genomeName) {
	name = genomeName;
	searched = false;
	beenAdded = false;
}

GenomeNode::~GenomeNode() {
}

/*
Add the given node to the list of neighbors with the given homology
Neighbors are stored as tuples.
*/
void GenomeNode::addNeighbor(GenomeNode * neighbor, double homology) {

	tuple<GenomeNode *, double, bool> newTup;

	get<0>(newTup) = neighbor;
	get<1>(newTup) = homology;
	get<2>(newTup) = true;

	neighbors.push_back(newTup);
}

/*
Recursively set all nodes in the same cluster to true
*/
void GenomeNode::clear() {

	searched = true;

	//loop through all neighbors and clear if they have not
	//been searched yet
	for (auto i : neighbors) {
		if (get<2>(i)) {
			if((get<0>(i))->searched == false)
				(*(get<0>(i))).clear();
		}

	}

}

/*
Remove the given node from neighbors.

Removal means setting the validity boolean in
the corresponding tuple to false.
*/
void GenomeNode::removeNode(GenomeNode * node, double hom) {

	//If neighbor is valid, check if name is equivalent
	//Then, invaludate neighbor.
	for (auto &t : neighbors) {
		if (get<2>(t)) {

			if (((get<0>(t))->name == node->name)) {
				get<2>(t) = false;
			}

		}
	}

}

/*
Recursively add all nodes in the cluster to the given vector.
*/
void GenomeNode::addFamily(vector<GenomeNode *> &vec) {

	//Add this to vector
	vec.push_back(this);
	beenAdded = true;

	//If neighbor is valid and has not been added
	//Add its cluster to vector.
	for (auto a : neighbors) {
		if (get<2>(a)) {
			if (!((get<0>(a))->beenAdded)) {
				(*(get<0>(a))).addFamily(vec);
			}
		}

	}
}