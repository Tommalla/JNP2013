//Badanie działania na różnych sieciach (polaczenie test0, test1, test2)
#include <assert.h>
#include <stdio.h>
#include <string>
#include "network.h"
#include "growingnet.h"

int main()
{
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

	int growingnet2 = network_new(0);
    network_add_link(growingnet2, "0", "0");
    assert(network_nodes_number(growingnet2) == 1);
    assert(network_links_number(growingnet2) == 1);
    assert(network_out_degree(growingnet2, "0") == 1);
    assert(network_in_degree(growingnet2, "0") == 1);
    
    network_remove_link(growingnet2, "0", "0");
	assert(network_nodes_number(growingnet2) == 1);
    assert(network_links_number(growingnet2) == 0);
    assert(network_out_degree(growingnet2, "0") == 0);
    assert(network_in_degree(growingnet2, "0") == 0);

    network_remove_node(growingnet2, "0");
    assert(network_nodes_number(growingnet2) == 0);
    assert(network_links_number(growingnet2) == 0);
    assert(network_out_degree(growingnet2, "0") == 0);
    assert(network_in_degree(growingnet2, "0") == 0);
    
    network_add_link(growingnet2, "0", "0");
    assert(network_nodes_number(growingnet2) == 1);
    assert(network_links_number(growingnet2) == 1);
    assert(network_out_degree(growingnet2, "0") == 1);
    assert(network_in_degree(growingnet2, "0") == 1);
    
    network_add_link(growingnet2, "1", "1");
    assert(network_nodes_number(growingnet2) == 2);
    assert(network_links_number(growingnet2) == 2);
    assert(network_out_degree(growingnet2, "0") == 1);
    assert(network_in_degree(growingnet2, "0") == 1);
    
    network_remove_node(growingnet2, "0");
	assert(network_nodes_number(growingnet2) == 1);
    assert(network_links_number(growingnet2) == 1);
    assert(network_out_degree(growingnet2, "0") == 0);
    assert(network_in_degree(growingnet2, "0") == 0);
    
    network_add_node(growingnet2, "kupa");
    assert(network_nodes_number(growingnet2) == 2);
    assert(network_links_number(growingnet2) == 1);
    assert(network_out_degree(growingnet2, "0") == 0);
    assert(network_in_degree(growingnet2, "0") == 0);
    
    network_add_link(growingnet2, "kupa", "0");
    assert(network_nodes_number(growingnet2) == 3);
    assert(network_links_number(growingnet2) == 2);
    assert(network_out_degree(growingnet2, "0") == 0);
    assert(network_in_degree(growingnet2, "0") == 1);
    
    network_remove_link(growingnet2, "0", "kupa");
	assert(network_nodes_number(growingnet2) == 3);
    assert(network_links_number(growingnet2) == 2);
    assert(network_out_degree(growingnet2, "0") == 0);
    assert(network_in_degree(growingnet2, "0") == 1);
    
    network_remove_link(growingnet2, "kupa", "0");
	assert(network_nodes_number(growingnet2) == 3);
    assert(network_links_number(growingnet2) == 1);
    assert(network_out_degree(growingnet2, "0") == 0);
    assert(network_in_degree(growingnet2, "0") == 0);

	network_clear(growingnet2);
	assert(network_nodes_number(growingnet2) == 0);
	assert(network_links_number(growingnet2) == 0);
	assert(network_out_degree(growingnet2, "0") == 0);
	assert(network_in_degree(growingnet2, "0") == 0);
	
	network_add_node(growingnet2, NULL);
	assert(network_nodes_number(growingnet2) == 0);
	assert(network_links_number(growingnet2) == 0);
	assert(network_out_degree(growingnet2, "0") == 0);
	assert(network_in_degree(growingnet2, "0") == 0);
	
	network_add_link(growingnet2, NULL, NULL);
	assert(network_nodes_number(growingnet2) == 0);
	assert(network_links_number(growingnet2) == 0);
	assert(network_out_degree(growingnet2, "0") == 0);
	assert(network_in_degree(growingnet2, "0") == 0);


	for(int i = 1; i < 1000; ++i) {
		network_add_link(growingnet2, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet2) == i+1);
		assert(network_links_number(growingnet2) == i);
		assert(network_out_degree(growingnet2, "0") == i);
		assert(network_in_degree(growingnet2, "0") == 0);
	}
	
	for(int i = 999; i > 0; --i) {
		network_remove_link(growingnet2, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet2) == 1000);
		assert(network_links_number(growingnet2) == i-1);
		assert(network_out_degree(growingnet2, "0") == i-1);
		assert(network_in_degree(growingnet2, "0") == 0);
	}
	
	int growingnet3 = network_new(1);
	
    network_add_link(growingnet3, "0", "0");
    assert(network_nodes_number(growingnet3) == 1);
    assert(network_links_number(growingnet3) == 1);
    assert(network_out_degree(growingnet3, "0") == 1);
    assert(network_in_degree(growingnet3, "0") == 1);

    network_remove_node(growingnet3, "0");
    assert(network_nodes_number(growingnet3) == 1);
    assert(network_links_number(growingnet3) == 1);
    assert(network_out_degree(growingnet3, "0") == 1);
    assert(network_in_degree(growingnet3, "0") == 1);

	for(int i = 1; i < 1000; ++i) {
		network_add_link(growingnet3, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet3) == i+1);
		assert(network_links_number(growingnet3) == i+1);
		assert(network_out_degree(growingnet3, "0") == i+1);
		assert(network_in_degree(growingnet3, "0") == 1);
	}
	
	for(int i = 999; i >= 0; --i) {
		network_remove_link(growingnet3, "0", std::to_string(i).c_str());
		assert(network_nodes_number(growingnet3) == 1000);
		assert(network_links_number(growingnet3) == 1000);
		assert(network_out_degree(growingnet3, "0") == 1000);
		assert(network_in_degree(growingnet3, "0") == 1);
	}

	network_add_node(growingnet3, "-1");
	assert(network_nodes_number(growingnet3) == 1001);
	assert(network_links_number(growingnet3) == 1000);
	assert(network_out_degree(growingnet3, "0") == 1000);
	assert(network_in_degree(growingnet3, "0") == 1);
	
	network_add_link(growingnet3, "0", "-1");
	assert(network_nodes_number(growingnet3) == 1001);
	assert(network_links_number(growingnet3) == 1001);
	assert(network_out_degree(growingnet3, "0") == 1001);
	assert(network_in_degree(growingnet3, "0") == 1);

	network_delete(growingnet2);
	assert(network_nodes_number(growingnet2) == 0);
	assert(network_links_number(growingnet2) == 0);
	assert(network_out_degree(growingnet2, "0") == 0);
	assert(network_in_degree(growingnet2, "0") == 0);

	network_clear(growingnet3);
	assert(network_nodes_number(growingnet3) == 1001);
	assert(network_links_number(growingnet3) == 1001);
	assert(network_out_degree(growingnet3, "0") == 1001);
	assert(network_in_degree(growingnet3, "0") == 1);
	
	network_delete(growingnet3);
	assert(network_nodes_number(growingnet3) == 0);
	assert(network_links_number(growingnet3) == 0);
	assert(network_out_degree(growingnet3, "0") == 0);
	assert(network_in_degree(growingnet3, "0") == 0);

    return 0;
}
