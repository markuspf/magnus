%module Magnus

%{
#include "SymmetricRelators.h"
%}

class SymmetricRelators {

//friend class SymmetricRelatorsIterator;

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  // No default constructor.

  // Copy constructor, operator=, and destructor supplied by compiler.
  
  SymmetricRelators ( const SetOf<Word>& relators );

  ~SymmetricRelators ( );


  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors:                                       //
  //                                                   //
  ///////////////////////////////////////////////////////

  int cardinality( ) const;

  const VectorOf<Word>& getRelators( );
/*
private:
  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Data members:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  VectorOf<Word> symmetricRelators;
  int *rootLengths;
*/  
};

