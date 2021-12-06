class stone
{
public:
  stone(int w, int h, int we) : _w(w), _h(h), _weight(we) {}
  bool operator< (const stone& rhs) const {
    return _weight < rhs._weight;
  }
private:
  int _w, _h, _weight;
};

template <class T>
inline const T& min(const T& a, const T& b) {
  return b < a ? b : a;
}
