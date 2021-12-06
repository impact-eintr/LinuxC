template <class T, class Qequence = deque<T> >
class queue {
protected:
  Sequence c; // 底层容器
public:
  // 一下完全利用c的操作函数完成
  bool empty() const {return c.empty();}
  size_type size() const {return c.size();}
  reference front() { return c.front();}
  reference back() {return c.back();}
  void push(const value_type& x) { c.push_back(x);}
  void pop() { c.pop_front();}
}
