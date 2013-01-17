#include <stdlib.h>
#include "cache.h"

struct Cache{
	uint32_t n_ways;
	uint32_t blk_size;
	uint32_t n_sets;
	uint64_t** cache_buf;
}; 

install_cache(uint32_t n_ways, uint32_t blk_size, uint32_t n_sets)
{
	struct Cache* cache;

	cache = malloc(sizeof(Cache));
	
}

bool 
access_test(struct Cache* cache, uint64_t addr, struct Ret* ret)
{
}