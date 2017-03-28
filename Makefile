# A simple makefile

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: genomecompare findfamilies

genomecompare: GenomeTrie.o TrieNode.o

findfamilies: GenomeNode.o GenomeNetwork.o

clean:
	rm -f pathfinder *.o core*

