///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
/*
  for template class, we mapped ClassName<Type>  to ClassNameType
  such as: MatrixInteger,MatrixRational and so on.
  at the same time,the constructor name will changed to MatrixInteger,MatrixRational.
*/
%module Magnus
%include DArray.i
%{
#include "Matrix.h"
%}

%define LISTMATRIX(MATRIX,DARRAY,TYPE)
%{
typedef DArray<TYPE> DARRAY;
typedef Matrix<TYPE> MATRIX;
%}

class MATRIX:public DARRAY{

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  MATRIX(int n = 0 );
  // Makes Matrix with height=width=n


  %name(MATRIX ## FromInt) MATRIX(int h, int w);
  ///by xuxw:name equals to "MATRIXFromInt", such as MatrixIntegerFromInt,MatrixRationalFromInt.
  // Makes Matrix with height == h and width == w
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Operators:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

//  Matrix& operator += ( const Matrix& M );

//  Matrix& operator -= ( const Matrix& M ) ;

//  Matrix& operator *= ( const Matrix& M ) ;
//  Matrix operator + ( const Matrix& M) const;

//  Matrix operator - ( const Matrix& M ) const;

//  Matrix operator * ( const Matrix& M ) const ;

//  Matrix operator - ( ) const;

  %addmethods {

	MATRIX __mul__(const MATRIX& w) {
		return (*self)*w;
	}

	MATRIX __add__(const MATRIX& w) {
		return (*self)+w;
	}

	MATRIX __sub__(const MATRIX& w) {
		return (*self)-w;
	}

  }

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // Inherited from DArray  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // Inherited from DArray  
/*
  protected :
	Matrix( MatrixRep<R> newrep ) : 
            DerivedObjectOf<DArray<R>,MatrixRep<R> >(new MatrixRep<R>(newrep)) {}
	Matrix( MatrixRep<R>* newrep ) : 
            DerivedObjectOf<DArray<R>,MatrixRep<R> >(newrep) {}
*/
};

%enddef
LISTMATRIX(MatrixInteger,DArrayInteger,Integer)
LISTMATRIX(MatrixRational,DArrayRational,Rational)
LISTMATRIX(Matrixdouble,DArraydouble,double)
