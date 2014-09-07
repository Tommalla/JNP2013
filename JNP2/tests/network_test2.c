#include <assert.h>
#include <stdio.h>
#include "network.h"

int main() {
	unsigned long one, two;
	
	one = network_new(1);
	two = network_new(0);
	
	assert(network_nodes_number(one) == 0);
	assert(network_nodes_number(two) == 0);
	
	network_add_node(one, "1");
	network_add_node(one, "2");
	network_add_link(two, "1", "2");
	
	assert(network_nodes_number(one) == 2);
	assert(network_links_number(one) == 0);
	assert(network_nodes_number(two) == 2);
	assert(network_links_number(two) == 1);
	
	network_add_link(one, "1", "2");
	network_add_link(two, "2", "2");
	
	assert(network_links_number(one) == 1);
	assert(network_links_number(two) == 2);
	
	network_remove_link(two, "2", "2");
	
	assert(network_nodes_number(one) == 2);
	assert(network_links_number(one) == 1);
	assert(network_nodes_number(two) == 2);
	assert(network_links_number(two) == 1);
	assert(network_in_degree(543, "sadsa") == 0);
	
	return 0;
}