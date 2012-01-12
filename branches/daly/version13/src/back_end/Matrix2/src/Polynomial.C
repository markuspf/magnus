// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of classes Monomial, Polynomial. 
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
#include "Polynomial.h"


//-------------------------------- Monomial ---------------------------------//


template <class R>
Monomial<R>::Monomial( const char *s )
{
  numberOfVariables = 0;
  istrstream istr(s);
  istr >> *this;
}


template <class R>
Monomial<R>::Monomial( R coef = 0, int numOfVars = 0,
		       int *powersOfVars = NULL ) :
    coefficient(coef), numberOfVariables(numOfVars)
{
#if SAFETY > 0
  if( numberOfVariables < 0 )
    error("Monomial<R>::Monomial( R, int, int * ) : "
	  "The number of variables can't be negative.");
//@db What about ( numberOfVariables > 0 && powersOfVars == NULL ) ?
#endif
  
  powersOfVariables = new int[numberOfVariables];
  memcpy( powersOfVariables, powersOfVars, numberOfVariables * sizeof(int) );
}


template <class R>
Monomial<R>::Monomial( const Monomial& M ) : coefficient(M.coefficient),
    numberOfVariables(M.numberOfVariables)
{
  powersOfVariables = new int[numberOfVariables];
  memcpy( powersOfVariables, M.powersOfVariables,
	  numberOfVariables * sizeof(int) );
}


template <class R>
Monomial<R>& Monomial<R>::operator = ( const Monomial& M )
{
  if( numberOfVariables != M.numberOfVariables ) {
    if( numberOfVariables > 0 )
      delete [] powersOfVariables;
    numberOfVariables = M.numberOfVariables;
    powersOfVariables = new int[numberOfVariables];
  }

  coefficient = M.coefficient;
  memcpy( powersOfVariables, M.powersOfVariables,
	  numberOfVariables * sizeof(int) );

  return *this;
}


template <class R>
bool Monomial<R>::operator == ( const Monomial& M ) const
{
  if( numberOfVariables != M.numberOfVariables ||
      coefficient != M.coefficient )
    return false;
  for( int i = 0; i < numberOfVariables; i++ )
    if( powersOfVariables[i] != M.powersOfVariables[i] )
      return false;
  return true;
}


template <class R>
Monomial<R> Monomial<R>::operator - () const
{
  Monomial<R> result(*this);
  result.coefficient = - result.coefficient;
  return result;
}


template <class R>
Monomial<R> Monomial<R>::operator * ( const Monomial& M ) const
{
  const Monomial *M1 = this;
  const Monomial *M2 = &M;
  if( M1->numberOfVariables < M2->numberOfVariables ) {
    const Monomial *R = M1;
    M1 = M2;
    M2 = R;
  }

  Monomial result(*M1);
  result.coefficient *= M2->coefficient;
  for( int i = 0; i < M2->numberOfVariables; ++i )
    result.powersOfVariables[i] += M2->powersOfVariables[i];

  return result;
}


template <class R>
int Monomial<R>::compare( const Monomial& M ) const
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
      if( num1 < num2 )
	return -1;
      else
	if( num1 > num2 )
	  return 1;
    }

  return 0;
}


template <class R>
void Monomial<R>::printOn(ostream& ostr) const
{
  if( coefficient == -1)
    if( numberOfVariables == 0)
      ostr << " - 1";
    else 
      ostr << " -";
  else
    if( coefficient != 1 || numberOfVariables == 0)
      ostr << " " << coefficient;
  
  for( int i = 0; i < numberOfVariables; ++i ) {
    int power = powersOfVariables[i];
    if( power > 0 ) {
      ostr << " x" << i+1;
      if( power != 1 )
	ostr << '^' << power;
    }
  }
}


//------------------------------- Polynomial --------------------------------//


template <class R>
Polynomial<R>::Polynomial( const Polynomial& P ) : BTree< Monomial<R> > ( )
{
  BTreeIterator< Monomial<R> > I(P);
  for( ; !I.done(); I.next() )
    this->insert(I.value());
}


template <class R>
Polynomial<R>::Polynomial( const Monomial<R>& M ) : BTree< Monomial<R> > ( )
{
  this->insert(M);
}


template <class R>
Polynomial<R>::Polynomial( const char *s )
{
  istrstream istr(s);
  istr >> *this;
}


template <class R>
Polynomial<R>& Polynomial<R>::operator = ( const Polynomial& P )
{
  deleteAll();
  BTreeIterator< Monomial<R> > I(P);
  for( ; !I.done(); I.next() )
    this->insert(I.value());
  return *this;
}


template <class R>
bool Polynomial<R>::operator == ( const Polynomial& P ) const
{
  //@db ?????
}

template <class R>
Polynomial<R>& Polynomial<R>::operator += ( const Polynomial& P )
{
  BTreeIterator< Monomial<R> > I(P);
  for( ; !I.done(); I.next() )
    this->insert(I.value());
  return *this;
}


template <class R>
Polynomial<R>& Polynomial<R>::operator -= ( const Polynomial& P )
{
  (*this) += -P;
}


template <class R>
Polynomial<R>& Polynomial<R>::operator *= ( const Polynomial& P )
{
  Polynomial<R> result = (*this) * P;
  *this = result;
}


template <class R>
Polynomial<R> Polynomial<R>::operator + (const Polynomial& P) const
{
  Polynomial<R> result(*this);
  BTreeIterator< Monomial<R> > I(P);
  for( ; !I.done(); I.next() )
    result.insert(I.value());
  
  return result;
}


template <class R>
Polynomial<R> Polynomial<R>::operator - () const
{
  Polynomial<R> result;
  BTreeIterator< Monomial<R> > I(*this);
  for( ; !I.done(); I.next() )
    result.insert(-I.value());
}


template <class R>
Polynomial<R> Polynomial<R>::operator - (const Polynomial& P) const
{
  return (*this) + (-P);
}


template <class R>
Polynomial<R> Polynomial<R>::operator * (const Polynomial& P) const
{
  Polynomial<R> result;
  BTreeIterator< Monomial<R> > I(*this);
  BTreeIterator< Monomial<R> > J(P);
  for( ; !I.done(); I.next() )
    for( ; !J.done(); J.next() )
      result.insert( I.value() * J.value() );
  return result;
}


template <class R>
void Polynomial<R>::printOn(ostream& ostr) const
{
  bool bFirst = true;
  BTreeIterator< Monomial<R> > I(*this);
  for( ; !I.done(); I.next() ) {
    if( !bFirst && I.value().coefficient > 0 )
      cout << "+";
    cout << I.value() << " ";
    bFirst = false;
  }
  cout << endl;
}


template class Monomial<Integer>;
template class Monomial<Rational>;
template class Polynomial<Integer>;
template class Polynomial<Rational>;

