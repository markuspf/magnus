%module Magnus

%{
#include "GroupFastChecks.h"
%}


class GroupFastChecks {

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  GroupFastChecks( const FPGroup& G );

  // Default & copy contructors, operator = provided by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool hasTrivialPresentation( );
  // True if the entered presentation has form < ; >.


  bool numOfRelsLessNumOfGens( );
  // True if the number of relators is less than the number of generators.

  bool existsGenWithExpSumZeroInEveryRelator( Generator& );
  // True if there's such generator exponent sum of which is equal to
  // zero in every relator. The generator'll be return as well.

  VectorOf<int> getExpSumOfGen( const Generator& g);
  // Return exponent sums of g in the group relators.

  int GCDOfExpSumOfGen( const Generator& g );
  // Return the GCD of exponent sums of g in the group relators.

/*
private:
 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  FPGroup theGroup;     // the group to work with
  int numOfGens;        // number of generators of the group
  SetOf<Word> relators; // group relators 
*/
};

