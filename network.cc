#include <map>
#include <set>
#include <cstdio>
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

static map<unsigned long, Graph>& getGraphs() {
	static map<unsigned long, Graph> graphs;

	return graphs;
}

#ifdef DEBUG
const bool debug = true;
#else
const bool debug = false;
#endif

unsigned long network_new(int growing) {
	//dopóki nie trafimy na wolne id, zwiększamy
	while (getGraphs().find(netId) != getGraphs().end())
		++netId;

	//wstawiamy pusty graf
	getGraphs().emplace(netId, Graph());
	//inicjujemy growing
	get<GROWING>(getGraphs().at(netId)) = growing;

	if (debug)
		fprintf(stderr, "network_new(%d)\nnetwork_new: network %lu created\n", growing, netId);
	
	return netId;
}

void network_delete(unsigned long id) {
	if (debug) {
		fprintf(stderr, "network_delete(%lu)\nnetwork_delete: ", id);
		if (getGraphs().find(id) != getGraphs().end())	//jeśli sieć faktycznie istnieje
			fprintf(stderr, "network %lu deleted\n", id);
		else
			fprintf(stderr, "no networks deleted\n");
	}
	getGraphs().erase(id);
}

size_t network_get_number(unsigned long id, const unsigned short type) {
	if (debug)
		fprintf(stderr, "network %lu ", id);

	GraphsIterator iter = getGraphs().find(id);
	size_t res;

	if (iter != getGraphs().end()) {
		res = (type == NODES) ? get<NODES>(iter->second).size() : get<LINKS_NO>(iter->second);
		if (debug)
			fprintf(stderr, "contains %zu %s\n", res, (type == NODES) ? "nodes" : "links");
	} else {
		res = 0;
		if (debug)
			fprintf(stderr, "doesn't exist\n");
	}

	return res;
}

size_t network_nodes_number(unsigned long id) {
	if (debug)
		fprintf(stderr, "network_nodes_number(%lu)\nnetwork_nodes_number: ", id);
	return network_get_number(id, NODES);
}

size_t network_links_number(unsigned long id) {
	if (debug)
		fprintf(stderr, "network_links_number(%lu)\nnetwork_links_number: ", id);
	return network_get_number(id, LINKS_NO);
}

void network_add_node(unsigned long id, const char* label) {
	if (debug)
		fprintf(stderr, "network_add_node(%lu, %s)\nnetwork_add_node: ",
			id, label);

	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && label != NULL) {
		if (debug)
			fprintf(stderr, "network %lu, node %s ", id, label);
		bool success = get<NODES>(iter->second).emplace(label, NodeInfo()).second;
		if (debug)
			fprintf(stderr, "%s\n", (success == true) ? "added" : "already in network");
	} else if (debug) {
		if (label == NULL)
			fprintf(stderr, "label == NULL\n");
		else
			fprintf(stderr, "network %lu doesn't exist", id);
	}
}

void network_add_link(unsigned long id, const char* slabel, const char* tlabel) {
	if (debug)
		fprintf(stderr, "network_add_link(%lu, %s, %s)\nnetwork_add_link: ",
			id, slabel, tlabel);

	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && slabel != NULL && tlabel != NULL) {
		if (debug)
			fprintf(stderr, "network %lu, link(%s,%s) ", id, slabel, tlabel);

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
			if (debug)
				fprintf(stderr, "added\n");
		} else if (debug)
			fprintf(stderr, "already in network\n");
	} else if (debug) {
		if (slabel != NULL && tlabel != NULL)
			fprintf(stderr, "network %lu doesn't exist\n", id);
		else
			fprintf(stderr, "at least one of the arguments equals to NULL\n");
	}
}

void network_remove_node(unsigned long id, const char* label) {
	if (debug)
		fprintf(stderr, "network_remove_node(%lu, %s)\nnetwork_remove_node: network %lu",
			id, label, id);

	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && get<GROWING>(iter->second) == 0) {
		if (debug)
			fprintf(stderr, ", node %s ", label);

		map<Node, NodeInfo>& nodes = get<NODES>(iter->second);
		map<Node, NodeInfo>::iterator it = nodes.find(label);
		if (it != nodes.end()) {	//wierzchołek faktycznie jest w grafie
			//zmniejsz ilość krawędzi w grafie
			get<LINKS_NO>(iter->second) -= get<OUT_NODES>(it->second).size() +
				get<IN_NODES>(it->second).size();

			//dla każdego wierzchołka, do którego jest krawędź od label
			//usuń label z jego wierzchołków wejściowych
			for (Node v: get<OUT_NODES>(it->second))
				get<IN_NODES>(get<NODES>(iter->second).at(v)).erase(label);

			//dla każdego wierzchołka, który ma krawędź do label
			//usuń label z jego wychodzących
			for (Node v: get<IN_NODES>(it->second))
				get<OUT_NODES>(get<NODES>(iter->second).at(v)).erase(label);

			//usuń krawędzi wychodzące z label
			get<NODES>(iter->second).erase(it);

			if (debug)
				fprintf(stderr, "removed\n");
		} else if (debug)
			fprintf(stderr, "not in network\n");
	} else if (debug)
		fprintf(stderr, "%s\n", (get<GROWING>(iter->second) != 0) ? " is growing" : " doesn't exist");
}

void network_remove_link(unsigned long id, const char* slabel, const char* tlabel) {
	if (debug)
		fprintf(stderr, "network_remove_link(%lu, %s, %s)\nnetwork_remove_link: network %lu",
			id, slabel, tlabel, id);

	GraphsIterator iter = getGraphs().find(id);
	if (iter != getGraphs().end() && get<GROWING>(iter->second) == 0) {
		if (debug)
			fprintf(stderr, ", link (%s, %s) ", slabel, tlabel);

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

			if (debug)
				fprintf(stderr, "removed\n");
		} else if (debug)
			fprintf(stderr, "doesn't exist\n");
	} else if (debug)
		fprintf(stderr, " %s\n", (get<GROWING>(iter->second) != 0) ? "is growing" : "doesn't exist");
}

void network_clear(unsigned long id) {
	//FIXME: bug: wyczyści sieć rosnącą
	if (debug)
		fprintf(stderr, "network_clear(%lu)\nnetwork_clear: network %lu ", id, id);
	
	GraphsIterator iter = getGraphs().find(id);
	
	if (iter != getGraphs().end()) {
		if (get<GROWING>(iter->second) == 0) {
			getGraphs().at(id) = Graph();
			if (debug)
				fprintf(stderr, "cleared\n");
		} else if (debug)
			fprintf(stderr, "is growing\n");
	} else if (debug)
		fprintf(stderr, "doesn't exist\n");
}

//type - IN_NODES/OUT_NODES
size_t network_get_degree(unsigned long id, const char* label, const unsigned short type) {
	size_t res = 0;
	GraphsIterator iter = getGraphs().find(id);

	if (iter != getGraphs().end()) {
		if (debug)
			fprintf(stderr, ", node %s ", label);
		map<Node, NodeInfo>& nodes = get<NODES>(iter->second);
		map<Node, NodeInfo>::iterator it = nodes.find(label);
		if (it != nodes.end()) {
			res = ((type == IN_NODES) ? get<IN_NODES>(it->second) : get<OUT_NODES>(it->second)).size();
			if (debug)
				fprintf(stderr, "has %s degree %zu\n", (type == IN_NODES) ? "in" : "out", res);
		} else if (debug)
			fprintf(stderr, "doesn't exist\n");
	} else if (debug)
		fprintf(stderr, "doesn't exist\n");

	return res;
}

size_t network_out_degree(unsigned long id, const char* label) {
	if (debug)
		fprintf(stderr, "network_out_degree(%lu, %s)\nnetwork_out_degree: network %lu",
			id, label, id);
	return network_get_degree(id, label, OUT_NODES);
}

size_t network_in_degree(unsigned long id, const char* label) {
	if (debug)
		fprintf(stderr, "network_in_degree(%lu, %s)\nnetwork_in_degree: network %lu",
			id, label, id);
	return network_get_degree(id, label, IN_NODES);
}
