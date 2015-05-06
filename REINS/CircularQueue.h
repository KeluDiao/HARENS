#pragma once
#include "Definition.h"
using namespace std;

/*
We have 2 threads accessing an object of this class simultaneously
one for push, the other for pop
*/
template <class T>
class CircularQueue
{
public:
	T* queue;
	int front, rear;	//rear point to the last used entry, there's an empty entry after rear
	uint size;
	mutex contentMutex;
	condition_variable contentCond;

	CircularQueue() {
		size = TEST_MAX_KERNEL_INPUT_LEN;
		queue = new T[size];
		front = 0;
		rear = size - 1;
		//The full situation is front == (rear + 2) % size
	}

	CircularQueue(int _size) {
		size = _size;
		queue = new T[size];
		front = 0;
		rear = size - 1;
		//The full situation is front == (rear + 2) % size
	}

	/*CircularUintQueue& operator=(CircularUintQueue obj) {
		this->queue = obj.queue;
		this->front = obj.front;
		this->rear = obj.rear;
		this->size = obj.size;
		return *this;
	}*/

	void Push(T hashValue) {
		//Make sure that the queue is not full
		unique_lock<mutex> contentLock(contentMutex);
		if ((rear + 2) % size == front) {
			contentCond.wait(contentLock);
		}
		contentLock.unlock();

		rear = (rear + 1) % size;
		queue[rear] = hashValue;
		//notify pop that one entry is added into queue
		contentCond.notify_one();
	}

	T Pop() {
		//Make sure that the queue is not empty
		unique_lock<mutex> contentLock(contentMutex);
		if ((rear + 1) % size == front) {
			contentCond.wait(contentLock);
		}
		contentLock.unlock();

		T ret = queue[front];
		front = (front + 1) % size;
		contentCond.notify_one();
		return ret;
	}

	bool IsEmpty() {
		bool isEmpty;
		unique_lock<mutex> contentLock(contentMutex);
		isEmpty = ((rear + 1) % size == front);
		contentLock.unlock();
		contentCond.notify_one();
		return isEmpty;
	}

	~CircularQueue() {
		delete[] queue;
	}
};
