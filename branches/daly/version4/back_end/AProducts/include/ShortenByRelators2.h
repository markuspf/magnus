// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of the ShortenByRelators2 class
//
//
// Principal Author: Dmitry Pechkin
//
// Status: in progress
//
// Usage:
//

#ifndef _SHORTEN2_BY_RELATORS_H_
#define _SHORTEN2_BY_RELATORS_H_

#include "SymmetricRelators.h"
#include "QuickAssociations.h"
#include "AP-fixups.h"

class ShortenByRelators2 {

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  // No default constructor
  // Copy constructor provided by compiler (does logical deep copy).
  
  ShortenByRelators2 ( const SetOf<Word>& relators );
  // To construct a ShortenByRelators2 class of given relators.

  ///////////////////////////////////////////////////////
  //                                                   //
  // Accessors:                                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  Word getShortenWord( const Word& w ) const;  
  // Return shorten form of word w.

  Word expressWordInConjugatesOfRelators( const Word& w, 
    ProductOfRelatorConjugates& productOfRelatorConjugates ) const;
  // Return shorten form of word w. 
  // Shortened part (=1) of w is r_0^c_0 * r_1^c_1 * .. * r_n^c_n,
  // where r_i are given relators and c_i some words in a free group generated by
  // generators of w.

private:

  virtual int compare( const Word& w1, const Word& w2 ) const;
  // returns -1 if w1 is lesser than w2 in some order,
  // returns 1 if w2 is greater than w2 in the order,
  // otherwise returns 0.

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Data members:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  QuickAssociationsOf< Word, int > relatorsPieces;
  VectorOf<Word> theRelators;
  VectorOf<int> relatorsLengths;
  int base;
};


#endif // _SHORTEN2_BY_RELATORS_H_
