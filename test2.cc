//Test dzialania sieci growing (nie growingnet)
#include <assert.h>
#include <stdio.h>
#include <string>
#include "network.h"
#include "growingnet.h"

int main()
{
	int growingnet = network_new(1);
	
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

	for(int i = 1; i < 1000; ++i) {
		network_add_link(growingnet, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet) == i+1);
		assert(network_links_number(growingnet) == i+1);
		assert(network_out_degree(growingnet, "0") == i+1);
		assert(network_in_degree(growingnet, "0") == 1);
	}
	
	for(int i = 999; i >= 0; --i) {
		network_remove_link(growingnet, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet) == 1000);
		assert(network_links_number(growingnet) == 1000);
		assert(network_out_degree(growingnet, "0") == 1000);
		assert(network_in_degree(growingnet, "0") == 1);
	}

	network_add_node(growingnet, "-1");
	assert(network_nodes_number(growingnet) == 1001);
	assert(network_links_number(growingnet) == 1000);
	assert(network_out_degree(growingnet, "0") == 1000);
	assert(network_in_degree(growingnet, "0") == 1);
	
	network_add_link(growingnet, "0", "-1");
	assert(network_nodes_number(growingnet) == 1001);
	assert(network_links_number(growingnet) == 1001);
	assert(network_out_degree(growingnet, "0") == 1001);
	assert(network_in_degree(growingnet, "0") == 1);

	network_clear(growingnet);
	assert(network_nodes_number(growingnet) == 1001);
	assert(network_links_number(growingnet) == 1001);
	assert(network_out_degree(growingnet, "0") == 1001);
	assert(network_in_degree(growingnet, "0") == 1);
	
	network_delete(growingnet);
	assert(network_nodes_number(growingnet) == 0);
	assert(network_links_number(growingnet) == 0);
	assert(network_out_degree(growingnet, "0") == 0);
	assert(network_in_degree(growingnet, "0") == 0);

    return 0;
}
