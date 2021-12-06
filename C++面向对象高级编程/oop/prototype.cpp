#include <iostream>

using namespace std;

enum imageType{
  LSAT,
  SPOT,
};

class Image
{
public:
  virtual void draw() = 0;
  static Image* findAndClone(imageType);
protected:
  virtual imageType returnType() = 0;
  virtual Image *clone() = 0;
  // As each subclass of Image is declared, it registers its protype
  static void addProtoType(Image *image) {
    _prototypes[_nextSlot++] = image;
  }
private:
  // addPrototype() saves each registered prototype here
  // 声明
  static Image* _prototypes[10];
  static int _nextSlot;
};

// 定义
Image *Image::_prototypes[];

// 定义
int Image::_nextSlot;

Image *Image::findAndClone(imageType type) {
  for (int i = 0;i < _nextSlot;i++) {
    if (_prototypes[i]->returnType() == type) {
      return _prototypes[i]->clone();
    }
  }
  return nullptr;
}

class LandSatImage: public Image
{
public:
  imageType returnType(){
    return LSAT;
  }

  void draw() {
    cout << "LSAT::draw" << _id << endl;
  }

  Image *clone() {
    return new LandSatImage;
  }
protected:
  LandSatImage(int foo) {
    _id = _count++;
  }

private:
  static LandSatImage _landSatImage;
  LandSatImage() {
    addProtoType(this);
  }
  int _id;
  static int _count;
};

LandSatImage LandSatImage::_landSatImage;
int LandSatImage::_count = 1;
