///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works

%module Magnus

%{
#include "GaussTransformation.h"
%}

%define LISTGAUSSTRANSFORMATION(GAUSSTRANSFORMATION,MATRIX,TYPE)
%{
typedef Matrix<TYPE> MATRIX;
typedef GaussTransformation<TYPE> GAUSSTRANSFORMATION;
%}

class GAUSSTRANSFORMATION {


public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  // If buildTransMatrix or buildInvTransMatrix = true the transformation matrix would be built
  GAUSSTRANSFORMATION(const MATRIX& M, bool buildTransMatrix = false, bool buildInvTransMatrix = false) ;

  // Copy constructor and operator = are disabled.
//  %name(GaussTransformation ## _Copy) GAUSSTRANSFORMATION(const GAUSSTRANSFORMATION&);
//undefined!!!  by xuxw
//  GaussTransformation operator = ( const GaussTransformation& );

  ~GAUSSTRANSFORMATION() ;
   
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // This was left, because sometimes Gauss runing with breaks or restarting,
  // so it's needed to reinitialize the algorithm
  void startComputation( );
         
  // Runs until the end of transformation. It's long time algorithm
  void run();

  // Run for 'rowNum' rows, then suspends.
  void runRow(int rowNum);

  // Runs until a zero element appears on diagonal.
  void runUntilDiagHasZero();

  // Runs while diagonal elements are 1 or -1.
  // It's a very stupid name for such procedure, but i couldn't find something better.
  void runWhileDiagHasSingles();

  // returns the indexs of current diagonal element.
  int getCurrentRow() const;
  int getCurrentCol() const;
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool canChange( ) const;
  // You can change matrix only if GausTransformation is not in process.
  // Return true if you can change matrix.

  bool done( ) const;
  // True if computation was finished
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  Trichotomy isSingular( ) const ;

  Trichotomy isInvertible( ) const; 
  
  // Returns transformated matrix
  const MATRIX& getMatrix() const;
  // Returns transformation matrix
  const MATRIX& getTrMatrix()const;
  // Returns inverse transformation matrix
  const MATRIX& getInvTrMatrix()const;
  
  // Returns reference on transformated matrix
  MATRIX& refMatrix();
    bool isTransformed() const;
/*

private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool bDone; 
  bool bStart;
  bool transformed;
  Trichotomy isSingularMatrix;
  Trichotomy isInvertibleMatrix;

  Matrix<T> matrix;  // Main  matrix
  Matrix<T>* transMatrix; // Transformation matrix
  Matrix<T>* invTransMatrix; // Inverse transformation matrix
  int untilRow;  // Number of rows to run gaus transformation for
  int currentRow; // Current row of transformation
  int currentCol; // Current column of transformation
  bool buildTransformations; // True if transformation matrix is building
  bool buildInverseTransformations; // True if inverse transformation matrix is building

  // Constants for types of running a computations.
  static const int  UNTILNOT_0 = 0;
  static const int  UNTIL_1 = 1;
  static const int  RUNROW = 2;
  static const int  RUNALL = 3;
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Inner methods:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  void continueComputation( int whatDeal);

  void finishComputation( Trichotomy isSingular, Trichotomy isInvertible )
  {
    if (isSingularMatrix != yes)
        isSingularMatrix = isSingular;
    if (isInvertibleMatrix != no)
        isInvertibleMatrix = isInvertible;
    if (matrix.height()!=matrix.width())
        isSingularMatrix = no;
    bDone = true;
    bStart = false;
  }

  void addRow( int firstRow, int secondRow, T koef);
  // Adds elements of secondRow * koef to elements of firstRow

  void makeZero( int& row1, int& row2 );
  // Makes a zero in column

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

LISTGAUSSTRANSFORMATION(GaussTransformationInteger,MatrixInteger,Integer)
LISTGAUSSTRANSFORMATION(GaussTransformationRational,MatrixRational,Rational)
LISTGAUSSTRANSFORMATION(GaussTransformationdouble,Matrixdouble,double)

