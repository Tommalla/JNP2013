JNP2
====

Poniżej znajduje się treść 2. zadania zaliczeniowego.

====

Biblioteka standardowa języka C++ udostępnia bardzo przydatne kontenery (np. set, map czy multimap), których nie ma w bibliotece C. Kontener multimap (mapa wielokrotna) umożliwia przechowywanie par (klucz, wartość), pozwalając na występowanie więcej niż jednej wartości pod jednym kluczem.

Często potrzebujemy łączyć kod C++ z kodem spadkowym w C. Celem tego zadania jest napisanie w C++ dwóch modułów obsługujących sieci (skierowane), tzn. grafy skierowane z węzłami etykietowanymi napisami, tak aby można ich było używać w C. Każdy moduł składa się z pliku nagłówkowego (z rozszerzeniem h) i pliku z imlementacją (z rozszerzeniem cc).

Moduł growingnet (pliki growingnet.h i growingnet.cc) powinien udostępniać stałą:

```cpp
const unsigned long growingnet;
```

Powyższa stała jest identyfikatorem Globalnej Sieci Rosnącej. Do sieci rosnącej można dodawać węzły i krawędzie, ale nie można ich usuwać.

Moduł network (pliki network.h i network.cc) powinien udostępniać następujące funkcje:

```cpp
unsigned long network_new(int growing);
```

Tworzy nową, pustą, sieć i zwraca jej identyfikator. Sieć pusta, to sieć z pustym zbiorem węzłów. Parametr growing mówi o tym, czy nowa sieć ma być rosnąca (growing != 0) czy nie (growing == 0).

```cpp
void network_delete(unsigned long id);
```

Jeżeli istnieje sieć o identyfikatorze id, to usuwa sieć, a w przeciwnym przypadku nic nie robi.

```cpp
size_t network_nodes_number(unsigned long id);
```

Jeżeli istnieje sieć o identyfikatorze id, zwraca liczbę jej węzłów, a w przeciwnym przypadku zwraca 0.

```cpp
size_t network_links_number(unsigned long id);
```

Jeżeli istnieje sieć o identyfikatorze id, zwraca liczbę jej krawędzi, a w przeciwnym przypadku zwraca 0.

```cpp
void network_add_node(unsigned long id, const char* label);
```

Jeżeli istnieje sieć o identyfikatorze id, label != NULL i sieć ta nie zawiera jeszcze węzła o etykiecie label, to dodaje węzeł o etykiecie label do sieci, a w przeciwnym przypadku nic nie robi.

```cpp
void network_add_link(unsigned long id, const char* slabel, const char* tlabel);
```

Jeżeli istnieje sieć o identyfikatorze id, slabel != NULL oraz tlabel != NULL, i sieć ta nie zawiera jeszcze krawędzi (slabel, tlabel), to dodaje krawędź (slabel, tlabel) do sieci, a w przeciwnym przypadu nic nie robi. Jeżeli w sieci nie istnieje węzeł o etykiecie któregoś z końców krawędzi, to jest on również dodawany.

```cpp
void network_remove_node(unsigned long id, const char* label);
```

Jeżeli istnieje sieć o identyfikatorze, a w niej węzeł o etykiecie label oraz sieć nie jest rosnąca, to usuwa węzeł z sieci wraz ze wszystkimi krawędziami wchodzącymi i wychodzącymi, a w przeciwnym przypadku nic nie robi.

```cpp
void network_remove_link(unsigned long id, const char* slabel, const char* tlabel);
```

Jeżeli istnieje sieć o identyfikatorze id, a w niej krawędź (slabel, tlabel), oraz sieć nie jest rosnąca, to usuwa krawędź z sieci, a w przeciwnym przypadku nic nie robi.

```cpp
void network_clear(unsigned long id);
```

Jeżeli istnieje sieć o identyfikatorze id i sieć nie jest rosnąca, usuwa wszystkie jej węzły i krawędzie, a w przeciwnym przypadku nic nie robi.

```cpp
size_t network_out_degree(unsigned long id, const char* label);
```

Jeżeli istnieje sieć o identyfikatorze id, a w niej węzeł o etykiecie label, to zwraca ilość krawędzi wychodzących z tego węzła, a w przeciwnym przypadku zwraca 0.

```cpp
size_t network_in_degree(unsigned long id, const char* label);
```

Jeżeli istnieje sieć o identyfikatorze id, a w niej węzeł o etykiecie label, to zwraca ilość krawędzi wchodzących do tego węzła, a w przeciwnym przypadku zwraca 0.



Moduły mają być kompilowane w dwóch wersjach.

* Wersja diagnostyczna (debuglevel=1) powinna:
    ** zawierać informację uruchomieniową pozwalającą otworzyć tekst źródłowy w debugerze,
    ** być kompilowana bez optymalizacji,
    ** dokładnie sprawdzać przekazane argumenty,
    ** wypisywać informacje uruchomieniowe na standardowy strumień błędów.

* Wersja zoptymalizowana (debuglevel=0) powinna:
    ** być pozbawiona wszelkich informacji uruchomieniowych,
    ** być optymalizowana ze względu na szybkość wykonywania,
    ** zakładać, że przekazane argumenty są poprawne.

Rozwiązanie powinno zawierać:

* pliki network.h, network.cc, growingnet.h, growingnet.cc,
* skrypt makefile umożliwiający skompilowanie modułów, tworzący odpowiednio pliki network.o i growingnet.o; uruchomienie 'make' lub 'make debuglevel=0' powinno generować wersję zoptymalizowaną, a uruchomienie 'make debuglevel=1' powinno generować wersję diagnostyczną.

Przykład użycia (wydruk pliku network_test1.c):

```cpp
    #include <assert.h>
    #include <stdio.h>
    #include "network.h"
    #include "growingnet.h"

    int main()
    {
        unsigned long n;

        n = network_new(0);

        network_add_link(n, "Kabaty", "Natolin");
        assert(network_nodes_number(n) == 2);
        assert(network_links_number(n) == 1);
        assert(network_out_degree(n, "Kabaty") == 1);
        assert(network_in_degree(n, "Kabaty") == 0);
        assert(network_out_degree(n, "Natolin") == 0);
        assert(network_in_degree(n, "Natolin") == 1);

        network_add_link(n, "Natolin", "Imielin");
        assert(network_nodes_number(n) == 3);
        assert(network_links_number(n) == 2);
        assert(network_out_degree(n, "Natolin") == 1);
        assert(network_in_degree(n, "Natolin") == 1);
        assert(network_out_degree(n, "Imielin") == 0);
        assert(network_in_degree(n, "Imielin") == 1);

        network_add_link(n, "Imielin", "Stokłosy");
        assert(network_nodes_number(n) == 4);
        assert(network_links_number(n) == 3);
        assert(network_out_degree(n, "Imielin") == 1);
        assert(network_in_degree(n, "Imielin") == 1);
        assert(network_out_degree(n, "Stokłosy") == 0);
        assert(network_in_degree(n, "Stokłosy") == 1);

        network_remove_link(n, "Kabaty", "Natolin");
        assert(network_nodes_number(n) == 4);
        assert(network_links_number(n) == 2);
        assert(network_out_degree(n, "Kabaty") == 0);
        assert(network_in_degree(n, "Kabaty") == 0);
        assert(network_out_degree(n, "Natolin") == 1);
        assert(network_in_degree(n, "Natolin") == 0);

        network_remove_node(n, "Imielin");
        assert(network_nodes_number(n) == 3);
        assert(network_links_number(n) == 0);
        assert(network_out_degree(n, "Kabaty") == 0);
        assert(network_in_degree(n, "Kabaty") == 0);
        assert(network_out_degree(n, "Natolin") == 0);
        assert(network_in_degree(n, "Natolin") == 0);
        assert(network_out_degree(n, "Stokłosy") == 0);
        assert(network_in_degree(n, "Stokłosy") == 0);

        network_add_node(n, "Imielin");
        assert(network_nodes_number(n) == 4);
        assert(network_links_number(n) == 0);
        assert(network_out_degree(n, "Imielin") == 0);
        assert(network_in_degree(n, "Imielin") == 0);

        network_add_link(n, "Kabaty", "Kabaty");
        assert(network_nodes_number(n) == 4);
        assert(network_links_number(n) == 1);
        assert(network_out_degree(n, "Kabaty") == 1);
        assert(network_in_degree(n, "Kabaty") == 1);

        network_clear(n);

        network_add_link(growingnet, "0", "0");
        assert(network_nodes_number(growingnet) == 1);
        assert(network_links_number(growingnet) == 1);
        assert(network_out_degree(growingnet, "0") == 1);
        assert(network_in_degree(growingnet, "0") == 1);

        network_remove_node(growingnet, "0");
        assert(network_nodes_number(growingnet) == 1);
        assert(network_links_number(growingnet) == 1);
        assert(network_out_degree(growingnet, "0") == 1);
        assert(network_in_degree(growingnet, "0") == 1);

        return 0;
    }
```

Plik network_test1.c można skompilować i połączyć z modułami za pomocą poleceń:

```bash
    gcc -Wall -O2 -c network_test1.c -o network_test1.o
    g++ network_test1.o growingnet.o network.o -o network_test1
```

Przykład informacji diagnostycznych wypisywanych przez powyższy
przykład użycia:

network_new(1)
network_new: network 0 created
network_new(0)
network_new: network 1 created
network_add_link(1, Kabaty, Natolin)
network_add_link: network 1, link (Kabaty,Natolin) added
network_nodes_number(1)
network_nodes_number: network 1 contains 2 nodes
network_links_number(1)
network_links_number: network 1 contains 1 links
network_out_degree(1, Kabaty)
network_in_degree(1, Kabaty)
network_out_degree(1, Natolin)
network_in_degree(1, Natolin)
network_add_link(1, Natolin, Imielin)
network_add_link: network 1, link (Natolin,Imielin) added
network_nodes_number(1)
network_nodes_number: network 1 contains 3 nodes
network_links_number(1)
network_links_number: network 1 contains 2 links
network_out_degree(1, Natolin)
network_in_degree(1, Natolin)
network_out_degree(1, Imielin)
network_in_degree(1, Imielin)
network_add_link(1, Imielin, Stokłosy)
network_add_link: network 1, link (Imielin,Stokłosy) added
network_nodes_number(1)
network_nodes_number: network 1 contains 4 nodes
network_links_number(1)
network_links_number: network 1 contains 3 links
network_out_degree(1, Imielin)
network_in_degree(1, Imielin)
network_out_degree(1, Stokłosy)
network_in_degree(1, Stokłosy)
network_remove_link(1, Kabaty, Natolin)
network_remove_link: link (Kabaty,Natolin) removed from network 1
network_nodes_number(1)
network_nodes_number: network 1 contains 4 nodes
network_links_number(1)
network_links_number: network 1 contains 2 links
network_out_degree(1, Kabaty)
network_in_degree(1, Kabaty)
network_out_degree(1, Natolin)
network_in_degree(1, Natolin)
network_remove_node(1, Imielin)
network_remove_node: network 1, node Imielin removed
network_nodes_number(1)
network_nodes_number: network 1 contains 3 nodes
network_links_number(1)
network_links_number: network 1 contains 0 links
network_out_degree(1, Kabaty)
network_in_degree(1, Kabaty)
network_out_degree(1, Natolin)
network_in_degree(1, Natolin)
network_out_degree(1, Stokłosy)
network_in_degree(1, Stokłosy)
network_add_node(1, Imielin)
network_add_node: network 1, node Imielin added
network_nodes_number(1)
network_nodes_number: network 1 contains 4 nodes
network_links_number(1)
network_links_number: network 1 contains 0 links
network_out_degree(1, Imielin)
network_in_degree(1, Imielin)
network_add_link(1, Kabaty, Kabaty)
network_add_link: network 1, link (Kabaty,Kabaty) added
network_nodes_number(1)
network_nodes_number: network 1 contains 4 nodes
network_links_number(1)
network_links_number: network 1 contains 1 links
network_out_degree(1, Kabaty)
network_in_degree(1, Kabaty)
network_clear(1)
network_clear: network 1 cleared
network_add_link(0, 0, 0)
network_add_link: network 0, link (0,0) added
network_nodes_number(0)
network_nodes_number: network 0 contains 1 nodes
network_links_number(0)
network_links_number: network 0 contains 1 links
network_out_degree(0, 0)
network_in_degree(0, 0)
network_remove_node(0, 0)
network_remove_node: attempt to remove node from grow-only network
network_nodes_number(0)
network_nodes_number: network 0 contains 1 nodes
network_links_number(0)
network_links_number: network 0 contains 1 links
network_out_degree(0, 0)
network_in_degree(0, 0)


Oczekiwane rozwiązanie powinno korzystać z kontenerów i metod udostępnianych przez standardową bibliotekę C++. Nie należy definiować własnych struktur lub klas. Do implementacji sieci można użyć kontenerów multimap i/lub set. W rozwiązaniu nie należy nadużywać kompilacji warunkowej. Fragmenty tekstu źródłowego realizujące wyspecyfikowane operacje na sieciach nie powinny zależeć od sposobu kompilacji (inaczej posiadanie wersji diagnostycznej nie miałoby sensu).

Pliki rozwiązania należy umieścić w repozytorium w katalogu

    grupaN/zadanie2/ab123456+cd123456

lub

    grupaN/zadanie2/ab123456+cd123456+ef123456

gdzie N jest numerem grupy, a ab123456, cd123456, ef123456 są identyfikatorami członków zespołu umieszczającego to rozwiązanie. Katalog z rozwiązaniem nie powinien zawierać innych plików, ale może zawierać podkatalog private, gdzie można umieszczać różne pliki, np. swoje testy. Pliki umieszczone w tym podkatalogu nie będą oceniane.
