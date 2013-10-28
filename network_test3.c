#include <assert.h>
#include <stdio.h>
#include "network.h"

int main() {
	unsigned long n;
	
	n = network_new(0);
	
	assert(network_nodes_number(n) == 0);
	assert(network_links_number(n) == 0);
	
	network_add_link(n,"a",NULL);
	assert(network_nodes_number(n) == 0);
	assert(network_links_number(n) == 0);
	
	network_add_link(n,NULL,NULL);
	assert(network_nodes_number(n) == 0);
	assert(network_links_number(n) == 0);
	
	network_add_link(n,NULL,"a");
	assert(network_nodes_number(n) == 0);
	assert(network_links_number(n) == 0);
	
	network_add_link(n,"a","a");
	assert(network_nodes_number(n) == 0);
	assert(network_links_number(n) == 0);
	//z zadania wynika, ze jak wywolamy 
	//add_link gdzie slabel = tlabel, to 
	//nie powinno sie nic dodac. Popraw
	//mnie jesli sie myle.
	
	return 0;
}