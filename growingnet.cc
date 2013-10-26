#include "growingnet.h"
#include "network.h"

unsigned long get_net() {
	network_new(1);
	return growingnet;
}

static int growingnet_init = get_net();
