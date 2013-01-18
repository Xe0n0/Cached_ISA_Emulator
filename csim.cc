#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include <cache.h>

using namespace std;

static tuple<Cache*, uint64_t, ReplaceMode>
read_cache_description(char const* path)
{
  ifstream ifs;
  ifs.exceptions(ifstream::failbit | ifstream::badbit);
  ifs.open(path);

  uint32_t line_size, n_ways, n_sets;
  string replacement_method;

  ifs >> line_size >> n_ways >> n_sets >> replacement_method;

  uint64_t block_mask = ~((static_cast<uint64_t>(1) << line_size) - 1);

  ReplaceMode m;
  if (replacement_method == "OPT")
    m = ReplaceModeOPT;
  else if (replacement_method == "LRU")
    m = ReplaceModeLRU;
  else
    {
      cerr << "Cache description: unknown replacement method: "
           << replacement_method << endl;
      exit(1);
    }

  return make_tuple(install_cache(n_ways, line_size, n_sets, m),
                    block_mask, m);
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
      cerr << "Usage: " << argv[0] << " cache-description trace" << endl;
      return 1;
    }
  Cache* c;
  uint64_t block_mask;
  ReplaceMode mode;
  tie(c, block_mask, mode) = read_cache_description(argv[1]);

  auto trace = read_trace(argv[2]);
  vector<uint64_t> btrace;

  for (auto addr_len : trace)
    {
      uint64_t addr = addr_len.first;
      uint32_t len = addr_len.second;
      uint64_t first_block = addr & block_mask;
      uint64_t last_block = (addr + len - 1) & block_mask;

      for (uint64_t a = first_block; a <= last_block; a++)
        btrace.push_back(a);
    }

  cout << showbase << hex;

  if (mode == ReplaceModeOPT)
    set_opt_profile(c, &btrace[0], (&btrace[0]) + btrace.size());

  for (uint64_t a : btrace)
    {
      Ret replacement;
      int ret = cache_access(c, a, &replacement);
      cout << a << " ";
      if (ret == 0)
        cout << "hit (" << replacement.set_index << ", "
             << replacement.line_index << ")\n";
      else
        {
          cout << "miss ";
          cout << "(" << replacement.set_index
               << ", " << replacement.line_index << ")";
          if (replacement.type == RetTypeCompulsoryMiss)
            cout << " filled\n";
          else if (replacement.type == RetTypeReplacedMiss)
            cout << " replaced (originally " << replacement.addr_old << ")\n";
          else
            cout << "???" << replacement.type << "\n";
        }
    }

  return 0;
}
