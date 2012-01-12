// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of classes PMArrayConfig, PMArray
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//

#ifndef _PMArray_H_
#define _PMArray_H_


#include "PM.h"
#include "RandomNumbers.h"
#include "GAConfig.h"


// ----------------------------- PMArrayConfig ----------------------------- //


class PMArrayConfig 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PMArrayConfig( UniformRandom& R, int Len, const GAConfig& GAC ) 
    : r( R ), len( Len ), gac( GAC ) { }

  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  UniformRandom& rng( ) const { return r; }

  int length( ) const { return len; }
 
 
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
  int len;
  const GAConfig& gac;
};


// ------------------------------- PMArray --------------------------------- //


class PMArray : public PM 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PMArray( const PMArrayConfig& CFG, PM** array );

  PMArray( const PMArray& pm );
  
  ~PMArray( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* clone( ) const { return new PMArray(*this); }
 

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

  PMArray& operator = ( const PMArray& pm );

  bool operator == ( const PM& pm ) const;

  const PM* value( int i ) const 
  {
  #if( SAFETY > 0 )
    if( i < 0 || i >= cfg.length() )
      error("const PM* PMArray::value( int i ) const : "
	    "index is out of bounds");
  #endif
    return A[i]; 
  } 


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

  const PMArrayConfig& cfg;
  PM** A;
};


#endif
