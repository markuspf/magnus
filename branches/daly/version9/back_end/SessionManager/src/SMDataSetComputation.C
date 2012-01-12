// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMDataSetComputation
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#include "SMDataSetComputation.h"
#include "OneVariableDataSet.h"
#include "OutMessages.h"

//---------------------------------------------------------------------------//
//---------------------------- SMDataSetComputation ---------------------------------------//
//---------------------------------------------------------------------------//

SMDataSetComputation::SMDataSetComputation(SMBaseOfDataSet& ds, const Chars ComputationView, const Chars heritage)
  : AlgebraicObject( heritage ),
    theDataSet( ds ),
    m_ComputationView( ComputationView )
{
}


void SMDataSetComputation::viewStructure(ostream& ostr) const
{
  ostr
	 << "viewBuilder " << m_ComputationView << " viewParameters {viewID "
	 << oid()
	 << " heritage {"
	 << heritage()
	 << "}}";
	 
}

void SMDataSetComputation::printDefinition(ostream& ostr) const{
  for(int i=0;i<theDataSet.getSet().getDimension();i++) {
      OneVariableDataSet ds(theDataSet.getSet(),i);
      ds.printDefinition(ostr);
      ostr << " ";
  }
}


