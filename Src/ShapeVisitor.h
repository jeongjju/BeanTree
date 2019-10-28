//ShapeVisitor.h
#ifndef _SHAPEVISITOR
#define _SHAPEVISITOR

class TopicBox;
class Image;
class Frame;
class ConnectingLine;
class ShapeVisitor {
public:
	virtual void VisitTopicBox(TopicBox *e) = 0;
	virtual void VisitImage(Image *e) = 0;
	virtual void VisitFrame(Frame *e)= 0;
	virtual void VisitConnectingLine(ConnectingLine *e) = 0;
};
#endif // _SHAPEVISITOR
