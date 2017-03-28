/*
Armon Azizi

findfamilies.cpp

This program takes a txt file consisting of genomes and their homology
percentage and calculates a set of families for the genomes based on their
similarity with each other. The number of clusters is defined by the user.

The input to this program is the output of genomecompare.cpp

The program works by building a network of genomes connected by their homologies.
It then performes a clustering algorithm on the network by trimming
the least relevant nodes until the defined number of clusters are present in the network.

The program returns a txt file containing a list of families and a list of
related genomes for each family.

The program takes input in the following way:

./findfamilies input_file.txt output_file.txt num_clusters

where:

input_file is a tab delimited file of genomes and homologies in the format:

GENOME1<TAB>GENOME2<TAB>HOMOLOGY_PERCENT
ecoli	salmonella	52.342
staph	strep	32.2376
...

output_file is the file to write to

and num_clusters is the final number of families.

*/

#include "GenomeNetwork.h"
#include "GenomeNode.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

//Build the graph reading input line by line
void buildGraph(string fileName, GenomeNetwork &net) {

	ifstream in(fileName);

	string gen1;
	string gen2;
	double homology;

	string line;

	//skip header
	getline(in, line);

	//Read every line
	while (in) {

		getline(in, line);

		if (line == "")
			break;

		istringstream s(line);

		string tempHom;
		
		//parse line into genomes and homology
		getline(s, gen1, '\t');
		getline(s, gen2, '\t');
		getline(s, tempHom, '\t');

		homology = atof(tempHom.c_str());

		//Add set of nodes to network
		net.addSet(gen1, gen2, homology);

		//Add homology edge to network
		net.addHomology(homology, gen1, gen2);

	}
}

//Write all families to the output file
void writeFile(string outFile, vector<pair<string, vector<GenomeNode *>>> families) {

	ofstream out(outFile);

	for (auto i : families) {

		//Write family name
		out << i.first << endl;
		out.flush();

		//Write all species in the family
		for (auto j : i.second) {
			out << j->name << endl;
			out.flush();
		}

		out << endl;
		out.flush();

	}

	out.close();
}

int main(int argc, char** argv) {

	string in_file = argv[1];
	string out_file = argv[2];
	int num_clusters = atoi(argv[3]);

	GenomeNetwork geneNet;

	//Build the network
	cout << "Building Network" << endl;
	buildGraph(in_file, geneNet);

	if (num_clusters > geneNet.numNodes()) {
		cout << "number of clusters must be smaller than number of nodes!" << endl;
		return -1;
	}

	//Find num_clusters families in the network
	cout << "Finding families" << endl;
	geneNet.cluster(num_clusters);

	//get list of families
	auto families = geneNet.getFamilyVector();

	//write families to file
	writeFile(out_file, families);

	cout << "Clusters calculated and output to file!" << endl;
}