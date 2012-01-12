// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of Gen2Config class
//
// Chars is a char* which knows its length, wrapped up as a user class.
//
// Principal Authors: Dmitry Bormotov, Roman Kuznets
//
// Status: in progress
//
// Revision History:
//


#ifndef _Gen2Config_H_
#define _Gen2Config_H_

#include "global.h"


//------------------------------ Gen2Config ----------------------------------//


class Gen2Config
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  Gen2Config( int pop = 50, int gen = -1, 
	      int breed = 20, int surg = 10, 
	      int crossF = 10, int save = 10,
	      bool fit = true );
  
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int populationSize( ) const { return population; }

  int numOfGenerations( ) const { return generations; }

  double fractionForBreeding( ) const { return double(breeding)/100; }
  // returns value betweeen 0 and 1
  
  double fractionForSurgery( ) const { return double(surgery)/100; }
  // returns value betweeen 0 and 1

  double fractionForCrossFert( ) const { return double(crossFert)/100; }
  // returns value betweeen 0 and 1

  double fractionForSaving( ) const { return double(saving)/100; }
  // returns value betweeen 0 and 1
  
  //  double fractionForGettingRid( ) const { return double(gettingRid)/100; }
  // returns value betweeen 0 and 1

  double fractionForNewBlood( ) const { return double(newBlood)/100; }
  // returns value betweeen 0 and 1

    
  //bool haveElitistSelection( ) const { return elitistSelection == 1; }

  //  bool haveStrongElitistSelection( ) const { return elitistSelection == 2; }

  bool haveFitnessScaling( ) const { return bFitnessScaling; }
  
  void setVariable( const char*, int );
  // used by readFrom() to set variables

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  // assignment operator supplied by compiler

  friend ostream& operator << ( ostream& ostr, const Gen2Config& C )
  {
    C.printOn(ostr);
    return ostr;
  }
    
  friend istream& operator >> ( istream& istr, Gen2Config& C )
  {
    C.readFrom(istr);
    return istr;
  }




private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void readFrom( istream& istr );

  void printOn( ostream& ostr ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  int population;
  int generations;
  int breeding;
  int surgery;
  int crossFert;
  int saving;
  //  int gettingRid;
  int newBlood;
  //  int elitistSelection;
  bool bFitnessScaling;
};

#endif
