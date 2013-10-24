#ifndef NETWORK_H
#define NETWORK_H

#include <cstddef>

namespace network {
	//TODO statyczne struktury danych na sieć
}

/**
 * Tworzy nową, pustą, sieć i zwraca jej identyfikator.
 * Sieć pusta, to sieć z pustym zbiorem węzłów. Parametr
 * growing mówi o tym, czy nowa sieć ma być rosnąca
 * (growing != 0) czy nie (growing == 0).
 */
extern "C" unsigned long network_new(int growing);

/**
 * Jeżeli istnieje sieć o identyfikatorze id,
 * to usuwa sieć, a w przeciwnym przypadku nic nie robi.
 */
extern "C" void network_delete(unsigned long id);

/**
 * Jeżeli istnieje sieć o identyfikatorze id,
 * zwraca liczbę jej węzłów, a w przeciwnym przypadku zwraca 0.
 */
extern "C" size_t network_nodes_number(unsigned long id);

/**
 * Jeżeli istnieje sieć o identyfikatorze id,
 * zwraca liczbę jej krawędzi, a w przeciwnym przypadku zwraca 0.
 */

extern "C" size_t network_links_number(unsigned long id);

/*
 * Jeżeli istnieje sieć o identyfikatorze id, label != NULL i sieć
 * ta nie zawiera jeszcze węzła o etykiecie label, to dodaje węzeł
 * o etykiecie label do sieci, a w przeciwnym przypadku nic nie robi.
 */
extern "C" void network_add_node(unsigned long id, const char* label);

/*
 * Jeżeli istnieje sieć o identyfikatorze id, slabel != NULL oraz
 * tlabel != NULL, i sieć ta nie zawiera jeszcze krawędzi (slabel, tlabel),
 * to dodaje krawędź (slabel, tlabel) do sieci, a w przeciwnym przypadu nic nie robi.
 * Jeżeli w sieci nie istnieje węzeł o etykiecie któregoś z końców krawędzi,
 * to jest on również dodawany.
 */
extern "C" void network_add_link(unsigned long id, const char* slabel, const char* tlabel);

/*
 * Jeżeli istnieje sieć o identyfikatorze, a w niej węzeł o etykiecie label oraz
 * sieć nie jest rosnąca, to usuwa węzeł z sieci wraz ze wszystkimi krawędziami
 * wchodzącymi i wychodzącymi, a w przeciwnym przypadku nic nie robi.
 */
extern "C" void network_remove_node(unsigned long id, const char* label);

/*
 * Jeżeli istnieje sieć o identyfikatorze id, a w niej krawędź (slabel, tlabel),
 * oraz sieć nie jest rosnąca, to usuwa krawędź z sieci, a w przeciwnym przypadku
 * nic nie robi.
 */
extern "C" void network_remove_link(unsigned long id, const char* slabel, const char* tlabel);

/*
 * Jeżeli istnieje sieć o identyfikatorze id i sieć nie jest rosnąca, usuwa wszystkie
 * jej węzły i krawędzie, a w przeciwnym przypadku nic nie robi.
 */
extern "C" void network_clear(unsigned long id);

/*
 * Jeżeli istnieje sieć o identyfikatorze id, a w niej węzeł o etykiecie label,
 * to zwraca ilość krawędzi wychodzących z tego węzła, a w przeciwnym przypadku zwraca 0.
 */
extern "C" size_t network_out_degree(unsigned long id, const char* label);

/*
 * Jeżeli istnieje sieć o identyfikatorze id, a w niej węzeł o etykiecie label,
 * to zwraca ilość krawędzi wchodzących do tego węzła, a w przeciwnym przypadku zwraca 0.
 */
extern "C" size_t network_in_degree(unsigned long id, const char* label);


#endif
