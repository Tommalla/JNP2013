//Test dzialania sieci nie growing
#include <assert.h>
#include <stdio.h>
#include <string>
#include "network.h"
#include "growingnet.h"

int main()
{
	int growingnet = network_new(0);
    network_add_link(growingnet, "0", "0");
    assert(network_nodes_number(growingnet) == 1);
    assert(network_links_number(growingnet) == 1);
    assert(network_out_degree(growingnet, "0") == 1);
    assert(network_in_degree(growingnet, "0") == 1);
    
    network_remove_link(growingnet, "0", "0");
	assert(network_nodes_number(growingnet) == 1);
    assert(network_links_number(growingnet) == 0);
    assert(network_out_degree(growingnet, "0") == 0);
    assert(network_in_degree(growingnet, "0") == 0);

    network_remove_node(growingnet, "0");
    assert(network_nodes_number(growingnet) == 0);
    assert(network_links_number(growingnet) == 0);
    assert(network_out_degree(growingnet, "0") == 0);
    assert(network_in_degree(growingnet, "0") == 0);
    
    network_add_link(growingnet, "0", "0");
    assert(network_nodes_number(growingnet) == 1);
    assert(network_links_number(growingnet) == 1);
    assert(network_out_degree(growingnet, "0") == 1);
    assert(network_in_degree(growingnet, "0") == 1);
    
    network_add_link(growingnet, "1", "1");
    assert(network_nodes_number(growingnet) == 2);
    assert(network_links_number(growingnet) == 2);
    assert(network_out_degree(growingnet, "0") == 1);
    assert(network_in_degree(growingnet, "0") == 1);
    
    network_remove_node(growingnet, "0");
	assert(network_nodes_number(growingnet) == 1);
    assert(network_links_number(growingnet) == 1);
    assert(network_out_degree(growingnet, "0") == 0);
    assert(network_in_degree(growingnet, "0") == 0);
    
    network_add_node(growingnet, "kupa");
    assert(network_nodes_number(growingnet) == 2);
    assert(network_links_number(growingnet) == 1);
    assert(network_out_degree(growingnet, "0") == 0);
    assert(network_in_degree(growingnet, "0") == 0);
    
    network_add_link(growingnet, "kupa", "0");
    assert(network_nodes_number(growingnet) == 3);
    assert(network_links_number(growingnet) == 2);
    assert(network_out_degree(growingnet, "0") == 0);
    assert(network_in_degree(growingnet, "0") == 1);
    
    network_remove_link(growingnet, "0", "kupa");
	assert(network_nodes_number(growingnet) == 3);
    assert(network_links_number(growingnet) == 2);
    assert(network_out_degree(growingnet, "0") == 0);
    assert(network_in_degree(growingnet, "0") == 1);
    
    network_remove_link(growingnet, "kupa", "0");
	assert(network_nodes_number(growingnet) == 3);
    assert(network_links_number(growingnet) == 1);
    assert(network_out_degree(growingnet, "0") == 0);
    assert(network_in_degree(growingnet, "0") == 0);

	network_clear(growingnet);
	assert(network_nodes_number(growingnet) == 0);
	assert(network_links_number(growingnet) == 0);
	assert(network_out_degree(growingnet, "0") == 0);
	assert(network_in_degree(growingnet, "0") == 0);
	
	network_add_node(growingnet, NULL);
	assert(network_nodes_number(growingnet) == 0);
	assert(network_links_number(growingnet) == 0);
	assert(network_out_degree(growingnet, "0") == 0);
	assert(network_in_degree(growingnet, "0") == 0);
	
	network_add_link(growingnet, NULL, NULL);
	assert(network_nodes_number(growingnet) == 0);
	assert(network_links_number(growingnet) == 0);
	assert(network_out_degree(growingnet, "0") == 0);
	assert(network_in_degree(growingnet, "0") == 0);


	for(int i = 1; i < 1000; ++i) {
		network_add_link(growingnet, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet) == i+1);
		assert(network_links_number(growingnet) == i);
		assert(network_out_degree(growingnet, "0") == i);
		assert(network_in_degree(growingnet, "0") == 0);
	}
	
	for(int i = 999; i > 0; --i) {
		network_remove_link(growingnet, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet) == 1000);
		assert(network_links_number(growingnet) == i-1);
		assert(network_out_degree(growingnet, "0") == i-1);
		assert(network_in_degree(growingnet, "0") == 0);
	}

	network_delete(growingnet);
	assert(network_nodes_number(growingnet) == 0);
	assert(network_links_number(growingnet) == 0);
	assert(network_out_degree(growingnet, "0") == 0);
	assert(network_in_degree(growingnet, "0") == 0);

    return 0;
}
