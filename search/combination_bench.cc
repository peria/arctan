#include <glog/logging.h>

#include <iostream>
#include <chrono>
#include <memory>

#include "base/base.h"
#include "base/combination.h"

using namespace std;

using Clock = chrono::system_clock;

class Combinator {
 public:
  Combinator(int r) : r_(r) {}
  virtual bool GetNext(vector<int>& data, vector<int>& out) = 0;

 protected:
  int r_;
};

class CombinatorImpl final : public Combinator {
 public:
  CombinatorImpl(int r) : Combinator(r) {}

  bool GetNext(vector<int>& data, vector<int>& out) final {
    bool ret = next_combination(data.begin(), data.begin() + r_, data.end());
    for (int i = 0; i < r_; ++i)
      out[i] = data[i];
    return ret;
  }
};

class CombinatorNew final : public Combinator {
 public:
  CombinatorNew(int r) : Combinator(r), index_(r) {
    for (int i = 0; i < r_; ++i)
      index_[i] = i;
  }

  bool GetNext(vector<int>& data, vector<int>& out) final {
    int n = data.size();
    Increment(n, r_ - 1, data, out);
    if (index_[r_ - 1] < n)
      return true;
    for (int i = 0; i < r_; ++i) {
      index_[i] = i;
      out[i] = data[i];
    }
    return false;
  }

 private:
  void Increment(const int n, const int id, vector<int>& data, vector<int>& out) {
    ++index_[id];
    if (index_[id] > n - r_ + id && id > 0) {
      Increment(n, id - 1, data, out);
      index_[id] = index_[id - 1] + 1;
    }
    if (index_[id] < n)
      out[id] = data[index_[id]];
  }

  vector<int> index_;
};

int main() {
  //const int32 n = 6000;
  const int32 n = 7;
  const int32 r = 5;

  Combinator* combinators[] = {
    new CombinatorImpl(r),
    new CombinatorNew(r),
  };

  vector<int> data(n);
  for (int i = 0; i < n; ++i)
    data[i] = i;

  for (Combinator* comb : combinators) {
    auto start = Clock::now();
    vector<int> out(r);
    for (int i = 0; i < 10000000; ++i) {
      while (comb->GetNext(data, out)) {}
    }
    auto end = Clock::now();
    cout << "Elapsed time : "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms\n";
  }

  return 0;
}
