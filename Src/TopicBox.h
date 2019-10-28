//TopicBox.h
#ifndef _TOPICBOX_H
#define _TOPICBOX_H
#include "Shape.h"
using namespace std;
class TopicBox : public Shape{
public:
	TopicBox(int capacity=100);
	TopicBox(int x, int y, int  width, int height, TCHAR(*topic));
	~TopicBox();
	TopicBox(const TopicBox& source);
	TopicBox& operator=(const TopicBox& source);
	int GetCurrentIndex()const;
	TCHAR* GetTopic()const;
	int GetLength()const;
	int GetCapacity()const;
	virtual TopicBox* Clone()const;
	virtual void Accept(ShapeVisitor *v)override;
	TopicBox& Resize(int x, int y, int width, int height);
	TopicBox& Modify(int x, int y, int width, int height,TCHAR (*topic),int currentIndex);
private:
	int currentIndex;
	TCHAR (*topic);
	int capacity;
	int length;
};
inline int TopicBox::GetCurrentIndex()const {
	return this->currentIndex;
}
inline TCHAR* TopicBox::GetTopic()const {
	return this->topic;
}
inline int TopicBox::GetCapacity()const {
	return this->capacity;
}
inline int TopicBox::GetLength()const {
	return this->length;
}
#endif//_TOPICBOX_H
