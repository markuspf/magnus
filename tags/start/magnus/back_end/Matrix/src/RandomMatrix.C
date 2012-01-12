/*
 *   $Id: RandomMatrix.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of classes RandomMatrix. 
//
// Principal Author: Dmitry Bormotov
//
// Status: In development
//
// Usage:
//
// Discussion:
//
// Revision History:
//
//


#include "RandomMatrix.h"

const int skip = 16;

//------------------------------ RandomMatrix -------------------------------//

int RandomMatrix<Integer>::coefficient( )
{
  return ((rand()>>skip) % 2 == 0) ? 1 : -1;
}


Matrix<Integer> RandomMatrix<Integer>::getAtomicMatrix( )
{
  int i,j;
  
  Matrix<Integer> result(size);
  for( i = 0; i < size; i++ ) {
    for( j = 0; j < size; j++ )
      result[i][j] = 0;
    result[i][i] = 1;
  }
  if( size > 1) {
    i = (rand()>>skip) % size;
//    do {
      j = (rand()>>skip) % size;
//    } while (i == j);
    result[i][j] = coefficient();
  }
  if( (rand()>>skip) % 2 == 0 ) {
    i = (rand()>>skip) % size;
    for( j = 0; j < size; j++ )
      result[i][j] = - result[i][j];
  }

  return result;
}


Matrix<Integer> RandomMatrix<Integer>::getRandomMatrix( )
{
  const int maxCount = 10;
  Matrix<Integer> result = getAtomicMatrix();
  int count = (rand()>>skip) % maxCount;
  for( int i = 0; i < count; i++ ) {
    result *= getAtomicMatrix();
  }

  return result;
}


Matrix<Rational> RandomMatrix<Rational>::getRandomMatrix( )
{
  const int maxNumber = 10;
  Matrix<Rational> result(size);
  for( int i = 0; i < size; i++ )
    for( int j = 0; j < size; j++ ) {
      int numerator = (rand()>>skip) % maxNumber;
      int denominator;
      do {
	denominator = (rand()>>skip) % maxNumber;
      } while( denominator == 0 );
      result[i][j] = Rational(numerator, denominator);
    }

  return result;
}
