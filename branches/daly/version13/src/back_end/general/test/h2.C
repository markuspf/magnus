// Contents: The version of merge sort which runs insertion sort
//           when the number of elements to be sorted is <= k.
//           Time measurements are taken for different values of k.
//
// Principal Author: Dmitry Bormotov
//
// Status: Completed
//
// Revision History:
//

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fstream.h>
#include "global.h"
#include "RandomNumbers.h"

void swap( int* A, int i, int j );
int findLargest(int* A, int heapSize, int d, int i);
void printArray(int* A, int ASize);


// Definitions:
//
//   A[0 .. ASize-1] - array of integers, represents a heap
//   d - arity of the heap ( d >= 2 )
//   heapSize - size of the heap, always <= ASize 
//   Subtree(i) - subtree rooted at node i
//   Children(i) = { x: x belongs to the intersection of
//   {d*i+1,...,d*i+d} and {x < heapSize}  


void heapify( int* A, int heapSize, int d, int a )
// pre:  A[0 .. heapSize-1], 0 <= a < heapSize 
//       For all x, x is in Children(a) -> Subtree(x) is a heap
//
// post: Subtree(a) is a heap
{
  int i = a;
  int largest = findLargest(A,heapSize,d,i);

  // * Invariant: 
  // All nodes of Children(a) except i satisfy the heap property.
  // Node i may or may not satisfy the heap property.
  // largest is the node of the largest value among A[i] 
  // and values of children of i.

  while( largest != i ) {
    swap(A,i,largest);
    i = largest;
    largest = findLargest(A,heapSize,d,i);
  }
}

void swap( int* A, int i, int j )
// swaps values of A[i] and A[j] 
{
 int r = A[i]; 
 A[i] = A[j]; 
 A[j] = r;
}

int findLargest(int* A, int heapSize, int d, int i)
// finds the largest value among A[i] and values of children of i
// and returns its node in the heap
{
  int maxInd = i;
  for( int j = d*i+1; j <= d*i+d; ++j )
    if( j < heapSize && A[maxInd] < A[j] ) 
      maxInd = j;
  return maxInd;
}

void buildHeap( int* A, int ASize, int d, int& heapSize )
{
  heapSize = ASize;
  for( int i = ASize - 1; i >= 0; --i )
    heapify(A,heapSize,d,i);
}

void heapSort(int* A, int ASize, int d)
{
  int heapSize;
  buildHeap(A,ASize,d,heapSize);
  for( int i = ASize-1; i >= 1; --i ) {
    swap(A,0,i);
    --heapSize;
    heapify(A,heapSize,d,0);
  }
}

void printArray(int* A, int ASize)
{
  for( int i = 0; i < ASize; ++i )
    cout << " " << A[i];
  cout << endl;
}

double diffTime(timeval t2, timeval t1)
// computes the time diffirence between t2 and t1 
{
  double result;
  long s = t2.tv_sec - t1.tv_sec;
  result = (t2.tv_usec - t1.tv_usec) * 0.000001 + s;
  //if( t2.tv_usec < t1.tv_usec ) --result;
  return result;
}

int extractMax( int* A, int& heapSize, int d )
{
  if( heapSize < 1 )
    error("int extractMax( int* A, int heapSize, int d ) : "
	  "heap underflow");
  int max = A[0];
  A[0] = A[heapSize-1];
  --heapSize;
  heapify(A,heapSize,d,0);
  return max;
}

int main ( )
{
  timeval t1,t2;
  int maxSize = 1000000;
  int A[maxSize];
  const int ASize = 1000000;
  UniformRandom r;
  int heapSize;

  for( int d = 2; d <= 20; ++d ) {
    
    for( int i = 0; i < ASize; ++i )
      A[i] = ASize-i-1;
    //A[i] = int(r.rand() * ASize);

    buildHeap(A,ASize,d,heapSize);
    
    gettimeofday(&t1, NULL);

    for( int i = 0; i < 100000; ++i )
      extractMax(A,heapSize,d);

    gettimeofday(&t2, NULL);

    double dif = diffTime(t2,t1);
    cout << "d = " << d << "   time elapsed: " << dif << endl;
  }
}

