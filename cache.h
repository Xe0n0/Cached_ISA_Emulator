#pragma once

struct Cache;
struct Ret
{
	uint32_t set;
	uint32_t line;
	uint32_t addr_old;
	uint32_t addr_new;
};

struct Cache* install_cache(uint32_t n_ways, uint32_t blk_size, uint32_t n_sets);
bool access_test(struct Cache* cache, uint64_t addr, struct Ret* ret);