#ifndef GROWINGNET_H
#define GROWINGNET_H

namespace growingnet_init {
	static unsigned long get_net();
}

static const unsigned long growingnet = growingnet_init::get_net();

#endif