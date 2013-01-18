#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <cache.h>

using namespace std;

static Cache*
read_cache_structure(char const* path)
{
  ifstream ifs;
  ifs.exceptions(ifstream::failbit | ifstream::badbit);
  ifs.open(path);

  uint32_t line_size, n_ways, n_sets;

  ifs >> line_size >> n_ways >> n_sets;
  return install_cache(n_ways, line_size, n_sets, ReplaceModeLRU);
}

static vector<pair<uint64_t, uint32_t>>
read_trace(char const* path)
{
  ifstream ifs;
  ifs.open(path);

  vector<pair<uint64_t, uint32_t>> trace;
  while (ifs)
    {
      uint64_t addr;
      uint32_t len;

      ifs >> addr >> len;
      trace.push_back(make_pair(addr, len));
    }

  if (ifs.bad())
    cerr << "Warning: there may be problems in the trace file" << endl;

  return std::move(trace);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    {
      cerr << "Usage: " << argv[0] << " cache-structure trace" << endl;
      return 1;
    }
  Cache* c = read_cache_structure(argv[1]);
  uint64_t block_mask = 0;
  auto trace = read_trace(argv[2]);

  for (pair<uint64_t, uint32_t> addr_len : trace)
    {
      uint64_t addr = addr_len.first;
      uint32_t len = addr_len.second;
      uint64_t first_block = addr & block_mask;
      uint64_t last_block = (addr + len - 1) & block_mask;

      for (uint64_t a = first_block; a <= last_block; a++)
        {
          Ret replacement;
          int ret = access(c, a, &replacement);
          if (ret == 0)
            cout << "hit\n";
          else
            cout << "miss\n";
        }
    }

  return 0;
}
