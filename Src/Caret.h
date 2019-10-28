//Caret.h
#ifndef _CARET_H
#define _CARET_H
using namespace std;

class Caret {
public:
	Caret();
	Caret(int x, int y);
	~Caret();
	int GetX()const;
	int GetY()const;
	void Move(int x, int y);

private:
	int x;
	int y;
};

inline int Caret::GetX()const {
	return this->x;
}
inline int Caret::GetY()const {
	return this->y;
}
#endif//_CARET_H
