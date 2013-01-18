#include <cstdint>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

typedef unsigned int uint;

class cache
{
  struct line
  {
    uint64_t tag;
    bool valid;
  };

  uint log2_n_sets;
  uint n_sets;
  uint n_ways;
  uint log2_line_size;
  uint line_size;
  vector<vector<line>> sets;

public:
  cache(uint l2_ls, uint n_ways, uint l2_sets)
    : log2_n_sets(l2_sets),
      n_sets(1 << l2_sets),
      n_ways(n_ways),
      log2_line_size(l2_ls),
      line_size(1 << l2_ls)
  {
    sets.reserve(n_sets);
    uint i = n_sets;
    while (i--)
      sets.push_back(vector<line>(line_size));
  }

  uint addr2set(uint64_t addr)
  {
    return (addr >> log2_line_size) & (n_sets - 1);
  }

  uint64_t addr2tag(uint64_t addr)
  {
    return addr >> (log2_line_size + log2_n_sets);
  }

  bool hit(uint64_t addr, uint set, uint line)
  {
    if (addr2set(addr) != set)
      return false;
    if (line >= line_size)
      return false;
    auto l = sets[set][line];
    if (!l.valid)
      return false;
    if (l.tag != addr2tag(addr))
      return false;
    return true;
  }

  bool miss(uint64_t addr, uint set, uint line)
  {
    if (addr2set(addr) != set)
      return false;
    if (line >= line_size)
      return false;
    for (auto l : sets[set])
      if (l.valid && l.tag == addr2tag(addr))
        return false;
    sets[set][line].valid = true;
    sets[set][line].tag = addr2tag(addr);
    return true;
  }
};

static cache
read_cache_description(char const* path)
{
  ifstream ifs;
  ifs.exceptions(ifstream::failbit | ifstream::badbit);
  ifs.open(path);

  uint32_t l2_ls, n_ways, l2_sets;

  ifs >> l2_ls >> n_ways >> l2_sets;

  return cache(l2_ls, n_ways, l2_sets);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
    {
      cerr << "Usage: " << argv[0] << " cache-structure" << endl;
      return 2;
    }
  auto c = read_cache_description(argv[1]);

  uint64_t lno = 1;
  string l;
  while(getline(cin, l))
    {
      istringstream is(l);
      uint64_t addr;
      string hit_or_miss;
      string useless;
      uint set, line;
      is >> hex >> addr >> hit_or_miss;
      is.ignore(10, '(');
      is >> set;
      is.ignore(10, ',');
      is >> line;
      bool ok;
      if (hit_or_miss == "hit")
        ok = c.hit(addr, set, line);
      else
        ok = c.miss(addr, set, line);
      if (!ok)
        {
          cerr << "Error at line " << lno << endl;
          return 1;
        }

      lno++;
    }

  if (cin.bad())
    cerr << "Warning: cin.bad()" << endl;
  return 0;
}
