#ifndef BASE_STACK_H
#define BASE_STACK_H

#include <netdb.h>
#include <assert.h>
#include "Superblock.h"

namespace hoard {

template<typename T>
class BaseStack {

public:
	BaseStack() : head_(nullptr) {
	}

	void Push(T *t) {
		SetNext(t, head_);
		SetPrev(t, nullptr);
		head_ = t;
	}

	T *Pop() {
		assert(!IsEmpty());
		T *t = head_;
		head_ = Next(head_);
		SetNext(t, nullptr);
		return t;
	}

	bool IsEmpty() {
		return head_ == nullptr;
	}

protected:
	virtual void SetNext(T *t, T *value) = 0;

	virtual void SetPrev(T *t, T *value) = 0;

	virtual T *Next(T *t) = 0;

private:
	T *head_;
};

}

#endif // BASE_STACK_H