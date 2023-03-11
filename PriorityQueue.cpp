
#include "PriorityQueue.h"
#include <exception>
#include <iostream>

using namespace std;


PriorityQueue::PriorityQueue(Relation r) {
    this->cap = 5;
    this->arr = new tuple<int, Element, int>[cap];
    this->root = -1;
    this->firstEmpty = 0;
    this->nrElems = 0;
    this->rel = r;

    for (int i = 0; i < cap; i++)
        arr[i] = make_tuple(i + 1, NULL_TELEM, -1);
    arr[cap - 1] = make_tuple(-1, NULL_TELEM, -1);

}

PriorityQueue::PriorityQueue(const PriorityQueue &pq) {

    this->cap = pq.cap;
    this->root = pq.root;
    this->firstEmpty = pq.firstEmpty;
    this->nrElems = pq.nrElems;
    this->rel = pq.rel;
    this->arr = new tuple<int,Element,int>[this->cap];

    for(int i=0; i<cap; i++)
        arr[i] = pq.arr[i];

}

//best 0(1) worst 0(1) avg 0(1)
bool PriorityQueue::emptyL(int pos) const {
    if (get<0>(arr[pos]) == -1)
        return true;
    else
        return false;
}

//best 0(1) worst 0(1) avg 0(1)
bool PriorityQueue::emptyR(int pos) const {

    if (get<2>(arr[pos]) == -1)
        return true;
    else
        return false;
}

//best 0(1) worst 0(1) avg 0(1)
bool PriorityQueue::leaf(int pos) const{

    if (emptyR(pos) && emptyL(pos) )
        return true;
    else
        return false;
}

//best 0(1) worst 0(n) avg 0(log n)(amortizat)
void PriorityQueue::push(TElem e, TPriority p) {

    if (nrElems == cap) {
        cap *= 2;
        tuple<int, Element, int> *temp;
        temp = new tuple<int, Element, int>[cap];
        for (int i = 0; i < cap; i++)
            temp[i] = make_tuple(i + 1, NULL_TELEM, -1);   //resize
        temp[cap - 1] = make_tuple(-1, NULL_TELEM, -1);

        for (int i = 0; i < nrElems; i++)
            temp[i] = arr[i];

        firstEmpty = cap/2;
        delete[] arr;
        arr = temp;
    }

    if (root == -1) {
        root = firstEmpty;
        firstEmpty = get<0>(arr[firstEmpty]);
        get<1>(arr[root]) = make_pair(e, p);   //daca punem radacina
        get<0>(arr[root]) = -1;
        get<2>(arr[root]) = -1;
    } else {
        int crnt = root;
        while (true) {
            if (rel(p,get<1>(arr[crnt]).second)){
                if(!emptyL(crnt)) {
                    crnt = get<0>(arr[crnt]);
                }else if(emptyL(crnt)){
                    int nw=firstEmpty;
                    firstEmpty = get<0>(arr[nw]);
                    get<1>(arr[nw]) = make_pair(e, p);  //daca punem in stanga
                    get<0>(arr[nw]) = -1;
                    get<2>(arr[nw]) = -1;
                    get<0>(arr[crnt])=nw;
                    break;
                }
            } else{
                if(!emptyR(crnt)) {
                    crnt = get<2>(arr[crnt]);
                }else if(emptyR(crnt)){
                    int nw=firstEmpty;
                    firstEmpty = get<0>(arr[nw]);
                    get<1>(arr[nw]) = make_pair(e, p);  //daca punem in dreapta
                    get<0>(arr[nw]) = -1;
                    get<2>(arr[nw]) = -1;
                    get<2>(arr[crnt])=nw;
                    break;
                }
            }
        }
    }

    nrElems++;
}

//throws exception if the queue is empty
//best 0(1) worst 0(n) avg 0(log n)(amortizat)
Element PriorityQueue::top() const {
    if (isEmpty())
        throw exception();
    else {
        int crnt = root;
        while (get<0>(arr[crnt]) != -1)
            crnt = get<0>(arr[crnt]);

        return get<1>(arr[crnt]);
    }
}

//best 0(1) worst 0(n) avg 0(log n)(amortizat)
Element PriorityQueue::pop() {
    if (isEmpty()) throw exception();
    else {
        int parent = root;
        int crnt = root;
        while (get<0>(arr[crnt])!=-1) {
                parent = crnt;
                crnt = get<0>(arr[crnt]);
            }
        Element el = get<1>(arr[crnt]);      //el cu prioritatea cea mai mare
        if (crnt == root) {

            if (get<2>(arr[crnt])!=-1) {

                root = get<2>(arr[crnt]);
                arr[crnt] = make_tuple(firstEmpty,NULL_TELEM,-1);  //in cazul in care exista cv in dreapta radacinii
                firstEmpty = crnt;
                nrElems--;
                return el;
            } else {

                arr[crnt] = make_tuple(firstEmpty,NULL_TELEM,-1);
                firstEmpty = crnt;    //in cazul in care a ramas doar radacina
                nrElems--;
                return el;

            }

        } else {

            if (leaf(crnt)) {

                get<0>(arr[parent]) = -1;
                arr[crnt] = make_tuple(firstEmpty,NULL_TELEM,-1);  //daca elementul este o frunza
                firstEmpty = crnt;
                nrElems--;
                return el;

            } else {

                get<0>(arr[parent]) = get<2>(arr[crnt]);
                arr[crnt] = make_tuple(firstEmpty,NULL_TELEM, -1);  //daca el care trebuie sters mai are cv in dreapta
                firstEmpty = crnt;
                nrElems--;
                return el;

            }

        }
    }
}

//best 0(1) worst 0(1) avg 0(1)
bool PriorityQueue::isEmpty() const {
    if (this->nrElems == 0)
        return true;
    return false;
}

//best 0(1) worst 0(1) avg 0(1)
PriorityQueue::~PriorityQueue() {
    delete[] arr;
};

//best 0(n) worst 0(n) avg 0(n)
void PriorityQueue::afis(PriorityQueue pq1){
    Relation rel1=pq1.rel;
    PriorityQueue *pq2= new PriorityQueue(rel1);
    TPriority ar[]={};
    for(int i=0;i<pq1.nrElems;i++){
        ar[i]=INT_MIN;
    }

    int i=0;
    while(!pq1.isEmpty()){
        Element el=pq1.pop();
        ar[i]=el.second;
        i++;
        pq2->push(el.first,el.second);
    }

    while (!pq2->isEmpty()){
        Element el2=pq2->pop();
        pq1.push(el2.first,el2.second);
    }

    for(int j=pq1.nrElems;j>=1;j--)
        cout<<ar[i]<<" ";

}