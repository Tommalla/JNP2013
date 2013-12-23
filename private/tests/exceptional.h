#ifndef EXCEPTIONAL_H
#define EXCEPTIONAL_H
#include "my_exceptions.h"
#include <random>
#include <ctime>
#include <functional>

using namespace std;

template<unsigned int prob>
struct Exceptional {
  static_assert(prob <= 100, "Prawdopodobiensto wyjatku nie moze przekraczac 100%%");
    static void mess() {
        static default_random_engine generator(time(0));
        static discrete_distribution<int> distribution {100 - prob, prob};
        static auto coin = bind(distribution, generator);
      if(coin() == 1) {
        throw RandomException();
      }
    }
};

#endif
