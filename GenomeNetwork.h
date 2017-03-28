/*
Armon Azizi
A91037300

CSE100 PA4 Extension

GenomeNetwork.h

This class represents a network of genomes
related by how homologous they are to eachother.

The network is mad up of a set of GenomeNodes, each one represents
a different species. The nodes are connected to each other through
pointers with each pointer corresponding to a certain homology,
or degree of similarity.

Using the graph, genomes can be clustered into families based on
how related they are to each other. The clustering usis achieved using a markov-like
algorithm that removes the least homologous edges one by one until the number of clusters
is equivalent to the number predefined.

I use a clustering algorithm to cluster all of the species into 
a number of families predefined by the user.
*/

#ifndef GENOMENETWORK_H
#define GENOMENETWORK_H

#include "GenomeNode.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

//This is used to keep the list of homology edges in decreasing order
//so that they can be removed from the graph in order. During clustering
struct SetComp {

	//How to compare tuples
	bool operator()(const tuple<double, GenomeNode*, GenomeNode*> lhs, 
		const tuple<double, GenomeNode*, GenomeNode*> rhs) const {

		return get<0>(lhs) < get<0>(rhs);
	}
};

class GenomeNetwork {

private:
	//Hashmap of all nodes
	unordered_map<string, GenomeNode*> nodes;

	//Vector of all families in the network
	vector<pair<string, vector<GenomeNode *>>> families;

	/*
	Set representatinve of all edges in the network
	This set is used during clustering. It stores the homology connections
	in decreasing order so the they can be removed from the graph
	in decreasing order.
	*/
	set<tuple<double, GenomeNode*, GenomeNode*>> homologyConnections;


public:

	GenomeNetwork();

	~GenomeNetwork();

	//Add a connection between the two genomes
	void addSet(string genome1, string genome2, double homology);

	//Return true if the graph contains the node
	bool hasNode(string genome);

	//Add a homology edge to the homology set
	void addHomology(double homology, string gen1, string gen2);

	//Get the given node from the network and return it
	GenomeNode* getNode(string genome);

	//Return the vector of families that have been calculated from the tree.
	vector<pair<string, vector<GenomeNode *>>> getFamilyVector();

	//Clustering algorithm that results in num_clusters clusters of genomes.
	void cluster(int cluster_num);

	//Return the current number of clusters in the network
	int numClusters();

	//Set all nodes' searched variable to false
	void reset();

	//Print graph for debugging
	void print();

	//Return number of nodes
	int numNodes();


};


#endif // GENOMENETWORK_H