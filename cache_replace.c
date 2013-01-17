#include "cache_replace.h"
#include "cache.h"



LRU_unit*
lru_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets)
{
	struct LRU_unit *unit = (struct LRU_unit *)malloc(sizeof(LRU_unit))
	unit->lu_t = (uint64_t*)malloc(n_ways * sizeof(uint64_t));
	unit->cur_t = 0;
}

int blk_hit(blk* set, uint64_t tag, uint32_t n_ways, blk** blk)
{
	size_t i;
	uint64_t* cur = set;

	for (i = 0; i < n_ways; i++){
		if (*set == (tag | CL_P)) // tag | CL_P ensure a valid block
		cur += sizeof(blk);
	}
	return -1;
}

int
lru_blk_hit_test(LRU_unit* unit, blk* set, uint64_t tag, blk** evict_blk)
{
	unit->opt_unit->cur_t ++;
	blk **blk;

	if (!(blk_hit(set, tag, n_ways, blk) == 0))
	{
		blk *p;

		for (p = set; p < set + n_ways; p++)
		{
			if (!(p & CL_P))
		}
	}

}

OPT_unit*
opt_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets)
{
	struct OPT_unit *unit = (struct OPT_unit *)malloc(sizeof(OPT_unit));
	unit->predict_f = (char *)malloc(n_ways * sizeof(char));
	return unit;
}

void
opt_init_unit(OPT_unit* unit, uint64_t* profile, uint64_t* profile_end){
	unit->profile = profile;
	unit->profile_end = profile_end;	
}