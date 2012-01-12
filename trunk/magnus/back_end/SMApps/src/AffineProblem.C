// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class AffineProblem
//
// Principal Author: Nathan Penton
//
// Status: in progress
//
// Revision History:
//

#include "AffineProblem.h"
#include "viewStructure.h"
#include "Affine.h"
#include "SMFPGroup.h"
#include "OutMessages.h"

ostream& operator < ( ostream& ostr, const GrobnerIPC &G)
{
   int i;
   ostr < G.poly_deg;
   ostr < G.grobner.size();
   for(i=0;i<G.grobner.size();i++){
      ostr < G.grobner[i];
   }
	return ostr;
}

istream& operator > ( istream& istr, GrobnerIPC &G)
{
   int size,i;
   istr > G.poly_deg;
   polynomial grob(G.poly_deg);
   istr > size;
   G.grobner.clear();
   for(i=0;i<size;i++){
      istr > grob;
      G.grobner.push_back(grob);
   }
	return istr;
}

AffineARCer::AffineARCer( ComputationManager& boss )
  : ARCer( boss ) 
{
}

AffineARCer::~AffineARCer( )
{
}

void AffineARCer::setArguments(  const SMFPGroup &G, int degree )
{
	deg=degree;
	theGroup = G.getFPGroup();
}

void AffineARCer::runComputation( )
{
  Affine theAffine( theGroup, deg );
  grobner_basis = theAffine.get_grobner_basis();
  grobner_ipc.set(grobner_basis,theAffine.get_poly_deg());
  dimension = theAffine.get_dimension();
}

void AffineARCer::writeResults( ostream& ostr )
{
  ostr < grobner_ipc;
  ostr < dimension; 
}

void AffineARCer::readResults( istream& istr )
{
  istr > grobner_ipc;
  istr > dimension;
  grobner_basis = grobner_ipc.get_basis();
}



//---------------------------------------------------------------------------//
//----------------------------- AffineProblem -------------------------------//
//---------------------------------------------------------------------------//


AffineProblem::AffineProblem( const AffineSupervisor& affine )
  : theGroup( affine.group() ), theDegree( affine.degree() ),
    AffineArcer( *this ), arcerStarted( false ), solved( false )
{
  adminStart();
}


void AffineProblem::takeControl( )
{
  int i;
  if( freeARCs() > 0 ) {
    
    if( !arcerStarted ) {
      AffineArcer.setArguments( theGroup,theDegree );
      arcerStarted = true;
      usedOneARC();
    }
    else {
      if( AffineArcer.takeControl() ) {
	vector<polynomial> grobner = AffineArcer.getGrobnerBasis();
	LogMessage msg( *this, theGroup );
	msg << "Grobner basis of affine algebraic set of representations "
		<< "in SL(" << theDegree << ",C):";
	msg.send();
	for(i=0;i<grobner.size();i++){
		msg << grobner[i] ;
		msg.send();
	}
	msg << "Dimension: " << AffineArcer.getDimension() << ".";
	msg.send();
	solved = true;
	adminTerminate();
	
      }
      else
	usedOneARC();
    }
  }
}



//---------------------------------------------------------------------------//
//-------------------------- AffineSupervisor -------------------------------//
//---------------------------------------------------------------------------//

AffineSupervisor::AffineSupervisor(SMFPGroup& G, int d)
  : Supervisor( true ), theGroup( G ), theDegree( d ), 
    affine( *this )
{
}

void AffineSupervisor::takeControl( ) 
{
  if( affine->isSolved() )
    adminTerminate();
}

void AffineSupervisor::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Affine Algebraic Representations"),
  Text("Compute the dimension of the affine algebraic set of representations in SL(")+ theDegree +",C)  of "
		  + Name( theGroup ),
		  helpID("AffineProblem", theGroup),
		  "Affine"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), affine.arcSlotID(), 50);

  pv.done();
}


