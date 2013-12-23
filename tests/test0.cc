/* Test z tresci zadania */
#include "../../virus_genealogy.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Virus {
  public:
    typedef typename std::string id_type;
    Virus(id_type const &_id) : id(_id) {
    }
    id_type get_id() const {
      return id;
    }
  private:
    id_type id;
};

typedef typename Virus::id_type id_type;

int main() {
  VirusGenealogy<Virus> gen("A1H1");
  id_type const id1 = gen.get_stem_id();
  assert(gen.exists(id1));
  assert(gen.get_parents(id1).size() == 0);
  gen.create("A", id1);
  gen.create("B", id1);
  assert(gen.get_children(id1).size() == 2);
  gen.create("C", "A");
  gen.connect("C", "B");
  assert(gen.get_parents("C").size() == 2);
  assert(gen.get_children("A").size() == 1);
  std::vector<id_type> parents;
  parents.push_back("A");
  parents.push_back("B");
  gen.create("D", parents);
  assert(gen.get_parents("D").size() == parents.size());
  assert(gen.get_children("A").size() == 2);
  assert("D" == gen["D"].get_id());
  gen.remove("A");
  assert(!gen.exists("A"));
  assert(gen.exists("B"));
  assert(gen.exists("C"));
  assert(gen.exists("D"));
  gen.remove("B");
  assert(!gen.exists("A"));
  assert(!gen.exists("B"));
  assert(!gen.exists("C"));
  assert(!gen.exists("D"));
  try {
    gen["E"];
  }
  catch (VirusNotFound const &e) {
    assert(e.what() == string("VirusNotFound"));
  }
  try {
    gen.create("E", "A1H1");
    gen.create("E", "A1H1");
  }
  catch (VirusAlreadyCreated const &e) {
    assert(e.what() == string("VirusAlreadyCreated"));
  }
  try {
    gen.remove("A1H1");
  }
  catch (TriedToRemoveStemVirus const &e) {
      assert(e.what() == string("TriedToRemoveStemVirus"));
  }
  cout << "OK" << endl;
}
