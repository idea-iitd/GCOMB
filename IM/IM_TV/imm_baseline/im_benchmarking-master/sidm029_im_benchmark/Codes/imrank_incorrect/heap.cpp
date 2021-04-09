#include "heap.h"
#include <float.h>
/**
 * The heap is a min-heap sorted by Key.
 */

void debugHeap(Heap *h)
{
	cout << "heap size = " << '\t' << h->count << endl;

	for(int i = 1; i <= h->count; i++)
		cout << h->elements[i].key << '\t' << h->elements[i].value << endl;

	cout << endl;

	for(int i = 1; i <= h->count; i++)
		cout << h->elements[i].key << '\t'  << h->index[h->elements[i].key] << endl;

}

void initHeap(Heap *h, int size)
{
	h->count = 0;
	h->elements = (HeapNode *) malloc(size * sizeof(HeapNode));
	h->index = (int *) malloc(size * sizeof(int));
}

void freeHeap(Heap *h)
{
	h->count = 0;
	free(h->index);
	free(h->elements);
}

void decreaseKeyHeap(Heap *h, int index, double value)
{
	int parentIndex, curIndex, curKey, minIndex;
	double parentValue, curValue, minValue;
	bool satisfyHeap = false;

	//cout << "decreaseKey phase" << endl;
	//debugHeap(h);

	curIndex = index;
	curValue = value;
	curKey = h->elements[index].key;
	
	while(!satisfyHeap)
	{
		//cout << "curIndex = " << curIndex << endl;

		parentIndex = curIndex / 2;
		
		if(parentIndex >= 1) 
			parentValue = h->elements[parentIndex].value;
		else break;

		minValue = curValue;
		minIndex = curIndex;

		if(minValue >= parentValue)
		{
			minValue = parentValue;
			minIndex = parentIndex;
		}

		if(minIndex == parentIndex) satisfyHeap = true;
		else 
		{
			h->elements[curIndex] = h->elements[parentIndex];
			h->index[h->elements[parentIndex].key] = curIndex;
			curIndex = parentIndex;
		}
	}
	h->elements[curIndex].key = curKey;
	h->elements[curIndex].value = curValue;
	h->index[curKey] = curIndex;	

	//debugHeap(h);
	//cin.get();

}

void insertHeap(Heap *h, HeapNode hNode)
{
	h->elements[++h->count] = hNode;
	h->index[hNode.key] = h->count;
	decreaseKeyHeap(h, h->count, hNode.value);
}

void heapifyHeap(Heap *h, int index)
{
	int leftIndex, rightIndex, curIndex, minIndex, curKey;
	double leftValue, rightValue, curValue, minValue;
	bool satisfyHeap = false;
	curIndex = index;
	curValue = h->elements[index].value;
	curKey = h->elements[index].key;
	
	while(!satisfyHeap)
	{
		//cout << "curIndex = " << curIndex << endl;

		leftIndex = curIndex * 2;
		rightIndex = curIndex * 2 + 1;

		if(leftIndex <= h->count) 
			leftValue = h->elements[leftIndex].value;
		else leftValue = DBL_MAX;

		if(rightIndex <= h->count)
			rightValue = h->elements[rightIndex].value;
		else rightValue = DBL_MAX;

		minValue = curValue;
		minIndex = curIndex;

		if(minValue > leftValue)
		{
			minValue = leftValue;
			minIndex = leftIndex;
		}

		if(minValue > rightValue)
		{
			minValue = rightValue;
			minIndex = rightIndex;
		}

		if(minIndex == curIndex) satisfyHeap = true;
		else 
		{
			h->elements[curIndex] = h->elements[minIndex];
			h->index[h->elements[minIndex].key] = curIndex;			
			curIndex = minIndex;
		}

		//debugHeap(h);
		//cin.get();
	}
	h->elements[curIndex].key = curKey;
	h->elements[curIndex].value = curValue;
	h->index[curKey] = curIndex;	
}

void removeMinHeap(Heap *h)
{
	//cout << "remove phase" << endl;
	h->elements[1] = h->elements[h->count--];
	h->index[h->elements[1].key] = 1;
	heapifyHeap(h, 1);
	//debugHeap(h);
	//	cout << endl <<  h->elements[1].key << '\t' << h->elements[1].value << endl << endl;
		//cin.get();
}