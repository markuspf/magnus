// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of class GA
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//

#ifndef _GA_H_
#define _GA_H_


#include "RandomNumbers.h"
#include "GAConfig.h"
#include "PM.h"


// ---------------------------------- GA ----------------------------------- //


class GA
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GA( const GAConfig& GAC );

  GA::~GA( );

  // copy constructor and operator '=' are hidden (not implemented)


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Abstract functions:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual void initPopulation( ) = 0;
  // must be called before run(), must initialize the population
  // with instances of *aPM, where aPM is a class that is derived from PM

  virtual int fitness( const PM* pm ) = 0;
  // fitness function


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Hook functions:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void computeFitness( );

  void adjustFitness( );

  void doCrossover( );

  void doMutation( );

  void doReplacement( );


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Abstract private functions:                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Other private functions:                                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // copy constructor and operator '=' are hidden (not implemented)
  
  GA( const GA& );

  GA& operator = ( const GA& );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

protected:  

  const GAConfig& gac;
  PM **pop, **newPop;
  int popSize;
  int *fit, *copyFit;
  int minFit, maxFit, minFitInd;
  bool bFirstGen;
  UniformRandom r;

private:
  
};

#endif
