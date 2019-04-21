//MindMapIterator.h
#include"MindMapIterator.h"
MindMapIterator::MindMapIterator(MindMap *mindMap) :current(0){
	CopyMind(mindMap->GetCenterFrame(), 1);
}
void MindMapIterator::CopyMind(Frame *frame, int level) {
	int i = 0;
	if (mindMapLevel.GetLength() < level) {
		Array<Frame*>temp(1000);
		if (mindMapLevel.GetLength() < mindMapLevel.GetCapacity()) {
			mindMapLevel.Store(level - 1, temp);
		}
		else {
			mindMapLevel.AppendFromRear(temp);
		}
	}
	if (mindMapLevel.GetAt(level - 1).GetCapacity() >= mindMapLevel.GetAt(level - 1).GetLength()) {
		mindMapLevel.GetAt(level - 1).Store(mindMapLevel.GetAt(level - 1).GetLength(), frame);
	}
	else {
		mindMapLevel.GetAt(level - 1).AppendFromRear(frame);
	}
	while (i<frame->GetLength())
	{
		if (dynamic_cast<Frame*>(frame->GetChild(i))) {
			CopyMind((Frame*)frame->GetChild(i), level + 1);
		}
		i++;
	}
}
void MindMapIterator::First() {
	this->current = 0;
}
void MindMapIterator::Next() {
	this->current++;
}
bool MindMapIterator::IsDone() const {
	bool ret = false;
	if (this->current >= mindMapLevel.GetLength()) {
		ret = true;
	}
	return ret;
}
Array<Frame*> MindMapIterator::Current() {
	return this->mindMapLevel.GetAt(current);
}