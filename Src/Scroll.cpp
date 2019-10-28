//Scroll.cpp
#include "stdafx.h"
#include "Scroll.h"

Scroll::Scroll() {
	this->x = 0;
	this->y = 0;
}

void Scroll::Move(int x,int y) {
	this->x = x;
	this->y = y;
}
Scroll::~Scroll() {
}

