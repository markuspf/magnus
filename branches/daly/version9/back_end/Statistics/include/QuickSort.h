// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of OneVariableDataSet class
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#ifndef _QUICKSORT_H_
#define _QUICKSORT_H_

#include "DArray.h"

template <class R> 
void swap(R& x,R& y);

template <class R>
void QuickSort(DArray<R>& A,int first,int last);

#endif
