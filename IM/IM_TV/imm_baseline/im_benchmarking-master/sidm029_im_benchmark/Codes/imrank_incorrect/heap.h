#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <iostream>

using namespace std;

typedef struct _HeapNode
{
	int key;
	double value;
} HeapNode;

typedef struct _Heap
{
	int count;
	int* index;
	HeapNode * elements; 
} Heap;

void initHeap(Heap *h, int);
void freeHeap(Heap *h);
void insertHeap(Heap *, HeapNode);
void heapifyHeap(Heap *, int);
void removeMinHeap(Heap *);
void debugHeap(Heap *);
void decreaseKeyHeap(Heap *, int, double);

#endif