//Image.cpp
#include "stdafx.h"
#include "Image.h"
#include "ShapeVisitor.h"

Image::Image() :Shape(100, 100, 200, 200),imagePath(""){
}
Image::Image(int x,int y,int width,int height,string imagePath) : Shape(x,y,width,height),imagePath(imagePath){
}
Image::~Image() {
}
Image::Image(const Image& source) : Shape(source.x,source.y,source.width,source.height),imagePath(source.imagePath){
}
Image& Image::operator=(const Image& source) {
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	this->imagePath = source.imagePath;
	return *this;
}
Image* Image::Clone()const {
	return new Image(*this);
}
void Image::Accept(ShapeVisitor *v) {
	v->VisitImage(this);
}
Image& Image::Resize(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	return *this;
}

//#include<iostream>
//int main(int argc, char *argv[]) {
//	Image image;
//	cout << "image x " << image.GetX() << "image y " << image.GetY() << "image width " << image.GetWidth() << "image height " << image.GetHeight()
//		<< "imagePath " << image.GetImagePath() << endl;
//	image.Resize(10, 10, 10, 10);
//	cout << "image x " << image.GetX() << "image y " << image.GetY() << "image width " << image.GetWidth() << "image height " << image.GetHeight()
//		<< "imagePath " << image.GetImagePath() << endl;
//
//	Image image2(10, 10, 10, 10, "C:\\Users\\houe0\\Desktop\\박컴학원\\5단계 프로젝트");
//	cout << "image2 x " << image2.GetX() << "image2 y " << image2.GetY() << "image2 width " << image2.GetWidth() << "image2 height " << image2.GetHeight()
//		<< "image2Path " << image2.GetImagePath() << endl;
//	
//	Image *image3 = new Image(image);
//	cout << "image3 x " << image3->GetX() << "image3 y " << image3->GetY() << "image3 width " << image3->GetWidth() << "image3 height " << image3->GetHeight()
//		<< "image3Path " << image3->GetImagePath() << endl;
//
//	*image3 = image2;
//	cout << "image3 x " << image3->GetX() << "image3 y " << image3->GetY() << "image3 width " << image3->GetWidth() << "image3 height " << image3->GetHeight()
//		<< "image3Path " << image3->GetImagePath() << endl;
//
//	return 0;
//}	