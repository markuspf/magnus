// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of class SMRandomDataSet
//
// Principal Author: xiaowei xu
//
// Status: in progress
//
// Revision History:
//

#ifndef _SMRandomDataSet_H_
#define _SMRandomDataSet_H_

#include "AlgebraicObject.h"
#include "SMBaseOfDataSet.h"

//---------------------------------------------------------------------------//
//---------------------------- SMRandomDataSet ---------------------------------------//
//---------------------------------------------------------------------------//

class SMRandomDataSet : public AlgebraicObject
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMRandomDataSet (SMBaseOfDataSet& G, const Chars def, const Chars heritage);

  SMRandomDataSet (SMBaseOfDataSet& ds) : AlgebraicObject(""), theDataSet(ds) 
    { }
  //@rn Bogus ctor; see `But no' comment in Menu.h.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMBaseOfDataSet& getParent( ) const { return theDataSet; }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMRandomDataSet"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSet; }  // overrides SMObject

  void viewStructure(ostream& ostr) const;              // overrides SMObject

  void printProperties(ostream& ostr) const{}            // overrides SMObject

  void printDefinition(ostream& ostr) const;           // overrides SMObject


protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Restricted Access:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void readMessage(istream&) { }                        // overrides SMObject


protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMBaseOfDataSet& theDataSet;
  Chars m_ComputationView;
};

#endif
