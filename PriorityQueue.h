#pragma once
#include <vector>
#include <utility>
#include <tuple>

using namespace std;

//DO NOT CHANGE THIS PART
typedef int TElem;
typedef int TPriority;
typedef std::pair<TElem, TPriority> Element;
typedef bool (*Relation)(TPriority p1, TPriority p2);
#define NULL_TELEM pair<TElem, TPriority> (-11111,-11111)

class PriorityQueue {
private:
	tuple<int,Element ,int> *arr;  //left,info,prioritate,right
    int root;
    int firstEmpty;
    int cap;
    int nrElems;
    Relation rel;

public:
	//implicit constructor
	PriorityQueue(Relation r);

    PriorityQueue(const PriorityQueue& pq);

    bool emptyL(int pos) const;

    bool emptyR(int pos) const;

    bool leaf(int pos) const;

	//pushes an element with priority to the queue
	void push(TElem e, TPriority p);

	//returns the element with the highest priority with respect to the order relation
	//throws exception if the queue is empty
	Element top()  const;

	//removes and returns the element with the highest priority
	//throws exception if the queue is empty
	Element pop();

	//checks if the queue is empty
	bool isEmpty() const;

	//destructor
	~PriorityQueue();

    void afis(PriorityQueue pq1);

};