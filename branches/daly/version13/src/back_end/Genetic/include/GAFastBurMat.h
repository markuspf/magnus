// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of classes BurauPoly, GAFastBurMat
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//


#ifndef _GAFastBurMat_H_
#define _GAFastBurMat_H_


#include "PMDebornoyWord.h"
#include "PMArray.h"
#include "GA.h"
#include "FreeGroup.h"
#include "Matrix.h"
#include "Polynomial.h"
#include "Int2.h"


// ----------------------------- BurauPoly --------------------------------- //


// to be used by GAFastBurMat only

class BurauPoly {

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  BurauPoly( int coef = 0 ); 

  BurauPoly( const BurauPoly& );

  BurauPoly( int pn, int* pc );

  ~BurauPoly( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int degree( ) const { return n; }

  bool zero( ) const { return n == 0 && c[0] == 0; }

  BurauPoly& operator = ( const BurauPoly& );

  void add( const BurauPoly& p );

  void sub( const BurauPoly& p );

  void multByX( );

  void multByIX( );
  
  /*
  bool operator == ( const Int2& i ) const { return v == i.v; }

  bool operator != ( const Int2& i ) const { return !(*this == i); }

  Int2 operator - ( ) const { return -v; }

  Int2 operator + ( const Int2& i ) const { return v + i.v; }

  Int2& operator += ( const Int2& i ) { v += i.v; return *this; }

  Int2 operator * ( const Int2& i ) const { return v * i.v; }

  Int2& operator *= ( const Int2& i ) { v *= i.v; return *this; }

  bool operator < ( const Int2& i ) const { return v < i.v; }

  bool operator > ( const Int2& i ) const { return v > i.v; }

  int value( ) const { return v; }
  */


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator << ( ostream& ostr, const BurauPoly& p )
  {
    p.printOn(ostr);
    return ostr;
  }

private:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void expandTo( int pn );

  void shrink( );

  void printOn( ostream& ostr ) const;
  
  void debugPrint( ostream& ostr ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
 
  int n;
  int* c;
};


// --------------------------- GAFastBurMat ------------------------------- //


class GAFastBurMat : public GA
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GAFastBurMat( const GAConfig& GAC, const FreeGroup& Br );
  
  ~GAFastBurMat( );
  
  // copy constructor and operator '=' are hidden (not implemented)


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void initPopulation( );

  int fitness( const PM* pm );
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Word getSolution( ostream* o = NULL );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // copy constructor and operator '=' are hidden (not implemented)
  
  GAFastBurMat( const GAFastBurMat& );

  GAFastBurMat& operator = ( const GAFastBurMat& );
  
  void init( );
    
  bool checkForSolution( Word& res );

  BurauPoly** productMatrix( const PM* pm );

  void destroyMatrix( BurauPoly** R );

  void printMatrix( BurauPoly** R );

  void penalizeFitness( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FreeGroup B;
  int numOfGens, n, g;
  bool keepDetails;
  ostream* out;
  PMDebornoyWordConfig PMDWC;
};


#endif
