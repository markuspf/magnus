// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class NCProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//

#include "NCProblem.h"
#include "OutMessages.h"
#include "viewStructure.h"


//---------------------------- NCProblem ------------------------------------//


NCProblem::NCProblem(class SMFPGroup& G)
  : Supervisor( true ),
    theGroup( G ),
    NC( *this, G.gcm().normalClosure )
{ }


void NCProblem::takeControl( )
{
  if( theGroup.gic.haveCompleteCayleyGraph() )
    adminTerminate();
}


void NCProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Enumerate normal closure of relators",
		  Text("Enumerate normal closure of the relators of ") 
		  + Name( theGroup ),
		  helpID("NCProblem", theGroup),
		  "Norm Clos"
		  );

  pv.startItemGroup();

  pv.add("For the use of this problem", NC.arcSlotID(), 100);

  pv.done();
}
