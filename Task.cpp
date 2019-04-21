//Task.cpp
#include "Task.h"
#include"MindMap.h"
#define Max 10

Task::Task()
	:unDo(Max),reDo(Max),unDoLength(0),unDoCapacity(Max),reDoLength(0),reDoCapacity(Max)
{
}

Task::~Task()
{
}

int Task::UnDoWrite(MindMap *mindMap)
{
	MindMap *unDoMindMap = new MindMap(*mindMap);
	unDo.Push(unDoMindMap);
	return this->unDoLength++;
}

int Task::ReDoWrite(MindMap * mindMap)
{
	MindMap *reDoMindMap = new MindMap(*mindMap);
	reDo.Push(reDoMindMap);
	return this->reDoLength++;
}

MindMap* Task::UnDoRead()
{
	MindMap *mindMap=unDo.Pop();
	this->unDoLength--;
	return mindMap;
}

MindMap * Task::ReDoRead()
{
	MindMap *mindMap = reDo.Pop();
	this->reDoLength--;
	return mindMap;
}

//#include"TopicBox.h"
//#include<iostream>
//int main(int argc, char *argv[]) {
//	
//	MindMap *mindMap = new MindMap;
//	mindMap->InsertCenterFrame(10, 10, 10, 10);
//	MindMap *mindMap2 = new MindMap;
//	MindMap *mindMap3 = new MindMap;
//	
//	Task task;
//	int index = task.UnDoWrite(mindMap);
//	cout << index << endl;
//	cout << task.GetReDoLength() << endl;
//	MindMap *mindMap4 = task.UnDoRead();
//	cout << mindMap4->GetCenterFrame()->GetX() << endl;
//
//
//
//
//	return 0;
//}


