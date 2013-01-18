#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Cache;

typedef struct
{
	uint32_t set_index;
	uint32_t line_index;
	uint64_t addr_old;
	uint64_t addr_new;
}Ret;

typedef enum
{
	ReplaceModeOPT,
	ReplaceModeLRU
}ReplaceMode;

// cache.c
struct Cache* install_cache(uint32_t n_ways, uint32_t blk_size, uint32_t n_sets, ReplaceMode mode);
int cache_access(struct Cache* cache, uint64_t addr, Ret* ret);
void set_opt_profile(struct Cache* cache, uint64_t* profile, uint64_t* profile_end);

#ifdef __cplusplus
} /* extern "C" */
#endif
