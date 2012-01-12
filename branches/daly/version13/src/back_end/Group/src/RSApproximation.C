// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class RSApproximation
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// Special Notes:
//


#include <values.h>
#include "RSApproximation.h"
#include "conversions.h"

//#define debug_RSApproximation


// --------------------------- RSApproximation ----------------------------- //


RSApproximation::RSApproximation( const FPGroup& G )
  : theGroup( G ), F( G.namesOfGenerators() )
{
  SetOf<Word> S = G.getRelators();

#ifdef debug_RSApproximation
  cout << "Original relators: ";
  G.printSetOfWords(cout,S);
  cout << endl << endl;
#endif

  theRels = VectorOf<RSWord>( S.cardinality() );
  min = VectorOf<int>( S.cardinality() );
  max = VectorOf<int>( S.cardinality() );

  SetIterator<Word> I(S);
  int i = -1;

  for( ; !I.done(); I.next() ) {
    theRels[++i] = RSWord ( F, I.value() );

#ifdef debug_RSApproximation
    if( i == 0 ) 
      cout << "Relators in the rewritten form: " << endl << endl;
    cout << theRels[i] << endl;
#endif
  }

  // compute min and max indices for every relation
  
  for( i = 0; i < theRels.length(); ++i ) {

    min[i] = MAXINT; max[i] = -MAXINT;

    for( int j = 0; j < theRels[i].length(); ++j ) {
      if( theRels[i][j].index < min[i] )
	min[i] = theRels[i][j].index;
      if( theRels[i][j].index > max[i] )
	max[i] = theRels[i][j].index;
    }
  }
}


SetOf<RSWord> RSApproximation::getApproximation( int l ) const
{
  SetOf<RSWord> res;

  for( int i = 0; i < theRels.length(); ++i ) {

    int off = -l - min[i]; 
    for( int j = 0; j <= 2 * l - max[i] + min[i]; ++j ) {

      RSWord RSW(F, theRels[i]);
      for( int k = 0; k < RSW.length(); ++k )
	RSW[k].index += off + j;
      res |= RSW;
    }
  }
  
  return res;

  /*
  {
    SetOf<RSWord> res;
    for( int i = 0; i < theRels.length(); ++i )
      res |= theRels[i];
    return res;
  }
  */
}
