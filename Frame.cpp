//Frame.cpp
#include"Frame.h"
#include"TopicBox.h"
#include"Image.h"
#include"ShapeVisitor.h"
#include"FrameIterator.h"
#include"ConnectingLine.h"
Frame::Frame(Long capacity) :Shape(100, 100, 200, 200), children(capacity), capacity(capacity), length(0),emphasize(false) {
}

Frame::~Frame() {
	int i = 0;
	while (i < this->length) {
		delete this->children[i];
		i++;
	}
}

Frame::Frame(int x, int y, int width, int height) : Shape(x, y, width, height), children(100), capacity(100), length(0),emphasize(false) {}

int Frame::Add(Shape *shape) {
	int index;
	if (this->length < this->capacity) {
		index = this->children.Store(this->length, shape);
	}
	else {
		index = this->children.AppendFromRear(shape);
		this->capacity++;
	}
	if (dynamic_cast<Frame*>(shape)) {
		((Frame*)shape)->Add(new ConnectingLine(this->x + this->width / 2, this->y + this->height / 2, shape->GetX() + shape->GetWidth() / 2,
			shape->GetY() + shape->GetHeight() / 2));
	}
	this->length++;
	return index;
}

Frame::Frame(int x, int y, int width, int height, bool emphasize)
	: Shape(x,y,width,height),children(100),capacity(100),length(0),emphasize(emphasize){}

int Frame::Remove(Shape *shape) {
	Long index = children.LinearSearchUnique(shape, CompareShapes);
	delete this->children[index];
	int ret = (int)children.Delete(index);
	this->capacity--;
	this->length--;
	return ret;
}

Shape* Frame::Remove(int index) {
	Shape *shape = this->children[index];
	delete shape;
	Long ret = children.Delete(index);
	this->capacity--;
	this->length--;
	if (ret == -1) {
		shape = 0;
	}
	return shape;
}
Shape* Frame::GetChild(int index) {
	return this->children.GetAt(index);
}
int Frame::Find(Shape *shape) {
	int index = this->children.LinearSearchUnique(shape, CompareShapes);
	return index;
}
Frame::Frame(const Frame& source) :Shape(source.x, source.y, source.width, source.height), children(source.capacity), capacity(source.capacity),emphasize(source.emphasize) {
	int i = 0;
	this->length = 0;
	Shape* shape;
	while (i < source.GetLength()) {
		shape = (const_cast<Frame&>(source)).GetChild(i);
		this->children.Store(i, shape->Clone());
		this->length++;
		i++;
	}
}
Frame& Frame::operator=(const Frame& source) {
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	this->capacity = source.capacity;
	this->emphasize = source.emphasize;

	int i = 0;
	while (i < this->length) {
		delete this->children[i];
		i++;
	}
	this->length = 0;
	i = 0;
	Shape* shape;
	while (i < source.GetLength()) {
		shape = (const_cast<Frame&>(source)).GetChild(i);
		this->children.Store(i, shape->Clone());
		this->length++;
		i++;
	}
	return *this;
}
Frame* Frame::Clone()const {
	// 복사생성자
	return new Frame(*this);
}
void Frame::Accept(ShapeVisitor *v) {
	for (FrameIterator i(this); !i.IsDone(); i.Next()) {
		if (dynamic_cast<Frame*>(i.Current())) {
			int j = 0;
			while(j<((Frame*)i.Current())->GetLength()) {
				Shape *shape = i.Current()->GetChild(j);
				if (dynamic_cast<ConnectingLine*>(shape)) {
					shape->Accept(v);
				}
				j++;
			}
		}
	}
	v->VisitFrame(this);
	for (FrameIterator i(this); !i.IsDone(); i.Next()) {
		if (!dynamic_cast<ConnectingLine*>(i.Current())) {
			i.Current()->Accept(v);
		}
	}
}
Iterator<Shape*>* Frame::CreateIterator() {
	return new FrameIterator(this);
}
Frame& Frame::Resize(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	return *this;
}

int Frame::MeasureTotalHeight() {
	int totalHeight = 0;
	Shape *shape;
	int i = 0;
	int height = 0;
	while (i < this->length) {
		shape = this->GetChild(i);
		if (dynamic_cast<Frame*>(shape)) {
			height = ((Frame*)shape)->GetHeight();
			((Frame*)shape)->Node_MeasureLowRankFrame(&height);
			if (totalHeight > 0) {
				totalHeight += 10;
			}
			totalHeight += height;
		}
		i++;
	}
	if (totalHeight == 0) {
		totalHeight = this->height;
	}
	return totalHeight;
}

void Frame::Node_MeasureLowRankFrame(int *height) {
	int lowTotalHeight = 0;
	Shape *shape;
	int i = 0;
	int frameHeight = 0;
	while (i < this->length) {
		shape = this->GetChild(i);
		if (dynamic_cast<Frame*>(shape)) {
			frameHeight = shape->GetHeight();
			((Frame*)shape)->Node_MeasureLowRankFrame(&frameHeight);
			if (lowTotalHeight > 0) {
				lowTotalHeight += 10;
			}
			lowTotalHeight = lowTotalHeight + frameHeight;
		}
		i++;
	}
	if (lowTotalHeight > *height) {
		*height = lowTotalHeight;
	}
}

void Frame::LeftRadialSort() {
	int startY = (this->y + this->y + height) / 2 - this->MeasureTotalHeight() / 2;
	int i = 0;
	while (i < this->length) {
		if (dynamic_cast<Frame*>(this->GetChild(i))) {
			Frame *frame = (Frame*)this->GetChild(i);
			frame->Node_LeftRadialSort(this->x, this->y, this->width, this->height, startY);
			int totalHeight = frame->MeasureTotalHeight();
			startY = startY + totalHeight + 10;
		}
		i++;
	}
}
void Frame::Node_LeftRadialSort(int frameX, int frameY, int frameWidth, int frameHeight, int startY) {
	int	lowTotalHeight = this->MeasureTotalHeight();
	int lowCenterX = frameX - 10 - this->width / 2;
	int lowCenterY = startY + lowTotalHeight / 2;
	this->Resize(lowCenterX - this->width / 2, lowCenterY - this->height / 2, this->width, this->height);
	int j = 0;
	while (j < this->length) {
		if (dynamic_cast<ConnectingLine*>(this->GetChild(j))) {
			ConnectingLine *connectingLine = (ConnectingLine*)this->GetChild(j);
			connectingLine->Resize((frameX + frameX + frameWidth) / 2, (frameY + frameY + frameHeight) / 2, lowCenterX, lowCenterY);
		}
		else if (dynamic_cast<TopicBox*>(this->GetChild(j))) {
			TopicBox *topicBox = (TopicBox*)this->GetChild(j);
			topicBox->Resize(lowCenterX - 45, lowCenterY - 20, topicBox->GetWidth(), topicBox->GetHeight());
		}
		j++;
	}
	int i = 0;
	Frame *frame;
	while (i < this->length) {
		if (dynamic_cast<Frame*>(this->GetChild(i))) {
			frame = (Frame*)this->GetChild(i);
			frame->Node_LeftRadialSort(this->x, this->y, this->width, this->height, startY);
			int totalHeight = frame->MeasureTotalHeight();
			startY = startY + totalHeight + 10;
		}
		i++;
	}
}

void Frame::RightRadialSort() {
	int startY = (this->y + this->y + height) / 2 - this->MeasureTotalHeight() / 2;
	int i = 0;
	while (i < this->length) {
		if (dynamic_cast<Frame*>(this->GetChild(i))) {
			Frame *frame = (Frame*)this->GetChild(i);
			frame->Node_RightRadialSort(this->x, this->y, this->width, this->height, startY);
			int totalHeight = frame->MeasureTotalHeight();
			startY = startY + totalHeight + 10;
		}
		i++;
	}
}

void Frame::Node_RightRadialSort(int frameX, int frameY, int frameWidth, int frameHeight, int startY) {
	int lowTotalHeight = this->MeasureTotalHeight();
	int lowCenterX = frameX + frameWidth + 10 + this->width / 2;
	int lowCenterY = startY + lowTotalHeight / 2;
	this->Resize(lowCenterX - this->width / 2, lowCenterY - this->height / 2, this->width, this->height);
	int j = 0;
	while (j < this->length) {
		if (dynamic_cast<ConnectingLine*>(this->GetChild(j))) {
			ConnectingLine *connectingLine = (ConnectingLine*)this->GetChild(j);
			connectingLine->Resize((frameX + frameX + frameWidth) / 2, (frameY + frameY + frameHeight) / 2, lowCenterX, lowCenterY);
		}
		else if (dynamic_cast<TopicBox*>(this->GetChild(j))) {
			TopicBox *topicBox = (TopicBox*)this->GetChild(j);
			topicBox->Resize(lowCenterX - 45, lowCenterY - 20, topicBox->GetWidth(), topicBox->GetHeight());
		}
		j++;
	}
	int i = 0;
	Frame *frame;
	while (i < this->length) {
		if (dynamic_cast<Frame*>(this->GetChild(i))) {
			frame = (Frame*)this->GetChild(i);
			frame->Node_RightRadialSort(this->x, this->y, this->width, this->height, startY);
			int totalHeight = frame->MeasureTotalHeight();
			startY = startY + totalHeight + 10;
		}
		i++;
	}
}

void Frame::RadialSort() {
	int i = 0;
	int j = 0;
	int totalHeight = 0;
	int count = 0;
	Frame*(*indexes) = new Frame*[this->length];
	while (i < this->length) {
		if (dynamic_cast<Frame*>(this->GetChild(i))) {
			Frame *frame = (Frame*)this->GetChild(i);
			indexes[j] = frame;
			count++;
			int frameTotalHeight = frame->MeasureTotalHeight();
			totalHeight += frameTotalHeight;
			j++;
		}
		i++;
	}
	int halfTotalHeight = totalHeight / 2;
	int sum = 0;
	int halfIndex = 0;
	int leftTotalHeight = 0;
	int rightTotalHeight = 0;
	i = 0;
	while (i < count) {
		sum += indexes[i]->MeasureTotalHeight();
		if (halfTotalHeight >= sum) {
			int frameTotalHeight = indexes[i]->MeasureTotalHeight();
			leftTotalHeight += frameTotalHeight;
			halfIndex++;
		}
		else {
			int frameTotalHeight = indexes[i]->MeasureTotalHeight();
			rightTotalHeight += frameTotalHeight;
		}
		i++;
	}
	int leftStartY = (this->y + this->y + height) / 2 - leftTotalHeight / 2;
	int rightStartY = (this->y + this->y + height) / 2 - rightTotalHeight / 2;
	i = 0;
	while (i < count) {
		if (i < halfIndex) {
			indexes[i]->Node_LeftRadialSort(this->x, this->y, this->width, this->height, leftStartY);
			int totalHeight = indexes[i]->MeasureTotalHeight();
			leftStartY = leftStartY + totalHeight + 10;
		}
		else {
			indexes[i]->Node_RightRadialSort(this->x, this->y, this->width, this->height, rightStartY);
			int totalHeight = indexes[i]->MeasureTotalHeight();
			rightStartY = rightStartY + totalHeight + 10;
		}
		i++;
	}
	if (indexes != 0) {
		delete indexes;
	}
}
void Frame::Emphasize() {
	if (this->emphasize == false) {
		this->emphasize = true;
	}
	else {
		this->emphasize = false;
	}
}
int CompareShapes(void *one, void *other) {
	int ret = -1;
	if (*(static_cast<Shape**>(one)) == other) {
		ret = 0;
	}
	return ret;
}
//#include<iostream>
//int main(int argc, char *argv[]) {
//	Frame frame(10,20,30,40);
//	int index = frame.Add(new Frame(10,20,30,50));
//	cout << index << endl;
//	int totalHeight = frame.MeasureTotalHeight();
//	cout << totalHeight << endl;
//	return 0;
//}
