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


#ifndef _Gen4Config_H_
#define _Gen4Config_H_

#include "global.h"


//------------------------------ Gen4Config ----------------------------------//


class Gen4Config
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  

  Gen4Config( float dihSym = 50, 
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

  float dihSymmFraction( ) const { return float(dihSym)/100; }
  // returns value between 0 and 1

  float fractionForBreeding( ) const { return float(breeding)/100; }
  // returns value betweeen 0 and 1
  
  float fractionForSurgery( ) const { return float(surgery)/100; }
  // returns value betweeen 0 and 1

  float fractionForCrossFert( ) const { return float(crossFert)/100; }
  // returns value betweeen 0 and 1

  float fractionForSaving( ) const { return float(saving)/100; }
  // returns value betweeen 0 and 1
  
  float fractionForNewBlood( ) const { return float(newBlood)/100; }
  // returns value betweeen 0 and 1

  bool haveFitnessScaling( ) const { return bFitnessScaling; }
  
  void setVariable( const char*, int );
  // used by readFrom() to set variables

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  // assignment operator supplied by compiler

  friend ostream& operator << ( ostream& ostr, const Gen4Config& C )
  {
    C.printOn(ostr);
    return ostr;
  }
    
  friend istream& operator >> ( istream& istr, Gen4Config& C )
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
  
  float dihSym;
  int breeding;
  int surgery;
  int crossFert;
  int saving;
  int newBlood;
  bool bFitnessScaling;
};

#endif
