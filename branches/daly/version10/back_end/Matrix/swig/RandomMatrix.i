///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works

%module Magnus

%{
#include "RandomMatrix.h"
%}

%define LISTRANDOMMATRIX(RANDOMMATRIX,MATRIX,TYPE)
%{
typedef Matrix<TYPE> MATRIX;
typedef RandomMatrix<TYPE> RANDOMMATRIX;
%}

class RANDOMMATRIX
{

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  RANDOMMATRIX( int n = 0 );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int getSize() const;  
    
  MATRIX getRandomMatrix( );  
 
/*
private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int size;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int coefficient( );

  Matrix<R> getAtomicMatrix( );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  //friend int main( );

#endif
*/
};

%enddef
LISTRANDOMMATRIX(RandomMatrixInteger,MatrixInteger,Integer)
LISTRANDOMMATRIX(RandomMatrixRational,MatrixRational,Rational)
//LISTRANDOMMATRIX(RandomMatrixdouble,Matrixdouble,double)   not supported