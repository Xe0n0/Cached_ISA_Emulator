#pragma once

#define blk uint64_t

struct LRU_unit{
	uint64_t *lu_t; // last used
	uint64_t cur_t; // current time
}

struct OPT_unit{
	uint64_t *profile;
	uint64_t *profile_end;
	char *predict_f;
};

LRU_unit* lru_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets);
int lru_blk_hit_test(blk* set, uint64_t tag, blk** evict_blk);

OPT_unit* opt_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets);
int opt_blk_hit_test(blk* set, uint64_t tag, blk** evict_blk);
void opt_init_unit(OPT_unit* unit, uint64_t* profile, uint64_t* profile_end);