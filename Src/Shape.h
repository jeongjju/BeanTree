//Shape.h
#ifndef _SHAPE_H
#define _SHAPE_H
#include"Iterator.h"
class ShapeVisitor;
class Shape {
public:
	Shape();
	Shape(int x, int y, int width, int height);
	virtual int Add(Shape *shape);
	virtual int Remove(Shape *shape);
	virtual Shape* Remove(int index);
	virtual Shape* GetChild(int index);
	virtual int Find(Shape *shape);
	virtual ~Shape()=0;
	Shape(const Shape& source);
	Shape& operator=(const Shape& source);
	virtual Shape* Clone() const = 0;
	virtual void Accept(ShapeVisitor *v) = 0;
	virtual Iterator<Shape*>* CreateIterator();

	int GetX()const;
	int GetY()const;
	int GetWidth()const;
	int GetHeight()const;
protected:
	int x;
	int y;
	int width;
	int height;
};
inline int Shape::GetX()const {
	return this->x;
}
inline int Shape::GetY()const {
	return this->y;
}
inline int Shape::GetWidth()const {
	return this->width;
}
inline int Shape::GetHeight()const {
	return this->height;
}
#endif//_SHAPE_H
