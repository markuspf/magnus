// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMRandomDataSet
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#include "SMRandomDataSet.h"
#include "OneVariableDataSet.h"
#include "OutMessages.h"

//---------------------------------------------------------------------------//
//---------------------------- SMRandomDataSet ---------------------------------------//
//---------------------------------------------------------------------------//

SMRandomDataSet::SMRandomDataSet(SMBaseOfDataSet& ds, const Chars ComputationView, const Chars heritage)
  : AlgebraicObject( heritage ),
    theDataSet( ds ),
    m_ComputationView( ComputationView )
{
}


void SMRandomDataSet::viewStructure(ostream& ostr) const
{
  ostr
	 << "viewBuilder " << m_ComputationView << " viewParameters {viewID "
	 << oid()
	 << " heritage {"
	 << heritage()
	 << "}}";
	 
}

void SMRandomDataSet::printDefinition(ostream& ostr) const{
  OneVariableDataSet ds(theDataSet.getSet());
  ds.printDefinition(ostr);
}


