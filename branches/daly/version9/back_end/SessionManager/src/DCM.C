// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class DCM
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#include "DCM.h"
#include "SMBaseOfDataSet.h"


//---------------------------------------------------------------------------//
//-------------------------------- DCM --------------------------------------//
//---------------------------------------------------------------------------//


DCM::DCM( class SMBaseOfDataSet& D )
  : Supervisor( false ),
    theSMDataSet( D )
{
  adminStart();
}



void DCM::takeControl( )
{
}


void DCM::start( ) { }

void DCM::terminate( ) { }

void DCM::viewStructure(ostream& ostr) const { }

