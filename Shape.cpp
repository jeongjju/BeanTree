//Shape.cpp
#include"Shape.h"
Shape::Shape():x(100),y(100),width(200),height(200){}
Shape::~Shape(){}
Shape::Shape(int x, int y, int width, int height) :x(x),y(y),width(width),height(height){}
int Shape::Add(Shape *shape) { return -1; }	
int Shape::Remove(Shape *shape) {return -1;}
Shape* Shape::Remove(int index) { return 0;}
Shape* Shape::GetChild(int index) { return 0;}
int Shape::Find(Shape *shape) { return -1;}
Shape::Shape(const Shape& source) : x(source.x),y(source.y),width(source.width),height(source.height){
}
Shape& Shape::operator=(const Shape& source) {
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	return *this;
}
Iterator<Shape*>* Shape::CreateIterator() { return 0; }