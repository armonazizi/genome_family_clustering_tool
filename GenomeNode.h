/*
Armon Azizi

GenomeNode.h

This class represents a node that is stored in the GenomeNetwork
class. It represents a single genome and points to all
of the genomeNodes that this genome is homologous to.
*/

#ifndef GENOMENODE_H
#define GENOMENODE_H

#include <string>
#include <vector>
#include <set>

using namespace std;

class GenomeNode {

public:

	GenomeNode(string genomeName);

	~GenomeNode();

	//Species name
	string name;

	//True if this node has been searched for in numClusters()
	bool searched;

	//True if this node has been added to a family.
	bool beenAdded;

	//Vector of pointers and corresponding homologies to all neighbors
	vector<tuple<GenomeNode *, double, bool>> neighbors;

	//Adds a node neighbor with given homology
	void addNeighbor(GenomeNode * neighbor, double homology);

	//Sets searched to true for all nodes in this cluster
	void clear();

	//Remove the given node from neighbors
	void removeNode(GenomeNode * node, double hom);

	//Add this node and all of its children to a family.
	void addFamily(vector<GenomeNode *> &vec);
	

};


#endif // GENOMENODE_H