#include <stdlib.h>
#include <memory.h>

#include "cache.h"
#include "cache_replace.h"

// tag field is considered to be no more than 63 bit
#define CL_P (0x1 << 63)

// typedef uint64_t blk_t;

typedef struct Cache{
	uint32_t n_ways;
	uint32_t log2_blksize;
	uint32_t log2_n_sets;
	blk_t* cache_buf;	// a block contains only tag field and valid field now
	Unit unit;
	int (* blk_hit_test)(Unit unit, blk_t* set, uint64_t tag, blk_t** evict_blk);
}Cache;

struct Cache *
install_cache(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets, ReplaceMode mode)
{
	struct Cache* cache;

	cache = malloc(sizeof(Cache));
	memmove((void *)cache, (void *)&n_ways, 3 * sizeof(uint32_t));

	size_t cache_size = (1 << log2_n_sets) * (1 << n_ways) * sizeof(uint64_t);
	cache->cache_buf = (blk_t *)malloc(cache_size);
	memset((void *)cache->cache_buf, 0, sizeof(cache_size));

	//choose handler and corresponding unit for cache
	switch(mode){
		case ReplaceModeLRU:
			cache->blk_hit_test = lru_blk_hit_test;
			cache->unit.lru_unit = lru_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets);
			break;
		case ReplaceModeOPT:
			cache->blk_hit_test = opt_blk_hit_test;
			cache->unit.opt_unit = opt_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets);
			break;
		default:
			break;
	}

	return cache;
}

int 
cache_access(struct Cache* cache, uint64_t addr, Ret* ret)
{
	uint64_t tag;
	uint64_t set_index;
	uint64_t log2_index;
	uint64_t log2_tag;

	blk_t *set;
	blk_t **evict_blk;

	log2_index = (cache->log2_blksize + cache->log2_n_sets);
	log2_tag = 64 - log2_index;

	tag = addr >> log2_index;
	set_index = (addr << log2_tag) >> log2_index;
	set = cache->cache_buf + set_index * cache->n_ways * sizeof(blk_t);

	if (!(cache->blk_hit_test(cache->unit, set, tag, evict_blk) == 0))
	{
		ret->set_index = set_index;
		ret->line_index = *evict_blk - set;
		ret->addr_new = (addr >> cache->log2_blksize) << cache->log2_blksize;
		ret->addr_old = (**evict_blk & (~CL_P)) << log2_index | \
						(set_index << cache->log2_blksize);
		**evict_blk = (ret->addr_new >> log2_index) | CL_P;
		return -1;
	}

	return 0;
}
void 
set_opt_profile(struct Cache* cache, uint64_t* profile, uint64_t* profile_end)
{
	opt_init_unit(cache->unit.opt_unit, profile, profile_end);
}
