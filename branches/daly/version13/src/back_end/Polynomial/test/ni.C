// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Experimenting with the Newton's iteration
//
// Principal Authors: Dmitry Bormotov
//


//#include "double.h"
#include "WordParser.h"
#include "Matrix.h"
#include "Polynomial.h"
#include "Map.h"
#include "MatrixComputations.h"


Matrix<double> makeToeplitzMatrix( const VectorOf<double>& u, 
				     const VectorOf<double>& v )
  // contructs Toeplitz matrix from column u and row v, 
  // the first coordinate of v (0) is ignored.
{
  int n = u.length();
  Matrix<double> M(n);
  for( int i = 0; i < n; ++i )
    for( int j = i; j < n; ++j )
      M[j][j-i] = u[i];
  for( int i = 1; i < n; ++i )
    for( int j = i; j < n; ++j )
      M[j-i][j] = v[i];
  return M;
}

void printMatrix( const Matrix<double>& M )
{
  for ( int i = 0; i < M.height(); i++ ){

    cout << "| ";
    for ( int j = 0; j < M.width(); j++ )
      cout << double(M[i][j]) << ", ";

    if( i==M.height()-1 ) 
      cout << "|."<<endl;
    else 
      cout <<"|"<< endl;
  } 
} 


int main( )
{
  Polynomial<Integer> p1;
  Monomial<Integer> m;
  Map Ma;

  //cout.precision(20);

  /*
  const int size = 3;
  VectorOf<double> u(size), v(size);
  
  for( int i = 0; i < size; ++i ) {
    u[i] = (double(i+1))/10;
    v[i] = (double(i+1))/10;
  }
  
  Matrix<double> M = makeToeplitzMatrix(u,v);
  */

  
  const int size = 2;
  Matrix<double> M(size);
  VectorOf<double> u(size), v(size);
  /*
  M[0][0] = double(5,10);
  M[0][1] = double(4,10);
  M[1][0] = double(4,10);
  M[1][1] = double(5,10);
  */
  M[0][0] = .5;
  M[0][1] = .4;
  M[1][0] = .4;
  M[1][1] = .5;


  cout << "M: " << endl << M << endl;

  /*
  MatrixComputations<double> MC(M);
  MC.invertMatrix();
  //cout << endl << MC.isInvertibleMatrix() << endl;
  if( MC.isInvertibleMatrix() != yes ) {
    cout << "The matrix is not invertible" << endl; 
    return 0;
  }

  Matrix<double> MI = MC.getInverseMatrix() ;
  cout << "Inverse: " << endl << MI << endl;
  
  cout << "Checking M * M^-1 :" << endl << M * MI << endl; 
  */
  
  for( int i = 0; i < size; ++i ) {
    u[i] = 0;
    v[i] = 0;
  }
  
  u[0] = 1;

  Matrix<double> I = makeToeplitzMatrix(u,v);
  
  cout << "I: " << endl << I << endl;
  
  Matrix<double> I2 = I;
  for( int i = 0; i < size; ++i )
    I2[i][i] = 2;

  cout << "Newton's Iteration: " << endl;
  char c = 'y';
  int s = 0;
  //Matrix<double> Y = I;
  Matrix<double> Y(size);
  //Y[0][0] = double(5,10);
  //Y[0][1] = double(-4,10);
  //Y[1][0] = double(-4,10);
  //Y[1][1] = double(5,10);

  Y[0][0] = 1;
  Y[0][1] = 0;
  Y[1][0] = 0;
  Y[1][1] = 1;

  while ( c != 'n' && c != 'N' ) {
    
    cout << "Step " << ++s << ":" << endl;
    
    //cout << "M" << endl << M << endl;
    //cout << "Y" << endl << Y << endl;
   

    Matrix<double> R1 = M * Y;
    //cout << "M * Y" << endl << R1 << endl;
  
    Matrix<double> R2 = I2 - R1;
    //cout << "2*I - M * Y" << endl << R2 << endl;

    Matrix<double> R = Y * R2;

    //Matrix<double> R = Y * (I2 - M * Y);
    
    Y = R;
    cout << "Y:" << endl << Y << endl;
    /*
    cout << "Y:" << endl;
    printMatrix(Y);
    cout << endl;
    */
    
    cout << "The product M * M^-1: " << endl << M * Y << endl;
    
    cout << "Continue (y/n)? ";
    cin >> c;
  }

}




