%module Magnus

%{
#include "ORProblems.h"

%}

class ORProblems {

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  ORProblems( int numOfGens, const Word& relator ) ;
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool isTrivial( ) const;

  bool isAbelian( ) const;

  bool isFinite( ) const;

  bool isFree( ) const;

  int order( ) const;
/*
  
private:
 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int numberOfGenerators;

  Word theRelator;
*/
};
/*

inline bool ORProblems::isFinite( ) const
{
  return numberOfGenerators == 1;
}


inline int ORProblems::order( ) const
{
  if( isFinite() )
    return theRelator.length();
  else
    return 0;
}

#endif

*/