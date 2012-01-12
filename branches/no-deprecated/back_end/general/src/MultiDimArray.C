#include "MultiDimArray.h"
#include "Word.h"

template <typename T>
MultiDimArray<T>::MultiDimArray(const VectorOf<int>& aDimns):
	pool(total_size(aDimns)), dimns(aDimns), multipliers(aDimns.length())
{
	compute_multipliers();
}

template <typename T>
void MultiDimArray<T>::compute_multipliers() {
   int lastindex = dimns.length()-1;
   multipliers[lastindex] = 1;
   for(int i = lastindex; i > 0; i--)
      multipliers[i-1] = dimns[i]*multipliers[i];
}

template <typename T>
int MultiDimArray<T>::total_size(const VectorOf<int>& aDimns) {
	int total(1);
	for(int i = 0; i < aDimns.length(); i++)
		total *= aDimns[i];
	return total;
}

template <typename T>
MultiDimArray<T>::MultiDimArray(int d1, int d2, int d3):
   pool(d1*d2*d3), dimns(3), multipliers(3)
{
	dimns[0] = d1;
	dimns[1] = d2;
	dimns[2] = d3;
	compute_multipliers();
}

template <typename T>
int MultiDimArray<T>::linearize(const int* coords) const {
	int result(0);
	for(int i = 0; i < dimns.length(); i++)
		result += coords[i]*multipliers[i];
	return result;
}

template <typename T>
T MultiDimArray<T>::get(const int* coords) const {
	return pool[linearize(coords)];
}

template <typename T>
T& MultiDimArray<T>::get(const int* coords) {
   return pool[linearize(coords)];
}

template <typename T>
T MultiDimArray<T>::get(int i, int j, int k) const {
	int coords[]={i,j,k};
   return pool[linearize(coords)];
}

template <typename T>
T& MultiDimArray<T>::get(int i, int j, int k) {
	int coords[]={i,j,k};
   return pool[linearize(coords)];
}

template class MultiDimArray<Word>;
