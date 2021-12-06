#include <iterator>

template<typename _Iter>
inline typename std::iterator_traits<_Iter>::iterator_category
__tierator_category(const _Iter&)
{
  return typename std::iterator_traits<_Iter>::iterator_category();
} // 返回分类 [1]

template<typename _RandonAccessIterator>
void __rotate(
  _RandonAccessIterator __first,
  _RandonAccessIterator __middle,
  _RandonAccessIterator __last,
  std::random_access_iterator_tag) // 见[1]
{
  typedef typename std::iterator_traits<_RandonAccessIterator>::difference_type _Distance; // it的距离
  typedef typename std::iterator_traits<_RandonAccessIterator>::value_type _valueType; // it的值类型
  // ...
}

template<typename _ForwardIterator>
inline void
rotate(_ForwardIterator __first,
       _ForwardIterator __middle,
       _ForwardIterator __last)
{
  __rotate(__first, __middle, __last , std::__iterator_category(__first));
}
