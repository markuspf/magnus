%module Magnus

%{
#include "RandomPrimitiveElement.h"
%}


class RandomPrimitiveElement
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  RandomPrimitiveElement( const FreeGroup& F, int seed );
  // Giving the same `seed' you obtain the same sequence of automorphisms.

  ~RandomPrimitiveElement( );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

//  Word getElement( int avgNumGens );
  // `avgNumGens' gives the average number of Whitehead automorphisms to apply.
/*
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  FreeGroup theGroup;

  int numberOfGroupGens;

  NormalRandom numGensPicker;
  UniformRandom typeGenPicker;
*/
};

