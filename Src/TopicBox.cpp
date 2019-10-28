//TopicBox.cpp
#include "stdafx.h"
#include "TopicBox.h"
#include "ShapeVisitor.h"

TopicBox::TopicBox(int capacity) :Shape(100,100,200,200){
	this->topic = new TCHAR[capacity];
	this->length = 0;
	this->capacity = capacity;
}
TopicBox::TopicBox(int x,int y,int width,int height,TCHAR(*topic)):Shape(x,y,width,height){
	int i = 0;
	int count = 0;

	while (topic[i] != '\0') {
		count++;
		i++;
	}
	this->topic = new TCHAR[count+1];
	i=0;
	while (i<count)
	{
		this->topic[i] = topic[i];
		i++;
	}
	this->topic[i] = '\0';
	this->length = count+1;
	this->capacity = count+1;
	this->currentIndex = this->length - 1;
}
TopicBox::~TopicBox() {
	if (this->topic != 0) {
		delete[]this->topic;
	}
}
TopicBox::TopicBox(const TopicBox& source):Shape(source.x,source.y,source.width,source.height){
	if (source.capacity > 0) {
		this->topic = new TCHAR[source.capacity];
	}
	int i = 0;
	while (i < source.length) {
		this->topic[i] = source.topic[i];
		i++;
	}
	this->length = source.length;
	this->capacity = source.capacity;
	this->currentIndex = source.currentIndex;
}
TopicBox& TopicBox::operator=(const TopicBox& source) {

	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	if (this->topic != 0) {
		delete[]this->topic;
	}
	if (source.capacity > 0) {
		this->topic = new TCHAR[source.capacity];
	}
	int i = 0;
	while (i < source.length) {
		this->topic[i] = source.topic[i];
		i++;
	}
	this->length = source.length;
	this->capacity = source.capacity;
	this->currentIndex = source.currentIndex;
	return *this;
}
TopicBox* TopicBox::Clone()const {
	return new TopicBox(*this);
}
void TopicBox::Accept(ShapeVisitor *v) {
	v->VisitTopicBox(this);
}
TopicBox& TopicBox::Resize(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	return *this;
}
TopicBox& TopicBox::Modify(int x, int y, int width, int height,TCHAR (*topic),int currentIndex) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->currentIndex = currentIndex;
	int topicCount=0;
	
	/*if (this->topic != 0) {
		delete[]this->topic;
		this->length = 0;
		this->capacity = 0;
	}*/
	int i = 0;
	while (topic[i]!='\0') {
		topicCount++;
		i++;
	}
	if (topicCount > 0) {
		this->topic=new TCHAR[topicCount+1];
		this->capacity = topicCount + 1;
	}
	i = 0;
	while (i < topicCount) {
		this->topic[i] = topic[i];
		this->length++;
		i++;
	}
	this->topic[i] = '\0';
	this->length++;
	return *this;
}

//#include<iostream>
//using namespace std;
//int main(int argc, char *argv[]) {
//	TopicBox topicBox(10, 20, 30, 40, _T("중심토픽"));
//	cout << " x " << topicBox.GetX() << " y " << topicBox.GetY() << " width " << topicBox.GetWidth() << " height " << topicBox.GetHeight() << " topic " << topicBox.GetTopic()
//		<< endl;
//	
//	TopicBox topicBox2(topicBox);
//	cout << " x " << topicBox2.GetX() << " y " << topicBox2.GetY() << " width " << topicBox2.GetWidth() << " height " << topicBox2.GetHeight() << " topic " << topicBox2.GetTopic() << endl;
//	
//	TopicBox topicBox3;
//	cout << " x " << topicBox3.GetX() << " y " << topicBox3.GetY() << " width " << topicBox3.GetWidth() << " height " << topicBox3.GetHeight() << " topic " << topicBox3.GetTopic() << endl;
//	
//	topicBox3 = topicBox2;
//	cout << " x " << topicBox3.GetX() << " y " << topicBox3.GetY() << " width " << topicBox3.GetWidth() << " height " << topicBox3.GetHeight() << " topic " << topicBox3.GetTopic() << endl;
//	
//	topicBox3.Resize(50, 50, 50, 50);
//	cout << " x " << topicBox3.GetX() << " y " << topicBox3.GetY() << " width " << topicBox3.GetWidth() << " height " << topicBox3.GetHeight() << " topic " << topicBox3.GetTopic() << endl;
//	
//	topicBox3.Modify(60, 60, 60, 60, _T("토픽"),5);
//	cout << " x " << topicBox3.GetX() << " y " << topicBox3.GetY() << " width " << topicBox3.GetWidth() << " height " << topicBox3.GetHeight() << " topic " << topicBox3.GetTopic() << endl;
//
//	return 0;
//}