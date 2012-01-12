// Contents: Given a sorted array and integer s the program checks 
//           in linear time whether there are two elements in the array 
//           (not necessarily distinct) which add up to s.
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


// pre: x[a..b] - sorted, a <= b, s

// post: if( found ) x[c] + x[d] == s
//       else there's no i,j from { x[a],...,x[b] } : s = i + j

int main ( )
{
  // fill array x

  int size = 20;
  int x[size];
  for( int i = 0; i < size; ++i )
    x[i] = i+1;
  int a = 0, b = size - 1;

  // Enter s:
  
  cout << "Enter sum s: ";
  int s;
  cin >> s;
  cout << endl;

  // initialization

  bool found = false;
  int c = a, d = b; 

  /* while inv: 
     c <= d + 1 &&
     if( found ) x[c] + x[d] == s
     else there's no i from { x[a],...,x[c-1] } 
            and there's no j from { x[d+1],...,x[b] }
	  : i + j == s
  */
  
  while( !found && c <= d ) {
    
    int dif = s - x[c];
    if( dif == x[d] )
      found = true;
    else if( dif < x[d] ) 
      --d;
    else
      ++c;
  }

  if( found ) 
    cout << " s = x[" << c << "] + x[" << d << "] = " << x[c] << " + " 
	 << x[d] << endl;
  else
    cout << "There is no i,j in the array such that i + j == s" << endl; 
}

