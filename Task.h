//Task.h
#ifndef _TASK_H
#define _TASK_H
#include"Stack.h"
class MindMap;
class Task{
public:
	Task();
	~Task();
	int UnDoWrite(MindMap *mindMap);
	int ReDoWrite(MindMap *mindMap);
	MindMap* UnDoRead();
	MindMap* ReDoRead();

	int GetUnDoLength()const;
	int GetUnDoCapacity()const;

	int GetReDoLength()const;
	int GetReDoCapacity()const;

private:
	Stack<MindMap*> unDo;
	int unDoLength;
	int unDoCapacity;

	Stack<MindMap*> reDo;
	int reDoLength;
	int reDoCapacity;

};

inline int Task::GetUnDoLength()const {
	return this->unDoLength;
}
inline int Task::GetUnDoCapacity()const {
	return this->unDoCapacity;
}
inline int Task::GetReDoLength()const {
	return this->reDoLength;
}
inline int Task::GetReDoCapacity()const {
	return this->reDoCapacity;
}
#endif//_TASK_H
