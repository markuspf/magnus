// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of the GaussTransformation class
//
// Principal Authors: Dmitry Bormotov
//
// Status: provisional implementation
//
// Revision History:
//


#include "GaussTransformation.h"


// ------------------------- GaussTransformation ----------------------------//

template <class T>
void GaussTransformation<T>::addRow( int firstRow, int secondRow, T koef )
{
  for( int l = current; l < numOfCols; l++ )
    matrix[firstRow][l] += koef * matrix[secondRow][l];
}


void GaussTransformation<Integer>::makeZero( int& row1, int& row2 )
{
  while( true ) {
    if( abs(matrix[row1][current]) >= abs(matrix[row2][current]) ) {
      addRow(row1, row2, -matrix[row1][current] / matrix[row2][current]);
      if( sign(matrix[row1][current]) == 0 ) {
	row1 = row2;
	break;
      }
    }
    else {
      addRow(row2, row1, -matrix[row2][current] / matrix[row1][current]);
      if( sign(matrix[row2][current]) == 0 )
	break;
    }
  }
}


void GaussTransformation<Rational>::makeZero( int& row1, int& row2 )
{
  addRow(row2, row1, -matrix[row2][current] / matrix[row1][current]);
}


template <class T>
void GaussTransformation<T>::continueComputation( )
{
#if SAFETY > 0
  if ( !bStart )
    error("void GaussTransformation<T>::continueComputation( ) : "
	  "tried to continue computation before it's started");
  if ( bDone )
    error("void GaussTransformation<T>::continueComputation( ) : "
	  "tried to continue computation after it's done");
#endif

//  printMatrix("matrix = ", matrix, numOfRows, numOfCols);
  
  if( current < numOfRows - 1 ) {
    int row1 = -1;
    
    for( int i = current; i < numOfRows; i++ ) 
      if( sign( matrix[i][current] ) != 0 ) {
	row1 = i;
	break;
      }
    
    if( row1 == -1 ) {
      finishComputation(yes, no);
      return;
    }
    
    int row2 = row1;

    while( true ) {

      bool bSuccess = false;
      
      for( i = row2 + 1; i < numOfRows; i++ ) 
	if( sign( matrix[i][current] ) != 0 ) {
	row2 = i;
	bSuccess = true;
	break;
      }

      if( !bSuccess ) {
	if( row1 != current ) {
	  addRow(current, row1, 1);
	  addRow(row1, current, -1);
	}
	if( bInverseChecking )
	  if( abs( matrix[current][current] ) != 1 )
	    finishComputation(dontknow, no);
	current++;
	return;
      }
      else
	makeZero(row1, row2);
    }
    
  }
  else
    if( sign(matrix[current][current]) == 0 )
      finishComputation(yes, no);
    else
      if( bInverseChecking ) {
	if( abs( matrix[current][current] ) != 1 )
	  finishComputation(no, no);
	else
	  finishComputation(no, yes);
      }
      else
	finishComputation(no, dontknow);
}


/*
// Debugging stuff:

#ifdef DEBUG

template <class T>
void GaussTransformation<T> :: printMatrix( char *name )
{
  cout << name << endl;
  for (int i = 0; i < numOfRows; i++) {
    for (int j = 0; j < numOfCols; j++)
      cout << " " << matrix[i][j];
//      matrix[i][j].printon(cout,10,7);
      
    cout << endl;
  }
  cout << endl;
}
 
#endif
*/

// ----------------------------- Determinant --------------------------------//

template <class T>
void Determinant<T>::startComputation( )
{
  int size = originalMatrix.size;

#if SAFETY > 0
  if( size <= 0 )
    error("void Determinant<T>::startComputation( ) : "
	  "incorrect size of matrix");
#endif

  matrix = new (T*)[size];
  for( int i = 0; i < size; i++ ) {
    matrix[i] = new T[size];
    for( int j = 0; j < size; j++ )
      matrix[i][j] = originalMatrix.theMatrix[i][j];
  }
  GT = new GaussTransformation<T>(matrix, size, size);
  GT -> startComputation( );
}

template <class T>
void Determinant<T>::finishComputation( )
{
  int size = originalMatrix.size;
  if( (GT->isSingular()) == YES )
    determinant = 0;
  else {
    determinant = 1;
    for ( int i = 0; i < size; i++ )
      determinant *= GT->matrix[i][i];
  }

  for( int i = 0; i < size; i++ )
    delete [] matrix[i];
  delete [] matrix;
  delete GT;
}  
  

// ------------------------------- Inverse ----------------------------------//


void Inverse<Integer>::makeGaussTransformation( Integer** matrix, int size )
{
  GT = new GaussTransformation<Integer>(matrix, size, size*2, true);
}


void Inverse<Rational>::makeGaussTransformation( Rational** matrix, int size )
{
  GT = new GaussTransformation<Rational>(matrix, size, size*2);
}


template <class T>
void Inverse<T>::startComputation( )
{
  int size = originalMatrix.size;
  matrix = new (T*)[size];
  for( int i = 0; i < size; i++ ) {
    matrix[i] = new T[size*2];
    for( int j = 0; j < size; j++ ) {
      matrix[i][j] = originalMatrix.theMatrix[i][j];
      matrix[i][size+j] = 0;
    }
    matrix[i][size+i] = 1;
  }

//  printMatrix("theMatrix = ", originalMatrix.theMatrix, size, size);
//  printMatrix("matrix = ", matrix, size, size*2);

  makeGaussTransformation(matrix, size);
  GT -> startComputation( );
}


void Inverse<Integer>::abolishCoefficients( int size )
{
  int realSize = size * 2;
  for( int i = 0; i < size; i++ )
    if( sign(matrix[i][i]) < 0 )
      for( int j = i; j < realSize; j++ )
	matrix[i][j] = - matrix[i][j];
}


void Inverse<Rational>::abolishCoefficients( int size )
{
  int realSize = size * 2;
  for( int i = 0; i < size; i++ )
    if( matrix[i][i] != 1 ) {
      Rational koef = matrix[i][i];
      for( int j = i; j < realSize; j++ )
	matrix[i][j] /= koef;
    }
}


template <class T>
void Inverse<T>::finishComputation( )
{
  int size = originalMatrix.size;
  int realSize = 2 * size;
  int i, j, k;
  if( (GT->isInvertible()) != no ) {
    for( i = 0; i < size - 1; i++ ) 
      for( j = i + 1; j < size; j++ )
	if( sign(matrix[i][j]) != 0 ) {
	  T quotient = matrix[i][j] / matrix[j][j];
	  for( k = j; k < realSize; k++ )
	    matrix[i][k] -= quotient * matrix[j][k];
	}

    abolishCoefficients(size);

    for( i = 0; i < size; i++ )
      for( j = 0; j < size; j++ )
	(*inverseMatrix)[i][j] = matrix[i][size+j];
  }
  
  for( i = 0; i < size; i++ )
    delete [] matrix[i];
  delete [] matrix;
  delete GT;
}  


template class GaussTransformation<Integer>;
template class Determinant<Integer>;
template class Inverse<Integer>;
template class GaussTransformation<Rational>;
template class Determinant<Rational>;
template class Inverse<Rational>;
