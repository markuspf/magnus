// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of classes AbelianInvariants, AbelInvariantsProb,
//           AbelianRank, AbelianWordProblem, AbelianIsomProblem
//
// Principal Author: Dmitry Bormotov,Alexey Myasnikov
//
// Status: in progress
//
// Revision History:
//


#include "AbelianProblems.h"
#include "SMWord.h"
#include "SMSubgroup.h"
#include "SMMap.h"
#include "SMHomomorphism.h"
#include "viewStructure.h"
#include "OutMessages.h"


// ----------------------- AbelianWordProblem ------------------------------ //


AbelianWordProblem::AbelianWordProblem(SMWord& w)
: Supervisor( ! w.getParent().gic.haveCyclicDecomposition() ),
  theWord( w ),
  abelianInvariants( *this, w.getParent().gcm().abelianInvariants )
{
  if ( w.getParent().gic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianWordProblem::takeControl( )
{
  if ( theWord.getParent().gic.haveCyclicDecomposition() ) {

    const AbelianGroup& A = 
      theWord.getParent().gic.getCyclicDecomposition();

    LogMessage msg( *this, theWord );

    msg << "The canonical decomposition of " << Name( theWord ) << ": ";

    A.printWordInNewGens( msg, A.oldToNewGens
      ( A.oldInAbelianForm( theWord.getWord() ) ) );

    msg << ".";

    msg.send();

    adminTerminate();
  }
}


void AbelianWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Compute the canonical decomposition of a word",
		  Text("Compute the canonical decomposition of ")
		  + Name( theWord ),
		  helpID("AbelianWordProblem", theWord.getParent()),
		  "Can dec"
		  );

  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
			 abelianInvariants.arcSlotID(),
			 50
			 );

  pv.done();
}


// ----------------------- AbelianIsomProblem ------------------------------ //


AbelianIsomProblem::AbelianIsomProblem(SMFPGroup& g1, SMFPGroup& g2)
  : Supervisor( ! g1.gic.haveCyclicDecomposition() ||
		! g2.gic.haveCyclicDecomposition()
		),
    G1( g1 ),
    G2( g2 ),
    abelianInvariants1( *this, g1.gcm().abelianInvariants ),
    abelianInvariants2( *this, g2.gcm().abelianInvariants )
{
  if ( G1.gic.haveCyclicDecomposition() && G2.gic.haveCyclicDecomposition() )
    resultIsFast();
}


void AbelianIsomProblem::takeControl( )
{
  if ( G1.gic.haveCyclicDecomposition() && G2.gic.haveCyclicDecomposition() ) {
    
    const AbelianGroup& A1 = G1.gic.getCyclicDecomposition();
    const AbelianGroup& A2 = G2.gic.getCyclicDecomposition();
    
    LogMessage msg( *this, G1, G2 );
    msg << Name( G1 ) << " and " << Name( G2 ) << " are ";

    if ( A1.rankOfFreeAbelianFactor() != A2.rankOfFreeAbelianFactor() ) {

      msg << "not isomorphic: they have different free ranks.";
	   
    } else {
	   
      if ( A1.invariants() != A2.invariants() ) {
	     
	msg << "not isomorphic because their torsion subgroups differ.";
	     
      } else {
	     
	msg << "isomorphic because they have the same invariants.";
      }
    }
	 
    msg.send();

    adminTerminate();
  }
}


void AbelianIsomProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Abelian isomorphism problem",
		  Text("Are") + Name( G1 ) + "and"
		  + Name( G2 ) + "isomorphic?",
		  helpID("AbelianIsomProblem", G1),
		  "Isomorphic ?"
		  );
  
  pv.startItemGroup();

  if ( ! G1.gic.haveCyclicDecomposition() ) {

	 pv.add( Text("Compute abelian invariants of") + Name( G1 ),
		 abelianInvariants1.arcSlotID(),
		 50
		 );
  }

  if ( ! G2.gic.haveCyclicDecomposition() ) {

	 pv.add( Text("Compute abelian invariants of") + Name( G2 ),
		 abelianInvariants2.arcSlotID(),
		 50
		 );
  }

  pv.done();
}


//------------------- AbelianTorsionFreeRankProblem -------------------------//


AbelianTorsionFreeRankProblem::AbelianTorsionFreeRankProblem(SMFPGroup& G)
  : Supervisor( ! G.gic.haveCyclicDecomposition() ),
	 theGroup( G ),
 	 abelianInvariants( *this, G.gcm().abelianInvariants )
{
  if ( theGroup.gic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianTorsionFreeRankProblem::takeControl( )
{
  if ( theGroup.gic.haveCyclicDecomposition() ) {

	 const AbelianGroup& A = theGroup.gic.getCyclicDecomposition();

	 LogMessage msg( *this, theGroup );
	 msg <<	"The torsion-free rank of " << Name( theGroup ) << " is "
	     << A.rankOfFreeAbelianFactor( )
	     << ": this follows from the canonical decomposition of "
	     << Name(theGroup) << "."  ;

	 msg.send();

	 adminTerminate();
  }
}


void AbelianTorsionFreeRankProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Find the torsion-free rank",
		  Text("Find the torsion-free rank of") + Name( theGroup ),
		  helpID("TorsionFreeRankProblem", theGroup),
		  "T-f rank"
		  );

  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
}


//-------------- AbelianOrderOfTheTorsionSubgroupProblem --------------------//


AbelianOrderOfTheTorsionSubgroupProblem::
    AbelianOrderOfTheTorsionSubgroupProblem(SMFPGroup& G)
    : Supervisor( ! G.gic.haveCyclicDecomposition() ),
      theGroup( G ),
      abelianInvariants( *this, G.gcm().abelianInvariants )
{
  if ( theGroup.gic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianOrderOfTheTorsionSubgroupProblem::takeControl( )
{
  if ( theGroup.gic.haveCyclicDecomposition() ) {

	 const AbelianGroup& A = theGroup.gic.getCyclicDecomposition();
	 Integer theOrder = A.orderOfTheTorsionSubgroup();

	 LogMessage msg( *this, theGroup );

	 msg <<	"The order of the torsion subgroup of " << Name( theGroup )
	     << " is ";

	 if( sign(theOrder) == 0 )
	   msg << "infinite";
	 else
	   msg << theOrder;

	 msg << ": this follows from the canonical decomposition of "
	     << Name( theGroup ) << ".";

	 msg.send();

	 adminTerminate();
  }
}


void AbelianOrderOfTheTorsionSubgroupProblem::viewStructure(ostream& ostr)
     const
{
  ProblemView pv( ostr, oid(), "Find the order of the torsion subgroup",
		  Text("Find the order of the torsion subgroup of ")
		  + Name( theGroup ), 
		  helpID("AbelianOrderOfTheTorsionSubgroupProblem", 
			 theGroup), "Ord tor subgp"
		  );

  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
			 abelianInvariants.arcSlotID(),
			 50
			 );

  pv.done();
}


//------------------------- AbelianOrderProblem -----------------------------//


AbelianOrderProblem::AbelianOrderProblem(SMFPGroup& G)
    : Supervisor( ! G.gic.haveCyclicDecomposition() ),
      theGroup( G ),
      abelianInvariants( *this, G.gcm().abelianInvariants )
{
  if ( theGroup.gic.haveOrder() || theGroup.gic.haveCyclicDecomposition() )
    resultIsFast();
}


void AbelianOrderProblem::takeControl( )
{
  if( theGroup.gic.haveOrder() ) {
    LogMessage( theGroup.gic.getOrderMessage() ).send();
    adminTerminate();
    return;
  }
  
  if( theGroup.gic.haveCyclicDecomposition() ) {

    AbelianGroup& A = (AbelianGroup&)(theGroup.gic.getCyclicDecomposition());

    Chars explanation = Text("this follows from the canonical decomposition of")
      + Name(theGroup);
    theGroup.gic.putHaveOrder(A.order(), explanation);
    
    if( !fastResult() )
      LogMessage(*this, theGroup.gic.getOrderMessage(), 2).send();
    
    adminTerminate();
  }
}


void AbelianOrderProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Compute the order",
		  Text("Find the order of ") + Name( theGroup ),
		  helpID("AbelianOrderProblem", theGroup),
		  Text("Order") + Name(theGroup)
		  );

  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
			 abelianInvariants.arcSlotID(),
			 50
			 );

  pv.done();
}


// -------------------- AbelianOrderOfEltProblem --------------------------- //


AbelianOrderOfEltProblem::AbelianOrderOfEltProblem(SMWord& w)
  : Supervisor( ! w.getParent().gic.haveCyclicDecomposition() ),
    theWord( w ),
    abelianInvariants( *this, w.getParent().gcm().abelianInvariants )
{
  if ( w.getParent().gic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianOrderOfEltProblem::takeControl( )
{
  if ( theWord.getParent().gic.haveCyclicDecomposition() ) {

	 const AbelianGroup& A = theWord.getParent().gic.getCyclicDecomposition();
	 Integer theOrder = A.orderOfElt( theWord.getWord() );
	 
	 LogMessage msg( *this, theWord );

	 msg << "The order of " << Name( theWord ) << " is ";

	 if( theOrder == 0 )
	   msg << "infinite";
	 else
	   msg << theOrder;

	 msg << ": this follows from the canonical decomposition.";

	 msg.send();

	 adminTerminate();
  }
}


void AbelianOrderOfEltProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Find the order",
		  Text("Find the order of ") + Name( theWord ),
		  helpID("AbelianOrderOfEltProblem", theWord.getParent()),
		  Text("Order") + Name(theWord)
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
			 abelianInvariants.arcSlotID(),
			 50
			 );

  pv.done();
}


// --------------------- AbelianEqualityProblem ---------------------------- //


AbelianEqualityProblem::AbelianEqualityProblem(SMWord& w1, SMWord& w2)
  : Supervisor( ! w1.getParent().gic.haveCyclicDecomposition() ),
    word1( w1 ), word2( w2 ),
    abelianInvariants( *this, w1.getParent().gcm().abelianInvariants )
{
  if ( w1.getParent().gic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianEqualityProblem::takeControl( )
{
  if ( word1.getParent().gic.haveCyclicDecomposition() ) {

    const AbelianGroup& A = word1.getParent().gic.getCyclicDecomposition();
    bool answer = A.areEqual( word1.getWord(), word2.getWord() );
    
    LogMessage msg( *this, word1, word2 );
    
    msg << Name( word1 ) << " is ";
    
    if( !answer )
      msg << "not ";

    msg << "equal to " << Name( word2 )
	<< ": this follows from the canonical decompositions.";
    
    msg.send();
    
    adminTerminate();
  }
}


void AbelianEqualityProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Equality problem",
		  Text("Is ") + Name( word1 ) + Text(" = ")
		  + Name( word2 ) + Text(" ?"), 
		  helpID("AbelianEqualityProblem", word1.getParent()),
		  Text(Name(word1)) + "=" + Name(word2) + "?"
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( Parent( word1 ) ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
}


// ------------------------ WordInSGOfAbelian ------------------------------ //


WordInSGOfAbelian::WordInSGOfAbelian( SMSubgroup& S, SMWord& w )
  : Supervisor( true ), theSubgroup( S ), theWord( w ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() )
    resultIsFast();
}


void WordInSGOfAbelian::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    bool answer = A.isTrivial( theWord.getWord() );
    File file(A.getFileName());
  
    LogMessage msg( *this, theSubgroup, theWord );
    
    msg << Name( theWord ) << " is ";
    
    if( !answer )
      msg << "not ";
    
    msg << "in " << Name( theSubgroup ) 
	<< " : this follows from the canonical decomposition of "
	<< Name( theWord ) << " in the factor group "
	<< Name( Parent( theSubgroup ) ) << "/" 
	<< Name( theSubgroup ) << "."; 
    msg.send();

    FPGroup G = theWord.getParent().getFPGroup();
    file << endl << endl << "The canonical decomposition of "
	 << Name( Parent( theSubgroup ) ) << "/" 
	 << Name( theSubgroup ) <<  ":" << endl << endl
	 << A << "." << endl << endl;

    file << "The canonical decomposition of the image of "
	 << Name( theWord ) << " in the new generators of the factor group "
	<< Name( Parent( theSubgroup ) ) << "/" 
	 << Name( theSubgroup ) << ":" << endl << endl; 

    AbelianWord abW = A.oldInAbelianForm(theWord.getWord());
    A.printWordInNewGens(file,A.oldToNewGens(abW));
    file << "." << endl;

    LogMessage msg1(*this,theWord, theSubgroup);
    msg1 << Link(Chars( "Click here to see the canonical decomposition of ")+
		 Text(Name( theWord ))+ Chars(" in the factor group ")+ 
		 Text(Name( Parent( theSubgroup ) )) + Chars("/")  +
		 Text(Name( theSubgroup )),
		 "CyclicDecomposition",
		 file.getFileName() );
    msg1.send();   
    adminTerminate();
  }
}


void WordInSGOfAbelian::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Subgroup membership problem",
		  Text("Is ") + Name( theWord ) + Text(" in ")
		  + Name( theSubgroup ) + Text(" ?"), 
		  helpID("WordInSGOfAbelian",theWord.getParent()),
		  Text(Name( theWord )) + "in" + Name(theSubgroup) + "?"
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of ") + Name( Parent( theWord ) )
	  + Text("/") + Name( theSubgroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
}


// ------------------- AbelianIsSGEqualToTheGroup -------------------------- //


AbelianIsSGEqualToTheGroup::AbelianIsSGEqualToTheGroup( SMSubgroup& S )
  : Supervisor( true ),
    theSubgroup( S ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() )
    resultIsFast();
}


void AbelianIsSGEqualToTheGroup::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    bool answer = A.isTrivial();
    
    LogMessage msg( *this, theSubgroup );
    
    msg << Name( theSubgroup ) << " is ";
    
    if( !answer )
      msg << "not ";

    msg << "equal to " << Name( theSubgroup.getParent() )
	<< ": this follows by computing the abelian invariants of "
	<< Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup ) << ".";
  
    msg.send();
    
    adminTerminate();
  }
}


void AbelianIsSGEqualToTheGroup::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Is a subgroup equal to a group ?",
		  Text("Is ") + Name( theSubgroup ) + Text(" = ")
		  + Name( theSubgroup.getParent() ) + Text(" ?"),
		  helpID("AbelianIsSGEqualToTheGroup", 
			 theSubgroup.getParent()),
		  Text(Name(theSubgroup)) + "="
		  + Name(theSubgroup.getParent()) + "?"
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of ")
	  + Name( Parent( theSubgroup ) ) + Text("/") + Name( theSubgroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
}


// ---------------------- AbelianSGIndexProblem ---------------------------- //


AbelianSGIndexProblem::AbelianSGIndexProblem( SMSubgroup& S )
  : Supervisor( true ),
    theSubgroup( S ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() )
    resultIsFast();
}


void AbelianSGIndexProblem::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    Integer answer = A.order();
    
    LogMessage msg( *this, theSubgroup );
    
    msg << Name( theSubgroup ) << " has";
    
    if( answer == 0 )
      msg << " infinite index";
    else {
      msg << " index " << answer;
    }
    msg << ": the canonical decomposition of "
	<< Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup ) 
	<< " is " << A << ".";
  
    msg.send();
    
    adminTerminate();
  }
}


void AbelianSGIndexProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Index of a subgroup problem",
		  Text("Compute the index of") + Name( theSubgroup )
		  + Text("in") + Name( theSubgroup.getParent() ),
		  helpID("AbelianSGIndexProblem", theSubgroup.getParent()),
		  Text("Index") + Name(theSubgroup)
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of ")
	  + Name( Parent( theSubgroup ) ) + Text("/") + Name( theSubgroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
}


// -------------------- AbelianIsSGIsolatedProblem ------------------------- //


AbelianIsSGIsolatedProblem::AbelianIsSGIsolatedProblem( SMSubgroup& S )
  : Supervisor( true ),
    theSubgroup( S ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() )
    resultIsFast();
}


void AbelianIsSGIsolatedProblem::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    Integer answer = A.orderOfTheTorsionSubgroup();
    
    LogMessage msg( *this, theSubgroup );
    
    msg << Name( theSubgroup ) << " is";
    
    if( answer > 1 )
      msg << " not";
    msg << " isolated";
    
    msg << ": the canonical decomposition of "
	<< Name( Parent( theSubgroup ) ) << "/" 
	<< Name( theSubgroup ) << " is " 
	<< A << ".";
  
    msg.send();
    
    adminTerminate();
  }
}


void AbelianIsSGIsolatedProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Isolation problem",
		  Text("Is") + Name( theSubgroup )
		  + Text("isolated in") + Name( theSubgroup.getParent() ) +
		  Text("?"),
		  helpID("AbelianIsSGIsolatedProblem", 
			 theSubgroup.getParent()),
		  Text("Is")+Text(Name(theSubgroup)) + Text("isolated?")
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of ")
	  + Name( Parent( theSubgroup ) ) + Text("/") + Name( theSubgroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
}


// ------------------- AbelianSGContainmentProblem ------------------------- //


AbelianSGContainmentProblem::AbelianSGContainmentProblem(
    SMSubgroup& S1, SMSubgroup& S2 )
  : Supervisor( true ),
    subgroup1( S1 ),
    subgroup2( S2 ),
    abelianInvariants( *this, S1.scm().abelianInvariantsOfFactor )
{
  if( subgroup1.sic.haveCyclicDecompositionOfFactor() )
    resultIsFast();
}


void AbelianSGContainmentProblem::takeControl( )
{
  if ( subgroup1.sic.haveCyclicDecompositionOfFactor() ) {
    
    const AbelianGroup& A = subgroup1.sic.getCyclicDecompositionOfFactor();
    bool answer = yes;
    VectorOf<Word> V = subgroup2.getSubgroup().generators();
    
    for( int i = 0; i < V.length(); ++i )
      if( !A.isTrivial( V[i] ) ) {
	answer = no;
	break;
      }
    
    LogMessage msg( *this, subgroup1, subgroup2 );
    
    msg << Name( subgroup1 );
    
    if( !answer )
      msg << " doesn't contain ";
    else
      msg << " contains ";
   
    msg << Name( subgroup2 )
       << ": this follows by computing the abelian invariants of "
       << Name( Parent( subgroup1 ) ) << "/" << Name( subgroup1 ) << ".";
    
    msg.send();
    
    adminTerminate();
  }
}


void AbelianSGContainmentProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Subgroup containment problem",
		  Text("Does ") + Name( subgroup1 ) + Text(" contain ")
		  + Name( subgroup2 ) + Text(" ?"), 
		  helpID("AbelianSGContainmentProblem", 
			 subgroup1.getParent()),
		  Text(Name(subgroup1)) + "cont" + Name( subgroup2 ) + "?"
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of ") + Name( Parent( subgroup1 ) )
	  + Text("/") + Name( subgroup1 ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
}

// -------------------- AbelianPHeightOfEltProblem --------------------------- //

AbelianPHeightOfEltProblem::AbelianPHeightOfEltProblem(SMWord& w, int p)
  : Supervisor( ! w.getParent().gic.havePrimaryDecomposition() ),
    theWord( w ),
    abDone( false ),
    P (p),
    abelianInvariants( *this, w.getParent().gcm().abelianInvariants ),
    abelianPrimes( *this, w.getParent().gcm().abelianPrimes ),
    notPrime(false)
{
  if (!PrimeNumbers().isPrime(p)){
    notPrime = true;
    resultIsFast();
  }
  if ( w.getParent().gic.haveCyclicDecomposition() ) 
                abDone = true;
  if ( w.getParent().gic.havePrimaryDecomposition() ) 
                resultIsFast();
}


void AbelianPHeightOfEltProblem::takeControl( )
{
  if (notPrime){
    LogMessage msg( *this, theWord );
    msg << P <<" is not a prime number." ;
    msg.send();
    adminTerminate();
    return;    
  }
  if ( theWord.getParent().gic.havePrimaryDecomposition() ) {

	 const AbelianGroup& A = theWord.getParent().gic.getCyclicDecomposition();
	 Integer theOrder = A.orderOfElt( theWord.getWord() );
 	 LogMessage msg( *this, theWord );
         if (gcd(theOrder,P)==1){
	   msg << Name( theWord ) << 
	     " is of order prime to "<< P <<", it's "
	       << P << "-height is infinite.";
	   msg.send();
	   adminTerminate();
	   return;
         }
         Integer pHeight = theWord.getParent().gic.
	                   getCyclicDecomposition().
	                   pHeightOfElt(theWord.getWord(),P,theOrder);
	 Integer power;
         power = IntProblems().powerOf(theOrder,P);
         if (power!=-1){
	   msg << Name( theWord ) << " is of order "<< 
	     P <<"^" << power <<", it's "<< P <<
	     "-height is ";
	   
	   if (pHeight<0)
	     msg << "infinite";
	   else
	     msg << pHeight ;
	   msg << ".";
	   msg.send();
	   adminTerminate();
	   return;
         }  	 
         
	 msg << Name( theWord ) << " is of ";
	 if (theOrder == 0)
	   msg <<"infinite order";
	 else
	   msg << "order " << theOrder;
	 msg << ", it's "<< P <<"-height is ";
	 if (pHeight<0)
	   msg << "infinite";
	 else
	   msg << pHeight ;
	 msg << ".";
	 msg.send();
 	 adminTerminate();
   }
}


void AbelianPHeightOfEltProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Find the p-height",
		  Text("Find the p-height of ") + Name( theWord ),
		  helpID("AbelianPHeightOfEltProblem", theWord.getParent()),
		  Text("p-height") + Name(theWord)
		  );
  
  pv.startItemGroup();

  if (!abDone)
     pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
			 abelianInvariants.arcSlotID(),
			 50
			 );
  pv.add(Text("Find the primary decomposition of")+Name( Parent( theWord ) ),
                         abelianPrimes.arcSlotID(), 50);

  pv.done();
}
//-------------- AbelianComputeTorsionSubgroup --------------------//


AbelianComputeTorsionSubgroup::
    AbelianComputeTorsionSubgroup(SMFPGroup& G)
    : Supervisor( ! G.gic.haveCyclicDecomposition() ),
      theGroup( G ),
      abelianInvariants( *this, G.gcm().abelianInvariants )
{
  if ( theGroup.gic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianComputeTorsionSubgroup::takeControl( )
{
  if ( theGroup.gic.haveCyclicDecomposition() ) {
     LogMessage msg( *this, theGroup );
     const AbelianGroup& A = theGroup.gic.getCyclicDecomposition();
     VectorOf<AbelianWord> cyclicGens = A.newToOldGens();
     VectorOf<Word> torsionGens(cyclicGens.length()-A.rankOfFreeAbelianFactor());

     if (cyclicGens.length() == A.rankOfFreeAbelianFactor() ){
          msg << Name( theGroup ) << " does not have any non trivial torsion subgroup.";
          msg.send();
      }
     else{
        for (int i=0;i<torsionGens.length();i++)
                 torsionGens[i]=cyclicGens[i+A.rankOfFreeAbelianFactor()].getWord();

        SGofFreeGroup tmpSG(theGroup.getFreePreimage(),torsionGens);
          
         msg << "Torsion subgroup of " << Name(theGroup ) << " is computed.";
         msg.send();
         SMObject* smo = new SMSubgroup( theGroup,
		    tmpSG,
		    Text("Torsion subgroup of") + Name( theGroup ) 
		    );
         ListOf<OID> dependencies( theGroup );
         CheckinMessage( *smo, "", dependencies ).send(); 
     }
     adminTerminate();
  }
}


void AbelianComputeTorsionSubgroup::viewStructure(ostream& ostr)
     const
{
  ProblemView pv( ostr, oid(), "Compute the torsion subgroup of ",
		  Text("Compute the torsion subgroup of ")
		  + Name( theGroup ), 
		  helpID("AbelianComputeTorsionSubgroup", theGroup),
		  "Tor subgp"
		  );

  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
			 abelianInvariants.arcSlotID(),
			 50
			 );

  pv.done();
}

// ---------------------- AbelianEltPowerSubgrARCer --------------------------- //


void AbelianEltPowerSubgrARCer::setArguments( const AbelianGroup& group, const Word& w, 
                                                                 const VectorOf<Word>& sG)
{
  AG = new AbelianGroup( group );
  theWord = w;
  theSubgroup = sG;
}


void AbelianEltPowerSubgrARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void AbelianPrimesARCer::runComputation( ) : "
	   "You have The primary decomposition has not been computed yet");
#endif

  result = AG->powerOfEltInSubgroup(theWord,theSubgroup);
}


void AbelianEltPowerSubgrARCer::writeResults( ostream& out ) 
{
  out < result;
}


void AbelianEltPowerSubgrARCer::readResults( istream& in )
{
  in > result;
}


Integer AbelianEltPowerSubgrARCer::getPowerOfEltSubgrup() 
{
#if SAFETY > 0
  if( AG == 0 )
    error( "AbelianGroup AbelianInvariantsARCer::getCyclicDecomposition() : "
	   "The primary decomposition has not been computed yet");
#endif
  
  return result;
}

// -------------------------- AbelianEltPowerSubgrComp ---------------------------- //


AbelianEltPowerSubgrComp::AbelianEltPowerSubgrComp(AbelianEltPowerSubgr& sup)
    :theSupervisor( sup ),
     finished( false ),
      A(*this)
{
  AbelianGroup AG(sup.getWord().getParent().getFPGroup());
  A.setArguments( AG,sup.getWord().getWord(),sup.getSubgroup().getSubgroup().generators());
  adminStart();
}

void AbelianEltPowerSubgrComp::takeControl( )
{
  if (freeARCs()>0){
    if( A.takeControl() ) {
      LogMessage msg(*this,theSupervisor.getWord());
      Integer power = A.getPowerOfEltSubgrup();
      if (power!=0){
	Chars posfix = ordinalPostfix(power.as_long());
	msg << power << posfix << " power of " 
	    <<  Name( theSupervisor.getWord())
	    << " belongs to " << Name( theSupervisor.getSubgroup()) 
	    << " and it is minimal positive possible.";
      }
      else{
	msg << "No powers of " <<  Name( theSupervisor.getWord())
	    << " belongs to " 
	    <<  Name( theSupervisor.getSubgroup()) << ".";
      }
      msg.send();  
      finished = true;
      adminTerminate();
        return;
    } 
  usedOneARC();
  }
}

// ------------------------- AbelianEltPowerSubgr ---------------------------- //


AbelianEltPowerSubgr::AbelianEltPowerSubgr( const SMSubgroup& smg, const SMWord& smw)
  : Supervisor( true ),
	 theWord( smw ),
         theSubgroup( smg ),
         abelianEltPowerSubgr( *this )
{
}


void AbelianEltPowerSubgr::takeControl( )
{
  if (abelianEltPowerSubgr->isDone()){
     adminTerminate();
     return;
  }     
}


void AbelianEltPowerSubgr::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Compute the order of an element modulo a subgroup",
		  Text("Compute the order of ")
		  + Name( theWord ) + Text("modulo")+ Name( theSubgroup ) ,
		  helpID("AbelianEltPowerSubgr", theSubgroup.getParent()),
		  "Pow in subgp"
		  );
  
  pv.startItemGroup();

  pv.add(Text("Compute the order of ")+Name(theWord) + 
	 Text("modulo")+Name( theSubgroup ),
	 abelianEltPowerSubgr.arcSlotID(), 100);

  pv.done();
}

// -------------------- AbelianPowerProblem --------------------------- //

AbelianPowerProblem::AbelianPowerProblem(SMWord& w)
  : Supervisor( ! w.getParent().gic.havePrimaryDecomposition() ),
    theWord( w ),
    abDone( false ),
    abelianInvariants( *this, w.getParent().gcm().abelianInvariants ),
    abelianPrimes( *this, w.getParent().gcm().abelianPrimes )
{
  if ( w.getParent().gic.haveCyclicDecomposition() ) 
    if ( w.getParent().gic.getCyclicDecomposition().orderOfElt( w.getWord() ) !=0)
      resultIsFast();
    else
      abDone = true;

  if ( w.getParent().gic.havePrimaryDecomposition() )
    resultIsFast();
}


void AbelianPowerProblem::takeControl( )
{
  if ( theWord.getParent().gic.havePrimaryDecomposition() || fastResult() ) {
    LogMessage msg( *this, theWord );
    if ( theWord.getParent().gic.getCyclicDecomposition().orderOfElt( theWord.getWord() ) !=0){
      msg << Name( theWord ) << " is of finite order, hence it is a proper power.";
      msg.send();
      adminTerminate();
      return;
    }
    if (theWord.getParent().gic.getCyclicDecomposition().isEltProperPower( theWord.getWord() ))
      msg << Name( theWord ) << " is of infinite order and it is a proper power.";
    else
      msg << Name( theWord ) << " is of infinite order and it is not a proper power.";
    
    msg.send();
    adminTerminate();
    return; 
  }

}
void AbelianPowerProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Is element a proper power",
		  Text("Find is the ") + Name( theWord ) 
		  + Text(" a proper power"),
		  helpID("AbelianPowerProblem", theWord.getParent()),
		  Text("Powr prob")
		  );
  
  pv.startItemGroup();
  
  if (!abDone)
    pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
			 abelianInvariants.arcSlotID(),
			 50
			 );
  pv.add(Text("Find the primary docomposition of")+ Name( Parent( theWord ) ),
                         abelianPrimes.arcSlotID(), 50);

  pv.done();
}

// ---------------------- AbelianInvariantsOfSGARCer --------------------------- //


void AbelianInvariantsOfSGARCer::setArguments( const AbelianGroup& group,const VectorOf<Word>& sgGens )
{
  theSGGens = sgGens;
  AG = new AbelianGroup( group );
}


void AbelianInvariantsOfSGARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void AbelianInvariantsOfSGARCer::runComputation( ) : "
	   "You have The cyclic decomposition has not been computed yet");
#endif
  
  ASG = new AbelianSGPresentation(AG->makeSubgroupPresentation(theSGGens) );
  ASG->computeCyclicDecomposition();
}


void AbelianInvariantsOfSGARCer::writeResults( ostream& out ) 
{
     out < *ASG;
}


void AbelianInvariantsOfSGARCer::readResults( istream& in )
{
    ASG = new AbelianSGPresentation();
    in > *ASG;
}


AbelianSGPresentation AbelianInvariantsOfSGARCer::getSGCyclicDecomposition() 
{
#if SAFETY > 0
  if( ASG == 0 )
    error( "AbelianInvariantsOfSGARCer::getSGCyclicDecomposition() : "
	   "The cyclic decomposition has not been computed yet");
#endif
  
  return *ASG;
}


// -------------------------- AbelianSGInvariants ---------------------------- //


AbelianSGInvariants::AbelianSGInvariants(SCM& scm)
  : theSCM( scm ),    
    presentationStarted( false ),
    arcer( *this )
{
  adminStart();
}


void AbelianSGInvariants::takeControl( )
{
  if (freeARCs()>0){
   if (theSCM.getSubgroup().getParent().gic.haveCyclicDecomposition()){
     if (!presentationStarted){
         arcer.setArguments( theSCM.getSubgroup().getParent().gic.getCyclicDecomposition(),
                             theSCM.getSubgroup().getSubgroup().generators());
         presentationStarted = true;   
     }
    
     if( arcer.takeControl() ) {
        theSCM.getSubgroup().sic.putHaveCyclicDecomposition( arcer.getSGCyclicDecomposition() );
            LogMessage msg(*this,theSCM.getSubgroup());
            msg << "The canonical decomposition of " << Name( theSCM.getSubgroup() ) << " :";
            msg << arcer.getSGCyclicDecomposition(); 
            msg << ".";
            msg.send();       
            LogMessage msg1(*this,theSCM.getSubgroup());
            msg1 << Link(Chars( "Click here to see the generators of the canonical decomposition of ")+
                          Text( Name(theSCM.getSubgroup()) ),
		          "CyclicDecomposition",
			   arcer.getSGCyclicDecomposition().getFileName() );
	    msg1.send();  
            adminTerminate();
            return;
      } 
    }
    usedOneARC();
  }
}

// ------------------- AbelianSGCyclicDecomposition -----------------------//

AbelianSGCyclicDecomposition::AbelianSGCyclicDecomposition(SMSubgroup& S)
  : Supervisor( true ),
	 theSubgroup( S ),
         abDone(false),
         abelianInvariants( *this, S.getParent().gcm().abelianInvariants ),
         abelianSGInvariants( *this, S.scm().abelianSGInvariants )
{
  if (theSubgroup.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if( theSubgroup.sic.haveCyclicDecomposition( ) ){
    resultIsFast();
  }
}


void AbelianSGCyclicDecomposition::takeControl( )
{
  if (fastResult()){
     LogMessage msg;
     msg << "The canonical decomposition of " << Name( theSubgroup ) << " :";
     msg << theSubgroup.sic.getSGPresentation(); 
     msg << ".";
     msg.send( );
     adminTerminate();
     return;
  }     
   if (theSubgroup.sic.haveCyclicDecomposition( )){
     adminTerminate();
  }
}


void AbelianSGCyclicDecomposition::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Compute the canonical decomposition of a subgroup",
		  Text("Compute the canonical decomposition of")
		  + Name( theSubgroup ),
		  helpID("AbelianSGCyclicDecomposition", 
			 theSubgroup.getParent()), "Can dec"
		  );
  
  pv.startItemGroup();

  if (!abDone)
       pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup ) ),
                                    abelianInvariants.arcSlotID(), 50);

  pv.add(Text("Compute abelian invariants of") + Name( theSubgroup )
                                   , abelianSGInvariants.arcSlotID(), 50);

  pv.done();
}
// ---------------------- AbelianPrimesOfSGARCer --------------------------- //


void AbelianPrimesOfSGARCer::setArguments( const AbelianSGPresentation& subGroup )
{

  ASG = new AbelianSGPresentation(subGroup);
}


void AbelianPrimesOfSGARCer::runComputation( )
{
#if SAFETY > 0
  if( ASG == 0 )
    error( "void AbelianPrimesOfSGARCer::runComputation( ) : "
	   "You have The primary decomposition has not been computed yet");
#endif
  
  ASG->findPrimaryBasis();
}


void AbelianPrimesOfSGARCer::writeResults( ostream& out ) 
{
     out < *ASG;
}


void AbelianPrimesOfSGARCer::readResults( istream& in )
{
    in > *ASG;
}


AbelianSGPresentation AbelianPrimesOfSGARCer::getSGPrimaryDecomposition() 
{
#if SAFETY > 0
  if( ASG == 0 )
    error( "AbelianSGPresentation AbelianPrimesOfSGARCer::getSGPrimaryDecomposition() : "
	   "The primary decomposition has not been computed yet");
#endif
  
  return *ASG;
}


// -------------------------- AbelianSGPrimes ---------------------------- //


AbelianSGPrimes::AbelianSGPrimes(AbelianSGPrimesDecomposition& sup)
  : theSupervisor( sup ),    
    primesStarted( false ),
    arcer( *this )
{
  adminStart();
}


void AbelianSGPrimes::takeControl( )
{
  if (freeARCs()>0){
   if (theSupervisor.getSubgroup().sic.haveCyclicDecomposition()){
     if (!primesStarted){
         arcer.setArguments( theSupervisor.getSubgroup().sic.getSGPresentation() );
         primesStarted = true;   
     }
    
     if( arcer.takeControl() ) {
        theSupervisor.getSubgroup().sic.putHavePrimaryDecomposition( arcer.getSGPrimaryDecomposition() );
            LogMessage msg(*this,theSupervisor.getSubgroup());
            msg << "The primary decomposition of " << Name( theSupervisor.getSubgroup() ) << " :";
            arcer.getSGPrimaryDecomposition().printPrimaryDec(msg); 
            msg << ".";
            msg.send();       
            LogMessage msg1(*this,theSupervisor.getSubgroup());
            msg1 << Link(Chars( "Click here to see the generators of the primary decomposition of ")+
                           Text(Name(theSupervisor.getSubgroup())),
		          "PrimaryDecomposition",
			   arcer.getSGPrimaryDecomposition().getFileNameOfPDGens() );
	    msg1.send();  
            adminTerminate();
            return;
      } 
    }
    usedOneARC();
  }
}

// ------------------- AbelianSGPrimesDecomposition -----------------------//

AbelianSGPrimesDecomposition::AbelianSGPrimesDecomposition(SMSubgroup& S)
  : Supervisor( true ),
	 theSubgroup( S ),
         abSGDone(false),
         abDone(false),
         abelianInvariants( *this, S.getParent().gcm().abelianInvariants ),
         abelianSGInvariants( *this, S.scm().abelianSGInvariants ),
         abelianSGPrimes( *this )
{
  if (theSubgroup.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if( theSubgroup.sic.haveCyclicDecomposition( ) )
        abSGDone = true;
  if( theSubgroup.sic.havePrimaryDecomposition( ) )
       resultIsFast();
}


void AbelianSGPrimesDecomposition::takeControl( )
{
  if (fastResult()){
     LogMessage msg;
     msg << "The primary decomposition of " << Name( theSubgroup ) << " :";
     theSubgroup.sic.getSGPresentation().printPrimaryDec(msg); 
     msg << ".";
     msg.send( );
     adminTerminate();
     return;
  }     
   if (theSubgroup.sic.havePrimaryDecomposition( )){
     adminTerminate();
  }
}


void AbelianSGPrimesDecomposition::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), 
		  "Compute the primary decomposition of a subgroup",
		  Text("Compute the primary decomposition of")
		  + Name( theSubgroup ),
		  helpID("AbelianSGPrimesDecomposition", 
			 theSubgroup.getParent()), "Prime dec"
		  );
  
  pv.startItemGroup();

  if (!abDone)
       pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup ) ),
                                        abelianInvariants.arcSlotID(), 50);
  if (!abSGDone)
       pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
                                        abelianSGInvariants.arcSlotID(), 50);

  pv.add(Text("Find the primary decomposition of") + Name(theSubgroup ),
                                        abelianSGPrimes.arcSlotID(), 50);

  pv.done();
}
// ------------------- AbelianSGOrder -----------------------//

AbelianSGOrder::AbelianSGOrder(SMSubgroup& S)
  : Supervisor( true ),
	 theSubgroup( S ),
         abDone(false),
         abelianInvariants( *this, S.getParent().gcm().abelianInvariants ),
         abelianSGInvariants( *this, S.scm().abelianSGInvariants )
{
  if (theSubgroup.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if( theSubgroup.sic.haveCyclicDecomposition( ) )
       resultIsFast();
}


void AbelianSGOrder::takeControl( )
{
   if (theSubgroup.sic.haveCyclicDecomposition( )){
     Integer theOrder = theSubgroup.sic.getSGPresentation().order();
     PrimeNumbers pn;
     LogMessage msg;
     msg << "The order of " << Name( theSubgroup ) << " is ";
	 if( sign(theOrder) == 0 )
	   msg << "infinite";
	 else {
   	     msg << theOrder;
          if (theOrder > 3){
  	     msg << "=";
             pn.printPrimeDecom(msg,theOrder);
           }
         }
	 msg << ": this follows from the canonical decomposition of "
	     << Name( theSubgroup ) << ".";
     msg.send( );
     adminTerminate();
  }
}


void AbelianSGOrder::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Compute the order of a subgroup",
		  Text("Compute the order of")
		  + Name( theSubgroup ),
		  helpID("AbelianSGOrder", theSubgroup.getParent()),
		  "Order"
		  );
  
  pv.startItemGroup();

  if (!abDone)
       pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup ) ),
                            abelianInvariants.arcSlotID(), 50);
  pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
                            abelianSGInvariants.arcSlotID(), 50);
 
  pv.done();
}
// ---------------------- AbelianMaximalRootARCer --------------------------- //


void AbelianMaximalRootARCer::setArguments( const AbelianGroup& group, const Word& w)
{
  AG = new AbelianGroup( group );
  theWord = w;
}


void AbelianMaximalRootARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void AbelianMaximalRootARCer::runComputation( ) : "
	   "You have The primary decomposition has not been computed yet");
#endif

 Word w;
 Integer p;
 AG->abelianMaximalRoot(theWord,theRoot,thePower);

}


void AbelianMaximalRootARCer::writeResults( ostream& out ) 
{
  out < theRoot;
  out < thePower;
}


void AbelianMaximalRootARCer::readResults( istream& in )
{
  in > theRoot;
  in > thePower;
}


Integer AbelianMaximalRootARCer::getPower() const
{
#if SAFETY > 0
  if( AG == 0 )
    error( "AbelianGroup AbelianInvariantsARCer::getCyclicDecomposition() : "
	   "The primary decomposition has not been computed yet");
#endif
  
  return thePower;
}
Word AbelianMaximalRootARCer::getRoot() const
{
#if SAFETY > 0
  if( AG == 0 )
    error( "AbelianGroup AbelianInvariantsARCer::getCyclicDecomposition() : "
	   "The primary decomposition has not been computed yet");
#endif
  
  return theRoot;
}
// -------------------------- AbelianMaximalRootComp ---------------------------- //


AbelianMaximalRootComp::AbelianMaximalRootComp(AbelianMaximalRoot& sup)
    :theSupervisor( sup ),
     rootStarted( false ),
      A(*this)
{
  adminStart();
}

void AbelianMaximalRootComp::takeControl( )
{
  if (freeARCs()>0){
    if (theSupervisor.getWord().getParent().gic.haveCyclicDecomposition())
      if (theSupervisor.getWord().getParent().gic.
	  getCyclicDecomposition().orderOfElt(theSupervisor.getWord().
					      getWord() )   !=  0  ){
	
	LogMessage msg(*this,theSupervisor.getWord());
	msg << Name( theSupervisor.getWord() ) << 
	  " is of finite order, so it does not have a maximal root.";
	msg.send();   
	theSupervisor.getWord().wic.putHaveMaximalRoot(Word(),0);
	adminTerminate();
	return;
      }   
    
    if (theSupervisor.getWord().getParent().gic.havePrimaryDecomposition()){
      if (!rootStarted){
	A.setArguments( theSupervisor.getWord().getParent().
			gic.getCyclicDecomposition(),
			theSupervisor.getWord().getWord()  );
	rootStarted = true;   
      }
      
      if( A.takeControl() ) {
	LogMessage msg(*this,theSupervisor.getWord());
	if (A.getPower() == 0)
	  msg << Name(theSupervisor.getWord() ) << 
	    " is of infinite order and it does not have a maximal root.";
	else{
	  msg << Name( theSupervisor.getWord() ) << 
	    " is of infinite order, it's maximal root is ";
	  theSupervisor.getWord().getParent().getFPGroup().
	    printWord(msg,A.getRoot());
	  msg << " and " << A.getPower() << 
	    ordinalPostfix(A.getPower().as_long()) <<
	    " power of it is " << Name(theSupervisor.getWord() );
	  msg << ".";
	}
	msg.send();  
	theSupervisor.getWord().wic.putHaveMaximalRoot(A.getRoot(),A.getPower());
	adminTerminate();
	return;
      } 
    }
    usedOneARC();
  }
}
// ------------------------- AbelianMaximalRoot ---------------------------- //


AbelianMaximalRoot::AbelianMaximalRoot(SMWord& smw)
  : Supervisor( true ),
	 theWord( smw ),
         abDone( false ),
         abPDone (false ),
         abelianInvariants( *this, smw.getParent().gcm().abelianInvariants ),
         abelianPrimes( *this, smw.getParent().gcm().abelianPrimes ),
         abelianMaximalRoot( *this )
{
  if (theWord.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if( theWord.getParent().gic.havePrimaryDecomposition( ) )
        abPDone=true;  
  if( theWord.wic.isPower( )!=dontknow )
        resultIsFast();
}


void AbelianMaximalRoot::takeControl( )
{
  if (fastResult()){
      Word w;
      Integer expon;
      LogMessage msg(*this,theWord);
      Integer order = theWord.getParent().gic.getCyclicDecomposition().orderOfElt
                  (theWord.getWord() );
      if (order!=0 )
            msg << Name(theWord ) 
		<< " is of finite order, so it does not have a maximal root.";
      else{
         if (theWord.wic.isPower( )==no)
               msg << Name(theWord ) 
		   << " is of infinite order, it does not have a maximal root.";
         else{
            theWord.wic.getMaximalRoot(w,expon);
            msg << Name( theWord ) 
		<< " is of infinite order, it's maximal root is ";
            theWord.getParent().getFPGroup().printWord(msg,w);
            msg << " and " << expon << "-th power of it is " << Name(theWord );
	    msg << ".";
         }
      }
      msg.send();      
      adminTerminate();
      return;
  }
  if (theWord.wic.isPower()!=dontknow){
     adminTerminate();
     return;
  }     
}


void AbelianMaximalRoot::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Compute the maximal root of element",
		  Text("Compute the maximal root of ")
		  + Name( theWord )  ,
		  helpID("AbelianMaximalRoot", theWord.getParent()), 
		  "Max root"
		  );
  
  pv.startItemGroup();
  
  if (!abDone)
     pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
                            abelianInvariants.arcSlotID(), 50);
  if (!abPDone)
     pv.add(Text("Find the primary decomposition of")+Name( Parent( theWord ) ),
                            abelianPrimes.arcSlotID(), 50);

  pv.add("Compute the maximal root", abelianMaximalRoot.arcSlotID(), 100);

  pv.done();
}
// ------------------- AbelianIsIsomorphicSG -----------------------//

AbelianIsIsomorphicSG::AbelianIsIsomorphicSG(SMSubgroup& S1,SMSubgroup& S2)
  : Supervisor( true ),
	 theSubgroup1( S1 ),
	 theSubgroup2( S2 ),
         abDone(false),
         sg1abDone( false ),
         sg2abDone( false ),
         abelianInvariants( *this, S1.getParent().gcm().abelianInvariants ),
         abelianSG1Invariants( *this, S1.scm().abelianSGInvariants ),
         abelianSG2Invariants( *this, S2.scm().abelianSGInvariants )
{
  if (theSubgroup1.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if( theSubgroup1.sic.haveCyclicDecomposition( ) )
        sg1abDone = true;
  if( theSubgroup2.sic.haveCyclicDecomposition( ) )
        sg2abDone = true;
  if (sg2abDone && sg1abDone)
       resultIsFast();
}


void AbelianIsIsomorphicSG::takeControl( )
{
   if (theSubgroup1.sic.haveCyclicDecomposition( ) && theSubgroup2.sic.haveCyclicDecomposition( ) ){
    LogMessage msg(*this,theSubgroup1,theSubgroup2);
    if (theSubgroup1.sic.getSGPresentation().isomorphicTo(theSubgroup2.sic.getSGPresentation()) )
       msg << Name( theSubgroup1 ) 
	   << " is isomorphic to " 
	   << Name( theSubgroup2 ) << ".";
    else
       msg << Name( theSubgroup1 ) 
	   << " is not isomorphic to " 
	   << Name( theSubgroup2 ) << ".";

    msg.send( );
    adminTerminate();
   }
}


void AbelianIsIsomorphicSG::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Isomorphism problem",
                  Text("Is ") + Name( theSubgroup1 ) + 
                  Text(" isomorphic to ") +  Name( theSubgroup2 ),
		  helpID("AbelianIsIsomorphicSG", theSubgroup1.getParent()),
		  "Is isom"
		  );
  
  pv.startItemGroup();

  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup1 ) ),
                            abelianInvariants.arcSlotID(), 50);
  if (!sg1abDone)
    pv.add(Text("Compute abelian invariants of") + Name( theSubgroup1 ),
                            abelianSG1Invariants.arcSlotID(), 50);
  if (!sg2abDone)
    pv.add(Text("Compute abelian invariants of") + Name( theSubgroup2 ),
                            abelianSG2Invariants.arcSlotID(), 100);
  pv.done();
}
//------------------- AbelianTorsionFreeRankOfSG -------------------------//


AbelianTorsionFreeRankOfSG::AbelianTorsionFreeRankOfSG(SMSubgroup& SG)
  : Supervisor( ! SG.sic.haveCyclicDecomposition() ),
	 theSubgroup( SG ),
 	 abelianInvariants( *this, SG.getParent().gcm().abelianInvariants ),
 	 abelianSGInvariants( *this, SG.scm().abelianSGInvariants ),
         abDone( false )
{
  if ( theSubgroup.getParent().gic.haveCyclicDecomposition() ) 
           abDone = true;
  if ( theSubgroup.sic.haveCyclicDecomposition() ) 
           resultIsFast();
}


void AbelianTorsionFreeRankOfSG::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) {

	 LogMessage msg( *this, theSubgroup );
	 msg <<	"The torsion-free rank of " << Name( theSubgroup ) << " is "
	     << theSubgroup.sic.getSGPresentation().rankOfFreeAbelianFactor( )
	     << ": this follows from the canonical decomposition of "
	     << Name(theSubgroup) << "."  ;

	 msg.send();

	 adminTerminate();
  }
}


void AbelianTorsionFreeRankOfSG::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Find the torsion-free rank of a subgroup",
		  Text("Find the torsion-free rank of ") + Name( theSubgroup ),
		  helpID("AbelianTorsionFreeRankOfSG", theSubgroup.getParent()),
		  "T-f rank"
		  );

  pv.startItemGroup();

  if (!abDone)
    pv.add( Text("Compute abelian invariants of ") + Name( theSubgroup.getParent() ),
			 abelianInvariants.arcSlotID(),
			 50
			 );

    pv.add( Text("Compute abelian invariants of subgroup ") + Name( theSubgroup ),
			 abelianSGInvariants.arcSlotID(),
			 50
			 );

  pv.done();
}
//-------------- AbelianOrderOfTheTorsionSubgroupOfSG --------------------//


AbelianOrderOfTheTorsionSubgroupOfSG::
    AbelianOrderOfTheTorsionSubgroupOfSG(SMSubgroup& SG)
    : Supervisor( ! SG.sic.haveCyclicDecomposition() ),
	 theSubgroup( SG ),
 	 abelianInvariants( *this, SG.getParent().gcm().abelianInvariants ),
 	 abelianSGInvariants( *this, SG.scm().abelianSGInvariants ),
         abDone( false )
{
  if ( theSubgroup.getParent().gic.haveCyclicDecomposition() ) 
           abDone = true;
  if ( theSubgroup.sic.haveCyclicDecomposition() ) 
           resultIsFast();
}


void AbelianOrderOfTheTorsionSubgroupOfSG::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) {
         PrimeNumbers pn;
	 Integer theOrder = theSubgroup.sic.getSGPresentation().orderOfTheTorsionSubgroup();

	 LogMessage msg( *this, theSubgroup );

	 msg <<	"The order of the torsion subgroup of " << Name( theSubgroup )
	     << " is ";
         
	 if( sign(theOrder) == 0 )
	   msg << "infinite";
	 else {
           msg << theOrder;
           if (theOrder > 3){
  	     msg << "=";
             pn.printPrimeDecom(msg,theOrder);
           }
         }
	 msg << ": this follows from the canonical decomposition of "
	     << Name( theSubgroup ) << ".";

	 msg.send();

	 adminTerminate();
  }
}

void AbelianOrderOfTheTorsionSubgroupOfSG::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Compute the order of the torsion subgroup",
		  Text("Compute the order of the torsion subgroup of ") 
		  + Name( theSubgroup ),
		  helpID("AbelianOrderOfTheTorsionSubgroupOfSG", 
			 theSubgroup.getParent()), "Ordr t-f subg"
		  );

  pv.startItemGroup();

  if (!abDone)
    pv.add( Text("Compute abelian invariants of ") + Name( theSubgroup.getParent() ),
			 abelianInvariants.arcSlotID(),
			 50
			 );

    pv.add( Text("Compute abelian invariants of ") + Name( theSubgroup ),
			 abelianSGInvariants.arcSlotID(),
			 50
			 );

  pv.done();
}

// ------------------------- EltPrimeForm ---------------------------- //


EltPrimeForm::EltPrimeForm(SMWord& w)
  : Supervisor( true ),
	 theWord( w ),
         abDone(false),
         abelianPrimes( *this, w.getParent().gcm().abelianPrimes),
         abelianInvariants( *this, w.getParent().gcm().abelianInvariants )
{
  if (theWord.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if( theWord.getParent().gic.havePrimaryDecomposition( ) ){
    resultIsFast();
  }
}


void EltPrimeForm::takeControl( )
{
   if (theWord.getParent().gic.havePrimaryDecomposition( )){
     LogMessage msg(*this,theWord);
     msg << "The primary decomposition of " << Name( theWord ) << ": ";
     theWord.getParent().gic.getCyclicDecomposition().printInPrimaryForm( msg,
         theWord.getParent().gic.getCyclicDecomposition().findEltPrimeForm(theWord.getWord()));
     msg <<".";
     msg.send();
     adminTerminate();
     return;
  }     
}


void EltPrimeForm::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Compute the primary decomposition of an element",
		  Text("Compute the primary decomposition of") + Name( theWord ),
		  helpID("EltPrimeForm", theWord.getParent()),
		  "Prime form"
		  );
  
  pv.startItemGroup();

  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + 
	   Name( Parent( theWord ) ),
	   abelianInvariants.arcSlotID(), 50);
  
  pv.add(Text("Find the primary decomposition of") + 
	 Name( Parent( theWord ) ),
	 abelianPrimes.arcSlotID(), 50);
  
  pv.done();
}
// ------------------------- virtualFreeCompARCer ------------------------ //

void virtualFreeCompARCer::setArguments(  const AbelianGroup& group,  
					  const VectorOf<Word>& sG)
{
  AG = new AbelianGroup( group );
  theSubgroup = sG;
}


void virtualFreeCompARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void virtualFreeCompARCer::runComputation( ) : "
	   "The canonical decomposition has not been computed yet");
#endif
  
  result = AG->findVirtFreeComplementOfSG( theSubgroup );
}


void virtualFreeCompARCer::writeResults( ostream& out ) 
{
  out < result;
}


void virtualFreeCompARCer::readResults( istream& in )
{
  in > result;
}

// ------------------- virtualFreeComp ------------------------ //

virtualFreeComp::virtualFreeComp(SMSubgroup& s)
  : Supervisor( true ),
    theSubgroup( s ),
    abDone(false),
    arcer( *this ),
    started( false ),
    abelianInvariants( *this, s.getParent().gcm().abelianInvariants )
{
  if (theSubgroup.getParent().gic.haveCyclicDecomposition())
    abDone = true;
}

void virtualFreeComp::takeControl( )
{
  if (freeARCs()>0){
    if (theSubgroup.getParent().gic.haveCyclicDecomposition( )){
      if ( !started ) {
	
	const AbelianGroup& A = 
	  theSubgroup.getParent().gic.getCyclicDecomposition();
	
	arcer.setArguments( A, theSubgroup.getSubgroup().generators());
	started = true;
      }
      if ( arcer.takeControl() ) {
	LogMessage msg(*this,theSubgroup);
	SGofFreeGroup tmpSG( theSubgroup.getParent().getFreePreimage(),
			     arcer.getVFComplement() );
	
	if (tmpSG.generators().length() == 0){
	  msg << Name(theSubgroup ) 
	      << " does not have a non-trivial virtual free complement.";
	  msg.send();
	}
	else{
	  msg << "Found a virtual free complement of " 
	      << Name(theSubgroup ) << ".";
	  msg.send();
	  SMObject* smo = new SMSubgroup( theSubgroup.getParent(),
					  tmpSG,
					  Text("Virtual free complement of") + 
					  Name( theSubgroup ) 
					  );
	  ListOf<OID> dependencies( theSubgroup.getParent() );
	  CheckinMessage( *smo, "", dependencies ).send();
	}
	
	adminTerminate();
	return;
      } 
      
    }
    usedOneARC();
  }
}


void virtualFreeComp::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Find a virtual free complement of a subgroup.",
		  Text("Compute a virtual free complement of")
		  + Name( theSubgroup ),
		  helpID("virtualFreeComp", theSubgroup.getParent()),
		  "Virt comp"
		  );
  
  pv.startItemGroup();
  
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + 
	   Name( Parent( theSubgroup ) ),
	   abelianInvariants.arcSlotID(), 50);
  
  pv.add(Text("Compute a virtual free complement of") +
	 Name( theSubgroup ),
	 ThisARCSlotID(), 50);
  
  pv.done();
}

// ------------------------- SubgroupIsolatorARCer ------------------------- //

void SubgroupIsolatorARCer::setArguments( const AbelianGroup& group,  
					  const VectorOf<Word>& sG)
{
  AG = new AbelianGroup( group );
  theSubgroup = sG;
}


void SubgroupIsolatorARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void SubgroupIsolatorARCer::runComputation( ) : "
	   "The canonical decomposition has not been computed yet");
#endif
  
  result = AG->findSubgroupIsolator( theSubgroup );
}


void SubgroupIsolatorARCer::writeResults( ostream& out ) 
{
  out < result;
}


void SubgroupIsolatorARCer::readResults( istream& in )
{
  in > result;
}

// ------------------------- SubgroupIsolator ---------------------------- //


SubgroupIsolator::SubgroupIsolator(SMSubgroup& s)
  : Supervisor( true ),
    theSubgroup( s ),
    abDone(false),
    arcer( *this ),
    abelianInvariants( *this, s.getParent().gcm().abelianInvariants ),
    started( false )
  
{
  if (theSubgroup.getParent().gic.haveCyclicDecomposition())
    abDone = true;
}


void SubgroupIsolator::takeControl( )
{
  if (freeARCs()>0){
    if (theSubgroup.getParent().gic.haveCyclicDecomposition( )){
      if ( !started ) {
	
	const AbelianGroup& A = 
	  theSubgroup.getParent().gic.getCyclicDecomposition();
	
	arcer.setArguments( A, theSubgroup.getSubgroup().generators());
	started = true;
      }

      if ( arcer.takeControl() ) {
	LogMessage msg(*this,theSubgroup);
	SGofFreeGroup tmpSG( theSubgroup.getParent().getFreePreimage(),
			     arcer.getIsolator() );
	
	if (tmpSG.generators().length() == 0){
	  msg << Name(theSubgroup ) 
	      << " does not have a non-trivial isolator.";
	  msg.send();
	}
	else{
	  msg << "Found the isolator of " << Name(theSubgroup ) << ".";
	  msg.send();
	  SMObject* smo = new SMSubgroup( theSubgroup.getParent(),
					  tmpSG,
					  Text("The isolator of") + 
					  Name( theSubgroup ) 
					  );
	  ListOf<OID> dependencies( theSubgroup.getParent() );
	  CheckinMessage( *smo, "", dependencies ).send(); 
	}
	adminTerminate();
  	return;
      }
    }
    usedOneARC();
  }
}


void SubgroupIsolator::viewStructure(ostream& ostr) const
{
  char *temp;
  
  ProblemView pv( ostr, oid(), "Find subgroup isolator.",
		  Text("Compute the subgroup isolator of")
		  + Name( theSubgroup ),
		  helpID("SubgroupIsolator", theSubgroup.getParent()),
		  "Isolator"
		  );
  
  pv.startItemGroup();
  
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + 
	   Name( Parent( theSubgroup ) ),
	   abelianInvariants.arcSlotID(), 50);
  
  pv.add(  Text("Compute the subgroup isolator of") +
	   Name( theSubgroup ),
	   ThisARCSlotID(), 50);
  
  
  pv.done();
}

// ---------------------- AbelianSGPurityARCer --------------------------- //


void AbelianSGPurityARCer::runComputation( )
{
  isPure = computePurity( );
}


void AbelianSGPurityARCer::writeResults( ostream& out ) 
{
  out < isPure;
}


void AbelianSGPurityARCer::readResults( istream& in )
{
  in > isPure;
}


bool AbelianSGPurityARCer::isPureSubgroup() const
{
  return isPure;
}

bool AbelianSGPurityARCer::computePurity() const
{
  // get generators of the cyclic decomposition of a subgroup 
  VectorOf<AbelianWord> cyclicGens = 
    theSubgroup.sic.getSGPresentation().newToOldGens();
  
  Word tmpWord;
  for (int i=0;i<cyclicGens.length();i++){
    // transfer a cyclic generator to the group generators
    tmpWord = 
      theSubgroup.sic.getSGPresentation().
      fromSGPGensToSGGens(cyclicGens[i].getWord());
    
    // If cyclic subgroup generated by the generator of
    // the cyclic decomposition of a subgroup not pure, than 
    // the whole subgroup is not pure.
    if (!theSubgroup.getParent().gic.getCyclicDecomposition().
	isPureCyclSubgroup(tmpWord))
      return false;
  }
  return true;
}



// ------------------- AbelianSGPurityProblem -----------------------//

AbelianSGPurityProblem::AbelianSGPurityProblem(SMSubgroup& sms)
  : Supervisor( true ),
	 theSubgroup( sms ),
         abDone(false),
         PDDone(false),
         sgAbDone(false),
         abelianInvariants( *this, sms.getParent().gcm().abelianInvariants ),
         abelianPrimes( *this, sms.getParent().gcm().abelianPrimes ),
         abelianSGInvariants( *this, sms.scm().abelianSGInvariants ),
    //         abelianSGPurityCM(*this)
         arcer(*this, sms)
{
  if (theSubgroup.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if (theSubgroup.getParent().gic.havePrimaryDecomposition())
        PDDone = true;
  if( theSubgroup.sic.haveCyclicDecomposition( ) )
        sgAbDone = true;
  if( theSubgroup.sic.isPure( )!= dontknow )
       resultIsFast();
}


void AbelianSGPurityProblem::takeControl( )
{
  if (freeARCs()>0){
      if (theSubgroup.sic.haveCyclicDecomposition() && 
	  theSubgroup.getParent().gic.havePrimaryDecomposition() ){
	  if (arcer.takeControl( ) ){
           theSubgroup.sic.putIsPure(arcer.isPureSubgroup() );
	  }     
      }
     usedOneARC();
  }
   if (theSubgroup.sic.isPure( ) != dontknow ){
           LogMessage msg(*this,theSubgroup);
           if (theSubgroup.sic.isPure( )== yes )
               msg << "The subgroup " << Name( theSubgroup ) << " is pure.";
           if (theSubgroup.sic.isPure( )== no )
               msg << "The subgroup " << Name( theSubgroup ) << " is not pure.";          
           msg.send( );
           adminTerminate();
           return;
   }
}


void AbelianSGPurityProblem::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Is the subgroup pure",
		  Text("Is the subgroup ")+ Name( theSubgroup ) +
                   Text(" pure"),
		  helpID("AbelianSGPurityProblem", theSubgroup.getParent()),
		  "Pure"
		  );
  
  pv.startItemGroup();

  if (!abDone)
       pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup ) ),
                            abelianInvariants.arcSlotID(), 50);
  if (!PDDone)
      pv.add(Text("Find the primary decomposition of") + Name( Parent( theSubgroup ) ),
                            abelianPrimes.arcSlotID(), 50);
  if (!sgAbDone)
       pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
                            abelianSGInvariants.arcSlotID(), 50);

  pv.add("For use of this problem", ThisARCSlotID(), 50);

  pv.done();
}

// ------------------- AbelianDoesGensSummand -----------------------//

AbelianDoesGensSummand::AbelianDoesGensSummand(SMWord& smw)
  : Supervisor( true ),
	 theWord( smw ),
         abDone(false),
         abelianInvariants( *this, smw.getParent().gcm().abelianInvariants ),
         abelianPrimes( *this, smw.getParent().gcm().abelianPrimes )
{
  if (theWord.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if (theWord.getParent().gic.havePrimaryDecomposition())
       resultIsFast();
}


void AbelianDoesGensSummand::takeControl( )
{
   if( theWord.getParent().gic.havePrimaryDecomposition() ){
           bool result = theWord.getParent().gic.getCyclicDecomposition().
                                              isPureCyclSubgroup(theWord.getWord());
           LogMessage msg(*this,theWord);
           msg << Name( theWord );
	   if (result)
               msg << " generates ";
           else
               msg << " does not generate ";
	   msg << " a direct summand.";
           msg.send( );
           adminTerminate();
           return;
   }
}


void AbelianDoesGensSummand::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), 
		  "Does the word generate a direct summand",
		  Text("Does ") + Name( theWord ) + Text(" generate a direct summand."),
		  helpID("AbelianDoesGensSummand", 
			 theWord.getParent()),
		  "Gens direct sumnd?"
		  );
  
  pv.startItemGroup();

  if (!abDone)
     pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
                            abelianInvariants.arcSlotID(), 50);

     pv.add(Text("Find the primary decomposition of") + Name( Parent( theWord ) ),
                            abelianPrimes.arcSlotID(), 50);

  pv.done();
}

// ------------------- AbelianSGGenedByWordPurityProblem -----------------------//

AbelianSGGenedByWordPurityProblem::AbelianSGGenedByWordPurityProblem(SMWord& smw)
  : Supervisor( true ),
	 theWord( smw ),
         abDone(false),
         abelianInvariants( *this, smw.getParent().gcm().abelianInvariants ),
         abelianPrimes( *this, smw.getParent().gcm().abelianPrimes )
{
  if (theWord.getParent().gic.haveCyclicDecomposition())
        abDone = true;
  if (theWord.getParent().gic.havePrimaryDecomposition())
       resultIsFast();
}


void AbelianSGGenedByWordPurityProblem::takeControl( )
{
   if( theWord.getParent().gic.havePrimaryDecomposition() ){
           bool result = theWord.getParent().gic.getCyclicDecomposition().
                                              isPureCyclSubgroup(theWord.getWord());
           LogMessage msg(*this,theWord);
           if (result)
               msg << "The subgroup generated by  " << Name( theWord ) << " is pure.";
           else
                msg << "The subgroup generated by  " << Name( theWord ) << " is not pure.";
           msg.send( );
           adminTerminate();
           return;
   }
}


void AbelianSGGenedByWordPurityProblem::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), 
		  "Is the subgroup generated by a word pure",
		  Text("Compute is the subgroup generated by ")
		  + Name( theWord ) + Text(" pure"),
		  helpID("AbelianSGGenedByWordPurityProblem", 
			 theWord.getParent()),
		  "Is SG pure"
		  );
  
  pv.startItemGroup();

  if (!abDone)
     pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
                            abelianInvariants.arcSlotID(), 50);

     pv.add(Text("Find the primary decomposition of") + Name( Parent( theWord ) ),
                            abelianPrimes.arcSlotID(), 50);

  pv.done();
}
// ------------------- AbelianSGEqualityProblem ------------------------- //


AbelianSGEqualityProblem::AbelianSGEqualityProblem(SMSubgroup& S1, SMSubgroup& S2 )
  : Supervisor( true ),
    subgroup1( S1 ),
    subgroup2( S2 ),
    s1AbDone( false ),
    s2AbDone( false ),
    s1AbelianInvariantsOfFactor( *this, S1.scm().abelianInvariantsOfFactor ),
    s2AbelianInvariantsOfFactor( *this, S2.scm().abelianInvariantsOfFactor )
{
 if( subgroup1.sic.haveCyclicDecompositionOfFactor() )
    s1AbDone = true;
 if( subgroup2.sic.haveCyclicDecompositionOfFactor() )
    s2AbDone = true;
 if (s1AbDone && s2AbDone)
    resultIsFast();
}


void AbelianSGEqualityProblem::takeControl( )
{
  if ( subgroup1.sic.haveCyclicDecompositionOfFactor() && 
                     subgroup2.sic.haveCyclicDecompositionOfFactor() ) {
    
    const AbelianGroup& A1 = subgroup1.sic.getCyclicDecompositionOfFactor();
    const AbelianGroup& A2 = subgroup2.sic.getCyclicDecompositionOfFactor();

    bool answer = yes;
    VectorOf<Word> V1 = subgroup1.getSubgroup().generators();
    VectorOf<Word> V2 = subgroup2.getSubgroup().generators();
    
    for( int i = 0; i < V1.length(); ++i )
      if( !A2.isTrivial( V1[i] ) ) {
	answer = no;
	break;
      }
    for( int i = 0; i < V2.length(); ++i )
      if( !A1.isTrivial( V2[i] ) ) {
	answer = no;
	break;
      }
    
    LogMessage msg( *this, subgroup1, subgroup2 );
    
    msg << Name( subgroup1 );
    
    if( !answer )
      msg << " is not equal to  ";
    else
      msg << " is equal to  ";
   
    msg << Name( subgroup2 ) << ".";
    
    msg.send();
    
    adminTerminate();
  }
}


void AbelianSGEqualityProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Subgroup equality problem",
		  Text("Does ") + Name( subgroup1 ) + Text(" = ")
		  + Name( subgroup2 ) + Text(" ?"), 
		  helpID("AbelianSGEqualityProblem", subgroup1.getParent()),
		  Text(Name(subgroup1)) + "=" + Name( subgroup2 ) + "?"
		  );
  
  pv.startItemGroup();

  if (!s1AbDone)
  pv.add( Text("Compute abelian invariants of ") + Name( Parent( subgroup1 ) )
	  + Text("/") + Name( subgroup1 ),
	  s1AbelianInvariantsOfFactor.arcSlotID(),
	  50
	  );
  if (!s2AbDone)
  pv.add( Text("Compute abelian invariants of ") + Name( Parent( subgroup1 ) )
	  + Text("/") + Name( subgroup2 ),
	  s2AbelianInvariantsOfFactor.arcSlotID(),
	  50
	  );

  pv.done();
}
// ---------------------- IsAbelianWordPowerOfSecondArcer --------------------------- //

void IsAbelianWordPowerOfSecondArcer::setArguments(const AbelianGroup& abGroup,
                                const SMWord& w1,const SMWord& w2)
{
   AG = new AbelianGroup(abGroup);
   theWord1 = w1.getWord();
   theWord2 = w2.getWord();
}
void IsAbelianWordPowerOfSecondArcer::runComputation( )
{
  power = AG->isPowerOfSecond(theWord1,theWord2 );
}


void IsAbelianWordPowerOfSecondArcer::writeResults( ostream& out ) 
{
  out < power;
}


void IsAbelianWordPowerOfSecondArcer::readResults( istream& in )
{
  in > power;
}


int IsAbelianWordPowerOfSecondArcer::getPower() const
{
  return power;
}


// ------------------- IsAbelianWordPowerOfSecond -----------------------//

IsAbelianWordPowerOfSecond::IsAbelianWordPowerOfSecond(SMWord& w1,SMWord& w2)
  : Supervisor( true ),
	 theWord1( w1 ),
	 theWord2( w2 ),
         abDone(false),
         arcerSet(false),
         abelianInvariants( *this, w1.getParent().gcm().abelianInvariants ),
         arcer(*this)
{
  if (theWord1.getParent().gic.haveCyclicDecomposition())
        abDone = true;
}


void IsAbelianWordPowerOfSecond::takeControl( )
{
  if (freeARCs()>0){
      if (theWord1.getParent().gic.haveCyclicDecomposition()){
	if (!arcerSet){
           arcer.setArguments(theWord1.getParent().gic.getCyclicDecomposition(),theWord1,theWord2); 
           arcerSet = true;
        }
	  if (arcer.takeControl( ) ){
              int power = arcer.getPower();
              LogMessage msg( *this, theWord1, theWord2 );
              char s[10];
              sprintf(s, "%d", power);
              if (!power)
                  msg << Name(theWord1)<<" is not a power of "<<Name(theWord2)<<".";
              else
                  msg << Name(theWord1)<<" = "<<Name(theWord2)<<"^"<<s<<".";
              msg.send();
              adminTerminate();
              return;
	  }     
      }
     usedOneARC();
  }
}


void IsAbelianWordPowerOfSecond::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Is a word a power of another",
		  Text("Is ") + Name(theWord1) + " a power of "
		  + Name(theWord2),
		  helpID("IsAbelianWordPowerOfSecond", theWord1.getParent()),
		  Text("Is") + Text(Name(theWord1)) + "a power of" 
		  + Text(Name(theWord2))+"?"
		 );
  
  pv.startItemGroup();

  if (!abDone)
     pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord1 ) ),
                            abelianInvariants.arcSlotID(), 50);
  pv.add("For use of this problem", ThisARCSlotID(), 50);

  pv.done();
}
// ------------------- AbelianHomIsMonoComp -----------------------//

AbelianHomIsMonoComp::AbelianHomIsMonoComp(MCM& mcm)
    : theMCM( mcm ),
      sgInvStarted( false),
      sgInvFinished( false),
      abSG(NULL),
      arcer(*this)
{
  adminStart();
}

void AbelianHomIsMonoComp::takeControl( )
{
  if (freeARCs()>0){
    if (theMCM.getMap().getRange().gic.haveCyclicDecomposition()){
     if (!sgInvStarted){
       arcer.setArguments( theMCM.getMap().getRange().gic.getCyclicDecomposition(),
                             theMCM.getMap().getMap().generatingImages());
         sgInvStarted = true;   
     }
     if (!sgInvFinished){
      if(arcer.takeControl() ) {
       abSG = new AbelianSGPresentation(arcer.getSGCyclicDecomposition());
       sgInvFinished = true;
     } 
    }
    if (sgInvFinished && theMCM.getMap().getDomain().gic.haveCyclicDecomposition()){
        AbelianGroup ab = theMCM.getMap().getDomain().gic.getCyclicDecomposition();
        if (abSG->rankOfFreeAbelianFactor()!=ab.rankOfFreeAbelianFactor()) {
            theMCM.getMap().mic.putIsMono(false);
	    adminTerminate();
            return;
        }
	if (abSG->invariants()!=ab.invariants())
	  theMCM.getMap().mic.putIsMono(false);
	else
	  theMCM.getMap().mic.putIsMono(true);
	adminTerminate();
	return;
    }
    }
   usedOneARC();
  }
}

// ------------------- AbelianHomIsMono -----------------------//

AbelianHomIsMono::AbelianHomIsMono(SMMap& map)
  : Supervisor( true ),
    theMap(map ),
    abRangeDone(false), 
    abDomainDone(false), 
    abelianInvariants1( *this, map.getRange().gcm().abelianInvariants ),
    abelianInvariants2( *this, map.getDomain().gcm().abelianInvariants ),
    abelianHomIsMono( *this, map.mcm().abelianHomIsMono ),
    abelianHomIsEpi( *this, map.mcm().abelianHomIsEpi )
{
  if (theMap.getRange().gic.haveCyclicDecomposition())
    abRangeDone = true;
  if (theMap.getDomain().gic.haveCyclicDecomposition())
    abDomainDone = true;
  if (theMap.mic.isMono()!=dontknow)
    resultIsFast();
}


void AbelianHomIsMono::takeControl( )
{
  if (theMap.mic.isMono()!=dontknow){
    LogMessage msg( *this, theMap );
    msg << Name(theMap);
    if (theMap.mic.isMono() == no)
      msg << " is not ";
    else
      msg << " is ";
    msg << "a monomorphism.";
    msg.send();    
    adminTerminate();
    return;
  }
}


void AbelianHomIsMono::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Is a monomorphism",
		  Text("Compute is ")+Name(theMap)+" a monomorphism. ",
		  helpID("AbelianHomIsMono", theMap.getDomain()),
		  Text("Is mono")+"?"
		 );
  
  pv.startItemGroup();

  if (!abRangeDone)
     pv.add(Text("Compute abelian invariants of") + Name( theMap.getRange() ),
                            abelianInvariants1.arcSlotID(), 50);
  if (!abDomainDone)
     pv.add(Text("Compute abelian invariants of") + Name( theMap.getDomain() ),
                            abelianInvariants2.arcSlotID(), 50);
  if (!strcmp(theMap.typeID(),"SMHomomorphism"))
      pv.add(Text("Compute the image of")+ Name(theMap.getDomain())+
	     Text(" under the")+Name(theMap),
	     abelianHomIsEpi.arcSlotID(), 50);
      
  pv.add(Text("For use of this problem") ,
	 abelianHomIsMono.arcSlotID(), 50);

  pv.done();
}
// ------------------- AbelianHomIsEpiComp -----------------------//

AbelianHomIsEpiComp::AbelianHomIsEpiComp(MCM& mcm)
    : theMCM( mcm ),
      arcer(*this)
{
  FPGroup G( theMCM.getMap().getRange().getFPGroup() );
  SetOf<Word> S;
  VectorOf<Word> v = theMCM.getMap().getMap().generatingImages();
  int vLen = v.length();
  for( int i = 0; i < vLen; ++i )
    S |= v[i];
  G.addRelators(S);
  //  AbelianGroup abg(G);
  //  abg.computeCyclicDecomposition();
  arcer.setArguments(G, false);
  adminStart();
}

void AbelianHomIsEpiComp::takeControl( )
{
   if ( freeARCs() > 0 )
    
    if( arcer.takeControl() ) {
      AbelianGroup A = arcer.getCyclicDecomposition();
      bool answer = A.isTrivial();
      theMCM.getMap().mic.putIsEpi(answer);
      if (!strcmp(theMCM.getMap().typeID(),"SMHomomorphism") && answer){
	//@am this check, because i'm using this supervisor for NilpotentGroups
	if (theMCM.getMap().getRange().gic.isAbelian() == yes)
	  theMCM.getMap().mic.putIsMono(answer);
	IsAuto IA( theMCM.getMap() );
	FEDataUpdate( IA, True() ).send();
      }
      adminTerminate();                         
    }
    else
      usedOneARC();
}

// ------------------- AbelianHomIsEpi -----------------------//

AbelianHomIsEpi::AbelianHomIsEpi(SMMap& map)
  : Supervisor( true ),
	 theMap(map ),
         abelianHomIsEpi( *this, map.mcm().abelianHomIsEpi )
{
  if (theMap.mic.isEpi()!=dontknow)
    resultIsFast();
}


void AbelianHomIsEpi::takeControl( )
{
  if (theMap.mic.isEpi()!=dontknow){
    LogMessage msg( *this, theMap );
    msg << Name(theMap);
    if (theMap.mic.isEpi() == no)
      msg << " is not ";
    else
      msg << " is ";
    msg << "an epimorphism.";
    msg.send();
    adminTerminate();
    return;
  }   
}


void AbelianHomIsEpi::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Is an epimorphism",
		  Text("Compute is the ")+Name(theMap)+" an epimorphism. ",
		  helpID("AbelianHomIsEpi", theMap.getDomain()),
		  Text("Is epi")+"?"
		 );
  
  pv.startItemGroup();

  pv.add(Text("For use of this problem") ,
                            abelianHomIsEpi.arcSlotID(), 50);

  pv.done();
}

// ------------------- AbelianHomIsAuto -----------------------//

AbelianHomIsAuto::AbelianHomIsAuto(SMHomomorphism& homo)
  : Supervisor( true ),
	 theHomo(homo ),
         abelianHomIsEpi( *this, homo.mcm().abelianHomIsEpi )
{
  if (theHomo.mic.isEpi()!=dontknow)
    resultIsFast();
}


void AbelianHomIsAuto::takeControl( )
{
  if (theHomo.mic.isEpi()!=dontknow){
    LogMessage msg( *this, theHomo );
    msg << Name(theHomo);
    if (theHomo.mic.isEpi() == no)
      msg << " is not ";
    else
      msg << " is ";
    msg << "an automorphism.";
    msg.send();    adminTerminate();
    return;
  }
}


void AbelianHomIsAuto::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Is an automorphism",
		  Text("Compute is the ")+Name(theHomo)+" an automorphism. ",
		  helpID("AbelianHomIsAuto", theHomo.getDomain()),
		  Text("Is auto")+"?"
		 );
  
  pv.startItemGroup();

  pv.add(Text("Compute is ")+Name(theHomo)+Text(" an epimorphism") ,
                            abelianHomIsEpi.arcSlotID(), 50);

  pv.done();
}
// ------------------- AbelianHomIsIso -----------------------//

AbelianHomIsIso::AbelianHomIsIso(SMHomomorphism2& homo)
  : Supervisor( true ),
	 theHomo(homo ),
         abRangeDone(false), 
         abDomainDone(false), 
         abelianInvariants1( *this, homo.getRange().gcm().abelianInvariants ),
         abelianInvariants2( *this, homo.getDomain().gcm().abelianInvariants ),
         abelianHomIsMono( *this, homo.mcm().abelianHomIsMono ),
         abelianHomIsEpi( *this, homo.mcm().abelianHomIsEpi )
{
  if (theHomo.getRange().gic.haveCyclicDecomposition())
    abRangeDone = true;
  if (theHomo.getDomain().gic.haveCyclicDecomposition())
    abDomainDone = true;
  if (theHomo.mic.isMono()!=dontknow && theHomo.mic.isEpi()!=dontknow)
    resultIsFast();
}


void AbelianHomIsIso::takeControl( )
{
  if (theHomo.mic.isMono()!=dontknow && theHomo.mic.isEpi()!=dontknow){
    LogMessage msg( *this, theHomo );
    msg << Name(theHomo);
    if (theHomo.mic.isMono() == yes && theHomo.mic.isEpi()==yes )
      msg << " is ";
    else
      msg << " is not ";
    msg << "an isomorphism.";
    msg.send();
    adminTerminate();
    return;
  }   
}


void AbelianHomIsIso::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Is isomorphism",
		  Text("Compute is the ") + Name(theHomo) + " an isomorphism. ",
		  helpID("AbelianHomIsIso", theHomo.getDomain()),
		  Text("Is iso")+"?"
		 );
  
  pv.startItemGroup();

  if (!abRangeDone)
     pv.add(Text("Compute abelian invariants of") + Name( theHomo.getRange() ),
                            abelianInvariants1.arcSlotID(), 50);
  if (!abDomainDone)
     pv.add(Text("Compute abelian invariants of") + Name( theHomo.getDomain() ),
                            abelianInvariants2.arcSlotID(), 50);

  if (theHomo.mic.isMono()==dontknow)  
    pv.add(Text("Compute is ")+ Name( theHomo) + Text(" a monomorphism") , 
	   abelianHomIsMono.arcSlotID(), 50);

   if (theHomo.mic.isEpi()==dontknow)  
    pv.add(Text("Compute is ")+ Name( theHomo) + Text(" an epimorphism") , 
	   abelianHomIsEpi.arcSlotID(), 50);

 pv.done();
}

// ---------------------- AbelianOrderOfAutoARCer --------------------------- //


void AbelianOrderOfAutoARCer::setArguments( const AbelianGroup& group, 
					    const VectorOf<Word>& images)
{
  AG = new AbelianGroup( group );
  theImages = images;
}


void AbelianOrderOfAutoARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void AbelianOrderOfAutoARCer::runComputation( ) : "
	   "Abelian group has not been computed yet");
#endif

  result = AG->orderOfAuto(theImages);
}


void AbelianOrderOfAutoARCer::writeResults( ostream& out ) 
{
  out < result;
}


void AbelianOrderOfAutoARCer::readResults( istream& in )
{
  in > result;
}

// ------------------- AbelianOrderOfAuto -----------------------//

AbelianOrderOfAuto::AbelianOrderOfAuto(const SMHomomorphism& aut)
  : Supervisor( true ),
    theAuto(aut ),
    abelianInvariants( *this, aut.getDomain().gcm().abelianInvariants ),
    arcer(*this),
    started(false)
{
  if (theAuto.mic.isEpi()!=yes)
    error("Don't know is it automorphism!!!");
}


void AbelianOrderOfAuto::takeControl( )
{
  if (freeARCs() > 0){
    if (theAuto.getDomain().gic.haveCyclicDecomposition()){
      if (!started){
	arcer.setArguments(theAuto.getDomain().gic.getCyclicDecomposition(),
			   theAuto.getMap().generatingImages());
	started = true;
      }
      if (arcer.takeControl()){
	int result = arcer.getOrder();
	LogMessage msg( *this, theAuto );
	if (result <  0 )
	  msg << "Can't find the order of " << Name(theAuto);
	else
	  msg << "The order of " << Name(theAuto) << " is " << result << ".";
	msg.send();
	adminTerminate();
	return;
      }
    }
    usedOneARC();
  }
}


void AbelianOrderOfAuto::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Compute the order of an automorphism",
		  Text("Compute the order of ")+Name(theAuto),
		  helpID("AbelianOrderOfAuto", theAuto.getDomain()),
		  Text("order")
		 );
  
  pv.startItemGroup();

  if (!theAuto.getDomain().gic.haveCyclicDecomposition() &&
      theAuto.getDomain().getFPGroup().getRelators().cardinality()!=0)
    pv.add( Text("Compute abelian invariants of") + Name( theAuto.getDomain( ) ),
	    abelianInvariants.arcSlotID(),
	    50
	    );

  pv.add("For the use of this problem", ThisARCSlotID(), 50);

  pv.done();
}
// ---------------------- AbelianInverseAutoARCer --------------------------- //


void AbelianInverseAutoARCer::setArguments( const AbelianGroup& group, 
					    const VectorOf<Word>& images)
{
  AG = new AbelianGroup( group );
  theImages = images;
}


void AbelianInverseAutoARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void AbelianInverseAutoARCer::runComputation( ) : "
	   "Abelian group has not been computed yet");
#endif

  result = AG->inverseAuto(theImages);
}


void AbelianInverseAutoARCer::writeResults( ostream& out ) 
{
  out < result;
}


void AbelianInverseAutoARCer::readResults( istream& in )
{
  in > result;
}

// ------------------- AbelianInverseAuto -----------------------//

AbelianInverseAuto::AbelianInverseAuto(const SMHomomorphism& aut)
  : Supervisor( true ),
    theAuto(aut ),
    arcer(*this)
{
  if (theAuto.mic.isEpi()!=yes)
    error("Don't know is it automorphism!!!");
  else{
    AbelianGroup abg(theAuto.getDomain().getFPGroup());
    arcer.setArguments(abg,theAuto.getMap().generatingImages());    
  }
}


void AbelianInverseAuto::takeControl( )
{
  if (freeARCs() > 0){
    if (arcer.takeControl()){
      Map inverseMap(theAuto.getDomain().getFPGroup(),
		     theAuto.getDomain().getFPGroup(),
		     arcer.getInverse());
      SMHomomorphism* smo = new SMHomomorphism( theAuto.getDomain(), 
						inverseMap,
						Text("The inverse of ")+Name(theAuto));
      IsAuto IA( *smo );
      FEDataUpdate( IA, True() ).send();
      smo->mic.putIsEpi(true);

      ListOf<OID> dependencies( theAuto.getDomain() );
      CheckinMessage( *smo, "", dependencies ).send();
      adminTerminate();
      return;
      }
    usedOneARC();
  }
}


void AbelianInverseAuto::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), "Find inverse of an automorphism",
		  Text("Find inverse of ")+Name(theAuto),
		  helpID("AbelianInverseAuto", theAuto.getDomain()),
		  Text("inverse")
		 );
  
  pv.startItemGroup();

  pv.add("For the use of this problem", ThisARCSlotID(), 50);

  pv.done();
}

// ---------------------AbelianFixedPointsOfAutoARCer----------------------- //

void AbelianFixedPointsOfAutoARCer::setArguments( const AbelianGroup& gr , 
						  const VectorOf<Word>& images )
{
  A = gr;
  theImages = images;
}

void AbelianFixedPointsOfAutoARCer::runComputation( )
{
  result = A.fixedPointsOfAuto( theImages );
}

void AbelianFixedPointsOfAutoARCer::writeResults( ostream& out ) 
{
  out < result;
}

void AbelianFixedPointsOfAutoARCer::readResults( istream& in )
{
  in > result;
}

//-----------------------AbelianFixedPointsOfAutoProblem---------------------//

AbelianFixedPointsOfAutoProblem::AbelianFixedPointsOfAutoProblem(const class SMHomomorphism& aut )
  : Supervisor( true ),
    theAuto( aut ),
    arcer( *this )
{
  if (theAuto.mic.isEpi() != yes)
    error("Don't know is it automorphism!!!");
  else
    {
      AbelianGroup a( theAuto.getDomain().getFPGroup() );
      arcer.setArguments( a , theAuto.getMap().generatingImages() );    
    }
}

void AbelianFixedPointsOfAutoProblem::takeControl()
{
  if (freeARCs() > 0)
    {
      if (arcer.takeControl())
	{
	  SMSubgroup* smo = new SMSubgroup( theAuto.getDomain() , SGofFreeGroup( theAuto.getDomain().getFreePreimage() , arcer.getResult() ) , Text("The subgroup of fixed points of ") + Name( theAuto ) );
	  
	  ListOf<OID> dependencies( theAuto.getDomain() );
	  CheckinMessage( *smo, "", dependencies ).send();
	  
	  /*LogMessage msg1( *this , theAuto.getDomain() );
	    msg1 << " The generating set for fixed points of " << Name( theAuto ) << " : ";
	    
	    for( int i = 0 ; i < arcer.getResult().length() ; i++ )
	    {
	    theAuto.getDomain().getFPGroup().printWord( msg1 , arcer.getResult()[i] );
	    if( i != arcer.getResult().length() - 1 )
	    msg1 << " , ";
	    }
	    msg1 << " . ";
	    
	    msg1.send();*/
	  
	  adminTerminate();
	  return;
	}
      usedOneARC();
    }
}

void AbelianFixedPointsOfAutoProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Compute the subgroup of fixed points",
		  Text("Compute the fixed points of ") + Name( theAuto ),
		  helpID("AbelianFixedPointsOfAutoProblem", 
			 theAuto.getDomain()),
		  Text("fixedPoints")
		 );
  
  pv.startItemGroup();
  
  pv.add("For the use of this problem", ThisARCSlotID(), 50);

  pv.done();
}

// ---------------------- AbelianSGIntersectionARCer ----------------------- //

void AbelianSGIntersectionARCer::setArguments( const AbelianGroup& g, 
					       const VectorOf<Word>& v1,
					       const VectorOf<Word>& v2)
{
  A = g;
  s1 = v1;
  s2 = v2;
}
  
void AbelianSGIntersectionARCer::runComputation( )
{
  result = A.findSubgIntersection( s1 , s2 , file );
}

void AbelianSGIntersectionARCer::writeResults( ostream& out )
{
  out < result;
}

void AbelianSGIntersectionARCer::readResults( istream& in )
{
  in > result;
}  

// -------------------- AbelianSGIntersectionProblem ----------------------- //

AbelianSGIntersectionProblem::AbelianSGIntersectionProblem( class SMSubgroup& S1, class SMSubgroup& S2)
  : Supervisor( true ),
    subgroup1( S1 ),
    subgroup2( S2 ),
    init( false ),
    abelianInvariants( *this , S1.getParent().gcm().abelianInvariants ),
    arcer(*this)
{
  VectorOf<Word> v1 = S1.getSubgroup().generators();
  VectorOf<Word> v2 = S2.getSubgroup().generators();
  
  if( !v1.length() || !v2.length() )
    resultIsFast();
}

void AbelianSGIntersectionProblem::takeControl( )
{
  if( fastResult() )
    {
      VectorOf<Word> v1 = subgroup1.getSubgroup().generators();
      VectorOf<Word> v2 = subgroup2.getSubgroup().generators();
      
      if( !v1.length() )
	{
	  LogMessage msg1( *this , subgroup1.getParent() );
	  msg1 << Name( subgroup1 ) << " is trivial.";
	  msg1.send();
	}
	  
      if( !v2.length() )
	{
	  LogMessage msg2( *this , subgroup1.getParent() );
	  msg2 << Name( subgroup2 ) << " is trivial.";
	  msg2.send();
	}
      
      LogMessage msg3( *this , subgroup1.getParent() );
      msg3 << "Intersection of " << Name( subgroup1 ) << " and " << Name( subgroup2 ) << " is trivial.";
      msg3.send();
      
      adminTerminate();
      return;
    }

  if( subgroup1.getParent().gic.haveCyclicDecomposition() ) 
    {
      if( !init )
	{
	  AbelianGroup A = subgroup1.getParent().gic.getCyclicDecomposition( );
	  VectorOf<Word> v1 = subgroup1.getSubgroup().generators();
	  VectorOf<Word> v2 = subgroup2.getSubgroup().generators();
	  
	  arcer.setArguments( A , v1 , v2 );
	  init = true;
	}
      
      if ( freeARCs() > 0 ) 
      if( arcer.takeControl() ) 
	{
	  VectorOf<Word> result = arcer.getResult();
	  
	  LogMessage msg( *this, subgroup1.getParent() );
	  
	  if( !result.length() )
	    msg << "Intersection of " << Name( subgroup1 ) << " and " << Name( subgroup2 ) << " is trivial.";
	  else
	    msg << Link("Click here to see the generators of intersection" , "Intersection" , arcer.getFileName() );
	
	  msg.send();
	  
	  adminTerminate();
	  return;
	}
      else
	usedOneARC();
    }
}
void AbelianSGIntersectionProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Find the intersection of two subgroups in abelian group",
		  Text("Find the intersection of ") 
		  + Name( subgroup1 ) + Text(" and ") + Name( subgroup2 ) +
		  Text(" in ") + Name( subgroup1.getParent() ) ,
		  helpID("AbelianSGIntersectionProblem", subgroup1.getParent()) ,
		  Text("Intersection")
		  );
  
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( subgroup1.getParent() ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.add( Text("For the use of this problem") + Name( subgroup1.getParent() ),
	  ThisARCSlotID(),
	  100
	  );
  pv.done();
}

// ------------------- AbelianIntegralHomologyARCer ------------------------ //

void AbelianIntegralHomologyARCer::setArguments( const AbelianGroup& g,int n )
{
  A = g;
  d = n;
}
  
void AbelianIntegralHomologyARCer::runComputation( )
{
  result = A.computeIntegralHomology( d );
}

void AbelianIntegralHomologyARCer::writeResults( ostream& out )
{
  out < result;
}

void AbelianIntegralHomologyARCer::readResults( istream& in )
{
  in > result;
}  

// ----------------------- AbelianIntegralHomologyProblem -------------------------- //

AbelianIntegralHomologyProblem::AbelianIntegralHomologyProblem( class SMFPGroup& G, 
								int n = 2 )
  : Supervisor( true ),
    init( false ),
    theGroup( G ),
    d( n ),
    abelianInvariants( *this , G.gcm().abelianInvariants ),
    arcer(*this)
{  }

void AbelianIntegralHomologyProblem::takeControl( )
{
  if( theGroup.gic.haveCyclicDecomposition() ) 
    {
      if( !init )
	{
	  AbelianGroup A = theGroup.gic.getCyclicDecomposition( );
	  
	  arcer.setArguments( A , d );
	  init = true;
	}
      
      if ( freeARCs() > 0 ) 
	if( arcer.takeControl() ) 
	  {
	    AbelianGroup result = arcer.getResult();
	    result.computeCyclicDecomposition();
	    
	    LogMessage msg( *this, theGroup );
	    msg << "Dimension " << d << " integral homology of " << Name( theGroup ) << " : " << result;
	    msg.send();
	  
	    adminTerminate();
	    return;
	  }
	else
	  usedOneARC();
    }
}

void AbelianIntegralHomologyProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Integral homology for abelian group",
		  Text("Compute the dimension") + d + "homology of"
		  + Name( theGroup ),
		  helpID("AbelianHomologyProblem", theGroup),
		  "Homology"
		  );
 
  pv.startItemGroup();

  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.add( Text("For the use of this problem") + Name( theGroup ),
	  ThisARCSlotID(),
	  50
	  );
  
  pv.done();
}

