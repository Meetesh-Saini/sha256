all: hash

hash: basics.o hash.cpp
	g++ -o hash hash.cpp basics.o

basics.o: basics.cpp
	g++ -g -c basics.cpp