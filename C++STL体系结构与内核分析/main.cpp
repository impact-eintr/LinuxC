#include <cstdlib>
#include <ctime>
#include <exception>
#include <functional>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

const long ASIZE = 10000 * 100;

namespace util {
long get_a_target_long() {
  long target = 0;
  cout << "target (0~" << RAND_MAX << "): ";
  cin >> target;
  return target;
}

string get_a_target_string() {
  long target = 0;
  char buf[10];

  cout << "target (0~" << RAND_MAX << "): ";
  cin >> target;
  snprintf(buf, 10, "%ld", target);
  return string(buf);
}

int compareLongs(const void *a, const void *b) {
  return (*(long *)a - *(long *)b);
}

int compareStrings(const void *a, const void *b) {
  if (*(string *)a > *(string *)b) {
    return 1;
  } else if (*(string *)a < *(string *)b) {
    return -1;
  } else {
    return 0;
  }
}
} // namespace util

#include <array>

namespace array_test {
void test_array() {
  cout << "test_array()...........\n";
  array<long, ASIZE> c;

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; i++) {
    c[i] = rand();
  }
  cout << "milli-seconds: " << (clock() - timeStart) << endl;
  cout << "array.size(): " << c.size() << endl;
  cout << "array.front(): " << c.front() << endl;
  cout << "array.back(): " << c.back() << endl;
  cout << "array.data(): " << c.data() << endl;

  long target = util::get_a_target_long();
  timeStart = clock();
  qsort(c.data(), ASIZE, sizeof(long), util::compareLongs);
  long *pItem = (long *)bsearch(&target, (c.data()), ASIZE, sizeof(long),
                                util::compareLongs);

  cout << "qsort() + bsearch(), milli-seconds : " << (clock() - timeStart)
       << endl;

  if (pItem != nullptr) {
    cout << "found, " << *pItem << endl;
  } else {
    cout << "not found!" << endl;
  }
}
} // namespace array_test

#include <vector>
namespace vector_test {
void test_vector() {
  cout << "test_vector()...\n";
  vector<string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; ++i) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.push_back(string(buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
      abort();
    }
  }
  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "vector.size(): " << c.size() << endl;
  cout << "vector.front(): " << c.front() << endl;
  cout << "vector.back(): " << c.back() << endl;
  cout << "vector.data(): " << c.data() << endl;
  cout << "vector.capacity(): " << c.capacity() << endl;

  string target = util::get_a_target_string();
  {
    timeStart = clock();

    vector<string>::iterator pItem = ::find(c.begin(), c.end(), target);
    cout << "::find(), mill-seconds: " << (clock() - timeStart) << endl;

    if (pItem != c.end()) {
      cout << "found, " << *pItem << endl;
    } else {
      cout << "not found" << endl;
    }
  }

  {
    timeStart = clock();
    sort(c.begin(), c.end());
    string *pItem = (string *)bsearch(&target, (c.data()), c.size(),
                                      sizeof(string), util::compareStrings);
    cout << "sort-bsearch(), mill-seconds: " << (clock() - timeStart) << endl;
    if (pItem != nullptr) {
      cout << "found, " << *pItem << endl;
    } else {
      cout << "not found" << endl;
    }
  }
}
} // namespace vector_test

#include <list>
namespace list_test {
void test_list() {
  cout << "test_list()...\n";
  list<string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; ++i) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.push_back(string(buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
      abort();
    }
  }
  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "list.size(): " << c.size() << endl;
  cout << "list.max_size(): " << c.max_size() << endl;
  cout << "list.front(): " << c.front() << endl;
  cout << "list.back(): " << c.back() << endl;

  string target = util::get_a_target_string();
  {
    timeStart = clock();

    list<string>::iterator pItem = ::find(c.begin(), c.end(), target);
    cout << "::find(), mill-seconds: " << (clock() - timeStart) << endl;

    if (pItem != c.end()) {
      cout << "found, " << *pItem << endl;
    } else {
      cout << "not found" << endl;
    }
  }

  {
    timeStart = clock();
    c.sort();
    cout << "sort-bsearch(), mill-seconds: " << (clock() - timeStart) << endl;
  }
}
} // namespace list_test

#include <forward_list>
namespace flist_test {
void test_flist() {
  cout << "test_flist()...\n";
  forward_list<string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; ++i) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.push_front(string(buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
      abort();
    }
  }
  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "flist.max_size(): " << c.max_size() << endl;
  cout << "flist.front(): " << c.front() << endl;

  string target = util::get_a_target_string();
  {
    timeStart = clock();

    forward_list<string>::iterator pItem = ::find(c.begin(), c.end(), target);
    cout << "::find(), mill-seconds: " << (clock() - timeStart) << endl;

    if (pItem != c.end()) {
      cout << "found, " << *pItem << endl;
    } else {
      cout << "not found" << endl;
    }
  }

  {
    timeStart = clock();
    c.sort();
    cout << "sort-bsearch(), mill-seconds: " << (clock() - timeStart) << endl;
  }
}
} // namespace flist_test

#include <deque>
namespace deque_test {
void test_deque() {
  cout << "test_deque()...\n";
  deque<string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE / 2; ++i) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.push_back(string(buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
      abort();
    }
  }
  for (long i = 0; i < ASIZE / 2; ++i) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.push_front(string(buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
      abort();
    }
  }
  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "deque.size(): " << c.size() << endl;
  cout << "deque.max_size(): " << c.max_size() << endl;
  cout << "deque.front(): " << c.front() << endl;
  cout << "deque.back(): " << c.back() << endl;

  string target = util::get_a_target_string();
  {
    timeStart = clock();

    deque<string>::iterator pItem = ::find(c.begin(), c.end(), target);
    cout << "::find(), mill-seconds: " << (clock() - timeStart) << endl;

    if (pItem != c.end()) {
      cout << "found, " << *pItem << endl;
    } else {
      cout << "not found" << endl;
    }
  }

  {
    timeStart = clock();
    sort(c.begin(), c.end());
    cout << "sort-bsearch(), mill-seconds: " << (clock() - timeStart) << endl;
  }
}
} // namespace deque_test

#include <set>
namespace multiset_test {
void test_multiset() {
  cout << "test_multiset()..... \n";
  multiset<string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; i++) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.insert(string(buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
    }
  }

  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "multiset.size(): " << c.size() << endl;
  cout << "multiset.max_size(): " << c.max_size() << endl;
  string target = util::get_a_target_string();

  {
    timeStart = clock();

    multiset<string>::iterator pItem = ::find(c.begin(), c.end(), target);
    cout << "::find(), mill-seconds: " << (clock() - timeStart) << endl;

    if (pItem != c.end()) {
      cout << "found, " << *pItem << endl;
    } else {
      cout << "not found" << endl;
    }
  }

  {
    timeStart = clock();
    c.find(target);
    cout << "c.find(), mill-seconds: " << (clock() - timeStart) << endl;
  }
}

} // namespace multiset_test

#include <map>
namespace multimap_test {
void test_multimap() {
  cout << "test_multiset()..... \n";
  multimap<long, string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; i++) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.insert(pair<long, string>(i, buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
    }
  }

  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "multimap.size(): " << c.size() << endl;
  cout << "multimap.max_size(): " << c.max_size() << endl;
  long target = util::get_a_target_long();

  {
    timeStart = clock();
    c.find(target);
    cout << "c.find(), mill-seconds: " << (clock() - timeStart) << endl;
  }
}
} // namespace multimap_test

#include <unordered_set>
namespace unordered_multiset_test {
void test_unoredered_multiset() {
  cout << "test_unoredered_multiset().....\n";
  unordered_multiset<string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; i++) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.insert(string(buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
    }
  }

  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "unordered_multiset.size(): " << c.size() << endl;
  cout << "unordered_multiset.max_size(): " << c.max_size() << endl;
  cout << "unordered_multiset.bucket_count(): " << c.bucket_count() << endl;
  cout << "unordered_multiset.load_factor(): " << c.load_factor() << endl;
  cout << "unordered_multiset.max_load_factor(): " << c.max_load_factor()
       << endl;
  cout << "unordered_multiset.max_bucket_count(): " << c.max_bucket_count()
       << endl;

  for (unsigned i = 0; i < 20; i++) {
    cout << "B[" << i << "] hash " << c.bucket_size(i) << " elems\n";
  }
  string target = util::get_a_target_string();
  {
    timeStart = clock();

    auto pItem = ::find(c.begin(), c.end(), target);
    cout << "::find(), mill-seconds: " << (clock() - timeStart) << endl;

    if (pItem != c.end()) {
      cout << "found, " << *pItem << endl;
    } else {
      cout << "not found" << endl;
    }
  }

  {
    timeStart = clock();
    c.find(target);
    cout << "c.find(), mill-seconds: " << (clock() - timeStart) << endl;
  }
}
} // namespace unordered_multiset_test

#include <unordered_map>
namespace unordered_multimap_test {
void test_unoredered_multimap() {
  cout << "test_unoredered_multimap().......\n";
  unordered_multimap<long, string> c;
  char buf[10];

  clock_t timeStart = clock();
  for (long i = 0; i < ASIZE; i++) {
    try {
      snprintf(buf, 10, "%d", rand());
      c.insert(pair<long, string>(i, buf));
    } catch (exception &p) {
      cout << "i=" << i << " " << p.what() << endl;
    }
  }

  cout << "mill-seconds: " << (clock() - timeStart) << endl;
  cout << "unordered_multiset.size(): " << c.size() << endl;
  cout << "unordered_multiset.max_size(): " << c.max_size() << endl;
  cout << "unordered_multiset.bucket_count(): " << c.bucket_count() << endl;
  cout << "unordered_multiset.load_factor(): " << c.load_factor() << endl;
  cout << "unordered_multiset.max_load_factor(): " << c.max_load_factor()
       << endl;
  cout << "unordered_multiset.max_bucket_count(): " << c.max_bucket_count()
       << endl;

  for (unsigned i = 0; i < 20; i++) {
    cout << "B[" << i << "] hash " << c.bucket_size(i) << " elems\n";
  }
  long target = util::get_a_target_long();
  {
    timeStart = clock();
    c.find(target);
    cout << "c.find(), mill-seconds: " << (clock() - timeStart) << endl;
  }
}
} // namespace unordered_multimap_test

namespace set {
void test_set() { cout << "test_set().......\n"; }
} // namespace set

namespace map_test {
void test_map() { cout << "test_map().......\n"; }
} // namespace map_test

namespace unoredred_set_test {
void test_unoredered_set() { cout << "test_unoredered_set().......\n"; }
} // namespace unoredred_set_test

namespace unoredred_map_test {
void test_unoredered_map() { cout << "test_unoredered_map().......\n"; }
} // namespace unoredred_map_test



int main(int argc, char *argv[]) {

  return 0;
}
