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
#include "List.h"
#include "RandomNumbers.h"


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

Matrix<double> multByConst( const Matrix<double>& M, double k )
{
  Matrix<double> R(M.width());

  for( int i = 0; i < M.width(); ++i )
    for( int j = 0; j < M.width(); ++j )
      R[i][j] = M[i][j] * k;
  
  return R;
}

template<class R> void printMatrix( const Matrix<R>& M )
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

Matrix<double> transpose( const Matrix<double>& M )
{
  Matrix<double> R(M.width());

  for( int i = 0; i < M.width(); ++i )
    for( int j = 0; j < M.width(); ++j )
      R[i][j] = M[j][i];
  
  return R;
}


Matrix<double> makeRandomToeplitz( int size )
{
  const double max = 100;
  UniformRandom r(1),r2(2345);
  VectorOf<double> u(size), v(size);
  for( int i = 0; i < size; ++i ) {
    u[i] = max * r.rand();
    v[i] = max * r.rand(); 
    if( r2.rand() < .5 )
      u[i] *= -1;
    if( r2.rand() < .5 )
      v[i] *= -1;
  }
  return makeToeplitzMatrix(u,v);
}


Matrix<double> makeRandomMatrix( int size )
{
  const double max = 10;
  UniformRandom r(1),r2(2345);
  Matrix<double> R(size);

  for( int i = 0; i < size; ++i )
    for( int j = 0; j < size; ++j ) {
      R[i][j] =  max * r.rand();
      if( r2.rand() < .5 )
	R[i][j] *= -1;
    }

  return R;
}


int main( )
{
  Polynomial<Integer> p1;
  Monomial<Integer> m;
  Map Ma;

  cout.precision(10);
  const int numOfIter = 50;
  double tau = .1;

  const int size = 5;
  VectorOf<double> u(size), v(size);
  /*
  u[0] = 0.196;
  u[1] = 0.2;
  u[2] = 0.3;
  u[3] = 0.4;
  u[4] = 0.5;
  v[1] = 0.2;
  v[2] = 0.3;
  v[3] = 0.4;
  v[4] = 0.5;
  
  Matrix<double> M = makeToeplitzMatrix(u,v);
  */

  //Matrix<double> M = makeRandomToeplitz(size);
  Matrix<double> M = makeRandomMatrix(size);

  cout << "M: " << endl << M << endl;
  for( int i = 0; i < 1; ++i ) {
    M = M * transpose(M);
    cout << "M: " << endl << M << endl;
  }

  /*
  const int size = 2;
  Matrix<double> M(size);
  VectorOf<double> u(size), v(size);
  */
  /*
  M[0][0] = double(5,10);
  M[0][1] = double(4,10);
  M[1][0] = double(4,10);
  M[1][1] = double(5,10);
  */
  /*
  M[0][0] = .5;
  M[0][1] = .4;
  M[1][0] = .4;
  M[1][1] = .5;
  */

  //cout << "M: " << endl << M << endl;

  
  Matrix<Rational> MR(size);
  for( int i = 0; i < size; ++i )
    for( int j = 0; j < size; ++j )
      MR[i][j] = M[i][j];
  
  //cout << "MR: " << endl;
  //printMatrix(MR);
  //cout << endl;
  /*
  MatrixComputations<Rational> MC(MR);
  MC.invertMatrix();
  //cout << endl << MC.isInvertibleMatrix() << endl;
  if( MC.isInvertibleMatrix() != yes ) {
    cout << "The matrix is not invertible" << endl; 
    return 0;
  }

  Matrix<Rational> MI = MC.getInverseMatrix() ;
  cout << "Inverse: " << endl;
  printMatrix(MI);
  cout << endl;
  
  cout << "Checking M * M^-1 :" << endl << MR * MI << endl; 
  */
  for( int i = 0; i < size; ++i ) {
    u[i] = 0;
    v[i] = 0;
  }
  
  u[0] = 1;

  Matrix<double> I = makeToeplitzMatrix(u,v);
  
  //cout << "I: " << endl << I << endl;
  
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


  Matrix<double> X0(I), Mk(size);
  
  const double eps = 0.001, min = 1e-10, max = 1e10;
  ListOf<double> steps,ts;
  double t = 0, old_t = 0;
  
  while( true ) {

    double step = 1 - t;

    while( true ) {

      t += step;
      
      cout << "Trying step: " << step << "  t: " << t << endl << endl;
      
      Mk = I + multByConst(M - I, t);
      Y = X0;
      
      //cout << "Mk: " << Mk << endl;
      //cout << "X0: " << X0 << endl;
      
      while( true ) {

	for( int s = 0; s < 10; ++s ) {
	  Matrix<double> R1 = Mk * Y;
	  //cout << "R1: " << endl << R1 << endl;
	  Matrix<double> R2 = I2 - R1;
	  //cout << "R2: " << endl << R2 << endl;
	  Matrix<double> R = Y * R2;
	  //cout << "R: " << endl << R << endl;
	  Y = R;
	}
	Matrix<double> P =  Mk * Y;
	//cout << "The product M * M^-1: " << endl << P << endl;
	cout << "10 steps ... " << endl;
	 if( P[0][0] < min || P[0][0] > max || fabs(P[0][0] - 1) < min )
	  break;
      }
      
      //cout << "Approximating M^-1:" << endl << Y << endl;
      Matrix<double> P =  Mk * Y;
      //cout << "The product M * M^-1: " << endl << P << endl;
      if( P[0][0] < min || P[0][0] > max ) {
	cout << "Divergence!" << endl << endl;
	t -= step;
	step /= 2;
	continue;
      }
      else {
	cout << "Convergence!" << endl << endl;
	
	//cout << "fabs(t - 1): " << fabs(t - 1) << endl;
	//cout << "eps: " << eps << endl;

	if( fabs(t - 1) < eps || step < eps ) {
	  step = t - old_t;
	  cout << "Final step: " << step << "  t: " << t << endl << endl;
	  steps.append(step);
	  ts.append(t);
	  old_t = t;
	  break;
	}
	step /= 2;
	continue;
      }
    }
    
    if( fabs(t - 1) < eps )
      break;
    X0 = Y;  
  }

  cout << "Steps: " << steps << endl << "Tau_k: " << ts << endl;
}





