#include <ext/pool_allocator.h>

template <class T>
struct __list_node {
  typedef void* void_pointer;
  void_pointer prev;
  void_pointer next;
  T data;
};

template <class T, class Ref, class Ptr>
struct __list_iterator {

  typedef __list_iterator<T, Ref, Ptr> self;
  // TODO
  typedef T value_type;
  typedef Ptr pointer;
  typedef Ref reference;

  typedef __list_node<T>* link_type;
  // TODO
  link_type node;

  reference operator*() const { return (*node).data; }
  pointer operator->() const { return &(operator*()); }
  // self& 和 self的区别是为了处理 ++++i i++++(这种c++不支持)
  // ++i
  self& operator++() {
    node = (link_type)((*node).next);
    return *this;
  }
  // i++
  self operator++(int) {
    self tmp = *this;
    ++*this;
    return tmp;
  }
};

template <class T, class Alloc = __gnu_cxx::__pool_alloc<T>> class list {
  protected:
    typedef __list_node<T> list_node;
  public:
    typedef list_node* link_type;
    typedef __list_iterator<T, T&, T*> iterator;
  protected:
    link_type node; // 8bytes
};
