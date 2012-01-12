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

double diffTime(timeval t2, timeval t1)
{
  double result;
  long s = t2.tv_sec - t1.tv_sec;
  result = (t2.tv_usec - t1.tv_usec) * 0.000001 + s;
  //if( t2.tv_usec < t1.tv_usec ) --result;
  return result;
}

bool binSearch( int* x, int a, int b, int y, int& j )
{
  int c = a, d = b;
  bool found = false;

  while( !found && c <= d ) {
    j = (c+d)/2;
    if( x[j] == y ) found = true;
    else if( x[j] < y ) c = j+1;
    else d = j-1;
  }
  return found;
}

void insert( int* y, int a, int b, int j )
{
  int k, key = y[j];

  binSearch(y,a,j-1,y[j],k);
  
  if( y[k] < y[j] ) ++k;
  
  for( int i = j; i > k; --i )
    y[i] = y[i-1];
  y[k] = key;
}

int main ( )
{
  timeval t1,t2;
  gettimeofday(&t1, NULL);
  int maxSize = 10000;
  int x[maxSize];

  for( int z = 0; z < 1; ++z ) {
    
    int size = 10000;

    for( int i = 0; i < size; ++i )
      x[i] = size-i-1;
    int a = 0, b = size - 1;
    
    int j = a+1;
    while( j <= b ) {
      insert(x,a,b,j);
      ++j;
    }
    /*
    if( z == 0 ) {
      for( int i = 0; i < size; ++i )
	cout << " " << x[i];
      cout << endl;
    }
    */
  }

  gettimeofday(&t2, NULL);
  double dif = diffTime(t2,t1);

  cout << "Time: " << dif << endl;
}

