#include <map>
#include <set>
#include "network.h"
#include "growingnet.h"

using std::map;
using std::set;
using std::tuple;
using std::get;

const unsigned short int IN_NODES = 0;
const unsigned short int OUT_NODES = 1;
const unsigned short int NODES = 0;
const unsigned short int LINKS_NO = 1;
const unsigned short int GROWING = 2;

typedef const char* Node;
typedef tuple<set<Node>, set<Node> > NodeInfo;
typedef tuple<map<Node, NodeInfo>, size_t, int> Graph;
typedef map<unsigned long, Graph>::iterator GraphsIterator;

static unsigned long netId = 0;
static bool initialized = false;

static map<unsigned long, Graph>& getGraphs() {
	static map<unsigned long, Graph> graphs;

	//przed jakąkolwiek pierwszą operacją, trzeba stworzyć GSR
	if (initialized == false) {
		graphs.emplace(growingnet, Graph());
		get<GROWING>(graphs.at(growingnet)) = 1;
		initialized = true;
	}
	return graphs;
}
//id sieci -> wierzchołki sieci -> (para 2 setów - wierzchołki do których jest
//od nas krawędź oraz te, z których jest krawędź do nas)

unsigned long network_new(int growing) {
	//dopóki nie trafimy na wolne id, zwiększamy
	while (getGraphs().find(netId) != getGraphs().end())
		++netId;

	//wstawiamy pusty graf
	getGraphs().emplace(netId, Graph());
	//inicjujemy growing
	get<GROWING>(getGraphs().at(netId)) = growing;
	return netId;
}

void network_delete(unsigned long id) {
	getGraphs().erase(id);
}

size_t network_nodes_number(unsigned long id) {
	GraphsIterator iter = getGraphs().find(id);
	return (iter != getGraphs().end()) ? get<NODES>(iter->second).size() : 0;
}

size_t network_links_number(unsigned long id) {
	GraphsIterator iter = getGraphs().find(id);
	return (iter != getGraphs().end()) ? get<LINKS_NO>(iter->second) : 0;
}

void network_add_node(unsigned long id, const char* label) {
	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && label != NULL)
		get<NODES>(iter->second).emplace(label, NodeInfo());
}

void network_add_link(unsigned long id, const char* slabel, const char* tlabel) {
	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && slabel != NULL && tlabel != NULL) {
		map<Node, NodeInfo>::iterator its, itt;
		map<Node, NodeInfo>& nodes = get<NODES>(iter->second);
		its = nodes.emplace(slabel, NodeInfo()).first;
		itt = nodes.emplace(tlabel, NodeInfo()).first;
		//powyższe 2 linijki: jeśli nie ma slabel lub tlabel w sieci, stworzą je,
		//jeśli już istnieją, zwrócą do nich iteratory (<3 C++11)

		//jeśli krawędź nie istnieje jeszcze:
		if (get<OUT_NODES>(nodes.at(slabel)).find(tlabel) ==
			get<OUT_NODES>(nodes.at(slabel)).end()) {
			get<LINKS_NO>(iter->second)++;
			get<IN_NODES>(nodes.at(tlabel)).insert(slabel);
			get<OUT_NODES>(nodes.at(slabel)).insert(tlabel);
		}
	}
}

void network_remove_node(unsigned long id, const char* label) {
	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && get<GROWING>(iter->second) == 0) {
		map<Node, NodeInfo>& nodes = get<NODES>(iter->second);
		map<Node, NodeInfo>::iterator it = nodes.find(label);
		if (it != nodes.end()) {
			//zmniejsz ilość krawędzi w grafie
			get<LINKS_NO>(iter->second) -= get<OUT_NODES>(it->second).size() +
				get<IN_NODES>(it->second).size();

			//dla każdego wierzchołka, do którego jest krawędź od label
			//usuń label z jego wierzchołków wejściowych
			for (const char* v: get<OUT_NODES>(it->second))
				get<IN_NODES>(get<NODES>(iter->second).at(v)).erase(label);

			//dla każdego wierzchołka, który ma krawędź do label
			//usuń label z jego wychodzących
			for (const char* v: get<IN_NODES>(it->second))
				get<OUT_NODES>(get<NODES>(iter->second).at(v)).erase(label);

			//usuń krawędzi wychodzące z label
			get<NODES>(iter->second).erase(it);
		}
	}
}

void network_remove_link(unsigned long id, const char* slabel, const char* tlabel) {
	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && get<GROWING>(iter->second) == 0) {
		map<Node, NodeInfo>& nodes = get<NODES>(iter->second);
		map<Node, NodeInfo>::iterator its, itt;
		its = nodes.find(slabel);
		itt = nodes.find(tlabel);

		//jeśli chociaż jeden wierzchołek istnieje
		if (its != nodes.end()) {
			set<Node>& sOutNodes = get<OUT_NODES>(its->second);
			set<Node>& tInNodes = get<IN_NODES>(itt->second);

			//jeśli tlabel ma krawędź wychodzącą do slabel
			if (sOutNodes.find(tlabel) != sOutNodes.end()) {
				//zapominamy o krawędzi
				get<LINKS_NO>(iter->second)--;
				//oraz usuwamy ją z setów
				sOutNodes.erase(sOutNodes.find(tlabel));
				tInNodes.erase(tInNodes.find(slabel));
			}
		}
	}
}

void network_clear(unsigned long id) {
	getGraphs().erase(id);
}

//type - IN_NODES/OUT_NODES
size_t network_get_degree(unsigned long id, const char* label, const unsigned short type) {
	size_t res = 0;
	GraphsIterator iter = getGraphs().find(id);

	if (iter != getGraphs().end()) {
		map<Node, NodeInfo>& nodes = get<NODES>(iter->second);
		map<Node, NodeInfo>::iterator it = nodes.find(label);
		if (it != nodes.end())
			res = ((type == IN_NODES) ? get<IN_NODES>(it->second) : get<OUT_NODES>(it->second)).size();
	}

	return res;
}

size_t network_out_degree(unsigned long id, const char* label) {
	return network_get_degree(id, label, OUT_NODES);
}

size_t network_in_degree(unsigned long id, const char* label) {
	return network_get_degree(id, label, IN_NODES);
}
