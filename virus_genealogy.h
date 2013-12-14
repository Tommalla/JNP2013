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

	class Node {

	public:
		const Virus vir;

		Node(typename Virus::id_type const &id) : vir(id), id{id} {} //for stem

		Node(typename Virus::id_type const &id, weak_ptr<Node> parent) : vir(id), id{id} {
			parents.insert(parent);
		}

		Node(typename Virus::id_type const &id, set<weak_ptr<Node>> parent_set) : vir(id), id{id} {
			
			parents = std::move(parent_set);
		}

		//TODO second ctor

		const typename Virus::id_type id;
		set<shared_ptr<Node>> children;	//FIXME this can be an unordered_set (see the doc, esp. the Hash function)
		set<weak_ptr<Node>, std::owner_less<weak_ptr<Node>>> parents;
	};

	const typename Virus::id_type stem_id;
	typedef map<typename Virus::id_type, weak_ptr<Node>> Graph;
	Graph nodes;
	shared_ptr<Node> stem;

 	typename Graph::const_iterator get_iterator(typename Virus::id_type const &id) const {
		auto iter = nodes.find(id);
		if(iter != nodes.end())
			if(iter->second.use_count() == 0)
				iter = nodes.end();
		if(iter == nodes.end()) 
			throw VirusNotFound();
		return iter;
	}

public:
	//constructor
	VirusGenealogy(typename Virus::id_type const &stem_id) : stem_id{stem_id}, stem( new Node(stem_id) ) {
		nodes.emplace(stem_id, weak_ptr<Node>(stem));	//FIXME add strong guarantee
	}

	//getters
	typename Virus::id_type get_stem_id() const noexcept {
		return stem_id;
	}

	std::vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const {
		auto iter = get_iterator(id);
		vector<typename Virus::id_type> res;
		for (auto const &ptr: iter->second.lock()->children)
			res.push_back(ptr->id);

		return res;
	}

	std::vector<typename Virus::id_type> get_parents(typename Virus::id_type const &id) const {
		auto iter = get_iterator(id);
		vector<typename Virus::id_type> res;
		for (auto const &ptr: iter->second.lock()->parents)
			res.push_back(ptr.lock()->id);

		return res;
	}

	//TODO try & catch on get_iterator here
	bool exists(typename Virus::id_type const &id) const {
		return get_iterator(id)!=nodes.end();
	}
	
	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {
		auto iter = get_iterator(parent_id);
		shared_ptr<Node> sp( new Node(id, iter->second ));
		nodes.emplace(id, weak_ptr<Node>(sp));
		iter->second.lock()->children.insert(sp);
	}



	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id) {
		auto parent = get_iterator(parent_id), child = get_iterator(child_id);
		parent->second.lock()->children.insert( shared_ptr<Node> ( child->second ) );
		child->second.lock()->parents.insert( weak_ptr<Node> ( parent->second ) );
	}


	//TODO sluzy do debugu, usunac na koncu
	int get_usage(typename Virus::id_type const &id){
		auto iter = get_iterator(id);
		return iter->second.use_count();
	}
};

#endif