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

class VirusAlreadyCreated : public exception {
public:
	virtual const char* what() const noexcept
	{
		return "VirusAlreadyCreated";
	}
};

class VirusNotFound : public exception {
public:
	virtual const char* what() const noexcept
	{
		return "VirusNotFound";
	}
};

class TriedToRemoveStemVirus : public exception {
public:
	virtual const char* what() const noexcept
	{
		return "TriedToRemoveStemVirus";
	}
};

template<class Virus>
class VirusGenealogy {
private:
	VirusGenealogy(VirusGenealogy<Virus> const &other) = delete;
	VirusGenealogy& operator=(VirusGenealogy<Virus> const &other) = delete;

	class Node {
	public:
		typedef weak_ptr<Node> WeakPtr;
		typedef shared_ptr<Node> SharedPtr;
		typedef set<WeakPtr, std::owner_less<WeakPtr>> ParentSet;
		typedef set<SharedPtr> ChildrenSet;

		Node(typename Virus::id_type const &id, VirusGenealogy *c) : vir{id}, id{id}, container{c} {} //for stem

		Node(typename Virus::id_type const &id, VirusGenealogy *c, WeakPtr const &parent) : vir{id}, id{id}, container{c} {
			parents.insert(parent);
		}

		Node(typename Virus::id_type const &id, VirusGenealogy *c, ParentSet &parent_set) : vir{id},
			id{id}, container{c}, parents{std::move(parent_set)} {}

		~Node() noexcept {
			auto iter = container->nodes.find(id);

			if (iter != container->nodes.end()) {
				for(auto ptr = children.begin(); ptr != children.end(); ptr++)
					(*ptr)->parents.erase( iter->second );

				container->nodes.erase(iter);
			}
		}

		Virus vir;
		const typename Virus::id_type id;
		VirusGenealogy *container;
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
			if(iter->second.expired())
				iter = nodes.end();

		if(iter == nodes.end())
			throw VirusNotFound();

		return iter;
	}

public:
	//constructor
	VirusGenealogy(typename Virus::id_type const &stem_id) : stem_id{stem_id}, stem{new Node(stem_id, this)} {
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

	void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id) {
		if (exists(id))
			throw VirusAlreadyCreated();
		auto iter = get_iterator(parent_id);
		typename Node::SharedPtr sp( new Node(id, this, iter->second ));
		nodes.emplace(id, typename Node::WeakPtr(sp));
		iter->second.lock()->children.insert(sp);
	}

	void create(typename Virus::id_type const &id, std::vector<typename Virus::id_type> const &parent_ids) {
		if(parent_ids.size() == 0)
			throw VirusNotFound();

		if (exists(id))
			throw VirusAlreadyCreated();
		typename Node::ParentSet parent_set;
		for(auto ptr = parent_ids.begin(); ptr != parent_ids.end(); ptr++)
			parent_set.insert(typename Node::WeakPtr( get_iterator( *ptr )->second ));

		typename Node::SharedPtr sp( new Node(id, this, parent_set) );
		for(auto ptr = sp->parents.begin(); ptr != sp->parents.end(); ptr++)
			ptr->lock()->children.insert(typename Node::SharedPtr(sp));

		nodes.emplace(id, typename Node::WeakPtr(sp));
	}

	bool exists(typename Virus::id_type const &id) const {
		auto k = nodes.end();
		try{
			k = get_iterator(id);
		} catch (VirusNotFound& e){}
		return k != nodes.end();
	}

	void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id) {
		auto parent = get_iterator(parent_id), child = get_iterator(child_id);
		parent->second.lock()->children.insert( typename Node::SharedPtr( child->second ) );
		child->second.lock()->parents.insert( typename Node::WeakPtr( parent->second ) );
	}

	void remove(typename Virus::id_type const &id) {
		auto iter = get_iterator(id);
		if (iter->first == stem_id)
			throw TriedToRemoveStemVirus();

		typename Node::SharedPtr sp(iter->second);
		for(auto ptr = iter->second.lock()->parents.begin(); ptr != iter->second.lock()->parents.end(); ptr++)
			(*ptr).lock()->children.erase( sp );

		//~Node() runs when out of this scope
	}

	Virus& operator[](typename Virus::id_type const &id) const {
		return get_iterator(id)->second.lock()->vir;
	}

};

#endif