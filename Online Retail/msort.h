#ifndef MSORT_H
#define MSORT_H

#include <iostream>
#include <vector>
using namespace std;

template <class T, class Comparator>
class mSort {
	public:
		void mergeSort(vector<T>& myArray, Comparator comp);
		void merge(vector<T>& first, vector<T>& second, vector<T>& current, Comparator comp);
};

template <class T, class Comparator>
void merge(vector<T>& first, vector<T>& second, vector<T>& current, Comparator comp) {
	int indexA, indexB;
	indexA = 0;
	indexB = 0;
	current.clear();
	current.reserve(first.size() + second.size());
	while (indexA < first.size() && indexB < second.size() ) {
		if (comp(first[indexA], second[indexB]) == 1) {
			current.push_back(first[indexA]);
			indexA++;
		}
		else {
			current.push_back(second[indexB]);
			indexB++;
		}
	}
	while (indexA < first.size()) {
		current.push_back(first[indexA]);
		indexA++;
	}
	while (indexB < second.size()) {
		current.push_back(second[indexB]);
		indexB++;
	}
}

template <class T, class Comparator>
void mergeSort(vector<T>& myArray, Comparator comp) {
	if (myArray.size() < 2)
		return;
	vector<T> first, second;
	for (unsigned int i = 0; i < myArray.size() / 2; ++i)
	{
		first.push_back(myArray[i]);
	}
	for (unsigned int i = myArray.size() / 2; i < myArray.size(); ++i)
	{
		second.push_back(myArray[i]);
	}
	mergeSort(first, comp);
	mergeSort(second, comp);
	merge(first, second, myArray, comp);
}
#endif 
