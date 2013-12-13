#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H
#include <exception>
#include <map>
#include <set>

using std::exception;
using std::map;
using std::set;

class VirusAlreadyCreated : public exception {};

class VirusNotFound : public exception {};

class TriedToRemoveStemVirus : public exception {};

template<class Virus>
class VirusGenealogy {
public:
	//TODO methods
private:
	//FIXME/TODO I think node should be a private nested class and a node
	//should contain shared_ptrs to its' sons
	typedef set<Virus::id_type> Node;
	typedef map<Virus::id_type, Node> Graph;
	Graph nodes;
};

#endif