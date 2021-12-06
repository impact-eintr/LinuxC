class StringRep;
class String {
public:
  String();
  String(const char* s);
  String(const String& s);
  String &operator=(const String& s);
  ~String();
private:
  StringRep *rep;
};

namespace{
  class StringRep {
    friend class String;
    StringRep(const char*);
    ~StringRep();
    int count;
    char* rep;
  };
}
