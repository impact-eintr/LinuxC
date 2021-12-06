#include <exception>
#include <ext/pool_allocator.h>

template <class T, class Alloc = __gnu_cxx::__pool_alloc<T>> class vector {
public:
  typedef T value_type;
  typedef value_type *iterator; // T*
  typedef value_type &reference;
  typedef size_t size_type;

protected:
  iterator start;
  iterator finish;
  iterator end_of_storage;

public:
  iterator begin() { return start; }
  iterator end() { return finish; }
  size_type size() const { return size_type(end() - begin()); }
  size_type capacity() const { return size_type(end_of_storage() - begin()); }
  bool empty() const { return begin() == end(); }
  reference operator[](size_type n) { return *(begin() + n); }
  reference front() { return *begin(); }
  reference back() { return *(end() - 1); }

  void push_back(const value_type& x);

private:
  void insert_aux(iterator, const value_type&);
};

template<class T, class Alloc>
void vector<T,Alloc>::push_back(const T& x) {
  if (finish != end_of_storage) { // 还有备用空间
    construct(finish, x); // 全局函数
    ++finish; // 调整水位
  } else { // 没有备用空间
    insert_aux(end(), x);
  }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(vector<T, Alloc>::iterator position, const T& x) {
  if (finish != end_of_storage) {
    // 在备用空间起始处构建一个元素 并以vector最后一个元素作为其初值
    construct(finish, *(finish-1)); // 全局函数
    ++finish; // 调整水位
    T x_copy = x;
    copy_backward(position, finish - 2, finish - 1);
    *position  = x_copy;
  } else { // 没有备用空间
    const size_type old_size = size();
    const size_type len = old_size != 0 ? 2 * old_size : 1; // 扩展二倍
    iterator new_start = data_allocator::alocate(len);
    iterator new_finish = new_start;
    try {
      // 将原有的cevtor的内容拷贝到新的vector
      new_finish = uninitialized_copy(start, position, new_start);
      construct(new_finish, x); // 为新元素
      ++new_finish;
      // 拷贝插入点后的原本内容 这是给 insert 用的
      new_finish = uninitialized_copy(position, finish, new_finish);
    } catch(std::exception& e) {
      destroy(new_start, new_finish);
      data_allocator::dealocate(new_start, len);
      throw;
    }
    // 析构并释放原vector
    destroy(begin(), end());
    deallocate();
    // 调整迭代器 指向新的vector
    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + len;
  }
}
