// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of class GAMatrixBurau
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//


#ifndef _GAMatrixBurau_H_
#define _GAMatrixBurau_H_


#include "PMDebornoyWord.h"
#include "PMArray.h"
#include "GA.h"
#include "FreeGroup.h"
#include "Matrix.h"
#include "Polynomial.h"
#include "Int2.h"


// --------------------------- GAMatrixBurau ------------------------------- //


class GAMatrixBurau : public GA
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GAMatrixBurau( const GAConfig& GAC, const FreeGroup& Br );
  
  ~GAMatrixBurau( );
  
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
  
  GAMatrixBurau( const GAMatrixBurau& );

  GAMatrixBurau& operator = ( const GAMatrixBurau& );
  
  void init( );
    
  bool checkForSolution( Word& res );

  Matrix< Polynomial<Int2> > productMatrix( const PM* pm );

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
  Matrix< Polynomial<Int2> > *bgens, *bgensI, I;
  PMDebornoyWordConfig PMDWC;
};


#endif
