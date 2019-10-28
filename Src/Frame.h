#ifndef _FRAME_H
#define _FRAME_H
#include "Shape.h"
#include "Array.h"
using namespace std;
class ShapeVisitor;
class Frame : public Shape{
public:
	Frame(Long capacity=100);
	Frame(int x, int y, int width, int height);
	Frame(int x, int y, int width, int height, bool emphasize);
	~Frame();
	Frame(const Frame& source);
	Frame& operator=(const Frame& source);
	int Add(Shape *shape);
	int Remove(Shape *shape);
	Shape* Remove(int index);
	Shape* GetChild(int index);
	int Find(Shape *shape);
	virtual Frame* Clone()const;
	virtual void Accept(ShapeVisitor *v)override;
	virtual Iterator<Shape*>* CreateIterator();
	Frame& Resize(int x, int y, int width, int height);
	int MeasureTotalHeight();
	void Node_MeasureLowRankFrame(int *height);
	void LeftRadialSort();
	void Node_LeftRadialSort(int frameX,int frameY,int frameWidth,int frameHeight,int startY);
	void RightRadialSort();
	void Node_RightRadialSort(int frameX, int frameY, int frameWidth, int frameHeight, int startY);
	void RadialSort();
	void Emphasize();
	int GetCapacity()const;
	int GetLength()const;
	bool GetEmphasize()const;
private:
	Array<Shape*> children;
	int length;
	int capacity;
	bool emphasize;
};
inline int Frame::GetCapacity()const {
	return this->capacity;
}
inline int Frame::GetLength()const {
	return this->length;
}
inline bool Frame::GetEmphasize()const {
	return this->emphasize;
}
int CompareShapes(void *one, void *other);
#endif//_FRAME_H

