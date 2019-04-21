//FrameIterator.cpp
#include "FrameIterator.h"

FrameIterator::FrameIterator(Frame *frame)
	:pFrame(frame), current(0){
}
void FrameIterator::First() {
	this->current = 0;
}
void FrameIterator::Next() {
	this->current++;
}
bool FrameIterator::IsDone() const {
	bool ret = false;
	if (this->current >= pFrame->GetLength()) {
		ret = true;
	}
	return ret;
}
Shape* FrameIterator::Current()  {
	return pFrame->GetChild(this->current);
}
