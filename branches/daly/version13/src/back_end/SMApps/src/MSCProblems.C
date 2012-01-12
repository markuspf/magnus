// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//

#include "MSCProblems.h"


// -------------------------- MSCLambdaARCer ------------------------------- //


void MSCLambdaARCer::setArguments( const FPGroup& group )
{
  G = group;
}


void MSCLambdaARCer::runComputation( )
{
  theLambda = G.cancellationLambda();
}


void KBSupervisorARCer::writeResults( ostream& out ) 
{
  out < theLambda;
}


void KBSupervisorARCer::readResults( istream& in )
{
  in > theLambda;
}


// ---------------------------- MSCLambda ---------------------------------- //


MSCLambda::MSCLambda( class GCM& gcm )
  : theGroup( gcm.getSMFPGroup() ),
    arcer( *this )
{
  adminStart();
  if( theGroup.gic.isMSC() ) resultIsFast();
}


void MSCLambda::start( )
{
  arcer.setArguments( theGroup.getFPGroup() );
}


void MSCLambda::takeControl( )
{
  if( fastResult() ) {
    theGroup.gic.postIsMSCReport();
    adminTerminate();
    return;
  }
  
  if ( freeARCs() > 0 ) 

    if( arcer.takeControl() ) {

      ostrstream msg;
      int lambda = arcer.lambda();

      if ( lambda < 6 && lambda != 0 ) {

	msg << "The presentation of " << Name( theGroup )
	    << " is not metric small cancellation; it is only C'(1/"
	    << lambda << ')';

      } else {
	 
	msg << "The presentation of " << Name( theGroup ) << " is C'(";
	if ( lambda != 0 ) msg << "1/";
	msg << lambda << ')';
      }

      msg << "." << ends;

      theGroup.gic.putIsMSC( true, lambda, msg.str() );
      theGroup.gic.postIsMSCReport();

      adminTerminate();
    }
    else
      usedOneARC();
}


// ------------------------- MSCLambdaProblem ------------------------------ //


MSCLambdaProblem::MSCLambdaProblem( class SMFPGroup& G )
  : Supervisor( true ),
    theGroup( G ),
    MSCLambda( *this, G.gcm().MSCLambda )
{ }


void MSCLambdaProblem::takeControl( )
{
  if( theGroup.gic.haveConfluentKBMachine() )
    adminTerminate();
}


void KBProblem::viewStructure(ostream& ostr) const
{
  Problem//@dbView pv( ostr, oid(), "Seek rewriting system",
		  Text("Seek a rewriting system for") + Name( theGroup ),
		  "KBProblem", "Rewr sys"
		  );

  pv.startItemGroup();

  pv.add("For the use of this problem", kbSupervisor.arcSlotID(), 100);

  pv.done();
}
