// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of class BraidGroupToolkit
//
// Principal Author: Dmitry Bormotov
//
// Status: In development
//
// Usage:
//
// Discussion:
//
// Revision History:
//
// Next Implementation Steps:
//


#include "BraidGroupToolkit.h"


// ------------------------- BraidGroupToolkit ----------------------------- //


template <class R>
Polynomial<R> BraidGroupToolkit<R>::FoxDerivative( Word w, int var )
{
  Polynomial<R> result;
  int wLen = w.length();
  for( int i = 0; i < wLen; ++i ) {
    int g = ord(w[i]);
    if( abs(g) == var )
      if( g > 0 ) {
	Monomial<R> M = wordToMonomial(w.initialSegment(i));
	if( M.getCoefficient() != 0 )
	  result += M;
      }
      else {
	Monomial<R> M = wordToMonomial(w.initialSegment(i+1));
	if( M.getCoefficient() != 0 )
	  result -= M;
      }
  }
  return result;
}

template <class R>
Monomial<R> BraidGroupToolkit<R>::wordToMonomial( Word w )
{
  int wLen = w.length();
  if( wLen == 0 )
    return Monomial<R>((R)1);

  
  int numOfGens;
  for( int i = 0; i < wLen; ++i )
    if( abs( ord(w[i]) ) > numOfGens )
      numOfGens = abs( ord(w[i]) );

  int powers[numOfGens];
  for( int i = 0; i < numOfGens; ++i ) 
    powers[i] = 0;

  for( int i = 0; i < wLen; ++i ) {
    int g = ord(w[i]);
    if( g > 0 )
      powers[abs(g)-1] += 1;
    else 
      powers[abs(g)-1] -= 1;
  }

  int numOfVars = numOfGens;
  for( int i = numOfGens-1; i >= 0; --i ) 
    if( powers[i] != 0 )
      break;
    else
      --numOfVars;

  if( numOfVars == 0 )
    return Monomial<R>((R)0);
  else
    return Monomial<R>(1,numOfVars,powers);
}


template class BraidGroupToolkit<Integer>;
//template class BraidGroupToolkit<Rational>;





