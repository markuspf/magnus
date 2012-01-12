// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of classes PMPolyConfig, PMPolynomial
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//

#ifndef _PMPolynomial_H_
#define _PMPolynomial_H_


#include "Int2.h"
#include "PM.h"
#include "RandomNumbers.h"
#include "Polynomial.h"


// ----------------------------- PMPolyConfig ------------------------------ //


class PMPolyConfig
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PMPolyConfig( UniformRandom& R,  
		int MaxCoef, 
		int MaxPower,
		int NumOfVars,
		int MaxNumOfMonoms
		) 
    : r( R ), 
      maxCoef( MaxCoef ), 
      maxPower( MaxPower ), 
      numOfVars( NumOfVars ), 
      maxNumOfMonoms( MaxNumOfMonoms )
    { }
  
  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  UniformRandom& rng( ) const { return r; }

  int getMaxCoef( ) const { return maxCoef; }
 
  int getMaxPower( ) const { return maxPower; }

  int getNumOfVars( ) const { return numOfVars; }
 
  int getMaxNumOfMonoms( ) const { return maxNumOfMonoms; }


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  UniformRandom& r;
  int maxCoef, maxPower, numOfVars, maxNumOfMonoms;
};


// ----------------------------- PMPolynomial ------------------------------ //


class PMPolynomial : public PM 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PMPolynomial( const PMPolyConfig& CFG, 
		Polynomial<Int2> p = Polynomial<Int2>() ) 
    : cfg( CFG ), poly( p ) { }

  // copy constructor, destructor and operator = supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* clone( ) const { return new PMPolynomial(*this); }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden hook functions:                                           //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* crossover( const PM* ) { return clone(); }

  PM* mutate( );

  PM* randomPM( ) { }
  // no implementation

  void printOn( ostream& ostr ) const { ostr << poly; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Polynomial<Int2> getPolynomial( ) const { return poly; }
  
  const PMPolyConfig& getCfg( ) { return cfg; }

  bool operator == ( const PM& pm ) const 
    { return poly == ((PMPolynomial&)pm).poly; }
  

private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Monomial<Int2> randomMonomial( int maxCoef, int maxNumOfVars, 
				 int maxPower, int absentVar );

  Polynomial<Int2> randomPolynomial( int maxNumOfMonoms, int absentVar );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const PMPolyConfig& cfg;
  Polynomial<Int2> poly;
};

#endif
