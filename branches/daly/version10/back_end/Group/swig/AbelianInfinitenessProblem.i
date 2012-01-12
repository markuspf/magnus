%module Magnus

%{
#include "AbelianInfinitenessProblem.h"
%}


class AbelianInfinitenessProblem 
{

public:

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  AbelianInfinitenessProblem( const FPGroup& G);

  ~AbelianInfinitenessProblem( ) ;
      

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Activation members:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void startComputation( );
  // Start the computation. 
  // You shouldn't call this more than one time.
             
  bool continueComputation( ) ;
  // Advance the computation. 
  // You have to call startComputation() before.
  // This function returns true if the computation is done.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Status Queries:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool done( ) const;
  // true only when the computation is finished.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // You can call all these functions iff the computation is finished
  // ( when the done() functions returns true ).

  bool isInfinite( ) ;
  /*
private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool bDone;
  bool bStart;
  bool itIsInfinite;
  Matrix<Rational> *matrix;
  int width;
  int height;
  const FPGroup theGroup;
  GaussTransformation<Rational> *GT;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private methods:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  AbelianInfinitenessProblem( const AbelianInfinitenessProblem& );
  // It is hidden, not implemented.

  AbelianInfinitenessProblem& operator = ( const AbelianInfinitenessProblem& );
  // It is hidden, not implemented.

  void finishComputation( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Debugging stuff:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG
  //friend int main( );
#endif
*/
};
