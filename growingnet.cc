#include "growingnet.h"
#include "network.h"

unsigned long get_net() {
	network_new(growingnet);
	return growingnet;
}

static int growingnet_init = get_net();
