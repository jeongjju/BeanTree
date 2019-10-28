//DrawVisitor.h
#ifndef _DRAWVISITOR_H
#define _DRAWVISITOR_H
#include "ShapeVisitor.h"
class DrawVisitor :public ShapeVisitor{
public:
	virtual void VisitTopicBox(TopicBox *e)override;
	virtual void VisitImage(Image *e)override;
	virtual void VisitFrame(Frame *e)override;
	virtual void VisitConnectingLine(ConnectingLine *connectingLine)override;
};
#endif // _DRAWVISITOR_H
