#include "growingnet.h"
#include "network.h"

unsigned long growingnet_init::get_net() {
	const unsigned long id = 42;
	network_new(id);
	return id;
}

