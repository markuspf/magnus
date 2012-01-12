// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of class GAConfig
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
//    GAConfig reads commonly used parameters for genetic algorithm
// from a file and provides interface for accessing their values. Additional 
// parameters can be specified by subclassing GAConfig and overriding hook 
// function setUserVariable(). GAConfig has no abstract functions. Possible
// variables(parameters) and their values are listed in test/ga.cfg .
//
// Revision History:
//

#ifndef _GAConfig_H_
#define _GAConfig_H_

#include "Chars.h"


// ------------------------------ GAConfig --------------------------------- //


class GAConfig
{
public:

  enum ElitistSelectionType { SIMPLE, STRONG, NONE }; 

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GAConfig( int popSize = 50, 
	    int numOfGens = -1, 
	    int crossChance = 100, 
	    int mutChance = 100, 
	    ElitistSelectionType elitType = SIMPLE, 
	    bool fitScaling = true,
	    int Report = 1,
	    int FitnessPrint = 10,
	    int PopulationPrint = 100
	    );
  
  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Hook functions:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual bool setUserVariable( Chars varName, Chars value ) {  }
  // to be overriden in subclasses in order to accept additional
  // parameters; must return 'true' if successful and 'false' if either
  // 'varName' or 'value' are illegal


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int populationSize( ) const { return population; }

  int numOfGenerations( ) const { return generations; }

  double chanceOfCrossover( ) const { return double(crossover)/100; }
  // returns value betweeen 0 and 1
  
  double chanceOfMutation( ) const { return double(mutation)/100; }
  // returns value betweeen 0 and 1
  
  ElitistSelectionType elitistSelectionType( ) const 
    { return elitistSelection; }

  bool haveFitnessScaling( ) const { return bFitnessScaling; }

  int report( ) const { return theReport; }

  int fitnessPrint( ) const { return theFitnessPrint; }

  int populationPrint( ) const { return thePopulationPrint; }
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  friend ostream& operator << ( ostream& ostr, const GAConfig& C )
  {
    C.printOn(ostr);
    return ostr;
  }
    
  friend istream& operator >> ( istream& istr, GAConfig& C )
  {
    C.readFrom(istr);
    return istr;
  }

  virtual void readFrom( istream& istr );

  virtual void printOn( ostream& ostr ) const;


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void setVariable( Chars varName, Chars value );
  // used by readFrom() to set variables

  void eatWhite( char* input );
  // kills spaces in 'input'


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  int population;                         // population size
  int generations;                        // number of generations
  int crossover;                          // chance of crossover
  int mutation;                           // chance of mutation
  ElitistSelectionType elitistSelection;  // type of elitist selection
  bool bFitnessScaling;                   // fitness scaling: yes/no
  int theReport;                             // report every ... generations
  int theFitnessPrint;    // print all fitness values every ... generations
  int thePopulationPrint; // print the population every ... generations
};

#endif
