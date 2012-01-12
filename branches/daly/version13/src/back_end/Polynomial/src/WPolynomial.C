// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of classes WMonomial, WPolynomial. 
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


#include "memory.h"
#include "WPolynomial.h"


//------------------------------- WMonomial ---------------------------------//


template <class R>
WMonomial<R>::WMonomial( const char *s )
{
  numberOfVariables = 0;
  istrstream istr(s);
  istr >> *this;
}


template <class R>
WMonomial<R>::WMonomial( R coef = 0, int numOfVars = 0,
		       int *powersOfVars = NULL ) :
    theCoefficient(coef), numberOfVariables(numOfVars)
{
#if SAFETY > 0
  if( numberOfVariables < 0 )
    error("WMonomial<R>::WMonomial( R, int, int * ) : "
	  "The number of variables can't be negative.");
//@db What about ( numberOfVariables > 0 && powersOfVars == NULL ) ?
#endif
  
  powersOfVariables = new int[numberOfVariables];
  memcpy( powersOfVariables, powersOfVars, numberOfVariables * sizeof(int) );
}


template <class R>
WMonomial<R>::WMonomial( const WMonomial& M ) 
  : theCoefficient(M.theCoefficient),
    numberOfVariables(M.numberOfVariables)
{
  powersOfVariables = new int[numberOfVariables];
  memcpy( powersOfVariables, M.powersOfVariables,
	  numberOfVariables * sizeof(int) );
}


template <class R>
WMonomial<R>& WMonomial<R>::operator = ( const WMonomial& M )
{
  if( numberOfVariables != M.numberOfVariables ) {
    if( numberOfVariables > 0 )
      delete [] powersOfVariables;
    numberOfVariables = M.numberOfVariables;
    powersOfVariables = new int[numberOfVariables];
  }

  theCoefficient = M.theCoefficient;
  memcpy( powersOfVariables, M.powersOfVariables,
	  numberOfVariables * sizeof(int) );

  return *this;
}


template <class R>
bool WMonomial<R>::operator == ( const WMonomial& M ) const
{
  if( numberOfVariables != M.numberOfVariables ||
      theCoefficient != M.theCoefficient )
    return false;
  for( int i = 0; i < numberOfVariables; i++ )
    if( powersOfVariables[i] != M.powersOfVariables[i] )
      return false;
  return true;

  //return compare(M) == 0;
}

template <class R>
WMonomial<R> WMonomial<R>::operator + ( const WMonomial& M ) const
{
  //@db Can there be the situation in Polynomial when we 
  // add two incompatible monomials? We don't have to worry about
  // other classes since Monomial is for the use in Polynomial only.

  WMonomial<R> result(*this);
  result.theCoefficient += M.theCoefficient;
  return result;
}

template <class R>
WMonomial<R>& WMonomial<R>::operator += ( const WMonomial& M )
{
  theCoefficient += M.theCoefficient;
  return *this;
}

template <class R>
WMonomial<R> WMonomial<R>::operator - () const
{
  WMonomial<R> result(*this);
  result.theCoefficient = - result.theCoefficient;
  return result;
}


template <class R>
WMonomial<R> WMonomial<R>::operator * ( const WMonomial& M ) const
{
  const WMonomial *M1 = this;
  const WMonomial *M2 = &M;
  if( M1->numberOfVariables < M2->numberOfVariables ) {
    const WMonomial *R = M1;
    M1 = M2;
    M2 = R;
  }

  WMonomial result(*M1);
  result.theCoefficient *= M2->theCoefficient;
  if( result.theCoefficient == 0 )
    return WMonomial(0);

  for( int i = 0; i < M2->numberOfVariables; ++i )
    result.powersOfVariables[i] += M2->powersOfVariables[i];

  // Because of the negative powers we need to check for zeros

  int numOfVars = result.numberOfVariables;
  if( !result.powersOfVariables[numOfVars-1] == 0 )
    return result;
  else {

    for( int i = numOfVars-1; i>=0; --i )
      if( result.powersOfVariables[i] != 0 ) 
	break;
      else
	--numOfVars;

    if( numOfVars == 0 )
      return WMonomial( result.theCoefficient );

    int powers[numOfVars];
    for( int i = 0; i < numOfVars; ++i )
      powers[i] = result.powersOfVariables[i];
    
    return WMonomial( result.theCoefficient, numOfVars, powers );
  }
}


template <class R>
int WMonomial<R>::compare( const WMonomial& M ) const
{
  if( numberOfVariables < M.numberOfVariables )
    return -1;
  else
    if( numberOfVariables > M.numberOfVariables )
      return 1;
  else 
    for( int i = 0; i < numberOfVariables; ++i ) {
      int num1 = powersOfVariables[i];
      int num2 = M.powersOfVariables[i];
      if( abs(num1) < abs(num2) )
	return -1;
      else
	if( abs(num1) > abs(num2) )
	  return 1;
	else
	  if( num1 < 0 && num2 > 0 )
	    return -1;
	  else
	    if( num1 > 0 && num2 < 0 )
	      return 1;
     }

  return 0;
}


template <class R>
void WMonomial<R>::printOn(ostream& ostr) const
{
  if( theCoefficient == -1)
    if( numberOfVariables == 0)
      ostr << " - 1";
    else 
      ostr << " -";
  else
    if( theCoefficient != 1 || numberOfVariables == 0)
      ostr << " " << theCoefficient;
  
  for( int i = 0; i < numberOfVariables; ++i ) {
    int power = powersOfVariables[i];
    if( power > 0 ) {
      ostr << " x" << i+1;
      if( power != 1 )
	ostr << '^' << power;
    }
  }
}

template <class R>
int WMonomial<R>::getPowerOfVariable( int i ) const
{
  if( i < 0 || i >= numberOfVariables )
    error("int WMonomial<R>::getPowerOfVariable( int i ) const : "
	  "the index i is out of range");
  return powersOfVariables[i];
}

template <class R>
void WMonomial<R>::setPowerOfVariable( int i, int v )
{
  if( i < 0 || i >= numberOfVariables )
    error("int WMonomial<R>::setPowerOfVariable( ... ) : "
	  "the index i is out of range");
  powersOfVariables[i] = v;
}



//------------------------------ WPolynomial --------------------------------//


template <class R>
WPolynomial<R>::WPolynomial( const WPolynomial& P ) 
  : PBTree< WMonomial<R>, WMonomial<R> > ( )
{
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(P);
  for( ; !I.done(); I.next() )
    this->insert(I.getValue(), I.getValue());
}


template <class R>
WPolynomial<R>::WPolynomial( const WMonomial<R>& M ) 
  : PBTree< WMonomial<R>, WMonomial<R> > ( )
{
  this->insert(M,M);
}


template <class R>
WPolynomial<R>::WPolynomial( const char *s )
  : PBTree< WMonomial<R>, WMonomial<R> > ( )
{
  int sLen = strlen(s);
  char t[sLen+2];
  for( int i = 0; i < sLen; ++i )
    t[i] = s[i];
  t[sLen] = ',';
  t[sLen+1] = 0;
  
  istrstream istr(t);
  istr >> *this;
}


template <class R>
WPolynomial<R>& WPolynomial<R>::operator = ( const WPolynomial& P )
{
  deleteAll();
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(P);
  for( ; !I.done(); I.next() ) {
    //cout << "New WMonomial: " << I.getValue() << endl;
    this->insert(I.getValue(), I.getValue());
    //cout << "The tree: " << endl;
    //printAll();
  }
  return *this;
}

template <class R>
bool WPolynomial<R>::operator == ( const WPolynomial& P ) const
{
  WPolynomial result( *this - P );
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(result);
  return I.done();
}

template <class R>
WPolynomial<R>& WPolynomial<R>::operator += ( const WPolynomial& P )
{
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(P);
  for( ; !I.done(); I.next() )
    this->insert(I.getValue(), I.getValue());
  
  return *this;
}


template <class R>
WPolynomial<R>& WPolynomial<R>::operator -= ( const WPolynomial& P )
{
  (*this) += -P;
  return *this;
}


template <class R>
WPolynomial<R>& WPolynomial<R>::operator *= ( const WPolynomial& P )
{
  WPolynomial<R> result = (*this) * P;
  *this = result;
  return *this;
}


template <class R>
WPolynomial<R> WPolynomial<R>::operator + (const WPolynomial& P) const
{
  WPolynomial<R> result(*this);
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(P);
  for( ; !I.done(); I.next() )
    result.insert(I.getValue(), I.getValue());
 
  return result;
}


template <class R>
WPolynomial<R> WPolynomial<R>::operator - () const
{
  WPolynomial<R> result;
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(*this);
  for( ; !I.done(); I.next() )
    result.insert(-I.getValue(), -I.getValue());

  return result;
}


template <class R>
WPolynomial<R> WPolynomial<R>::operator - (const WPolynomial& P) const
{
  return (*this) + (-P);
}


template <class R>
WPolynomial<R> WPolynomial<R>::operator * (const WPolynomial& P) const
{
  WPolynomial<R> result;
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(*this);
  PBTreeIterator< WMonomial<R>, WMonomial<R> > J(P);
  for( ; !I.done(); I.next() )
    for( J.reset(); !J.done(); J.next() ) {
      //cout << "New WMonomial: " << I.getValue() * J.getValue() << endl;
      result.insert( I.getValue() * J.getValue(), 
		     I.getValue() * J.getValue() );
      //cout << "The tree: " << endl;
      //result.printAll();
      //cout << "The tree has been printed" << endl;
    }
  return result;
}


template <class R>
void WPolynomial<R>::printOn(ostream& ostr) const
{
  bool bFirst = true;
  PBTreeIterator< WMonomial<R>, WMonomial<R> > I(*this);
  if( I.done() ) {
    cout << "0" << endl;
    return;
  }
  for( ; !I.done(); I.next() ) {
    if( !bFirst && I.getValue().theCoefficient > 0 )
      cout << "+";
    cout << I.getValue() << " ";
    bFirst = false;
  }
  cout << endl;
}


template class WMonomial<Integer>;
template class WMonomial<Rational>;
template class WPolynomial<Integer>;
template class WPolynomial<Rational>;





