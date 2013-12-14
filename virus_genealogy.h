#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H
#include <exception>
#include <map>
#include <set>
#include <memory>
#include <vector>

using std::exception;
using std::map;
using std::set;
using std::shared_ptr;
using std::weak_ptr;
using std::vector;

//FIXME something should probrably be done to fix the what() method of those:
class VirusAlreadyCreated : public exception {};

class VirusNotFound : public exception {};

class TriedToRemoveStemVirus : public exception {};

template<class Virus>
class VirusGenealogy {

	VirusGenealogy(VirusGenealogy<Virus> const &other) = delete;
	VirusGenealogy& operator=(VirusGenealogy<Virus> const &other) = delete;

	const typename Virus::id_type stem_id;

	class Node {
	public:
		Node(typename Virus::id_type const &id, Node* parent) : id{id} {
			if (parent != nullptr)
				parents.insert(weak_ptr<Node>());
		}

		//TODO second ctor

		const typename Virus::id_type id;
		set<shared_ptr<Node>> children;	//FIXME this can be an unordered_set (see the doc, esp. the Hash function)
		set<weak_ptr<Node>, std::owner_less<weak_ptr<Node>>> parents;
	};

	typedef map<typename Virus::id_type, Node> Graph;
	Graph nodes;

public:
	//constructor
	VirusGenealogy(typename Virus::id_type const &stem_id) : stem_id{stem_id} {
		nodes.emplace(stem_id, Node(stem_id, nullptr));	//FIXME add strong guarantee
	}

	//getters
	typename Virus::id_type get_stem_id() const noexcept {
		return stem_id;
	}

	std::vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const {
		auto iter = nodes.find(id);
		if (iter == nodes.end())
			throw VirusNotFound();

		vector<typename Virus::id_type> res;
		for (auto const &ptr: iter->second.children)
			res.push_back(ptr->id);

		return res;
	}

	std::vector<typename Virus::id_type> get_parents(typename Virus::id_type const &id) const {
		auto iter = nodes.find(id);
		if (iter == nodes.end())
			throw VirusNotFound();

		vector<typename Virus::id_type> res;
		for (auto const &ptr: iter->second.parents)
			res.push_back(ptr.lock()->id);

		return res;
	}

	//TODO the rest of the methods
	bool exists(typename Virus::id_type const &id) const {
		return nodes.find(id) != nodes.end();
	}
};

#endif