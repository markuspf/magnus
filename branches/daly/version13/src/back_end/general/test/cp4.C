// Contents: Computer Project 4
//           Explicit solution for the heat equation
//
// Principal Authors: Dmitry Bormotov
//


//#include "math.h"
#include "global.h"

void main( )
{
 const int size = 8;
 const int t = 1; 
 const int x = 2;
 double A[size][size+1];

 for( int u = 0; u < size; ++u )
   for( int v = 0; v <= size; ++v )
     A[u][v] = -1;

 for( int i = 1; i < size; ++i )
   A[0][i] = 0;
 A[0][0] = 1;
 for( int i = 1; i < size; ++i )
   A[i][0] = 1 / ( 2 * sqrt(PI*i*t) );
 for( int i = 0; i < size; ++i )
   A[i][size] = 0;
 
 double a = double(t)/(x*x);
 double b = a;
 double c = -2 * a + 1;
 
 for( int i = 1; i < size; ++i )
   for( int j = 1; j < size; ++j )     
     A[i][j] = a * A[i-1][j+1] + b * A[i-1][j-1] + c * A[i-1][j];
     
 cout.precision(3);
 cout.width(7);
 
 for( int j = 0; j < size; ++j ) {
   for( int i = 0; i < size; ++i ) {
     cout.width(9);
     cout << A[i][j];
   }
   cout << endl;
 }
 cout << endl;
}


