#include "Vector.h"

template <typename T> class MultiDimArray {
public:
	MultiDimArray(const VectorOf<int>& aDimns);
	MultiDimArray(int d1, int d2, int d3);
	int linearize(const int* coords) const;
	T get(const int* coords) const;
	T& get(const int* coords);
   T get(int i, int j, int k) const;
	T& get(int i, int j, int k);
	//	MultiDimArrayCursor operator [] (int i);

	// total number of Ts for a given set of dimensions
	static int total_size(const VectorOf<int>& aDimns);
private:
	void compute_multipliers();

	VectorOf<int> dimns;
	VectorOf<int> multipliers;
	VectorOf<T> pool;
};

/*
// This class will point to a sub array of MultiDimArray
template <typename T> class MultiDimArrayCursor {
public:
	MultiDimArrayCursor(const MultiDimArrayCursor& current, int next)
private:
	MultiDimArray& array;
	VectorOf<int> coords;
	int* next_dim;
	T* sub_array;
};
*/	
