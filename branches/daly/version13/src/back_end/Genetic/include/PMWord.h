// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of PMWord class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//

#ifndef _PMWord_H_
#define _PMWord_H_


#include "Word.h"
#include "PM.h"
#include "RandomNumbers.h"


// ----------------------------- PMWordConfig ------------------------------ //


class PMWordConfig 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PMWordConfig( UniformRandom& R,  
		int NumOfGens, 
		int WordMaxLen,
		bool allowTrivialWords = true
		) 
    : r( R ), 
      numOfGens( NumOfGens ), 
      wordMaxLen( WordMaxLen ),
      bAllowTrivialWords( allowTrivialWords )
    { }

  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  UniformRandom& rng( ) const { return r; }

  int getNumOfGens( ) const { return numOfGens; }
 
  int getWordMaxLen( ) const { return wordMaxLen; }

  bool allowTrivialWords( ) const { return bAllowTrivialWords; }

 
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
  int numOfGens, wordMaxLen;
  bool bAllowTrivialWords;
};


// ------------------------------- PMWord ---------------------------------- //


class PMWord : public PM 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PMWord( const PMWordConfig& CFG, Word w = Word() ) 
    : cfg( CFG ), theWord( w ) { }

  // copy constructor, destructor and operator = supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* clone( ) const { return new PMWord(*this); }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden hook functions:                                           //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* crossover( const PM* );

  PM* mutate( );

  PM* randomPM( ); 

  void printOn( ostream& ostr ) const { ostr << theWord; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Word getWord( ) const { return theWord; }
  
  const PMWordConfig& getCfg( ) { return cfg; }

  bool operator == ( const PM& pm ) const 
  { return theWord == ((PMWord&)pm).theWord; }


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int randomGen( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const PMWordConfig& cfg;
  Word theWord;
};

#endif
