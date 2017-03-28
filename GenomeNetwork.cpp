/*
Armon Azizi

GenomeNetwork.cpp

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

#include "GenomeNetwork.h"
#include "GenomeNode.h"

#include <vector>
#include <set>
#include <iostream>

using namespace std;

GenomeNetwork::GenomeNetwork() {
}

//Delete all nodes from the graph.
GenomeNetwork::~GenomeNetwork() {
	for (auto i : nodes) {
		delete i.second;
	}
}

/*
Add a connection between the two specified genomes with the given homology

If the nodes don't already exist in the graph, create them and add connection.
*/
void GenomeNetwork::addSet(string genome1, string genome2, double homology) {

	GenomeNode * temp1;
	GenomeNode * temp2;
	
	//Check if node exists in graph and if not, add new node.
	if (hasNode(genome1)) {
		temp1 = nodes[genome1];
	}
	else {
		temp1 = new GenomeNode(genome1);

		nodes[genome1] = temp1;
	}

	if (hasNode(genome2)) {
		temp2 = nodes[genome2];
	}
	else {
		temp2 = new GenomeNode(genome2);

		nodes[genome2] = temp2;
	}

	//Create connection between the 2 nodes
	(*temp1).addNeighbor(temp2, homology);
	(*temp2).addNeighbor(temp1, homology);

}

//Add a homology edge to the list of homologies.
void GenomeNetwork::addHomology(double homology, string gen1, string gen2) {

	tuple<double, GenomeNode*, GenomeNode*> newTuple;

	get<0>(newTuple) = homology;
	get<1>(newTuple) = getNode(gen1);
	get<2>(newTuple) = getNode(gen2);

	homologyConnections.insert(newTuple);
}

/*
Remove edges from the graph until there are num_clusters clusters
in the graph. 

Then, find all families in the graph and add them to teh family vector
*/
void GenomeNetwork::cluster(int cluster_num) {

	//Remove edges until number of clusters is big enough
	while (numClusters() < cluster_num) {
		
		//Find smallest edge
		auto hom = *(homologyConnections.begin());
		
		int homology = get<0>(hom);

		GenomeNode * gen1 = get<1>(hom);
		GenomeNode * gen2 = get<2>(hom);
		
		//Remove smallest edge from its 2 nodes
		(*gen1).removeNode(gen2, homology);
		
		(*gen2).removeNode(gen1, homology);
		
		//Remove edge from edge set
		homologyConnections.erase(homologyConnections.begin());
		
	}

	//Store nodes in vector so they can be iterated through easily
	vector<GenomeNode *> nodeVec;

	for (auto g : nodes) {
		nodeVec.push_back(g.second);
	}

	//Add each family to the family vector
	for (int i = 0; i < numClusters(); ++i) {

		pair<string, vector<GenomeNode *>> temp;

		temp.first = "Family " + to_string(i);

		vector<GenomeNode *> newVec;
		
		for (auto i : nodeVec) {
			
			if (!(i->beenAdded)) {
				
				//Add nodes recursively to the vector
				(*i).addFamily(newVec);
				break;
			}
		}

		//Add information to family vector
		temp.second = newVec;

		families.push_back(temp);

	}

}

//Returns the number of clusters in the graph.
int GenomeNetwork::numClusters() {

	int numClusters = 0;

	unordered_map<string, GenomeNode*> temp = nodes;

	vector<GenomeNode *> nodeVec;

	//Create a vector of all nodes to iterate easily
	for (auto g : nodes) {
		nodeVec.push_back(g.second);
	}

	//For all nodes, clear cluster by cluster.
	//Each time 1 cluster is cleared, increment number of clusters
	for (auto i : nodeVec) {
		if (!(i->searched)) {
			numClusters++;
			(*i).clear();
		}
	}

	//Reset all searched values.
	reset();

	return numClusters;

}

//Returns vector of families.
vector<pair<string, vector<GenomeNode *>>> GenomeNetwork::getFamilyVector() {
	return families;
}

//Returns pointer to given node
//precondition: node must exist in network
GenomeNode* GenomeNetwork::getNode(string genome) {
	auto itr = nodes.find(genome);

	return (*itr).second;
}

//Return true if node exists in network
bool GenomeNetwork::hasNode(string genome) {
	auto itr = nodes.find(genome);

	return itr != nodes.end();
}

//Set all searched values to false
void GenomeNetwork::reset() {
	for (auto i : nodes)
		(i.second)->searched = false;
}

//Print graph for debugging
void GenomeNetwork::print() {

	cout << "PRINTING" << endl;
	for (auto i : nodes) {
		/*cout << i.second->name;

		for (auto j : i.second->neighbors)
			if (get<2>(j))
				cout << get<0>(j)->name << endl;

		cout << endl;*/

		//cout << i.second->searched << endl;
	}
	cout << "END" << endl;
}


//Return number of nodes
int GenomeNetwork::numNodes() {
	return nodes.size();
}