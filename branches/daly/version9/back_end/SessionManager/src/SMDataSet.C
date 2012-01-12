// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMDataSet
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "SMDataSet.h"


//---------------------------- SMDataSet ---------------------------------//


SMDataSet::SMDataSet( const DataSet& S, const Chars heritage )
  : AlgebraicObject( heritage ),
    theSet( S )
{ }

void SMDataSet::printProperties(ostream& ostr) const
{
}

void SMDataSet::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}


void SMDataSet::printDefinition(ostream& ostr) const
{
  ostr << theSet;
}




