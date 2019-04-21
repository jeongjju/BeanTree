//MindMapIterator.h
#ifndef _MINDMAPITERATOR_H
#define _MINDMAPITERATOR_H
#include"Iterator.h"
#include"MindMap.h"
#include"Frame.h"
#include"Array.h"
class MindMapIterator :public Iterator<Array<Frame*>> {
public:
	MindMapIterator(MindMap *mindMap);
	virtual void First();
	virtual void Next();
	virtual bool IsDone() const;
	virtual Array<Frame*> Current();
	Long GetCurrent()const;
	void CopyMind(Frame *frame, int level);
private:
	Long current;
	Array<Array<Frame*>> mindMapLevel;
};
inline Long MindMapIterator::GetCurrent() const {
	return this->current;
}

#endif // _MINDMAPITERATOR_H
