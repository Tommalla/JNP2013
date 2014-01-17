/* Testy do zadania Genealogia Wirusów
 * Języki i narzędzia programowania 1, edycja 2013/14
 */

#if TEST_NUM <= 199

#include "virus_genealogy.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

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

#elif TEST_NUM <= 699

#include "virus_genealogy.h"
#include <cassert>
#include <exception>

// Wyjątek zgłaszany przez klasy Virus i Virus::id_type
class ExceptionSafetyTester : public std::exception {
  public:
     ExceptionSafetyTester() : std::exception() {
     }
};

// Pomocnicza klasa służąca do systematycznego zgłaszania wyjątków przy
// każdej nadarzającej się okazji
class Thrower {
  public:
    Thrower() : enabled(false), throw_value(0), throw_counter(0), thrown(false) {
    }
    void reset(long val) {
      throw_value = val;
      throw_counter = 0;
      thrown = false;
    }
    void enable(bool en) {
      enabled = en;
    }
    void execute() {
      if (enabled) {
        if (throw_value == throw_counter++) {
          thrown = true;
          throw ExceptionSafetyTester();
        }
      }
    }
  private:
    bool enabled;
    long throw_value;
    long throw_counter;
  public:
    bool thrown;
};

static Thrower thrower;

// Nasza bardzo wyjątkowa klasa
class Virus {
  public:
    class id_type {
      public:
        // Virus::id_type ma konstruktor bezargumentowy, konstruktor
        // kopiujący, operator przypisania.
        id_type() : id(0) {
          thrower.execute();
        }
        id_type(id_type const &that) : id(that.id) {
          thrower.execute();
        }
        id_type& operator=(id_type const& that) {
          thrower.execute();
          id = that.id;
          return *this;
        }

        // Konstruktor do tworzenia identyfikatorów
        id_type(int i) : id(i) {
          thrower.execute();
        }

        // Destruktor musi być no-throw.
        ~id_type() {
        }

        // Wartości typu Virus::id_type tworzą porządek liniowy i można je
        // porównywać za pomocą operatorów ==, !=, <=, >=, <, >.
        bool operator==(id_type const &that) const {
          thrower.execute();
          return id == that.id;
        }
        bool operator!=(id_type const &that) const {
          thrower.execute();
          return id != that.id;
        }
        bool operator<(id_type const &that) const {
          thrower.execute();
          return id < that.id;
        }
        bool operator<=(id_type const &that) const {
          thrower.execute();
          return id <= that.id;
        }
        bool operator>(id_type const &that) const {
          thrower.execute();
          return id > that.id;
        }
        bool operator>=(id_type const &that) const {
          thrower.execute();
          return id >= that.id;
        }

        // Gwarantujemy, że swap jest no-throw.
        void swap(id_type &that) {
          int tmp = id;
          id = that.id;
          that.id = tmp;
        }

      private:
        int id;
    };

  public:
    // Klasa Virus ma konstruktor przyjmujący argument typu Virus::id_type.
    Virus(id_type const &id) : vid(id) {
      thrower.execute();
    }

    // Destruktor musi być no-throw.
    ~Virus() {
    }

    // Klasa Virus ma metodę Virus::id_type get_id() const.
    id_type get_id() const {
      thrower.execute();
      return vid;
    }

    // Do testowania, czy obiekt faktycznie istnieje.
    void set(int v) {
      value = v;
    }
    int get() const {
      return value;
    }

  private:
    id_type vid;
    int value;

    Virus();
    Virus(Virus const &n);
    void operator=(Virus const &n);
};

// Gwarantujemy, że swap jest no-throw.
void swap(Virus::id_type &a, Virus::id_type &b) {
  a.swap(b);
}

// Poniższe byty nie są static, aby kompilator nie ostrzegał o ich nieużyciu.

int stem_id = 10777;

void RunTest(void (* BuildGraph)(VirusGenealogy<Virus> &),
             void (* ModifyGraph)(VirusGenealogy<Virus> &),
             void (* CheckBuiltGraph)(VirusGenealogy<Virus> &),
             void (* CheckModifiedGraph)(VirusGenealogy<Virus> &),
             bool enable_constructor_exceptions,
             bool enable_method_exceptions,
             bool enable_destruction_phase_exceptions,
             bool virus_already_created_exception_expected,
             bool virus_not_found_exception_expected,
             bool tried_to_remove_stem_virus_exception_expected) {
  for (long throw_value = 0; ; ++throw_value) {
    try {
      thrower.reset(throw_value);

      // Enable, aby testować odporność na wyjątki w konstruktorze.
      thrower.enable(enable_constructor_exceptions);
      VirusGenealogy<Virus> r(stem_id);

      // Nie chcemy tu wyjątków.
      thrower.enable(false);
      BuildGraph(r);
      CheckBuiltGraph(r);

      try {
        // Enable, aby testować odporność na wyjątki w poszczególnych metodach.
        thrower.enable(enable_method_exceptions);
        ModifyGraph(r);

        // Nie chcemy wyjątków przy sprawdzaniu poprawności.
        thrower.enable(false);
        CheckModifiedGraph(r);
      }
      catch (ExceptionSafetyTester) {
        // Nie chcemy wyjątków przy sprawdzaniu poprawności.
        thrower.enable(false);
        assert(thrower.thrown);
        thrower.thrown = false;
        CheckBuiltGraph(r);

        // Testuj, aż przestaną być zgłaszane wyjątki.
        continue;
      }
      catch (...) {
        // Zakładamy, że tylko ModifyGraph zgłasza wyjątki inne niż ExceptionSafetyTester.
        thrower.enable(false);
        CheckBuiltGraph(r);

        // Pozostałe przepuszczaj po sprawdzeniu.
        throw;
      }

      // Enable, aby testować no-throw destruktora.
      thrower.enable(enable_destruction_phase_exceptions);
    }
    catch (ExceptionSafetyTester) {
      // Tu wchodzimy, gdy wyjątek został zgłoszony w konstruktorze lub destruktorze.
      assert(thrower.thrown);
      thrower.thrown = false;
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      continue;
    }
    catch (VirusAlreadyCreated) {
      assert(virus_already_created_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (VirusNotFound) {
      assert(virus_not_found_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (TriedToRemoveStemVirus) {
      assert(tried_to_remove_stem_virus_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (...) {
      // Każdy inny wyjątek jest tu błędem.
      assert(false);
      return;
    }

    // Nie było wyjątku, a oczekiwaliśmy go.
    assert(!thrower.thrown);
    assert(!virus_already_created_exception_expected);
    assert(!virus_not_found_exception_expected);
    assert(!tried_to_remove_stem_virus_exception_expected);

    // Nie ma więcej wyjątków, więc test zakończył się pomyślnie.
    return;
  }
}

void NOP(VirusGenealogy<Virus> &) {
}

void CheckGraph0(VirusGenealogy<Virus>& graph) {
  assert(!graph.exists(1));
  assert(!graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_children(stem_id).size() == 0);
  assert(graph.get_parents(stem_id).size() == 0);
}

void CheckGraph1(VirusGenealogy<Virus>& graph) {
  assert(graph.exists(1));
  assert(!graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  std::vector<Virus::id_type> children0 = graph.get_children(stem_id);
  assert(children0.size() == 1);
  assert(children0[0] == 1);

  std::vector<Virus::id_type> parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == stem_id);
  assert(graph.get_children(1).size() == 0);
}

void CheckGraph12(VirusGenealogy<Virus>& graph) {
  assert(graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  std::vector<Virus::id_type> children0 = graph.get_children(stem_id);
  assert(children0.size() == 1);
  assert(children0[0] == 1);

  std::vector<Virus::id_type> parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == stem_id);
  std::vector<Virus::id_type> children1 = graph.get_children(1);
  assert(children1.size() == 1);
  assert(children1[0] == 2);

  std::vector<Virus::id_type> parents2 = graph.get_parents(2);
  assert(parents2.size() == 1);
  assert(parents2[0] == 1);
  assert(graph.get_children(2).size() == 0);
}

void CheckGraph2(VirusGenealogy<Virus>& graph) {
  assert(!graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  std::vector<Virus::id_type> children0 = graph.get_children(stem_id);
  assert(children0.size() == 1);
  assert(children0[0] == 2);

  std::vector<Virus::id_type> parents2 = graph.get_parents(2);
  assert(parents2.size() == 1);
  assert(parents2[0] == stem_id);
  assert(graph.get_children(2).size() == 0);
}

void CheckGraph123(VirusGenealogy<Virus>& graph) {
  assert(graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  std::vector<Virus::id_type> children0 = graph.get_children(stem_id);
  assert(children0.size() == 2);
  assert((children0[0] == 1 && children0[1] == 2) ||
         (children0[0] == 2 && children0[1] == 1));

  std::vector<Virus::id_type> parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == stem_id);
  std::vector<Virus::id_type> children1 = graph.get_children(1);
  assert(children1.size() == 1);
  assert(children1[0] == 2);

  std::vector<Virus::id_type> parents2 = graph.get_parents(2);
  assert(parents2.size() == 2);
  assert((parents2[0] == 1 && parents2[1] == stem_id) ||
         (parents2[0] == stem_id && parents2[1] == 1));
  assert(graph.get_children(2).size() == 0);
}

void Create1(VirusGenealogy<Virus>& graph) {
  graph.create(1, stem_id);
}

void Create2single(VirusGenealogy<Virus>& graph) {
  graph.create(2, 1);
}

void Create2vector(VirusGenealogy<Virus>& graph) {
  std::vector<Virus::id_type> parents;
  parents.push_back(1);
  parents.push_back(stem_id);
  graph.create(2, parents);
}

void CreateEmpty(VirusGenealogy<Virus>& graph) {
  std::vector<Virus::id_type> parents;
  graph.create(2, parents);
}

void Graph2(VirusGenealogy<Virus>& graph) {
  Create1(graph);
  Create2single(graph);
}

void Graph3(VirusGenealogy<Virus>& graph) {
  Create1(graph);
  Create2vector(graph);
}

void Connect20(VirusGenealogy<Virus>& graph) {
  graph.connect(2, stem_id);
}

void Connect25(VirusGenealogy<Virus>& graph) {
  graph.connect(2, 5);
}

void Remove0(VirusGenealogy<Virus>& graph) {
  graph.remove(stem_id);
}

void Remove1(VirusGenealogy<Virus>& graph) {
  graph.remove(1);
}

void Remove2(VirusGenealogy<Virus>& graph) {
  graph.remove(2);
}

void Connect20Remove2(VirusGenealogy<Virus>& graph) {
  Connect20(graph);
  Remove2(graph);
}

void GetChildren(VirusGenealogy<Virus>& graph) {
  assert(graph.get_children(100).size() == 0);
}

void GetParents(VirusGenealogy<Virus>& graph) {
  assert(graph.get_parents(100).size() == 0);
}

void Operator(VirusGenealogy<Virus>& graph) {
  assert(graph[100].get_id() == 0);
}

void SimpleTest(VirusGenealogy<Virus>& graph) {
  graph.create(7, stem_id);
  assert(graph.exists(stem_id));
  assert(!graph.exists(6));
  assert(graph.exists(7));
  assert(!graph.exists(8));
  Virus &v0 = graph[stem_id];
  Virus &v7 = graph[7];
  v0.set(66);
  v7.set(123);
  assert(v0.get() == 66);
  assert(v7.get() == 123);
}

void AdvancedTest(VirusGenealogy<Virus>& graph) {
  int i, n;

  for (n = 2; n <= 10; ++n) {
    graph.create(1, stem_id);
    graph.create(2, stem_id);
    for (i = 1; i <= n - 2; ++i) {
      graph.create(i + 2, i);
    }
    for (i = 1; i <= n - 3; ++i) {
      graph.connect(i + 3, i);
    }
    graph.remove(1);
    graph.remove(2);
    assert(graph.exists(stem_id));
    for (i = 0; i <= n + 1; ++i) {
      assert(!graph.exists(i));
    }
  }
}

#else

#include "virus_genealogy.h"
#include <cassert>
#include <exception>

#endif

int main() {

  // Test z treści zadania

  #if TEST_NUM == 100
  VirusGenealogy<Virus> gen("A1H1");
  Virus::id_type const id1 = gen.get_stem_id();
  assert(gen.exists(id1));
  assert(gen.get_parents(id1).size() == 0);
  gen.create("A", id1);
  gen.create("B", id1);
  assert(gen.get_children(id1).size() == 2);
  gen.create("C", "A");
  gen.connect("C", "B");
  assert(gen.get_parents("C").size() == 2);
  assert(gen.get_children("A").size() == 1);
  std::vector<Virus::id_type> parents;
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
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  try {
    gen.create("E", "A1H1");
    gen.create("E", "A1H1");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  try {
    gen.remove("A1H1");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  #endif

  // Proste testy poprawności poszczególnych operacji

  #if TEST_NUM == 200
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 201
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 202
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 203
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 204
  RunTest(Graph2, Connect20, CheckGraph12, CheckGraph123,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 205
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 206
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 207
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 208
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 209
  RunTest(Graph3, Connect20, CheckGraph123, CheckGraph123,
          false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 210
  RunTest(Graph3, Connect20Remove2, CheckGraph123, CheckGraph1,
          false, false, false, false, false, false);
  #endif

  // Testy poprawności zgłaszania wyjątków

  #if TEST_NUM == 300
  RunTest(Create1, Create1, CheckGraph1, CheckGraph1,
          false, false, false, true, false, false); // VirusAlreadyCreated
  #endif

  #if TEST_NUM == 301
  RunTest(Graph2, Create2vector, CheckGraph12, CheckGraph12,
          false, false, false, true, false, false); // VirusAlreadyCreated
  #endif

  #if TEST_NUM == 302
  RunTest(NOP, Create2single, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 303
  RunTest(NOP, Create2vector, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 304
  RunTest(NOP, Connect20, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 305
  RunTest(NOP, Remove0, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true); // TriedToRemoveStemVirus
  #endif

  #if TEST_NUM == 306
  RunTest(Graph3, Remove0, CheckGraph123, CheckGraph123,
          false, false, false, false, false, true); // TriedToRemoveStemVirus
  #endif

  #if TEST_NUM == 307
  RunTest(NOP, GetChildren, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 308
  RunTest(NOP, GetParents, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 309
  RunTest(NOP, Operator, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 310
  RunTest(Graph2, Connect25, CheckGraph12, CheckGraph12,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 311
  RunTest(Create1, Remove2, CheckGraph1, CheckGraph1,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 312
  RunTest(NOP, CreateEmpty, CheckGraph0, CheckGraph0,
          false, false, false, false, true, false); // VirusNotFound
  #endif

  // Testy no-throw destruktora

  #if TEST_NUM == 400
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 401
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 402
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 403
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 404
  RunTest(Graph2, Connect20, CheckGraph12, CheckGraph123,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 405
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 406
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 407
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 408
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 409
  RunTest(Graph3, Connect20, CheckGraph123, CheckGraph123,
          false, false, true, false, false, false);
  #endif

  // Testy odporności na wyjątki

  #if TEST_NUM == 500
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          true, false, false, false, false, false);
  #endif

  #if TEST_NUM == 501
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 502
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 503
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 504
  RunTest(Graph2, Connect20, CheckGraph12, CheckGraph123,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 505
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 506
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 507
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 508
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 509
  RunTest(Graph3, Connect20, CheckGraph123, CheckGraph123,
          false, true, false, false, false, false);
  #endif

  // Różne testy

  #if TEST_NUM == 600
  RunTest(NOP, SimpleTest, NOP, NOP,
          true, true, false, false, false, false);
  #endif

  #if TEST_NUM == 601
  RunTest(NOP, SimpleTest, NOP, NOP,
          false, true, true, false, false, false);
  #endif

  #if TEST_NUM == 602
  RunTest(NOP, AdvancedTest, NOP, NOP,
          true, true, false, false, false, false);
  #endif

  #if TEST_NUM == 603
  RunTest(NOP, AdvancedTest, NOP, NOP,
          false, true, true, false, false, false);
  #endif

  // Test, czy jest header guard.

  #if TEST_NUM == 700
  #include "virus_genealogy.h"
  #include "virus_genealogy.h"
  #endif

  // Testy, czy wyjątki dziedziczą po std::exception.

  #if TEST_NUM == 800
  try {
    throw VirusAlreadyCreated();
  }
  catch (std::exception e) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  #if TEST_NUM == 801
  try {
    throw VirusNotFound();
  }
  catch (std::exception e) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  #if TEST_NUM == 802
  try {
    throw TriedToRemoveStemVirus();
  }
  catch (std::exception e) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  // Próba użycia konstruktora kopiującego lub operatora przypisania dla
  // obiektów klasy VirusGenealogy powinna zakończyć się błędem kompilacji.

  #if TEST_NUM == 900
  VirusGenealogy<Virus> r1(stem_id);
  VirusGenealogy<Virus> r1(r2);
  #endif

  #if TEST_NUM == 901
  VirusGenealogy<Virus> r1(1);
  VirusGenealogy<Virus> r2(2);
  r1 = r2;
  #endif
}
