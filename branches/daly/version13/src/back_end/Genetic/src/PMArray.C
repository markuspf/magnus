// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation class PMArray
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "PMArray.h"


// ------------------------------- PMArray --------------------------------- //


PMArray::PMArray( const PMArrayConfig& CFG, PM** array )
  : cfg( CFG ), A( array )
{ }



PMArray::PMArray( const PMArray& pm )
  : cfg( pm.cfg ) 
{ 
  A = new PM*[cfg.length()];
  for( int i = 0; i < cfg.length(); ++i )
    A[i] = pm.A[i]->clone();
}


PMArray& PMArray::operator = ( const PMArray& pm )
{ 
  // cfg is the same !
  for( int i = 0; i < cfg.length(); ++i ) {
    delete A[i];
    A[i] = pm.A[i]->clone();
  }
}


PMArray::~PMArray( ) 
{
  for( int i = 0; i < cfg.length(); ++i )
    delete A[i];
  delete [] A;
}


bool PMArray::operator == ( const PM& p ) const
{ 
  const PMArray& pm = (const PMArray&) p;
  bool bEqual = true;
  for( int i = 0; i < cfg.length(); ++i )
    if( *A[i] != *(pm.A[i]) ) { bEqual = false; break; }
  return bEqual;
}


void PMArray::printOn( ostream& ostr ) const
{
  for( int i = 0; i < cfg.length(); ++i )
    cout << *A[i] << endl;
}


PM* PMArray::crossover( const PM* u )
{
  PM** X = new PM*[cfg.length()];
  for( int i = 0; i < cfg.length(); ++i )
    X[i] = A[i]->crossover(((PMArray*)u)->A[i]);
  return new PMArray( cfg, X );
}


PM* PMArray::mutate( )
{
  PM** X = new PM*[cfg.length()];
  for( int i = 0; i < cfg.length(); ++i )
    X[i] = A[i]->mutate();
  return new PMArray( cfg, X );
}
 

PM* PMArray::randomPM( )
{
  PM** X = new PM*[cfg.length()];
  for( int i = 0; i < cfg.length(); ++i )
    X[i] = A[i]->randomPM();
  return new PMArray( cfg, X );
}

