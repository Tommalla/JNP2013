/* Test z tresci zadania, wirus i jego id
 * rzucaja id kiedy moga 
 * Parametry do zmiany w linijce FIXME 
 * */
#include "../../virus_genealogy.h"
#include "viruses/virus_nasty.h"
#include "ids/string_nasty.h"
#include "my_exceptions.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <memory>
#define F(l) (Tester<T>::force([&] () {(l);}))

static const int prob_id_except = 10; // FIXME
static const int prob_virus_except = 50;
static const int number_of_tries = 1000;

using namespace std;

template<unsigned int T>
struct Tester {
    static_assert(T > 0, "Liczba prob powinna byc dodatnia");
    int counter, line_counter;
    Tester() : line_counter(0) {}
    virtual void test_run() = 0;
    void run() {
        counter = 0;
        cout << "Running..." << endl;
        test_run();
        cout << "OK, RandomException number = " << counter << endl;
    }
    void force(std::function<void (void)> fun) {
        line_counter++;
        bool rep;
        for(int i = 0; i < T; i++) {
            rep = false;
            try {
                fun();
            } catch(RandomException const &re) {
                rep = true;
                counter++;
            } catch (...) {
                cout << "An exception is thrown in F: " << line_counter << endl;
                throw;
            }
            if(!rep) break;
        }
        if(rep) {
            cout << "Timeout in " << line_counter << endl;
            cout << "I tried " << T << " times, " << counter << " exceptions in total." << endl;
            cout << "This is not an error. Increase T or decrease prob." << endl;
            throw TimeoutException();
        }
    }
};

template<class Virus, unsigned int T>
struct Test0 : Tester<T> {
  typedef typename Virus::id_type vid_t;
  void test_run() {
      shared_ptr<VirusGenealogy<Virus>> ptr;
      F( ptr.reset(new VirusGenealogy<Virus>("A1H1")) );
      VirusGenealogy<Virus> &gen = *ptr;
      shared_ptr<vid_t> id_ptr;
      F( id_ptr.reset( new vid_t() ));
      vid_t &id1 = *id_ptr;
      F( id1 = gen.get_stem_id() );
      F( assert(gen.exists(id1)) );
      F( assert(gen.get_parents(id1).size() == 0) );
      F( gen.create("A", id1) );
      F( gen.create("B", id1) );
      F( assert(gen.get_children(id1).size() == 2) );
      F( gen.create("C", "A") );
      F( gen.connect("C", "B") );
      F( assert(gen.get_parents("C").size() == 2) );
      F( assert(gen.get_children("A").size() == 1) );
      std::vector<vid_t> parents;
      F( parents.push_back("A") );
      F( parents.push_back("B") );
      F( gen.create("D", parents) );
      F( assert(gen.get_parents("D").size() == parents.size()) );
      F( assert(gen.get_children("A").size() == 2) );
      F( assert(gen["D"].get_id() == "D") );
      F( gen.remove("A"));//
      F( assert(!gen.exists("A")) );
      F( assert(gen.exists("B")) );
      F( assert(gen.exists("C")) );
      F( assert(gen.exists("D")) );
      F( gen.remove("B") );
      F( assert(!gen.exists("A")) );
      F( assert(!gen.exists("B")) );
      F( assert(!gen.exists("C")) );
      F( assert(!gen.exists("D")) );
      try {
        F( gen["E"] );
      }
      catch (VirusNotFound const &e) {
        assert(e.what() == string("VirusNotFound"));
      }
      try {
        F( gen.create("E", "A1H1") );
        F( gen.create("E", "A1H1") );
      }
      catch (VirusAlreadyCreated const &e) {
        assert(e.what() == string("VirusAlreadyCreated"));
      }
      try {
        F( gen.remove("A1H1") );
      }
      catch (TriedToRemoveStemVirus const &e) {
          assert(e.what() == string("TriedToRemoveStemVirus"));
      }
  }
};

int main() {
    Test0<VirusNasty<StringNasty<prob_id_except>, prob_virus_except>, number_of_tries> t0;
    t0.run();
    return 0;
}
