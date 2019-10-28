//Image.h
#ifndef _IMAGEBOX_H
#define _IMAGEBOX_H
#include "Shape.h"
using namespace std;
class Image : public Shape{
public:
	Image();
	Image(int x,int y,int width,int height,string imagePath);
	~Image();
	Image(const Image& source);
	Image& operator=(const Image& source);
	virtual Image* Clone()const;
	virtual void Accept(ShapeVisitor *v)override;
	Image& Resize(int x, int y, int width, int height);
	string& GetImagePath()const;

private:
	string imagePath;
};
inline string& Image::GetImagePath()const {
	return const_cast<string&>(this->imagePath);
}
#endif//_IMAGE_H
