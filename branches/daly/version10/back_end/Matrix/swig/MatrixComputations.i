///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works

%module Magnus

%{
#include "MatrixComputations.h"
%}

%define LISTMATRIXCOMPUTATIONS(MATRIXCOMPUTATIONS,MATRIX,TYPE)
%{
typedef Matrix<TYPE> MATRIX;
typedef MatrixComputations<TYPE> MATRIXCOMPUTATIONS;
%}




class MATRIXCOMPUTATIONS {

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  MATRIXCOMPUTATIONS( const MATRIX& matrix) ;

  ~MATRIXCOMPUTATIONS();


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
    const MATRIX& matrix() const { return theMatrix;}

  int size() const;
  // Returns the size of SquareMatrix

  bool isIdentity() const;
  // True if matrix is identical

  TYPE getDeterminant();
  // Returns determinant if exists, if not then computes it.

  bool detKnow() const;
  // true if determinant was computed

  Trichotomy isInvertibleMatrix() const;
  // Yes if matrix invertible
  
  void invertMatrix();
  // Makes inverse matrix if it exists
 
  const MATRIX& getInverseMatrix() const;

  TYPE det() const;
/*
  // This function nedet only for constant acces to determinant
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  IPC tools:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  friend ostream& operator < ( ostream& ostr, const MatrixComputations& DA )
  {
    DA.write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, MatrixComputations& DA)
  {
    DA.read(istr);
    return istr;
  }
private:
  void write( ostream& ostr ) const{
     ostr < theMatrix;
     ostr < detKnown;
     ostr < isInvertible;
     ostr < determinant;
     if (isInvertible == yes) ostr < *inverseMatrix;
  };
 
  void read( istream& istr ){
     istr > theMatrix;
     istr > detKnown;
     istr > isInvertible;
     istr > determinant;
     delete inverseMatrix;
     if (isInvertible == yes){
      Matrix<R> tmpMatrix;
      istr > tmpMatrix;
      inverseMatrix  = new Matrix<R>(tmpMatrix);
     }
  }

  void abolishCoefficients(Matrix<R>& matrix );

  bool detKnown;
  // True if determinant was computed

  Trichotomy isInvertible;
  // Yes if inverse matrix was built, no if matrix couldn't be inverted, 
  // otherwize -  dontknow

  R determinant;
  // determinant

  Matrix<R>* inverseMatrix;
  // Holds inverse matrix, if it was built

  Matrix<R> theMatrix;
  // Holds the matrix.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  //friend int main(...);

#endif
*/
};

%enddef
LISTMATRIXCOMPUTATIONS(MatrixComputationsInteger,MatrixInteger,Integer)
LISTMATRIXCOMPUTATIONS(MatrixComputationsRational,MatrixRational,Rational)
LISTMATRIXCOMPUTATIONS(MatrixComputationsdouble,Matrixdouble,double)