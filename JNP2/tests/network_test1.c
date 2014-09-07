#include <assert.h>
#include <stdio.h>
#include "network.h"
#include "growingnet.h"

int main() {
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