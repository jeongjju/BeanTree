// ConnectingLine.h
#ifndef _CONNECTINGLINE_H
#define _CONNECTINGLINE_H
#include"Shape.h"
class ConnectingLine :public Shape {
public:
	ConnectingLine();
	ConnectingLine(int x, int y, int childX, int childY);
	~ConnectingLine();
	ConnectingLine(const ConnectingLine& source);
	ConnectingLine& operator=(const ConnectingLine& source);
	virtual ConnectingLine* Clone() const;
	virtual void Accept(ShapeVisitor *v);
	ConnectingLine& Resize(int x, int y, int width, int height);
	int GetChildX();
	int GetChildY();
private:
	int childX;
	int childY;
};
inline int ConnectingLine::GetChildX() {
	return this->childX;
}
inline int ConnectingLine::GetChildY() {
	return this->childY;
}
#endif // _CONNECTINGLINE_H