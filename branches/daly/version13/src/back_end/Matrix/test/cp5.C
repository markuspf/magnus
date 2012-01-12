// Contents: Computer Project 5
//           Implicit solution for the heat equation
//
// Principal Authors: Dmitry Bormotov
//


#include "global.h"
#include "MatrixComputations.h"

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
 
 double a = - double(t)/(x*x);
 double c = a;
 double b = -2 * a + 1;
 
 // Initialize the trasformation matrix

 Matrix<Rational> T(size);
 for( int i = 0; i < size; ++i )
   for( int j = 0; j < size; ++j )
     T[i][j] = 0;

 for( int i = 1; i < size-1; ++i ) {
   T[i][i-1] = c;
   T[i][i] = b;
   T[i][i+1] = a;
 }

 T[0][0] = b; 
 T[0][1] = a;
 T[size-1][size-2] = c; 
 T[size-1][size-1] = b;
 
 cout << "T: " << endl << T << endl;


 MatrixComputations<Rational> MC(T);
 MC.invertMatrix();
 const Matrix<Rational>& T2 = MC.getInverseMatrix();
 cout << "Inverse of T: " << endl << endl;

 cout.precision(3);
 
 for( int j = 0; j < size; ++j ) {
   for( int i = 0; i < size; ++i ) {
     cout.width(9);
     cout << double(T2[j][i]);
   }
   cout << endl;
 }
 cout << endl;


 
 for( int i = 1; i < size; ++i )
   for( int j = 1; j < size; ++j ) {
     A[i][j] = 0;
     for( int k = 0; k < size; ++k )
       A[i][j] += T2[j][k] * A[i-1][k]; 
   }
     
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


