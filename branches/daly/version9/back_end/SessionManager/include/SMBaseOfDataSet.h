// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of class SMBaseOfDataSet
//all  the different kinds of dataset will inherit from this class
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#ifndef _SMBaseOfDataSet_H_
#define _SMBaseOfDataSet_H_


#include "DataSet.h"
#include "AlgebraicObject.h"
#include "DIC.h"
#include "DCM.h"
#include "Supervisor.h"

// --------------------------- SMBaseOfDataSet ---------------------------------- //


class SMBaseOfDataSet : public AlgebraicObject
{
public:
  ///add by xuxw, simulate SMFPGroup
  enum Checkin_Type {
	DATASET, ONEVARIABLE,TWOVARIABLE,MULTIVARIABLE,CROSSCLASSIFIED,TIMESERIES,QUALITYCONTROL,PROBABILITY
//DescriptionOfDataSet
  };

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMBaseOfDataSet( const DataSet& S, const Chars heritage, Checkin_Type t = DATASET, Chars name="S1");

  SMBaseOfDataSet( ) : AlgebraicObject(""), dic( oid() ), theDCM( 0 ) 
  { }
  //@rn Bogus ctor; see `But no' comment in Menu.h


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  DataSet getSet( ) const { return theSet; }
  // add by xuxw for type consistent 

  Checkin_Type getCheckinType( ) const { return checkin_type; }

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMBaseOfDataSet"; }

  const char* typeID( ) const { return type(); }          // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSet; }
  // overrides SMObject
  // @db need to change the ID later

  void viewStructure(ostream& ostr) const;                // overrides SMObject

  void printProperties(ostream& ostr) const;              // overrides SMObject

  void printDefinition(ostream& ostr) const;              // overrides SMObject
 Chars getName() const { return m_Name; }

protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Restricted Access:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void readMessage(istream&) { }                        // overrides SMObject

public:

  DIC dic;

private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  DataSet theSet;

  Checkin_Type  checkin_type;

  DCM* theDCM;  // TheObjects removes theDCM when this is removed.
 Chars m_Name;
};

#endif
