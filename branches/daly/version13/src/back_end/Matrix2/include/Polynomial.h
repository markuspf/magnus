// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definitions of classes Monomial, Polynomial.
//
// Principal Author: Dmitry Bormotov
//
// Status: In development
//
// Usage:
//
// Revision History:
//


#ifndef _POLYNOMIAL_H_
#define _POLYNOMIAL_H_

#include "Integer.h"
#include "Rational.h"
#include "RingParser.h"
#include "IStreamPoll.h"
#include "BTree.h"


//-------------------------------- Monomial ---------------------------------//


template <class R> class Monomial
{

  friend class Polynomial<R>;

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Monomial( const char * );

  Monomial( R coef = 0, int numOfVars = 0, int *powersOfVars = NULL );

  Monomial( const Monomial& );

  ~Monomial( ) 
  {     
    if( numberOfVariables > 0 )
      delete [] powersOfVariables; 
  }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Operators:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool operator == ( const Monomial& ) const;

  Monomial& operator = ( const Monomial& );

  Monomial operator - () const;

  Monomial operator * ( const Monomial& ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int compare( const Monomial& ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator << ( ostream& ostr, const Monomial& M )
  {
    M.printOn(ostr);
    return ostr;
  }

  friend IStreamPoll operator >> ( istream& istr, Monomial& M )
  {
    Chars errMesg;
    M = M.readFrom(istr, errMesg);
    return IStreamPoll( istr, errMesg );
  }


private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  R coefficient;
  int numberOfVariables;
  int *powersOfVariables;


  //////////////////////////////////////////////////////////////
  //                                                          //
  // Private functions:                                       //
  //                                                          //
  //////////////////////////////////////////////////////////////

  void printOn( ostream& ) const;

  Monomial readFrom( istream& istr, Chars& errMesg )
  {
    MonomialParser<R> P(istr);
    return P.parseMonomial(errMesg);
  }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  friend int main( );

#endif

};


//------------------------------- Polynomial --------------------------------//


template <class R> 
class Polynomial : public BTree< Monomial<R> >
{

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Polynomial( ) : BTree< Monomial<R> >( ) { }  

  Polynomial( const char*);

  Polynomial( const Monomial<R>& );

  Polynomial( const Polynomial& );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Operators:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool operator == ( const Polynomial& ) const;

  Polynomial& operator = ( const Polynomial& );

  Polynomial& operator += ( const Polynomial& );

  Polynomial& operator -= ( const Polynomial& );

  Polynomial& operator *= ( const Polynomial& );

  Polynomial operator + ( const Polynomial& ) const; 

  Polynomial operator - ( const Polynomial& ) const; 

  Polynomial operator * ( const Polynomial& ) const; 

  Polynomial operator - ( ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool isIdentity( ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator << ( ostream& ostr, const Polynomial& P )
  {
    P.printOn(ostr);
    return ostr;
  }

  friend IStreamPoll operator >> ( istream& istr, Polynomial& P )
  {
    Chars errMesg;
    P = P.readFrom(istr, errMesg);
    return IStreamPoll( istr, errMesg );
  }

private:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void printOn( ostream& ) const;

  Polynomial readFrom( istream& istr, Chars& errMesg )
  {
    PolynomialParser<R> P(istr);
    return P.parsePolynomial(errMesg);
  }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Debugging stuff:                                                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

  friend int main( );

#endif

};

#endif



