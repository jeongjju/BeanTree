//FrameIterator.h
#ifndef _FRAMEITERATOR_H
#define _FRAMEITERATOR_H
#include "Iterator.h"
#include "Frame.h"
#include "Array.h"
class FrameIterator :public Iterator<Shape*> {
public:
	FrameIterator(Frame *frame);
	virtual void First();
	virtual void Next();
	virtual bool IsDone() const;
	virtual Shape* Current();
	Long GetCurrent()const;
private:
	Long current;
	Frame *pFrame;
};
inline Long FrameIterator::GetCurrent()const {
	return this->current;
}
#endif //_FRAMEITERATOR_H
