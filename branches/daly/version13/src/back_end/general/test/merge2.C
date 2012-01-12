// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <fstream.h>
#include "global.h"

int Y[10000];

void insertionSort(int* x, int a, int b);

double diffTime(timeval t2, timeval t1)
{
  double result;
  long s = t2.tv_sec - t1.tv_sec;
  result = (t2.tv_usec - t1.tv_usec) * 0.000001 + s;
  //if( t2.tv_usec < t1.tv_usec ) --result;
  return result;
}

void merge (int ax, int bx, int ay, int by, int az, int bz,
              const int X[], const int Y[], int Z[])
    {
    	int kx, ky, kz;
    	
    	kx = ax; ky = ay; kz = az;

	/* invariant indication: 
         *   Z[az..kz-1] is the ordered merge of X[ax..kx-1] and Y[ay..ky-1]
         *   and
         *   az <= kz <= bz + 1
         *   and
         *   ax <= kx <= bx + 1
         *   and
         *   ay <= ky <= by + 1
         */
    	
    	while (kz <= bz)
    	{
    		if (ky > by)
    		  {Z[kz] = X[kx]; kx++; kz++;}
    		  
    		else if (kx > bx)
    		  {Z[kz] = Y[ky]; ky++; kz++;}
    		  
    		else if (X[kx] <= Y[ky])
    		  {Z[kz] = X[kx]; kx++; kz++;}
    		  
    		else
    		  {Z[kz] = Y[ky]; ky++; kz++;}
    	}

	/*  kz > bz, hence kz = bz + 1.  One readily argues from this and the
	 *  invariant that Z[az..bz] is now the ordered merge of X[ax..bx] and
         *  Y[ay..by].  
         */
    	
    }
    
    


void mergesort2 (int X[], int a, int b, int K)
/* pre: X[a..b] is a non-empty array segment, K >= 2 */  
{
  int gap = b - a + 1, k, m;

  if( gap <= K )
    insertionSort(X,a,b);
  
  else { // a < b
  
    //int* Y = malloc( gap * sizeof(int)  );  
	
    m = (a + b - 1) / 2;
    mergesort2(X, a, m, K); mergesort2(X, m + 1, b, K);
    merge(a, m, m + 1, b, 0, b - a, X, X, Y);
    k = a - 1;
	    
    /* invariant:  X[a..k] == Y[0..k-a] && k <= b */
    while (k < b) { 
      k = k + 1;
      X[k] = Y[k - a];
    }
    /* X[a..b] is sorted, and has values and multiplicities
     * unchanged from X[a..b] as input
     */
    
    //free(Y);
  }
} 

void insert( int* y, int a, int j )
{
  int i, key = y[j];
  i = j - 1;

  while( (i >= a) && (y[i] > key) ) {
    y[i+1] = y[i];
    --i;
  }
  y[i+1] = key;
}

void insertionSort(int* x, int a, int b)
{
  int j = a+1;
  while( j <= b ) {
    insert(x,a,j);
    ++j;
  }
} 

int main ( )
{
  timeval t1,t2;
  int maxSize = 10000;
  int x[maxSize];
  int n = 1024;
 
  for( int k = 2; k < n; k *= 2 ) {
    
    for( int i = 0; i < n; ++i )
      x[i] = n-i-1;
    int a = 0, b = n - 1;

    gettimeofday(&t1, NULL);
  
    mergesort2(x,a,b,k);
    
    gettimeofday(&t2, NULL);

    double dif = diffTime(t2,t1);
    cout << "k = " << k << "   time: " << dif << endl;
  }
}

