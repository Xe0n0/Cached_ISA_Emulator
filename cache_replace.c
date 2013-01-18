#include <stdlib.h>

#include "cache_replace.h"
#include "cache.h"

#define CL_P ((uint64_t) 1 << 63)

LRU_unit*
lru_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets)
{
	LRU_unit *unit = (LRU_unit *)malloc(sizeof(LRU_unit));
	unit->lu_t = (uint64_t*)malloc(n_ways * sizeof(uint64_t));
	unit->cur_t = 0;
        return unit;
}

int
blk_hit(blk_t* set, uint64_t tag, uint32_t n_ways, blk_t** blk)
{
	size_t i;
	blk_t* cur = set;

	for (i = 0; i < n_ways; i++)
	{
		if (*cur == (tag | CL_P)) // tag | CL_P ensure a valid block
		{
			*blk = cur;
			return RetTypeHit;
		}
		cur += 1;
	}
	return -1;
}

int
lru_blk_hit_test(Unit* unit, blk_t* set, uint64_t n_ways, uint64_t tag, blk_t** evict_blk)
{
	unit->lru_unit->cur_t ++;
	blk_t *blk;

	if (!(blk_hit(set, tag, n_ways, &blk) == 0))
	{
		blk_t *p;

		for (blk = p = set; p < set + n_ways; p++)
		{
			if (!(*p & CL_P))
			{
				*evict_blk = p;
				return -RetTypeCompulsoryMiss;
			}
			if (*(unit->lru_unit->lu_t + (blk - set)) > \
				*(unit->lru_unit->lu_t + (p - set)))
				blk = p;
		}
		*evict_blk = blk;
		*(unit->lru_unit->lu_t + (blk - set)) = unit->lru_unit->cur_t;
		return -RetTypeReplacedMiss;
	}
	*(unit->lru_unit->lu_t + (blk - set)) = unit->lru_unit->cur_t;
	*evict_blk = blk;

	return RetTypeHit;

}

OPT_unit*
opt_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets)
{
	OPT_unit *unit = (OPT_unit *)malloc(sizeof(OPT_unit));
	unit->predict_f = (char *)malloc(n_ways * sizeof(char));
	return unit;
}

void
opt_init_unit(OPT_unit* unit, uint64_t* profile, uint64_t* profile_end){
	unit->profile = profile;
	unit->profile_end = profile_end;
}

int
opt_blk_hit_test(Unit* unit, blk_t* set, uint64_t n_ways, uint64_t tag, blk_t** evict_blk)
{

	return 0;
}
