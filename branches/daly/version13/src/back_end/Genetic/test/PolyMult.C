// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include "GAMatrixBurau.h"
#include "PBTree.h"
#include "Int2.h"
#include <sys/time.h>

double diffTime(timeval t2, timeval t1)
{
  double result;
  long s = t2.tv_sec - t1.tv_sec;
  result = (t2.tv_usec - t1.tv_usec) * 0.000001 + s;
  //if( t2.tv_usec < t1.tv_usec ) --result;
  return result;
}

int main ( )
{
  Polynomial<Integer> x;
  Polynomial<Int2> y;
  Word w;
  Map Ma;
  const int n = 400;
  const int iter = 1;

  Polynomial<Integer> p("1-x1,");
  Polynomial<Integer> q("1,");

  timeval t1,t2;
  gettimeofday(&t1, NULL);

  for( int t = 0; t < iter; ++t ) {
    q = Polynomial<Integer>("1,");
    for( int i = 0; i < n; ++i ) {
      q = q * p;
    }
  }

  gettimeofday(&t2, NULL);
  double dif = diffTime(t2,t1);
  cout << "Time: " << dif << endl;

  //cout << "q^n = " << q << endl;
}


