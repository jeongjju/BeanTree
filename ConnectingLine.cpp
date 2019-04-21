//ConnectingLine.cpp
#include"ConnectingLine.h"
#include"ShapeVisitor.h"

ConnectingLine::ConnectingLine()
	:Shape(0, 0, 0, 0), childX(0), childY(0)
{
}
ConnectingLine::ConnectingLine(int x, int y, int childX, int childY)
	: Shape(x, y, 0, 0), childX(childX), childY(childY)
{
}
ConnectingLine::~ConnectingLine() {}
ConnectingLine::ConnectingLine(const ConnectingLine& source)
	: Shape(source.x, source.y, 0, 0), childX(source.childX), childY(source.childY)
{
}
ConnectingLine& ConnectingLine:: operator=(const ConnectingLine& source)
{
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	this->childX = source.childX;
	this->childY = source.childY;
	return *this;
}
ConnectingLine* ConnectingLine::Clone() const {
	return new ConnectingLine(*this);
}
void ConnectingLine::Accept(ShapeVisitor *v) {
	v->VisitConnectingLine(this);
}
ConnectingLine& ConnectingLine::Resize(int x, int y, int childX, int childY) {
	this->x = x;
	this->y = y;
	this->childX = childX;
	this->childY = childY;
	return *this;
}

//#include<iostream>
//using namespace std;
//int main(int argc, char *argv[]) {
//	ConnectingLine connectingLine(10,20,30,40);
//	cout << " connectingLine x " << connectingLine.GetX() << " connectingLine y " << connectingLine.GetY() << " connectingLine width " << connectingLine.GetWidth() <<
//		" connectingLine height " << connectingLine.GetHeight() << " connectingLine childX " << connectingLine.GetChildX() << " connectingLIne ChildY " << connectingLine.GetChildY() << endl;
//	
//	connectingLine.Resize(100, 100, 100, 100);
//	cout << " connectingLine x " << connectingLine.GetX() << " connectingLine y " << connectingLine.GetY() << " connectingLine width " << connectingLine.GetWidth() <<
//		" connectingLine height " << connectingLine.GetHeight() << " connectingLine childX " << connectingLine.GetChildX() << " connectingLIne ChildY " << connectingLine.GetChildY() << endl;
//
//	ConnectingLine connectingLine2(200, 200, 200, 200);
//	cout << " connectingLine2 x " << connectingLine2.GetX() << " connectingLine2 y " << connectingLine2.GetY() << " connectingLine2 width " << connectingLine2.GetWidth() <<
//		" connectingLine2 height " << connectingLine2.GetHeight() << " connectingLine2 childX " << connectingLine2.GetChildX() << " connectingLine2 ChildY " << connectingLine2.GetChildY() << endl;
//
//	ConnectingLine connectingLine3(connectingLine2);
//	cout << " connectingLine3 x " << connectingLine3.GetX() << " connectingLine3 y " << connectingLine3.GetY() << " connectingLine3 width " << connectingLine3.GetWidth() <<
//		" connectingLine3 height " << connectingLine3.GetHeight() << " connectingLine3 childX " << connectingLine3.GetChildX() << " connectingLine3 ChildY " << connectingLine3.GetChildY() << endl;
//
//	connectingLine3 = connectingLine;
//	cout << " connectingLine3 x " << connectingLine3.GetX() << " connectingLine3 y " << connectingLine3.GetY() << " connectingLine3 width " << connectingLine3.GetWidth() <<
//		" connectingLine3 height " << connectingLine3.GetHeight() << " connectingLine3 childX " << connectingLine3.GetChildX() << " connectingLine3 ChildY " << connectingLine3.GetChildY() << endl;
//
//	return 0;
//}