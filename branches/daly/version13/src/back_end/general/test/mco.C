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

#include <sys/time.h>
#include <unistd.h>
#include "global.h"
#include "values.h"

const int n = 6;
int m[n+1][n+1], s[n+1][n+1];

void printOptimalParens( int i, int j )
{
  if( i == j ) { cout << "A" << i; return; }
  if( i+1 == j ) { cout << "A" << i << " A" << j; return; }
  
  int k = s[i][j];
  cout << "(";
  printOptimalParens(i,k);
  cout << ")(";
  printOptimalParens(k+1,j);
  cout << ")";
}

int main ( )
{
  //int p[] = {30,35,15,5,10,20,25 };
  int p[] = {5,10,3,12,5,50,6};
  int pLen = n+1;
  

  for( int i = 1; i <= n; ++i )
    for( int j = 1; j <= n; ++j ) {
      m[i][j] = 0;
      s[i][j] = 0;
    }
  
  for( int i = 1; i <= n; ++i )
    m[i][i] = 0;
  for( int l = 2; l <= n; ++l )
    for( int i = 1; i <= n-l+1; ++i ) {
      int j = i+l-1;
      m[i][j] = MAXINT;
      for( int k = i; k <= j-1; ++k ) {
	int q = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];
	if( q < m[i][j] ) {
	  m[i][j] = q;
	  s[i][j] = k;
	}
      }
    }

  cout << "m: " << endl;
  for( int i = 1; i <= n; ++i ) {
    for( int j = 1; j <= n; ++j )
      cout << " " << m[i][j];
    cout << endl;
  }

  cout << "s: " << endl;
  for( int i = 1; i <= n; ++i ) {
    for( int j = 1; j <= n; ++j )
      cout << " " << s[i][j];
    cout << endl;
  }

  printOptimalParens(1,n);
}



