#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

void shiftDown(vector<int> &heap, int i) 
{
	int curr = i;
	while (curr*2+1 < heap.size()) {
		int min = curr;
		if (heap[min] > heap[curr*2+1]) {
			min = curr*2 + 1;
		}
		if (curr*2+2 < heap.size() && heap[min]>heap[curr*2+2]) {
			min = curr*2 + 2;
		}
		if (min == curr) {
			break;
		}
		swap(heap[curr], heap[min]);
		curr = min;
	}
}

void shiftUp(vector<int> &heap, int i) 
{
	int curr = i;
//	while ((curr-1)/2 >= 0) {
	while (curr > 0) {
		if (heap[curr] < heap[(curr-1)/2]) {
			swap(heap[curr], heap[(curr-1)/2]);
			curr = (curr-1) / 2;
		}
		else {
			break;
		}
	}
}

void makeHeap(vector<int> &heap)
{
	int first = heap.size()/2 - 1;
	for (int i=first; i>=0; --i) {
		shiftDown(heap, i);
	}
}

int pop(vector<int> &heap)
{
	int x = heap[0];
	heap[0] = heap[heap.size()-1];
	heap.pop_back();
	shiftDown(heap, 0);
	return x;
}


int main() 
{
	vector<int> heap = {10, 12, 7, 15, 5, 6, 4, 1, 3, 13, 8};
	for_each(heap.begin(), heap.end(), [](int x){cout << x << " ";});
	cout << endl;
	makeHeap(heap);
	for_each(heap.begin(), heap.end(), [](int x){cout << x << " ";});
	cout << endl;
	heap.push_back(3);
	shiftUp(heap, heap.size()-1);
	for_each(heap.begin(), heap.end(), [](int x){cout << x << " ";});
	cout << endl;
	while (!heap.empty()) {
		cout << pop(heap) << " ";
	}
	cout << endl;
	return 0;
}
