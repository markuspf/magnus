// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class DIC
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//
//
//

#include "DIC.h"
#include "OutMessages.h"
#include "SMBaseOfDataSet.h"

//---------------------------------------------------------------------------//
//------------------------------ DIC ----------------------------------------//
//---------------------------------------------------------------------------//

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// Constructors:                                                       //
//                                                                     //
/////////////////////////////////////////////////////////////////////////
  
DIC::DIC(OID group_oid) :
  groupOID( group_oid )
{ 
}


DIC::~DIC( )
{
}


/////////////////////////////////////////////////////////////////////////
//                                                                     //
// Reporting Information:                                              //
//                                                                     //
/////////////////////////////////////////////////////////////////////////


//======================================================================//
//                                                                      //
//  Property: isOneVariableDataSet                                      //
//                                                                      //
//======================================================================//

Trichotomy DIC::isOneVariableDataSet( ) const 
{ 
  return haveProperty( IsOneVariableDataSetProperty::type() );
}


void DIC::putIsOneVariableDataSet( ) 
{ 
  setKnownProperty( IsOneVariableDataSetProperty() );
  /// to add database Type and Object relation
  //@db 2.95.2 workaround
  CheckinType ct( groupOID );
  FEDataUpdate( ct, SMBaseOfDataSet::ONEVARIABLE ).send();
}

//======================================================================//
//                                                                      //
//  Property: isTwoVariableDataSet                                      //
//                                                                      //
//======================================================================//

Trichotomy DIC::isTwoVariableDataSet( ) const 
{ 
  return haveProperty( IsTwoVariableDataSetProperty::type() );
}

void DIC::putIsTwoVariableDataSet( ) 
{ 
  setKnownProperty( IsTwoVariableDataSetProperty() );
  /// to add database Type and Object relation
  //@db 2.95.2 workaround
  CheckinType ct( groupOID );
  FEDataUpdate( ct, SMBaseOfDataSet::TWOVARIABLE ).send();
}

//======================================================================//
//                                                                      //
//  Property: isCrossClassifiedDataSet                                      //
//                                                                      //
//======================================================================//

Trichotomy DIC::isCrossClassifiedDataSet( ) const 
{ 
  return haveProperty( IsCrossClassifiedDataSetProperty::type() );
}

void DIC::putIsCrossClassifiedDataSet( ) 
{ 
  setKnownProperty( IsCrossClassifiedDataSetProperty() );
  /// to add database Type and Object relation
  //@db 2.95.2 workaround
  CheckinType ct( groupOID );
  FEDataUpdate( ct, SMBaseOfDataSet::CROSSCLASSIFIED ).send();
}

//---------------------------------------------------------------------------
//------------------ Static data members ------------------------------------
//---------------------------------------------------------------------------

#define UNIQUE() GenericProperty::unique()

const PropertyType IsOneVariableDataSetProperty :: theTypeID = UNIQUE();
const PropertyType IsTwoVariableDataSetProperty :: theTypeID = UNIQUE();
const PropertyType IsCrossClassifiedDataSetProperty :: theTypeID = UNIQUE();

