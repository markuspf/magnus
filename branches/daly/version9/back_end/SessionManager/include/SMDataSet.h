// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of class SMDataSet
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//

#ifndef _SMDataSet_H_
#define _SMDataSet_H_


#include "DataSet.h"
#include "AlgebraicObject.h"


// --------------------------- SMDataSet ---------------------------------- //


class SMDataSet : public AlgebraicObject
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMDataSet( const DataSet& S, const Chars heritage);

  SMDataSet( ) : AlgebraicObject("")
  { }
  //@rn Bogus ctor; see `But no' comment in Menu.h


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  DataSet getSet( ) const { return theSet; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSet"; }

  const char* typeID( ) const { return type(); }          // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSet; }
  // overrides SMObject
  // @db need to change the ID later

  void viewStructure(ostream& ostr) const;                // overrides SMObject

  void printProperties(ostream& ostr) const;              // overrides SMObject

  void printDefinition(ostream& ostr) const;              // overrides SMObject


protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Restricted Access:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void readMessage(istream&) { }                        // overrides SMObject


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  DataSet theSet;

};

#endif
