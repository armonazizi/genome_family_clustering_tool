/*
Armon Azizi

genomecompare.cpp

This program calculates the homology between a set of genomes
given a sequence length to compare universally.

It works by first building a genome multiway trie for each genome with a 
depth of the given sequence length. This trie will contain all
sequences of that length from the genome.

Then for each trie, the program will compare every other genome
to the trie by splitting each genome into sequences of the given
length and searching for them in trie.

To determine the homology between two arbitrary genomes
ex. genome1 and genome2, genome1 is first mapped onto genome2 to 
determine homology, then genome2 is mapped onto genome1 to determine homology.
the average of the two genomes is the total homology between them.

The program takes input in the following way:

./genomecompare genome_directory file_names out_file sequence_length

where:

genome_directory is the path to a directory where all of the genome files
are stored, one file per genome. The genome files are in .fasta format.

file_names is a text file with the titles of all of the genome files.
one file name per line.

out_file is the output file where homology percentages are output.
the format of output_file is:

genome1<TAB>genome2<TAB>%homology
genome1<TAB>genome3<TAB>%homology
...

sequence_length is the length of sequences to compare
when determining the homology between genomes.
naturally, a longer sequence length will result in a
smaller percentage of mapped reads. But, the length remains constant 
for all genomes and yields relative homologies between genomes
that are somewhat accurate.


*/

#include "GenomeTrie.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

/*
given a path to a fasta file containing a genome, build a 
GenomeTrie that contains all of its sequences. The trie will only
contain sequences of length seqLen.
*/
void buildTrie(string fileName, GenomeTrie &trie, int seqLen) {

	//Read fasta file
	ifstream infile(fileName);
	
	string tempSequence = "";

	//read file line by line, and parse in into the correct length sequences.
	while (infile) {

		string s;

		// get the next line
		if (!getline(infile, s)) break;
		
		//skip fasta header lines
		if (s[0] == '>') continue;

		//Add current sequence to temp
		tempSequence += s;

		//If the temp is shorter than the length we want, read another line.
		if (tempSequence.size() < seqLen) continue;

		//While the temp is longer than the sequence we want,
		//remove sequence from front of temp and add it to the trie.
		while (tempSequence.size() > seqLen) {
			
			string newSeq = tempSequence.substr(0, seqLen);
			
			trie.addSequence(newSeq);
			
			tempSequence = tempSequence.substr(1);
		}

	}

	infile.close();

}

/*
Given a built genometrie and a path to a fasta file of the genome
that we want to map to the trie, return the proportion of mapped reads
contained in the genome of the given length.
*/
double getMappedPercentage(string fileName, GenomeTrie &trie, int seqLen) {

	double numMappedReads = 0;
	double totalReads = 0;


	ifstream infile(fileName);

	string tempSequence = "";

	//Read file line by line.
	while (infile) {

		string s;

		// get the next line
		if (!getline(infile, s)) break;


		//skip fasta header lines
		if (s[0] == '>') continue;


		tempSequence += s;

		if (tempSequence.size() < seqLen) continue;

		//Read next sequence and seqch for it in the trie.
		while (tempSequence.size() > seqLen) {

			string newSeq = tempSequence.substr(0, seqLen);

			if (trie.containsSequence(newSeq))
				++numMappedReads;

			++totalReads;

			tempSequence = tempSequence.substr(seqLen);

		}

	}

	//return the number of mapped reads over the number of reads searched for in the trie.
	return (double)(numMappedReads / totalReads);

	infile.close();

}

/*
Given a path to the directory that all of the files are stored in and the
titla of a file that contains all of the fasta file titles,
return a vector of strings that contains the path to each fasta file.
*/
vector<string> getFileNames(string genome_directory, string file_names) {
	
	vector <string> result;

	ifstream infile(file_names);

	//Read title file line by line
	while (infile) {

		string s;

		// get the next line
		if (!getline(infile, s)) break;

		//Strip newline character from file title
		//s.erase(s.find_last_not_of("\n") + 1);
		s.pop_back();

		if (s == "") break;

		//new complete path to file.
		string newName = genome_directory + '/' + s;

		result.push_back(newName);
	}

	return result;
}

/*
Writes all of the values in a matrix of proportions to the given out file.
*/
void writeValues(string out, vector<string> files, vector<vector<double>> values) {

	ofstream outFile(out);

	int numFiles = files.size();

	//Write Header
	outFile << "Genome1\tGenome2\tHomology Percent" << endl;

	//visit each location in matrix and write 
	//values to file.
	for (int i = 0; i < numFiles; ++i) {

		for (int j = i + 1; j < files.size(); ++j) {

			double percent = ((values[i][j] + values[j][i]) / 2) * 100;

			string newLine = files[i] + '\t' + files[j] + '\t' + to_string(percent);

			outFile << newLine << endl;
			outFile.flush();
		}

	}

	outFile.close();

}

/*
Get file names, then for each genome, build a trie and 
compare all other genomes to it. Calculate all homologies and
write them to the output file.
*/
int main(int argc, char** argv) {

	string genome_directory = argv[1];
	string file_names = argv[2];
	string out_file = argv[3];
	int sequence_length = atoi(argv[4]);

	cout << "getting file names" << endl;

	//Get all genome fasta file paths
	vector<string> files = getFileNames(genome_directory, file_names);

	int numFiles = files.size();

	//Create matrix to store all homology values
	vector<vector<double>> values(numFiles, std::vector<double>(numFiles, 0));
	
	//Compare every genome to every other genome to determine homology.
	for (unsigned int i = 0; i < files.size(); ++i) {

		GenomeTrie trie;

		string file1 = files[i];

		//Build a trie for genome i
		cout << "Building trie for :" << file1 << endl;
		buildTrie(file1, trie, sequence_length);


		for (unsigned int j = 0; j < files.size(); ++j) {

			if (j != i) {
				string file2 = files[j];

				//Calculate homology between genome j and genome i
				cout << "Calculating Homology For: " << file1 << " " << file2 << endl;
				values[i][j] = getMappedPercentage(file2, trie, sequence_length);
				cout << values[i][j] << endl;
			}
		}
	}

	//Write homology values to the file.
	writeValues(out_file, files, values);

	cout << "homology calculated and written to file" << endl;


}