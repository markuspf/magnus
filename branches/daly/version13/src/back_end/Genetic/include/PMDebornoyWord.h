// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of PMDebornoyWord class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//

#ifndef _PMDebornoyWord_H_
#define _PMDebornoyWord_H_


#include "PMWord.h"
#include "FreeGroup.h"


// ------------------------ PMDebornoyWordConfig --------------------------- //


class PMDebornoyWordConfig : public PMWordConfig 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
 
  PMDebornoyWordConfig( UniformRandom& R,  
			int NumOfGens, 
			int WordMaxLen,
			const FreeGroup Br, 
			bool allowTrivialWords = true
			) 
    : PMWordConfig( R, NumOfGens, WordMaxLen, allowTrivialWords ), 
      B( Br )
    { }

  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  FreeGroup getFreeGroup( ) const { return B; }
 
 
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

  FreeGroup B;
};


// --------------------------- PMDebornoyWord ------------------------------ //


class PMDebornoyWord : public PM 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PMDebornoyWord( const PMDebornoyWordConfig& CFG, Word w = Word() ) 
    : pm( CFG, w ), cfg( CFG ) { }

  PMDebornoyWord( const PMDebornoyWordConfig& CFG, const PMWord& w ) 
    : pm( w ), cfg( CFG ) { }

  // copy constructor, destructor and operator = supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* clone( ) const { return new PMDebornoyWord(*this); }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden hook functions:                                           //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* crossover( const PM* );

  PM* mutate( );

  PM* randomPM( ); 

  void printOn( ostream& ostr ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PMWord getPMWord( ) const { return pm; }

  bool operator == ( const PM& pm2 ) const 
  { return pm == pm2; }

  const PMDebornoyWordConfig& getCfg( ) { return cfg; }


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool hasFirstGen( const PMWord& r );
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const PMDebornoyWordConfig& cfg;
  PMWord pm;
};

#endif
