//Stack.h
#ifndef _STACK_H
#define _STACK_H

typedef signed long int Long;
template <typename T>

class Stack {
public:
	Stack(int length = 10);
	~Stack();
	Long Push(T data); // 레벨 하나 생성
	T Pop(); // 레벨 하나 없애기
	bool IsEmpty();
	Long GetLength() const;
	Long GetCapacity()const;
	T GetAt(Long index)const;
private:
	T(*stack);
	Long capacity; //maxSize
	Long length; // top
};
template<typename T>
inline T Stack<T>::GetAt(Long index)const {
	return this->stack[index];
}

template <typename T>
inline Long Stack<T>::GetCapacity() const {
	return this->capacity;
}
template <typename T>
inline Long Stack<T>::GetLength() const {
	return this->length;
}
template<typename T>
Stack<T>::Stack(int capacity) :capacity(capacity),length(0) {
	this->stack = new T[capacity];
}
template<typename T>
Stack<T>::~Stack() {
	delete[]this->stack;
}

template<typename T>
Long Stack<T>::Push(T data) {
	Long index;
	if (length < capacity) {
		index = length;
		stack[index] = data;
		this->length++;
	}
	else {
		T(*temp) = new T[capacity];
		for (int i = 0; i < capacity-1; i++) {
			temp[i] = stack[i + 1];
			index = i+1;
		}
		temp[index] = data;
		if (this->stack != 0) {
			delete[] this->stack;
		}
		this->stack = temp;
	}
	return index;
}
template<typename T>
T Stack<T>::Pop() {
	return stack[--length]; 
}

template<typename T>
bool Stack<T>::IsEmpty() {
	return (length == 0);
	//참이면 1,거짓이면 0
}
#endif // _STACK_H