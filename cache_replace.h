#include <stdint.h>

#pragma once

typedef uint64_t blk_t;


typedef struct{
	uint64_t *lu_t; // last used
	uint64_t cur_t; // current time
}LRU_unit;

typedef struct{
	uint64_t *profile;
	uint64_t *profile_end;
	char *predict_f;
	uint32_t log2_tag;
}OPT_unit;

typedef union {
	LRU_unit* lru_unit;
	OPT_unit* opt_unit;
}Unit;


LRU_unit* lru_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets);
int lru_blk_hit_test(Unit* unit, blk_t* set, uint64_t n_ways, uint64_t tag, blk_t** evict_blk);

OPT_unit* opt_get_unit(uint32_t n_ways, uint32_t log2_blksize, uint32_t log2_n_sets);
int opt_blk_hit_test(Unit* unit, blk_t* set, uint64_t n_ways, uint64_t tag, blk_t** evict_blk);
void opt_init_unit(OPT_unit* unit, uint64_t* profile, uint64_t* profile_end);