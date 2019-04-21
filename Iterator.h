#ifndef _ITERATOR_H
#define _ITERATOR_H
template <typename T>
class Iterator {
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() const = 0;
	virtual T Current()=0;
};
#endif // _ITERATOR_H

