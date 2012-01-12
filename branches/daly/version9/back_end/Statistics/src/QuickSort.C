// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of OneVariableDataSet class
//
// Principal Author: Dmitry Bormotov,Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//
//

#include "QuickSort.h"

template <class R> 
void swap(R& x,R& y){
   R temp=x;
   x=y;
   y=temp;
}

template <class R>
void QuickSort(DArray<R>& A,int first,int last){
   int left,right;
   left=first;
   right=last;
   if(left>=right)
       return;
   int vetex=left;   
   while(left<right){
      while(A[vetex][0]>=A[left][0]&&left<last) {left++;}
      while(A[vetex][0]<A[right][0]&&right>first) {right--;}
      ///swap left and right
      if(left<right)
        swap(A[left][0],A[right][0]);
   }
   ////swap the first and mid
   swap(A[vetex][0],A[right][0]);
   QuickSort(A,first,right-1);
   QuickSort(A,right+1,last);
}

void xxwtest(){
  DArray<double> A(5,1);
  QuickSort(A,0,4);  
}

