//Scroll.h
#ifndef _SCROLL_H
#define _SCROLL_H
#include<afxwin.h>

class Scroll {
public:
	Scroll();
	~Scroll();
	void Move(int x,int y);
	int GetX()const;
	int GetY()const;
private:
	int x;
	int y;
};

inline int Scroll::GetX()const {
	return this->x;
}
inline int Scroll::GetY()const {
	return this->y;
}


#endif//_SCROLL_H
