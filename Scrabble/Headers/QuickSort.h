#pragma once

#include <vector>
#include <iostream>
#include <chrono>
#include <ratio>
#include <tuple>
#include <string>

using std::vector;
using std::tuple;
using std::string;


///<summary>
	///<para>
	///A class containing vector based quick sort
	///</para>
	///</summary>
class CQuickSort
{

public:


	///<summary>
	///<para>
	///Quick sort with vectors
	///</para>
	///</summary>
	template <class T>
	void Quicksort(vector<T> &vecSortable) {
		if (vecSortable.size() <= 1){
			return;
		}

		int iPivotIndex;

		if (vecSortable.size() >= 3){
			iPivotIndex = this->GetPivotIndex(vecSortable.size() - 1, vecSortable);
		}

		else{
			iPivotIndex = vecSortable.size() - 1;
		}

		std::unique_ptr<vector<T>> pvecLower(new vector<T>);
		std::unique_ptr<vector<T>> pvecHigher(new vector<T>);

		T pivotValue = vecSortable.at(iPivotIndex);

		bool bHasSkippedPivot = false;

		for (typename vector<T>::iterator it = vecSortable.begin(); it != vecSortable.end(); ++it)
		{
			T value = *it;

			if (value == pivotValue && !bHasSkippedPivot){
				bHasSkippedPivot = true;
			}

			else if (value == pivotValue) {
				pvecLower->push_back(value);
			}

			else if (value < pivotValue){
				pvecLower->push_back(value);
			}

			else{
				pvecHigher->push_back(value);
			}
		}

		vecSortable.clear();

		if (pvecLower->size()>0){
			this->Quicksort(*pvecLower);
			vecSortable.insert(vecSortable.end(), pvecLower->begin(), pvecLower->end());
		}

		vecSortable.insert(vecSortable.end(), pivotValue);

		if (pvecHigher->size() > 0){
			this->Quicksort(*pvecHigher);
			vecSortable.insert(vecSortable.end(), pvecHigher->begin(), pvecHigher->end());
		}
	}

private:

	///<summary>
	///<para>
	///Get pivot point for vector array
	///</para>
	///</summary>
	template <class T>
	int GetPivotIndex(int lastIndex, vector<T>& vectorToSort) {
		tuple<int, T> valuePairs[3];

		srand((unsigned int)std::chrono::system_clock::now().time_since_epoch().count()); //create a unique seed

		for (size_t i = 0; i < 3; i++)
		{
			int randomIndex = rand() % lastIndex;
			valuePairs[i] = { randomIndex, vectorToSort.at(randomIndex) };
		}

		srand(1); //set back to default seed
		int medianIndex = this->GetMedian(3, valuePairs);
		return medianIndex;
	}

	///<summary>
	///<para>
	///Get the moda of number array
	///</para>
	///</summary>
	template <class T>
	int GetMedian(int size, tuple<int, T>valuePairs[]) {
		//sort out the numbers 
	//can't use quicksort as it would create infinite recursion
	//insert sort
		for (int i = 0; i < size; i++)
		{
			tuple<int, T> key = valuePairs[i];

			int j = i;

			while (j > 0 && std::get<1>(valuePairs[j - 1]) >std::get<1>( key))
			{
				valuePairs[j] = valuePairs[j - 1];
				j--;
			}

			valuePairs[j] = key;
		}

		int numberToReturn = std::get<0>(valuePairs[size / 2]); //return the middle value
		return numberToReturn;
	}

};

