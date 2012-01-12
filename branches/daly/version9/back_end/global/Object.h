// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of class Object
//
// Principal Author: Dmitry Bormotov
//
// Status: under development - don't use it yet!
//
// Description:
//
// * Root for Magnus class hierarchy
//
// * Defines a set of methods to be overriden by all classes
//
// Usage:
//
// * Derive all classes from Object or its derivation
//
//   Every class must have 
//
//   - default and copy constructors
//   - virtual destructor
//   - operators = and ==   
//   - I/O functions printOn and readFrom
//   - IPC functions read and write  
//
// Revision History:
//

#ifndef _Object_h_
#define _Object_h_


#include "global.h"


// -------------------------------- Object --------------------------------- //


class Object
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  Object( ) { }
  
  virtual ~Object( ) { }

  // copy constructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool operator != ( const Object& o ) const { return !(*this == o); }

  // I/O

  friend ostream& operator << ( ostream& ostr, const Object& o ) {
    o.printOn(ostr);
    return ostr;
  }

  friend istream& operator >> ( istream& istr, Object& o ) {
    o.readFrom(istr);
    return istr;
  }

  // IPC

  friend ostream& operator < ( ostream& ostr, const Object& o )
  {
    o.write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, Object& o )
  {
    o.read(istr);
    return istr;
  }
 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Abstract functions:                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual Object& operator = ( const Object& );

  virtual bool operator == ( const Object& ) const = 0;
 

protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Protected abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // I/O and IPC

  virtual void printOn( ostream& ostr ) const = 0;

  virtual void readFrom( istream& istr ) = 0;

  virtual void write( ostream& ostr ) const = 0;

  virtual void read( istream& istr ) = 0;


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
