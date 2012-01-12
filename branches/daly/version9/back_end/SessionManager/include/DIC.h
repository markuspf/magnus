// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Declaration of class DIC (Group Information Center)
//
// Principal Author: Xiaowei Xu
//
// Status: this can never be complete
//
// Usage:
//
// For a given piece of info, there are up to four kinds of names needed:
// 1) The data member
// 2) The member function through which it is reported
// 3) The member function via which its existence is queried
// 4) The member function via which it is accessed
// 
// In an attempt at coherence, we try to use these conventions for a
// Trichotomy (resp. bool) <I>
// e.g., I == abelian, (resp. I == solvedWordProblem):
// 
// 1) <I>
// 2) putIs<I>  resp. putHave<I>
// 3) is<I>     resp. have<I>
// 
// and in other cases (e.g., I == confluentKBMachine):
// 
// 1) <I>
// 2) putHave<I>
// 3) have<I>
// 4) get<I>
//
//
// Next Implementation Steps:
//
// * Better exception instrumentation, e.g., conflicting reports of order
//
// * It is not clear how much the explanations will need to be accessed,
//   since they are sent to the FE when the explained info is reported.
//   Put accessors in as needed.
//
//
// Revision History:
//



#ifndef _DIC_H_
#define _DIC_H_



#include "FEData.h"
#include "BaseProperties.h"
#include "InformationCenter.h"
#include "OID.h"


//---------------------------------------------------------------------------//
//--------------------- DIC PROPERTIES --------------------------------------//
//---------------------------------------------------------------------------//

///add by xuxw
class IsOneVariableDataSetProperty : public NoDataProperty {
public:
  IsOneVariableDataSetProperty() : NoDataProperty() {}
  PropertyType actualType()   const { return type(); }
  static PropertyType type() { return theTypeID; }
  Chars propertyName() const { return "IsOneVariableDataSet"; }
protected:
  GenericProperty* clone() const { return new IsOneVariableDataSetProperty(*this); }
private:
  static const PropertyType theTypeID;
};

class IsTwoVariableDataSetProperty : public NoDataProperty {
public:
  IsTwoVariableDataSetProperty() : NoDataProperty() {}
  PropertyType actualType()   const { return type(); }
  static PropertyType type() { return theTypeID; }
  Chars propertyName() const { return "IsTwoVariableDataSet"; }
protected:
  GenericProperty* clone() const { return new IsTwoVariableDataSetProperty(*this); }
private:
  static const PropertyType theTypeID;
};

class IsCrossClassifiedDataSetProperty : public NoDataProperty {
public:
  IsCrossClassifiedDataSetProperty() : NoDataProperty() {}
  PropertyType actualType()   const { return type(); }
  static PropertyType type() { return theTypeID; }
  Chars propertyName() const { return "IsCrossClassifiedDataSet"; }
protected:
  GenericProperty* clone() const { return new IsCrossClassifiedDataSetProperty(*this); }
private:
  static const PropertyType theTypeID;
};


// By its nature as a `knowledge representer', which no one knows how to
// do, this has evolved in odd and sometimes conflicting ways as
// requirements have become apparent.  It will need combing and
// rethinking periodically.


//---------------------------------------------------------------------------//
//-------------------------------- DIC --------------------------------------//
//---------------------------------------------------------------------------//


class DIC : public InformationCenter, protected FEData
{  
public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  DIC(OID group_oid);

  ~DIC( );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Reporting Information:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // add by xuxw  to show Front_end This is a One Variable Data Set
  Trichotomy isOneVariableDataSet( ) const;
  Trichotomy isTwoVariableDataSet( ) const;
  Trichotomy isCrossClassifiedDataSet( ) const;
  void putIsCrossClassifiedDataSet( );
  void putIsOneVariableDataSet( );

  void putIsTwoVariableDataSet( );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessing Information:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


private:

  DIC(const DIC&); // Disable copying

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  OID groupOID;
};


#endif
