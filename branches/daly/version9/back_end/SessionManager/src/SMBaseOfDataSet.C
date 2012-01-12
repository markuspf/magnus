// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMBaseOfDataSet
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#include "SMBaseOfDataSet.h"


//---------------------------- SMBaseOfDataSet ---------------------------------//


SMBaseOfDataSet::SMBaseOfDataSet( const DataSet& S, const Chars heritage, Checkin_Type t,Chars name)
  : AlgebraicObject( heritage ),
    theSet( S ),
    dic( oid() ),
    theDCM( 0 ),
    checkin_type( t )
{  m_Name=name; }

void SMBaseOfDataSet::printProperties(ostream& ostr) const
{
}

void SMBaseOfDataSet::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}


void SMBaseOfDataSet::printDefinition(ostream& ostr) const
{
  ostr << theSet;
}




