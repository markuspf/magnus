// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of class GANagata
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//


#ifndef _GANagata_H_
#define _GANagata_H_


//#include "PMDebornoyWord.h"
//#include "PMArray.h"
#include "GA.h"
#include "FreeGroup.h"
//#include "Matrix.h"
#include "PMPolynomial.h"
//#include "Int2.h"
#include "GAConfig.h"


// -------------------------- GANagataConfig ------------------------------- //


class GANagataConfig : public GAConfig
{
public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  GANagataConfig( int popSize = 50, 
		  int numOfGens = -1, 
		  int crossChance = 100, 
		  int mutChance = 100, 
		  ElitistSelectionType elitType = GAConfig::SIMPLE, 
		  bool fitScaling = true, 
		  int Report = 1, 
		  int FitnessPrint = 10,
		  int PopulationPrint = 100, 
		  int NumberOfVariables = 3,
		  Polynomial<Int2> aPolynomial 
		    = Polynomial<Int2>("x1 + x2^2,"), 
		  int MaximalCoefficient = 2, 
		  int MaximalPower = 5,
		  int MaximalNumberOfMonomials = 1 
		  );
 
  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Hook functions:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual bool setUserVariable( Chars varName, Chars value );
 

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int numberOfVariables( ) const { return theNumberOfVariables; }

  Polynomial<Int2> polynomial( ) const { return thePolynomial; }
  
  int maximalCoefficient( ) const { return theMaximalCoefficient; }

  int maximalPower( ) const { return theMaximalPower; }

  int maximalNumberOfMonomials( ) const { return theMaximalNumberOfMonomials; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
 
  void printOn( ostream& ostr ) const;


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
   
  int theNumberOfVariables;
  Polynomial<Int2> thePolynomial;
  int theMaximalCoefficient;
  int theMaximalPower;
  int theMaximalNumberOfMonomials;
};


// ----------------------------- GANagata --------------------------------- //


class GANagata : public GA
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GANagata( const GANagataConfig& GAC );
  
  ~GANagata( );
  
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

  Polynomial<Int2> getSolution( ostream* o = NULL );


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
  
  GANagata( const GANagata& );

  GANagata& operator = ( const GANagata& );
  
  void init( );
    
  bool checkForSolution( Polynomial<Int2>& res );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  ostream* out;
  PMPolyConfig PMPC;
  int g;
};


#endif
