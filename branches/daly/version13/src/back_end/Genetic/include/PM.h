// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of class PM
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//

#ifndef _PM_H_
#define _PM_H_


#include "global.h"


// ---------------------------------- PM ----------------------------------- //


class PM
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PM( ) { }
  
  virtual ~PM( ) { }

  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Abstract functions:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual PM* clone( ) const = 0;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Hook functions:                                                     //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual bool operator == ( const PM& ) const = 0;

  virtual PM* crossover( const PM* ) = 0;

  //virtual void crossover( const PM*, PM*, PM* );

  virtual PM* mutate( ) = 0;

  virtual PM* randomPM( ) = 0; 

  virtual void printOn( ostream& ostr ) const = 0;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool operator != ( const PM& pm ) const { return !(*this == pm); }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator << ( ostream& ostr, const PM& pm )
  {
    pm.printOn(ostr);
    return ostr;
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

};


#endif
