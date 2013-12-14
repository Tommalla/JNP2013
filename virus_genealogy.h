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
		typedef weak_ptr<Node> WeakPtr;
		typedef shared_ptr<Node> SharedPtr;
		typedef set<WeakPtr, std::owner_less<WeakPtr>> ParentSet;
		typedef set<SharedPtr> ChildrenSet;	//FIXME this can be an unordered_set (see the doc, esp. the Hash function)

		Virus vir;

		Node(typename Virus::id_type const &id) : vir{id}, id{id} {} //for stem

		Node(typename Virus::id_type const &id, WeakPtr const &parent) : vir{id}, id{id} {
			parents.insert(parent);
		}

		Node(typename Virus::id_type const &id, ParentSet const &parent_set) : vir{id},
			id{id}, parents(parent_set) {}

		const typename Virus::id_type id;
		ChildrenSet children;
		ParentSet parents;
	};

	const typename Virus::id_type stem_id;
	typedef map<typename Virus::id_type, typename Node::WeakPtr> Graph;
	Graph nodes;
	typename Node::SharedPtr stem;

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
	VirusGenealogy(typename Virus::id_type const &stem_id) : stem_id{stem_id}, stem{new Node(stem_id)} {
		nodes.emplace(stem_id, typename Node::WeakPtr(stem));	//FIXME add strong guarantee
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

	void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids) {
		if(parent_ids.size() == 0)
			throw VirusNotFound();

		typename Graph::const_iterator iter;
		typename Node::ParentSet parent_set;
		for(auto ptr = parent_ids.begin(); ptr != parent_ids.end(); ptr++)
			parent_set.insert(typename Node::WeakPtr( get_iterator( *ptr )->second ) );

		typename Node::SharedPtr sp( new Node(id, parent_set) );
		for(auto ptr = parent_set.begin(); ptr != parent_set.end(); ptr++)
			ptr->lock()->children.insert(typename Node::SharedPtr(sp));

		nodes.emplace(id, typename Node::WeakPtr(sp));
	}

	//TODO try & catch on get_iterator here
	bool exists(typename Virus::id_type const &id) const {
		return get_iterator(id) != nodes.end();
	}

	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {
		auto iter = get_iterator(parent_id);
		typename Node::SharedPtr sp( new Node(id, iter->second ));
		nodes.emplace(id, typename Node::WeakPtr(sp));
		iter->second.lock()->children.insert(sp);
	}

	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id) {
		auto parent = get_iterator(parent_id), child = get_iterator(child_id);
		parent->second.lock()->children.insert( typename Node::SharedPtr( child->second ) );
		child->second.lock()->parents.insert( typename Node::WeakPtr( parent->second ) );
	}

	//TODO sluzy do debugu, usunac na koncu
	int get_usage(typename Virus::id_type const &id) {
		auto iter = get_iterator(id);
		return iter->second.use_count();
	}
};

#endif