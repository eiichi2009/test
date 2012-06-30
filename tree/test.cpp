#include "tree.hpp"
#include <vector>
#include <algorithm>
#include "bench.hpp"

using namespace std;

int
main()
{
  const int N = 5000000;
  vector<int> v;
  for (int i = 0; i < N; ++i) {
    v.push_back(i);
  }
  random_shuffle(v.begin(), v.end());

  AvlTree at;
  benchmark {
    for (unsigned int i = 0; i < v.size(); ++i) {
      at.insert(v[i]);
    }
  }

  benchmark {
    for (unsigned int i = 0; i < v.size(); ++i) {
      at.find(v[i]);
    }
  }


  benchmark {
    for (unsigned int i = 0; i < v.size(); ++i) {
      at.remove(v[i]);
    }
  }


  PlainTree pt;
  benchmark {
    for (unsigned int i = 0; i < v.size(); ++i) {
      pt.insert(v[i]);
    }
  }

  benchmark {
    for (unsigned int i = 0; i < v.size(); ++i) {
      pt.find(v[i]);
    }
  }

  benchmark {
    for (unsigned int i = 0; i < v.size(); ++i) {
      pt.remove(v[i]);
    }
  }

  return 0;
}
