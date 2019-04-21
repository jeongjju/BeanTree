//MindMap.h
#ifndef _MINDMAP_H
#define _MINDMAP_H
#include"Frame.h"
#include<string>
#pragma warning(disable:4996)
using namespace std;
class MindMap {
public:
	MindMap();
	~MindMap();
	MindMap(const MindMap& source);
	MindMap& operator=(const MindMap& source);
	Frame* InsertCenterFrame(int x, int y, int width, int height);
	Frame* InsertCenterFrame(int x, int y, int width, int height, bool emphasize);
	Frame* GetCenterFrame()const;
	Iterator<Array<Frame*>>* CreateIterator();
	Frame* HighRankFrame(Frame* frame);
	int Save();
	int SaveAs(string filePath);
	void Node_Save(FILE *file,Frame *frame,int *frameOrder);
	int Load(string filePath);
	void Node_Load(FILE *file, Array<Frame*> *indexes,int *index);
private:
	Frame *centerFrame;
};
inline Frame* MindMap::GetCenterFrame()const {
	return this->centerFrame;
}
#endif//_MINDMAP_H
