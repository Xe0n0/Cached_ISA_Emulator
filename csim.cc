#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>

#include <cache.h>

using namespace std;

static pair<Cache*, uint64_t>
read_cache_structure(char const* path)
{
  ifstream ifs;
  ifs.exceptions(ifstream::failbit | ifstream::badbit);
  ifs.open(path);

  uint32_t line_size, n_ways, n_sets;

  ifs >> line_size >> n_ways >> n_sets;

  uint64_t block_mask = ~((static_cast<uint64_t>(1) << line_size) - 1);

  return make_pair(install_cache(n_ways, line_size, n_sets, ReplaceModeLRU),
                   block_mask);
}

static vector<pair<uint64_t, uint32_t>>
read_trace(char const* path)
{
  ifstream ifs;
  ifs.open(path);
  ifs >> hex;

  vector<pair<uint64_t, uint32_t>> trace;
  uint64_t addr;
  uint32_t len;
  while (ifs >> addr >> len)
    trace.push_back(make_pair(addr, len));

  return trace;
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    {
      cerr << "Usage: " << argv[0] << " cache-structure trace" << endl;
      return 1;
    }
  Cache* c;
  uint64_t block_mask;
  auto cb = read_cache_structure(argv[1]);
  c = cb.first;
  block_mask = cb.second;

  auto trace = read_trace(argv[2]);

  cout << showbase << hex;

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
          cout << a << " ";
          if (ret == 0)
            cout << "hit\n";
          else
            cout << "miss\n";
        }
    }

  return 0;
}
