// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition of the GaussTransformation class.
//
// Principal Authors: Dmitry Bormotov
//
// Status: provisional
//
// Revision History:
//


#ifndef _GAUSS_TRANSFORMATION_H_
#define _GAUSS_TRANSFORMATION_H__

#include "global.h"
#include "Integer.h"
#include "Rational.h"
#include "Matrix.h"


template <class T> class Determinant;
template <class T> class Inverse;

// ------------------------- GaussTransformation ----------------------------//


template <class T> class GaussTransformation {

friend class Determinant<T>;

friend class Inverse<T>;

friend class AbelianInfinitenessProblem;

private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  GaussTransformation( T **M, int n, int m, bool inverseChecking = false) : 
    matrix( M ), numOfRows( n ), numOfCols( m ), 
    bDone( false ), bStart( false ), 
    bInverseChecking( inverseChecking ),
    isSingularMatrix( dontknow ), isInvertibleMatrix( dontknow ) { }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void startComputation( ) 
  {
  #if SAFETY > 0   
    if ( bStart )
      error("void GaussTransformation::startComputation( ) : "
	    "the computation has been already started.");
  #endif

    bStart = true;
    bDone = false;
    current = 0;
  }  
         
  void continueComputation( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) const { return bDone; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Trichotomy isSingular( ) const 
  {
  #if SAFETY > 0
    if ( !bDone )
      error("Trichotomy GaussTransformation::isSingular( ) : "
	    "tried to get result before the computation is finished.");
  #endif

    return isSingularMatrix;
  }

  Trichotomy isInvertible( ) const 
  {
  #if SAFETY > 0
    if ( !bDone )
      error("Trichotomy GaussTransformation::isInvertible( ) const : "
	    "tried to get result before the computation is finished");
  #endif

    return isInvertibleMatrix;
  }

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool bDone;
  bool bStart;
  bool bInverseChecking;
  Trichotomy isSingularMatrix;
  Trichotomy isInvertibleMatrix;

  T **matrix;
  int numOfRows;
  int numOfCols;
  int current;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Inner methods:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void finishComputation( Trichotomy isSingular, Trichotomy isInvertible )
  {
    isSingularMatrix = isSingular;
    isInvertibleMatrix = isInvertible;
    bDone = true;
    bStart = false;
  }

  void addRow( int firstRow, int secondRow, T koef);

  void makeZero( int& row1, int& row2 );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

//  void printMatrix( char *name );
  friend int main( );

#endif

};


// ----------------------------- Determinant --------------------------------//


template <class T> class Determinant {


public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Determinant( const Matrix<T>& M ) : originalMatrix( M ) { } 

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void startComputation( );
         
  void continueComputation( ) 
  {
  #if SAFETY > 0
    if ( !GT )
      error("void Determinant::continueComputation( ) : "
	    "tried to continue computation before it's started.");
  #endif
    
    GT->continueComputation();
    if( GT->done() ) 
      finishComputation();
  }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) const 
  { 
  #if SAFETY > 0
    if ( !GT )
      error("bool Determinant::done( ) const : "
	    "tried to call this function before computation is started.");
  #endif
        
    return GT -> done( ); 
  }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  T getDeterminant( ) const
  {
  #if SAFETY > 0
    if ( !GT )
      error("T Determinant::getDeterminant( ) const : "
	    "tried to get result before computation is started.");
    if( !( GT->done() ) )
      error("T Determinant::getDeterminant( ) const : "
	    "tried to get determinant before the computation is finished");
  #endif
    
    return determinant;
  }


 private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const Matrix<T>& originalMatrix;
  T **matrix;
  GaussTransformation<T> *GT;
  T determinant;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private methods:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void finishComputation( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  friend int main( );

#endif

};


// ------------------------------- Inverse ----------------------------------//


template <class T> class Inverse {


public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Inverse( const Matrix<T>& M ) : originalMatrix( M ) 
  {
    inverseMatrix = new Matrix<T>( M.size );

//    cout << "originalMatrix = " << originalMatrix << endl;
  } 

  ~Inverse( ) { delete inverseMatrix; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void startComputation( );
         
  void continueComputation( ) 
  {
  #if SAFETY > 0
    if ( !GT )
      error("void Inverse::continueComputation( ) : "
	    "tried to continue computation before it's started.");
  #endif
    GT->continueComputation();
    if( GT->done() ) 
      finishComputation();
  }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) const 
  { 
  #if SAFETY > 0
    if ( !GT )
      error("bool Inverse::done( ) const : "
	    "tried to call this function before computation is started.");
  #endif
   
    return GT -> done( ); 
  }

  bool isInvertible( ) const 
  { 
  #if SAFETY > 0
    if ( !GT )
      error("bool Inverse::isInvertible( ) const : "
	    "tried to get result before computation is started.");
  #endif
    return ( (GT -> isInvertible( )) != no ? true : false );
  }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const Matrix<T>& getInverseMatrix( ) const
  {
  #if SAFETY > 0
    if ( !GT )
      error("const Matrix<T>& Inverse::getInverseMatrix( ) const : "
	    "tried to get result before computation is started.");
    if( !( GT->done() ) )
      error("const Matrix<T>& Inverse::getInverseMatrix( ) const : "
	    "tried to get inverse matrix before the computation is finished");
    if( (GT->isInvertible()) == no )
      error("const Matrix<T>& Inverse::getInverseMatrix( ) const : "
	    "tried to get inverse matrix when it doesn't exist");
  #endif
    
    return *inverseMatrix;
  }


 private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const Matrix<T>& originalMatrix;
  T **matrix;
  GaussTransformation<T> *GT;
  Matrix<T> *inverseMatrix;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private methods:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Inverse( const Inverse& );

  void finishComputation( );

  void makeGaussTransformation( T** matrix, int size );

  void abolishCoefficients( int size );

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  friend int main( );

#endif

};

#ifdef DEBUG

template <class T>
inline void printMatrix( char *name, T **matrix, 
			 int numOfRows, int numOfCols )
{
  cout << name << endl;
  for( int i = 0; i < numOfRows; i++ ) {
    for( int j = 0; j < numOfCols; j++ )
      cout << " " << matrix[i][j];
    cout << endl;
  }
  cout << endl;
}
#endif

#endif



