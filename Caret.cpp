//Caret.cpp
#include"Caret.h"

Caret::Caret() {
	this->x = 0;
	this->y = 0;
}
Caret::Caret(int x, int y) {
	this->x = x;
	this->y = y;
}
Caret::~Caret() {
}
void Caret::Move(int x, int y) {
	this->x = x;
	this->y = y;
}
