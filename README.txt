Armon Azizi

A bioinformatics tool to cluster genomes into families.


NOTE: to compile, run: “make all”


There are 2 programs. These programs take a set of genomes, calculate the similarity/homology between each genome, build a network using that data and cluster the genomes into families using a graph clustering algorithm that I implemented.


I obtained my genome data from: ftp://ftp.ncbi.nih.gov/genomes/archive/old_refseq/Bacteria/


The files that I downloaded are the .ffn files for each bacterial genome. These small genomes can be analyzed using this tool on a standard laptop computer.


Skip to the end of this text for information on how to run the programs with the provided files.


How the programs work:


Program 1:


genomecompare.cpp 


Relies on:
GenomeTrie.cpp
GenomeTrie.h
TrieNode.cpp
TrieNode.h


How it works:


genomecompare  calculates the homology between a set of genomes given a sequence length to compare universally. It works by first building a multiway trie for each genome with a 
depth of the given sequence length. This trie will contain all sequences of that length from the genome. Then for each trie, the program will compare every other genome to the trie by splitting each genome into sequences of the given length and searching for them in trie. Homology is calculated by determining the percentage of mapped fragments out of the total number of fragments.


To determine the homology between two arbitrary genomes (for example: genome1 and genome2), genome1 is first mapped onto genome2’s trie to determine homology, then genome2 is mapped onto genome1’s trie to determine homology. The average of the two genome homologies is the total homology between them.


The program takes input in the following way:


./genomecompare genome_directory file_names.txt out_file.txt sequence_length


where:


genome_directory is the path to a directory where all of the genome files are stored, one file per genome. The genome files are in .fasta format. So if the genomes are in a directory called “genomes”, the first command would just be ‘genomes’. If the genomes are in the current directory, command line input would just be ‘/’.


file_names is the path to a text file with the titles of all of the genome files in genome_directory. One filename per line.


Format of file_names.txt:


genome1.ffn
genome2.ffn
genome3.ffn
….


out_file is the output file where homology percentages are written. The format of output_file is:


HEADER
genome1<TAB>genome2<TAB>%homology
genome1<TAB>genome3<TAB>%homology
...


sequence_length is the length of sequences to compare when determining the homology between genomes. Naturally, a longer sequence length will result in a smaller percentage of mapped reads. But, the length remains constant for all genomes and yields relative homologies between genomes that are accurate.












Program 2


findfamilies.cpp 


Relies on:
GenomeNetwork.cpp
GenomeNetwork.h
GenomeNode.cpp
GenomeNode.h


How it works:


findfamilies takes a txt file consisting of genomes and their homology percentage and calculates a set of families for the genomes based on their similarity with each other. The number of clusters/famlies is defined by the user.


The input to this program is the output of genomecompare.cpp


The program works by building a network of genomes. Each edge represents a percentage homology between the 2 genomes. It then performs a clustering algorithm on the network by trimming the lowest homology edges until the defined number of clusters are present in the network. The program returns a .txt file containing a list of families and a list of related genomes for each family.


The program takes input in the following way:


./findfamilies input_file.txt output_file.txt num_clusters


where:


input_file is a tab delimited file of genomes and homologies in the format:


GENOME1<TAB>GENOME2<TAB>HOMOLOGY_PERCENT
ecoli        salmonella        52.342
staph        strep        32.2376
...


output_file is the file to write to


num_clusters is the final number of families desired.






*************************
*************************
HOW TO RUN THE PROGRAMS


To compile the programs simply type: “make all”


The basic pipeline for the clustering looks like this:


(set of genomes in .fasta) ----[genomecompare.cpp]---> (genome_homologies.txt)-------[findfamilies.cpp]-------> (genome_families.txt).


To run genomecompare.cpp:


The sample genome fasta files are in a directory called genomes. The filenames.txt is also in the genome directory. We want to write to out.txt in the current directory, and we want to compare all sequences of length 10 between all genomes.


Type into the command line:


./genomecompare genomes genomes/filenames.txt out.txt 10


This will read all files in the genomes directory that have names contained in filenames.txt.


It will create a file called out.txt in the current directory with all homologies.


I have included 2 small sample genomes to run this program on. They are in a directory called genomes. I have included filenames.txt, also located in genomes. So simply run the above command to run the program. After it has completed, you can view out.txt and see the homology between the 2 genomes. Normally there would be a bunch of comparisons but there is only 1 since we are only comparing 2 genomes.








To run findfamilies.cpp: 


The input to findfamilies would normally be out.txt from running genomecompare.But since running genomecompare on a lot of genomes takes a while, I did it for you and have included it’s output in a file called homologies.txt. Take a look at homologies.txt to see what the similarities between different bacteria look like.


homologies.txt consists of bacteria from various families that are related. I wanted to see if my program could cluster these bacteria into their actual families by only looking at their genome sequence similarities.


So to run the program, the input will be homologies.txt, the output file can be whatever you want, for example families.txt. The final command line argument is the number of families we want to cluster to. I found that for the given dataset in homologies.txt, clustering into 9 families works well. So on the command line, type:


./findfamilies homologies.txt families.txt 9


This will cluster the genomes in homologies.txt into 9 families and write the clusters to families.txt. Open families.txt to see what the clusters/families look like.






Overall, the program performs well. It is able to cluster most of the bacteria accurately into their correct families only based on their genome sequences. Also, genomecomapre runs in O(n1 + n2) (where n1 is length of genome1 and n2 is length of genome2) time which is pretty good for mapping 2 genomes to each other.
