#line 1  "src/AGModule.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes AutGroupARCer, AGSupervisor, AGProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//

#include "AGModule.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "ARCer.h"

// ---------------------------AutGroupARCer -------------------------------- //

void AutGroupARCer::setArguments( const FPGroup group )
{
  G = group;
}


void AutGroupARCer::runComputation( )
{
  KBM = new KBmagPackage(G.namesOfGenerators(), G.getRelators());
  retValue = KBM->autgroup();
}

void AutGroupARCer::writeResults( ostream& out ) 
{
  out < retValue;
  if( retValue == yes )
    out < KBM->differenceMachine(1) < KBM->differenceMachine(2) 
        < KBM->wordAcceptor() < KBM->generalMultiplier();
  delete KBM;
}

void AutGroupARCer::readResults( istream& in )
{
  in > retValue;
  if( retValue == yes ) in > DM1 > DM2 > WA > GM;
}

DiffMachine AutGroupARCer::getDiffMachine1( ) 
{ 
#if SAFETY > 0
  if ( retValue != yes )
    error( "DiffMachine AutGroupARCer::getDiffMachine1( ) : "
	   "cannot give result of the computation that failed.");
#endif

  return DM1; 
}

DiffMachine AutGroupARCer::getDiffMachine2( ) 
{ 
#if SAFETY > 0
  if ( retValue != yes )
    error( "DiffMachine AutGroupARCer::getDiffMachine2( ) : "
	   "cannot give result of the computation that failed.");
#endif

  return DM2; 
}

GroupDFSA AutGroupARCer::getWordAcceptor( ) 
{
#if SAFETY > 0
  if ( retValue != yes )
    error( "GroupDFSA AutGroupARCer::getWordAcceptor( ) : "
	   "cannot give result of the computation that failed.");
#endif

  return WA; 
}

GenMult AutGroupARCer::getGeneralMultiplier( ) 
{ 
#if SAFETY > 0
  if ( retValue != yes )
    error( "GenMult AutGroupARCer::getGeneralMultiplier( ) : "
	   "cannot give result of the computation that failed.");
#endif

  return GM; 
}

// ----------------------------- AGSupervisor ------------------------------ //

AGSupervisor::AGSupervisor( class GCM& gcm )
  : theGroup( gcm.getSMFPGroup() ),
    arcer( *this )
{
  adminStart();
}

void AGSupervisor::start( )
{
  if (theGroup.gic.isAbelian()==yes) {
    CommutatorIterator I(theGroup.getFPGroup().numberOfGenerators(), 2);
    SetOf<Word> newRelators = theGroup.getFPGroup().getRelators();
    for( I.reset(); !I.done(); I.next() ) newRelators |= I.value();
    FPGroup g(theGroup.getFPGroup().namesOfGenerators(), newRelators);
    arcer.setArguments(g);
  }
  else arcer.setArguments(theGroup.getFPGroup());
}

void AGSupervisor::takeControl( )
{
  if ( freeARCs() > 0 ) 
    if( arcer.takeControl() ) {
      Trichotomy answer = arcer.isAutomatic();
      theGroup.gic.putIsAutomatic(answer);
      {
	LogMessage msg( *this, theGroup );
	msg << theGroup.gic.getAutomaticMessage();
	msg.send();
      }
      if( answer == yes ) {
	File fileWA, fileDM1, fileDM2, fileGM;
	arcer.getWordAcceptor().printOn(fileWA);
	fileWA << end;
	arcer.getDiffMachine1().printOn(fileDM1);
	fileDM1 << end;
	arcer.getDiffMachine2().printOn(fileDM2);
	fileDM2 << end;
	arcer.getGeneralMultiplier().printOn(fileGM);
	fileGM << end;
	{
	  LogMessage msg( *this, theGroup );
	  msg << "Have a ShortLex automatic structure for " << Name( theGroup )
	      << ", discovered by KBMAG, Version 1.2.";
	  msg.send();
	}
	{
	  LogMessage msg( *this, theGroup );
	  msg << Link("Click here to see a word acceptor.",
		      "AutWordAcceptor", fileWA.getFileName(), false);
	  msg.send(); 
	}
	{
	  LogMessage msg( *this, theGroup );
	  msg << Link("Click here to the first difference machine.",
		      "AutDiffMachine1", fileDM1.getFileName(), false);
	  msg.send(); 
	}
	{
	  LogMessage msg( *this, theGroup );
	  msg << Link("Click here to the second difference machine.",
		      "AutDiffMachine2", fileDM2.getFileName(), false);
	  msg.send(); 
	}
	{
	  LogMessage msg( *this, theGroup );
	  msg << Link("Click here to the general multiplier.",
		      "AutGeneralMultiplier", fileGM.getFileName(), false);
	  msg.send(); 
	}
	theGroup.gic.putHaveAutomatic( arcer.getWordAcceptor(), 
				       arcer.getDiffMachine1() );
      }
      adminTerminate();
    }
    else usedOneARC();
}

// ---------------------------- AGProblem ---------------------------------- //

AGProblem::AGProblem(SMFPGroup& G)
  : Supervisor( true ),
    theGroup( G ),
    agSupervisor( *this, G.gcm().agSupervisor )
{ 
  if( theGroup.gic.isAutomatic() != dontknow ) resultIsFast();
}

void AGProblem::takeControl( )
{
  if( fastResult() ) {
    LogMessage( theGroup.gic.getAutomaticMessage() ).send();
    adminTerminate();
    return;
  }
  if( theGroup.gcm().agSupervisor->state() == ComputationManager::TERMINATED ) adminTerminate();
}

void AGProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Seek automatic structure"),
		  Text("Seek a ShortLex automatic structure for")
		  + Name( theGroup ), helpID( "AGProblem", theGroup ),
		  Text("Aut struct") );
  pv.startItemGroup();
  pv.add(Text("For the use of this problem"), agSupervisor.arcSlotID(), 100);
  pv.done();
}
#line 1  "src/AbelianInvariants.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of classes AbelianInvariants, AbelInvariantsProb,
//           AbelianRank, AbelianWordProblem, AbelianIsomProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "AbelianInvariants.h"
#include "GCM.h"
#include "SMFPGroup.h"
#include "SMWord.h"
#include "SMSubgroup.h"
#include "viewStructure.h"
#include "AbelianInfinitenessProblem.h"
#include "OutMessages.h"


// ---------------------- AbelianInvariantsARCer --------------------------- //


void AbelianInvariantsARCer::setArguments( const FPGroup& group, 
					   bool makeFile )
{
  delete AG;
  AG = new AbelianGroup( group, makeFile );
}


void AbelianInvariantsARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void AbelianInvariantsARCer::runComputation( ) : "
	   "The cyclic decomposition has not been computed yet");
#endif

  AG->computeCyclicDecomposition();
}


void AbelianInvariantsARCer::writeResults( ostream& out ) 
{
  out < *AG;
}


void AbelianInvariantsARCer::readResults( istream& in )
{
  in > *AG;
}


AbelianGroup AbelianInvariantsARCer::getCyclicDecomposition() 
{
#if SAFETY > 0
  if( AG == 0 )
    error( "AbelianGroup AbelianInvariantsARCer::getCyclicDecomposition() : "
	   "The cyclic decomposition has not been computed yet");
#endif
  
  return *AG;
}


// -------------------------- AbelianInvariants ---------------------------- //


AbelianInvariants::AbelianInvariants(GCM& gcm)
  : theGCM( gcm ),    
    arcer( *this )
{
  arcer.setArguments( theGCM.getSMFPGroup().getFPGroup(), true );
  adminStart();
}


void AbelianInvariants::takeControl( )
{
  if ( freeARCs() > 0 )

    if( arcer.takeControl() ) {
     
      theGCM.getSMFPGroup().gic.putHaveCyclicDecomposition
	( arcer.getCyclicDecomposition() );

      adminTerminate();
    }
    else
      usedOneARC();
}


// ---------------------- AbelianInvariantsOfFactor ------------------------ //


AbelianInvariantsOfFactor::AbelianInvariantsOfFactor(SCM& scm)
  : theSCM( scm ), 
    arcer( *this )
{
  FPGroup G( theSCM.getSubgroup().getParent().getFPGroup() );
  SetOf<Word> S;
  VectorOf<Word> v = theSCM.getSubgroup().getSubgroup().generators();
  int vLen = v.length();
  for( int i = 0; i < vLen; ++i )
    S |= v[i];
  G.addRelators(S);
  
  arcer.setArguments(G, true);
  adminStart();
}


void AbelianInvariantsOfFactor::takeControl( )
{
  if ( freeARCs() > 0 )
    
    if( arcer.takeControl() ) {
      
      theSCM.getSubgroup().sic.putHaveCyclicDecompositionOfFactor
	( arcer.getCyclicDecomposition() );
      
      adminTerminate();
    }
    else
      usedOneARC();
}


// ------------------------- AbelInvariantsProb ---------------------------- //


AbelInvariantsProb::AbelInvariantsProb(SMFPGroup& G)
  : Supervisor( true ),
	 theGroup( G ),
 	 abelianInvariants( *this, G.gcm().abelianInvariants )
{
  if( theGroup.gic.haveCyclicDecomposition() )
    resultIsFast();
}


void AbelInvariantsProb::takeControl( )
{
  if( fastResult() ) {
    LogMessage( theGroup.gic.getCyclicDecompositionMessage() ).send();
    adminTerminate();
    return;
  }

  if ( theGroup.gcm().abelianInvariants->state() == TERMINATED ) {
    LogMessage( *this, theGroup.gic.getCyclicDecompositionMessage(), 2 ).send();
    adminTerminate();
  }
}


void AbelInvariantsProb::viewStructure(ostream& ostr) const
{
  char *temp;
  
  if( theGroup.getCheckinType() == SMFPGroup::ABELIAN )
    temp = "";
  else 
    temp = " abelianized";
    
  ProblemView pv( ostr, oid(), Text("Compute the canonical decomposition"),
		  Text("Compute the canonical decomposition of")
		  + Name( theGroup ) + Text(temp),
		  helpID( "AbelInvariantsProb", theGroup ),
		  "Can dec"
		  );
  
  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), abelianInvariants.arcSlotID(), 100);

  pv.done();
}


// -------------------------- AbelianRank ---------------------------------- //


AbelianRank::AbelianRank(GCM& gcm)
  : ComputationManager( false ),
	 theGroup( gcm.getSMFPGroup() ),
	 theProb( 0 )
{
  adminStart();
}


AbelianRank::~AbelianRank( ) { delete theProb; }


void AbelianRank::start()
{
  theProb = new AbelianInfinitenessProblem( theGroup.getFPGroup() );

  theProb->startComputation();
}


void AbelianRank::takeControl( )
{
  if ( freeARCs() > 0 )

    if ( theProb->done() ) {
      
      bool infinite = theProb->isInfinite();
      
      if( theGroup.gic.isAbelian() == yes || theGroup.gic.isNilpotent() == yes
	  || infinite ) {
	
	Chars text;
	
	if( theGroup.gic.isAbelian() == yes )
	  text = Text("this follows by computing the torsion-free rank of")
	    + Name( theGroup );
	else
	  text = Text("this follows by computing the torsion-free rank of"
		      " the abelianization of") + Name( theGroup );
	
	theGroup.gic.putIsFinite( !infinite, text );
      }
      
      delete theProb;
      theProb = 0;
      adminTerminate();
      
    }
    else {
      theProb->continueComputation();
      usedOneARC();
    }
}
// ---------------------- AbelianPrimesARCer --------------------------- //


void AbelianPrimesARCer::setArguments( const AbelianGroup& group )
{
  AG = new AbelianGroup( group );
}


void AbelianPrimesARCer::runComputation( )
{
#if SAFETY > 0
  if( AG == 0 )
    error( "void AbelianPrimesARCer::runComputation( ) : "
	   "You have not set the arguments yet");
#endif

  AG->findPrimaryBasis();
}


void AbelianPrimesARCer::writeResults( ostream& out ) 
{
  out < *AG;
}


void AbelianPrimesARCer::readResults( istream& in )
{
  in > *AG;
}


AbelianGroup AbelianPrimesARCer::getPrimaryDecomposition() 
{
#if SAFETY > 0
  if( AG == 0 )
    error( "AbelianGroup AbelianPrimesARCer::getPrimaryDecomposition() : "
	   	   "You have not set the arguments yet");
#endif
  
  return *AG;
}

// -------------------------- AbelianPrimes ---------------------------- //


AbelianPrimes::AbelianPrimes(GCM& gcm)
    : theGCM( gcm ),
      primesStarted( false),
      A(*this)
{
  adminStart();
}

void AbelianPrimes::takeControl( )
{
  if (freeARCs()>0){
   if (theGCM.getSMFPGroup().gic.haveCyclicDecomposition()){
     if (!primesStarted){
         A.setArguments( theGCM.getSMFPGroup().gic.getCyclicDecomposition());
         primesStarted = true;   
     }
    
     if( A.takeControl() ) {
        theGCM.getSMFPGroup().gic.putHavePrimaryDecomposition( A.getPrimaryDecomposition());
            LogMessage msg(*this,theGCM.getSMFPGroup());
            msg << "The primary decomposition of " << Name( theGCM.getSMFPGroup() ) << " :";
            A.getPrimaryDecomposition().printPrimaryDec(msg); 
            msg << ".";
            msg.send();       
            LogMessage msg1(*this,theGCM.getSMFPGroup());
            msg1 << Link(Chars( "Click here to see the generators of the primary decomposition of ")+
                          Text( Name(theGCM.getSMFPGroup()) ),
		          "PrimaryDecomposition",
			   A.getPrimaryDecomposition().getFileNameOfPDGens() );
	    msg1.send();  
            adminTerminate();
            return;
      } 
    }
    usedOneARC();
  }
}
// ------------------------- AbelianPrimeDecomp ---------------------------- //

AbelianPrimeDecomp::AbelianPrimeDecomp(SMFPGroup& G)
  : Supervisor( true ),
	 theGroup( G ),
         abDone(false),
         abelianPrimes( *this, G.gcm().abelianPrimes),
         abelianInvariants( *this, G.gcm().abelianInvariants )
{
  if( theGroup.gic.havePrimaryDecomposition( ) ){
    resultIsFast();
  }
  if (theGroup.gic.haveCyclicDecomposition() && !fastResult()){
    if (theGroup.gic.getCyclicDecomposition().isFree()){
      AbelianGroup tmpAG = theGroup.gic.getCyclicDecomposition();
      tmpAG.findPrimaryBasis();
      theGroup.gic.putHavePrimaryDecomposition( tmpAG );
      resultIsFast();
    }
    abDone = true;
  }
}


void AbelianPrimeDecomp::takeControl( )
{
  if (fastResult()){
     LogMessage msg;
     msg << "The primary decomposition of " << Name( theGroup ) << " :";
     theGroup.gic.getCyclicDecomposition().printPrimaryDec(msg); 
     msg << ".";
     msg.send( );
     adminTerminate();
     return;
  }     
   if (theGroup.gic.havePrimaryDecomposition( )){
     adminTerminate();
  }
}


void AbelianPrimeDecomp::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), Text("Compute the primary decomposition"),
		  Text("Compute the primary decomposition of")
		  + Name( theGroup ),
		  helpID("AbelianPrimeDecomp", theGroup),
		  "Prime dec"
		  );
  
  pv.startItemGroup();

  if (!abDone)
     pv.add(Text("Compute abelian invariants of") + Name( theGroup ),
                            abelianInvariants.arcSlotID(), 100);

     pv.add(Text("Find the primary decomposition of") + Name( theGroup ),
                            abelianPrimes.arcSlotID(), 100);

  pv.done();
}
#line 1  "src/KBModule.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes KBSupervisor, KBProblem
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//

#include <sys/wait.h>
#include "KBModule.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"


// ------------------------ KBSupervisorARCer ------------------------------ //


void KBSupervisorARCer::setArguments( const FPGroup& group )
{
  G = group;
}


void KBSupervisorARCer::runComputation( )
{
  // Pre-compute one of the parameters to RKBPackage:

  int maxRelLen = 0;
  SetIterator<Word> I( G.getRelators() );
  while ( !I.done() ) {
    Word w = I.value();
    int l = w.length();
    if ( l > maxRelLen ) maxRelLen = l;
    I.next();
  }
  int rkbp_par = 2 * maxRelLen;

  // Make the RKBPackage:

  RKBP = new RKBPackage(G.namesOfGenerators(), G.getRelators());

  do {
    RKBP->runKB(rkbp_par, -1, rkbp_par, rkbp_par);
    rkbp_par += (rkbp_par >> 1);
  } while( !RKBP->isProvedConfluent() );
}


void KBSupervisorARCer::writeResults( ostream& out ) 
{
  out < (RKBP->KnuthBendixMachine());
  delete RKBP;
}


void KBSupervisorARCer::readResults( istream& in )
{
  in > KBM;
}


//----------------------------- KBSupervisor ---------------------------------//


KBSupervisor::KBSupervisor(class GCM& gcm)
  : theGroup( gcm.getSMFPGroup() ),
    arcer( *this )
{
  adminStart();
}


void KBSupervisor::start( )
{
  arcer.setArguments(theGroup.getFPGroup());
}


void KBSupervisor::takeControl( )
{
  if ( freeARCs() > 0 ) 

    if( arcer.takeControl() ) {
      theGroup.gic.putHaveConfluentKBMachine( arcer.getKBMachine() );
      adminTerminate();
    }
    else
      usedOneARC();

}


void KBSupervisor::terminate( )
{
//  arcer.terminate();
}


//---------------------------- KBProblem ------------------------------------//


KBProblem::KBProblem(class SMFPGroup& G)
  : Supervisor( true ),
    theGroup( G ),
    kbSupervisor( *this, G.gcm().kbSupervisor )
{ }

void KBProblem::start( )
{
/*@db
  LogMessage msg( *this, theGroup );
  msg <<
	 "Warning: Magnus currently executes the Rutgers Knuth-Bendix"
	 " Package *synchronously*. This means that Magnus will appear to"
	 " freeze if rkbp takes a long time to respond. We are working on"
	 " an asynchronous version.";
  msg.send();
*/
}


void KBProblem::takeControl( )
{
  if( theGroup.gic.haveConfluentKBMachine() )
    adminTerminate();
}


void KBProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Seek rewriting system"),
		  Text("Seek a rewriting system for") + Name( theGroup ),
		  helpID("KBProblem", theGroup),
		  "Rewr sys"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), kbSupervisor.arcSlotID(), 100);

  pv.done();
}
#line 1  "src/NilpotentQuotients.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes NilpotentWPARCer, NilpotentWPInQuotients,
//            NilpotentWP, NGcomputeBasisARCer, NGcomputeBasis,
//            NilpotentQuotients, NGcomputeBasisProblem, NGPresentationARCer,
//            NGdecomposeWordARCer, NGdecomposeWord 

// Principal Author: Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//


#include "NilpotentQuotients.h"
#include "CommutatorIterator.h"
#include "SMFPGroup.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "conversions.h"

// ---------------------- NilpotentWPARCer --------------------------- //
void NilpotentWPARCer::setGroup(const NilpotentGroup& NG)
{
   theGroup = new NilpotentGroup(NG);
}
void NilpotentWPARCer::setArguments(const int& length,const NilpotentGroup& NG)
{
  theCommutatorsLength = length;
  isCommutatorsChecks = true;
  if (theGroup)
    delete theGroup;
  setGroup(NG);

  int genLen = NG.numberOfGenerators();
  generators = VectorOf<Word>(genLen);
  for( int i = 0; i < genLen; ++i )
    generators[i] = Word(Generator(i+1));
}
void NilpotentWPARCer::setArguments(const int& length,const NilpotentGroup& NG,
				    const VectorOf<Word>& gen)
{
  theCommutatorsLength = length;
  generators = gen;
  isCommutatorsChecks = true;
  if (theGroup)
    delete theGroup;
  setGroup(NG);
}

void NilpotentWPARCer::setArguments(const SetOf<Word>& setW, const NilpotentGroup& NG)
{
  int card = setW.cardinality();
  theWords = VectorOf<Word>(card);
  isCommutatorsChecks = false;

  SetIterator<Word> I(setW);
  for( int i = 0 ; !I.done(); I.next(), ++i )
    theWords[i] = Word(I.value());

 if (theGroup)
    delete theGroup;
  setGroup(NG);
}
void NilpotentWPARCer::setArguments(const VectorOf<Word>& vecW, const NilpotentGroup& NG)
{
  theWords = vecW;
  isCommutatorsChecks = false;

  if (theGroup)
    delete theGroup;
  setGroup(NG);
}
void NilpotentWPARCer::runComputation( )
{
#if SAFETY > 0
  if( theGroup == 0 )
    error( " NilpotentWPARCer::runComputation() : "
	   "The word problem has not been started yet");
#endif
  if (isCommutatorsChecks)
    commutatorsCheck();
  else
    wordsCheck(); 
}

void NilpotentWPARCer::commutatorsCheck( ) 
{
  result = true;
  
  CommutatorIterator I(generators, theCommutatorsLength);
  for( I.reset(); !I.done(); I.next() ){
    // @am if wordProblem == dontknow it will be error.
    // but i suppose it have to be only yes or no, so i leave it.
    retWord = I.value();
    commutator = I.components();
    if (!theGroup->wordProblem(I.value())){
      result = false;
      break;
    }
  }
}      
    
void NilpotentWPARCer::wordsCheck( ) 
{
  int wordNum = theWords.length();
  result = true;
  for (int i=0;i<wordNum;i++){
    retWord = theWords[i];
    if (!theGroup->wordProblem(theWords[i])){
      result = false;
      break;
    }
  }
}      

void NilpotentWPARCer::writeResults( ostream& out ) 
{
  out < result;
  out < retWord;
  out < commutator;
}


void NilpotentWPARCer::readResults( istream& in )
{
  in > result;
  in > retWord;
  in > commutator;
}


bool NilpotentWPARCer::getResult() const
{
#if SAFETY > 0
  if( theGroup == 0 )
    error( "bool NilpotentWPARCer::getResult() const : "
	   "The word problem has not been computed yet");
#endif
  return result;
}

Word NilpotentWPARCer::getWord() const
{
#if SAFETY > 0
  if( theGroup == 0 )
    error( "bool NilpotentWPARCer::getResult() const : "
	   "The word problem has not been computed yet");
#endif
  return retWord;
}
VectorOf<int> NilpotentWPARCer::getCommutator() const
{
#if SAFETY > 0
  if( theGroup == 0 )
    error( "bool NilpotentWPARCer::getResult() const : "
	   "The word problem has not been computed yet");
#endif
  if (!isCommutatorsChecks)
   error( "bool NilpotentWPARCer::getCommutator() const : "
	   "Commutators were not checked");
    
  return commutator;
}
//---------------------------------------------------------------------------//
//---------------------------- NilpotentWP ----------------------------------//
//---------------------------------------------------------------------------//


NilpotentWP::NilpotentWP( Supervisor& sup )
  : arcer( *this ),
    theGroup(0),
    isStarted (false),
    is_trivial( dontknow )
{ 
  //  adminStart();
}
void NilpotentWP::initialize(const int& length, const SMFPGroup* group)
{
  theGroup = group;
  theCommutatorsLength = length;
  isCommutatorsCheck = true;
  theClass = theGroup->gic.getNilpotentcyClass();
  adminStart();
}
void NilpotentWP::initialize(const int& length, const SMFPGroup* group,
			     const VectorOf<Word>& gen)
{
  theGroup = group;
  theCommutatorsLength = length;
  generators = gen;
  isCommutatorsCheck = true;
  theClass = theGroup->gic.getNilpotentcyClass();
  adminStart();
}
void NilpotentWP::initialize(const SetOf<Word>& w, const SMFPGroup* group)
{
  theWords = w;
  isCommutatorsCheck = false;
  theGroup = group;
  theClass = theGroup->gic.getNilpotentcyClass();
  adminStart();
}
void NilpotentWP::initialize(const VectorOf<Word>& vecW, const SMFPGroup* group)
{
  theWords = makeSetOf(vecW);
  isCommutatorsCheck = false;
  theGroup = group;
  theClass = theGroup->gic.getNilpotentcyClass();
  adminStart();
}
void NilpotentWP::start()
{
#if SAFETY > 0
  if( theGroup == 0 )
    error( "void NilpotentWP::start() : "
	   "The computation manager was not initialized yet");
#endif
}

void NilpotentWP::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if ( ! isStarted && theGroup->gic.haveNilpotentQuotInited
	 (theGroup->gic.getNilpotentcyClass() ) ) {
      if (isCommutatorsCheck) // If we want to check enumerated commutators
	if (generators.length()==0)// Commutate the generators of the group
	  arcer.setArguments( theCommutatorsLength, 
			      theGroup->gic.getNilpotentQuotInited
			      (theGroup->gic.getNilpotentcyClass()));
	else // Commutate the generators, given as a parameter
	  arcer.setArguments( theCommutatorsLength, 
			      theGroup->gic.getNilpotentQuotInited
			      (theGroup->gic.getNilpotentcyClass() ),
			      generators);
      else // Check the set of words
	arcer.setArguments( theWords, theGroup->gic.getNilpotentQuotInited
			    (theGroup->gic.getNilpotentcyClass() ));
      isStarted = true;
    }
    
    if ( isStarted ) 
      if (arcer.takeControl()){
	is_trivial = arcer.getResult();
	retWord = arcer.getWord();
	if (isCommutatorsCheck)
	  commutator = arcer.getCommutator();
	adminTerminate();
	return;
      }
    
    usedOneARC();
  }
}

Trichotomy NilpotentWP::isTrivial() const
{
  return is_trivial;
}

Word NilpotentWP::getWord() const
{
  return retWord;
}
VectorOf<int> NilpotentWP::getCommutator() const
{
  return commutator;
}

//---------------------------------------------------------------------------//
//---------------------------- NilpotentWPInQuotients -----------------------//
//---------------------------------------------------------------------------//


NilpotentWPInQuotients::NilpotentWPInQuotients(Supervisor& sup)
  : 
    currentClass(2),
    arcer( *this ),
    isStarted (false),
    is_trivial( dontknow ),
    theGroup(0)
{

}

void NilpotentWPInQuotients::initialize(const SetOf<Word>& w,const SMFPGroup* group,
					int startClass)
{
  theWords = w;
  theGroup = group;
  isCommutatorsCheck = false;
  currentClass = startClass;
  if (theGroup->gic.isNilpotent()==yes || theGroup->gic.isFreeNilpotent()==yes )
    upperBound = theGroup->gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  adminStart();
}
void NilpotentWPInQuotients::initialize(const VectorOf<Word>& vecW,const SMFPGroup* group,
					int startClass)
{
  theWords = makeSetOf(vecW);
  theGroup = group;
  isCommutatorsCheck = false;
  currentClass = startClass;
  if (theGroup->gic.isNilpotent()==yes || theGroup->gic.isFreeNilpotent()==yes)
    upperBound = theGroup->gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  adminStart();
}
void NilpotentWPInQuotients::initialize(const int& length, const SMFPGroup* group,
					const VectorOf<Word>& gen,
					int startClass)
{
  theGroup = group;
  theCommutatorsLength = length;
  generators = gen;
  currentClass = startClass;
  isCommutatorsCheck = true;
  if (theGroup->gic.isNilpotent()==yes || theGroup->gic.isFreeNilpotent()==yes)
    upperBound = theGroup->gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  adminStart();
}
void NilpotentWPInQuotients::initialize(const int& length, const SMFPGroup* group,int startClass)
{
  theGroup = group;
  theCommutatorsLength = length;
  currentClass = startClass;
  isCommutatorsCheck = true;
  if (theGroup->gic.isNilpotent()==yes || theGroup->gic.isFreeNilpotent()==yes)
    upperBound = theGroup->gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  adminStart();
}
void NilpotentWPInQuotients::start()
{
#if SAFETY > 0
  if( theGroup == 0 )
    error( "void NilpotentWPInQuotients::start() : "
	   "The computation manager was not initialized yet");
#endif
}

Trichotomy NilpotentWPInQuotients::isTrivial( int& nClass ) const
{
  if (is_trivial != dontknow )
    nClass = currentClass;
  return is_trivial;
}

Word NilpotentWPInQuotients::getWord() const
{
  return retWord;
}

VectorOf<int> NilpotentWPInQuotients::getCommutator() const
{
  return commutator;
}

void NilpotentWPInQuotients::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if (currentClass > upperBound ){
      //      currentClass--;
      adminTerminate();
      return;
    }   
    if ( ! isStarted && theGroup->gic.haveNilpotentQuotInited( currentClass ) ) {
      if (isCommutatorsCheck) // If we want to check enumerated commutators
	if (generators.length() == 0) // Commutate the generators of the group
	  arcer.setArguments( theCommutatorsLength, 
			      theGroup->gic.getNilpotentQuotInited( currentClass ));
	else // Commutate the generators, given as a parameter
	  arcer.setArguments( theCommutatorsLength, 
			      theGroup->gic.getNilpotentQuotInited( currentClass ),
			      generators);
      else // Check the givem set of words
	arcer.setArguments( theWords, 
			    theGroup->gic.getNilpotentQuotInited( currentClass ));
      isStarted = true;
    }
    
    if ( isStarted ) 
      if (arcer.takeControl()){
	if (!arcer.getResult()){
	  is_trivial = false;
	  retWord = arcer.getWord();
	  if (isCommutatorsCheck)
	    commutator = arcer.getCommutator();
	  adminTerminate();
	  return;
	}
	currentClass++;
	isStarted = false;
    }
    usedOneARC();
  }
}
// ---------------------- NGcomputeBasisARCer --------------------------- //


void NGcomputeBasisARCer::setArguments(const FPGroup& group,const int& nilpClass)
{
  if (init)
    delete init;
  // This ARCer can be called for several times, so we have to delete
  // init, if it exists.
  
  init = new NilpotentGroup(group.namesOfGenerators(),
			    nilpClass,
			    makeVectorOf(group.getRelators()));
}


void NGcomputeBasisARCer::runComputation( )
{
#if SAFETY > 0
  if( init == 0 )
    error( "void NGcomputeBasisARCer::runComputation( ): "
	   "The quotient has not been initialized");
#endif
  if (!init->isInitialized())
    init->initialize();
  if (putInFile){
    init->printBasis(structFile);
  }
}


void NGcomputeBasisARCer::writeResults( ostream& out ) 
{
  out < *init;
}


void NGcomputeBasisARCer::readResults( istream& in )
{
  in > *init;
}


const NilpotentGroup& NGcomputeBasisARCer::getGroup() const
{
#if SAFETY > 0
  if( init == 0 )
    error( "NilpotentGroup NGcomputeBasisARCer::getGroup() : "
	   "The quotient has not been computed yet");
#endif
  
  return *init;
}
//---------------------------------------------------------------------------//
//------------------------- NGcomputeBasis ----------------------------------//
//---------------------------------------------------------------------------//


NGcomputeBasis::NGcomputeBasis(GCM& gcm, bool internal)
  : ComputationManager( ! internal ),
    theGCM(gcm),
    arcsNumber (0),
    arcer (*this,true)
{
  if (theGCM.getSMFPGroup().gic.getBasisFileName().length()>0)
    resultIsFast();
  else{
    arcer.setArguments(theGCM.getSMFPGroup().getFPGroup(),
		       theGCM.getSMFPGroup().gic.getNilpotentcyClass());
  }
  adminStart();
}


NGcomputeBasis::~NGcomputeBasis( )
{
}


void NGcomputeBasis::start( )
{
}


void NGcomputeBasis::takeControl( )
{
  if (fastResult()){
    adminTerminate();
    return;
  }
  if ( freeARCs() > 0 ) {
    if (arcsNumber == 99){
      LogMessage msg;
      msg << " Many algorithms involving nilpotent groups are particularly"
      	" sensitive to the ordering of the generators. If you find that"
	" Magnus is working very hard, without coming up with any answers,"
	" try relabeling the generators. For example, instead of the presentation";
      msg << "G= 3 <a,b,c; a^2, [b,c]>,";
      msg << "use the presentation";
      msg << "G= 3 <a,b,c; [a,b], c^2 >.";

      msg << "Surprisingly, Magnus computes the Hirsch number of G, using the latter"
	" presentation, almost immediately, but struggles forever with the first"
	" presentation, before coming up with the Hirsch number.";
      msg.send();
    }    
    if (arcer.takeControl()) 
      //arcer.runComputation();
      {
	theGCM.getSMFPGroup().gic.
	  putHaveNilpotentGroupInited( arcer.getGroup(),
				       arcer.getFileName());
	theGCM.getSMFPGroup().gic.putHaveOrder( arcer.getGroup().order(),
						Chars("this follows from an inspection of a basis of ")+ 
						Text(Name(theGCM.getSMFPGroup())));
	adminTerminate();
	return;
      } else{
	arcsNumber++;
	usedOneARC();
      }
  }
}


void NGcomputeBasis::terminate( )
{
  
}



//---------------------------------------------------------------------------//
//------------------------ NilpotentQuotients -------------------------------//
//---------------------------------------------------------------------------//


NilpotentQuotients::NilpotentQuotients(class GCM& gcm, bool internal)
  : ComputationManager( ! internal ),
    theGCM( gcm ),
    theGroup( gcm.getSMFPGroup() ),
    arcer(*this),
    currentClass( 2 )
{
  if (theGroup.gic.isNilpotent()==yes)
    upperBound = theGroup.gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  // Checking upper class.
  
  if ( increaseCurrentClass() )
    arcer.setArguments(theGroup.getFPGroup(),
		       currentClass);
  // Looking for computed quotients.

  adminStart();
}


NilpotentQuotients::~NilpotentQuotients( )
{
}


void NilpotentQuotients::takeControl( )
{

  if ( freeARCs() > 0 ) {
    if (currentClass > upperBound){
      adminTerminate();
      return;
    }
    
    if (arcer.takeControl()) {
      theGroup.gic.putHaveNilpotentQuotInited( arcer.getGroup(), currentClass);
      
      LogMessage msg( *this,theGroup );
      msg << "Computed the nilpotent quotient of class " << currentClass;
      msg << " of " << Name( theGroup ) << ".";
      msg.send();     
      
      currentClass++;
      if (!increaseCurrentClass() ){
	adminTerminate();
	return;
      }
      else 
	arcer.setArguments(theGroup.getFPGroup(),
			   currentClass);
      
    } 
    usedOneARC();
  }
}


void NilpotentQuotients::start( )
{

}


void NilpotentQuotients::terminate( ) { }
bool NilpotentQuotients::increaseCurrentClass()
{
  while (currentClass <= upperBound && 
	 theGroup.gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;     
  return !(currentClass > upperBound);
}


//---------------------------------------------------------------------------//
//--------------------- NGcomputeBasisProblem ----------------------------//
//---------------------------------------------------------------------------//


NGcomputeBasisProblem::NGcomputeBasisProblem(class SMFPGroup& G)
  : Supervisor( true ),
    theGroup( G ),
    computeBasis( *this, G.gcm().computeBasis )
{ 
  if (theGroup.gic.getBasisFileName().length()>0 )
    resultIsFast();  
}


void NGcomputeBasisProblem::start( )
{

}


void NGcomputeBasisProblem::takeControl( )
{
  if (theGroup.gic.haveNilpotentGroupInited() && 
      theGroup.gic.getBasisFileName().length()>0 ){
    LogMessage msg( *this, theGroup);
    if (theGroup.gic.isFreeNilpotent() == yes){
      msg <<  Link(Text("Click here to view basic commutators of " )
		  + Name(theGroup) + Text(".") ,
		  "NilpotentBasis",theGroup.gic.getBasisFileName());
    }
    else{
      Chars basis;
      //      if (theGroup.gic.getNilpotentGroupInited().isMalcevBasis())
      //	basis = "Mal'cev";
      msg << Link(Text("Click here to view the basis of ")
		  + Name(theGroup) + Text(".") ,
		  "NilpotentBasis",theGroup.gic.getBasisFileName());
    }
    msg.send();
    adminTerminate();
  }
}


void NGcomputeBasisProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute a basis of the group "),
		  Text("Compute a basis of ")+
		  Name( theGroup ),
		  helpID("NGcomputeBasisProblem", theGroup),
		  "basis"
		  );


  pv.startItemGroup();
  
  pv.add(Text("For the use of this problem"), computeBasis.arcSlotID(), 100);
  
  pv.done();
}
// ----------------- NGPresentationARCer ------------------------ //

void NGPresentationARCer::runComputation( )
{
  *thePresentation = theGroup->makePresentation( );  
}


void NGPresentationARCer::
setArguments(const NilpotentGroup& presentation )
{
   theGroup = new NilpotentGroup(presentation);
   thePresentation = new PresentationForNG(presentation.collector(),
					   presentation.relators());
}

const PresentationForNG& NGPresentationARCer::getPresentation() const
{
 #if SAFETY > 0
  if( theGroup == 0 )
    error( "const PresentationForFNG& NGPresentationARCer::getPresentation() const:"
	   "You have not initialized the presentation");
#endif
  return *thePresentation;
}

void NGPresentationARCer::writeResults( ostream& out ) 
{
  out < *thePresentation;
}

void NGPresentationARCer::readResults( istream& in )
{
  in > *thePresentation;
}


// ------------------- NGdecomposeWordARCer --------------------- //


void NGdecomposeWordARCer::runComputation( )
{
  retWord = theNG->decompose(theWord);
}


void NGdecomposeWordARCer::setArguments( const NilpotentGroup& NG,
					 const Word& w )
{
  theNG = new NilpotentGroup(NG);
  theWord = w;
}


void NGdecomposeWordARCer::writeResults( ostream& out ) 
{
  out < retWord;
}

void NGdecomposeWordARCer::readResults( istream& in )
{
  in > retWord;
}
//---------------------------------------------------------------------------//
//------------------------- NGdecomposeWord ---------------------------------//
//---------------------------------------------------------------------------//


NGdecomposeWord::NGdecomposeWord(WCM& wcm)
  : ComputationManager( false ),
    theWCM(wcm),
    theWord(wcm.getWord()),
    started ( false ),
    arcer (*this)
{
  if (theWCM.getWord().wic.haveCollectedForm())
    resultIsFast();
  adminStart();
}


NGdecomposeWord::~NGdecomposeWord( )
{
}


void NGdecomposeWord::start( )
{
}


void NGdecomposeWord::takeControl( )
{
  if (fastResult()){
    adminTerminate();
    return;
  }
  if ( freeARCs() > 0 ) {
    if (theWord.getParent().gic.haveNilpotentGroupInited()){
      if (!started){
	arcer.setArguments(theWord.getParent().gic.getNilpotentGroupInited(),
			   theWord.getWord());
	started = true;
      }
      if (arcer.takeControl() ){
	theWord.wic.putHaveCollectedForm(arcer.decomposition());
	adminTerminate();
	return;
      }
    }
    usedOneARC();
  }
} 



void NGdecomposeWord::terminate( )
{
  
}

#line 1  "src/NormalClosure.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of classes NormalClosureARCer, NormalClosure
//
// Principal Author: Dmitry Bormotov, Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//
//

#include "NormalClosure.h"
#include "GCM.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "Timer.h"
#include "DecomposeInSubgroup.h"

// ------------------------ NormalClosureARCer ----------------------------- //

NormalClosureARCer::NormalClosureARCer( ComputationManager& boss ) 
  : ARCer2( boss ), theDecomposer( 0 ), tempDecomposer( 0 )
{ } 

NormalClosureARCer::~NormalClosureARCer( ) 
{ 
  delete theDecomposer; 
  delete tempDecomposer; 
}


void NormalClosureARCer::setArguments( const FPGroup& G, 
				       const VectorOf<Word>& sgens )
{
  theDecomposer = new DecomposeInSubgroupOfFPGroup( G, sgens );
}


void NormalClosureARCer::runComputation( )
{
  Timer t(delay()*1000);
  while( !t.expired() ) {
    theDecomposer->expandGraph();
  }
}


void NormalClosureARCer::writeResults( ostream& out ) 
{
  out < *theDecomposer;
}


bool NormalClosureARCer::readResults2( istream& in )
{
  if( tempDecomposer == 0 ) {
    tempDecomposer = 
      new DecomposeInSubgroupOfFPGroup( FPGroup(), VectorOf<Word>() );
  }

  Timer timer(1000);
  bool ret = tempDecomposer->readPiece(in,timer);
  
  if( ret ) {
    delete theDecomposer;
    theDecomposer = tempDecomposer;
    tempDecomposer = 0;
  }

  return ret;
}


// ---------------------------- NormalClosure ------------------------------ //


NormalClosure::NormalClosure(GCM& gcm)
  : arcer(*this),
    theCM( gcm ),
    theGroup( gcm.getSMFPGroup() ),
    theObject( gcm.getSMFPGroup() )
{
  adminStart();
  arcer.setArguments( theGroup.getFPGroup(), VectorOf<Word>() );
}


NormalClosure::NormalClosure(SCM& scm)
  : arcer(*this),
    theCM( scm ),
    theGroup( scm.getSubgroup().getParent() ),
    theObject( scm.getSubgroup() )
{
  adminStart();
  arcer.setArguments( theGroup.getFPGroup(), 
		      scm.getSubgroup().getSubgroup().generators() 
		      );
}

DecomposeInSubgroupOfFPGroup& NormalClosure::getDecomposer( )
{
  return arcer.getDecomposer();
}

SubgroupGraph NormalClosure::getSubgroupGraph( ) const 
{ 
  return arcer.getDecomposer().graph(); 
}

Trichotomy NormalClosure::isTrivial(const Word& w) const
{
  DecomposeInSubgroupOfFPGroup& D = arcer.getDecomposer();

  if ( D.graph().isComplete() )
    return D.contains(w);
  else
    return ( D.contains(w) ? yes : dontknow );
}


void NormalClosure::takeControl( )
{
  if ( freeARCs() > 0 ) {
    
    if( arcer.takeControl() ) {
      
      DecomposeInSubgroupOfFPGroup& D = arcer.getDecomposer();

      putHaveWordDecomposer( D );

      LogMessage msg( theObject ); //.oid(), 2 );
      if( theObject.typeID() == SMFPGroup::type() ) {
	msg << "The trivial word acceptor has ";
      }
      else {
	msg << "The word acceptor has ";
      }
      msg << D.graph().vertexCount() << " vertices.";
      msg.send();
      
      if ( D.graph().isComplete() ) {
	putHaveCompleteCayleyGraph( D.graph() );
	adminTerminate();
      }
      
    }
    
    usedOneARC();
  }
}

void NormalClosure::putHaveWordDecomposer( 
  const DecomposeInSubgroupOfFPGroup& D )
{
  if( theObject.typeID() == SMFPGroup::type() ) {
    ((SMFPGroup &)theObject).gic.putHaveWordDecomposer( D );
  }
  else {
    ((SMSubgroup &)theObject).sic.putHaveWordDecomposer( D );
  }
}


void NormalClosure::putHaveCompleteCayleyGraph( 
  const SubgroupGraph& S )
{
  if( theObject.typeID() == SMFPGroup::type() ) {
    ((SMFPGroup &)theObject).gic.putHaveCompleteCayleyGraph( S );
  }
  else {
    ((SMSubgroup &)theObject).sic.putHaveCompleteCayleyGraph( S );
  }
}
#line 1  "src/WordProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes ORWordProblemARCer, ORWordProblemCM,
//           WordProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "WordProblem.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "OneRelatorGroup.h"
#include "OneRelatorGroupWithTorsion.h"
#include "GAWP.h"
#include "GeneticProblems.h"
#include "FNWP.h"


// ------------------------ ORWordProblemARCer ----------------------------- //


ORWordProblemARCer::ORWordProblemARCer( ComputationManager& boss )
  : ARCer( boss ), answer(dontknow)
{ } 


void ORWordProblemARCer::setArguments( const Word& relator,
				       const Word& testWord )
{
  theRelator = relator;
  theTestWord = testWord;
}

void ORWordProblemARCer::runComputation( ) 
{
  Generator g1 = theRelator.maxOccurringGenerator();
  Generator g2 = theTestWord.maxOccurringGenerator();
  int numOfGens = max( abs(ord(g1)), abs(ord(g2)) );

  const OneRelatorGroup G( numOfGens, theRelator );
  const bool keepDetails = true;
  answer = G.wordProblem( theTestWord, keepDetails, wDeco );
}

void  ORWordProblemARCer::writeResults( ostream& out )
{
  out < answer;
  if( answer == yes ) 
    out < wDeco;
}


void ORWordProblemARCer::readResults( istream& in )
{
  in > answer;
  if( answer == yes ) {
    retValue = true;
    in > wDeco;
  }
  else if( answer == no )
    retValue = false;
  else 
    error( "Internal error in OneRelatorGroup::wordProblem() " );
}


void ORWordProblemARCer::printWord(const Word& w, ostream& ostr)
{
  int len = w.length();
  ostr << len << endl;
  for( int i = 0; i < len; ++i ) {
	 int g = ord(w[i]);
	 ostr << char( abs(g) + 'a' - 1 );
	 if ( g < 0 ) ostr << -1;
	 else ostr << 1;
  }
  ostr << endl;
}


// -------------------------- ORWordProblemCM ------------------------------ //


ORWordProblemCM::ORWordProblemCM( WordProblem& PO )
: theRelator( PO.getTestWord().getParent().gic.getOneRelator() ),
  testWord( PO.getTestWord() ),
  itIsTrivial( dontknow ),
  bStarted( false ),
  arcer( *this )
{
  adminStart();
}

void ORWordProblemCM::terminate( )
{
  arcer.terminate(); 
}

void ORWordProblemCM::start( )
{
}

void ORWordProblemCM::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if( !bStarted ) {
      arcer.setArguments(theRelator, testWord.getWord());
      bStarted = true;
    }
    if( arcer.takeControl() ) {
      itIsTrivial = arcer.getRetValue();
      wDeco = arcer.wordDecomposition();
      adminTerminate();
      return;
    }
    else
      usedOneARC();
  }
}


// ----------------------------- WordProblem ------------------------------- //


WordProblem::WordProblem(SMWord& w)
  : Supervisor( ! w.getParent().gic.haveFastWordProblem() ),
    theWord( w ),
    theChecker( w.getWord(), w.getParent() ),
    theAnswer( dontknow ),
    triedAbelianization( false ),
    normalClosure( *this, w.getParent().gcm().normalClosure ),
    abelianInvariants( *this, w.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, w.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, w.getParent().gcm().agSupervisor ),
    computeBasis( *this, w.getParent().gcm().computeBasis ),
    nilpotentQuotients( *this, w.getParent().gcm().nilpotentQuotients ),
    nilpotentWPInQuotients(*this),
    nilpotentWP( *this ),	 
    orwp( *this ),
    genetic( *this ),
    fnwp( *this ),
    geneticForORG( *this ),
    linkGenORHasBeenSent( false )
{
  if (theWord.wic.isTrivial()!=dontknow)
    resultIsFast();
  else{
    nilpotentWP->initialize(theWord.getWord(),&theWord.getParent());
    nilpotentWPInQuotients->initialize(theWord.getWord(),&theWord.getParent());
    genetic->init(w.getParent().getFPGroup(), w.getWord(), GeneticWPCM::WORD);
    fnwp->init( w.getParent().getFreePreimage(), 
		w.getParent().gic.getNilpotentcyClass( ), w.getWord() );
    if ( ! displayInFE() ) adminStart();
  }

  theChecker.enablePutDetailsToFile();
}

void WordProblem::takeControl( )
{
  if ( ! displayInFE() || freeARCs() > 0 ) {
    Chars explanation;
    if (!fastResult()){
      if (nilpotentWP->state() == ComputationManager::TERMINATED){
	explanation = " this follows from an inspection of a basis." ;
	theAnswer = nilpotentWP->isTrivial();
	theWord.wic.putIsTrivial(theAnswer,explanation);
      }
      if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
	if (nilpotentWPInQuotients->isTrivial()==no){
	  int retClass;
	  theAnswer = nilpotentWPInQuotients->isTrivial(retClass);
	  ostrstream msgTmp;
	  msgTmp << " because it's not trivial in lower central quotient of class " 
		 << retClass << ends;
	  explanation = msgTmp.str();
	  theWord.wic.putIsTrivial(theAnswer,explanation);
	}
    }
    if ( (theAnswer = theWord.wic.isTrivial(explanation) )!=dontknow){
      LogMessage msg( *this, theWord );
      msg << Name( theWord ) << " is ";
      if ( theAnswer == no ) msg << "not ";
      msg << "trivial in " << Name( Parent( theWord ) );
      if( explanation.length() > 0 )
	msg << " : " << explanation;
      msg.send();
      adminTerminate();     
      return;
    }
    
    if ( ( theAnswer = theChecker.isTrivial() ) != dontknow ) {

      LogMessage msg( *this, theWord );
      msg << Name( theWord ) << " is ";
      if ( theAnswer == no ) msg << "not ";
      msg << "trivial in " << Name( Parent( theWord ) );

      Chars explanation = theChecker.getExplanation();

      if ( theWord.getParent().gic.isAbelian() == yes){
	ostrstream msgTmp;
	msgTmp << "because it's canonical decomposition is ";

	const AbelianGroup& A = 
	  theWord.getParent().gic.getCyclicDecomposition();
	
	A.printWordInNewGens(msgTmp,A.oldToNewGens
			     (A.oldInAbelianForm(theWord.getWord() )));
	msgTmp << ends;
	explanation = msgTmp.str();
      }

      if( theWord.getParent().gic.haveMSC() 
	  || theWord.getParent().gic.isOneRelatorWithTorsion() ) {
	explanation = explanation + ". " + 
	  Link( Chars("Click here to see word transformations of ")
		+ Text(Name(theWord)),
		"DehnTransformations", 
		theChecker.getDehnTransformationFileName()
		);
      }

      if( explanation.length() > 0 )
	msg << " : " << explanation;

      msg << ".";
      msg.send();
      theWord.wic.putIsTrivial(theAnswer,explanation);
      
      if( theWord.getParent().gic.isOneRelatorWithTorsion() == true ) {
	OneRelatorGroupWithTorsion G( theWord.getParent().getFreePreimage()
				      .namesOfGenerators(),
				      theWord.getParent().gic.getOneRelator()
				    );
	ProductOfRelatorConjugates wDeco;
	const bool answer = G.wordProblem( theWord.getWord(), wDeco );

	if( answer ) {
	  FPGroup G1 = theWord.getParent().getFPGroup();
	  DetailedReport wordDecomposition( G1 );
	  wordDecomposition.printTrivialWordDetails( theWord.getWord(), wDeco,
						     true );
	  LogMessage msg( *this, theWord );
	  msg << Link( Chars("Click here to see a decomposition of ")
		       + Text(Name(theWord)) 
		       + Text(" as a product of conjugates of the relator."),
		       "ORWordProblem", 
		       wordDecomposition.getFileName()
		       );
	  msg.send();
	}
      }

      if( theChecker.haveDetails() ) {
	LogMessage msg( *this, theWord );
	msg << Link( Chars("Click here to see a decomposition of ")
		     + Text(Name(theWord)) + 
		     Text(" as a product of conjugates of the relators."),
		     "WordProblemDetails", 
		     theChecker.getDecompositionFileName()
		     );
	msg.send();
      }
      
      adminTerminate();
    }
    else if ( theWord.getParent().gic.isOneRelator() ) {
      if( ( theAnswer = orwp->isTrivial() ) != dontknow ) {
	
	LogMessage msg( *this, theWord );
	msg << Name( theWord ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "trivial in " << Name( Parent( theWord ) );
	
	Chars explanation = "according to the Magnus breakdown";
	msg << " : " << explanation << ".";
	msg.send();
	theWord.wic.putIsTrivial(theAnswer,explanation);
	
	if( theAnswer == yes ) {
	  DetailedReport details( theWord.getParent().getFPGroup() );
	  details.printTrivialWordDetails( theWord.getWord(),
					   orwp->wordDecomposition(), 
					   true );
	  LogMessage msg( *this, theWord );
	  msg << Link( Chars("Click here to see a decomposition of ")
		       + Text(Name(theWord)) 
		       + Text(" as a product of conjugates of the relator."),
		       "ORWordProblem", 
		       details.getFileName()
		       );
	  msg.send();
	  theWord.wic.putIsTrivial( theAnswer, explanation );
	}
	adminTerminate();
      }
      if( !linkGenORHasBeenSent ) {
	linkGenORHasBeenSent = true;
	
	LogMessage msg( *this , theWord );
	msg << Link("Click here to see details of the genetic algorithm 2.",
		    "GAWPforORG", geneticForORG->getFileName(), true);
	msg.send();
      }

      if( ( theAnswer = geneticForORG->answer() ) != dontknow ) {
	
	LogMessage msg( *this, theWord );
	msg << Name( theWord ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "trivial in " << Name( Parent( theWord ) );
	
	Chars explanation = "according to the genetic algorithm 2";
	msg << " : " << explanation << ".";
	msg.send( );
	
	theWord.wic.putIsTrivial( theAnswer , explanation );
	adminTerminate();
      }      
    } 
    else if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
	
      LogMessage msg( *this, theWord );
      msg << Name( theWord ) << " is ";
      //if ( theAnswer == no ) msg << "not ";
      msg << "trivial in " << Name( Parent( theWord ) );
      
      Chars explanation = "according to the genetic algorithm 1";
      msg << " : " << explanation << ".";
      msg.send();
      theWord.wic.putIsTrivial(theAnswer,explanation);
      adminTerminate();
    }
    else if ( ( theAnswer = fnwp->areTrivial() ) != dontknow ) {
	
      LogMessage msg( *this, theWord );
      msg << Name( theWord ) << " is ";
      if ( theAnswer == no ) msg << "not ";
      msg << "trivial in " << Name( Parent( theWord ) );
      
      Chars explanation = "solved using power series";
      msg << " : " << explanation << ".";
      msg.send();
      theWord.wic.putIsTrivial(theAnswer,explanation);
      adminTerminate();
    }
    else {
      usedOneARC();
    }
  }
}

void WordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Word problem"),
		  Text("Is") + Name( theWord ) + Text("trivial in")
		  + Name( Parent( theWord ) ) + Text("?"),
		  helpID( "WordProblem", theWord.getParent() ), 
		  Text(Name(theWord)) + Text("= 1 ?")
		  );
  

  pv.startItemGroup();
  
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	 abelianInvariants.arcSlotID(),
	 15
	 );


  pv.startItemGroup( CheckinType( Parent( theWord ) ) ==
		     SMFPGroup::ONE_RELATOR);

  pv.add(Text("Compute the Magnus breakdown"), orwp.arcSlotID(), 50);

  pv.startItemGroup( CheckinType( Parent( theWord ) ) != SMFPGroup::ABELIAN );

  pv.startItemGroup( CheckinType( Parent( theWord ) ) != SMFPGroup::ABELIAN &&
		     CheckinType( Parent( theWord ) ) != SMFPGroup::NILPOTENT &&
		     CheckinType( Parent( theWord ) ) != SMFPGroup::FREE_NILPOTENT
		     );

  pv.add(Text("Enumerate normal closure of relators"),
	 normalClosure.arcSlotID(), 50);
  
  pv.add(Text("Seek a rewriting system for") + Name( Parent( theWord ) ),
	 kbSupervisor.arcSlotID(),
	 50
	 );
  
  pv.add(Text("Seek a ShortLex automatic structure for")
	 + Name( Parent( theWord ) ),
	 agSupervisor.arcSlotID(),
	 50
	 );
  
  pv.startItemGroup( CheckinType( Parent( theWord ) ) != SMFPGroup::ABELIAN &&
		     CheckinType( Parent( theWord ) ) != SMFPGroup::NILPOTENT &&
		     CheckinType( Parent( theWord ) ) != SMFPGroup::FREE_NILPOTENT
		     );
  pv.add(Text("Run genetic algorithm 1"), genetic.arcSlotID(), 50);

  pv.startItemGroup( CheckinType( Parent( theWord ) ) ==
		     SMFPGroup::ONE_RELATOR || 
		     CheckinType( Parent( theWord ) ) ==
		     SMFPGroup::ONE_RELATOR_WITH_TORSION
		     );

  pv.add(Text("Run genetic algorithm 2") , geneticForORG.arcSlotID(), 50);
  
  pv.startItemGroup( CheckinType( Parent( theWord ) ) == SMFPGroup::NILPOTENT ||
		     CheckinType(Parent( theWord ) ) == SMFPGroup::FREE_NILPOTENT
		     );

  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add(Text("Compute a basis of") + Name( Parent( theWord ) ),
	   computeBasis.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using basis of")
	 + Name( Parent( theWord ) ),
	 nilpotentWP.arcSlotID(),
	 50
	 );
  
  pv.startItemGroup
    ( CheckinType( Parent( theWord ) ) == SMFPGroup::NILPOTENT ||
      CheckinType( Parent( theWord ) ) == SMFPGroup::FREE_NILPOTENT ||
      CheckinType( Parent( theWord ) ) == SMFPGroup::FP
      );

  int upperBound, currentClass = 2;
  if (theWord.getParent().gic.isNilpotent()==yes || 
      theWord.getParent().gic.isFreeNilpotent()==yes)
    upperBound = theWord.getParent().gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theWord.getParent().gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;

  if (!(currentClass > upperBound) )
    pv.add(Text("Compute lower central quotients for") 
	   + Name( Parent( theWord ) ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using lower central quotients of")
	 + Name( Parent( theWord ) ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );
  

  pv.startItemGroup
    ( CheckinType( Parent( theWord ) ) == SMFPGroup::FREE_NILPOTENT
      );

  pv.add(Text("Solve using power series"),
	 fnwp.arcSlotID(),
	 50
	 );

  pv.done();
}
#line 1  "src/fastProblems.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of CM classes for fast problems
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//
// * 10/11/96 EP added:
//
//               void SGOfFreeWhiteheadReduction::takeControl( )
//               void SGOfFreeIsAFreeFactor::takeControl( )
//
// * 96/10/10 @dp added:
//		FreeGetNextN_thElement::takeControl(),
//		TerminalSegmentOfWord::takeControl(),
//		SegmentOfWord::takeControl()
//
// * 96/10/16 @dp added:
//		SGOfFreeIsMalnormal::takeControl( ),
//		APOfFree_DoEltsCommute::takeControl( ),
//		APOfFreeIsHyperbolic::takeControl( ),
//		APOfFreeNumberOfSubstitutions::takeControl( ).
//
//
// *00/04/04 @db: FastComputation is under reconstruction - ATS is
// about to be introduced
//


#include "fastProblems.h"
#include "OutMessages.h"
#include "conversions.h"

// Includes required by the computations:

#include "SMFPGroup.h"
#include "SMWord.h"
#include "SMSubgroup.h"
#include "SMMap.h"
#include "SMHomomorphism.h"
#include "SMEquation.h"
#include "SMList.h"

#include "ORProblems.h"
#include "SGofFreeGroup.h"

#include "MagnusBreakdown.h"
#include "File.h"
#include "Whitehead.h"
#include "OneRelatorGroupWithTorsion.h"
#include "Products.h"
#include "CommutatorIterator.h"
#include "APwithOneRelator.h"
#include "RipsConstruction.h"

//#define ATS


//---------------------------------------------------------------------------//
//------------------------- FastComputation ---------------------------------//
//---------------------------------------------------------------------------//


void FastComputation::viewStructure(ostream& ostr) const
{
  //#ifdef ATS
  ProblemView pv( ostr, oid(), Text("Temporary"),
		  Text("Temporary"),
		  "none",
		  "Temporary"
		  );

  pv.startItemGroup();

  //pv.add("For the use of genetic algorithm", GA.arcSlotID(), 100);

  pv.done();
  //#endif
}

#ifndef ATS

FastComputation::FastComputation( ) : ComputationManager( )
{
  resultIsFast();
}

#else

FastComputation::FastComputation( ) : ComputationManager( true )
{
  //attachClient( *this );
  //resultIsFast();
}

#endif


//---------------------------------------------------------------------------//
//--------------------- Free Group Computations -----------------------------//
//---------------------------------------------------------------------------//


void CommutatorInFree::takeControl( )
{
  Word u, v;
  FreeGroup F = theWord.getParent().getFreePreimage();
  LogMessage msg( theWord );
  msg << Name( theWord );

  if ( F.isCommutator( theWord.getWord(), u, v ) ) {
	 msg << " is the commutator of ";
	 F.printWord( msg, u );
	 msg << " and ";
	 F.printWord( msg, v );
  } else {
	 msg << " is not a commutator";
  }
  msg << " in " << Name( Parent( theWord ) ) << ".";

  msg.send();
}


void FreeInCommutatorSG::takeControl( )
{
  FreeGroup F = theWord.getParent().getFreePreimage();
  LogMessage msg( theWord );

  msg << Name( theWord ) << " is";
  if ( !F.inCommutatorSG(theWord.getWord()) )
    msg << " not";
  msg << " in the commutator subgroup of " << Name( Parent( theWord ) ) << ".";

  msg.send();
}


void ProductOfCommutators::takeControl( )
{
  FreeGroup F = theWord.getParent().getFreePreimage();
  
  Chars result = F.productOfCommutators(theWord.getWord(),theFile);
  LogMessage msg( theWord );
  
  if ( result == Chars("not in derived") )
    msg << Name( theWord )  << " isn't in derived subgroup.";
  else
    if ( result == Chars("trivial") )
      msg << Name( theWord ) << " is trivial.";
    else
     {
       msg << Name( theWord ) << " can be rewritten as:  " << result << " . ";
       
       LogMessage msg1( theWord );
       msg1 << Link("Click here to see the steps of the rewriting process" , "FastCommutators" , theFile.getFileName() );
       msg1.send();
     }
  
  msg.send();
}

void ProductOfSquares::takeControl( )
{
  FreeGroup F = theWord.getParent().getFreePreimage();
  
  Chars result = F.productOfSquares(theWord.getWord(),theFile);
  LogMessage msg( theWord );
  
  if ( result == Chars("not in square") )
    msg << Name( theWord )  << " isn't in subgroup generated by all squares.";
  else
    if ( result == Chars("trivial") )
      msg << Name( theWord ) << " is trivial.";
    else
      {
	msg << Name( theWord ) << " can be rewritten as:  " << result << " . ";
  
	LogMessage msg1( theWord );
	msg1 << Link("Click here to see the steps of the rewriting process" , "FastSquares" , theFile.getFileName() );
	msg1.send();
      }

  msg.send();
}

void FreeIsElementAProperPower::takeControl( )
{
  Word w = theWord.getWord().cyclicallyReduce();
  LogMessage msg( theWord );
  msg << Name( theWord );

  if ( w.isProperPower() ) {
    int r = maximalRoot( w );
    msg << " is a proper power. " << Name( theWord ) << " is equal ";
    theWord.getParent().getFreePreimage().printWord(msg,
	    w.initialSegment( w.length()/r ) );
    msg << " in power " << r;
  }
  else 
    msg << " is not a proper power";
  
  msg << ".";

  msg.send();
}


void FreeMaximalRootOfElement::takeControl( )
{
  Word w = theWord.getWord().cyclicallyReduce();
  SMObject* smo =
    new SMWord( theWord.getParent(),
		w.initialSegment( w.length()/maximalRoot( w ) ),
		Text("The maximal root of") + Name( theWord )
		);
  
  ListOf<OID> dependencies( theWord.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
  
  //@rn Factor out somewhere
}

void FreeCentolizerOfElement::takeControl( )
{
  Word w = theWord.getWord().cyclicallyReduce();
  VectorOf<Word> generator(1);
  generator[0] = w.initialSegment( w.length()/maximalRoot( w ) );
  SGofFreeGroup S2(theWord.getParent().getFreePreimage(), generator);
  SMObject* smo = new SMSubgroup(theWord.getParent(),S2,
		    Text("The centralizer of") + Name( theWord ) );
  
  ListOf<OID> dependencies( theWord.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
  
}

void FreeGetN_thElement::takeControl( )
{
  char s[10];
  sprintf(s,"%u",theNumber);
  
  SMObject* smo =
    new SMWord( theGroup,
		theGroup.getFreePreimage().getN_thElement(theNumber),
		Text("The") + Text(s) + Text("- th element of")
		+ Name( theGroup )
		);
  
  ListOf<OID> dependencies( theGroup );
  CheckinMessage( *smo, "", dependencies ).send();
  
  //@rn Factor out somewhere
}
void WordProblemInFree::takeControl( )
{
  Word u, v;
  FreeGroup F = theWord.getParent().getFreePreimage();
  LogMessage msg( theWord );
  msg << Name( theWord );

  if ( F.wordProblem( theWord.getWord() ) ) {
	 msg << " is trivial";
  } else {
	 msg << " is not trivial";
  }
  msg << " in " << Name( Parent( theWord ) ) << ".";

  msg.send();
}


void WordsAreEqual::takeControl( )
{
  LogMessage msg( word1, word2 );

  msg << Name( word1 ) << " is ";

  if ( word1.getWord().freelyReduce() != word2.getWord().freelyReduce() )
	 msg << "not ";

  msg << "equal to " << Name( word2 ) << ".";

  msg.send();
}


void EndoOnFreeIsMono::takeControl( )
{
  if( map.mic.isMono() == dontknow ) {
    const Map& M = map.getMap();
    SGofFreeGroup S( map.getDomain().getFreePreimage(), M.generatingImages() );
    bool isMono = ( S.rank() == M.generatingImages().length() );
    map.mic.putIsMono( isMono );
  }

  LogMessage msg( map );
  msg << Name( map ) << " is ";
  if ( map.mic.isMono() == no ) 
    msg << "not ";
  msg << "an monomorphism of " << Name( Domain( map ) ) << ".";
  msg.send();
}

void EndoOnFreeIsEpi::takeControl( )
{
  if( map.mic.isEpi() == dontknow ) {
    const FreeGroup& F = map.getDomain().getFreePreimage();
    SGofFreeGroup S( F, map.getMap().generatingImages() );
    bool isEpi = true;
    for( int i = 1; i <= F.numberOfGenerators(); ++i ) {
      if( !S.contains( Generator(i) ) ) {
	isEpi = false;
	break;
      }
    }
    map.mic.putIsEpi( isEpi );
  }

  LogMessage msg( map );
  msg << Name( map ) << " is ";
  if ( map.mic.isEpi() == no ) 
    msg << "not ";
  msg << "an epimorphism of " << Name( Domain( map ) ) << ".";
  msg.send();
}

struct IsAuto;

void EndoOnFreeIsAut::takeControl( )
{
  LogMessage msg( map );
  msg << Name( map ) << " is ";

  if ( ! map.getDomain().getFreePreimage()
         .isAutomorphism( map.getMap().generatingImages() )
       )
    msg << "not ";
  else {
    IsAuto IA(map);
    FEDataUpdate( IA, True() ).send();
  }

  msg << "an automorphism of " << Name( Domain( map ) ) << ".";
  msg.send();
}


void EndoOnFreeIsInner::takeControl( )
{
  LogMessage msg( map );
  msg << Name( map ) << " is ";

  if ( ! map.getDomain().getFreePreimage()
         .isInnerAutomorphism( map.getMap().generatingImages() )
       )
    msg << "not ";
  // We do not record innerness in the FE...

  msg << "an inner automorphism of " << Name( Domain( map ) ) << ".";
  msg.send();
}

void EndoOnFreeIsIAAut::takeControl( )
{
  LogMessage msg( map );
  msg << Name( map ) << " is ";

  bool answer = map.getDomain().getFreePreimage()
    .isIAAutomorphism( map.getMap().generatingImages() );

  if( !answer )
    msg << "not ";
  // We do not record IA-ness in the FE...

  msg << "an IA-automorphism of " << Name( Domain( map ) ) << ".";
  msg.send();
}


void InverseAuto::takeControl( )
{
  SMObject* smo =
    new SMHomomorphism( map.getDomain(),
		      Map( map.getDomain().getFPGroup(),
			   map.getDomain().getFPGroup(),
			   map.getDomain().getFreePreimage()
			   .inverseAutomorphism( map.getMap().generatingImages() )
			   ),
		      Text("The inverse of") + Name( map )
		      );
  
  ListOf<OID> dependencies( map.getDomain() );
  CheckinMessage( *smo, "", dependencies ).send();

  IsAuto IA( *smo );
  FEDataUpdate( IA, True() ).send(); 
}

void AutoWhiteheadDecomposition::takeControl( )
{
  const FreeGroup& F = map.getDomain().getFreePreimage();
  ListOf<ElementaryWhiteheadAuto> decoList = 
    whiteheadDecomposition( map.getMap().generatingImages() ).getAutoList();

  
  File file;
  file.setColor( titleColor );
  file << "Decomposition of the automorphism of free group" << endl
       << "into a sequence of elementary Whitehead automorphisms :" << endl << endl;
  file.setColor( mainColor );
  ListIterator< ListOf<ElementaryWhiteheadAuto> > I(decoList);
  const VectorOf<Chars> genNames = F.namesOfGenerators();
  int i = 0;
  for( ; !I.done(); I.next() ) {
    file << ' ' << ++i << ". ";
    I.value().printOn( file, genNames );
    file << endl;
  }
  file << end;

  LogMessage msg( map );
  msg << Link( Chars("Click here to see the Whitehead decomposition "
		     "of the automorpisms ") + Text( Name(map) ),
	       "WhiteheadDecomposition",
	       file.getFileName() 
	       );
  msg << ".";
  msg.send();
}


void WordInSGOfFree::takeControl( )
{
  LogMessage msg( word );
  msg << Name( word ) << " does ";

  Word w = word.getWord();
  SGofFreeGroup H = subgroup.getSubgroup();
  FreeGroup F = H.parentGroup();
  bool represent = H.contains( w );
  if ( ! represent  )
	 msg << "not ";

  msg << "represent an element of " << Name( subgroup ) << ".";
  msg.send();

  if( represent ) {

    Word wInSubgroupGens = expressWordInSubgroupGenerators( H, w );

    File logFile;
    logFile.setColor( titleColor );
    logFile << "The subroup " << Name( subgroup ) 
	    << " of free group is generated by " << endl << endl;
    
    logFile.setColor( mainColor );
    VectorOf<Word> hGens = H.generators();
    for( int i = 0; i < hGens.length(); ++i ) {
      logFile << " h" << i+1 << " = ";
      F.printWord( logFile, hGens[i] );
      logFile << endl;
    }
    logFile << endl;
    
    logFile.setColor( titleColor );
    logFile << "The given word is expressed in the subgroup generators as follows:" 
	    << endl << endl;

    VectorOf<Chars> hNames( hGens.length() );
    for( int i = 0; i < hNames.length(); ++i ) {
      char tmpName[20];
      sprintf( tmpName, "h%d", i+1);
      hNames[i] = tmpName;
    }
    FreeGroup FH( hNames );
    logFile.setColor( mainColor );
    logFile << " w = ";
    FH.printWord( logFile, wInSubgroupGens );

    
    LogMessage msg( word );
    msg << Link( 
		Chars("Click here to see the word ") + Text(Name(word))
		+ Chars(" written in subgroup generators"),
		"WordInSubgroupGenerators",
		logFile.getFileName()
		)
	<< ".";
    msg.send();
  }
}


void PowerOfWordInSGOfFree::takeControl( )
{
  LogMessage msg( word );
  msg << Name( word ) << " to ";

  long answer = subgroup.getSubgroup().powerInSubgroup( word.getWord() );
  if ( answer != 0 )
	 msg << "the power " << answer;
  else
	 msg << "no power other than 0";

  msg << " represents an element of " << Name( subgroup ) << ".";
  msg.send();
}

void ConjugacyProblemInFree::takeControl( )
{
  LogMessage msg( word1,word2 );

  Word conjugator;
  Trichotomy answer = word1.getParent().getFreePreimage().
    conjugacyProblem(word1.getWord(),word2.getWord(),conjugator);

  if ( answer == yes )
    {
      msg << Name( word1 ) << " and  " << Name( word2 ) << " are conjugate: ";
      msg << "X " << Name( word1 ) << " x = " <<  Name( word2 ) << ", where x = ";
      word1.getParent().getFPGroup().printWord(msg,conjugator);
    }
  else
    msg << Name( word1 ) << " and  " << Name( word2 ) << " are not conjugate ";
  
  msg << ".";
  msg.send();
}

void ConjugateOfWordInSGOfFree::takeControl( )
{
  LogMessage msg( word );

  Word conjugator;

  if ( subgroup.getSubgroup().conjugateInSubgroup( word.getWord(), conjugator ) )
	 {
		msg << Name( word ) << " conjugated by ";
		word.getParent().getFPGroup().printWord( msg, conjugator );
	 }
  else
	 msg << "No conjugate of " << Name( word );

  msg << " represents an element of " << Name( subgroup ) << ".";

  msg.send();
}


void WordInNielsenBasisSGOfFree::takeControl( )
{
  LogMessage msg( subgroup, word );
  SGofFreeGroup S( subgroup.getSubgroup() );
  if( !S.contains( word.getWord() ) ) {
    msg << Name( subgroup ) << " does not contain " << Name(word);
  }
  else {
    FreeGroup F( S.nielsenBasis().length() );
    msg << "The canonical decomposition of " << Name( word ) 
	<< " in the Nielsen basis for " << Name( subgroup ) 
	<< " : " << Name( word ) << " = ";
    F.printWord( msg, S.inNielsenWords( word.getWord() ) );
  }
  msg << ".";
  msg.send();
}

void SchreierRepOfWordInSGOfFree::takeControl( )
{
  SMObject* smo =
	 new SMWord( word.getParent(),
		     subgroup.getSubgroup()
		     .rightSchreierRepresentative( word.getWord() ),
		     Text("The right Schreier representative of") + Name( word )
		     + Text("mod") + Name( subgroup )
		     );

  ListOf<OID> dependencies( word.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void SGOfFreeContainment::takeControl( )
{
  LogMessage msg( subgroup1, subgroup2 );
  msg << Name( subgroup1 ) << " does ";

  if ( ! subgroup1.getSubgroup().contains( subgroup2.getSubgroup() ) )
	 msg << "not ";

  msg << "contain " << Name( subgroup2 ) << ".";
  msg.send();
}

void SGOfFreeAreEqual::takeControl( )
{
  LogMessage msg( subgroup1, subgroup2 );
  msg << Name( subgroup1 ) << " and " << Name( subgroup2 ) << " are";

  if ( ! subgroup1.getSubgroup().equalTo( makeSetOf( 
	 subgroup2.getSubgroup().generators() ) ) )
    msg << " not";
  
  msg << " equal.";
  msg.send();
}

void SGOfFreeJoin::takeControl( )
{
  SGofFreeGroup S2 = subgroup2.getSubgroup();
  SMObject* smo =
    new SMSubgroup( subgroup1.getParent(),
		    subgroup1.getSubgroup().join( S2 ),
		    Text("The join of") + Name( subgroup1 ) + Text("and")
		    + Name( subgroup2 )
		    );
  
  ListOf<OID> dependencies( subgroup1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void SGOfFreeIntersection::takeControl( )
{
  SGofFreeGroup S2 = subgroup2.getSubgroup();
  SMObject* smo =
    new SMSubgroup( subgroup1.getParent(),
		    subgroup1.getSubgroup()
		    .intersection( S2 ),
		    Text("The intersection of") + Name( subgroup1 ) + Text("and")
		    + Name( subgroup2 )
		    );
  
  ListOf<OID> dependencies( subgroup1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void SGOfFreeIsNormal::takeControl( )
{
  LogMessage msg( subgroup );
  msg << Name( subgroup ) << " is ";
  
  bool isnormal = subgroup.getSubgroup().isNormal();

  subgroup.sic.putIsNormal(isnormal);

  if ( ! isnormal )
	 msg << "not ";



  msg << "normal in " << Name( Parent( subgroup ) ) << ".";
  msg.send();
}

void SGOfFreeIsAFreeFactor::takeControl( )
{
  LogMessage msg( subgroup );
  msg << Name( subgroup ) << " is ";

  if ( ! subgroup.getSubgroup().isAFreeFactor() )
	 msg << "not ";

  msg << "a free factor of " << Name( Parent( subgroup ) ) << ".";
  msg.send();
}

// Conjugates each word of the given vector with the word `w'.
// This is duplicate of static function in SGofFreeGroup.C
// @dp: This function is for SGOfFreeIsMalnormal::takeControl() only.

static VectorOf<Word> conjugateBy(const VectorOf<Word>& V, const Word& w)
{ 
  VectorOf<Word> result(V.length());
  for( int i = 0; i < V.length(); ++i ) 
    result[i] = Word( Word(V[i]).conjugateBy(w) ).freelyReduce();
  return result;
}

void SGOfFreeIsMalnormal::takeControl( )
{
  LogMessage msg( subgroup );
  Word conjugator;
  SGofFreeGroup S = subgroup.getSubgroup();
  
  if ( S.isMalnormal( conjugator ) ) 
    msg << Name( subgroup ) << " is malnormal in " 
	<< Name( Parent( subgroup ) ) << ".";
  else {
    FPGroup G = subgroup.getParent().getFPGroup();
    SGofFreeGroup S1( FreeGroup(G.numberOfGenerators()), 
		      conjugateBy(S.generators(), conjugator) );
    SGofFreeGroup S2 = S.intersection( S1 );
    msg << Name( subgroup ) << " is not malnormal in " 
	<< Name( Parent( subgroup ) ) << " because an intersection of " 
	<< Name( subgroup ) << " and " 
	<< Name( subgroup ) << " conjugated by ";
    G.printWord( msg, conjugator );
    msg << " is not trivial. The intersection is gp( ";
    int numOfGens = S2.generators().length();
    for( int i = min( 4, numOfGens-1 ); i >= 0; --i ) {
      G.printWord( msg, S2.generators()[i] );
      if( i != 0) msg << ", ";
    }
    if( numOfGens > 5 ) msg << ", ...";
    msg << " ).";
  }

  msg.send();
}

void QuadEquationSurfaceForm::takeControl( )
{
  LogMessage msg( equation );
  msg << "A surface form of " << Name( equation ) << " : ";

  QEqnSolutionsInFreeGroup solver( equation.getFreeGroup(), equation.getWord(),
				   equation.numberOfVariables() );
  Word surf = solver.surfaceForm();
  equation.getFreeGroup().printWord( msg, surf );
  msg << ".";
  msg.send();

  LogMessage msg1( equation );
  msg1 << "Mapped by automorphism : ";
  msg1.send();

  LogMessage msg2( equation );
  solver.toSurfaceForm().printOn(msg2);
  msg2.send();
}


void SGOfFreeWhiteheadReduction::takeControl( )
{
  SGofFreeGroup sg(vect.getParent().getFreePreimage(), vect.getWords());
  VectorOf<Word> answer = sg.findWhiteheadBasis();
  SMObject* smo =
    new SMVectorOfWords( vect.getParent(), answer,
			 Text("A Whitehead reduction of tuple") + Name(vect));

  ListOf<OID> dependencies( vect.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void SGOfFreeNielsenBasis::takeControl( )
{
  const FreeGroup F = subgroup.getParent().getFreePreimage();
  const VectorOf<Word> generators = subgroup.getSubgroup().generators();
  FreeGroup::NielsenBasis nielsenBasisProblem = F.nielsenBasis( generators, true );
  VectorOf<Word> nielsenBasis = nielsenBasisProblem.newGenerators();

  // Print the basis into a file.  
  File file;
  file.setColor( titleColor );
  file << "The generators of a Nielsen basis for subgroup of free group:"
       << endl << endl;
  file.setColor( mainColor );
  for( int i = 0; i < nielsenBasis.length(); ++i ) {
    file << 'x' << i+1 << " = ";
    F.printWord( file, nielsenBasis[i] );
    file <<endl;
  }
  file << end;
  
  {
    // Make link to the file with computed nielsen basis.
    LogMessage msg( subgroup );
    msg << Link( Chars("Click here to see the generators of a Nielsen basis of ")+
		 Text( Name(subgroup) ), "NielsenBasis", 
		 file.getFileName() ) << ".";
    msg.send();
  }

  {
    // Make link to the file with details of computation of nielsen basis.
    LogMessage msg( subgroup );
    msg << Link( Chars("Click here to see computation details of a Nielsen basis of ")
		 + Text( Name(subgroup) ), "NielsenBasisDetails",
		 nielsenBasisProblem.getFileName() ) << "."; 
    msg.send();
  }
}


void SGOfFreeIndex::takeControl( )
{
  LogMessage msg( subgroup );
  msg << Name( subgroup ) << " has ";
  int index = subgroup.getSubgroup().findIndex();
  if ( index == 0 ) msg << "infinite index";
  else msg << "index " << index;
  msg << " in " << Name( Parent( subgroup ) ) << ".";
  
  msg.send();
}

void SGOfFreeRank::takeControl( )
{
  LogMessage msg( subgroup );
  msg << "The rank of " << Name( subgroup ) << " is  "
      << subgroup.getSubgroup().rank() << ".";
  msg.send();
}


void SGOfFreeNormaliser::takeControl( )
{
  SMObject* smo =
    new SMSubgroup( subgroup.getParent(),
		    SGofFreeGroup(subgroup.getParent().getFreePreimage(),
				  subgroup.getSubgroup().normalizer() ),
		    Text("The normaliser of") + Name( subgroup ) + Text("in")
		    + Name( Parent( subgroup ) ) );
  ListOf<OID> dependencies( subgroup.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void SGOfFreeHallCompletion::takeControl( )
{
  SMObject* smo =
    new SMSubgroup( subgroup.getParent(),
		    subgroup.getSubgroup().MHallCompletion(),
		    Text("A finite index subgroup of")
		    + Name( Parent( subgroup ) ) + "with"
		    + Name( subgroup ) + Text("as a free factor") );
  ListOf<OID> dependencies( subgroup.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void NormalApproximationProblem::takeControl()
{
  SMFPGroup& G = subgroup.getParent();
  VectorOf<Word> v = subgroup.getSubgroup().generators();
  SetOf<Word> s;
  for(int i=0; i<v.length(); i++) s |= v[i];
  for(int i=1; i<=level; i++) {
    Word w = G.getFreePreimage().getN_thElement(i);
    SetOf<Word> s1;
    SetIterator<Word> I(s);
    for(I.reset(); !I.done(); I.next()) s1 |= (w*I.value()*w.inverse()).freelyReduce();
    s |= s1;
  }
  VectorOf<Word> v1;
  SetIterator<Word> I(s);
  for(I.reset(); !I.done(); I.next()) v1.append(I.value());
  SMObject* smo =
    new SMSubgroup( G, SGofFreeGroup(G.getFreePreimage(), v1),
		    Text(level) + "-th Normal Approximation of" 
		    + Name(subgroup) );
  ListOf<OID> dependencies( G );
  CheckinMessage( *smo, "", dependencies ).send();
}

void FreeIsSGTrivial::takeControl( )
{
  LogMessage msg( subgroup );

  msg << Name( subgroup ) << " is";

  if ( !subgroup.getSubgroup().isTrivial() )
    msg << " not";
  msg << " trivial in " << Name( Parent( subgroup ) ) << ".";
  
  msg.send();
}


void FreeIsAutomatic::takeControl( )
{
  LogMessage msg( group );

  msg << "Free groups are automatic. If X freely generates the "
         "finitely generated free group F, then (A,L) is a rational structure "
         "for F, where here A is the union of X and the set of all inverses of"
         " elements of X and L is the set of all reduced X-words.";
  
  msg.send();
}

void FreeIsHyperbolic::takeControl( )
{
  LogMessage msg( group );

  msg << "Free groups are hyperbolic.";
  
  msg.send();
}

//---------------------------------------------------------------------------//
//---------------------- Misc Fast Computations -----------------------------//
//---------------------------------------------------------------------------//



void FreelyReduceWord::takeControl( )
{
  SMObject* smo =
    new SMWord( theWord.getParent(),
		theWord.getWord().freelyReduce(),
		Text("The freely reduced form of") + Name( theWord )
		);

  ListOf<OID> dependencies( theWord.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
  
  //@rn Factor out somewhere
}


void CyclicallyReduceWord::takeControl( )
{
  SMObject* smo =
    new SMWord( theWord.getParent(),
		theWord.getWord().cyclicallyReduce(),
		Text("The cyclically reduced form of") + Name( theWord ) );
  
  ListOf<OID> dependencies( theWord.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void FormalInverseOfWord::takeControl( )
{
  SMObject* smo =
    new SMWord( theWord.getParent(),
		theWord.getWord().inverse(),
		Text("The inverse of") + Name( theWord )
		);
  
  ListOf<OID> dependencies( theWord.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void WordLength::takeControl( )
{
  SMFPGroup& theGroup = theWord.getParent();
  Word w = theWord.getWord().freelyReduce();
  if( theGroup.gic.isAbelian()==yes)
    w = AbelianWord(theGroup.getFPGroup().numberOfGenerators(), w).getWord();
  LogMessage msg( theWord );
  msg << "The length of " << Name( theWord ) 
      << " in the freely reduced form is equal to " 
      << w.length() << ".";
  msg.send();
}


void InitialSegmentOfWord::takeControl( )
{
  SMFPGroup& theGroup = theWord.getParent();
  Word w = theWord.getWord();
  if(theGroup.gic.isAbelian()==yes)
    w = AbelianWord(theGroup.getFPGroup().numberOfGenerators(), w).getWord();
  int trueLen = min( theLength, w.length() );
  char s[10];
  sprintf(s,"%u",trueLen);
  SMObject* smo =
    new SMWord( theGroup, w.initialSegment( trueLen ),
		Text("The ") + Text(s) + Text("-th initial segment of ")
		+ Name( theWord ) );
  
  ListOf<OID> dependencies( theGroup );
  CheckinMessage( *smo, "", dependencies ).send();
  
  //@rn Factor out somewhere
}

void TerminalSegmentOfWord::takeControl( )
{
  SMFPGroup& theGroup = theWord.getParent();
  Word w = theWord.getWord();
  if(theGroup.gic.isAbelian()==yes)
    w = AbelianWord(theGroup.getFPGroup().numberOfGenerators(), w).getWord();
  int trueLen = min( theLength, w.length() );
  char s[10];
  sprintf(s,"%u",trueLen);
  SMObject* smo =
    new SMWord( theGroup, w.terminalSegment( trueLen ),
		Text("The ") + Text(s) + Text("-th terminal segment of ")
		+ Name( theWord ) );
  ListOf<OID> dependencies( theGroup );
  CheckinMessage( *smo, "", dependencies ).send();
}


void SegmentOfWord::takeControl( )
{
  SMFPGroup& theGroup = theWord.getParent();
  Word w = theWord.getWord();
  if(theGroup.gic.isAbelian()==yes)
    w = AbelianWord(theGroup.getFPGroup().numberOfGenerators(), w).getWord();
  int start = min( w.length(), theStart );
  int stop  = min( w.length(), theStart + theLength - 1 );
  char startText[10];
  sprintf(startText,"%u",start);
  char lenText[10];
  sprintf( lenText,"%u", stop-start+1 );
  SMObject* smo =
    new SMWord( theGroup, w.subword(start-1, stop),
		Text("User defined a subword of length") + Text(lenText)
		+ Text("(starting from") + startText + Text(") of")
		+ Name( theWord ) );
  ListOf<OID> dependencies( theGroup );
  CheckinMessage( *smo, "", dependencies ).send();
}


void FreeGetNextN_thElement::takeControl( )
{
  SMFPGroup& theGroup = theWord.getParent();
  const FreeGroup& F = theGroup.getFreePreimage();
  int number = F.numberOfElement( theWord.getWord() ) + theNumber;
  char s[20];
  sprintf(s,"%u", theNumber );
  SMObject* smo =
    new SMWord( theGroup, F.getN_thElement(number),
		Text("The") + Text(s) + Text("- th element of")
		+ Name( theGroup ) + Text("(after the word") 
		+ Name(theWord) + Text("in lex order)") );
  ListOf<OID> dependencies( theGroup );
  CheckinMessage( *smo, "", dependencies ).send();  
}



void FormalProductOfWords::takeControl( )
{
  SMObject* smo =
    new SMWord( word1.getParent(),
		(word1.getWord() * word2.getWord()).freelyReduce(),
		Text("The product of") + Name( word1 )
		+ Text("and") + Name( word2 ) );
  ListOf<OID> dependencies( word1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void ConjugateOfWord::takeControl( )
{
  SMObject* smo =
    new SMWord( theWord1.getParent(),
		(theWord2.getWord().inverse() * theWord1.getWord() 
		 * theWord2.getWord()).freelyReduce(),
		Text("The conjugate of") + Name( theWord1 )
		+ Text("by") + Name( theWord2 )	);

  ListOf<OID> dependencies( theWord1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void CommutatorOfWords::takeControl( )
{
  SMObject* smo =
    new SMWord( theWord1.getParent(),
		(theWord1.getWord().inverse() * theWord2.getWord().inverse()
		 * theWord1.getWord() * theWord2.getWord()).freelyReduce(),
		Text("The commutator of") + Name( theWord1 )
		+ Text("and") + Name( theWord2 ) );
  
  ListOf<OID> dependencies( theWord1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void PowerOfMap::takeControl( )
{
  //@rn Need new member in Map to do this
	
  Map m = map.getMap();
  Map result = composition(m, m);
  // We know the power is at least 2, since we put that restriction
  // in the menu definition.
	 
  int count = power - 1;
  while ( --count ) result = composition(m, result);

  SMObject* smo =
    new SMMap( map.getDomain(), map.getRange(), result,
	       Text("The map") + Name( map ) + Text("to the power") + power );
  ListOf<OID> dependencies( map.getDomain() );
  dependencies.append( map.getRange() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void ComposeMaps::takeControl( )
{
  if( map1.getDomain().oid() == map2.getRange().oid() ) {
    SMHomomorphism* smo = new SMHomomorphism
      ( map1.getDomain(),
	composition( map2.getMap(), map1.getMap() ),
	Text("The composition") + Name( map2 ) + Text(Name( map1 )) );
    
    ListOf<OID> dependencies( map1.getDomain() );
    CheckinMessage( *smo, "", dependencies ).send();
  }

  else {
    SMHomomorphism2* smo = new SMHomomorphism2
      ( map1.getDomain(), map2.getRange(),
	composition( map2.getMap(), map1.getMap() ),
	Text("The composition") + Name( map2 ) + Text(Name( map1 )) );
    
    ListOf<OID> dependencies( map1.getDomain() );
    dependencies.append( map2.getRange() );
    CheckinMessage( *smo, "", dependencies ).send();
  }
}


void FreeAreHomsEqual::takeControl( )
{
  FreeGroup F = map1.getDomain().getFreePreimage();
  int numOfGens = F.numberOfGenerators();
  Map m1 = map1.getMap();
  Map m2 = map2.getMap();
  
  bool bEqual = true;
  for( int i = 0; i < numOfGens; ++i )
    if( !F.areEqual(m1.generatingImages(i), m2.generatingImages(i))) {
      bEqual = false;
      break;
    }
  
  LogMessage msg( map1, map2 );
  
  msg << Name( map1 ) << " is ";

  if ( !bEqual )  msg << "not ";
  msg << "equal to " << Name( map2 ) << ".";
  
  msg.send();
}


void ImageUnderMap::takeControl( )
{
  Word tmpWord = map.getMap().imageOf( word.getWord() );
  if ( map.getRange().gic.isAbelian()==yes)
    tmpWord = AbelianWord( map.getRange().getFPGroup().numberOfGenerators(),
			   tmpWord ).getWord();
  SMObject* smo =
    new SMWord( map.getRange(),tmpWord,
		Text("The image of") + Name( word ) 
		+ Text("under") + Name( map ) );
  
  ListOf<OID> dependencies( map.getRange() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void SGImageUnderMap::takeControl( )
{
  Map M = map.getMap();
  Word tmpWord;
  VectorOf<Word> gens = subgroup.getSubgroup().generators();
  int VLen = gens.length();
  for( int i = 0; i < VLen; ++i ){
    tmpWord = M.imageOf(gens[i]).freelyReduce();
    if ( map.getRange().gic.isAbelian()==yes)
      tmpWord = AbelianWord( map.getRange().getFPGroup().numberOfGenerators(),
			       tmpWord ).getWord();    
    gens[i] = tmpWord;
  }
      
  SMObject* smo = new SMSubgroup
    ( map.getRange(),
      SGofFreeGroup( map.getRange().getFreePreimage(), gens ),
      Text("The image of") + Name( subgroup ) + Text("under") + Name( map )
      );
  
  ListOf<OID> dependencies( map.getRange() );
  CheckinMessage( *smo, "", dependencies ).send();
}


ExtendFreeByAut::ExtendFreeByAut(const SMMap& m)
: theGroup( m.getDomain() ), theMap( m ) { }


void ExtendFreeByAut::takeControl( )
{
  //@rn  An unfortunate chicken-and-egg problem: to make the SMFPGroup,
  //@rn  we must supply the FPGroup, but we can't do that without the
  //@rn  FreeByCylic, which knows what the generators and relations are.
  //@rn  The real FreeByCylic is held by the GIC, which does not exist
  //@rn  until the SMFPGroup does.
  //@rn  So we make a fake, temp FreeByCylic (jeez).

  FreeByCyclic temp( theGroup.getFreePreimage(), theMap.getMap() );
  
  SMFPGroup* smo =
    new SMFPGroup( FPGroup( temp.namesOfGenerators(), temp.getRelators() ),
		   Text("Extension of") + Name(theGroup) + Text("by") + Name(theMap)
		   );

  smo->gic.putIsFreeByCyclic( theGroup.getFreePreimage(), theMap.getMap() );

  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();
}

static 
bool expressAsConjugate( const SetOf<Word>& S, const Word& w, 
			 Word& u, Word& conjugator )
{
  SetIterator<Word> I(S);
  for( ; !I.done(); I.next() ) {
    u = I.value();
    if( u.length() != w.length() )
      continue;
    for( int i = 0; i < u.length(); ++i ) {
      if( w == u.cyclicallyPermute( i ) ) {
	conjugator = u.initialSegment(i);
	return true;
      }
    }
  }
  u = conjugator = Word();
  return false;
}

static 
void printRelatorAsConjugateOfOriginalOne( 
  File& file,  const FPGroup& G, const Word& relator, int relNo )
{
  Word originalRelator, conjugator;
  int power = 1;

  if( !expressAsConjugate( G.getRelators(), relator, 
			   originalRelator, conjugator ) ) {
    power = -1;
    expressAsConjugate( G.getRelators(), relator.inverse(), 
			originalRelator, conjugator );
    originalRelator = originalRelator.inverse();
  }

  if( conjugator.length() == 0 ) {
    file << "r" << relNo << " is ";
    if( power == -1 ) 
      file << " inverse of ";
    file << "the original relator.";
  }
  else {
    file << "r" << relNo 
	 << " can be expressed as a conjugate of the original relator"
	 << "as follows:\n      r" << relNo << " = (";
    G.printWord( file, originalRelator );
    file << ") ^ (";
    G.printWord( file, conjugator );
    file << ").";
  }
  file << '\n' << endl;
}

static 
Chars mscInfo( const FPGroup& G )
  // Prints info about structure of the given MSC-presentation 
  // in the file and returns the file name.
{
  File file;
  // 1. Prints symmetrized set of relators.
  SymmetricRelators symRels = G.getRelators();
  SymmetricRelatorsIterator I( symRels );

  file << 
"The symmetrized set of relators for the given presentation is as follows:\n ";
  int n = 0;
  for( ; !I.done(); I.next() ) {
    file << "r" << ++n << " = ";
    G.printWord( file, I.value() );
    file << '\n';
  }
  file << '\n';

  int lambda = G.cancellationLambda();

  if( !lambda ) {
    file << "The relators have no common pieces." << '\n';
  }
  else {
    // 2. Shows the longest common piece.
    Word R1, R2;
    int N1 = 0, N2 = 0;
    int rMinLen = 1, rCommonLen = 0;

    int i = 1;
    for( I.reset(); !I.done(); I.next(), ++i ) {
      Word r1 = I.value();
      SymmetricRelatorsIterator J = I;
      int j = i+1;
      for( J.next(); !J.done(); J.next(), ++j ) {
	Word r2 = J.value();
	int commonLen = r1.agreementLength( r2 );
	int minLen = min( r1.length(), r2.length() );

	if(  commonLen != 0 && minLen * rCommonLen < commonLen * rMinLen ) {
	  // found longest relative common piece.
	  N1 = i;
	  N2 = j;
	  R1 = r1;
	  R2 = r2;
	  rMinLen = minLen;
	  rCommonLen = commonLen;
	}
      }
    }
    file << "r" << N1 << " and r" << N2 << " have longest common piece, ";
    G.printWord( file, R1.initialSegment(rCommonLen) );
    file << ".\n";
    
    // 3. Expresses r1 and r2 as conjugates of the original relators.
    file << '\n';
    printRelatorAsConjugateOfOriginalOne( file, G, R1, N1 );
    printRelatorAsConjugateOfOriginalOne( file, G, R2, N2 );
  }

  file << end;

  return file.getFileName();
}

void FPIsMSC::takeControl( )
{
  LogMessage msg( theGroup );

  int lambda = theGroup.getFPGroup().cancellationLambda();

  if ( lambda < 6 && lambda != 0 ) {

    msg << "The presentation of " << Name( theGroup )
	<< " is not metric small cancellation; it is only C'(1/"
	<< lambda << ')';
    msg.send();
  } else {
    File file;
    
    theGroup.gic.putHaveMSC( theGroup.getFPGroup(), lambda );
    
    msg << "The presentation of " << Name( theGroup ) << " is C'(";
    if ( lambda != 0 ) msg << "1/";
    msg << lambda << "). ";
    msg.send();
    
    LogMessage msg1( theGroup );
    msg1 << Link( Chars("Click here to details about the group presentation"),
		  "MSCInfo", mscInfo( theGroup.getFPGroup() ) );      
    msg1.send();
  }
}

void FastHomology::takeControl( )
{
  // We assume that the menu item which causes this to be invoked ensures
  // that the selected group is one-relator, or abelian.
  //@rn The abelian case is not handled yet.

  LogMessage msg( theGroup );
  msg << "The integral homology groups of " << Name( theGroup ) << " are: ";

  // Which case are we doing?

  if ( theGroup.gic.isOneRelatorWithTorsion() ) {
    // We rely on the assumption that the GCM has already checked not only
    // for 1-relatorness, but for torsion.
    
    // Worth the cost to compute max'l root again:
    Word rel = theGroup.gic.getOneRelator();
    int e = maximalRoot( rel );
    Word r = rel.initialSegment( e );
    
    // Dimension 2 is the only special case:
    
    msg << "H_2( " << Name( theGroup ) << " ) = ";
    if ( r.allExponentSumsZero() ) {
      msg << "Z_" << e << ", ";
    } else {
      msg << "0, ";
    }
    
    // All other dimensions:
    
    msg << "H_2n( " << Name( theGroup ) << " ) = 0 for n > 1, ";
    msg << "H_2n-1( " << Name( theGroup ) << " ) = Z_" << e
	<< " for n > 1.";

    msg.send();

  } else if ( theGroup.gic.isOneRelator() ) {

    // We rely on the assumption that the GCM has already checked not only
    // for 1-relatorness.

    // Dimension 2 is the only special case:
    
    msg << "H_2( " << Name( theGroup ) << " ) = ";
    if ( theGroup.gic.getOneRelator().allExponentSumsZero() ) {
      msg << "Z, ";
    } else {
      msg << "0, ";
    }
    
    // All other dimensions:

    msg << "H_n( " << Name( theGroup ) << " ) = 0 for n > 2.";

    msg.send();
  } else if ( theGroup.getCheckinType() == SMFPGroup::ABELIAN ) {

    //@rn Temporarily bad case.
    Warning msg;
    msg << "Homology of abelian groups not implemented yet; ignoring request.";
    msg.send();

  } else {
	 
    // Bad case.
    Warning msg;
    msg << "Bad static group type passed to FastHomology; ignoring request.";
    msg.send();
  }
}


void SubgroupJoin::takeControl( )
{
  SetOf<Word> S(makeSetOf(subgroup2.getSubgroup().generators()));
  VectorOf<Word> gensS1 = subgroup1.getSubgroup().generators();
  for (int i=0;i<gensS1.length();i++)
    S |= gensS1[i];
  
  SGofFreeGroup SG( subgroup1.getSubgroup().parentGroup(), makeVectorOf(S) );

  SMObject* smo =
    new SMSubgroup( subgroup1.getParent(), SG,
		    Text("The join of") + Name( subgroup1 ) + "and"
		    + Name( subgroup2 )
		    );
  
  ListOf<OID> dependencies( subgroup1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void SubgroupConjugateBy::takeControl( )
{
  VectorOf<Word> result(theSubgroup.getSubgroup().generators());

  for( int i = 0; i < result.length(); ++i ) 
    result[i] = Word( Word(result[i]).conjugateBy(theWord.getWord()) ).freelyReduce();
  
  SGofFreeGroup SG( theSubgroup.getSubgroup().parentGroup(), result );

  SMObject* smo =
    new SMSubgroup( theSubgroup.getParent(), SG,
		    Text(Name( theSubgroup )) + Text("conjugated by") + Name( theWord )
		    );
  
  ListOf<OID> dependencies( theSubgroup.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


//--------------------- Abelian Group Computations --------------------------//


void FastAbelianForm::takeControl( )
{
  FPGroup G = theWord.getParent().getFPGroup();
  AbelianGroup A(G);
  LogMessage msg( theWord );
  msg << "The reduced form of " << Name( theWord ) << " is ";
  G.printWord( msg, A.oldInAbelianForm( theWord.getWord() ).getWord() );
  msg << ".";
  msg.send();
}


void FastInverseInAbelianForm::takeControl( )
{
  FPGroup G = theWord.getParent().getFPGroup();
  AbelianGroup A(G);
  LogMessage msg( theWord );
  msg << "The inverse of " << Name( theWord ) << " is ";
  //  << " in abelian form is ";
  G.printWord( msg, 
	       A.oldInAbelianForm( theWord.getWord() ).inverse().getWord() );
  msg << ".";
  msg.send();
}


void ProductInAbelianForm::takeControl( )
{
  FPGroup G = word1.getParent().getFPGroup();
  AbelianGroup A(G);
  /*  LogMessage msg( word1, word2 );
      msg << "The product " << Name( word1 ) << " " << Name( word2 ) << " is ";
      G.printWord( msg, A.oldInAbelianForm( word1.getWord() * word2.getWord() )
      .getWord() );
      msg << ".";
      msg.send();
  */

  SMObject* smo =
    new SMWord( word1.getParent(),
		A.oldInAbelianForm( word1.getWord() * word2.getWord() )
		.getWord(),
		Text("The product of") + Name( word1 )
		+ Text("and") + Name( word2 ));
  ListOf<OID> dependencies( word1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void AbelianSGJoin::takeControl( )
{
  AbelianGroup A(subgroup1.getParent().getFPGroup());
  SGofFreeGroup S2(subgroup1.getParent().getFreePreimage(),
		   A.joinSubgroups(subgroup1.getSubgroup().generators(),
				   subgroup2.getSubgroup().generators()));
  SMObject* smo = new SMSubgroup( subgroup1.getParent(),S2,
				  Text("The join of") + Name( subgroup1 ) 
				  + Text("and") + Name( subgroup2 ) );
  ListOf<OID> dependencies( subgroup1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void AbelianIsAutomatic::takeControl()
{
  LogMessage("All abelian groups are automatic.").send();
}

void AbelianIsConfluent::takeControl()
{
  LogMessage("All abelian groups have a confluent rewriting system.").send();
}

//----------------------- MSC Group Computations ----------------------------//

void MSCOrder::takeControl( )
{
  if( theGroup.gic.haveOrder() )
    LogMessage(theGroup.gic.getOrderMessage()).send();
  else
    theGroup.gic.putHaveOrder( theGroup.gic.getMSC().order() );
}

void MSCIsTrivial::takeControl( )
{
  bool answer = ( theGroup.gic.getMSC().isTrivial() == yes );
  
  LogMessage msg( *this, theGroup );
  
  msg << Name( theGroup ) << " is";
  
  if( !answer )
    msg << " not";
  
  msg << " trivial.";

  msg.send();
}


void MSCIsFinite::takeControl( )
{
  if( theGroup.gic.isFinite() != dontknow )
    LogMessage(theGroup.gic.getFiniteMessage()).send();
  else {
    bool answer = ( theGroup.gic.getMSC().isFinite() == yes );
    theGroup.gic.putIsFinite(answer);
  }
}


void MSCIsAbelian::takeControl( )
{
  bool answer = ( theGroup.gic.getMSC().isAbelian() == yes );
  
  LogMessage msg( *this, theGroup );
  
  msg << Name( theGroup ) << " is";
  
  if( !answer )
    msg << " not";
  
  msg << " abelian.";

  msg.send();
  if( answer ) theGroup.gic.putIsAbelian();
}


// ------------------ One Relator Group Computations ----------------------- //


void ORIsTrivial::takeControl( )
{
  bool answer = ORProblems( theGroup.getFPGroup().numberOfGenerators(),
			    theGroup.gic.getOneRelator() ).isTrivial();
  
  LogMessage msg( theGroup );
  
  msg << Name( theGroup ) << " is";
  
  if( !answer )
    msg << " not";
  
  msg << " trivial.";

  msg.send();
}


void ORIsAbelian::takeControl( )
{
  bool answer = ORProblems( theGroup.getFPGroup().numberOfGenerators(),
			    theGroup.gic.getOneRelator() ).isAbelian();
  
  LogMessage msg( theGroup );
  
  msg << Name( theGroup ) << " is";
  
  if( !answer )
    msg << " not";
  
  msg << " abelian.";

  msg.send();
  if( answer ) theGroup.gic.putIsAbelian();
}


void ORIsFinite::takeControl( )
{
  if( theGroup.gic.isFinite() != dontknow )
    LogMessage(theGroup.gic.getFiniteMessage()).send();
  else {
    bool answer = ORProblems( theGroup.getFPGroup().numberOfGenerators(),
			      theGroup.gic.getOneRelator() ).isFinite();
    theGroup.gic.putIsFinite( answer );
  }
}

void OROrder::takeControl( )
{
  if( theGroup.gic.haveOrder() )
    LogMessage(theGroup.gic.getOrderMessage()).send();
  else {
    int theOrder = ORProblems( theGroup.getFPGroup().numberOfGenerators(),
			       theGroup.gic.getOneRelator() ).order();
    theGroup.gic.putHaveOrder( theOrder );
  }
}

/*
void ORWithTorsionWordProblem::takeControl()
{
  SMFPGroup& theGroup = theWord.getParent();
  OneRelatorGroupWithTorsion  G( theGroup.getFPGroup().namesOfGenerators(), 
				 theGroup.gic.getOneRelator() );

  ProductOfRelatorConjugates prodDeco;
  Trichotomy trivial = G.wordProblem( theWord.getWord(), prodDeco );

  if( trivial == yes ) {
    DetailedReport report( theGroup.getFPGroup() );
    report.printDehnTransformationDetails( theWord.getWord() );

    LogMessage msg( theWord );
    msg << Name( theWord ) << " is trivial in " << Name( theGroup )
	<< " -- this follows by using Dehn's algorithm."
	<< Link( Chars("Click here to see details of the computation"),
		 "WordProblemDetails", report.getFileName()
	       );
    msg.send();
  }
  else if( trivial == no ) {
    LogMessage msg( theWord );
    msg << Name( theWord ) << " is not trivial in " << Name( theGroup ) 
	<< " -- this follows by using Dehn's algorithm.";
    msg.send();
  }
  else {
    LogMessage msg( theWord );
    msg << "Internal error -- Magnus cannot determine whether " 
	<< Name( theWord ) << " is trivial or not in " << Name( theGroup ) 
	<< " by using Dehn's algorithm.";
    msg.send();
  }
}
*/

void ORWithTorsionAreEltsEqual::takeControl()
{
  SMFPGroup& theGroup = theWord1.getParent();
  OneRelatorGroupWithTorsion  G( theGroup.getFPGroup().namesOfGenerators(), 
				 theGroup.gic.getOneRelator() );
  Word elt = (theWord1.getWord()*theWord2.getWord().inverse()).freelyReduce();
  ProductOfRelatorConjugates prodDeco;
  Trichotomy trivial = G.wordProblem( elt, prodDeco );

  if( trivial == yes ) {
    DetailedReport report( theGroup.getFPGroup() );
    report.file() << Name(theWord1) << " * " << Name(theWord2) << "^-1 = ";
    G.printWord( report.file(), elt );
    report.file() << ".\n";
    report.printDehnTransformationDetails( elt );

    LogMessage msg( theWord1, theWord2 );
    msg << Name( theWord1 ) << " is equal to " << Name(theWord2) 
	<< " in " << Name( theGroup ) 
	<< " -- this follows by using Dehn's algorithm."
	<< Link( Chars("Click here to see details of the computation"),
		 "WordProblemDetails", report.getFileName()
	       );
    msg.send();
  }
  else if( trivial == no ) {
    LogMessage msg( theWord1, theWord2 );
    msg << Name( theWord1 ) << " is not equal to " << Name(theWord2) 
	<< " in " << Name( theGroup ) 
	<< " -- this follows by using Dehn's algorithm.";
    msg.send();
  }
  else {
    LogMessage msg( theWord1, theWord2 );
    msg << "Internal error -- Magnus cannot determine whether " 
	<< Name( theWord1 ) << " is equal to " << Name( theWord2 ) 
	<< " -- by using Dehn's algorithm.";
    msg.send();
  }
}


void ORWithTorsionExtendedWordProblem::takeControl()
{
  OneRelatorGroupWithTorsion 
    G( theWord.getParent().getFPGroup().namesOfGenerators(), 
       theWord.getParent().gic.getOneRelator() );

  VectorOf<Word> subgroupWords = theSubgroup.getSubgroup().generators();
  VectorOf<Generator> subgroupGens( subgroupWords.length() );

  for( int i = 0; i < subgroupWords.length(); ++i ) {
    Word gen = subgroupWords[i].freelyReduce();
    if( gen.length() != 1 ) {
      Message msg("Warning");
      msg << "The given subgroup is not a Magnus one.";
      msg.send();
      return;
    }
    subgroupGens[i] = gen[0];
  }

  if( !G.isProperMagnusSubgroup(subgroupGens) ) {
    Message msg("Warning");
    msg << "The given subgroup is not a Magnus one.";
    msg.send();
    return;
  }

  Word wDecomposition;
  bool answer = G.doesMagnusSubgroupContainElt( subgroupGens, 
						theWord.getWord(), 
						wDecomposition
						);

  LogMessage msg( theSubgroup, theWord );

  msg << Name( theWord ) << " is ";
  if ( !answer ) msg << "not ";
  msg << "expressed in generators of " << Name(theSubgroup);
  
  if( answer ) {
    msg << " : " << Name(theWord) << " = ";
    theSubgroup.getSubgroup().printWord( msg, wDecomposition );
  }
  msg << '.';
  msg.send();
  
}


void ORWithTorsionEltFiniteOrder::takeControl()
{
  OneRelatorGroupWithTorsion
    G( theWord.getParent().getFPGroup().namesOfGenerators(), 
       theWord.getParent().gic.getOneRelator() );

  Word w = theWord.getWord();
  Word st, x;
  ProductOfRelatorConjugates wDeco;
  int order = G.powerOfElt( w, st, x, wDeco );
  
  {  
    // Report the order of the element .
    LogMessage msg( theWord );
    msg << Name(theWord);
    if( order ) 
      msg << " has order " << order;
    else
      msg << " has infinite order";
    msg << ".";
    msg.send();
  }

  {
    // Write into secondary logfile a decomposition of the element.
    File file;
    file.setColor( titleColor );
    file << "Decomposition of the word is \n\n";
    file.setColor( mainColor );

    if( st.length() ) {
      if( x.length() ) file << "(";
      G.printWord( file, st );
      if( x.length() ) {
	file << ") ^ (";
	G.printWord( file, x);
	file << ")";
      }
    }
    
    if( wDeco.length() ) {
      
      Word theRelator = G.relator();
      int decoLength = wDeco.length();
      for( int i = 0; i < decoLength; ++i ) {
	  file.setColor( titleColor );
	  if( i || i == 0 && order != 1 ) 
	    file << " * ";
	  file << ( wDeco[i].relator == theRelator ? "r^(" : "R^(" );
	  file.setColor( mainColor );
	  G.printWord( file, wDeco[i].conjugator );
	  file.setColor( titleColor );
	  file << ')';
      }
    }
    file << '\n'<< end;

    LogMessage msg( theWord );
    msg << Link( Chars("Click here to see decomposition of  ") 
		 + Text( Name(theWord) ), "OREltDeco", file.getFileName()
		 );      
    msg << '.';
    msg.send();
  }
}

void ORWithTorsionCentralizerOfElt::takeControl()
{
  OneRelatorGroupWithTorsion 
    G( theWord.getParent().getFPGroup().namesOfGenerators(), 
       theWord.getParent().gic.getOneRelator()
     );
  Word w = theWord.getWord();

  LogMessage msg( theWord );
  msg << "The centralizer of " << Name(theWord) 
      << " in one-relator group " << Name(theWord.getParent()) 
      << " with torsion is generated by ";
  G.printVectorOfWords(msg, G.centralizerOfElt( w ));
  msg << ".";
  msg.send();
}


void ORWithTorsionConjugacyProblem::takeControl()
{
  Word theRelator = word1.getParent().gic.getOneRelator();
  OneRelatorGroupWithTorsion 
    G( word1.getParent().getFPGroup().namesOfGenerators(), theRelator );
  
  Word conjugator;
  Trichotomy answer = G.conjugacyProblem( word1.getWord(), word2.getWord(),
					  conjugator );

  LogMessage msg( word1,word2 );
  if ( answer == yes )
    {
      msg << Name( word1 ) << " and  " << Name( word2 ) 
	  << " are conjugate in " << Name( word1.getParent() ) << " : ";
      msg << "X " << Name( word1 ) << " x = " <<  Name( word2 ) 
	  << ", where x = ";
      word1.getParent().getFPGroup().printWord(msg,conjugator);
    }
  else {
    Word maxRoot;
    int maxPower;
    maximalRootInFreeGroup( theRelator, maxRoot, maxPower );
    if( maxPower == 2 ) {
      msg << "Magnus cannot determine whether " << Name( word1 ) << " and " 
	  << Name( word2 ) << " are conjugate or not in " 
	  << Name( word1.getParent() ) << " -- a solution can be found for N>2"
	  << "where a defining relator R=S^N";
    }
    else {
      msg << Name( word1 ) << " and " << Name( word2 ) 
	  << " are not conjugate in " << Name( word1.getParent() );
    }
  }
  
  msg << ".";
  msg.send();

}


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Make Menu:                                                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


void MakeCyclicDecomposition::takeControl( )
{
  if( !theGroup.gic.haveCyclicDecomposition() ) {

    Message msg;

    msg << 
      "To construct canonical presentation of an abelian group you have"
      " to compute it first. To do so just choose the corresponding item in "
      "the tools menu.";

    msg.send();
  }

  else {

    AbelianGroup CD = theGroup.gic.getCyclicDecomposition()
      .getCanonicalSmithPresentation();
    
    SMFPGroup* smo
      = new SMFPGroup( CD.getFPGroup(),
		       Text("The canonical presentation of")
		       + Name( theGroup ),
		       SMFPGroup::ABELIAN
		       );
    
    smo->gic.putIsAbelian();
    smo->gic.putHaveCanonicalSmithPresentation(CD);
    
    ListOf<OID> dependencies;
    CheckinMessage( *smo, "", dependencies ).send();
    
    VectorOf<AbelianWord> a1 = 
      theGroup.gic.getCyclicDecomposition().oldToNewGens();
    VectorOf<Word> v1( a1.length() );
    for( int i = 0; i < a1.length(); ++i )
      v1[i] = a1[i].getWord();
 
    VectorOf<AbelianWord> a2 = 
      theGroup.gic.getCyclicDecomposition().newToOldGens();
    VectorOf<Word> v2( a2.length() );
    for( int i = 0; i < a2.length(); ++i )
      v2[i] = a2[i].getWord();
    

    SMHomomorphism2* hom1 = new SMHomomorphism2
      ( theGroup, *smo,
	Map( theGroup.getFPGroup(), smo->getFPGroup(), v1 ),
	Text("The isomorphism from generators of") + Name(theGroup)
	+ Text("to") + Name(*smo)
	);

    dependencies = ListOf<OID>( theGroup );
    dependencies.append( *smo );
    CheckinMessage( *hom1, "", dependencies ).send();


    SMHomomorphism2* hom2 = new SMHomomorphism2
      ( *smo, theGroup,
	Map(  smo->getFPGroup(), theGroup.getFPGroup(), v2 ),
	Text("The isomorphism from generators of") + Name(*smo)
	+ Text("to") + Name(theGroup)
	);

    dependencies = ListOf<OID>( *smo );
    dependencies.append( theGroup );
    CheckinMessage( *hom2, "", dependencies ).send();
  }
}


// ------------------------------ Quotients -------------------------------- //



void MakeQuotientFromSubgroup::takeControl( )
{
  if( theSubgroup.sic.isNormal() == dontknow && theSubgroup.getParent().gic.isAbelian() != yes) {

    Message msg;

    msg << 
      "Please use the subgroup tool in Magnus which determines if a subgroup is normal.";
    msg.send();
  } else if ( theSubgroup.sic.isNormal() == no  && theSubgroup.getParent().gic.isAbelian() != yes) {
     Message msg;
     
     msg << "Subgroup " << Name( theSubgroup ) << " in not normal. Cannot make quotient group " 
	 << Name( theSubgroup.getParent()) << "/" << Name(theSubgroup) << ".";
     msg.send(); 
  } else {
    SMFPGroup& pGroup = theSubgroup.getParent();

    SetOf<Word> newRelators( pGroup.getFPGroup().getRelators());
    VectorOf<Word> sGens = theSubgroup.getSubgroup().generators();

    for (int i=0;i<sGens.length();i++)
      newRelators |= sGens[i];

    FPGroup G( pGroup.getFPGroup().namesOfGenerators(),
	       newRelators);
    
    Chars text = Text("Quotient group ") + Name( pGroup ) + Text("/") + Name(theSubgroup);
    SMFPGroup* smo;
    
    if( pGroup.gic.isAbelian() == yes ) {
      smo = new SMFPGroup( G, text, SMFPGroup::ABELIAN );
      smo->gic.putIsAbelian();
    }
    else if( pGroup.gic.isNilpotent() == yes ) {
      
      int theClass = pGroup.gic.getNilpotentcyClass();
      char strClass[10];
      sprintf(strClass,"%u", theClass);
      
      smo = new SMFPGroup( G, text, SMFPGroup::NILPOTENT );
      smo->gic.putIsNilpotent( theClass );
    }
    else 
      smo = new SMFPGroup( G, text );
    
    ListOf<OID> dependencies;
    CheckinMessage( *smo, "", dependencies ).send();
    
    
    // Make the homomorphism from the group to its quotient
    
    int numOfGens = pGroup.getFPGroup().numberOfGenerators();
    VectorOf<Word> v(numOfGens);
    for( int i = 0; i < numOfGens; ++i )
      v[i]  = Word(Generator(i+1));
    
    SMHomomorphism2* hom = new SMHomomorphism2
      ( pGroup, *smo, Map( pGroup.getFPGroup(), smo->getFPGroup(), v),
	Text("The homomorphism from generators of") + Name(pGroup)
	+ Text("to") + Name(*smo)
	);
    
    dependencies = ListOf<OID>( pGroup );
    dependencies.append( *smo );
    CheckinMessage( *hom, "", dependencies ).send();
    
  }
}

void MakeAbelianQuotient::takeControl( )
{
  SMFPGroup* smo
    = new SMFPGroup( theGroup.getFPGroup(),
		     Text("User defined abelian quotient of")
		     + Name( theGroup ),
		     SMFPGroup::ABELIAN
		     );
  
  smo->gic.putIsAbelian();

  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();


  // Make the homomorphism from the group to its quotient

  int numOfGens = theGroup.getFPGroup().numberOfGenerators();
  VectorOf<Word> v(numOfGens);
  for( int i = 0; i < numOfGens; ++i )
    v[i]  = Word(Generator(i+1));

  SMHomomorphism2* hom = new SMHomomorphism2
    ( theGroup, *smo, Map( theGroup.getFPGroup(), smo->getFPGroup(), v),
      Text("The homomorphism from generators of") + Name(theGroup)
      + Text("to") + Name(*smo)
      );
  
  dependencies = ListOf<OID>( theGroup );
  dependencies.append( *smo );
  CheckinMessage( *hom, "", dependencies ).send();
}


void MakeNilpotentQuotient::takeControl( )
{
  if( ( theGroup.gic.isNilpotent() == yes 
	|| theGroup.gic.isFreeNilpotent() == yes ) 
      && theClass >= theGroup.gic.getNilpotentcyClass( ) ) {

    Message msg;
    msg << "This number is bigger than the class of " 
	<< Name( theGroup ) << "."; 
    msg.send();
    return;
  }

  char strClass[10];
  sprintf(strClass,"%u",theClass);
  
  SMFPGroup* smo
    = new SMFPGroup( theGroup.getFPGroup(),
		     Text("User defined nilpotent quotient of class")
		     + Text(strClass) + Text("of") + Name( theGroup ),
		     SMFPGroup::NILPOTENT
		     );
  
  smo->gic.putIsNilpotent( theClass );
  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();


  // Make the homomorphism from the group to its quotient

  int numOfGens = theGroup.getFPGroup().numberOfGenerators();
  VectorOf<Word> v(numOfGens);
  for( int i = 0; i < numOfGens; ++i )
    v[i]  = Word(Generator(i+1));

  SMHomomorphism2* hom = new SMHomomorphism2
    ( theGroup, *smo, Map( theGroup.getFPGroup(), smo->getFPGroup(), v),
      Text("The homomorphism from generators of") + Name(theGroup)
      + Text("to") + Name(*smo)
      );
  
  dependencies = ListOf<OID>( theGroup );
  dependencies.append( *smo );
  CheckinMessage( *hom, "", dependencies ).send();
}


void MakeQuotient::takeControl( )
{
  FPGroup G( theGroup.getFPGroup().namesOfGenerators(),
	     theGroup.getFPGroup().getRelators());
  G.addRelators(relators);

  Chars text = Text("User defined quotient of") + Name( theGroup );
  SMFPGroup* smo;
  
  if( theGroup.gic.isAbelian() == yes ) {
    smo = new SMFPGroup( G, text, SMFPGroup::ABELIAN );
    smo->gic.putIsAbelian();
  }
  else if( theGroup.gic.isNilpotent() == yes ) {
    
    int theClass = theGroup.gic.getNilpotentcyClass();
    char strClass[10];
    sprintf(strClass,"%u", theClass);
    
    smo = new SMFPGroup( G, text, SMFPGroup::NILPOTENT );
    smo->gic.putIsNilpotent( theClass );
  }
  else 
    smo = new SMFPGroup( G, text );
  
  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();


  // Make the homomorphism from the group to its quotient

  int numOfGens = theGroup.getFPGroup().numberOfGenerators();
  VectorOf<Word> v(numOfGens);
  for( int i = 0; i < numOfGens; ++i )
    v[i]  = Word(Generator(i+1));

  SMHomomorphism2* hom = new SMHomomorphism2
    ( theGroup, *smo, Map( theGroup.getFPGroup(), smo->getFPGroup(), v),
      Text("The homomorphism from generators of") + Name(theGroup)
      + Text("to") + Name(*smo)
      );
  
  dependencies = ListOf<OID>( theGroup );
  dependencies.append( *smo );
  CheckinMessage( *hom, "", dependencies ).send();
}


// ------------------------------ Products --------------------------------- //


void MakeAPOfFree::takeControl( )
{
  SGofFreeGroup S1 = subgroup1.getSubgroup( );
  SGofFreeGroup S2 = subgroup2.getSubgroup( );

  if( S1.generators().length() != S2.generators().length() ) {
    Message msg;
    msg << "The number of generators of the subgroups has to be equal.";
    msg.send();
    return;
  }

  if( S1.nielsenBasis().length() != S2.nielsenBasis().length() ) {
    Message msg;
    msg << "The associated subgroups must have the same rank.";
    msg.send();
    return;
  }
  
  SMFPGroup* smo;

  
  if( S1.generators().length() == 1 ) {

    APwithOneRelator AP(S2, S1);

    smo = new SMFPGroup
      ( AP,
	Text("User defined amalgamated product <")
	+ Name(Parent(subgroup2)) + Text("*") + Name(Parent(subgroup1)) + Text(";")
	+ Name(subgroup2) + Text("=") + Name(subgroup1) + Text(">"),
	SMFPGroup::AP_FREE_CYCLIC
	);

    smo->gic.putHaveAPOfFree(AP);
  }
  else {

    AmalgProductOfFreeGroups AP(S2, S1);

    smo = new SMFPGroup
    ( AP,
      Text("User defined amalgamated product <")
      + Name(Parent(subgroup2)) + Text("*") + Name(Parent(subgroup1)) + Text(";")
      + Name(subgroup2) + Text("=") + Name(subgroup1) + Text(">"),
      SMFPGroup::AP_FREE
      );

    smo->gic.putHaveAPOfFree(AP);
  }
 
  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();
}


void APOfFreeReducedForm::takeControl( )
{
  FPGroup G = theWord.getParent().getFPGroup();
  VectorOf<Word> dec = theWord.getParent().gic.getAPOfFree()
    .reducedDecomposition(theWord.getWord());
  
  LogMessage msg( theWord );
  msg << "The reduced form of " << Name( theWord ) << " is ";
  G.printVectorOfWords(msg, dec);
  msg << ".";
  msg.send();
}


void APOfFreeNormalForm::takeControl( )
{
  FPGroup G = theWord.getParent().getFPGroup();
  VectorOf<Word> dec = theWord.getParent().gic.getAPOfFree()
    .normalDecomposition(theWord.getWord());
  
  LogMessage msg( theWord );
  msg << "The normal form of " << Name( theWord ) << " is ";
  G.printVectorOfWords(msg, dec);
  msg << ".";
  msg.send();
}


void APOfFreeCyclicNormalForm::takeControl( )
{
  FPGroup G = theWord.getParent().getFPGroup();
  VectorOf<Word> dec;
  Word conjugator;
  
  theWord.getParent().gic.getAPOfFree()
    .cyclicDecomposition(theWord.getWord(), dec, conjugator);
  
  LogMessage msg( theWord );
  msg << "The cyclic normal form of " << Name( theWord ) << " is ";
  G.printVectorOfWords(msg, dec);
  msg << ".";
  msg.send();
}


void APOfFreeIsTrivial::takeControl( )
{
  bool answer = ( theGroup.gic.getAPOfFree().isTrivial() == yes );
  
  LogMessage msg( *this, theGroup );
  msg << Name( theGroup ) << " is";
  if( !answer )
    msg << " not";
  msg << " trivial.";

  msg.send();
}

void APOfFreeIsHyperbolic::takeControl( )
{
  AmalgProductOfFreeGroups G = theGroup.gic.getAPOfFree();
  Trichotomy answer = G.isHyperbolic();
  
  LogMessage msg( *this, theGroup );
  
  if( answer == dontknow )
    msg << "Magnus cannot determine whether " << Name( theGroup ) << " is hyperbolic.";
  else {
    msg << Name( theGroup );
    if( answer == yes ) 
      msg << " is hyperbolic because at least one of the associated subgroups is malnormal.";
    else
      msg << " is not hyperbolic.";
  }

  msg.send();
}

void APOfFreeIsFinite::takeControl( )
{
  bool answer = ( theGroup.gic.getAPOfFree().isFinite() == yes );
  
  LogMessage msg( *this, theGroup );
  msg << Name( theGroup ) << " is ";
  if( !answer )
    msg << "in";
  msg << "finite.";

  msg.send();
}


void APOfFreeIsAbelian::takeControl( )
{
  bool answer = ( theGroup.gic.getAPOfFree().isAbelian() == yes );
  
  LogMessage msg( *this, theGroup );
  msg << Name( theGroup ) << " is";
  if( !answer )
    msg << " not";
  msg << " abelian.";

  msg.send();
}


void APOfFreeOrder::takeControl( )
{
  if( theGroup.gic.haveOrder() )
    LogMessage(theGroup.gic.getOrderMessage()).send();
  else
    theGroup.gic.putHaveOrder( theGroup.gic.getAPOfFree().order() );
}


void APOfFreeWordProblem::takeControl( )
{
  bool answer = theWord.getParent().gic.getAPOfFree()
    .wordProblem( theWord.getWord() );

  LogMessage msg( theWord );

  msg << Name( theWord ) << " is";
  if( !answer )
    msg << " not";
  msg << " trivial";
  msg << " in " << Name( Parent( theWord ) ) << ".";

  msg.send();
}


void APOfFreeNumberOfSubstitutions::takeControl( )
{
  LogMessage msg( theWord );

  FPGroup G = theWord.getParent().getFPGroup();
  AmalgProductOfFreeGroups AP( theWord.getParent().gic.getAPOfFree() );

  if( AP.wordProblem( theWord.getWord() ) ) {

    int numOfSubsts = AP.numberOfSubstitutions( theWord.getWord() );    
    msg << "The number of uses of a relation a = b  to deduce that " 
	<< Name( theWord ) << " = 1 in " << Name( theWord.getParent() ) 
	<< " is " << numOfSubsts;
  }
  else {
    msg << "The word " << Name( theWord ) << " is not represent 1 in "
	<< Name( theWord.getParent() );
  }
  msg << ".";

  msg.send();
}



void APOfFreeAreEqual::takeControl( )
{
  bool answer = word1.getParent().gic.getAPOfFree()
    .areEqual( word1.getWord(), word2.getWord() );
  
  LogMessage msg( word1, word2 );

  msg << Name( word1 ) << " is";
  if ( !answer )
    msg << " not";
  msg << " equal to " << Name( word2 )
    // @am added explanation
      << ": they have different normal forms"
      << ".";

  msg.send();
}


void APOfFree_DoEltsCommute::takeControl( )
{
  bool answer = word1.getParent().gic.getAPOfFree()
    .commute( word1.getWord(), word2.getWord() );
  
  LogMessage msg( word1, word2 );

  msg << Name( word1 );
  if ( !answer )
    msg << " does not commute";
  else
    msg << " commutes";
  msg << " with " << Name( word2 ) << ".";

  msg.send();
}


void APOfFreeIsSGTrivial::takeControl( )
{
  bool answer = subgroup.getParent().gic.getAPOfFree()
    .isSubgroupTrivial( subgroup.getSubgroup().generators() );

  LogMessage msg( subgroup );

  msg << Name( subgroup ) << " is";

  if ( !answer )
    msg << " not";
  msg << " trivial in " << Name( Parent( subgroup ) ) << ".";
  
  msg.send();
}


void CheckinAPOfFree::takeControl( )
{
  Message msg;
  msg << 
    "Check-in the two factors and their respective subgroups, which must "
    "have the same number of generators. The generators of the two "
    "subgroups must be arranged so that the mapping which sends the n-th "
    "generator of the first subgroup to the n-th generator of the second "
    "subgroup defines an isomorphism between them. Then go to the Make "
    "menu. (Note: amalgamated products are, as of now, only implemented "
    "when the factors are checked in as free.)";
  msg.send();
}


void APOfFreeIsSGAbelian::takeControl( )
{
  bool answer =
    ( theSubgroup.getParent().gic.getAPOfFree()
      .isSubgroupAbelian( theSubgroup.getSubgroup().generators() ) );
  
  LogMessage msg( *this, theSubgroup );
  msg << Name( theSubgroup ) << " is";
  if( !answer )
    msg << " not";
  msg << " abelian.";

  msg.send();
}


void APOfFreeCyclic_CentralizerOfElt::takeControl( )
{
  FPGroup G = theWord.getParent().getFPGroup();
  APwithOneRelator AP( theWord.getParent().gic.getAPOfFree() );
  VectorOf<Word> cent = AP.centralizerOf( theWord.getWord() );
  
  LogMessage msg( theWord );
  msg << "The centralizer of " << Name( theWord ) << " is gp ";
  G.printVectorOfWords(msg, cent, "(", ")");
  msg << ".";
  msg.send();
}

void APOfFreeCyclic_ConjugacyProblem::takeControl( )
{
  Word conjugator;
  APwithOneRelator AP( word1.getParent().gic.getAPOfFree() );
  Trichotomy answer =
    AP.conjugacyProblem( word1.getWord(), word2.getWord(), conjugator );

  LogMessage msg( word1, word2 );

  if( answer == dontknow )

    msg << "Magnus cannot determine whether " << Name( word1 ) << " and "
	<< Name( word2 ) << " are conjugate or not";

  else {
    msg << Name( word1 ) << " and " << Name( word2 ) << " are";

    if( answer == no )
      msg << " not conjugate";
    else {
      msg << " conjugate: " << "X " << Name( word1 ) << " x = "
	  << Name( word2 ) << ", where x = ";
      word1.getParent().getFPGroup().printWord( msg, conjugator );
    }
  }

  msg << ".";
  msg.send();
}


void APOfFreeCyclic_MaximalRoot::takeControl( )
{
  Word root;
  int power;

  APwithOneRelator AP( theWord.getParent().gic.getAPOfFree() );
  AP.maximalRoot( theWord.getWord(), root, power );

  SMObject* smo =
    new SMWord( theWord.getParent(), root,
		Text("The maximal root of") + Name( theWord )
		);
  
  ListOf<OID> dependencies( theWord.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void APOfFreeCyclic_IsEltAProperPower::takeControl( )
{
  Word root;
  int power;

  APwithOneRelator AP( theWord.getParent().gic.getAPOfFree() );
  AP.maximalRoot( theWord.getWord(), root, power );

  LogMessage msg( theWord );
  msg << Name( theWord );

  if ( power > 1 ) {
    msg << " is a proper power. " << Name( theWord ) << " is equal ";
    theWord.getParent().getFPGroup().printWord(msg, root);
    msg << " in power " << power;
  }
  else 
    msg << " is not a proper power";
  
  msg << ".";

  msg.send();
}


void APOfFreeCyclic_IsEltAProperPowerOfSecond::takeControl( )
{
  int power;
  APwithOneRelator AP( word1.getParent().gic.getAPOfFree() );
  bool answer = AP.isProperPowerOfSecond( word1.getWord(), word2.getWord(), power );

  LogMessage msg( word1, word2 );

  msg << Name( word1 ) << " is";
  if ( !answer )
    msg << " not";
  msg << " a proper power of " << Name( word2 ) << ".";
  if( answer )
    msg << Name( word1 ) << " is equal " << Name( word2 ) << " in power " << power << ".";
  msg.send();
}

//---------------------------------------------------------------------------//
//--------------------- Nilpotent  Group Computations -----------------------//
//---------------------------------------------------------------------------//


void FNGAutoIsIAAut::takeControl( )
{
  LogMessage msg( map );
  msg << Name( map ) << " is ";
  int theNumberOfGenerators = map.getDomain().getFPGroup().numberOfGenerators();
  VectorOf<Word> V =  map.getMap().generatingImages();
  bool answer = true;

  for( int i = theNumberOfGenerators-1; i >= 0; --i ) {
    AbelianWord tmp( theNumberOfGenerators, inv(Generator(i+1)) * V[i] );
    if( tmp.fullLength() != 0 )
      answer = false;
  }
  
  if( !answer )
    msg << "not ";

  msg << "an IA-automorphism of " << Name( Domain( map ) ) << ".";
  msg.send();
}

void SGOfNGjoinSubgroupProblem::takeControl( )
{
  FPGroup theGroup = subgroup1.getParent().getFPGroup();
  NilpotentGroup group(theGroup.namesOfGenerators(),
		       subgroup1.getParent().gic.getNilpotentcyClass(),
		       makeVectorOf(theGroup.getRelators()));

  SGOfNilpotentGroup SG1(group, subgroup1.getSubgroup().generators());
  VectorOf<Word> SG2 = subgroup2.getSubgroup().generators();

  VectorOf<Word> basis = SG1.join(SG2);
  SGofFreeGroup F(subgroup1.getParent().getFreePreimage(), basis);

  SMObject* smo = 
    new SMSubgroup( subgroup1.getParent(), F,
		    Text("The join of") + Name( subgroup1 ) + Text("and")
		    + Name( subgroup2 )
		    );

  ListOf<OID> dependencies( subgroup1.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
  
}

void NGLCStermGensProblem::takeControl( )
{
  if (number > group.gic.getNilpotentcyClass()){
    Message msg;
    msg << "Number of quotient is bigger than nilpotency class of the group.";
    msg.send();
    return;
  }
  FPGroup theGroup = group.getFPGroup();
  NilpotentGroup ng(theGroup.namesOfGenerators(),
		       group.gic.getNilpotentcyClass(),
		       makeVectorOf(theGroup.getRelators()));

  VectorOf<Word> lcsTerm = ng.getLCSterm( number );
  SGofFreeGroup F(group.getFreePreimage(),lcsTerm);

  SMObject* smo = 
    new SMSubgroup( group, F,
		    Text("The ") + Text(Chars(number)+ordinalPostfix(number))+
		    Text(" term of the lower central series of ") + Name( group ) 
		    );

  ListOf<OID> dependencies( group );
  CheckinMessage( *smo, "", dependencies ).send();
  
}
// ------------------------------------------------------ //
// --------------- Products ------------------------------//
// ------------------------------------------------------ //
MakeFreeProduct::MakeFreeProduct( class SMFPGroup& g1, class SMFPGroup& g2) 
  : group1(g1), group2(g2), 
    fp( NULL ),
    fpGroup1(g1.getFPGroup()), fpGroup2(g2.getFPGroup()),
    isAbelian( false )
{
  if (g1.gic.isNilpotent() == yes || g1.gic.isFreeNilpotent() == yes ||
      g2.gic.isNilpotent() == yes || g2.gic.isFreeNilpotent() == yes )
    error("MakeFreeProduct( class SMFPGroup& g1, class SMFPGroup& g2) : "
	  "can not compute products of nilpotent groups");
  
  if (g1.gic.isAbelian() == yes && g2.gic.isAbelian() == yes ){
    isAbelian = true;
    return;
  }
  
  if (g1.gic.isAbelian() == yes &&  fpGroup1.numberOfGenerators() > 0 ){
    CommutatorIterator I( fpGroup1.numberOfGenerators() , 2 );
    SetOf<Word> relators;
    for (I.reset();!I.done();I.next()){
      if (I.value().length() > 0)
	relators |= I.value();
    }    
    fpGroup1.addRelators( relators );
  } 
  
  if (g2.gic.isAbelian() == yes &&  fpGroup2.numberOfGenerators() > 0){
    
    CommutatorIterator I( fpGroup2.numberOfGenerators() , 2 );
    SetOf<Word> relators;
    
    for (I.reset();!I.done();I.next()){
      if (I.value().length() > 0)
	relators |= I.value();
    }
    fpGroup2.addRelators( relators );
    
  } 

}
void MakeFreeProduct::makeHomomorphisms(SMFPGroup* smo)
{
  SMHomomorphism2* hom1 = new SMHomomorphism2
    ( group1, *smo, fp->mapFromG1(),
      Text("The homomorphism from generators of") + Name(group1)
      + Text("to") + Name(*smo)
      );

     ListOf<OID> dependencies = ListOf<OID>( group1 );
    dependencies.append( *smo );
    CheckinMessage( *hom1, "", dependencies ).send();

    SMHomomorphism2* hom2 = new SMHomomorphism2
      ( group2, *smo, fp->mapFromG2(),
	Text("The homomorphism from generators of") + Name(group2)
	+ "to" + Name(*smo)
	);
    
    dependencies = ListOf<OID>( group2 );
    dependencies.append( *smo );
    CheckinMessage( *hom2, "", dependencies ).send();

    SMHomomorphism2* hom3 = new SMHomomorphism2
      ( *smo, group1, fp->mapToG1(),
	Text("The homomorphism from generators of") + Name(*smo)
	+ "to" + Name(group1)
	);
    
    dependencies = ListOf<OID>( *smo );
    dependencies.append( group1 );
    CheckinMessage( *hom3, "", dependencies ).send();

    SMHomomorphism2* hom4 = new SMHomomorphism2
      ( *smo, group2, fp->mapToG2(),
	Text("The homomorphism from generators of") + Name(*smo)
	+ "to" + Name(group2)
	);
    
    dependencies = ListOf<OID>( *smo );
    dependencies.append( group2 );
    CheckinMessage( *hom4, "", dependencies ).send();  
}

void MakeFreeProduct::takeControl( )
{

  fp = new FreeProduct(fpGroup1,fpGroup2);

  SMFPGroup::Checkin_Type productType = SMFPGroup::FP;
  if (isAbelian)
    productType = SMFPGroup::ABELIAN;
  
  SMFPGroup* smo = 
    new   SMFPGroup(*fp,   Text("Free product ") + Name( group1 )
		    + Text(" * ")+Name( group2 ),productType );
  

  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();

  makeHomomorphisms( smo );

}

void MakeDirectProduct::takeControl( )
{

  fp = new DirectProduct(fpGroup1,fpGroup2);
  
  SMFPGroup::Checkin_Type productType = SMFPGroup::FP;
  if (isAbelian)
    productType = SMFPGroup::ABELIAN;

  SMFPGroup* smo = 
    new   SMFPGroup(*fp,   Text("Direct product"), 
		    productType,
		    Text(Name( group1 )) + Text(" X ") + Name( group2 )
		    ); 
  
  
  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();
  
  File file;
  FPGroup G = *fp;
  file << G << '\n';

  LogMessage msg( *smo );
  msg << Link( Chars("Click here to see presentation of ") + 
	       Text( Name( *smo) ),
	       "GroupPresentation",
	       file.getFileName() 
	       );
  msg << ".";
  msg.send();

  makeHomomorphisms( smo );

}

void MakeFactorGroup::takeControl( )
{
  FreeGroup F = theGroup.getFreePreimage();
  SGofFreeGroup H = theSubgroup.getSubgroup();

  if( theGroup.getCheckinType() == SMFPGroup::FREE && ! H.isNormal() 
      || theGroup.getCheckinType() != SMFPGroup::FREE 
         && theSubgroup.sic.isNormal() == dontknow ) {
    Message msg;
    msg << "Please first check whether the subgroup "
	<< Name(theSubgroup) << " is normal or not.";
    msg.send();
    return;
  }

  FPGroup G( F.namesOfGenerators(), makeSetOf(H.generators()) );

  SMFPGroup::Checkin_Type productType = SMFPGroup::FP;
  if( theGroup.gic.isAbelian() == yes )
    productType = SMFPGroup::ABELIAN;

  SMFPGroup* smo = 
    new SMFPGroup( G, Text("Factor group"), 
		   SMFPGroup::FP,
		   Text(Name( theGroup )) + Text("/") + Name( theSubgroup )
		   ); 
  
  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();
  
  File file;
  file << G << '\n';

  LogMessage msg( *smo );
  msg << Link( Chars("Click here to see presentation of ") + 
	       Text( Name( *smo) ),
	       "GroupPresentation",
	       file.getFileName() 
	       );
  msg << ".";
  msg.send();

}



// -------------------- SMList ------------------------ //

void MakeListOfWords::takeControl()
{
  
  SMList<Word>* smo = 
    new   SMList<Word>(enumerator,
		       Text("List of words from ") + 
		       Name(enumerator)
		       ); 
  
  
  ListOf<OID> dependencies( smo->getGroup() );
  CheckinMessage( *smo, "", dependencies ).send();
  
  LogMessage msg( *smo );
  msg << Link( Chars("Click here to see the contents of ") + 
	       Text( Name( *smo) ),
	       "WEnumerator",
	       smo->getData().getDataFileName() 
	       );
  msg.send();
     
}


// --------------------------- RipsConstruction ---------------------------- //


void MakeRipsConstruction::takeControl( )
{
  File f;
  FPGroup G = RipsConstruction(theGroup.getFPGroup())
    .getRipsConstruction(&f);

  if( G.numberOfGenerators() == 0 ) {
    Message msg;
    msg << "Cannot make the Rips construction: no free letters available";
    msg.send();
    return;
  }

  SMFPGroup* smo = 
    new SMFPGroup (G, Text("User defined the Rips construction of") 
		   + Name( theGroup ), SMFPGroup::FP);
  
  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies, false ).send();
  
  LogMessage msg( *this, *smo );
  msg << Link( Chars("Click here to see the relators of ") + 
	       Text( Name( *smo) ), "RipsConstruction", f.getFileName() );
  msg.send();
}
#line 1  "src/menuDefns.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of ObjectSmith methods outputCheckinMenuDefns,
//           outputToolsMenuDefns
//
// Principal Author: Dmitry Bormotov
//
// Status: Always being added to.
//
// Revision History:
//
// * 1/96 Dmitry B. completely rewrote almost all tools menus.
//

#include "ObjectSmith.h"

/////////////////////////////////////////////////////////////////////////
// Include the header for everything that can be made by choosing a
// menu item here:
//

#include "SMFPGroup.h"
#include "SMWord.h"
#include "SMEquation.h"
#include "SMEnumerator.h"
#include "SMHomomorphism.h"
#include "SMSetOfWords.h"
#include "SMEqSystem.h"
#include "SMPermutation.h"

#include "fastProblems.h"
#include "WordProblem.h"
#include "KBModule.h"
#include "AbelianProblems.h"
#include "NilpotentQuotients.h"
#include "QuadEquationSolver.h"
#include "HomologyProblem.h"
#include "FreeProblems.h"
#include "OrderProblem.h"
#include "ConjugacyProblem.h"
#include "NilpotentProblems.h"
//#include "FNGroupProblems.h"
#include "FreeIsPartOfBasisProblem.h"
#include "IsTrivialProblem.h"
#include "IsFreeProblem.h"
#include "IsFiniteProblem.h"
#include "ExtendToHomProblem.h"
#include "IsAbelianProblem.h"
#include "IsNilpotentProblem.h"
#include "IsEltCentral.h"
#include "AreEltsEqual.h"
#include "OrderOfElt.h"
#include "SubgroupProblems.h"
#include "HNNProblems.h" 
#include "OneRelatorProblems.h"
#include "TTProblem.h"
#include "ToddCoxeter.h"
#include "GAEquations.h"
#include "ACConjecture.h"
#include "WhiteheadMinimal.h"
#include "KernelPresentation.h"
#include "PresentationProblems.h"
#include "GAConjProblemForORGroup.h"
#include "RankOfSubgroup.h"
#include "RankOfElt.h"
#include "IsWordAPE.h"
#include "ACE.h"
#include "MakeRandomPresentation.h"

// Lists and Enumerators

#include "SGREnumerator.h"
#include "WEnumerator.h"
#include "REnumerator.h"
#include "AutoEnumerator.h"
#include "SGREnumerator.h"
#include "SubgroupEnumerator.h"
#include "ORConsequencesEnumerator.h"
#include "SubgroupContainmentProblem.h"
#include "Rewrites.h"
#include "HomEnumerators.h"
#include "EquationsInFPProblem.h"
#include "ListProblems.h"
#include "SMListWordProblem.h"
#include "SMListSubgroupProblems.h"
#include "FreeListProblems.h"
#include "PreAbelianRepProblem.h"

// --------------- ObjectSmith::outputCheckinMenuDefns --------------------- //


void ObjectSmith::outputCheckinMenuDefns(ostream& ostr)
{

  
// ----------------------------- Groups ------------------------------------ //

  {
    Menu0 M( Menu::CHECKIN );
	 


    M.startItemGroup();

    M.add( ReadSMAbelianGroup(), Text("Abelian Group"), DefineAbelianGroup() );

    //M.addDisabled( "Automatic Group" );

    M.add( Ctor0<CheckinAPOfFree>(), Text("Amalgamated Product") );

    M.add( ReadSMFPGroup(), Text("Finitely Presented Group"), DefineFPGroup() );

    M.add( ReadSMFreeGroup(), Text("Free Group"), DefineFreeGroup() );

    //M.addDisabled( "Hyperbolic Group" );

    //M.addDisabled( "Matrix Group" );

    //M.addDisabled( "Metabelian Group" );

    M.add( ReadSMFreeNilpotentGroup(), Text("Free Nilpotent Group"),
	   DefineFreeNilpotentGroup() );

    M.add( Ctor0<CheckinHNNofFreeGroup>(), Text("HNN-extension") );
    
    M.add( ReadSMNilpotentGroup(), Text("Nilpotent Group"),
	   DefineNilpotentGroup() );

    M.add( ReadSMORGroup(), Text("One Relator Group"), DefineORGroup() );

    M.addDisabled( Text("Permutation Group") );

    //M.addDisabled( "Polycyclic Group" );

    M.add( ReadSMSmallCancGroup(), Text("Small Cancellation Group"),
	   DefineSmallCancGroup() );

    //M.separator();

    //M.addDisabled( "Direct Product" );

    //M.addDisabled( "Semidirect Product" );
	 
    //M.addDisabled( "Free Product" );

    //M.addDisabled( "Tree Product" );

    //M.addDisabled( "Graph Product" );

    M.separator();
    
    M.startCascade( Text("Group definition templates") );

    M.add( ReadSymmetricGroup(), Text("Symmetric group"), 
	   DefineInt("Enter the number N", 2) );

    M.closeCascade();

    M.separator();
    
    M.add( ReadSMPermutation(), Text("Permutation"), DefinePermutation() );
	 
    M.done();
  }

//----------------------------- Substructures -------------------------------//

  {
    Menu1<SMFPGroup> M( Menu::CHECKIN );

    M.startItemGroup();
	 
    M.add(ReadSMWord(), Text("Word in the generators of") + Name(1),
	  DefineWord());

    M.add(ReadSMSubgroup(), Text("Subgroup of") + Name(1),
	  DefineSubgroup());

    M.add(ReadSMMap(),
	  Text("Map from the generators of") + Name(1) + "to" + Name(1),
	  DefineMap() );

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FP ||
		      CheckinType( 1 ) == SMFPGroup::SMALL_CANCELLATION ||
		      CheckinType( 1 ) == SMFPGroup::ONE_RELATOR ||
		      CheckinType( 1 ) == SMFPGroup::ONE_RELATOR_WITH_TORSION);
    
    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FREE );

    M.add(ReadSMEquation2(),
	  Text("Equation in") + Name(1),
	  DefineEquation2() );

    M.add(ReadSMEquation(),
	  Text("Quadratic equation in") + Name(1),
	  DefineEquation() );

    M.add(ReadSMEqSystem(),
	  Text("System of equations in") + Name(1),
	  DefineEqSystem() );

    M.add(ReadSMSetOfWords(), Text("Set of words in") + Name(1),
	  DefineSetOfWords());

    M.add(ReadSMVectorOfWords(), Text("Tuple of words in") + Name(1),
	  DefineVectorOfWords());

 	 
    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FP ||
		      CheckinType( 1 ) == SMFPGroup::ABELIAN );
    
    M.add(ReadSMEquation2(), Text("Equation in") + Name(1),
	  DefineEquation2());
    
    M.add(ReadSMEqSystem(), Text("System of equations in") + Name(1),
	  DefineEqSystem());	 
    
    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FP );
    
    M.add(ReadSMVectorOfWords(), Text("Tuple of words in") + Name(1),
	  DefineVectorOfWords());

    M.done();
  }

  {
    Menu2<SMFPGroup,SMFPGroup> M( Menu::CHECKIN );
    
    M.startItemGroup();
    
    M.add(ReadSMMap2(),
	  Text("Map from the generators of") + Name(1) + "to" + Name(2),
	  DefineMap2() );
    
    M.add(ReadSMMap2(true),
	  Text("Map from the generators of") + Name(2) + "to" + Name(1),
	  DefineInverseMap2() );
    
    M.done();
  }
}

//---------------------------------------------------------------------------//
//----------------- ObjectSmith::outputToolsMenuDefns -----------------------//
//---------------------------------------------------------------------------//

void ObjectSmith::outputToolsMenuDefns(ostream& ostr)
{
  {
    Menu1<SMFPGroup> M( Menu::TOOLS );

    /* template
    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::TYPE );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    M.closeCascade();

    M.startCascade( Text("Compute ...") );
    M.closeCascade();

    M.startCascade( Text("Find ...") );
    M.closeCascade();

    M.startCascade( Text("Enumerate ...") );
    M.closeCascade();
    */

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::ABELIAN );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor1<IsTrivialProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("trivial ?") );

    M.add( Ctor1<IsFiniteProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("finite ?") );
    
    M.add( Ctor1<IsFreeProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("free abelian ?") );
    
    M.add( Ctor1<AbelianIsAutomatic, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );

    M.add( Ctor1<IsAbelianHyperbolic, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("hyperbolic ?") );
  
    M.closeCascade();

    M.startCascade( Text("Compute ...") );
    
    M.add( Ctor1<AbelianOrderProblem, SMFPGroup>(),
	   Text("Compute the order of") + Name(1) );

    M.add( Ctor1<AbelianComputeTorsionSubgroup, SMFPGroup>(),
	   Text("Compute the torsion subgroup of") + Name(1) );

    M.add( Ctor1<AbelianOrderOfTheTorsionSubgroupProblem, SMFPGroup>(),
	   Text("Compute the order of the torsion subgroup of") + Name(1) );

    M.add( Ctor1<AbelInvariantsProb,SMFPGroup>(),
	   Text("Compute the canonical decomposition of") + Name(1) );

    M.add( Ctor1<AbelianPrimeDecomp,SMFPGroup>(),
	   Text("Compute the primary decomposition of") + Name(1) );

    M.add( Ctor1<AbelianTorsionFreeRankProblem, SMFPGroup>(),
	   Text("Compute the torsion-free rank of") + Name(1) );

    M.add( ReadHomologyItem1(),
	   Text("Compute an integral homology group of") + Name(1),
	   DefineInt("What dimension?", 2) );

    M.add( Ctor1<AGProblem,SMFPGroup>(),
	   Text("Compute a short-lex system of representatives for")
	   + Name(1) );

    M.closeCascade();
 
    M.startCascade( Text("Find ...") );

    M.add( Ctor1<PermutationRepProblem, SMFPGroup>(), 
	   Text("Find a permutation representation of") + Name(1)
	   + Text("when") + Name(1) + Text("is finite"));

    M.add( Ctor1<AbelianIsConfluent, SMFPGroup>(), 
	   Text("Find a finite rewriting system for") + Name(1));

    M.closeCascade();

    M.startCascade( Text("Enumerate ...") );
    
    M.add( Ctor1<WordEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate words of ") + Name(1));
    
    M.add( Ctor1<SGEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate subgroups of ") + Name(1));

    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );
    
    M.add( Ctor1<AutoEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate automorphisms of ") + Name( 1 ));
    
    M.add( Ctor1<FiniteAutoEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate automorphisms of ") + Name( 1 )
	   + Text("of finite order"));

    M.closeCascade();
 
    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::AP_FREE ||
		      CheckinType( 1 ) == SMFPGroup::AP_FREE_CYCLIC );

    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<APOfFreeIsAbelian, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("abelian ?") );

    M.add( Ctor1<APofFreeIsPerfect, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("perfect ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );
    
    M.add( Ctor1<APOfFreeIsFinite, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("finite ?") );

    M.add( Ctor1<AGProblem,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );
 
    M.add( Ctor1<APofFreeIsFree, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("free ?") );
 
    M.add( Ctor1<FPIsMSC,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("metric small cancellation ?") );
    M.closeCascade();

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<APOfFreeOrder,SMFPGroup>(),
	   Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );
    
    M.add( Ctor1<AbelInvariantsProb,SMFPGroup>(),
	   Text("Compute the canonical decomposition of") + Name(1)
	   + Text("abelianized") );
 
    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );
    
    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.addDisabled( Text("Compute Schreier representatives for") + Name(1) );
    
    M.add( ReadHomologyItem(),
	   Text("Compute an integral homology group of") + Name(1),
	   DefineInt("What dimension?", 2) );
    
    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.addDisabled( Text("Find an HNN-extension decomposition of") + Name(1) );

    M.separator();

    M.add( Ctor1<KBProblem,SMFPGroup>(),
	   Text("Find a finite rewriting system for") + Name(1) );

    M.separator();
    
    M.addDisabled( Text("Find an endomorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) );

    M.addDisabled( Text("Find an IA-automorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) 
		   + Text("of finite order"));

    M.closeCascade();


    M.startCascade( Text("Enumerate ...") );

    M.addDisabled( Text("Enumerate words of") + Name(1) );
   
    M.addDisabled( Text("Enumerate subgroups of") + Name(1) );

    M.addDisabled( Text("Enumerate relators of") + Name(1) );
  
    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );
    
    M.addDisabled( Text("Enumerate automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1)
 		   + Text("of finite order") );

    M.closeCascade();
 

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FP );


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<IsTrivialProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("trivial ?")
	   );

    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );
    
    M.add( Ctor1<IsAbelianProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("abelian ?")
	   );

    M.add( ReadIsNilpotentProblemItem(),
	   Text("Is") + Name(1) + Text("nilpotent ?"),
	   DefineInt("Enter the nilpotency class: ", 1)
	   );
    
    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );
    
    M.add( Ctor1<IsFiniteProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("finite ?") );

    M.add( Ctor1<AGProblem,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.add( Ctor1<FPIsMSC,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("metric small cancellation ?")
	   );

    M.closeCascade();


    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<OrderProblem,SMFPGroup>(),
	   Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.add( Ctor1<AbelInvariantsProb,SMFPGroup>(),
	   Text("Compute the canonical decomposition of") + Name(1)
	   + Text("abelianized") );
    
    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );
    
    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.add( Ctor1<SchreierTransversal,SMFPGroup>(),
	   Text("Compute Schreier representatives for") + Name(1) );

    
    M.add( ReadHomologyItem(),
	   Text("Compute an integral homology group of") + Name(1),
	   DefineInt("What dimension?", 2) );
    M.closeCascade();

  
    M.startCascade( Text("Find ...") );

    M.add( Ctor1<PermutationRepProblem, SMFPGroup>(),
	   Text("Find a permutation representation of") + Name(1)
	   + Text("when") + Name(1) + Text("is finite") );
    
    M.separator();

    M.add( Ctor1<KBProblem,SMFPGroup>(),
	   Text("Find a finite rewriting system for") + Name(1) );

    M.add( Ctor1<PreAbelianRepProblem, SMFPGroup>(),
	   Text("Find a pre-Abelian presentation for") + Name(1)
	   );

    M.separator();
   
    M.addDisabled( Text("Find an endomorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) );

    M.addDisabled( Text("Find an IA-automorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) 
		   + Text("of finite order"));

    M.closeCascade();


    M.startCascade( Text("Enumerate ...") );
    
    M.add( Ctor1<WordEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate words of ") + Name( 1 )
	   );
    
    M.add( Ctor1<SGEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate subgroups of ") + Name( 1 )
	   );

    M.add( Ctor1<NCRelatorEnumerator, SMFPGroup>(), 
	   Text("Enumerate relators of ") + Name( 1 )
	   );
    
    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1)
		   + Text("of finite order"));

    M.closeCascade();
  
    /*@db buggy!
    M.startCascade( Text("Run ...") );
    
    M.add( Ctor1<ACEProblem,SMFPGroup>(),
	   Text("Run Todd-Coxeter for") + Name( 1 )
	   );
    
    M.closeCascade();
    */

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FREE );

    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<FreeIsAutomatic, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );

    M.add( Ctor1<FreeIsHyperbolic, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.closeCascade();
    
    M.startCascade( Text("Compute ...") );
    
    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1)
		   + Text("abelianized") );

    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );
    
    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.addDisabled( Text("Compute Schreier representatives for") + Name(1) );
    
    M.addDisabled( Text("Compute an integral homology group of") + Name(1) );
    
    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.addDisabled( Text("Find a finite rewriting system for") + Name(1) );
    
    M.add( ReadFreeGetN_thElementItem(),
	   Text("Find the n-th element of") + Name(1),
	   DefineInt("Enter the number n: ", 0) );

    M.closeCascade();

    M.startCascade( Text("Enumerate ...") );

    M.add( Ctor1<WordEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate words of ") + Name( 1 ) );
    
    M.add( Ctor1<SGEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate subgroups of ") + Name( 1 )
	   );
    
    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );
    
    M.add( Ctor1<AutoEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate automorphisms of ") + Name( 1 )
	   );
    
    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );
    
    M.add( Ctor1<FiniteAutoEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate automorphisms of ") + Name( 1 )
	   + Text("of finite order")
	   );

    M.closeCascade();
    
 
    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FREE_NILPOTENT );
    

    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.addDisabled( Text("Is") + Name(1) + Text("abelian ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.closeCascade();


    M.startCascade( Text("Compute ...") );

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the torsion subgroup of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("abelianized") );
    M.addDisabled( Text("Compute the class of") + Name(1) );

    M.addDisabled( Text("Compute the derived length of") + Name(1) );

    M.add( ReadLCStermProblem(),
	   Text("Compute a term of the lower central series of") + Name(1),
	   DefineInt("Enter the number of the term ", 1) );
    
    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.add( Ctor1<NGHirschNumberProblem, SMFPGroup>(), 
	   Text("Compute the Hirsch number of") + Name(1) );
    
    M.add( Ctor1<NGcomputeBasisProblem, SMFPGroup>(),
	   Text("Compute a basis of ") + Name(1) );

    M.addDisabled( Text("Compute a short-lex system of representatives for")
		   + Name(1) );
  
    M.addDisabled( Text("Compute an integral homology group of") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Find ...") );
    
    M.add( Ctor1<NGbuildPresentationProblem, SMFPGroup>(),
	   Text("Find a presentation for ") + Name(1) );

    M.addDisabled( Text("Find a polycyclic presentation of") + Name(1) );

    M.addDisabled( Text("Find a permutation presentation of") + Name(1) );
	
    M.separator();
    
    M.addDisabled( Text("Find a finite rewriting system for") + Name(1) );
    
    M.closeCascade();
 
    M.startCascade( Text("Enumerate ...") );

    M.addDisabled( Text("Enumerate words of") + Name(1) );
   
    M.addDisabled( Text("Enumerate subgroups of") + Name(1) );

    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );
    
    M.addDisabled( Text("Enumerate automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1)
 		   + Text("of finite order") );

    M.closeCascade();
 

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::HNN_FREE );


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<HNNofFreeGroup_IsAbelian, SMFPGroup>(), 
	   Text("Is") + Name(1) + Text("abelian ?")
	   );

    M.addDisabled( Text("Is") + Name(1) + Text("nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );
    
    M.add( Ctor1<HNNofFreeGroup_IsFinite, SMFPGroup>(), 
	   Text("Is") + Name(1) + Text("finite ?") );

    M.add( Ctor1<AGProblem,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.add( Ctor1<HNNofFreeIsFree, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("free ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("metric small cancellation ?"));

    M.closeCascade();


    M.startCascade( Text("Compute ...") );

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );
    
    M.add( Ctor1<AbelInvariantsProb,SMFPGroup>(),
	   Text("Compute the canonical decomposition of") + Name(1)
	   + Text("abelianized") );
 
    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );
    
    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.addDisabled( Text("Compute Schreier representatives for") + Name(1) );
    
    M.add( ReadHomologyItem(),
	   Text("Compute an integral homology group of") + Name(1),
	   DefineInt("What dimension?", 2) );
    
    M.closeCascade();


    M.startCascade( Text("Find ...") );


    M.addDisabled( Text("Find an amalgamated product decomposition of") 
		   + Name(1) );

    M.addDisabled( Text("Find an HNN-extension decomposition of") + Name(1) );

    M.addDisabled( Text("Find a permutation representation of") + Name(1) 
		   + Text("when") + Name(1) + Text("is finite") );

    M.separator();

    M.add( Ctor1<KBProblem,SMFPGroup>(),
	   Text("Find a finite rewriting system for") + Name(1)
	   );

    M.separator();
    
    M.addDisabled( Text("Find an endomorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) );

    M.addDisabled( Text("Find an IA-automorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) 
		   + Text("of finite order"));

    M.closeCascade();


    M.startCascade( Text("Enumerate ...") );

    M.addDisabled( Text("Enumerate words of") + Name(1) );
   
    M.addDisabled( Text("Enumerate subgroups of") + Name(1) );

    M.addDisabled( Text("Enumerate relators of") + Name(1) );
  
    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );
    
    M.addDisabled( Text("Enumerate automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1)
 		   + Text("of finite order") );

    M.closeCascade();
 

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::NILPOTENT );
    

    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<IsTrivialProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("trivial ?")
	   );

    M.add( Ctor1<IsFiniteProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("finite ?")
	   );

    M.add( Ctor1<IsAbelianProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("abelian ?")
	   );
   
    M.add( Ctor1<NGisFreeNilpotentProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("free nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.closeCascade();


    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<OrderProblem,SMFPGroup>(),
	   Text("Compute the order of") + Name(1)
	   );

    M.addDisabled( Text("Compute the center of") + Name(1) );
    
    M.addDisabled( Text("Compute the torsion subgroup of") + Name(1) );

    M.add( Ctor1<AbelInvariantsProb,SMFPGroup>(),
	   Text("Compute the canonical decomposition of") + Name(1)
	   + Text("abelianized") );

    M.add( Ctor1<NGcomputeNClassProblem, SMFPGroup>(),
	   Text("Compute the class of") + Name(1) );
    
    M.addDisabled( Text("Compute the derived length of") + Name(1) );

    M.add( ReadLCStermProblem(),
	   Text("Find a term of the lower central series of") + Name(1),
	   DefineInt("Enter the number of the term ", 1)
	   );
    
    M.add( Ctor1<NGcomputeBasisProblem,SMFPGroup>(),
	   Text("Compute lower central quotients of") + Name(1)
	   );

    M.addDisabled( Text("Compute the derived series of") + Name(1) );


    M.addDisabled( Text("Compute quotients of the derived series of")
			+ Name(1) );

    M.add( Ctor1<NGHirschNumberProblem,SMFPGroup>(),
	   Text("Compute the Hirsch number of") + Name(1)
	   );

    M.add( Ctor1<NGcomputeBasisProblem,SMFPGroup>(),
	   Text("Compute a basis of ")
	   + Name(1)
	   );

    M.addDisabled( Text("Compute a short-lex system of representatives for")
			+ Name(1) );
  
    M.addDisabled( Text("Compute an integral homology group of") + Name(1) );

    M.closeCascade();


    M.startCascade( Text("Find ...") );

    M.add( Ctor1<NGbuildPresentationProblem, SMFPGroup>(),
	   Text("Find a polycyclic presentation of") + Name(1)
	   );

    M.separator();
 
    M.addDisabled( Text("Find a finite rewriting system for") + Name(1) );
 
    M.addDisabled( Text("Find a permutation presentation of") + Name(1) );
	
    M.closeCascade();
 

    M.startCascade( Text("Enumerate ...") );

    M.addDisabled( Text("Enumerate words of") + Name(1) );
   
    M.addDisabled( Text("Enumerate subgroups of") + Name(1) );

    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );
    
    M.addDisabled( Text("Enumerate automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1)
 		   + Text("of finite order") );

    M.closeCascade();


    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::ONE_RELATOR ||
		      CheckinType( 1 ) == SMFPGroup::ONE_RELATOR_WITH_TORSION
		      );


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<ORIsTrivial, SMFPGroup>(), 
	   Text("Is") + Name(1) + Text("trivial ?")
	   );

    M.add( Ctor1<ORIsAbelian, SMFPGroup>(), 
	   Text("Is") + Name(1) + Text("abelian ?")
	   );

    M.addDisabled( Text("Is") + Name(1) + Text("nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.add( Ctor1<ORIsFinite, SMFPGroup>(), 
	   Text("Is") + Name(1) + Text("finite ?") );
    
    M.add( Ctor1<AGProblem,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.add( Ctor1<ORIsFreeProblem, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("free ?") );

    M.add( Ctor1<FPIsMSC,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("metric small cancellation ?") );

    M.closeCascade();


    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<OROrder,SMFPGroup>(),
	   Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.add( Ctor1<AbelInvariantsProb,SMFPGroup>(),
	   Text("Compute the canonical decomposition of") + Name(1)
	   + Text("abelianized") );
    
    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );
    
    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.addDisabled( Text("Compute Schreier representatives for") + Name(1) );

    M.add( Ctor1<FastHomology,SMFPGroup>(),
	   Text("Compute the integral homology of") + Name(1) );
 
    M.closeCascade();

  
    M.startCascade( Text("Find ...") );

    M.addDisabled( Text("Find an amalgamated product decomposition of") 
		   + Name(1) );

    M.add( ReadSMMagnusBreakdown(),
	   Text("Find an HNN-extension decomposition of") + Name(1) );
    
    M.addDisabled( Text("Find a permutation representation of") + Name(1) 
		   + Text("when") + Name(1) + Text("is finite") );
  
    M.separator();

    M.add( Ctor1<KBProblem,SMFPGroup>(),
	   Text("Find a finite rewriting system for") + Name(1) );

    M.separator();
   
    M.addDisabled( Text("Find an endomorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) );

    M.addDisabled( Text("Find an IA-automorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) 
		   + Text("of finite order") );

    M.closeCascade();
  

    M.startCascade( Text("Enumerate ...") );
    
    M.add( Ctor1<WordEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate words of ") + Name( 1 ) );
    
    M.add( Ctor1<SGEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate subgroups of ") + Name( 1 ) );

    M.add( Ctor1<NCRelatorEnumerator, SMFPGroup>(), 
	   Text("Enumerate relators of ") + Name( 1 ) );

    M.add( Ctor1<ORConsequencesEnumerator,SMFPGroup>(),
	   Text("Enumerate consequences of the relator of ") + 
	   Name( 1 ) );
    
    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1)
		   + Text("of finite order") );

    M.closeCascade();
  

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::SMALL_CANCELLATION );


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<MSCIsTrivial, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("trivial ?") );
 
    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );
 
    M.add( Ctor1<MSCIsAbelian, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("abelian ?") );

    M.add( ReadIsNilpotentProblemItem(),
	   Text("Is") + Name(1) + Text("nilpotent ?"),
	   DefineInt("Enter the nilpotency class: ", 1) );
    
    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.add( Ctor1<MSCIsFinite, SMFPGroup>(),
	   Text("Is") + Name(1) + Text("finite ?") );

    M.add( Ctor1<AGProblem,SMFPGroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.closeCascade();


    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<MSCOrder,SMFPGroup>(),
	   Text("Compute the order of") + Name(1) );
  
    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.add( Ctor1<AbelInvariantsProb,SMFPGroup>(),
	   Text("Compute the canonical decomposition of") + Name(1)
	   + Text("abelianized") );
    
    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );
    
    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.add( Ctor1<SchreierTransversal,SMFPGroup>(),
	   Text("Compute Schreier representatives for") + Name(1)
	   );
    
    M.add( ReadHomologyItem(),
	   Text("Compute an integral homology group of") + Name(1),
	   DefineInt("What dimension?", 2)
	   );
    
    M.closeCascade();

  
    M.startCascade( Text("Find ...") );

    M.add( Ctor1<PermutationRepProblem, SMFPGroup>(),
	   Text("Find a permutation representation of") + Name(1) );
    
    M.separator();

    M.add( Ctor1<KBProblem,SMFPGroup>(),
	   Text("Find a finite rewriting system for") + Name(1) );

    M.separator();
   
    M.addDisabled( Text("Find an endomorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) );

    M.addDisabled( Text("Find an IA-automorphism of") + Name(1) );

    M.addDisabled( Text("Find an automorphism of") + Name(1) 
		   + Text("of finite order") );

    M.closeCascade();


    M.startCascade( Text("Enumerate ...") );
    
    M.add( Ctor1<WordEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate words of ") + Name( 1 ) );
    
    M.add( Ctor1<SGEnumeratorProblem, SMFPGroup>(), 
	   Text("Enumerate subgroups of ") + Name( 1 ) );

    M.add( Ctor1<NCRelatorEnumerator, SMFPGroup>(), 
	   Text("Enumerate relators of ") + Name( 1 ) );
    
    M.addDisabled( Text("Enumerate endomorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate IA-automorphisms of") + Name(1) );

    M.addDisabled( Text("Enumerate automorphisms of") + Name(1)
		   + Text("of finite order") );

    M.closeCascade();

    M.done();
  }

  {
    Menu1<SMWord> M( Menu::TOOLS );

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::ABELIAN );

    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor1<FastAbelianForm, SMWord>(), Text("Rewrite") + Name(1) 
	   + Text("in reduced form as an element of a free abelian group") );

    M.add( Ctor1<FastInverseInAbelianForm, SMWord>(),
	   Text("Rewrite the inverse of") + Name(1) 
	   + Text("in reduced form as an element of a free abelian group") );

    M.separator();
    
    M.add( ReadNthPower(),
	   Text("Rewrite") + Name(1) + Text("as an n-th power"),
	   DefineInt("n = ", 2) );

    M.closeCascade();
    
    M.startCascade( Text("Compute ...") );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the reduced form of") 
	   + Name(1) + Text("as an element of a free abelian group") );

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the reduced form of") 
	   + Name(1) + Text("as an element of a free abelian group"),
	   DefineInt("Enter length of the initial segment: ", 1) );

    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the reduced form of") 
	   + Name(1) + Text("as an element of a free abelian group"),
	   DefineInt("Enter length of the terminal segment: ", 1) );
    
    M.separator();

    M.add( Ctor1<WordRepresentativeProblem, SMWord>(),
	   Text("Compute the representative of") 
	   + Name(1) + Text("in a Schreier system of representatives for") 
	   + Name( Parent(1) ) );

    M.separator();
    
    M.add( Ctor1<AbelianWordProblem,SMWord>(),
	   Text("Compute the canonical decomposition of") + Name(1) );
    
    M.add( Ctor1<EltPrimeForm, SMWord>(),
	   Text("Compute the primary decomposition of ") + Name(1) );

    M.separator();

    M.add( Ctor1<AbelianOrderOfEltProblem, SMWord>(),
	   Text("Compute the order of ") + Name(1) );
    
    M.add( ReadPHeight(),
	   Text("Compute the p-height of ") + Name(1),
	   DefineInt("Enter the p  ", 2) );
    
    M.add( Ctor1<AbelianMaximalRoot, SMWord>(),
	   Text("Compute the maximal root of ") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<WordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?");
    
    M.add( Ctor1<AbelianPowerProblem, SMWord>(),
	   Text("Is ") + Name(1) + Text("a proper power ?") );
    
    M.add( Ctor1<AbelianSGGenedByWordPurityProblem, SMWord>(),
	   Text("Is the subgroup generated by")
	   + Name(1) + Text("pure ?") );
    
    M.closeCascade();
        
    M.startCascade( Text("Does") + Name(1) + Text("...") );
    
    M.add( Ctor1<AbelianDoesGensSummand, SMWord>(),
	   Text("Does ") + Name(1) + Text("generate a direct summand ?") );
    
    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::AP_FREE_CYCLIC );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1) );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1) );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1) );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1));

    M.addDisabled( Text("Compute the amalgamated product length of") + Name(1));

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of")
	   + Name(1), DefineInt("Enter length of the initial segment: ", 1) );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") 
	   + Name(1), DefineInt("Enter length of the terminal segment: ", 1) );

    M.separator();

    M.addDisabled( Text("Compute the representative of") + Name(1) 
		   + Text("in a Schreier system of representatives for")
		   + Name( Parent(1) )  );

    M.separator();
    
    M.add( Ctor1<APOfFreeReducedForm,SMWord>(),
	   Text("Compute the reduced form of") + Name(1) );

    M.add( Ctor1<APOfFreeNormalForm,SMWord>(),
	   Text("Compute the normal form of") + Name(1) );

    M.add( Ctor1<APOfFreeCyclicNormalForm,SMWord>(),
	   Text("Compute the cyclic normal form of") + Name(1) );

    M.add( Ctor1<APOfFreeCyclic_MaximalRoot, SMWord>(),
	   Text("Compute the maximal root of") + Name(1) );

    M.separator();

    M.add( Ctor1<APOfFreeCyclic_CentralizerOfElt, SMWord>(),
	   Text("Compute the centralizer of") + Name(1) );
	 
    M.addDisabled( Text("Compute the normalizer of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) ) );

    M.closeCascade();

    M.startCascade( Text("Rewrite ...") );
    
    M.add( Ctor1<SquareRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of squares") );
    
    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group") );
 
    M.add( Ctor1<CommutatorRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of commutators") );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators") );

    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a strictly alternating product") );
		   
    M.add( Ctor1<APOfFreeNormalForm,SMWord>(),
	   Text("Rewrite") + Name(1)
	   + Text("in the normal form for amalgamated products") );
    
    M.closeCascade();
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor1<APOfFreeWordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?" );

    M.add( Ctor1<APOfFreeCyclic_IsEltAProperPower, SMWord>(),
	   Text("Is") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + "a commutator ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::AP_FREE );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1) );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1) );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") +Name(1));
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1));

    M.addDisabled( Text("Compute the amalgamated product length of") +Name(1));

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1) );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1) );

    M.separator();

    M.addDisabled( Text("Compute the representative of") + Name(1) 
		   + Text("in a Schreier system of representatives for")
		   + Name( Parent(1) ) );

    M.separator();
    
    M.add( Ctor1<APOfFreeReducedForm,SMWord>(),
	   Text("Compute the reduced form of") + Name(1) );

    M.add( Ctor1<APOfFreeNormalForm,SMWord>(),
	   Text("Compute the normal form of") + Name(1) );

    M.add( Ctor1<APOfFreeCyclicNormalForm,SMWord>(),
	   Text("Compute the cyclic normal form of") + Name(1) );

    M.addDisabled( Text("Compute the maximal root of") + Name(1) );

    M.separator();

    M.addDisabled( Text("Compute the centralizer of") + Name(1) );
	 
    M.addDisabled( Text("Compute the normalizer of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) ) );

    M.closeCascade();


    M.startCascade( Text("Rewrite ...") );
    
    M.addDisabled( Text("Rewrite") + Name(1)
		   + Text("as a product of squares") );
    
    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group") );
 
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of commutators") );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators") );

    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a strictly alternating product") );
		   
    M.add( Ctor1<APOfFreeNormalForm,SMWord>(),
	   Text("Rewrite") + Name(1)
	   + Text("in the normal form for amalgamated products") );
    
    M.closeCascade();
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor1<APOfFreeWordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?" );

    M.addDisabled( Text("Is") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + "a commutator ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::HNN_FREE );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1) );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1) );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1) );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1));

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1) );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1) );

    M.separator();

    M.addDisabled( Text("Compute the representative of") + Name(1) 
		   + Text("in a Schreier system of representatives for")
		   + Name( Parent(1) ) );

    M.separator();
    
    M.addDisabled( Text("Compute the centralizer of") + Name(1) );
	 
    M.addDisabled( Text("Compute the normalizer of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) ) );

    M.closeCascade();

    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor1<SquareRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of squares") );
    
    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group") );

    M.add( Ctor1<CommutatorRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of commutators") );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators")  );

    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product without pinches") );

    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("in HNN-extension normal form") );
		   
    M.closeCascade();
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<HNNofFreeGroup_WordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?" );
    
    M.addDisabled( Text("Is") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + "a commutator ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FP );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1) );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1) );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1) );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1));

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1)  );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1) );

    M.separator();

    M.add( Ctor1<WordRepresentativeProblem, SMWord>(),
	   Text("Compute the representative of") + Name(1) 
	   + Text("in a Schreier system of representatives for")
	   + Name( Parent(1) ) );
    
    M.separator();
    
    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.separator();

    M.addDisabled( Text("Compute the centralizer of") + Name(1) );
	 
    M.addDisabled( Text("Compute the normalizer of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) ) );

    M.closeCascade();

    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor1<SquareRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of squares") );
    
    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group") );

    M.add( Ctor1<CommutatorRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of commutators") );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators") );
	   
    M.closeCascade();
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<WordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?" );

    M.add( Ctor1<IsEltCentral,SMWord>(),
	   Text("Is") + Name(1) + Text("central in")
	   + Name( Parent(1) ) + Text("?") );
    
    M.add( Ctor1<OrderOfElt, SMWord>(),
	   Text("Is") + Name(1) + Text("of finite order in")
	   + Name( Parent(1) ) + Text("?") );

    M.addDisabled( Text("Is") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + "a commutator ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1) );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1) );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1) );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1));

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1) );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1) );

    M.separator();

    M.add( Ctor1<FreeCentolizerOfElement, SMWord>(),
	   Text("Compute the centralizer of") + Name(1) + "in"
	   + Name( Parent(1) ) );
	 
    M.addDisabled( Text("Compute the normalizer of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) ) );
    
    M.separator();
 
    M.add( Ctor1<RankOfElementProblem,SMWord>(),
	   Text("Compute a rank of ") + Name(1) + " in " 
	   + Name( Parent(1) ) + "?" );
    
    M.add( Ctor1<FreeMaximalRootOfElement,SMWord>(),
	   Text("Compute the maximal root of") + Name(1)
	   + Text("in") + Name( Parent(1) ) );
        
    M.closeCascade();

    M.startCascade( Text("Rewrite ...") );
  
    M.add( Ctor1<ProductOfSquares,SMWord>(),
	   Text("Rewrite") + Name(1) + "as a product of squares" 
	   + Name( Parent(1) ) );

    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group") );

    M.add( Ctor1<ProductOfCommutators,SMWord>(),
	   Text("Rewrite") + Name(1) + "as a product of commutators" 
	   + Name( Parent(1) ) );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators") );
	   
    M.closeCascade();
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor1<WordProblemInFree,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?" );

    M.add( Ctor1<FreeIsElementAProperPower,SMWord>(),
	   Text("Is") + Name(1) + Text("a proper power ?") );
    
    M.add( Ctor1<CommutatorInFree,SMWord>(),
	   Text("Is") + Name(1) + "a commutator ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.add( Ctor1<TwoCommProblem,SMWord>(),
	   Text("Is") + Name(1) + "a product of two commutators ?" );

    M.add( Ctor1<IsWordAPEProblem,SMWord>(),
	   Text("Is ") + Name(1) + "almost primitive ?" );

    M.add( Ctor1<FreeIsPartOfBasisProblem,SMWord>(),
	   Text("Is") + Name(1) + "part of a free basis for" +
	   Name( Parent(1) ) + "?" );
    
    M.add( Ctor1<FreeInCommutatorSG,SMWord>(),
	   Text("Is") + Name(1) + "in the commutator subgroup of"
	   + Name( Parent(1) ) + Text("?") );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE_NILPOTENT );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1) );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1) );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1) );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1));

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1) );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1) );

    M.separator();

    M.add( Ctor1<NGcentralizer, SMWord>(),
	   Text("Compute the centralizer of ") + Name(1) );

    M.addDisabled( Text("Compute the normalizer of ") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) ) );

    M.separator();

    M.add( Ctor1<NGMaximalRootProblem, SMWord>(),
	   Text("Compute the maximal root of ") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Rewrite ...") );
  
    M.addDisabled( Text("Rewrite") + Name(1) + "as a product of squares" );

    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group") );

    M.addDisabled( Text("Rewrite") + Name(1) + "as a product of commutators" );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators") );
    
    M.separator();
   
    M.add( Ctor1<NGdecomposeWordProblem,SMWord>(),
	   Text("Rewrite") + Name(1)
	   + Text("as a product of basic commutators") );
    
    M.closeCascade();
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<WordProblem, SMWord>(),
	   Text("Is") + Name(1) + Text("trivial in")
	   + Name( Parent(1) ) + Text("?") );

    M.add( Ctor1<NGIsProperPower, SMWord>(),
	   Text("Is ") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("a commutator ?") );	

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.addDisabled( Text("Is") + Name(1) + "part of a free basis for" +
		   Name( Parent(1) ) + "?" );
    
    M.separator();
    
    M.add( Ctor1<NGisCentralProblem, SMWord>(),
	   Text("Is") + Name(1) + Text("central ?") );
    
    M.add( Ctor1<NGisWordInCommutatorSGProblem, SMWord>(),
	   Text("Is") + Name(1) 
	   + Text("an element of the commutator subgroup of")
	   + Name( Parent(1) ) + Text("?") );	 

    M.closeCascade();
       
    M.startCascade( Text("Find ...") );
    
    M.add( Ctor1<NGweightOfWordProblem,SMWord>(),
	   Text("Find the term of the lower central series in which") 
	   + Name(1) + Text("lies") );
    
    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::NILPOTENT );
    
    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1) );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1) );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1) );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1));

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1) );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1) );

    M.separator();

    M.addDisabled( Text("Compute the centralizer of") + Name(1) );

    M.addDisabled( Text("Compute the normalizer of ") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) ) );

    M.separator();

    M.add( Ctor1<NGorderOfEltProblem, SMWord>(),
	   Text("Compute the order of ") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Rewrite ...") );
  
    M.addDisabled( Text("Rewrite") + Name(1) + "as a product of squares" );

    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group") );

    M.addDisabled( Text("Rewrite") + Name(1) + "as a product of commutators" );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of basic commutators") );
    
    M.add( Ctor1<NGdecomposeWordProblem,SMWord>(),
	   Text("Rewrite") + Name(1)
	   + Text("in canonical form as a product of elements in a")
	   + Text("Malcev basis for") + Name( Parent(1) )
	   );

    M.closeCascade();

    
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<WordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?");

    M.addDisabled( Text("Is ") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("a commutator ?") );	

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.addDisabled( Text("Is") + Name(1) + "part of a free basis for" +
		   Name( Parent(1) ) + "?"
		   );

    M.separator();

    M.add( Ctor1<NGisCentralProblem, SMWord>(),
	   Text("Is") + Name(1) + Text("central ?")
	   );
    
    M.add( Ctor1<NGisWordInCommutatorSGProblem, SMWord>(),
	   Text("Is") + Name(1) + Text("an element of the commutator subgroup of")
	   + Name( Parent(1) ) + Text("?")
	   );	

 
    M.closeCascade();
      

    M.startCascade( Text("Find ...") );
    
    M.add( Ctor1<NGweightOfWordProblem,SMWord>(),
	   Text("Find the term of the lower central series in which") + Name(1)
	   + Text("lies")
	   );
    
    M.closeCascade();

    //@@@@@@

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::ONE_RELATOR );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1)
	   );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1)
	   );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1)
	   );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1)
	   );

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of")
	   + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1)
	   );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") 
	   + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1)
	   );

    M.separator();

    M.add( Ctor1<WordRepresentativeProblem, SMWord>(),
	   Text("Compute the representative of") + Name(1) 
	   + Text("in a Schreier system of representatives for")
	   + Name( Parent(1) )
	   );
    
    M.separator();
    
    M.add( Ctor1<OrderOfElt, SMWord>(),
	   Text("Compute the order of") + Name(1)
	   );

    M.separator();

    M.addDisabled( Text("Compute the centralizer of") + Name(1) );
	 
    M.addDisabled( Text("Compute the normalizer of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) )
		   );

    M.closeCascade();


    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor1<SquareRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of squares")
	   );
    
    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group")
		   );
    M.add( Ctor1<CommutatorRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of commutators")
	   );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators")
		   );

    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("in HNN-extension normal form")
		   );
	   
    M.closeCascade();

    
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<WordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?"
	   );

    M.add( Ctor1<IsEltCentral,SMWord>(),
	   Text("Is") + Name(1) + Text("central in")
	   + Name( Parent(1) ) + Text("?")
	   );

    M.addDisabled( Text("Is") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + "a commutator ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.closeCascade();


    M.startItemGroup( CheckinType( Parent(1) ) == 
		      SMFPGroup::ONE_RELATOR_WITH_TORSION );

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<FreelyReduceWord,SMWord>(),
	   Text("Compute the freely reduced form of") + Name(1)
	   );
    
    M.add( Ctor1<CyclicallyReduceWord,SMWord>(),
	   Text("Compute a cyclically reduced form of") + Name(1)
	   );

    M.add( Ctor1<FormalInverseOfWord,SMWord>(),
	   Text("Compute the freely reduced form of the inverse of") 
	   + Name(1)
	   );
    
    M.add( Ctor1<WordLength,SMWord>(),
	   Text("Compute the length of the freely reduced form of") + Name(1)
	   );

    M.add( ReadInitialSegmentItem(),
	   Text("Compute the i-th initial segment of the freely reduced form of")
	   + Name(1),
	   DefineInt("Enter length of the initial segment: ", 1)
	   );
    
    M.add( ReadTerminalSegmentItem(),
	   Text("Compute the j-th terminal segment of the freely reduced form of") 
	   + Name(1),
	   DefineInt("Enter length of the terminal segment: ", 1)
	   );

    M.separator();

    M.add( Ctor1<WordRepresentativeProblem, SMWord>(),
	   Text("Compute the representative of") + Name(1) 
	   + Text("in a Schreier system of representatives for")
	   + Name( Parent(1) )
	   );
    
    M.separator();
    
    M.add( Ctor1<ORWithTorsionEltFiniteOrder, SMWord>(),
	   Text("Compute the order of") + Name(1)
	   );

    M.separator();

    M.add( Ctor1<ORWithTorsionCentralizerOfElt, SMWord>(),
	   Text("Compute the centralizer of") + Name(1)
	   );
 
    M.addDisabled( Text("Compute the normalizer of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("modulo the derived group of") + Name( Parent(1) )
		   );

    M.closeCascade();


    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor1<SquareRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of squares")
	   );
    
    M.addDisabled( Text("Rewrite") + Name(1) + Text("as a product of its "
       "canonical form modulo the derived group and an element "
       "of the derived group")
		   );
    M.add( Ctor1<CommutatorRewriteProblem,SMWord>(),
	   Text("Rewrite") + Name(1) + Text("as a product of commutators")
	   );
    
    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("as a product of n-fold commutators")
		   );

    M.addDisabled( Text("Rewrite") + Name(1) 
		   + Text("in HNN-extension normal form")
		   );
	   
    M.closeCascade();

    
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor1<WordProblem,SMWord>(),
	   Text("Is") + Name(1) + "trivial ?"
	   );

    M.add( Ctor1<IsEltCentral,SMWord>(),
	   Text("Is") + Name(1) + Text("central in")
	   + Name( Parent(1) ) + Text("?")
	   );

    M.addDisabled( Text("Is") + Name(1) + Text("a proper power ?") );
    
    M.addDisabled( Text("Is") + Name(1) + "a commutator ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of squares ?" );

    M.addDisabled( Text("Is") + Name(1) + "a product of commutators ?" );

    M.closeCascade();

    M.done();
  }

  {
    Menu1<SMSubgroup> M( Menu::TOOLS );

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::ABELIAN );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor1<AbelianIsSGEqualToTheGroup, SMSubgroup>(),
	   Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.add( Ctor1<AbelianSGIndexProblem, SMSubgroup>(),
	   Text("Is") + Name(1) + "of finite index in" 
	   + Name( Parent(1) ) + "?" );

    M.add( Ctor1<AbelianSGPurityProblem, SMSubgroup>(),
	   Text("Is") + Name(1) + "pure in" + Name( Parent(1) ) + "?" );

    M.add( Ctor1<AbelianIsSGIsolatedProblem, SMSubgroup>(),
	   Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant in" 
		   + Name( Parent(1) ) + "?" );

    M.separator();

    M.add( Ctor1<IsSGTrivial, SMSubgroup>(),
	   Text("Is") + Name(1) + Text("trivial ?") );

    M.add( Ctor1<AbelianIsSGFinite, SMSubgroup>(),
	   Text("Is") + Name(1) + Text("finite ?") );
 
    M.add( Ctor1<AbelianIsSGFreeAbelian, SMSubgroup>(), 
	   Text("Is") + Name(1) + Text("free abelian ?") );

    M.add( Ctor1<AbelianIsAutomatic, SMSubgroup>(),
	   Text("Is") + Name(1) + Text("automatic ?") );

    M.add( Ctor1<AbelianIsSGHyperbolic, SMSubgroup>(), 
	   Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.closeCascade();

    M.startCascade( Text("Compute ...") );
    
    M.add( Ctor1<AbelianSGIndexProblem, SMSubgroup>(),
	   Text("Compute the index of") + Name(1) + Text("in")
	   + Name( Parent(1) ) );

    M.add( Ctor1<SchreierTransversal, SMSubgroup>(),
	   Text("Compute a Schreier transversal of") + Name(1)
	   + Text("in") + Name( Parent(1) ) );

    M.add( Ctor1<SubgroupIsolator, SMSubgroup>(),
	   Text("Compute the isolator of") + Name(1)
	   + Text("in") + Name( Parent(1) ) );

    M.add( Ctor1<virtualFreeComp, SMSubgroup>(),
	   Text("Compute a virtual free complement of") + Name(1) 
	   + Text("in") + Name( Parent(1) ) );
    
    M.separator();

    M.add( Ctor1<AbelianSGOrder, SMSubgroup>(),
	   Text("Compute the order of") + Name(1) );
 
    M.add( Ctor1<AbelianComputeTorsionSubgroupOfSG, SMSubgroup>(),
	   Text("Compute the torsion subgroup of") + Name(1) );
    
    M.add( Ctor1<AbelianOrderOfTheTorsionSubgroupOfSG, SMSubgroup>(),
           Text("Compute the order of the torsion subgroup of") + Name(1) );

    M.add( Ctor1<AbelianSGCyclicDecomposition, SMSubgroup>(),
	   Text("Compute the canonical decomposition of") + Name(1) );

    M.add( Ctor1<AbelianSGPrimesDecomposition, SMSubgroup>(),
	   Text("Compute the primary decomposition of") + Name(1) );
    
    M.add( Ctor1<AbelianTorsionFreeRankOfSG, SMSubgroup>(),
           Text("Compute the torsion-free rank of") + Name(1) );

    M.add( Ctor1<AbelianSGMinGens, SMSubgroup>(),
	   Text("Compute the minimal number of generators of") + Name(1) );   
  
    M.addDisabled( Text("Compute an integral homology group of") + Name(1) );

    M.addDisabled( Text("Compute the first n terms for a short-lex system of "
			"representatives for the elements of") + Name(1) );
    
    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.add( Ctor1<PermutationRepProblem, SMSubgroup>(),
	   Text("Find a permutation representation of") + Name( Parent(1) ) 
	   + Text("modulo") + Name(1) + Text("when the index of ") 
	   + Name(1) + " in " + Name( Parent(1) ) + Text("is finite") );

    M.add( Ctor1<AbelianIsConfluent, SMSubgroup>(),
	   Text("Find a finite rewriting system for") + Name(1) );

    M.addDisabled( Text("Find a ShortLex automatic structure for") + Name(1) );

    M.closeCascade();
 
    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::AP_FREE ||
		      CheckinType( Parent(1) ) == SMFPGroup::AP_FREE_CYCLIC );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "of finite index in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) 
		   + "?" );

    M.addDisabled( Text("Is") + Name(1) + "central in" + Name( Parent(1) ) 
		   + "?" );
    
    M.add( Ctor1<IsSGNormal,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("normal in") + Name(Parent(1)) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "subnormal in" + Name( Parent(1) ) 
		   + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant in" 
		   + Name( Parent(1) ) + "?" );

    M.separator();
 
    M.add( Ctor1<APOfFreeIsSGTrivial,SMSubgroup>(),
	   Text("Is") + Name(1) + "trivial ?" );
 
    M.add( Ctor1<APOfFreeIsSGAbelian,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("abelian ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finite ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finitely related ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.closeCascade();


    M.startCascade( Text("Compute ...") );
    
    M.addDisabled( Text("Compute the index of") + Name(1) + Text("in")
	   + Name( Parent(1) ) );
    
    M.addDisabled( Text("Compute a Schreier transversal of") + Name(1)
		   + Text("in") + Name( Parent(1) ) );

    M.separator();

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1)
		   + Text("abelianized") );

    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );

    M.closeCascade();


    M.startCascade( Text("Find ...") );
    
    M.addDisabled( Text("Find a permutation representation of")
		   + Name( Parent(1) ) + Text("modulo") + Name(1) 
		   + Text("when the index of ") + Name(1) + " in " 
		   + Name( Parent(1) ) + Text("is finite") );

    M.separator();
    
    M.add( Ctor1<SGPresentationProblem,SMSubgroup>(),
	   Text("Find a presentation for") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Enumerate ...") );

    M.addDisabled( Text("Enumerate relators of") + Name(1) );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::HNN_FREE );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "of finite index in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) 
		   + "?" );

    M.addDisabled( Text("Is") + Name(1) + "central in" + Name( Parent(1) ) 
		   + "?" );
    
    M.add( Ctor1<IsSGNormal,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("normal in") + Name( Parent(1) ) 
	   + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "subnormal in" + Name( Parent(1) ) 
		   + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant in" 
		   + Name( Parent(1) ) + "?" );

    M.separator();
    
    M.add( Ctor1<HNNofFreeGroup_IsSGTrivial,SMSubgroup>(),
	   Text("Is") + Name(1) + " trivial ?" );
    
    M.add( Ctor1<HNNofFreeGroup_IsSGAbelian,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("abelian ?") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finite ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finitely related ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.closeCascade();

    M.startCascade( Text("Compute ...") );
    
    M.addDisabled( Text("Compute the index of") + Name(1) + Text("in")
		   + Name( Parent(1) ) );
    
    M.addDisabled( Text("Compute a Schreier transversal of") + Name(1)
		   + Text("in") + Name( Parent(1) )  );

    M.separator();

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1)
		   + Text("abelianized") );

    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Find ...") );
    
    M.addDisabled( Text("Find a permutation representation of")
		   + Name( Parent(1) ) + Text("modulo") + Name(1) 
		   + Text("when the index of ") + Name(1) + " in "
		   + Name( Parent(1) ) + Text("is finite")  );

    M.closeCascade();

    M.startCascade( Text("Enumerate ...") );

    M.addDisabled( Text("Enumerate relators of") + Name(1) );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FP );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "of finite index in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) 
		   + "?" );

    M.add( Ctor1<IsSGCentral,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("central ?") );
 
    M.add( Ctor1<IsSGNormal,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("normal ?") );
 
    M.addDisabled( Text("Is") + Name(1) + "subnormal ?");
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic ?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant ?" );

    M.separator();
    
    M.add( Ctor1<IsSGTrivial,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("trivial ?") );

    M.add( Ctor1<IsSGAbelian,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("abelian ?") );

    M.add( ReadIsSGNilpotentItem(),
	   Text("Is") + Name(1) + Text("nilpotent ?"),
	   DefineInt("Enter the nilpotency class: ", 1) );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finite ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finitely related ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.closeCascade();

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<SGIndexProblem, SMSubgroup>(),
	   Text("Compute the index of") + Name(1) + Text("in")
	   + Name( Parent(1) ) );

    M.add( Ctor1<SchreierTransversal, SMSubgroup>(),
	   Text("Compute a Schreier transversal of") + Name(1)
	   + Text("in") + Name( Parent(1) ) );

    M.separator();

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1)
		   + Text("abelianized"));

    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.add( Ctor1<PermutationRepProblem, SMSubgroup>(),
	   Text("Find a permutation representation of")
	   + Name( Parent(1) ) + Text("modulo") + Name(1) 
	   + Text("when the index of ") + Name(1) + " in " + Name( Parent(1) )
	   + Text("is finite") );

    M.separator();

    M.add( Ctor1<SGPresentationProblem,SMSubgroup>(),
	   Text("Find a presentation for") + Name(1) );
    
    M.closeCascade();


    M.startCascade( Text("Enumerate ...") );
 
    M.add( Ctor1<SGRelatorEnumerator,SMSubgroup>(),
	   Text("Enumerate relators of ") + Name( 1 ) );

    M.closeCascade();

    M.startCascade( Text("Run ...") );
 
    M.add( Ctor1<ACEProblem,SMSubgroup>(),
	   Text("Run Todd-Coxeter for") + Name( 1 )  );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "of finite index in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) 
		   + "?" );

    M.addDisabled( Text("Is") + Name(1) + Text("central ?") );

    M.add( Ctor1<SGOfFreeIsNormal,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("normal ?") );
 
    M.addDisabled( Text("Is") + Name(1) + "subnormal ?");
 
    M.add( Ctor1<SGOfFreeIsMalnormal,SMSubgroup>(),
	   Text("Is") + Name(1) + "malnormal ?" );
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic ?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant ?" );

    M.separator();

    M.add( Ctor1<FreeIsSGTrivial,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("trivial ?") );

    M.addDisabled( Text("Is") + Name(1) + " abelian ?");

    M.addDisabled( Text("Is") + Name(1) + " nilpotent ?");

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finite ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finitely related ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("a free factor of") 
		   + Name( Parent(1) ) + " ?" );

    M.closeCascade();

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<SGOfFreeIndex,SMSubgroup>(),
	   Text("Compute the index of") + Name(1) + Text("in")
	   + Name( Parent(1) ) );

    M.add( Ctor1<SGOfFreeRank,SMSubgroup>(),
	   Text("Compute the rank of") + Name(1) + " ?" );

    M.addDisabled( Text("Compute a Schreier transversal of") + Name(1)
		   + Text("in") + Name( Parent(1) ) );

    M.separator();

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1)
		   + Text("abelianized"));
 
    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );
    
    M.separator();
    
    M.add( Ctor1<SGOfFreeNormaliser,SMSubgroup>(),
	   Text("Compute the normaliser of") + Name(1) + "in"
	   + Name( Parent(1) ) + "?"  );
 
    M.add( ReadNormalApproximation(),
	   Text("Compute the n-th normal approximation of ") + Name(1),
	   DefineInt("Enter the n  ", 1) );

    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.add( Ctor1<SGOfFreeHallCompletion,SMSubgroup>(),
	   Text("Find a virtual free factor of") + Name( Parent(1) )
	   + "containing" + Name(1) + "as a subgroup of finite index" );

    M.add( Ctor1<SGOfFreeNielsenBasis,SMSubgroup>(),
	   Text("Find a Nielsen basis for") + Name(1) );

    M.addDisabled( Text("Find a permutation representation of")
	   + Name( Parent(1) ) + Text("modulo") + Name(1) 
	   + Text("when the index of ") + Name(1) + " in " + Name( Parent(1) )
	   + Text("is finite") );
      
    M.closeCascade();

    M.startCascade( Text("Enumerate ...") );
 
    M.addDisabled( Text("Enumerate relators of ") + Name( 1 ) );

    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE_NILPOTENT );
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.addDisabled( Text("Is") + Name(1) + "abelian ?");

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.separator();

    M.add( Ctor1<IsSGCentral,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("central ?") );

    M.add( Ctor1<SGOfNGisNormalProblem,SMSubgroup>(),
	   Text("Is") + Name(1) + "normal ?" );

    M.closeCascade();
 
    M.startCascade( Text("Compute ...") );

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the torsion subgroup of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("abelianized") );
    M.addDisabled( Text("Compute the class of") + Name(1) );

    M.addDisabled( Text("Compute the derived length of") + Name(1) );

    M.addDisabled( Text("Compute the lower central series of") + Name(1) );

    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.addDisabled( Text("Compute the derived series of") + Name(1) );

    M.addDisabled( Text("Compute quotients of the derived series of")
		   + Name(1) );

    M.add( Ctor1<SGOfNGhirschNumberProblem,SMSubgroup>(),
	   Text("Compute the Hirsch number of") + Name(1) );

    M.addDisabled( Text("Compute a Malcev basis for") + Name(1) );

    M.addDisabled( Text("Compute a short-lex system of representatives for")
		   + Name(1) );

    M.addDisabled( Text("Compute an integral homology group of") + Name(1) );

    M.separator();
	
    M.add( Ctor1<SGOfNGindexProblem,SMSubgroup>(),
	   Text("Compute the index of") + Name(1) + "in" + Name( Parent(1) ) );
    
    M.add( Ctor1<SGOfNGcomputeBasisProblem,SMSubgroup>(),
	   Text("Compute a basis for") + Name(1) );

    M.add( Ctor1<SGOfNGnormalClosureGensProblem,SMSubgroup>(),
	   Text("Compute the normal closure of") + Name(1) +
	   Text("in") + Name( Parent(1) ) );
 
    M.closeCascade();

    
    M.startCascade( Text("Find ...") );

    M.addDisabled( Text("Find a polycyclic presentation of") + Name(1) );

    M.separator();

    M.addDisabled( Text("Find a finite rewriting system for") + Name(1) );

    M.addDisabled( Text("Find a permutation presentation of")
		   + Name( Parent(1) ) + Text("modulo") + Name(1) 
		   + Text("when the index of ") + Name(1) + " in "
		   + Name( Parent(1) ) + Text("is finite") );

    M.separator();

    M.add( Ctor1<SGOfNGbuildPresentationProblem,SMSubgroup>(),
	   Text("Find a presentation of") + Name(1) );
    
    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::NILPOTENT );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "of finite index in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) 
		   + "?" );
 
    M.add( Ctor1<IsSGCentral,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("central in") + Name( Parent(1) )
	   + "?" );

    M.add( Ctor1<SGOfNGisNormalProblem,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("normal in") + Name( Parent(1) )
	   + "?" );
 
    M.addDisabled( Text("Is") + Name(1) + "subnormal ?");
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic ?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant ?" );

    M.separator();

    M.add( Ctor1<IsSGTrivial,SMSubgroup>(),
	   Text("Is") + Name(1) + "trivial ?" );
    
    M.addDisabled( Text("Is") + Name(1) + "finite ?" );

    M.add( Ctor1<IsSGAbelian,SMSubgroup>(),
	   Text("Is") + Name(1) + "abelian ?" );

    M.addDisabled( Text("Is") + Name(1) + Text("free nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.closeCascade();

    M.startCascade( Text("Compute ...") );

    M.addDisabled( Text("Compute the index of") + Name(1) + "in" 
		   + Name( Parent(1) ) );

    M.addDisabled( Text("Compute a Schreier transversal of") + Name(1)
		   + Text("in") + Name( Parent(1) ) );
    
    
    M.addDisabled( Text("Compute the isolator of ") + Name(1)
		   + Text("in") + Name( Parent(1) ) );

    M.separator();

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the torsion subgroup of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1) 
		   + Text("abelianized") );
 
    M.add( Ctor1<SGOfNGcomputeNClassProblem,SMSubgroup>(),
	   Text("Compute the class of") + Name(1) );
    
    M.addDisabled( Text("Compute the derived length of") + Name(1) );

    M.addDisabled( Text("Compute the lower central series of") + Name(1) );

    M.addDisabled( Text("Compute lower central quotients of") + Name(1) );

    M.addDisabled( Text("Compute the derived series of") + Name(1) );

    M.addDisabled( Text("Compute quotients of the derived series of")
		   + Name(1) );

    M.add( Ctor1<SGOfNGhirschNumberProblem,SMSubgroup>(),
	   Text("Compute the Hirsch number of") + Name(1) );

    M.addDisabled( Text("Compute a Malcev basis for") + Name(1) );

    M.addDisabled( Text("Compute a short-lex system of representatives for")
		   + Name(1) );

    M.addDisabled( Text("Compute an integral homology group of") + Name(1) );

    M.separator();

    M.add( Ctor1<SGOfNGcomputeBasisProblem,SMSubgroup>(),
	   Text("Compute a basis for") + Name(1) );

    M.add( Ctor1<SGOfNGnormalClosureProblem,SMSubgroup>(),
	   Text("Compute the normal closure of") + Name(1) +
	   Text("in") + Name( Parent(1) ) );
  
    M.closeCascade();
    
    M.startCascade( Text("Find ...") );

    M.addDisabled( Text("Find a polycyclic presentation of") + Name(1) );

    M.separator();

    M.addDisabled( Text("Find a permutation presentation of")
		   + Name( Parent(1) ) + Text("modulo") + Name(1) 
		   + Text("when the index of ") + Name(1) + " in " 
		   + Name( Parent(1) ) + Text("is finite") );

    M.addDisabled( Text("Find a finite rewriting system for") + Name(1) );

    M.separator();

    M.add( Ctor1<SGOfNGbuildPresentationProblem,SMSubgroup>(),
	   Text("Find a presentation of") + Name(1) );
 
    M.closeCascade();

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::ONE_RELATOR ||
		      CheckinType( Parent(1) ) == SMFPGroup::ONE_RELATOR_WITH_TORSION
		      );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "of finite index in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) 
		   + "?" );

    M.add( Ctor1<IsSGCentral,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("central ?") );
 
    M.add( Ctor1<IsSGNormal,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("normal ?") );
 
    M.addDisabled( Text("Is") + Name(1) + "subnormal ?");
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic ?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant ?" );

    M.separator();
    
    M.add( Ctor1<IsSGTrivial,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("trivial ?") );

    M.add( Ctor1<IsSGAbelian,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("abelian ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("nilpotent ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finite ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finitely related ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.separator();

    M.add( Ctor1<ORIsMagnusSubgroup,SMSubgroup>(),
	   Text("Is") + Name(1) + "a Magnus subgroup ?" );

    M.closeCascade();

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<SGIndexProblem, SMSubgroup>(),
	   Text("Compute the index of") + Name(1) + Text("in")
	   + Name( Parent(1) ) );
    
    M.add( Ctor1<SchreierTransversal, SMSubgroup>(),
	   Text("Compute a Schreier transversal of") + Name(1)
	   + Text("in") + Name( Parent(1) ) );

    M.separator();

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1)
		   + Text("abelianized") );

    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.add( Ctor1<PermutationRepProblem, SMSubgroup>(),
	   Text("Find a permutation representation of")
	   + Name( Parent(1) ) + Text("modulo") + Name(1) 
	   + Text("when the index of ") + Name(1) + " in " + Name( Parent(1) )
	   + Text("is finite") );

    M.separator();

    M.add( Ctor1<SGPresentationProblem,SMSubgroup>(),
	   Text("Find a presentation for") + Name(1) );
    
    M.closeCascade();

    M.startCascade( Text("Enumerate ...") );
 
    M.add( Ctor1<SGRelatorEnumerator,SMSubgroup>(),
	   Text("Enumerate relators of ") + Name( 1 ) );

    M.closeCascade();

    M.startItemGroup(CheckinType(Parent(1)) == SMFPGroup::SMALL_CANCELLATION );

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + "=" + Name( Parent(1) ) + "?" );
    
    M.addDisabled( Text("Is") + Name(1) + "of finite index in" 
		   + Name( Parent(1) ) + "?" );

    M.addDisabled( Text("Is") + Name(1) + "isolated in" + Name( Parent(1) ) 
		   + "?" );

    M.add( Ctor1<IsSGCentral,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("central ?") );
 
    M.add( Ctor1<IsSGNormal,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("normal ?") );
 
    M.addDisabled( Text("Is") + Name(1) + "subnormal ?");
    
    M.addDisabled( Text("Is") + Name(1) + "characteristic ?" );

    M.addDisabled( Text("Is") + Name(1) + "fully invariant ?" );

    M.separator();
    
    M.add( Ctor1<IsSGTrivial,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("trivial ?") );

    M.add( Ctor1<IsSGAbelian,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("abelian ?") );

    M.add( ReadIsSGNilpotentItem(),
	   Text("Is") + Name(1) + Text("nilpotent ?"),
	   DefineInt("Enter the nilpotency class: ", 1) );

    M.addDisabled( Text("Is") + Name(1) + Text("polycyclic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("solvable ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("perfect ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finite ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("finitely related ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("automatic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("hyperbolic ?") );

    M.addDisabled( Text("Is") + Name(1) + Text("free ?") );

    M.closeCascade();

    M.startCascade( Text("Compute ...") );

    M.add( Ctor1<SGIndexProblem, SMSubgroup>(),
	   Text("Compute the index of") + Name(1) + Text("in")
	   + Name( Parent(1) ) );

    M.add( Ctor1<SchreierTransversal, SMSubgroup>(),
	   Text("Compute a Schreier transversal of") + Name(1)
	   + Text("in") + Name( Parent(1) ) );

    M.separator();

    M.addDisabled( Text("Compute the order of") + Name(1) );

    M.addDisabled( Text("Compute the center of") + Name(1) );

    M.addDisabled( Text("Compute the canonical decomposition of") + Name(1)
		   + Text("abelianized") );

    M.addDisabled( Text("Compute a nilpotent quotient of") + Name(1) );

    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.add( Ctor1<PermutationRepProblem, SMSubgroup>(),
	   Text("Find a permutation representation of")
	   + Name( Parent(1) ) + Text("modulo") + Name(1) 
	   + Text("when the index of ") + Name(1) + " in " + Name( Parent(1) )
	   + Text("is finite") );

    M.separator();

    M.add( Ctor1<SGPresentationProblem,SMSubgroup>(),
	   Text("Find a presentation for") + Name(1) );
    
    M.closeCascade();


    M.startCascade( Text("Enumerate ...") );
 
    M.add( Ctor1<SGRelatorEnumerator,SMSubgroup>(),
	   Text("Enumerate relators of ") + Name( 1 ) );

    M.closeCascade();
 
    M.done();
  }
  
  {
    Menu1<SMSetOfWords> M( Menu::TOOLS );
    
    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE );

    M.add( Ctor1<FreeGeneralIsPartOfBasisProblem,SMSetOfWords>(),
	   Text("Is") + Name(1) + "part of a basis of" +
	   Name( Parent(1) ) + "?" );
    
    M.done();
  }

  {
    Menu1<SMVectorOfWords> M( Menu::TOOLS );
    
    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE );
    /*
    M.add( Ctor1<SGOfFreeWhiteheadReduction,SMVectorOfWords>(),
	   Text("Find Whitehead reduction of") + Name(1) 
	   );
    */  
    M.add( Ctor1<ACConjectureProblem,SMVectorOfWords>(),
	   Text("Find Andrews-Curtis reduction of") + Name(1) );

    M.add( Ctor1<FindWhiteheadMinimalProblem,SMVectorOfWords>(),
	   Text("Find Whitehead reduction of") + Name(1) + "(genetic)" );

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FP ||
		      CheckinType( Parent(1) ) == SMFPGroup::FREE );
    
    M.add( Ctor1<FPNewPresentationProblem,SMVectorOfWords>(),
	   Text("Compute a presentation for") + Name( Parent(1) )
	   + Text("on new generators given by") + Name(1) );
  
    M.done();
  }

  {
    Menu1<SMMap> M( Menu::TOOLS );

    M.startItemGroup( CheckinType( Domain(1) ) == SMFPGroup::FREE &&
		      CheckinType( Range(1) ) == SMFPGroup::FREE );
    
    M.add( Ctor1<ExtendToHomProblem,SMMap>(),
	   Text("Extend") + Name( 1 ) + "to a homomorphism" );

    M.startItemGroup( (CheckinType( Domain(1) ) != SMFPGroup::FREE ||
		      CheckinType( Range(1) ) != SMFPGroup::FREE ) );
    
    M.add( Ctor1<ExtendToHomProblem,SMMap>(),
	   Text("Does") + Name( 1 ) + "extend to a homomorphism ?" );

/*
	 M.startItemGroup( ( Domain(1) == Range(1) ) &&
			   ( CheckinType( Domain(1) ) == SMFPGroup::FREE ||
			     IsHomo( 1 ) )
			   );
	 
	 M.add( ReadPowerOfMapItem(),
		Text("Compute a power of") + Name(1),
		DefineInt("What power?", 2)
		);
*/	 
/*
	 M.startItemGroup( Domain(1) == Range(1) &&
			   CheckinType( Domain(1) ) == SMFPGroup::FREE
			   );

	 M.add( Ctor1<EndoOnFreeIsAut,SMMap>(),
		Text("Is") + Name(1) + "an automorphism of"
		+ Name( Domain(1) ) + "?"
		);

	 M.add( Ctor1<EndoOnFreeIsInner,SMMap>(),
		Text("Is") + Name(1) + "an inner automorphism of"
		+ Name( Domain(1) ) + "?"
		);
	 
	 M.startItemGroup( IsAuto( 1 ) &&
			   CheckinType( Domain(1) ) == SMFPGroup::FREE
			   );


         M.add( Ctor1<InverseAuto,SMMap>(),
	 Text("Compute the inverse of") + Name( 1 )
	 );
	 
	 M.separator();
	 
	 M.add( Ctor1<ExtendFreeByAut,SMMap>(),
	 Text("Extend") + Name( Domain(1) ) + "by" + Name(1)
	 );
*/	 

    M.done();
  }

  {
    Menu1<SMHomomorphism> M( Menu::TOOLS );

    M.startItemGroup( );

    M.startItemGroup( CheckinType( Domain(1) ) != SMFPGroup::ABELIAN 
		      && CheckinType( Domain(1) ) != SMFPGroup::FREE );
 
    M.addDisabled( Text("Is") + Name( 1 ) + "a monomorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) != SMFPGroup::ABELIAN &&
		      CheckinType( Domain(1) ) != SMFPGroup::NILPOTENT &&
		      CheckinType( Domain(1) ) != SMFPGroup::FREE_NILPOTENT );

    M.addDisabled( Text("Is") + Name( 1 ) + "an epimorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) == SMFPGroup::ABELIAN );

    M.add( Ctor1<AbelianHomIsMono,SMHomomorphism>(),
	   Text("Is") + Name(1) + "a monomorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) == SMFPGroup::ABELIAN ||
		      CheckinType( Domain(1) ) == SMFPGroup::NILPOTENT ||
		      CheckinType( Domain(1) ) == SMFPGroup::FREE_NILPOTENT );

    M.add( Ctor1<AbelianHomIsEpi,SMHomomorphism>(),
	   Text("Is") + Name(1) + "an epimorphism ?" );

    M.add( Ctor1<AbelianHomIsAuto,SMHomomorphism>(),
	   Text("Is") + Name(1) + "an automorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) == SMFPGroup::FREE );

    M.add( Ctor1<EndoOnFreeIsMono,SMHomomorphism>(),
	   Text("Is") + Name( 1 ) + "a monomorphism ?" );

    M.add( Ctor1<EndoOnFreeIsEpi,SMHomomorphism>(),
	   Text("Is") + Name( 1 ) + "an epimorphism ?" );

    M.add( Ctor1<EndoOnFreeIsAut,SMHomomorphism>(),
	   Text("Is") + Name(1) + "an automorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) != SMFPGroup::FREE &&
		      CheckinType( Domain(1) ) != SMFPGroup::ABELIAN &&
		      CheckinType( Domain(1) ) != SMFPGroup::NILPOTENT &&
		      CheckinType( Domain(1) ) != SMFPGroup::FREE_NILPOTENT );
	 
    M.addDisabled( Text("Is") + Name( 1 ) + "an automorphism ?" );

    M.startItemGroup( IsAuto(1) &&
		      CheckinType( Domain(1) ) == SMFPGroup::ABELIAN );
    M.separator();

    M.add( Ctor1<AbelianOrderOfAuto,SMHomomorphism>(),
	   Text("Compute the order of ") + Name(1) );
    
    M.add( Ctor1<AbelianInverseAuto,SMHomomorphism>(),
	   Text("Find the inverse of ") + Name(1) );

    M.add( Ctor1<AbelianFixedPointsOfAutoProblem,SMHomomorphism>(),
	   Text("Compute the fixed points of ") + Name(1) );

    M.startItemGroup( IsAuto(1) &&
		       CheckinType( Domain(1) ) == SMFPGroup::FREE_NILPOTENT );
    M.separator();

    M.add( Ctor1<AutoInFreeIsFinite,SMHomomorphism>(),
	   Text("Is") + Name(1) + "of finite order?" );
      
    M.add( Ctor1<FNGAutoIsIAAut,SMHomomorphism>(),
	   Text("Is") + Name(1) + "an IA-automorphism ?" );
	
    M.add( Ctor1<NGInverseAuto,SMHomomorphism>(),
	   Text("Find the inverse of ") + Name(1) );
	
    M.startItemGroup( IsAuto(1) &&
		      CheckinType( Domain(1) ) == SMFPGroup::NILPOTENT );
    M.separator();
    
    M.add( Ctor1<NGAutoIsIAAut,SMHomomorphism>(),
	   Text("Is") + Name(1) + "an IA-automorphism ?" );

    M.startItemGroup( IsAuto(1) &&
		      CheckinType( Domain(1) ) == SMFPGroup::FREE );
    M.separator();
    
    M.add( Ctor1<AutoInFreeIsFinite,SMHomomorphism>(),
	   Text("Is") + Name(1) + "of finite order?" );   

    M.add( Ctor1<EndoOnFreeIsIAAut,SMHomomorphism>(),
	   Text("Is") + Name(1) + "an IA-automorphism ?" );

 
    M.add( Ctor1<EndoOnFreeIsInner,SMHomomorphism>(),
	   Text("Is") + Name(1) + "an inner automorphism ?" );

    M.add( Ctor1<InverseAuto,SMHomomorphism>(),
	   Text("Compute the inverse of") + Name( 1 ) );

    M.separator();

    M.add( Ctor1<ExtendFreeByAut,SMHomomorphism>(),
	   Text("Extend") + Name( Domain(1) ) + "by" + Name(1) );

    M.add( Ctor1<AutoWhiteheadDecomposition,SMHomomorphism>(),
	   Text("Find Whitehead decomposition of") + Name(1) );
	 	 
    M.startItemGroup( !IsAuto(1) ||
		      (CheckinType( Domain(1) ) != SMFPGroup::FREE &&
		       CheckinType( Domain(1) ) != SMFPGroup::FREE_NILPOTENT &&
		       CheckinType( Domain(1) ) != SMFPGroup::NILPOTENT &&
		       CheckinType( Domain(1) ) != SMFPGroup::ABELIAN) );

    M.addDisabled( Text("Is") + Name( 1 ) + "an IA-automorphism ?" );

    M.startItemGroup( !IsAuto(1) ||
		     CheckinType( Domain(1) ) != SMFPGroup::FREE );

    M.addDisabled( Text("Is") + Name( 1 ) + "an inner automorphism ?"  );

    M.done();
  }

  {
    Menu1<SMHomomorphism2> M( Menu::TOOLS );

    M.startItemGroup( );

    M.startItemGroup( CheckinType( Domain(1) ) != SMFPGroup::ABELIAN );

    M.addDisabled( Text("Is") + Name( 1 ) + "a monomorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) != SMFPGroup::ABELIAN &&
		      CheckinType( Domain(1) ) != SMFPGroup::FREE_NILPOTENT &&
		      CheckinType( Domain(1) ) != SMFPGroup::NILPOTENT );
    
    M.addDisabled( Text("Is") + Name( 1 ) + "an epimorphism ?" );
    
    M.startItemGroup( CheckinType( Domain(1) ) != SMFPGroup::ABELIAN );

    M.addDisabled( Text("Is") + Name( 1 ) + "an isomorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) == SMFPGroup::ABELIAN );

    M.add( Ctor1<AbelianHomIsMono,SMHomomorphism2>(),
	   Text("Is") + Name(1) + "a monomorphism ?" );
	 
    M.add( Ctor1<AbelianHomIsEpi,SMHomomorphism2>(),
	   Text("Is") + Name(1) + "an epimorphism ?" );

    M.add( Ctor1<AbelianHomIsIso,SMHomomorphism2>(),
	   Text("Is") + Name(1) + "an isomorphism ?" );

    M.startItemGroup( CheckinType( Domain(1) ) == SMFPGroup::NILPOTENT ||
		      CheckinType( Domain(1) ) == SMFPGroup::FREE_NILPOTENT );

    M.add( Ctor1<AbelianHomIsEpi,SMHomomorphism2>(),
	   Text("Is") + Name(1) + "an epimorphism ?" );

    M.startItemGroup( IsAuto( 1 ) &&
		      CheckinType( Domain(1) ) == SMFPGroup::FREE );

    M.add( Ctor1<InverseAuto,SMHomomorphism2>(),
	   Text("Compute the inverse of") + Name( 1 ) );
	 
    M.startItemGroup( ( CheckinType( Domain(1) ) == SMFPGroup::FP ) &&
		      ( CheckinType( Range(1) ) == SMFPGroup::FP  ) );
    
    M.add( Ctor1<FPKernelPresentationProblem,SMHomomorphism2>(),
	   Text("Compute a presentation for the kernel of") + Name( 1 ) );
	
    M.separator();
	 
    M.add( Ctor1<ExtendFreeByAut,SMHomomorphism2>(),
	   Text("Extend") + Name( Domain(1) ) + "by" + Name(1) );
	 	 
    M.done();
  }

  {
    Menu1<SMEquation2> M( Menu::TOOLS );

    M.startItemGroup( CheckinType( Parent( 1 ) ) == SMFPGroup::FREE );

    M.add( Ctor1<GAEquationProblem, SMEquation2>(),
	   Text("Find a solution for") + Name(1) );

    M.startItemGroup( CheckinType( Parent( 1 ) ) == SMFPGroup::FP ||
		      CheckinType( Parent( 1 ) ) == SMFPGroup::ABELIAN );
    
    M.add( Ctor1<EquationInFPProblem , SMEquation2>(),
	   Text("Find all solutions for") + Name(1) );

    M.done();
  }

  {
    Menu1<SMEquation> M( Menu::TOOLS );

    M.startItemGroup();

    M.add( Ctor1<QuadEquationSurfaceForm, SMEquation>(),
	   Text("Find a surface form of") + Name(1) );

	 
    M.add( Ctor1<EquationProblem,SMEquation>(),
	   Text("Solve the equation") + Name(1) + " = 1" );
	 	 
    M.add( Ctor1<QuickEquationProblem,SMEquation>(),
	   Text("Find basic solutions of the equation") + Name(1) + " = 1" );

    /* @dp Debug only menu items.
       M.separator();

       M.add( Ctor1<QEquationInFreeBasicSolutions,SMEquation>(),
       Text("View known basic solutions of ") + Name(1)
       );

       M.add( Ctor1<QEquationInFreeRegStabGenerators,SMEquation>(),
       Text("View known generators of regular stabilizer of") + Name(1)
       );
    */

    M.done();
  } 

  {
    Menu1<SMEqSystem> M( Menu::TOOLS );

    M.startItemGroup();

    M.add( Ctor1<EqSystemInFPProblem , SMEqSystem>(),
	   Text("Find solutions for") + Name(1) );

    M.done();
  }

  {
    Menu2<SMWord,SMWord> M( Menu::TOOLS );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::ABELIAN );
    
    M.add( Ctor2<ProductInAbelianForm, SMWord, SMWord>(),
	   Text("Compute the product") + Name(1) + Name(2)
	   + Text(", in abelian form") );

    M.add( Ctor2<ProductInAbelianForm, SMWord, SMWord>(true),
	   Text("Compute the product") + Name(2) + Name(1)
	   + Text(", in abelian form") );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) != SMFPGroup::ABELIAN );
    
    M.add( Ctor2<FormalProductOfWords,SMWord,SMWord>(),
	   Text("Compute the product") + Name(1) + " * " + Name(2) );
    
    M.add( Ctor2<FormalProductOfWords,SMWord,SMWord>(true),
	   Text("Compute the product") + Name(2) + " * " + Name(1) );
 
    M.add( Ctor2<ConjugateOfWord,SMWord,SMWord>(),
	   Text("Compute the conjugate of") + Name(1) + Text("by") + Name(2) );
    
    M.add( Ctor2<ConjugateOfWord,SMWord,SMWord>(true),
	   Text("Compute the conjugate of") + Name(2) + Text("by") + Name(1) );
    
    M.add( Ctor2<CommutatorOfWords,SMWord,SMWord>(),
	   Text("Compute the commutator of") + Name(1) + Text("and") +Name(2));

    M.add( Ctor2<CommutatorOfWords,SMWord,SMWord>(true),
	   Text("Compute the commutator of") + Name(2) + Text("and") +Name(1));
    
    M.startItemGroup();
 
    M.separator();
    
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::ABELIAN );
 
    M.add( Ctor2<AbelianEqualityProblem, SMWord, SMWord>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );

    M.add( Ctor2<IsAbelianWordPowerOfSecond,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("a power of") + Name(2) + Text("?") );

    M.add( Ctor2<IsAbelianWordPowerOfSecond,SMWord,SMWord>(true),
	   Text("Is") + Name(2) + Text("a power of") + Name(1) + Text("?") );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::FREE );
    
    M.add( Ctor2<WordsAreEqual,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
   
    M.add( Ctor2<ConjugacyProblemInFree,SMWord,SMWord>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "conjugate in"
	   + Name( Parent(1) ) + "?" );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::FREE_NILPOTENT );
  
    M.add( Ctor2<AreEltsEqual, SMWord, SMWord>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
    /* 
       M.addDisabled( Text("Are") + Name(1) + Text("and") + Name(2)
       + Text("conjugate in") + Name( Parent( 1 ) ) + Text("?")
       );
    */
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      (CheckinType( Parent(1) ) == SMFPGroup::AP_FREE ||
		       CheckinType( Parent(1) ) == SMFPGroup::AP_FREE_CYCLIC));
    
    M.add( Ctor2<APOfFreeAreEqual,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
    
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::HNN_FREE );
    
    M.add( Ctor2<HNNofFreeGroup_AreEqual,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      (CheckinType( Parent(1) ) == SMFPGroup::FP ||
		       CheckinType( Parent(1) ) == SMFPGroup::NILPOTENT) );
    
    M.add( Ctor2<AreEltsEqual,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::NILPOTENT );
    
    M.addDisabled( Text("Are") + Name(1) + Text("and") + Name(2)
		   + Text("conjugate in") + Name( Parent( 1 ) ) + Text("?") );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::ONE_RELATOR_WITH_TORSION );
    
    M.add( Ctor2<ORWithTorsionAreEltsEqual,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
    
    M.add( Ctor2<ORWithTorsionConjugacyProblem,SMWord,SMWord>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "conjugate in"
	   + Name( Parent(1) ) + "?" );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) != SMFPGroup::FREE &&
		      CheckinType( Parent(1) ) != SMFPGroup::ABELIAN &&
		      CheckinType( Parent(1) ) != SMFPGroup::NILPOTENT &&
		      CheckinType( Parent(1) ) != SMFPGroup::FREE_NILPOTENT &&
		      CheckinType( Parent(1) ) != SMFPGroup::AP_FREE_CYCLIC &&
		      CheckinType( Parent(1) ) != SMFPGroup::HNN_FREE &&
		      CheckinType( Parent(1) ) != SMFPGroup::FREE );
    
    M.add( Ctor2<ConjugacyProblem,SMWord,SMWord>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "conjugate in"
	   + Name( Parent(1) ) + "?"  );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::AP_FREE_CYCLIC );
    
    M.add( Ctor2<APOfFreeCyclic_ConjugacyProblem,SMWord,SMWord>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "conjugate in"
	   + Name( Parent(1) ) + "?" );
    
 
    M.add( Ctor2<APOfFreeCyclic_IsEltAProperPowerOfSecond,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("a proper power of")
	   + Name(2) + Text("?") );
    
    M.add( Ctor2<APOfFreeCyclic_IsEltAProperPowerOfSecond,SMWord,SMWord>(true),
	   Text("Is") + Name(2) + Text("a proper power of")
	   + Name(1) + Text("?") );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::HNN_FREE );
    
    M.add( Ctor2<HNNofFreeGroup_ConjugacyProblem,SMWord,SMWord>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "conjugate in"
	   + Name( Parent(1) ) + "?" );
    
 
    M.add( Ctor2<HNNofFreeGroup_IsProperPowerOfSecond,SMWord,SMWord>(),
	   Text("Is") + Name(1) + Text("a proper power of")
	   + Name(2) + Text("?") );
    
    M.add( Ctor2<HNNofFreeGroup_IsProperPowerOfSecond,SMWord,SMWord>(true),
	   Text("Is") + Name(2) + Text("a proper power of")
	   + Name(1) + Text("?") );
   
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) != SMFPGroup::FREE &&
		      CheckinType( Parent(1) ) != SMFPGroup::FREE_NILPOTENT &&
		      CheckinType( Parent(1) ) != SMFPGroup::AP_FREE_CYCLIC &&
		      CheckinType( Parent(1) ) != SMFPGroup::ABELIAN );
    
    M.addDisabled( Text("Is") + Name(1) + Text("a proper power of")
		   + Name(2) + Text("?") );
    
    M.addDisabled( Text("Is") + Name(2) + Text("a proper power of")
		   + Name(1) + Text("?") );

    M.done();
  }
  
  {
    Menu2<SMHomomorphism,SMHomomorphism> M( Menu::TOOLS );
  
    M.startItemGroup( Range( 1 ) == Domain( 2 ) );
    
    M.add( Ctor2<ComposeMaps,SMHomomorphism,SMHomomorphism>(),
	   Text("Form the composition") + Name(2) + Text(Name(1)) );
    
    M.startItemGroup( Range( 2 ) == Domain( 1 ) );
    
    M.add( Ctor2<ComposeMaps,SMHomomorphism,SMHomomorphism>(true),
	   Text("Form the composition") + Name(1) + Text(Name(2)) );
 
    M.startItemGroup( Domain( 1 ) == Domain( 2 ) &&
		      CheckinType( Domain(1) ) != SMFPGroup::FREE );
    
    M.addDisabled( Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
    
    
    M.startItemGroup( Domain( 1 ) == Domain( 2 ) &&
		      CheckinType( Domain(1) ) == SMFPGroup::FREE );
    
    M.add( Ctor2<FreeAreHomsEqual,SMHomomorphism,SMHomomorphism>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
    
    M.done();
  }
  
  {
    Menu2<SMHomomorphism2,SMHomomorphism2> M( Menu::TOOLS );
    
    M.startItemGroup( Range( 1 ) == Domain( 2 ) );
    
    M.add( Ctor2<ComposeMaps,SMHomomorphism2,SMHomomorphism2>(),
	   Text("Form the composition") + Name(2) + Text(Name(1))  );
    
    M.startItemGroup( Range( 2 ) == Domain( 1 ) );
    
    M.add( Ctor2<ComposeMaps,SMHomomorphism2,SMHomomorphism2>(true),
	   Text("Form the composition") + Name(1) + Text(Name(2)) );
    
    M.startItemGroup( Domain(1) == Domain(2) &&
		      Range(1) == Range(2) &&
		      CheckinType( Domain(1) ) != SMFPGroup::FREE );
	 
    M.addDisabled( Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
	 
    M.startItemGroup( Domain(1) == Domain(2) &&
		      Range(1) == Range(2) &&
		      CheckinType( Domain(1) ) == SMFPGroup::FREE );
	 
    M.add( Ctor2<FreeAreHomsEqual,SMHomomorphism2,SMHomomorphism2>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );
    
    M.done();
  }

  {
    Menu2<SMHomomorphism,SMHomomorphism2> M( Menu::TOOLS );
    
    M.startItemGroup( Range( 1 ) == Domain( 2 ) );
	 
    M.add( Ctor2<ComposeMaps,SMHomomorphism,SMHomomorphism2>(),
	   Text("Form the composition") + Name(2) + Text(Name(1)) );
	 
    M.startItemGroup( Range( 2 ) == Domain( 1 ) );
	 
    M.add( Ctor2<ComposeMaps,SMHomomorphism,SMHomomorphism2>(true),
	   Text("Form the composition") + Name(1) + Text(Name(2)) );
    
    M.done();
  }

  {
    Menu2<SMHomomorphism, SMWord> M( Menu::TOOLS );

    M.startItemGroup( Parent( 2 ) == Domain( 1 ) );
	 
    M.add( Ctor2<ImageUnderMap,SMHomomorphism,SMWord>(),
	   Text("Compute the image of") + Name(2) + "under" + Name(1) );
	 
    M.done();
  }

  {
    Menu2<SMHomomorphism2, SMWord> M( Menu::TOOLS );

    M.startItemGroup( Parent( 2 ) == Domain( 1 ) );
	 
    M.add( Ctor2<ImageUnderMap,SMHomomorphism2,SMWord>(),
	   Text("Compute the image of") + Name(2) + "under" + Name(1) );
	 
    M.done();
  }
  
  {
    Menu2<SMHomomorphism, SMSubgroup> M( Menu::TOOLS );

    M.startItemGroup( Parent( 2 ) == Domain( 1 ) );
	 
    M.add( Ctor2<SGImageUnderMap,SMHomomorphism,SMSubgroup>(),
	   Text("Compute the image of") + Name(2) + "under" + Name(1) );
	 
    M.done();
  }

  {
    Menu2<SMHomomorphism2, SMSubgroup> M( Menu::TOOLS );

    M.startItemGroup( Parent( 2 ) == Domain( 1 ) );
	 
    M.add( Ctor2<SGImageUnderMap,SMHomomorphism2,SMSubgroup>(),
	   Text("Compute the image of") + Name(2) + "under" + Name(1) );
	 
    M.done();
  }

  //!!!!!!!!!!!!

  {
    Menu2<SMSubgroup,SMWord> M( Menu::TOOLS );

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::ABELIAN );


    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor2<WordInSGOfAbelian, SMSubgroup, SMWord>(),
	   Text("Is") + Name(2) + "in" + Name(1) + "?" );
 
    M.add( Ctor2<WordPowerInSGOfAbelian, SMSubgroup, SMWord>(),
	   Text("Is a power of") + Name(2) + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a power of an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a non-trivial endomorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is a power of a non-trivial endomorphic image of") 
		   + Name(2) + "in" + Name(1) + "?" );
    
    M.add( Ctor2<WordInSGBasisOfAbelian, SMSubgroup, SMWord>(),
	   Text("Is") + Name(2) + "part of a canonical basis for" 
	   + Name(1) + "?" );
    
    M.closeCascade();

    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor2<AbelianSGWordProblem,SMSubgroup,SMWord>(),
	   Text("Rewrite") + Name(2) + "in terms of a canonical basis for"
	   + Name(1) );
    
    M.closeCascade();
    
    M.startCascade( Text("Compute ...") );

    M.add( Ctor2<AbelianEltPowerSubgr,SMSubgroup,SMWord>(),
	   Text("Compute the order of") + Name(2) + "in the factor group" 
	   + Name( Parent(1) ) + "/" + Name(1) );

    M.add( Ctor2<AbelianQuotientWordProblem,SMSubgroup,SMWord>(),
	   Text("Compute the the canonical decomposition of") 
	   + Name(2) + "in terms of a canonical basis for" 
	   + Name( Parent(1) ) + "/" + Name(1) );

    M.closeCascade();

    M.startItemGroup( Parent(1) == Parent(2) && 
		      ( CheckinType(Parent(1)) == SMFPGroup::AP_FREE ||
			CheckinType(Parent(1)) == SMFPGroup::AP_FREE_CYCLIC ||
			CheckinType(Parent(1)) == SMFPGroup::HNN_FREE ) );
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );
 
    M.add( Ctor2<SubgroupContainmentProblem,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "in" + Name(1) + "?" );
 
    M.addDisabled( Text("Is a power of") + Name(2) + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a power of an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a non-trivial endomorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is a power of a non-trivial endomorphic image of") 
		   + Name(2) + "in" + Name(1) + "?" );
    
    M.closeCascade();



    M.startItemGroup( Parent(1) == Parent(2) && 
		      (CheckinType(Parent(1))==SMFPGroup::FP || 
		       CheckinType(Parent(1))==SMFPGroup::SMALL_CANCELLATION));


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor2<SubgroupContainmentProblem,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "in" + Name(1) + "?" );
 
    M.addDisabled( Text("Is a power of") + Name(2) + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a power of an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a non-trivial endomorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is a power of a non-trivial endomorphic image of") 
		   + Name(2) + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is") + Name(2) + "part of a canonical basis for" 
		   + Name(1) + "?" );
    
    M.closeCascade();


    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor2<SGRewriteWordProblem,SMSubgroup,SMWord>(),
	   Text("Rewrite") + Name(2) + " in terms of Schreier generators of " 
	   + Name(1) );	 
    
    M.closeCascade();

    
    M.startCascade( Text("Find ...") );

    M.add( Ctor2<WordRepresentativeProblem, SMSubgroup,SMWord>(),
	   Text("Find a Schreier representative of ") + Name(2) +
	   " modulo " + Name(1) );	 

    M.closeCascade();



    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::FREE );


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor2<WordInSGOfFree,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "in" + Name(1) + "?" );

    M.add( Ctor2<PowerOfWordInSGOfFree,SMSubgroup,SMWord>(),
	   Text("Is a power of") + Name(2) + "in" + Name(1) + "?" );
 
    M.add( Ctor2<ConjugateOfWordInSGOfFree,SMSubgroup,SMWord>(),
	   Text("Is a conjugate of") + Name(2) + "in" + Name(1) + "?" );
 
    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a power of an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a non-trivial endomorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is a power of a non-trivial endomorphic image of") 
		   + Name(2) + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is") + Name(2) + "primitive in" + Name(1) + "?" );
    
    M.closeCascade();


    M.startCascade( Text("Rewrite ...") );

    M.add( Ctor2<WordInNielsenBasisSGOfFree,SMSubgroup,SMWord>(),
	   Text("Rewrite") + Name(2) + "in term of a Nielsen basis for" 
	   + Name(1) );

    M.closeCascade();

    
    M.startCascade( Text("Compute ...") );

    M.addDisabled( Text("Compute the length of") + Name(2) 
		   + "when rewritten as a reduced word in a Nielsen basis for" 
		   + Name(1) );

    M.addDisabled( Text("Compute the representative of") + Name(2)
	   + "in a left Schreier transversal of" + Name(1) 
	   + "in" + Name( Parent(1) ) );

    M.add( Ctor2<SchreierRepOfWordInSGOfFree,SMSubgroup,SMWord>(),
	   Text("Compute the representative of") + Name(2)
	   + "in a right Schreier transversal of" + Name(1) 
	   + "in" + Name( Parent(1) ) );

    M.closeCascade();
     
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) && 
		      (CheckinType( Parent(1) ) == SMFPGroup::FREE_NILPOTENT ||
		       CheckinType( Parent(1) ) == SMFPGroup::NILPOTENT));
    
   
    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor2<SGOfNGWordContainProblem,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "in" + Name(1) + "?"
	   );
    
    M.addDisabled( Text("Is a power of") + Name(2) + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a power of an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a non-trivial endomorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is a power of a non-trivial endomorphic image of") 
		   + Name(2) + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is") + Name(2) + "part of a canonical basis for" 
		   + Name(1) + "?" );
    
    M.closeCascade();



    M.startCascade( Text("Rewrite ...") );

    M.addDisabled( Text("Rewrite") + Name(2) 
		   + "in terms of a Malcev basis for" + Name(1) );

    M.separator();

    M.add( Ctor2<SGOfNGDecomposeWordProblem,SMSubgroup,SMWord>(),
	   Text("Rewrite") + Name(2) 
	   + "in terms of a basis for" + Name(1)
	   );
	
    M.closeCascade();



    M.startItemGroup( Parent( 1 ) == Parent( 2 ) && 
		      CheckinType( Parent(1) ) == SMFPGroup::ONE_RELATOR
		      );


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor2<SubgroupContainmentProblem,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "in" + Name(1) + "?"
	   );
 
    M.addDisabled( Text("Is a power of") + Name(2) + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a power of an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a non-trivial endomorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is a power of a non-trivial endomorphic image of") 
		   + Name(2) + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is") + Name(2) + "part of a canonical basis for" 
		   + Name(1) + "?" );
    
    M.separator();

    M.add( Ctor2<ORExtendedWordProblem,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "an element of Magnus subgroup"
	   + Name(1) + "?"
	   );

    M.closeCascade();



    M.startItemGroup( Parent( 1 ) == Parent( 2 ) && 
		      CheckinType( Parent(1) ) 
		      == SMFPGroup::ONE_RELATOR_WITH_TORSION
		      );


    M.startCascade( Text("Is") + Name(1) + Text("...") );

    M.add( Ctor2<SubgroupContainmentProblem,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "in" + Name(1) + "?"
	   );
 
    M.addDisabled( Text("Is a power of") + Name(2) + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a power of an automorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );

    M.addDisabled( Text("Is a non-trivial endomorphic image of") + Name(2) 
		   + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is a power of a non-trivial endomorphic image of") 
		   + Name(2) + "in" + Name(1) + "?" );
    
    M.addDisabled( Text("Is") + Name(2) + "part of a canonical basis for" 
		   + Name(1) + "?" );
    
    M.separator();

    M.add( Ctor2<ORWithTorsionExtendedWordProblem,SMSubgroup,SMWord>(),
	   Text("Is") + Name(2) + "an element of Magnus subgroup"
	   + Name(1) + "?"
	   );

    M.closeCascade();

    M.done();
  }
  
  {
    Menu2<SMSubgroup,SMSubgroup> M( Menu::TOOLS );
	 
    //@@@@@@@@@@@@

    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::ABELIAN );
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor2<AbelianSGEqualityProblem,SMSubgroup,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );

    M.add( Ctor2<AbelianSGContainmentProblem,SMSubgroup,SMSubgroup>(true),
	   Text("Is") + Name(1) + "contained in" + Name(2) + "?"  );

    M.add( Ctor2<AbelianSGContainmentProblem,SMSubgroup,SMSubgroup>(),
	   Text("Is") + Name(2) + "contained in" + Name(1) + "?"  );

    M.addDisabled( Text("Is an automorphic image of") + Name(1) 
		   + "contained in" + Name(2) + "?"  );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "contained in" + Name(1) + "?" );

    M.add( Ctor2<AbelianSGDirectFactorProblem,SMSubgroup,SMSubgroup>(true),
	   Text("Is") + Name(1) + "a direct factor of" + Name(2) + "?" );

    M.add( Ctor2<AbelianSGDirectFactorProblem,SMSubgroup,SMSubgroup>(),
	   Text("Is") + Name(2) + "a direct factor of" + Name(1) + "?" );

    M.separator();

    M.add( Ctor2<AbelianIsIsomorphicSG,SMSubgroup,SMSubgroup>(),
	   Text("Is ") + Name(1) + "isomorphic to" + Name(2) + "?" );
    
    M.closeCascade();
    
    M.startCascade( Text("Find ...") );

    M.add( Ctor2<AbelianSGJoin,SMSubgroup,SMSubgroup>(),
	   Text("Find the join of") + Name(1) + "and" + Name(2) );
    
    M.add( Ctor2<AbelianSGIntersectionProblem,SMSubgroup,SMSubgroup>(),
	   Text("Find the intersection of") + Name(1) + "and" + Name(2) );
    
    M.closeCascade();
    
    M.startItemGroup( Parent( 1 ) == Parent(2) && 
		      (CheckinType(Parent(1))==SMFPGroup::AP_FREE ||
		       CheckinType(Parent(1))==SMFPGroup::AP_FREE_CYCLIC ||
		       CheckinType(Parent(1))==SMFPGroup::HNN_FREE ||
		       CheckinType(Parent(1))==SMFPGroup::FP ||
		       CheckinType(Parent(1))==SMFPGroup::SMALL_CANCELLATION ||
		       CheckinType(Parent(1))==SMFPGroup::ONE_RELATOR ||
		       CheckinType(Parent(1))==SMFPGroup::ONE_RELATOR_WITH_TORSION));
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.addDisabled( Text("Is") + Name(1) + Text("=") + Name(2) + Text("?") );

    M.addDisabled( Text("Is") + Name(1) + "contained in" + Name(2) + "?" );

    M.addDisabled( Text("Is") + Name(2) + "contained in" + Name(1) + "?" );

    M.addDisabled( Text("Is an automorphic image of") + Name(1) 
		   + "contained in" + Name(2) + "?"
		   );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "contained in" + Name(1) + "?"
		   );

    M.addDisabled( Text("Is") + Name(1) 
		   + "a free factor of" + Name(2) + "?"
		   );

    M.addDisabled( Text("Is") + Name(2) 
		   + "a free factor of" + Name(1) + "?"
		   );
    
    M.addDisabled( Text("Is the intersection of") + Name(1) + "and" + Name(2) 
		   + "finitely generated ?"
		   );
    
    M.closeCascade();


    M.startItemGroup( Parent( 1 ) == Parent( 2 ) &&
		      CheckinType( Parent(1) ) == SMFPGroup::FREE
		      );
    
    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor2<SGOfFreeAreEqual,SMSubgroup,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?")
	   );

    M.add( Ctor2<SGOfFreeContainment,SMSubgroup,SMSubgroup>(true),
	   Text("Is") + Name(1) + "contained in" + Name(2) + "?"
	   );

    M.add( Ctor2<SGOfFreeContainment,SMSubgroup,SMSubgroup>(),
	   Text("Is") + Name(2) + "contained in" + Name(1) + "?"
	   );

    M.add( Ctor2<SGOfFreeContainsConjugate,SMSubgroup,SMSubgroup>(true),
	   Text("Is a conjugate of") + Name(1) + "contained in" + Name(2) + "?"
	   );

    M.add( Ctor2<SGOfFreeContainsConjugate,SMSubgroup,SMSubgroup>(),
	   Text("Is a conjugate of") + Name(2) + "contained in" + Name(1) + "?"
	   );

    M.addDisabled( Text("Is an automorphic image of") + Name(1) 
		   + "contained in" + Name(2) + "?"
		   );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "contained in" + Name(1) + "?"
		   );

    M.addDisabled( Text("Is") + Name(1) 
		   + "a free factor of" + Name(2) + "?"
		   );

    M.addDisabled( Text("Is") + Name(2) 
		   + "a free factor of" + Name(1) + "?"
		   );
    
    M.closeCascade();
    

    M.startCascade( Text("Compute ...") );
    
    M.addDisabled( Text("Compute the rank of the join of") + Name(1) + "and" + Name(2) );
 
    M.addDisabled( Text("Compute the rank of the intersection of") + Name(1) + "and" + Name(2) );
 
    M.closeCascade();

    M.startCascade( Text("Find ...") );

    M.add( Ctor2<SGOfFreeJoin,SMSubgroup,SMSubgroup>(),
	   Text("Find the join of") + Name(1) + "and" + Name(2) );

    M.add( Ctor2<SGOfFreeIntersection,SMSubgroup,SMSubgroup>(),
	   Text("Find the intersection of") + Name(1) + "and" + Name(2) );

    M.closeCascade();

    M.startCascade( Text("Are ...") );
    
    M.add( Ctor2<SGOfFreeConjugateTo,SMSubgroup,SMSubgroup>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "conjugate ?"
	   );

    M.closeCascade();


    M.startItemGroup( Parent( 1 ) == Parent( 2 ) && 
		      ( CheckinType( Parent(1) ) == SMFPGroup::FREE_NILPOTENT
		      || CheckinType( Parent(1) ) == SMFPGroup::NILPOTENT
			)
		      );
    

    M.startCascade( Text("Is") + Name(1) + Text("...") );
    
    M.add( Ctor2<SGOfNGequalSubgroupProblem,SMSubgroup,SMSubgroup>(),
	   Text("Is") + Name(1) + Text("=") + Name(2) + Text("?")
	   );

    M.add( Ctor2<SGOfNGcontainSubgroupProblem,SMSubgroup,SMSubgroup>(true),
	   Text("Is") + Name(1) + "contained in" + Name(2) + "?"
	   );

    M.add( Ctor2<SGOfNGcontainSubgroupProblem,SMSubgroup,SMSubgroup>(),
	   Text("Is") + Name(2) + "contained in" + Name(1) + "?"
	   );

    M.addDisabled( Text("Is an automorphic image of") + Name(1) 
		   + "contained in" + Name(2) + "?"
		   );

    M.addDisabled( Text("Is an automorphic image of") + Name(2) 
		   + "contained in" + Name(1) + "?"
		   );

    M.addDisabled( Text("Is") + Name(1) 
		   + "a free factor of" + Name(2) + "?"
		   );

    M.addDisabled( Text("Is") + Name(2) 
		   + "a free factor of" + Name(1) + "?"
		   );
    
    M.closeCascade();
    

    M.startCascade( Text("Compute ...") );
    
    M.addDisabled( 
      Text("Compute the minimal number of generators of the join of") 
      + Name(1) + "and" + Name(2) 
      );
 
    M.addDisabled( 
      Text("Compute the minimal number of generators of the intersection of") 
      + Name(1) + "and" + Name(2) 
      );
 
    M.closeCascade();


    M.startCascade( Text("Find ...") );
 
    M.add( Ctor2<SGOfNGjoinSubgroupProblem,SMSubgroup, SMSubgroup>(),
	   Text("Find a Malcev basis for the join of") 
	   + Name(1) + "and" + Name(2)
	   );

    M.addDisabled( Text("Find a Malcev basis for the intersection of") 
		   + Name(1) + "and" + Name(2)
		   );

    M.closeCascade();

    M.done();
  }

  {
    Menu2<SMFPGroup,SMFPGroup> M( Menu::TOOLS );

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::ABELIAN &&
		      CheckinType( 2 ) == SMFPGroup::ABELIAN
		      );

    M.add( Ctor2<AbelianIsomProblem,SMFPGroup,SMFPGroup>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "isomorphic ?"
	   );
   
    M.startItemGroup( ( CheckinType( 1 ) == SMFPGroup::NILPOTENT &&
			CheckinType( 2 ) == SMFPGroup::NILPOTENT ) ||
		      ( CheckinType( 1 ) == SMFPGroup::FP &&
			CheckinType( 2 ) == SMFPGroup::FP )
		      );
    
    M.add( Ctor2<NGIsomorphismProblem,SMFPGroup,SMFPGroup>(),
	   Text("Are") + Name(1) + "and" + Name(2) + "isomorphic ?"
	   );

    M.startItemGroup( ( CheckinType( 1 ) == SMFPGroup::FP ||
			CheckinType( 1 ) == SMFPGroup::FREE ) &&
		      ( CheckinType( 2 ) == SMFPGroup::FP ||
			CheckinType( 2 ) == SMFPGroup::FREE )
		      );
    
    M.add( Ctor2<TotalHomEnumeratorProblem,SMFPGroup,SMFPGroup>(),
	   Text("Totally enumerate homomorphisms from ") + Name(1) +
	   "to" + Name(2)
	   );
    
    M.add( Ctor2<TotalHomEnumeratorProblem,SMFPGroup,SMFPGroup>(true),
	   Text("Totally enumerate homomorphisms from ") + Name(2) +
	   "to" + Name(1)
	   );
    
    M.add( Ctor2<RandHomEnumeratorProblem,SMFPGroup,SMFPGroup>(),
	   Text("Randomly enumerate homomorphisms from ") + Name(1) +
	   "to" + Name(2)
	   );
    
    M.add( Ctor2<RandHomEnumeratorProblem,SMFPGroup,SMFPGroup>(true),
	   Text("Randomly enumerate homomorphisms from ") + Name(2) +
	   "to" + Name(1)
	   );

    M.done();
  }


  {
    Menu1< SMList<Word> > M( Menu::TOOLS );
    
    M.startItemGroup( );
    
    M.add( Ctor1<WordsOfLength, SMList<Word> >(),
	   Text("Extract words of a given length from ") + Name(1)
	   );	 
   
    M.add( Ctor1<SMListExtractTrivialWords, SMList<Word> >(),
	   Text("Extract trivial words from ") + Name(1)
	   );	  

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE );
    
    M.add( Ctor1<SMListExtractProperPowerInFree, SMList<Word> >(),
	   Text("Extract proper powers from ") + Name(1)
	   );	  
  
    M.add( Ctor1<SMListExtractCommutatorsInFree, SMList<Word> >(),
	   Text("Extract commutators from ") + Name(1)
	   );	  

    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FP );
    
    M.add( Ctor1<SMListExtractCentralWords, SMList<Word> >(),
	   Text("Extract central words from ") + Name(1)
	   );	    

    M.done();
  }

  {
    Menu1< SMList<Subgroup> > M( Menu::TOOLS );
    
    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE ||
		      CheckinType( Parent(1) ) == SMFPGroup::FP );
    
    M.add( Ctor1<SGListExtractOfIndex, SMList<Subgroup> >(),
	   Text("Extract subgroups of a given index from ") + Name(1)
	   );
    
    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE );
    
    M.add( Ctor1<SGListExtractNormal, SMList<Subgroup> >(),
	   Text("Extract normal subgroups from ") + Name(1)
	   );	 
    
    M.add( Ctor1<SGListExtractMalnormal, SMList<Subgroup> >(),
	   Text("Extract malnormal subgroups from ") + Name(1)
	   );	 
    
    M.add( Ctor1<SGListExtractOfRank, SMList<Subgroup> >(),
	   Text("Extract subgroups of a given rank from ") + Name(1)
	   );
    
    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FP );
    
    M.add( Ctor1<SMListExtractTrivialSubgroups, SMList<Subgroup> >(),
	   Text("Extract trivial subgroups from ") + Name(1)
	   );	  

     M.add( Ctor1<SMListExtractAbelianSubgroups, SMList<Subgroup> >(),
	   Text("Extract abelian subgroups from ") + Name(1)
	   );	    

     M.add( Ctor1<SMListExtractCentralSubgroups, SMList<Subgroup> >(),
	    Text("Extract central subgroups from ") + Name(1)
	    );	    
     
     M.done();
  }

 {
    Menu1< SMList<Map> > M( Menu::TOOLS );
    
    M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE );
    
    M.add( Ctor1<MapListExtractIAauto, SMList<Map> >(),
	   Text("Extract IA-automorphisms from ") + Name(1)
	   );
    
   
    M.add( Ctor1<MapListExtractInner, SMList<Map> >(),
	   Text("Extract inner automorphisms from ") + Name(1)
	   );

    M.done();
  }

  {
    Menu2< SMList<Word>, SMList<Word>  > M( Menu::TOOLS );
    
    
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) );
    
    M.add( Ctor2<SMListJoin<Word>, SMList<Word>, SMList<Word> >(),
	   Text("Compute the join of ") + Name(1) + Text(" and ") + Name(2)
	   );	 
    
    M.done();
  }

 {
    Menu2< SMList<Subgroup>, SMList<Subgroup>  > M( Menu::TOOLS );
    
    
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) );
    
    M.add( Ctor2<SMListJoin<Subgroup>, SMList<Subgroup>, SMList<Subgroup> >(),
	   Text("Compute the join of ") + Name(1) + Text(" and ") + Name(2)
	   );	 

    M.done();
  }

  {
    Menu2< SMList<Map>, SMList<Map>  > M( Menu::TOOLS );
    
    
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) );
    
    M.add( Ctor2<SMListJoin<Map>, SMList<Map>, SMList<Map> >(),
	   Text("Compute the join of ") + Name(1) + Text(" and ") + Name(2)
	   );	 
    
    M.done();
  }
 
 {
   Menu2<SMList<Word>,SMWord> M( Menu::TOOLS );
    
    
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) );
    
    M.add( Ctor2<IsWordInList, SMList<Word>,SMWord>(),
	   Text("Determine whether") + Name(2)
	   + Text("lies in") + Name(1)
	   );	 
    
    M.done();
  }
 {
    Menu2<SMList<Subgroup>,SMSubgroup> M( Menu::TOOLS );
    
    
    M.startItemGroup( Parent( 1 ) == Parent( 2 ) );
    
    M.add( Ctor2<IsSubgroupInList, SMList<Subgroup>,SMSubgroup>(),
	   Text("Determine whether") + Name(2)
	   + Text("lies in") + Name(1)
	   );	 
    
    M.done();
  }
 {
    Menu2<SMList<Map>,SMMap> M( Menu::TOOLS );
    
    
    M.startItemGroup(  Domain(2) == Parent( 1 ) );
    
    M.add( Ctor2<IsMapInList, SMList<Map>,SMMap>(),
	   Text("Determine whether") + Name(2)
	   + Text("lies in") + Name(1)
	   );	 
    
    M.done();
  }
 {
    Menu2<SMList<Map>,SMHomomorphism> M( Menu::TOOLS );
    
    
     M.startItemGroup(  Domain(2) == Parent( 1 ) );
    
    M.add( Ctor2<IsMapInList, SMList<Map>,SMHomomorphism>(),
	   Text("Determine whether") + Name(2)
	   + Text("lies in") + Name(1)
	   );	 
    
    M.done();
  }

}


// ----------------- ObjectSmith::outputMakeMenuDefns ---------------------- //


void ObjectSmith::outputMakeMenuDefns(ostream& ostr)
{
  /* @am 
  {
    Menu0 M( Menu::MAKE );
    
    
    M.startItemGroup();
    
    M.add( Ctor0<MakeRandomPresentation>( ),
	   Text("Random presentation")
	   );
    
    M.done();
  }
  */

 {
    Menu1<SMFPGroup> M( Menu::MAKE );
    


    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::ABELIAN );
    
    M.add( Ctor1<MakeCyclicDecomposition, SMFPGroup>( ),
	   Text("Canonical presentation of") + Name(1)
	   );


    
    M.startItemGroup();
    
    M.add( ReadMakeQuotientItem(),
	   Text("Quotient of") + Name(1),
	   DefineSetOfRelators()
	   );



    M.startItemGroup( CheckinType( 1 ) != SMFPGroup::ABELIAN );

    M.add( Ctor1<MakeAbelianQuotient, SMFPGroup>(),
	   Text("Abelian Quotient of") + Name(1)
	   );



    M.startItemGroup( CheckinType( 1 ) != SMFPGroup::ABELIAN
		      );
    
    M.add( ReadMakeNilpotentQuotientItem(),
	   Text("Nilpotent Quotient of") + Name(1),
	   DefineInt("Enter the nilpotency class: ", 1)
	   );


    
    M.startItemGroup();

    M.separator();

    /*@db
    M.addDisabled( Text("Another Presentation for") + Name(1)
		   );
    */

    M.add( Ctor1<TTProblem, SMFPGroup>(),
	   Text("Another Presentation of") + Name(1)
	   );

    M.add( Ctor1<MakeRipsConstruction, SMFPGroup>(),
	   Text("Rips construction of") + Name(1)
	   );
    
    
    M.done();
 }
  
 {
    Menu1<SMSubgroup> M( Menu::MAKE );

    M.startItemGroup();

    M.add( Ctor1<MakeQuotientFromSubgroup, SMSubgroup>(),
	   Text("Form the quotient group") + Name(Parent(1))+Text("/")+Name(1)
	   );
    
    M.done();
 }
 
 {
   Menu2<SMSubgroup, SMWord> M( Menu::MAKE );
   
   M.startItemGroup();
   
   M.add( Ctor2<SubgroupConjugateBy,SMSubgroup,SMWord>(),
	  Text(Name(1)) + Text("conjugated by") + Name(2)
	  );
   M.done();
 }

 {
   Menu2<SMSubgroup, SMSubgroup> M( Menu::MAKE );
   
   M.startItemGroup( CheckinType( Parent(1) ) == SMFPGroup::FREE &&
		     Parent( 1 ) == Parent( 2 )
		     ); 
   
   M.add( Ctor2<SGOfFreeJoin,SMSubgroup,SMSubgroup>(),
			      Text("The join of") + Name(1) + "and" + Name(2)
			      );
   
   M.add( Ctor2<SGOfFreeIntersection,SMSubgroup,SMSubgroup>(),
	  Text("The intersection of") + Name(1) + "and" + Name(2)
	  );
   
   M.startItemGroup( CheckinType( Parent(1) ) != SMFPGroup::FREE &&
		     Parent( 1 ) == Parent( 2 )
		     );
   
   M.add( Ctor2<SubgroupJoin,SMSubgroup,SMSubgroup>(),
	  Text("The join of") + Name(1) + "and" + Name(2)
	  ); 
   
   M.done();
 }
 {
   Menu4<SMFPGroup, SMFPGroup, SMSubgroup, SMSubgroup> M( Menu::MAKE );
   
   
   
   M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FREE &&
		     CheckinType( 2 ) == SMFPGroup::FREE &&
		     ( Parent(3) == Oid(1) || Parent(3) == Oid(2) ) &&
		     ( Parent(4) == Oid(1) || Parent(4) == Oid(2) ) &&
		     Parent(3) != Parent(4)
		     );
   
    M.add( Ctor4<MakeAPOfFree, SMFPGroup, SMFPGroup,
	   SMSubgroup, SMSubgroup> (),
	   Text("Amalgamated Product")
	   );

    
    
    M.done();
  }

  { 
    Menu3<SMFPGroup, SMSubgroup, SMSubgroup> M( Menu::MAKE );

    M.startItemGroup( CheckinType( 1 ) == SMFPGroup::FREE
		      && Parent(2) == Oid(1) && Parent(3) == Oid(1) 
		      );

    M.add( Ctor3<MakeHNNExtOfFreeGroup,SMFPGroup,SMSubgroup,SMSubgroup>(), 
	   Text("HNN-extension")
	   );

    M.done();
  }

  {
    Menu2<SMFPGroup, SMFPGroup> M( Menu::MAKE );
    
    M.startItemGroup( CheckinType( 1 ) != SMFPGroup::NILPOTENT &&
		      CheckinType( 2 ) != SMFPGroup::NILPOTENT &&
		      CheckinType( 1 ) != SMFPGroup::FREE_NILPOTENT &&
		      CheckinType( 2 ) != SMFPGroup::FREE_NILPOTENT
		      );

    M.add( Ctor2<MakeFreeProduct,SMFPGroup,SMFPGroup>(), 
	   Text("Make free product ")+Name(1)+Text("*")+Name(2)
	   );
    
    M.add( Ctor2<MakeDirectProduct,SMFPGroup,SMFPGroup>(), 
	   Text("Make direct product")+Name(1)+Text("X")+Name(2)
	   );
    
    M.done();
    
  }

  {
    Menu1< EnumeratorProblem< Word > > M( Menu::MAKE );
    
    M.startItemGroup( );
    ostrstream o;
    o << "Make list of words from \\\"" 
      << Name(1) 
      << "\\\"" << ends;
  
    M.add( Ctor1<MakeSMListOf<Word>,EnumeratorProblem< Word > >(), 
	   Text( o.str() )
	   );
    
    M.done();
    
  }
  {
    Menu1< EnumeratorProblem< Subgroup > > M( Menu::MAKE );
    
    M.startItemGroup( );
    
    ostrstream o;
    o << "Make list of subgroups from \\\"" 
      << Name(1) 
      << "\\\"" << ends;
    M.add( Ctor1<MakeSMListOf<Subgroup>,EnumeratorProblem< Subgroup > >(), 
	   Text(o.str())
	   );
    
    M.done();
    
  }
  {
    Menu1< EnumeratorProblem< Map > > M( Menu::MAKE );

    M.startItemGroup( );
    
    ostrstream o;
    o << "Make list of maps from \\\"" 
      << Name(1) 
      << "\\\"" << ends;
    M.add( Ctor1<MakeSMListOf<Map>,EnumeratorProblem< Map > >(), 
	   Text(o.str())
	   );
    
    M.done();
    
  }
  /* @db buggy !
  {
    Menu2<SMFPGroup, SMSubgroup> M( Menu::MAKE );
    
    M.startItemGroup( ( CheckinType( 1 ) == SMFPGroup::FREE
			|| CheckinType( 1 ) == SMFPGroup::FP
			|| CheckinType( 1 ) == SMFPGroup::ABELIAN 
			|| CheckinType( 1 ) == SMFPGroup::AP_FREE
			|| CheckinType( 1 ) == SMFPGroup::AP_FREE_CYCLIC
			|| CheckinType( 1 ) == SMFPGroup::SMALL_CANCELLATION
			|| CheckinType( 1 ) == SMFPGroup::ONE_RELATOR 
			|| CheckinType( 1 ) == SMFPGroup::ONE_RELATOR_WITH_TORSION
			|| CheckinType( 1 ) == SMFPGroup::HNN_FREE
		      )
		      && Parent(2) == Oid(1) );

    M.add( Ctor2<MakeFactorGroup,SMFPGroup,SMSubgroup>(), 
	   Text("Make a factor group ")+Name(1)+Text("/")+Name(2)
	   );

    M.done();
    
  }
  */
}


// --------------- ObjectSmith::outputTestingMenuDefns -------------------- //


void ObjectSmith::outputTestingMenuDefns(ostream& ostr)
{
  {
    Menu0 M( Menu::TESTING );

    M.startItemGroup( );
    
    M.addDisabled( Text("Nothing") );

    M.done();
  }
}

#line 1  "src/QuadEquationSolver.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes EquationProblem,
//           EquationSolver, QuickEquationSolver, EquationRandomSolutions
//
// Principal Author: Eugene Paderin, Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//

#include "QuadEquationSolver.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "QEqnSolutions.h"
#include "SolutionsEnum.h"


// ---------------------- EquationBasicSolutions ------------------------- //

void EquationSolverARCer::setArguments( SMEquation& equation, 
  const Chars& aBasicFileName, const Chars& aStabFileName,
  bool buildBasicSolutions,   bool buildStabilizer )
{
  F = equation.getFreeGroup();
  word = equation.getWord();
  numberOfVariables = equation.numberOfVariables();
  workOnBasicSolutions = buildBasicSolutions;
  workOnStabilizer = buildStabilizer;
  basicFileName = aBasicFileName;
  stabFileName  = aStabFileName;

}

void EquationSolverARCer::runComputation( )
{
  File basicFile( basicFileName );
  File stabFile ( stabFileName  );

  QEqnSolutionsInFreeGroup solver( F, word, numberOfVariables );
  
  solver.startComputation();
  
  if( !workOnBasicSolutions )
    solver.disableBuildingSolutions();
  
  if( !workOnStabilizer )
   solver.disableBuildingRegStab();
  
  do { 
    
    solver.continueComputation();
    
    while ( solver.haveNewSolution() ) {
      basicFile << solver.getSolution() << endl << ready << endl;
      solver.nextSolution();
    }
    
    while( solver.haveNewStabGenerator() ) {
      stabFile << solver.getStabGenerator() << endl << ready << endl;
      solver.nextStabGenerator();
    }
    
  } while ( ! solver.isComputationDone() );

  basicFile << end;
  stabFile << end;

  // Save summary results of computation to return the parent through IPC.
  numOfBasicSolutions    = solver.basicSolutions().length();
  numOfRegStabGenerators = solver.regStabGenerators().length();
}

void EquationSolverARCer::writeResults( ostream& ostr ) 
{
  ostr < numOfBasicSolutions < ' ' < numOfRegStabGenerators; 
}

void EquationSolverARCer::readResults( istream& istr ) 
{
  istr > numOfBasicSolutions > numOfRegStabGenerators;
}

int EquationSolverARCer::numberOfBasicSolutions( ) const 
{ 
  return numOfBasicSolutions; 
}

int EquationSolverARCer::numberOfRegStabGenerators( ) const 
{ 
  return numOfRegStabGenerators; 
}

// -------------------- EquationSolver ----------------------------------- //


EquationSolver::EquationSolver( EquationProblem& equationProblem )
  : equation( equationProblem.getEquation() ), arcer( *this ),
    workOnBasicSolutions( true ), workOnStabilizer( true ),
    basicFileName( equationProblem.getBasicSolutionsFileName() ), 
    stabFileName( equationProblem.getRegStabGeneratorsFileName() ),
    watchBasicSolutions( equationProblem.getBasicSolutionsFileName(), 2000 ),
    watchRegStabGenerators( equationProblem.getRegStabGeneratorsFileName(), 2000 )
{ 
  if( equation.eic.haveAllBasicSolutions()  )
    workOnBasicSolutions = false;
  if( equation.eic.haveAllRegStabGenerators() )
    workOnStabilizer = false;

  arcer.setArguments( equation, basicFileName, stabFileName,
		      workOnBasicSolutions, workOnStabilizer );
  adminStart();
}


EquationSolver::EquationSolver( QuickEquationProblem& equationProblem )
  : equation( equationProblem.getEquation() ), arcer( *this ),
    workOnBasicSolutions( true ), workOnStabilizer( false ),
    basicFileName( equationProblem.getBasicSolutionsFileName() ), 
    stabFileName( File().getFileName(/*dummy file name*/) ), 
    watchBasicSolutions( equationProblem.getBasicSolutionsFileName(), 2000 ),
    watchRegStabGenerators( stabFileName, 2000 )
{ 
  if( equation.eic.haveAllBasicSolutions()  )
    workOnBasicSolutions = false;

  arcer.setArguments( equation, basicFileName, stabFileName,
		      workOnBasicSolutions, workOnStabilizer );

  adminStart();
}


void EquationSolver::terminate( ) 
{
  if( foundBasicSolutions.length() > 0 )
    equation.eic.addBasicSolutions( foundBasicSolutions );
  if( foundRegStabGenerators.length() > 0 )
    equation.eic.addRegStabGenerators( foundRegStabGenerators );
}

void EquationSolver::updateBasicSolutions( ) 
{
  Endomorphism bsol( equation.getFreeGroup() );
  Chars fname = File().getFileName();
  fstream f( fname, ios::in | ios::out ); 

  while( watchBasicSolutions.changed() ) {
    Chars str = watchBasicSolutions.getline();

    if( str.length() == 0 ) continue;

    if( str[0] != '\n' ) {
      str = Chars("{ ") + str + Chars(" }");
      f.seekp( 0, ios::beg );
      f << str << endl << flush;
      f.seekg( 0, ios::beg );
      Chars errMesg = f >> bsol;
      if( errMesg.length() > 0 ) {
	error("Bad endomorphism on input in "
	      "EquationProblem::updateBasicSolutions()");
      }
      foundBasicSolutions.append( bsol );
    }   
  }

  if( foundBasicSolutions.length() > 10 ) {
    equation.eic.addBasicSolutions( foundBasicSolutions );
    foundBasicSolutions = ListOf<Endomorphism>();
  }

  f.close();
  unlink( fname );
}


void EquationSolver::updateRegStabGenerators( ) 
{
  Automorphism stabgen( equation.getFreeGroup() );
  Chars fname = File().getFileName();
  fstream f( fname, ios::in | ios::out ); 

  while( watchRegStabGenerators.changed() ) {
    Chars str = watchRegStabGenerators.getline();

    if( str[0] != '\n' ) {
      str = Chars("{ ") + str + Chars(" } ");
      f.seekp( 0, ios::beg );
      f << str << endl << flush;
      f.seekg( 0, ios::beg );
      Chars errMesg = f >> stabgen;
      if( errMesg.length() > 0 ) {
	error("Bad automorphism on input in "
	      "EquationProblem::updateRegStabGenerators()");
      }
      foundRegStabGenerators.append( stabgen );
    }   
  }
  
  if( foundRegStabGenerators.length() > 50 ) {
    equation.eic.addRegStabGenerators( foundRegStabGenerators );
    foundRegStabGenerators = ListOf<Automorphism>();
  }

  f.close();
  unlink( fname );
}

void EquationSolver::takeControl( )
{
  if( freeARCs() > 0 ) {

    bool arcerDone = arcer.takeControl();

    if( arcerDone ) {
      watchBasicSolutions.setInterval( 0 );
      watchRegStabGenerators.setInterval( 0 ); 

      #ifdef SYSV
        sleep(1); // @db: don't have usleep under Solaris
      #else    
	usleep(20);
      #endif   
    }
    
    if( workOnBasicSolutions ) 
      updateBasicSolutions();  
    
    if( workOnStabilizer ) 
      updateRegStabGenerators();

    if( arcerDone ) {
      LogMessage msg( *this, equation );
      msg << "Solving the equation is finished. Found " 
	  << arcer.numberOfBasicSolutions() << " basic solutions and " 
	  << arcer.numberOfRegStabGenerators() << " RegStab generators.";
      msg.send();

      if( workOnBasicSolutions ) 
	equation.eic.putHaveAllBasicSolutions( true );
      
      if( workOnStabilizer )
	equation.eic.putHaveAllRegStabGenerators( true );
	
      //LogMessage msg2( *this, theEquation );
      //msg2 << "Rank of the equation is " << theEquation.solver().rank();
      //msg2.send();
      
      adminTerminate();
      return;
    }

    usedOneARC();
  }

}

// -------------------- Random Solutions ---------------------------------- //

void EquationRandomSolutionARCer::setArguments( SMEquation& eq, 
						const Chars& aRandomFileName,
						const Chars& aBasicFileName,
						const Chars& aStabFileName )
{
  equation       = &eq;
  randomFileName = aRandomFileName;
  basicFileName  = aBasicFileName;
  stabFileName   = aStabFileName;
}

void EquationRandomSolutionARCer::runComputation( )
{ 
  // debugging trick.
  //bool f = true;
  //while( f ) 
  //;

  VectorPtrOf<Endomorphism> basicSolutions( equation->eic.getBasicSolutions().length(), true );
  {
    int i = 0;
    for( ListIterator< ListOf<Endomorphism> > I = equation->eic.getBasicSolutions();
	 !I.done(); I.next() ) {
      basicSolutions[i++] = I.value();
    }
  }

  VectorPtrOf<Automorphism> regStabGenerators( equation->eic.getRegStabGenerators().length(), true );
  { 
    int i = 0; 
    for( ListIterator< ListOf<Automorphism> > I = equation->eic.getRegStabGenerators();
	 !I.done(); I.next() ) {
      regStabGenerators[i++] = I.value();
    }
  }

  bool isWatchBasicSolutions = !equation->eic.haveAllBasicSolutions();
  bool isWatchRegStabGenerators = !equation->eic.haveAllRegStabGenerators();
  
  // Setup watchers for new basic solutions and regStab generators.
  const int interval = 2000; // Check input files for changes every 2 sec.
  LogFileWatcher watchBasicSolutions( basicFileName, interval );
  LogFileWatcher watchRegStabGenerators( stabFileName, interval );
  Endomorphism bsol( equation->getFreeGroup() );
  Automorphism stabGen( equation->getFreeGroup() );

  // Setup generator of random solutions.
  GeneratorOfRandomSolutions generator( equation->getFreeGroup(), equation->getWord(),
					equation->numberOfVariables() );
  generator.setBasicSolutions( basicSolutions );
  generator.setRegStabGenerators( regStabGenerators );

  File randomFile( randomFileName );
  fstream basicSolutionsFile( File().getFileName(), ios::in | ios::out );
  fstream regStabGensFile( File().getFileName(), ios::in | ios::out );

  if( !basicSolutionsFile || !regStabGensFile )
    error("Can't create temporary files in EquationRandomSolutionARCer::runComputation()");

  do { 

    // Update basic solutions.

    if( isWatchBasicSolutions ) {
      bool newBasicSolutions = false;
      while( watchBasicSolutions.changed() ) {
	Chars str = watchBasicSolutions.getline();
	if( str[0] != '\n' ) {
	  basicSolutionsFile.seekp( 0, ios::beg );
	  basicSolutionsFile << "{ " << str << " } " << endl;
	  basicSolutionsFile.seekg( 0, ios::beg );
	  Chars errMesg = basicSolutionsFile >> bsol;
	  if( errMesg.length() > 0 )
	    error("Bad endomorphism on input in EquationRandomSolutionsARCer::runComputation");
	  basicSolutions.append( bsol );
	  newBasicSolutions = true;
	}
      }   
      if( newBasicSolutions )
	generator.setBasicSolutions( basicSolutions );
    }

    // Update RegStab generators.

    if( isWatchRegStabGenerators ) {
      bool newRegStabGenerators = false;
      while( watchRegStabGenerators.changed() ) {
	Chars str = watchRegStabGenerators.getline();
	if( str[0] != '\n' ) {
	  regStabGensFile.seekp( 0, ios::beg );
	  regStabGensFile << "{ " << str << " } " << ends;
	  regStabGensFile.seekg( 0, ios::beg );
	  Chars errMesg = regStabGensFile >> stabGen;
	  if( errMesg.length() > 0 )
	    error("Bad automorphism on input "
		  "in EquationRandomSolutionsARCer::runComputation");
	  regStabGenerators.append( stabGen );
	  newRegStabGenerators = true;
	}   
      }   
      if( newRegStabGenerators )
	generator.setRegStabGenerators( regStabGenerators );
    }

    // Generate random solutions.

    for( int i = 0; i < 100; ++i ) {
      if( generator.hasSolutions() && generator.generateSolution() ) {
	Endomorphism sol = generator.getSolution();
	randomFile << sol << endl << ready << endl;
      }
    }

  } while ( 1 );

  randomFile << end;
}


EquationRandomSolutions::EquationRandomSolutions( EquationProblem& equationProblem )
  : arcer( *this )
{
  arcer.setArguments( equationProblem.getEquation(), 
		      equationProblem.getRandomSolutionsFileName(),
		      equationProblem.getBasicSolutionsFileName(),
		      equationProblem.getRegStabGeneratorsFileName() );
  adminStart();
}

void EquationRandomSolutions::takeControl( )
{
  if( freeARCs() > 0 ) {

    if( arcer.takeControl() ) {
	adminTerminate();
	return;
    }
    usedOneARC();
  }
}



// -------------------- EquationProblem --------------------------- //

EquationProblem::EquationProblem( SMEquation& anEquation )
  : Supervisor( true ), equation( anEquation ), linksSent( false ),
    basicSolutionsFileName( File().getFileName() ),
    regStabGeneratorsFileName( File().getFileName() ),
    randomSolutionsFileName( File().getFileName() ),
    basicSubordinate( *this ), randomSubordinate( *this )
{
}

void EquationProblem::takeControl( )
{
  if( !linksSent ) {
    
    if( !equation.eic.haveAllBasicSolutions() ) {
      LogMessage msg( *this, equation );
      msg << Link( "Click here to view basic solutions found so far.",
		   "EquationBasicSolutions", basicSolutionsFileName, true
		   );
      msg.send();
    }
    
    if( !equation.eic.haveAllRegStabGenerators() ) { 
      LogMessage msg( *this, equation );
      msg << Link( "Click here to view RegStab generators found so far.",
		   "EquationRegStabGenerators", regStabGeneratorsFileName, true
		   );
      msg.send();
    }
    
    LogMessage msg( *this, equation );
    msg << Link( "Click here to view random solutions generated so far.",
		 "EquationRandomSolutions", randomSolutionsFileName, true
		 );
    msg.send();

    linksSent = true;
  }

}



void EquationProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Solving a quadratic equation"),
		  Text("Find basic solutions and stabilizer of equation ")
		  + Name( equation ),
		  helpID("EquationProblem", equation.getParent()),
		  Text("Solving ") + Name( equation )
		  );

  pv.startItemGroup();

  if( !equation.eic.isSolved() )
    pv.add(Text("To find basic solutions"), basicSubordinate.arcSlotID(), 100);
  
  pv.add(Text("To generate random solutions"), randomSubordinate.arcSlotID(), 100);
  
  pv.done();
}



// -------------------- QuickEquationProblem --------------------------- //

QuickEquationProblem::QuickEquationProblem( SMEquation& anEquation )
  : Supervisor( true ), equation( anEquation ), linkSent( false ),
    basicSolutionsFileName( File().getFileName() ),
    basicSubordinate( *this )
{
  if( equation.eic.haveAllBasicSolutions() ) {
    resultIsFast();
  }
}

void QuickEquationProblem::takeControl( )
{
  if( equation.eic.haveAllBasicSolutions() ) {
    if( !fastResult() )
      adminTerminate();
  }
  else if( !linkSent ) {    
    LogMessage msg( *this, equation );
    msg << Link( "Click here to view basic solutions found so far.",
		 "EquationBasicSolutions", basicSolutionsFileName, true
		 );
    msg.send();
    linkSent = true;
  }
}



void QuickEquationProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Quick solving a quadratic equation"),
		  Text("Find basic solutions of equation ")
		  + Name( equation ),
		  helpID("QuickEquationProblem", equation.getParent()),
		  Text("Quick solving ") + Name( equation )
		  );

  pv.startItemGroup();

  pv.add(Text("To use for this problem"), basicSubordinate.arcSlotID(), 100);

  pv.done();
}



void QEquationInFreeBasicSolutions::takeControl( )
{
  LogMessage msg( equation );

  File file;
  file.setColor( titleColor );
  file << "The basic solutions of the equation in free group." << endl << endl;
  if( equation.eic.haveAllBasicSolutions() )
    file << "(complete list)";
  else
    file << "(incomplete list)";
  file << endl;
  file.setColor( mainColor );

  ListIterator< ListOf<Endomorphism> > basicSolutions = equation.eic.getBasicSolutions();
  for( int i = 0 ; !basicSolutions.done(); basicSolutions.next() ) {
    file << ++i << ". " << basicSolutions.value() << endl;
  }

  file.setColor( titleColor );
  file << "(end of the list)" << endl;
  file << end;
  
  msg << Link( Chars("Click here to see the basic solutions of ")
	       + Text( Name(equation) ),
	       "KnownBasicSolutions",
	       file.getFileName() 
	       )
      << ".";

  msg.send();
}


void QEquationInFreeRegStabGenerators::takeControl( )
{
  LogMessage msg( equation );

  File file;
  file.setColor( titleColor );
  file << "The basic solutions of the equation in free group." << endl << endl;
  if( equation.eic.haveAllRegStabGenerators() )
    file << "(complete list)";
  else
    file << "(incomplete list)";
  file << endl;
  file.setColor( mainColor );

  ListIterator< ListOf<Automorphism> > regStabGenerators = equation.eic.getRegStabGenerators();
  for( int i = 0; !regStabGenerators.done(); regStabGenerators.next() ) {
    file << ++i << ". " << regStabGenerators.value() << endl;
  }

  file.setColor( titleColor );
  file << "(end of the list)" << endl;
  file << end;
  
  msg << Link( Chars("Click here to see the generators of RegStab of ")
	       + Text( Name(equation) ),
	       "KnownRegStabGenerators",
	       file.getFileName() 
	       )
      << ".";
  
  msg.send();
}

#line 1  "src/HomologyProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class HomologyProblem
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "HomologyProblem.h"
#include "Homology.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"


HomologyARCer::HomologyARCer( ComputationManager& boss )
  : ARCer( boss ), kbmachine( 0 )
{
}

HomologyARCer::~HomologyARCer( )
{
  delete kbmachine;
}

void HomologyARCer::setArguments( const KBMachine& M, int startDimension, 
				  int endDimension )
{
  kbmachine = new KBMachine( M );
  startdim = startDimension;
  enddim = endDimension;
}

void HomologyARCer::runComputation( )
{
  Homology theHomology( *kbmachine , startdim, enddim );
  while( !theHomology.workOnNextGroup() );
  torsionFreeRank   = theHomology.getTorsionFreeRank();
  torsionInvariants = theHomology.getTorsionInvariants();
}

void HomologyARCer::writeResults( ostream& ostr )
{
  ostr < torsionFreeRank;
  ostr < torsionInvariants; 
}

void HomologyARCer::readResults( istream& istr )
{
  istr > torsionFreeRank;
  istr > torsionInvariants;
}



//---------------------------------------------------------------------------//
//--------------------------- HomologyProblem -------------------------------//
//---------------------------------------------------------------------------//


HomologyProblem::HomologyProblem( const HomologySupervisor& homology )
  : theGroup( homology.group() ), theDimension( homology.dimension() ),
    homologyArcer( *this ), arcerStarted( false ), solved( false )
{
  if ( ! theGroup.gic.haveConfluentKBMachine() ) {
    LogMessage msg( *this, theGroup );
    msg << "The system requires a confluent rewriting system for " 
	<< Name(theGroup) << " before it can start to compute its homology.";
    msg.send();
  }
  adminStart();
}


void HomologyProblem::takeControl( )
{
  if( theGroup.gic.haveConfluentKBMachine() && freeARCs() > 0 ) {
    
    if( !arcerStarted ) {
      homologyArcer.setArguments( theGroup.gic.getConfluentKBMachine(), 
				  theDimension, theDimension );
      arcerStarted = true;
      usedOneARC();
    }
    else {
      // homologyArcer.runComputation();
      if( homologyArcer.takeControl() ) {
	
	LogMessage msg( *this, theGroup );
	msg << "Dimension " << theDimension << " integral homology: Z^"
	    << homologyArcer.getTorsionFreeRank();
	
	VectorOf<Integer> invariants = homologyArcer.getTorsionInvariants();
	int len = invariants.length();
	for( int i = 0; i < len; ++i )
	  msg << " x Z_" << invariants[i];
	
	msg << ".";
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
//------------------------ HomologySupervisor -------------------------------//
//---------------------------------------------------------------------------//

HomologySupervisor::HomologySupervisor(SMFPGroup& G, int d)
  : Supervisor( true ), theGroup( G ), theDimension( d ), 
    homology( *this ), kbSupervisor( *this, G.gcm().kbSupervisor )
{
}

void HomologySupervisor::takeControl( ) 
{
  if( homology->isSolved() )
    adminTerminate();
}

void HomologySupervisor::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Integral homology"),
		  Text("Compute the dimension") + theDimension + "homology of"
		  + Name( theGroup ),
		  helpID("HomologyProblem", theGroup),
		  "Homology"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), homology.arcSlotID(), 50);

  //@rn Dodge updating the FE database for now:

  if( ! theGroup.gic.haveConfluentKBMachine() ) {
    
    pv.add(Text("Seek a rewriting system for") + Name( theGroup ),
	   kbSupervisor.arcSlotID(),
	   50
	   );
  }

  pv.done();
}

#line 1  "src/FreeProblems.C"
/*
 *   $Id: 
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes AutEnumerator, FiniteAutEnumerator
//
// Principal Author: Roger Needham, Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "FreeProblems.h"
#include "SMFPGroup.h"
#include "SMSubgroup.h"
#include "SMMap.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "conversions.h"
#include <unistd.h>

// ---------------------- AutoInFreeIsFinitARCer --------------------------- //

void AutoInFreeIsFinitARCer::runComputation( )
{
  RandomAutoInFree tempF(theGroup, 1);
  int order;
  retValue = tempF.isFiniteAutomorphism(*theMap,order);
  theOrder = order;
}


void AutoInFreeIsFinitARCer::setArguments( const Map& map,const FreeGroup& group )
{
 theGroup = group;
 theMap = new Map( map );
}


void AutoInFreeIsFinitARCer::writeResults( ostream& out ) 
{
  out < retValue < theOrder;
}

void AutoInFreeIsFinitARCer::readResults( istream& in )
{
  in > retValue > theOrder;
}
//---------------------------------------------------------------------------//
//-------------------- AutoInFreeIsFinit ---------- -------------------------//
//---------------------------------------------------------------------------//


AutoInFreeIsFinite::AutoInFreeIsFinite(const SMMap& hom)
  : Supervisor( true ),
    theHom( hom ),
    arcer( *this )
{
 arcer.setArguments( theHom.getMap(),theHom.getDomain().getFreePreimage()); 
}


void AutoInFreeIsFinite::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if (arcer.takeControl()){
      LogMessage msg( *this,theHom);
      Trichotomy answer = arcer.getRetValue();
      if (answer == dontknow){
	msg << "Can't solve problem is "<<Name(theHom)<<" finite.";
      }
      else{
	msg  << Name( theHom ) << " is";
	if (answer == no) msg << " not finite";
	else
	  msg << " finite, and it's order: " << arcer.getOrder() <<".";
      }
      msg.send();
      adminTerminate();
      return;
    }
    
    usedOneARC();
  }
}


void AutoInFreeIsFinite::start( ) { }


void AutoInFreeIsFinite::terminate( ) { }


void AutoInFreeIsFinite::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is finite automorphism"),
		  Text("Is") + Name( theHom ) + "of finite order? ",
		  helpID("AutoInFreeIsFinite", theHom.getDomain()),
		  "Is finite"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.done();
}

// ---------------------- SGOfFreeContainsConjugateARCer --------------------------- //

void SGOfFreeContainsConjugateARCer::runComputation( )
{
  retValue = theSubgroup->conjugateInSubgroup(  generators, conjugator );
}


void SGOfFreeContainsConjugateARCer::setArguments( const SGofFreeGroup& subgroup, 
					    const VectorOf<Word>& gens )
{
  theSubgroup = new SGofFreeGroup(subgroup);
  generators = makeSetOf(gens);
}


void SGOfFreeContainsConjugateARCer::writeResults( ostream& out ) 
{
  out < retValue;
  out < conjugator;
}

void SGOfFreeContainsConjugateARCer::readResults( istream& in )
{
  in > retValue;
  in > conjugator;
}
//---------------------------------------------------------------------------//
//-------------------- SGOfFreeContainsConjugate -------------------------//
//---------------------------------------------------------------------------//


SGOfFreeContainsConjugate::SGOfFreeContainsConjugate(const SMSubgroup& S1, const SMSubgroup& S2)
  : Supervisor( true ),
    subgroup1( S1 ),
    subgroup2( S2 ),
    arcer( *this )
{
 arcer.setArguments( subgroup1.getSubgroup(), subgroup2.getSubgroup().generators()); 
}


void SGOfFreeContainsConjugate::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if (arcer.takeControl()){
      LogMessage msg( *this,subgroup1);
      Word conjugator;
      if ( arcer.getRetValue( conjugator ))
	{
	  msg << Name( subgroup2 ) << " conjugated by ";
	  subgroup1.getParent().getFPGroup().printWord( msg, conjugator );
	}
      else {
	msg << "No conjugate of " << Name( subgroup2 );
      }
      
      msg << " is contained in " << Name( subgroup1 ) << ".";
      
      msg.send();
      adminTerminate();
      return;
    }
    
    usedOneARC();
  }
}


void SGOfFreeContainsConjugate::start( ) { }


void SGOfFreeContainsConjugate::terminate( ) { }


void SGOfFreeContainsConjugate::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Does contain a conjugate"),
		  Text("Does") + Name( subgroup1 ) + "contain a conjugate of"
		  + Name(subgroup2 ),
		  helpID("SGOfFreeContainsConjugate", subgroup1.getParent()),
		  "Cont conj"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.done();
}

// ---------------------- SGOfFreeConjugateToARCer --------------------------- //

void SGOfFreeConjugateToARCer::runComputation( )
{
  retValue = theSubgroup->conjugateTo( generators );
}


void SGOfFreeConjugateToARCer::setArguments( const SGofFreeGroup& subgroup, 
					    const VectorOf<Word>& gens )
{
  theSubgroup = new SGofFreeGroup(subgroup);
  generators = makeSetOf(gens);
}


void SGOfFreeConjugateToARCer::writeResults( ostream& out ) 
{
  out < retValue;
}

void SGOfFreeConjugateToARCer::readResults( istream& in )
{
  in > retValue;
}

//---------------------------------------------------------------------------//
//-------------------- SGOfFreeConjugateTo -------------------------//
//---------------------------------------------------------------------------//


SGOfFreeConjugateTo::SGOfFreeConjugateTo(const SMSubgroup& S1, const SMSubgroup& S2)
  : Supervisor( true ),
    subgroup1( S1 ),
    subgroup2( S2 ),
    arcer( *this )
{
 arcer.setArguments( subgroup1.getSubgroup(), subgroup2.getSubgroup().generators()); 
}


void SGOfFreeConjugateTo::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if (arcer.takeControl()){
      LogMessage msg( *this,subgroup1,subgroup2);
      msg << Name( subgroup1 ) << " and " << Name( subgroup2 ) << " are";
      if ( ! arcer.getRetValue())
	msg << " not";
      msg << " conjugate.";
      
      msg.send();
      adminTerminate();
      return;
    }
    
    usedOneARC();
  }
}


void SGOfFreeConjugateTo::start( ) { }


void SGOfFreeConjugateTo::terminate( ) { }


void SGOfFreeConjugateTo::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Are conjugate ?"),
		  Text("Are") + Name( subgroup1 ) + "and"
		  + Name(subgroup2 ) + "congugate",
		  helpID("SGOfFreeConjugateTo", subgroup1.getParent()),
		  "Are conjugate"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.done();
}


// ---------------------- AutEnumeratorARCer --------------------------- //


void AutEnumeratorARCer::setArguments( const FreeGroup& group, int avgNumbers, 
                                                                 bool isfinite)
{
  randAuto = new RandomAutoInFree( group, avgNumbers );
  isFinite = isfinite;
}


void AutEnumeratorARCer::runComputation( )
{
#if SAFETY > 0
  if( randAuto == 0 )
    error( "void AutEnumeratorARCer::runComputation( ) : "
	   "The enumerator has not been initialized yet");
#endif
  Map m;
  int order;
  int i = 1;
  while (true){
    if (!isFinite){
      m = randAuto->getAutomorphism();

      #ifdef SYSV
        sleep(1); // @db: don't have usleep under Solaris
      #else    
	usleep(30000);
      #endif   
    }
    else
      m = randAuto->getFiniteAutomorphism(order);
    file << "a" << i << " = ";
    m.printOn(file);
    if (isFinite){
      file.setColor(titleColor);
      file << endl << "The order of a" << i << ": " << order;
      file.setColor(mainColor);
    }
    file << ready << endl << endl;
    i++;
  }
}


void AutEnumeratorARCer::writeResults( ostream& out ) 
{

}


void AutEnumeratorARCer::readResults( istream& in )
{

}


Chars AutEnumeratorARCer::getFileName() 
{
  return file.getFileName();
}

//---------------------------------------------------------------------------//
//--------------------------- AutEnumerator ---------------------------------//
//---------------------------------------------------------------------------//


AutEnumerator::AutEnumerator(SMFPGroup& F, int avgNumGens, bool is_finite)
  : ComputationManager( true ),
	 theGroup( F ),
	 avgNumGens( avgNumGens ),
	 isFinite( is_finite ),
	 isStarted( false ),
         arcer(*this)
{ 
  LogMessage msg(*this,theGroup);
  if (theGroup.gic.isAbelian()==yes && theGroup.getFPGroup().getRelators().cardinality() > 0){
    Message msg;
    msg << " This group is not free abelian, can't built automorphisms.";
    msg.send();
    resultIsFast();
  }else
    if (theGroup.getFreePreimage().numberOfGenerators() < 2){
      LogMessage msg(*this,theGroup);
      if (theGroup.getFreePreimage().numberOfGenerators()==0)
	msg <<  Name(theGroup) << " is trivial, so it has only one automorphism: 1->1.";
      else
	{
	  Chars nameOfGen = theGroup.getFreePreimage().nameOfGenerator(0);
	  msg <<  Name(theGroup) << " with one generator, so it has only two automorphisms: " <<
	    nameOfGen << "->"<<nameOfGen <<" and "<<nameOfGen<<"->"<<nameOfGen<<"^-1.";
	}
      msg.send();
      resultIsFast();
    }
    else{
      arcer.setArguments(theGroup.getFreePreimage(),avgNumGens,isFinite);
      adminStart();
    }
}


AutEnumerator::~AutEnumerator( )
{

}


void AutEnumerator::start( )
{

}


void AutEnumerator::takeControl( )
{
  if (fastResult()){
    adminTerminate();
    return;
  }
  if ( freeARCs() == 0 ) {
    adminSuspend();
    return;
  }
  if (freeARCs() > 0){
    if (!isStarted){
      LogMessage msg(*this,theGroup);
      if (isFinite)
	msg << Link(Chars( "Click here to see the random automorphisms of finite order of ")+
		    Text( Name(theGroup) ),
		    "RandomAutomorphisms",
		    arcer.getFileName(), true);
      else
	msg << Link(Chars( "Click here to see the random automorphisms  of ")+
		    Text( Name(theGroup) ),
		    "RandomAutomorphisms",
		    arcer.getFileName(),true );
      msg.send();        
      isStarted = true;
    }
    
    if (arcer.takeControl()){
      adminTerminate();
      return;
    }
  usedOneARC();
  }
}


void AutEnumerator::terminate( )
{

}


const IconID AutEnumerator::iconID( ) const
{
  return IconID::enumerator_problem;
}


void AutEnumerator::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Enumerate automorphisms"),
		  Text("Randomly enumerate automorphisms of")
		  + Name( theGroup )
		  + ( isFinite ? "of finite order" : "" ),
		  helpID("AutEnumerator", theGroup),
		  "Enum aut"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this enumerator"), ThisARCSlotID(), 50);

  pv.done();
}
#line 1  "src/OrderProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class OrderProblem
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "OrderProblem.h"
#include "OutMessages.h"
#include "viewStructure.h"

// -------------------------- OrderProblem --------------------------------- //


OrderProblem::OrderProblem(SMFPGroup& G)
  : Supervisor( ! G.gic.haveMSC() ),
    theGroup( G ),
    kbSupervisor( *this, theGroup.gcm().kbSupervisor ),
    agSupervisor( *this, theGroup.gcm().agSupervisor ),
    computeBasis( *this, theGroup.gcm().computeBasis ),
    theToddCoxeter( *this, theGroup.gcm().theToddCoxeter),
    ghToddCoxeter( *this, theGroup.gcm().ghToddCoxeter)
{
  if ( theGroup.gic.haveOrder() ) resultIsFast();
}


void OrderProblem::takeControl( )
{
  
  // An OrderProblem merely allocates resources to parts of the GIC which
  // may discover the order as a byproduct of what they are doing.
  // Thus we have only to check whether our allocations have paid off.
  if (freeARCs() > 0){
      usedOneARC();
  }

  if ( theGroup.gic.haveOrder() ) {
    
    if( fastResult() )
      LogMessage(theGroup.gic.getOrderMessage()).send();
    else 
      LogMessage(*this, theGroup.gic.getOrderMessage(), 2).send();
    
    adminTerminate();
    return;
  }
  
}




void OrderProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Order problem"),
		  Text("Find the order of") + Name( theGroup ),
		  helpID("OrderProblem", theGroup),
		  Text("Order") + Name(theGroup)
		  );
/*@db  
  pv.startItemGroup();
  
  
  pv.add(Text("Determine whether the abelianization is infinite"),
	 abelianRank.arcSlotID(),
	 15
	 );
  
  pv.add(Text("Compute abelian invariants of") + Name( theGroup ),
	 abelianInvariants.arcSlotID(),
	 15
	 );
*/  


  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::FP ||
		     CheckinType( theGroup ) == SMFPGroup::NILPOTENT 
		     );
   pv.add(Text("Enumerate cosets")
	  + Name( theGroup ),
	  theToddCoxeter.arcSlotID(),
	  100
	  );   
   
   pv.add(Text("GH Todd-Coxeter")
	  + Name( theGroup ),
	  ghToddCoxeter.arcSlotID(),
	  100
	  );   

   pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::NILPOTENT );
   
   pv.add(Text("Compute a basis of ")
	  + Name( theGroup ),
	  computeBasis.arcSlotID(),
	  100
	  );
 
   pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::FP );
   
   pv.add(Text("Seek a rewriting system for") + Name( theGroup ),
	  kbSupervisor.arcSlotID(),
	  50
	  );
   
   
   pv.add(Text("Seek a ShortLex automatic structure for") + Name( theGroup ),
	  agSupervisor.arcSlotID(),
	  50
	  );
   
   pv.done();

}

#line 1  "src/ConjugacyProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class ConjugacyProblem
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//
//
// 11/19/95 db fixed:
//
// MSCConjugacyWrapper::MSCConjugacyWrapper(ConjugacyProblem& ),
//
// void MSCConjugacyWrapper::takeControl( ),
//
// FPConjugacyWrapper::FPConjugacyWrapper(ConjugacyProblem& CP),
//
// void FPConjugacyWrapper::takeControl( ).
//
// Now they correspond to the new interface of MSCGConjugacyProblem and
// GraphConjugacyProblem classes.
//
// @db & @au: 00/10/07 Added GA for solving conjugacy problem for OR groups  
//


#include "ConjugacyProblem.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "GraphConjugacyProblem.h"
#include "MSCGConjugacyProblem.h"


//---------------------------------------------------------------------------//
//------------------------- ConjugacyProblem --------------------------------//
//---------------------------------------------------------------------------//


ConjugacyProblem::ConjugacyProblem(const SMWord& w1, const SMWord& w2)
  : Supervisor( true ),
    u( w1 ),
    v( w2 ),
    scMethodFailed (false),
    fpConjugacyWrapper( *this ),
    mscConjugacyWrapper( *this ),
    GAConjugacy( *this ),
    geneticLinkHasBeenSent( false )
{ }


void ConjugacyProblem::takeControl( )
{
  if( fpConjugacyWrapper.exists() &&
      fpConjugacyWrapper->answer() != dontknow ) {
    adminTerminate();
  }

  if( !scMethodFailed ) {

    if( mscConjugacyWrapper.exists() 
	&& mscConjugacyWrapper->answer() != dontknow ) {

      if( !u.getParent().gic.haveMSC() 
	  && mscConjugacyWrapper->answer() == no ) {
	LogMessage msg( *this, u, v );
	msg <<
	  "The small cancellation method has failed; it says `no', "
	  "and the presentation of " << Name( u.getParent() ) <<
	  " is not known to the system to be small cancellation.";
	msg.send();
	scMethodFailed = true;
      }
      else 
	adminTerminate();
    }

  }

  if( GAConjugacy.exists() ) {

    if( !geneticLinkHasBeenSent ) {
      geneticLinkHasBeenSent = true;
      
      LogMessage msg( *this , u ,v );
      msg << Link("Click here to obtain details of the computation.",
		  "GACPforORG", GAConjugacy->getFileName(), true);
      msg.send();
    }
  
    if( GAConjugacy->answer() != dontknow ) {
      
      LogMessage msg( *this, u, v );
      
      if( GAConjugacy->answer() == no ) 
	msg << "Words " << Name(u) << " and " << Name(v) << " are not conjugate.";
      else 
	msg << "Words " << Name(u) << " and " << Name(v) << " are conjugate.";
      
      msg.send();
      adminTerminate();
    }
  }

}


void ConjugacyProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Conjugacy problem"),
		  Text("Are") + Name( u ) + "and" + Name( v )
		  + "conjugate in" + Name( Parent( u ) ) + "?",
		  helpID("ConjugacyProblem", u.getParent()),
		  "Conjugate ?"
		  );
  
  pv.startItemGroup();

  pv.add(Text("Try graph method"), fpConjugacyWrapper.arcSlotID(), 100);

  pv.add(Text("Try small cancellation method"), mscConjugacyWrapper.arcSlotID(), 60);

  pv.startItemGroup( CheckinType( Parent( u ) ) ==
		     SMFPGroup::ONE_RELATOR || 
		     CheckinType( Parent( u ) ) ==
		     SMFPGroup::ONE_RELATOR_WITH_TORSION
		     );

  pv.add(Text("Try genetic algorithm"), GAConjugacy.arcSlotID(), 60);

  pv.done();
}



//---------------------------------------------------------------------------//
//------------------------ FPConjugacyWrapper -------------------------------//
//---------------------------------------------------------------------------//

FPConjugacyARCer::FPConjugacyARCer( ComputationManager& boss )
  : ARCer( boss ), theGroup( 0 ), theAnswer( dontknow )
{  
}

void FPConjugacyARCer::setArguments( const class SMFPGroup& G,
				     const Word& u, const Word& w )
{ 
  theGroup = &G;
  firstWord = u;
  secondWord = w;
}

void FPConjugacyARCer::runComputation( )
{
  FPGroup G = theGroup->getFPGroup();
  GraphConjugacyProblem theGCP( G, firstWord, secondWord );
  SubgroupGraph normalClosure( theGroup->gcm().normalClosure->getSubgroupGraph() );
  Generator g(1);
  int numberOfGenerators = G.numberOfGenerators();

  theGCP.startComputation();

  while( !theGCP.done() ) {

    if( theGCP.theNewGraphIsNeeded() ) {

      if( normalClosure.isComplete() ) {
	// @dp
	theAnswer = no;
	return;
      }
      else { 
	// normalClosure.continueComputation();
	normalClosure.joinConjugate(g);
	int o = ord(g);
	if ( o > 0 )
	  o = -o;
	else
	  o = 1 - o;
	if ( o > numberOfGenerators ) o = 1;
	g = Generator(o);
	
	cerr << "The trivial word word acceptor has "
	     << normalClosure.vertexCount() << " vertices.";
	
	theGCP.continueComputation( normalClosure );
      }

    } 
    else {
      theGCP.continueComputation();
    }

  }
  
  // GraphConjugacyProblem finished with positive answer: 
  // the elements are conjugated.
  theAnswer = yes;
  theConjugator = theGCP.getConjugator();
}

void FPConjugacyARCer::writeResults( ostream& ostr )
{
  ostr < theAnswer;
  if( theAnswer == yes ) 
    ostr < theConjugator;
}

void FPConjugacyARCer::readResults( istream& istr )
{
  istr > theAnswer;
  if( theAnswer == yes )
    istr > theConjugator;
}


FPConjugacyWrapper::FPConjugacyWrapper(ConjugacyProblem& CP)
  : ComputationManager(false), 
    arcer( *this ), problem( CP ), theAnswer( dontknow )
{
  arcer.setArguments( CP.firstWord().getParent(),
		      CP.firstWord().getWord(),
		      CP.secondWord().getWord()
		      );
  adminStart();
}

void FPConjugacyWrapper::takeControl( )
{
  if ( freeARCs() > 0 ) {

    if( arcer.takeControl() ) {

      const SMWord& u = problem.firstWord();
      const SMWord& v = problem.secondWord();
      LogMessage msg( problem, u, v );
      msg << Name( u ) << " and " << Name( v ) << " are conjugate: " 
	  << "X " << Name( u ) << " x = " << Name( v ) << ", where x = ";
      u.getParent().getFPGroup().printWord( msg, arcer.conjugator() );
      msg << ".";
      msg.send();
      
      theAnswer = yes;
      adminTerminate();
      return;
    }
    usedOneARC();
  }
}

//---------------------------------------------------------------------------//
//------------------------ MSCConjugacyWrapper ------------------------------//
//---------------------------------------------------------------------------//

MSCConjugacyARCer::MSCConjugacyARCer( ComputationManager& boss )
  : ARCer( boss ), theAnswer( dontknow )
{
}

void MSCConjugacyARCer::setArguments( const FPGroup& G, const Word& u, const Word& v )
{
  theGroup = G;
  firstWord = u;
  secondWord = v;
}

void MSCConjugacyARCer::runComputation( )
{
  MSCGroup G( theGroup );
  MSCGConjugacyProblem theMSCCP( G, firstWord, secondWord );

  theMSCCP.startComputation( );
  while( !theMSCCP.continueComputation() ) {
    // do nothing: everything is done by `continueComputation'
  }

  theAnswer = theMSCCP.answer();
  if( theAnswer == yes ) 
    theConjugator = theMSCCP.conjugator();
}

void MSCConjugacyARCer::writeResults( ostream& ostr )
{
  ostr < theAnswer;
  if( theAnswer == yes ) 
    ostr < theConjugator;
}

void MSCConjugacyARCer::readResults( istream& istr )
{
  istr > theAnswer;
  if( theAnswer == yes )
    istr > theConjugator;
}


MSCConjugacyWrapper::MSCConjugacyWrapper(ConjugacyProblem& CP)
  : ComputationManager( ), 
    arcer( *this ), problem( CP ), theAnswer( dontknow )
{
  arcer.setArguments( CP.firstWord().getParent().getFPGroup(),
		      CP.firstWord().getWord(), 
		      CP.secondWord().getWord() 
		      );

  adminStart();
}

void MSCConjugacyWrapper::takeControl( )
{
  if ( freeARCs() > 0 ) {

    if( arcer.takeControl() ) {

      theAnswer = arcer.answer();
      const SMWord& u = problem.firstWord();
      const SMWord& v = problem.secondWord();

      if ( theAnswer == yes ) {
	LogMessage msg( problem, u, v );
	msg << Name( u ) << " and " << Name( v ) << " are conjugate: " 
	    << "X " << Name( u ) << " x = " << Name( v ) << ", where x = ";
	u.getParent().getFPGroup().printWord( msg, arcer.conjugator() );
	msg << ".";
	msg.send();
      } 
      else if ( u.getParent().gic.haveMSC() ) {
	LogMessage msg( problem, u, v );
	msg << Name( u ) << " and " << Name( v ) << " are not conjugate.";
	msg.send();
      } 

      adminTerminate();
    }

    usedOneARC();
  }

}


#line 1  "src/NilpotentProblems.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes NilpotentWP, NilpotentCenterProblem,
//           NilpotentCentralizersProblem,
//           NilpotentIsAbelianProblemARCer, NilpotentIsAbelianProblem,
//           NilpotentOrderProblemARCer, NilpotentOrderProblem,
//           NilpotentOrderOfEltProblemARCer, NilpotentOrderOfEltProblem,
//           NilpotentIsSGTrivialProblemARCer, NilpotentIsSGTrivialProblem,
//           NilpotentIsSGFiniteProblemARCer, NilpotentIsSGFiniteProblem,
//           NilpotentIsSGAbelianProblemARCer, NilpotentIsSGAbelianProblem
//
// Principal Author: Roger Needham, Dmitry Bormotov, Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//
// * 2/96 Dmitry B. added new classes:
//
//   NilpotentIsAbelianProblemARCer, NilpotentIsAbelianProblem,
//   NilpotentOrderProblemARCer, NilpotentOrderProblem,
//   NilpotentOrderOfEltProblemARCer, NilpotentOrderOfEltProblem,
//   NilpotentIsSGTrivialProblemARCer, NilpotentIsSGTrivialProblem,
//   NilpotentIsSGFiniteProblemARCer, NilpotentIsSGFiniteProblem,
//   NilpotentIsSGAbelianProblemARCer, NilpotentIsSGAbelianProblem
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "NilpotentProblems.h"
#include "SMFPGroup.h"
#include "SMWord.h"
#include "SMMap.h"
#include "SMSubgroup.h"
#include "SMHomomorphism.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "conversions.h"

// ------------------- NGOrderOfEltProblemARCer --------------------- //


void NGOrderOfEltProblemARCer::runComputation( )
{
  retValue = theNG->orderOf(theWord);
}


void NGOrderOfEltProblemARCer::setArguments( const NilpotentGroup& NG,
						    const Word& w )
{
  theNG = new NilpotentGroup(NG);
  theWord = w;
}


void NGOrderOfEltProblemARCer::writeResults( ostream& out ) 
{
  out < retValue;
}

void NGOrderOfEltProblemARCer::readResults( istream& in )
{
  in > retValue;
}

// ---------------------- NGorderOfEltProblem ----------------------- //


NGorderOfEltProblem::NGorderOfEltProblem(SMWord& w)
  : Supervisor( true ),
    theWord( w ),
    theGIC( w.getParent().gic ),
    computeBasis( *this, w.getParent().gcm().computeBasis ),
    started( false ),
    arcer( *this )
{ 
  if (theWord.wic.haveOrder())
    resultIsFast();
}


void NGorderOfEltProblem::takeControl( )
{
  if (!fastResult())
    if (freeARCs() > 0){
      if (theGIC.haveNilpotentGroupInited()){
	if (!started){
	  arcer.setArguments(theGIC.getNilpotentGroupInited(),
			     theWord.getWord());
	  started = true;
	}
	if (arcer.takeControl()){
	  theWord.wic.putHaveOrder(arcer.getOrder());
	}
      }
      usedOneARC();
    }
  if (theWord.wic.haveOrder()){
    LogMessage msg( *this, theWord );
    Integer theOrder = theWord.wic.getOrder();
    msg << "The order of " << Name( theWord ) << " is ";

    if( theOrder == 0 )
      msg << "infinite";
    else
      msg << theOrder;
    
    msg << ".";
    msg.send();
    adminTerminate();
  }
}


void NGorderOfEltProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the order"),
		  Text("Find the order of ") + Name( theWord ),
		  helpID("NGorderOfEltProblem", theWord.getParent()),
		  Text("order") 
		  );
  
  pv.startItemGroup();

  if (!theGIC.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name( theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    );
  pv.add(Text("Compute the order of ") + Name( theWord ), ThisARCSlotID(), 100);
  pv.done();
}




// ---------------------- NGHirschNumberProblem ----------------------- //


NGHirschNumberProblem::NGHirschNumberProblem(SMFPGroup& group)
  : Supervisor( true ),
    theGroup( group ),
    theGIC( group.gic ),
    computeBasis( *this, group.gcm().computeBasis )
{ 
  if (theGIC.haveNilpotentGroupInited())
    resultIsFast();
}


void NGHirschNumberProblem::takeControl( )
{
  if (theGIC.haveNilpotentGroupInited()){
    LogMessage msg( *this, theGroup );
    msg << Text("The Hirsch number of the group ") << Name(theGroup) 
	<< " is " << theGIC.getNilpotentGroupInited().theHirschNumber() << ".";
    msg.send();
    
    adminTerminate();
  }
}


void NGHirschNumberProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the Hirsch number"),
		  Text("Compute the Hirsch number of") + Name( theGroup ),
		  helpID("NGHirschNumberProblem", theGroup),
		  Text("Hir num") 
		  );
  
  pv.startItemGroup();
  
  pv.add( Text("Compute a basis of ")
	  + Name( theGroup ),
	  computeBasis.arcSlotID(),
	  100
	  );
  pv.done();
}



// ---------------------- NGcomputeNClassProblem ----------------------- //


NGcomputeNClassProblem::NGcomputeNClassProblem(SMFPGroup& group)
  : Supervisor( true ),
    theGroup( group ),
    theGIC( group.gic ),
    abelianInvariants( *this, group.gcm().abelianInvariants ),
    computeBasis( *this, group.gcm().computeBasis )
    //    computeLCSQuotientsComp( *this, group.gcm().computeLCSQuotientsComp )
{ 
  if (theGIC.haveCyclicDecomposition()) {
    const AbelianGroup& ag = theGIC.getCyclicDecomposition();
    if ((ag.rankOfFreeAbelianFactor() == 0 && ag.invariants().length() ==1) ||
	(ag.rankOfFreeAbelianFactor() == 1 && ag.invariants().length() ==0))
      theGIC.putActualNilpotencyClass(1);
  }
  if (theGIC.haveActualNilpotencyClass())
      resultIsFast();
}


void NGcomputeNClassProblem::takeControl( )
{
  if (theGIC.haveCyclicDecomposition()) {
    const AbelianGroup& ag = theGIC.getCyclicDecomposition();
    if ((ag.rankOfFreeAbelianFactor() == 0 && ag.invariants().length() ==1) ||
	(ag.rankOfFreeAbelianFactor() == 1 && ag.invariants().length() ==0))
      theGIC.putActualNilpotencyClass(1);
  }  
  if (theGIC.haveActualNilpotencyClass()){
      LogMessage msg( *this, theGroup);
      msg << Text("The nilpotency class of the group ") << Name(theGroup)  << " is " 
	  << theGIC.getActualNilpotencyClass() << ".";
      msg.send();
      adminTerminate();
  }
}


void NGcomputeNClassProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the nilpotency class"),
		  Text("Compute the nilpotency class of")
		  + Name( theGroup ),
		  helpID("NGcomputeNClassProblem", theGroup),
		  Text("class of ")+Name(theGroup)
		  );
  
  pv.startItemGroup();
  if (!theGIC.haveCyclicDecomposition())
    pv.add( Text("Compute abelianization of")
	    + Name( theGroup ),
	    abelianInvariants.arcSlotID(),
	    50
	    );  
  
  if (!theGIC.haveNilpotentQuotInited(theGIC.getNilpotentcyClass()))
    pv.add( Text("Compute a basis of ")
	    + Name( theGroup ),
	    computeBasis.arcSlotID(),
	    100
	    );
  
  /*  if (!theGIC.haveLCSQuotients())
    pv.add( Text("Compute the lower central factors of")
	    + Name( theGroup ),
	    computeLCSQuotientsComp.arcSlotID(),
	    100
	    );
	    */
  pv.done();
}
// ---------------------- NGisFreeNilpotentProblem ----------------------- //


NGisFreeNilpotentProblem::NGisFreeNilpotentProblem(SMFPGroup& group)
  : Supervisor( true ),
    theGroup( group ),
    theGIC( group.gic ),
    abelianInvariants( *this, group.gcm().abelianInvariants ),
    computeBasis( *this, group.gcm().computeBasis ),
    //    computeLCSQuotientsComp( *this, group.gcm().computeLCSQuotientsComp ),
    answer(dontknow)
{ 
  if (theGIC.haveNilpotentGroupInited()){
    answer = theGIC.getNilpotentGroupInited().
      isFreeNilpotent(&freeRank,&freeClass );
    resultIsFast();
    return;
  }
  if (theGIC.haveCyclicDecomposition())
    if (!theGIC.getCyclicDecomposition().isFree()){
      answer = no;
      resultIsFast();
    }
}


void NGisFreeNilpotentProblem::takeControl( )
{
  Chars explanation;
  if (!fastResult()){
    if (theGIC.haveNilpotentGroupInited())
      answer = theGIC.getNilpotentGroupInited().
	isFreeNilpotent(&freeRank,&freeClass );
    if (theGIC.haveCyclicDecomposition())
      if (!theGIC.getCyclicDecomposition().isFree()){
	explanation = Chars(": abelianization of ")+ Text(Name(theGroup))+ " is not free abelian group";
	answer = no;
      }
  }
  if (answer != dontknow){
    LogMessage msg( *this, theGroup );
    msg << "The group " << Name( theGroup ) << " is";
    if ( answer == no ){
      msg << " not free nilpotent";
      msg << explanation;
      msg << ".";
    }    
    else {
      if( freeRank == 0 && freeClass == 0 )
	msg << " trivial.";
      else
	msg << " free nilpotent of rank " << freeRank 
	    << " and class " << freeClass << ".";
    }
    msg.send();    
    adminTerminate();
  }
}


void NGisFreeNilpotentProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is the group free nilpotent?"),
		  Text("Is") + Name( theGroup ) + "free nilpotent ?",
		  helpID("NGisFreeNilpotentProblem", theGroup),
		  Text(Name(theGroup)) + "free nilp?" 
		  );
  
  pv.startItemGroup();
  
  if (!theGIC.haveNilpotentQuotInited(theGIC.getNilpotentcyClass()))
    pv.add( Text("Compute a basis of ")
	    + Name( theGroup ),
	    computeBasis.arcSlotID(),
	    100
	    );

  pv.add( Text("Compute abelianization of")
	  + Name( theGroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );  
  /*  pv.add( Text("Compute the lower central factors of")
	  + Name( theGroup ),
	  computeLCSQuotientsComp.arcSlotID(),
	  100
	  );
	  */
  pv.done();
}

// ---------------------- NGdecomposeWordProblem ----------------------- //


NGdecomposeWordProblem::NGdecomposeWordProblem(SMWord& word)
  : Supervisor( true ),
    theWord( word ),
    theWIC( word.wic ),
    computeBasis( *this,  word.getParent().gcm().computeBasis ),
    ngDecomposeWord(*this, word.wcm().ngDecomposeWord)
{ 
  if (theWIC.haveCollectedForm())
    resultIsFast();
}


void NGdecomposeWordProblem::takeControl( )
{

  if (theWIC.haveCollectedForm()){
    LogMessage msg1( *this, theWord );
    LogMessage msg2( *this, theWord );
    const NilpotentGroup& G = theWord.getParent().gic.getNilpotentGroupInited();
    if (theWord.getParent().gic.isFreeNilpotent() == yes){
      msg1 << "The canonical form of " << Name( theWord ) 
       << ", expressed as product of basic commutators: ";
      msg1 << G.asDecomposition(theWIC.getCollectedForm());
      msg2 << "The canonical form of " << Name( theWord ) 
	   << ", where the basic commutators are expressed directly "
	   << "in terms of the given generators: ";
      G.printWord(msg2,G.toWord(theWIC.getCollectedForm()));
      //      msg << G.toBracketedWordSL(theWIC.getCollectedForm());
    }
    else{
      msg1 << "Decomposition of " << Name( theWord ) << " in terms of basis: ";
      msg1 <<G.asDecomposition( theWIC.getCollectedForm() );
      msg2 << "The canonical form of " << Name( theWord ) 
	   << ", where the basic commutators are expressed directly "
	   << "in terms of the given generators: ";
      G.printWord(msg2,G.toWord(theWIC.getCollectedForm()));
    }
    msg1 << ".";
    msg2 << ".";
    msg1.send();    
    msg2.send();    
    adminTerminate();
  }
}



void NGdecomposeWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Decompose word in terms of a basis"),
		  Text("Decompose ") + Name( theWord ) + 
		  Text("in terms of a basis of ")+
		  Name(theWord.getParent()),
		  helpID("NGdecomposeWordProblem", theWord.getParent()),
		  Text("decomp") +Name(theWord) 
		  );
  
  pv.startItemGroup();
  
  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name( theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    );

  pv.add( Text("Decompose ")
	  + Name( theWord ),
	  ngDecomposeWord.arcSlotID(),
	  100
	  );
  pv.done();
}

// ---------------------- NGisWordInCommutatorSGProblem ----------------------- //


NGisWordInCommutatorSGProblem::NGisWordInCommutatorSGProblem(SMWord& word)
  : Supervisor( true ),
    theWord( word ),
    theGIC( word.getParent().gic ),
    abelianInvariants( *this, word.getParent().gcm().abelianInvariants )
{ 
  if (theGIC.haveCyclicDecomposition())
      resultIsFast();
}


void NGisWordInCommutatorSGProblem::takeControl( )
{
  if (theGIC.haveCyclicDecomposition()){
    LogMessage msg( *this,theWord );
    msg << Name( theWord ) << " is "; 
    if(!theGIC.getCyclicDecomposition().isTrivial(theWord.getWord()))  msg << "not ";
    msg << "in the commutator subgroup of " << Name( theWord.getParent() ) << 
      ": its image in the abelianization of "<< Name(theWord.getParent()) << " is ";
    if(!theGIC.getCyclicDecomposition().isTrivial(theWord.getWord()))  msg << "non-";
    msg << "trivial.";
    msg.send();
    adminTerminate();
  }
}


void NGisWordInCommutatorSGProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Decide if word is in the commutator subgroup"),
		  Text("Is") + Name(theWord) + Text("an element of the commutator subgroup in")
		  + Name( theWord.getParent() ),
		  helpID("NGisWordInCommutatorSGProblem", theWord.getParent()),
		  Text(Name(theWord)) + "in comm subg?" 
		  );
  
  pv.startItemGroup();
  
  pv.add( Text("Compute abelianization of")
	  + Name( theWord.getParent() ),
	  abelianInvariants.arcSlotID(),
	  50
	  );  
  pv.done();
}
// ------------------- NGweightOfWordARCer --------------------- //


void NGweightOfWordARCer::runComputation( )
{
  weight = theNG->weightOf(theWord);
}


void NGweightOfWordARCer::setArguments( const NilpotentGroup& NG,
					 const Word& w )
{
  theNG = new NilpotentGroup(NG);
  theWord = w;
}


void NGweightOfWordARCer::writeResults( ostream& out ) 
{
  out < weight;
}

void NGweightOfWordARCer::readResults( istream& in )
{
  in > weight;
}

// ---------------------- NGweightOfWordProblem ----------------------- //


NGweightOfWordProblem::NGweightOfWordProblem(SMWord& word)
  : Supervisor( true ),
    theWord( word ),
    theWIC( word.wic ),
    computeBasis( *this,  word.getParent().gcm().computeBasis ),
    arcer(*this),
    weight(-1),
    started(false)
{ 
  if (theWIC.haveCollectedForm())
    resultIsFast();
}


void NGweightOfWordProblem::takeControl( )
{
  if (fastResult()){
    weight = theWord.getParent().gic.getNilpotentGroupInited()
      .weightOf(theWIC.getCollectedForm());
  }
  else
    if (freeARCs() > 0){
      if (theWord.getParent().gic.haveNilpotentGroupInited()){
	if (!started){
	  arcer.setArguments(theWord.getParent().gic.getNilpotentGroupInited(),
			     theWord.getWord());
	  started = true;
	}
	if (arcer.takeControl() )
	  weight = arcer.getWeight();
      }
      usedOneARC();
    }
  
  if (weight >-1){
    LogMessage msg( *this, theWord );
    msg << Name( theWord ) << " lies in the " << weight << ordinalPostfix(weight) 
	<< " term of the lower central series: " 
	<<"this follows from an inspection of its canonical form.";
    msg.send();
    adminTerminate();
  }
}



void NGweightOfWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Term of the lower central series problem"),
		  Text("In which term of the lower central series does") +
		  Name( theWord ) + "lies.",
		  helpID("NGweightOfWordProblem", theWord.getParent()),
		  Text("Term of l.c.s. ?")
		  );
  
  pv.startItemGroup();
  
  if (!theWord.getParent().gic.haveNilpotentQuotInited
      (theWord.getParent().gic.getNilpotentcyClass()))
    pv.add( Text("Compute a basis of ")
	    + Name( theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    );

   pv.add( Text("Determine in which term of the lower central series ")
	  + Name( theWord ),
	  ThisARCSlotID(),
	  100
	  );
  pv.done();
}
// ---------------------- NGorderOfTorsionSubgroupProblem ----------------------- //


NGorderOfTorsionSubgroupProblem::NGorderOfTorsionSubgroupProblem(SMFPGroup& group)
  : Supervisor( true ),
    theGroup( group ),
    theGIC( group.gic ),
    computeBasis( *this, group.gcm().computeBasis )
    //    computeLCSQuotientsComp( *this, group.gcm().computeLCSQuotientsComp )
{ 
  if (theGIC.haveNilpotentGroupInited())
    resultIsFast();
}


void NGorderOfTorsionSubgroupProblem::takeControl( )
{
  if (theGIC.haveNilpotentGroupInited()){
      LogMessage msg( *this, theGroup );
      msg << "The order of the torsion subgroup of  " << Name( theGroup ) << " is ";
      msg << theGIC.getNilpotentGroupInited().
	orderOfTheTorsionSubgroup( ) << ".";
      msg.send();    
      adminTerminate();
  }
}


void NGorderOfTorsionSubgroupProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the order of the torsion subgroup"),
		  Text("Find the order of the torsion subgroup of ")
		  + Name( theGroup ), 
		  helpID("NGorderOfTorsionSubgroupProblem", 
			 theGroup), "Ord tor subgp"
		  );

  pv.startItemGroup();
  
  if (!theGIC.haveNilpotentQuotInited(theGIC.getNilpotentcyClass()))
    pv.add( Text("Compute a basis of ")
	    + Name( theGroup ),
	    computeBasis.arcSlotID(),
	    100
	    );

  /*  pv.add( Text("Compute the lower central factors of")
	  + Name( theGroup ),
	  computeLCSQuotientsComp.arcSlotID(),
	  100
	  );
	  */
  pv.done();
}
//---------------------------------------------------------------------------//
//-------------------- NGbuildPresentationProblem --------------------------//
//---------------------------------------------------------------------------//


NGbuildPresentationProblem::NGbuildPresentationProblem( SMFPGroup& group)
  : Supervisor( true ),
    theGroup( group ),
    arcer( *this),
    started(false),
    computeBasis( *this, group.gcm().computeBasis )
{
  
}


void NGbuildPresentationProblem::takeControl( )
{
   if (freeARCs() > 0){
     if (theGroup.gic.haveNilpotentGroupInited()){
       if (!started){
	 arcer.setArguments(theGroup.gic.getNilpotentGroupInited());    
	 started = true;
       }
       if (arcer.takeControl() ){
	 File file;
	 arcer.getPresentation().print(file);
	 LogMessage msg( *this, theGroup);
	 if (theGroup.gic.isFreeNilpotent() == yes)
	   msg << Link(Text("Click here to view the structure constants of ") + 
			Name(theGroup) + "." ,
			"FNGStructureConstants", 
			file.getFileName());	   
	 else
	   msg << Link(Text("Click here to view the presentation of ") +
		       Name(theGroup) + "." ,
		       "FNGStructureConstants", file.getFileName());
	 msg.send();
	 adminTerminate();
	 return;
       }
     }
     usedOneARC();
   }
}  


void NGbuildPresentationProblem::start( ) { }


void NGbuildPresentationProblem::terminate( ) { }

void NGbuildPresentationProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the presentation"),
		  Text("Compute the presentation of")
		  + Name( theGroup ),
		  helpID("NGbuildPresentationProblem", theGroup),
		  Text("Pres of") + Name( theGroup )
		  );

  pv.startItemGroup();
  if (!theGroup.gic.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name( theGroup ),
	    computeBasis.arcSlotID(),
	    100
	    );
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 100);
  pv.done();
}

// ---------------------- NGAutoIsIAAut ----------------------- //


NGAutoIsIAAut::NGAutoIsIAAut(SMMap& map)
  : Supervisor( true ),
    theMap( map ),
    theGIC( map.getDomain().gic ),
    abelianInvariants( *this, map.getDomain().gcm().abelianInvariants )
{ 
  if (theGIC.haveCyclicDecomposition())
    resultIsFast();
}


void NGAutoIsIAAut::takeControl( )
{
  if (theGIC.haveCyclicDecomposition()){
    int theNumberOfGenerators = theMap.getDomain().getFPGroup().numberOfGenerators();
    VectorOf<Word> V =  theMap.getMap().generatingImages();
    bool answer = true;
    const AbelianGroup& abGroup = theGIC.getCyclicDecomposition();
    for( int i = theNumberOfGenerators-1; i >= 0; --i ) {
      if( !abGroup.isTrivial(inv(Generator(i+1)) * V[i]) )
	answer = false;
    }
    LogMessage msg( theMap );
    msg << Name( theMap ) << " is ";    
    if( !answer )
      msg << "not ";
    
    msg << "an IA-automorphism of " << Name( Domain( theMap ) ) << ".";
    msg.send();
    adminTerminate();
  }
}

void NGAutoIsIAAut::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Test for an automorphism being an IA-automorphism"),
		  Text("Is") + Name( theMap  ) + "an IA-automorphism?",
		  helpID("NGAutoIsIAAut", theMap.getDomain()),
		  "Is an IA-auto?" 
		  );
  
  pv.startItemGroup();
  
  pv.add( Text("Compute abelianization of")
	  + Name( theMap.getDomain() ),
	  abelianInvariants.arcSlotID(),
	  50
	  );  
  pv.done();
}
 
// ----------------- NGisCentralARCer ------------------------ //

void NGisCentralARCer::runComputation( )
{
  theAnswer = theNG->isCentral( theWord );  
}


void NGisCentralARCer::setArguments( const NilpotentGroup& NG,
					     const Word& w)
{
  theNG = new NilpotentGroup( NG );
  theWord = w;
}


void NGisCentralARCer::writeResults( ostream& out ) 
{
  out < theAnswer;
}

void NGisCentralARCer::readResults( istream& in )
{
  in > theAnswer;
}

//---------------------------------------------------------------------------//
//-------------------- NGisCentralProblem ---------------------------//
//---------------------------------------------------------------------------//


NGisCentralProblem::NGisCentralProblem(SMWord& w)
  : Supervisor( true ),
    theWord( w ),
    computeBasis( *this, w.getParent().gcm().computeBasis ),
    answer(dontknow),
    started( false ),
    arcer( *this )
{
  if( w.getWord().freelyReduce().length() == 0) {
    answer = true;
    resultIsFast();
    return;
  }
  //  if( w.wic.haveCollectedForm() ) resultIsFast();
}


void NGisCentralProblem::takeControl( )
{
  if( fastResult() ) {
    if(answer == dontknow) {
      /*      const NilpotentGroup& NGroup = 
	theWord.getParent().gic.getNilpotentGroupInited();
      Generator firstLetter = theWord.wic.getCollectedForm().firstLetter().gen;
      answer = ( ord(firstLetter) >= 
		 NGroup.firstOfWeight( NGroup.nilpotencyClass() ) 
		 );
		 */
    }
  }
  else {
    if ( freeARCs() <= 0 ) return;
    if 	(theWord.getParent().gic.haveNilpotentGroupInited()){
      if (!started){
	arcer.setArguments(theWord.getParent().gic.getNilpotentGroupInited(),
			   theWord.getWord());    
	started = true;
      }
      if( arcer.takeControl() ) {
	answer = arcer.getAnswer();
      }
      else {
	usedOneARC();
	return;
      }
    }  
  }
  LogMessage msg( *this, theWord );
  msg << Name( theWord ) << " is ";
  if ( answer == no ) msg << "not ";
  msg << "central in " << Name( Parent( theWord ) ) << ".";
  msg.send();

  adminTerminate();

}

void NGisCentralProblem::start( ) { }


void NGisCentralProblem::terminate( ) { }


void NGisCentralProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Central element problem"),
		  Text("Is") + Name( theWord ) + "central in"
		  + Name( Parent( theWord ) ) + "?",
		  helpID("NGisCentralProblem", theWord.getParent()),
		  Text("Is") + Name(theWord) 
		  + "central ?"
		  );

  pv.startItemGroup();
  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name( theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    ); 
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 100);
  pv.done();
}

 
// ---------------------- NGMaximalRootARCer --------------------------- //


void NGMaximalRootARCer::setArguments( const NilpotentGroup& group,
					const Word& w)
{
  NG = new NilpotentGroup( group );
  theWord = w;
}


void NGMaximalRootARCer::runComputation( )
{
#if SAFETY > 0
  if( NG == 0 )
    error( "void NGMaximalRootARCer::runComputation( ) : "
	   "You have not initialized the group");
#endif
 PolyWord root;
 NG->maximalRoot(theWord, root, thePower);
 theRoot = NG->toWord(root);
}


void NGMaximalRootARCer::writeResults( ostream& out ) 
{
  out < theRoot;
  out < thePower;
}


void NGMaximalRootARCer::readResults( istream& in )
{
  in > theRoot;
  in > thePower;
}


int NGMaximalRootARCer::getPower() const
{
#if SAFETY > 0
  if( NG == 0 )
    error( "int NGMaximalRootARCer::getPower() const : "
	   "You have not initialized the group");
#endif
  
  return thePower;
}

Word NGMaximalRootARCer::getRoot() const
{
#if SAFETY > 0
  if( NG == 0 )
    error( "Word NGMaximalRootARCer::getRoot() const : "
	   "You have not initialized the group");
#endif
  
  return theRoot;
}

//---------------------------------------------------------------------------//
//-------------------- NGMaximalRootProblem----- ---------------------------//
//---------------------------------------------------------------------------//


NGMaximalRootProblem::NGMaximalRootProblem(SMWord& w)
  : Supervisor( true ),
    computeBasis( *this, w.getParent().gcm().computeBasis ),
    theWord( w ),
    started(false),
    arcer( *this )
{
  if( theWord.wic.isPower() != dontknow )
    resultIsFast();
}


void NGMaximalRootProblem::takeControl( )
{
  if(!fastResult() ) {
    if ( freeARCs() > 0 ) {
      if (theWord.getParent().gic.haveNilpotentGroupInited()){
	if (!started){
	  arcer.setArguments(theWord.getParent().gic.getNilpotentGroupInited(),
			     theWord.getWord());    	  
	  started = true;
	}
	if( arcer.takeControl() ) {
	  theWord.wic.putHaveMaximalRoot(arcer.getRoot(),arcer.getPower());
	}
      }
      usedOneARC();
    }
  }  
  
  // reporting result
  if (theWord.wic.isPower() != dontknow){
    LogMessage msg( *this, theWord);
    if (!theWord.wic.isPower()){
      msg << Name(theWord) <<" is not a power, so it does not have maximal root.";
    }else {
      Word root;
      Integer exp;
      theWord.wic.getMaximalRoot(root,exp);
      msg << "Maximal root of "<<Name(theWord ) << " is ";
      theWord.getParent().getFPGroup().printWord(msg,root);
      msg << " and " << exp << ordinalPostfix(exp.as_long())<<" power of it is "
	  << Name(theWord );
    }
    msg.send(); 
    
    adminTerminate();
  }
}

void NGMaximalRootProblem::start( ) { }


void NGMaximalRootProblem::terminate( ) { }

void NGMaximalRootProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the maximal root of element"),
		  Text("Compute the maximal root of ")
		  + Name( theWord )  ,
		  helpID("NGMaximalRootProblem", theWord.getParent()), 
		  "Max root"
		  );
  pv.startItemGroup();
  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name( theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    ); 
  pv.add(Text("Compute maximal root"), ThisARCSlotID(), 100);
  pv.done();
}

//---------------------------------------------------------------------------//
//-------------------- NGIsProperPower  ------------------------------------//
//---------------------------------------------------------------------------//


NGIsProperPower::NGIsProperPower(SMWord& w)
  : Supervisor( true ),
    theWord( w ),
    computeBasis( *this, w.getParent().gcm().computeBasis ),
    started(false),
    arcer( *this )
{
  if( theWord.wic.isPower() != dontknow )
    resultIsFast();
}


void NGIsProperPower::takeControl( )
{
  if(!fastResult() ) {
    if ( freeARCs() <= 0 ) return;
    if (theWord.getParent().gic.haveNilpotentGroupInited()){
      if (!started){
	arcer.setArguments(theWord.getParent().gic.getNilpotentGroupInited(),
			   theWord.getWord());    	
	started = true;
      }
      if( arcer.takeControl() ) {
	theWord.wic.putHaveMaximalRoot(arcer.getRoot(),arcer.getPower());
      }
      else {
	usedOneARC();
	return;
      }
    }
  }  
  
  // reporting result

  if( theWord.wic.isPower() != dontknow ){
    LogMessage msg( *this, theWord);
    bool answer = false;
    if (theWord.wic.isPower()){
      Word root;
      Integer exp;
      theWord.wic.getMaximalRoot(root,exp);
      if (exp > 1)
	answer = true;
    }
    msg << Name (theWord) << " is ";
    if (!answer) msg << " not ";
    msg << "a proper power.";
    msg.send(); 
    
    adminTerminate();
  }
}

void NGIsProperPower::start( ) { }


void NGIsProperPower::terminate( ) { }

void NGIsProperPower::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is element a proper power"),
		  Text("Find is the ") + Name( theWord ) 
		  + Text(" a proper power"),
		  helpID("NGIsProperPower", theWord.getParent()),
		  Text("Powr prob")
		  );

  pv.startItemGroup();
  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name(theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    ); 
  pv.add(Text("For use of this problem"), ThisARCSlotID(), 100);
  pv.done();
}

// ---------------------- NGInverseAutoARCer --------------------------- //


void NGInverseAutoARCer::setArguments( const NilpotentGroup& group, 
					    const VectorOf<Word>& images)
{
  NG = new NilpotentGroup( group );
  theImages = images;
}


void NGInverseAutoARCer::runComputation( )
{
#if SAFETY > 0
  if( NG == 0 )
    error( "void NGInverseAutoARCer::runComputation( ) : "
	   "Nilpotent group has not been computed yet");
#endif

  result = NG->inverseAuto(theImages);
}


void NGInverseAutoARCer::writeResults( ostream& out ) 
{
  out < result;
}


void NGInverseAutoARCer::readResults( istream& in )
{
  in > result;
}

// ------------------- NGInverseAuto -----------------------//

NGInverseAuto::NGInverseAuto(const SMHomomorphism& aut)
  : Supervisor( true ),
    theAuto(aut ),
    arcer(*this)
{
  if (theAuto.mic.isEpi()!=yes)
    error("Don't know is it automorphism!!!");
  else{
    FPGroup group = theAuto.getDomain().getFPGroup();
    NilpotentGroup  ng(group.namesOfGenerators(),
		       theAuto.getDomain().gic.getNilpotentcyClass(),
		       makeVectorOf(group.getRelators()));   
    arcer.setArguments(ng,theAuto.getMap().generatingImages());
  }    
}


void NGInverseAuto::takeControl( )
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


void NGInverseAuto::viewStructure(ostream& ostr) const
{
  char *temp;
    
  ProblemView pv( ostr, oid(), Text("Find inverse of an automorphism"),
		  Text("Find inverse of ")+Name(theAuto),
		  helpID("NGInverseAuto", theAuto.getDomain()),
		  Text("inverse")
		 );
  
  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.done();
}

 
// ---------------------- NGcentralizerARCer --------------------------- //


void NGcentralizerARCer::setArguments( const NilpotentGroup& group,
					const Word& w)
{
  NG = new NilpotentGroup( group );
  theWord = w;
}


void NGcentralizerARCer::runComputation( )
{
#if SAFETY > 0
  if( NG == 0 )
    error( "void NGcentralizerARCer::runComputation( ) : "
	   "You have not initialized the group");
#endif
 result = NG->centralizer(theWord);
}


void NGcentralizerARCer::writeResults( ostream& out ) 
{
  out < result;
}


void NGcentralizerARCer::readResults( istream& in )
{
  in > result;
}


const VectorOf<Word>& NGcentralizerARCer::getCentralizer() const
{
#if SAFETY > 0
  if( NG == 0 )
    error( " NGcentralizerARCer::getCentralizer() const : "
	   "You have not initialized the group");
#endif
  
  return result;
}

//---------------------------------------------------------------------------//
//-------------------- NGcentralizer ----------------------------------------//
//---------------------------------------------------------------------------//


NGcentralizer::NGcentralizer(SMWord& w)
  : Supervisor( true ),
    computeBasis( *this, w.getParent().gcm().computeBasis ),
    theWord( w ),
    started(false),
    arcer( *this )
{

}


void NGcentralizer::takeControl( )
{
  if(!fastResult() ) {
    if ( freeARCs() > 0 ) {
      if (theWord.getParent().gic.haveNilpotentGroupInited()){
	if (!started){
	  arcer.setArguments(theWord.getParent().gic.getNilpotentGroupInited(),
			     theWord.getWord());    	  
	  started = true;
	}
	if( arcer.takeControl() ) {
	  SGofFreeGroup F(theWord.getParent().getFreePreimage(),
			  arcer.getCentralizer() );
	  
	  SMObject* smo = 
	    new SMSubgroup( theWord.getParent(), F,
			    Text("The centralizer of") + Name( theWord )
			    ); 
	  
	  ListOf<OID> dependencies( theWord.getParent() );
	  CheckinMessage( *smo, "", dependencies ).send();
	  
	  adminTerminate();
	  return;
	}
      }
      usedOneARC();
    }
  }  
}

void NGcentralizer::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the centralizer of element"),
		  Text("Compute the centralizer of ")
		  + Name( theWord )  ,
		  helpID("NGcentralizer", theWord.getParent()), 
		  "centralizer"
		  );
  pv.startItemGroup();
  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name( theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    ); 
  pv.add(Text("Compute the centralizer "), ThisARCSlotID(), 100);
  pv.done();
}

// ---------------------- NGIsomorphismARCer --------------------------- //

void NGIsomorphismARCer::setArguments( const FPGroup& gr1 ,
				       const FPGroup& gr2 , 
				       int n1 , int n2 )
{
  g1 = gr1;
  g2 = gr2;
  nc1 = n1;
  nc2 = n2;
}

void NGIsomorphismARCer::runComputation( )
{
  if( nc1 && nc2 )
    {
      NilpotentGroup ng1( g1.namesOfGenerators() , nc1 ,
			  makeVectorOf(g1.getRelators()) );
      NilpotentGroup ng2( g2.namesOfGenerators() , nc2 ,
			  makeVectorOf(g2.getRelators()) );
      
      if( !ng1.isInitialized() ) ng1.initialize();
      if( !ng2.isInitialized() ) ng2.initialize();
      
      int i;
      
      for( i = 1 ; i <= nc1 && i <= nc2 ; i++ )
	if( !ng1.getLCSQuotient(i).abelianization.isomorphicTo(ng2.getLCSQuotient(i).abelianization) )
	  {
	    ans = no;
	    return;
	  }
      
      if( nc1 > nc2 )
	{
	  if( !ng1.getLCSQuotient( i ).abelianization.isTrivial() )
	    ans = no;
	}
      else
	if( nc1 < nc2 )
	  if( !ng2.getLCSQuotient( i ).abelianization.isTrivial() )
	    ans = no;
    }
  else
    for( int nc = 1 ; ; nc++ )
      {
	NilpotentGroup ng1( g1.namesOfGenerators() , nc ,
			    makeVectorOf(g1.getRelators()) );
	NilpotentGroup ng2( g2.namesOfGenerators() , nc ,
			    makeVectorOf(g2.getRelators()) );
	
	if( !ng1.isInitialized() ) ng1.initialize();
	if( !ng2.isInitialized() ) ng2.initialize();
	
	if( !ng1.getLCSQuotient(nc).abelianization.isomorphicTo(ng2.getLCSQuotient(nc).abelianization) )
	  {
	    ans = no;
	    break;
	  }
      }
}
 
void NGIsomorphismARCer::writeResults( ostream& out ) 
{
  out < ans;
}


void NGIsomorphismARCer::readResults( istream& in )
{
  in > ans;
}

// ------------------------- NGIsomorphismProblem ------------------------- //

NGIsomorphismProblem::NGIsomorphismProblem( SMFPGroup& gr1 , SMFPGroup& gr2 )
  : ComputationManager( true ), 
    theGroup1( gr1 ),
    theGroup2( gr2 ),
    arcer( *this )
{
  if( theGroup1.gic.haveActualNilpotencyClass() &&
      theGroup2.gic.haveActualNilpotencyClass() &&
      theGroup1.gic.getActualNilpotencyClass() != 
      theGroup2.gic.getActualNilpotencyClass() )
   {
     flag = 0;
     resultIsFast();
     return;
   }
  
  if( theGroup1.getFPGroup().numberOfGenerators() == 
      theGroup2.getFPGroup().numberOfGenerators() &&
      theGroup1.getFPGroup().getRelators() ==
      theGroup2.getFPGroup().getRelators() )
    if( theGroup1.getFPGroup().namesOfGenerators() ==
	theGroup2.getFPGroup().namesOfGenerators() )
      {
	flag = 1;
	resultIsFast();
	return;
      }
    else
      {
	flag = 2;
	resultIsFast();
	return;
      }
  
  int nc1,nc2;
  
  if( theGroup1.gic.isNilpotent() == yes )
    nc1 = theGroup1.gic.getNilpotentcyClass();
  else
    nc1 = 0;
  
  if( theGroup2.gic.isNilpotent() == yes )
    nc2 = theGroup2.gic.getNilpotentcyClass();
  else
    nc2 = 0;

  arcer.setArguments( theGroup1.getFPGroup() , 
		      theGroup2.getFPGroup() , 
		      nc1 , nc2 );
}

void NGIsomorphismProblem::takeControl( )
{
  if( fastResult() )
    {
      LogMessage msg1( *this , theGroup1 , theGroup2 );
      
      if( !flag )
	msg1 << Name( theGroup1 ) << " and " << Name( theGroup2 ) << " are not isomorphic because they are of different nilpotency classes.";
      else
	if( flag == 1 )
	  msg1 << Name( theGroup1 ) << " and " << Name( theGroup2 ) << " are isomorphic because they have the same presentation.";
	else
	  {
	    msg1 << Name( theGroup1 ) << " and " << Name( theGroup2 ) << " are isomorphic.";
	    msg1.send();
	    
	    LogMessage msg3( *this , theGroup1 , theGroup2 );
	    msg3 << "The isomorphism is: ";
	    
	    FPGroup G1 = theGroup1.getFPGroup();
	    FPGroup G2 = theGroup2.getFPGroup();
	    
	    for( int i = 0 ; i < G1.numberOfGenerators() ; i++ )
	      {
		msg3 << G1.nameOfGenerator( i ) << " -> " << G2.nameOfGenerator( i );
		if( i != G1.numberOfGenerators() - 1 )
		  msg3 << " , ";
	      }
	    
	    msg3 << " . ";
	    msg3.send();
	    
	    adminTerminate();
	    return;
	  }
      msg1.send();
      
      adminTerminate();
      return;
    }
  else
     if ( freeARCs() > 0 )
       if( arcer.takeControl() ) 
	 {
	   Trichotomy answer = arcer.getAnswer();
	   
	   if( answer == no )
	     {
	       LogMessage msg2( *this , theGroup1 , theGroup2 );
      	       msg2 << Name( theGroup1 ) << " and " << Name( theGroup2 ) << " are not isomorphic because they have nonisomorphic LCS quotients.";
	       msg2.send();
	     }
	   else
	     {
	       LogMessage msg3( *this , theGroup1 , theGroup2 );
      	       msg3 << "This algorithm can't give an exact answer.";
	       msg3.send();
	     }
	   
	   adminTerminate();
	   return;
	 }
       else
	 usedOneARC();
  
}
  
void NGIsomorphismProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Nilpotent isomorphism problem"),
		  Text("Are ") + Name( theGroup1 ) + " and " + 
		  Name( theGroup2 ) + " isomorphic? ",
		  helpID("IsomorphismProblem", theGroup1),
		  "Isomorphic ?" 
		  );
  
  pv.startItemGroup();
  
  pv.add( Text("Are ")+Name( theGroup1 ) + " and " + Name( theGroup2 ) + 
	  " isomorphic?",
	  ThisARCSlotID(),
	  100);
  
  pv.done();
}
#line 1  "src/FreeIsPartOfBasisProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes
//           FreeIsPartOfBasisProblem, FreeIsPartOfBasis
//           FreeGeneralIsPartOfBasisProblem, FreeGeneralIsPartOfBasis
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//

#include "FreeIsPartOfBasisProblem.h"
#include "OutMessages.h"
#include "viewStructure.h"



// -------------------------- GAIsPartOfBasisArcer ---------------------- //


GAIsPartOfBasisArcer::GAIsPartOfBasisArcer( ComputationManager& boss )
  : ARCer( boss )
{ } 

void GAIsPartOfBasisArcer::setArguments( FreeGroup f,Word w )
{
  theGroup  =  f;
  theWord = w;
}

void GAIsPartOfBasisArcer::runComputation( ) 
{
  ACConfig c( 50, 1000000, 1, 
	      85, 95, 1, 
	      0, 1,
	      75, 5,15,5,0,0
	      );


 GAIsPartOfBasis ispart(theGroup,c,theWord);
 
 ispart.isPartOfBasis( compFile,resultFile );

 theAuto = ispart.getAutomorphism();

}

void  GAIsPartOfBasisArcer::writeResults( ostream& out )
{
  out < theAuto;
}

void GAIsPartOfBasisArcer::readResults( istream& in )
{
  in > theAuto;
}


// ---------------------------- GAIsPartOfBasisCM ------------------------ //


GAIsPartOfBasisCM::GAIsPartOfBasisCM( FreeIsPartOfBasisProblem& boss )
  : arcer( *this ), answer( dontknow ), theWord(boss.getWord()),  linkHasBeenSent(false)

{
 arcer.setArguments( boss.getWord().getParent().getFreePreimage(),
		     boss.getWord().getWord());
 adminStart();
}

void GAIsPartOfBasisCM::terminate( )
{
}

void GAIsPartOfBasisCM::start( ) { }


void GAIsPartOfBasisCM::takeControl( )
{
  if ( freeARCs() > 0 ) {
    
    if( !linkHasBeenSent ) {
      
      linkHasBeenSent = true;
      
      LogMessage msg( *this, theWord );
      msg << Link("Click here to see details of the genetic algorithm.",
		  "FreeIsPartOfBasis", 
		  getComputationFileName(), true);
      msg.send();
      
    }

    if( arcer.takeControl() ) {
      answer = yes;
      adminTerminate();
      return;
    }
    else
      usedOneARC();
  }
}

// -------------------- FreeIsPartOfBasisProblem --------------------------- //


FreeIsPartOfBasisProblem::FreeIsPartOfBasisProblem(const SMWord& w)
    : Supervisor( true ), theWord( w ), freeIsPartOfBasis( *this ),
      link1HasBeenSent( false ),
      gaIsPartOfBasisCM( *this )
{ }


void FreeIsPartOfBasisProblem::takeControl( )
{
  Trichotomy answer = freeIsPartOfBasis->answer();
  Trichotomy answerGA = gaIsPartOfBasisCM->getAnswer();

  if ( freeIsPartOfBasis.exists() && answer != dontknow ) {
    
    LogMessage msg( *this, theWord );
    if( answer == no )
      msg << Name(theWord) << " is not part of any free basis of "
	  << Name(theWord.getParent()) << ".";
    else {
      msg << Name(theWord) << " is part of the free basis (";

      VectorOf<Word> V = freeIsPartOfBasis->getAutomorphism( )
	.generatingImages();

      int VLen = V.length();
      FreeGroup F  = theWord.getParent().getFreePreimage();
      for( int i = 0; i < VLen; ++i ){
	msg << " ";
	F.printWord( msg, V[i] );
	if( i < VLen - 1)
	  msg << ",";
	else
	  msg << " ): according to the Whitehead algorithm.";
      }
    }
    
    msg.send();
    
    adminTerminate();
    return;
  }

  if ( answerGA != dontknow ){
     LogMessage msg( *this, theWord );
     msg << Name(theWord) << " is part of the free basis (";

     FreeGroup F = theWord.getParent().getFreePreimage();

     VectorOf<Word> invA = 
       F.inverseAutomorphism(gaIsPartOfBasisCM->getAutomorphism());

     int VLen = invA.length();
     for( int i = 0; i < VLen; ++i ){
       msg << " ";
       Word gw( Generator(i+1));
       gw = gw.replaceGenerators(invA);
       F.printWord( msg, gw );
       if( i < VLen - 1)
	  msg << ",";
       else
	 msg << " ): according to the genetic algorithm.";
     }
     
     msg.send();

     LogMessage msgl( *this, theWord );
     msgl << Link("Click here to see the result of the genetic algorithm.",
		  "FreeIsPartOfBasis", 
		  gaIsPartOfBasisCM->getResultFileName());
     msgl.send();
     
     adminTerminate();
     return;
  }
  
}


void FreeIsPartOfBasisProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is part of free basis problem"),
		  Text("Is") + Name( theWord )
		  + "is part of a free basis in"
		  + Name( Parent( theWord ) ) + "?",
		  helpID("FreeIsPartOfBasisProblem", theWord.getParent()),
		  "Part of basis ?"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of the Whitehead algorithm"), freeIsPartOfBasis.arcSlotID(), 100);
  pv.add(Text("For the use of the genetic algorithm"), gaIsPartOfBasisCM.arcSlotID(), 100);

  pv.done();
}


// ------------------------ FreeIsPartOfBasis ------------------------------ //


FreeIsPartOfBasis::FreeIsPartOfBasis(FreeIsPartOfBasisProblem& problemObject)
    : theWord( problemObject.getWord() ),
      theAnswer( dontknow ), theGroup( theWord.getParent().getFreePreimage() ),
      theAutomorphism( theGroup, theGroup ),
      GW( new GeneralWhitehead( theGroup, true ) ),
      theFileName( GW->getFileName() ),
      linkHasBeenSent( false )
{
  GW->startComputation( theWord.getWord() );
  adminStart();
}


FreeIsPartOfBasis::~FreeIsPartOfBasis( )
{
  delete GW;
}


Map FreeIsPartOfBasis::getAutomorphism( ) const
{
#if SAFETY > 0
  if ( theAnswer != yes )
	 error("Called FreeIsPartOfBasis::getAutomorphism "
	       "when theAnswer != yes");
#endif
  return theAutomorphism;
}


void FreeIsPartOfBasis::takeControl( )
{
 if ( freeARCs() > 0 ) {

   if( !linkHasBeenSent ) {
     
     linkHasBeenSent = true;
     
     LogMessage msg( *this, theWord );
     msg << Link("Click here to obtain details of the computation.",
		 "FreeIsPartOfBasis", getFileName(), true);
     msg.send();
   }
   
    
    if ( GW->continueComputation() ) {
      
      if( GW->extendsToFreeBasis() )
	theAnswer = yes;
      else
	theAnswer = no;
      
      if ( theAnswer == yes )
	theAutomorphism = GW->getAutomorphism();

      delete GW;
      GW = 0;

      adminTerminate();
      return;
    }
    
    usedOneARC();
  }
}


// ------------------ FreeGeneralIsPartOfBasisProblem ---------------------- //


FreeGeneralIsPartOfBasisProblem::FreeGeneralIsPartOfBasisProblem
  (const SMSetOfWords& S)
    : Supervisor( true ), theSet( S ), freeIsPartOfBasis( *this ),
      linkHasBeenSent( false )
{ }


void FreeGeneralIsPartOfBasisProblem::takeControl( )
{
  if( !linkHasBeenSent ) {

    linkHasBeenSent = true;

    LogMessage msg( *this, theSet );
    msg << Link("Click here to obtain details of the computation.",
		"FreeIsPartOfBasis", freeIsPartOfBasis->getFileName(), true);
    msg.send();
   
  }
  
  Trichotomy answer = freeIsPartOfBasis->answer();

  if ( freeIsPartOfBasis.exists() && answer != dontknow ) {
    
    LogMessage msg( *this, theSet );
    if( answer == no )
      msg << Name(theSet) << " is not part of any free basis of "
	  << Name(theSet.getParent()) << ".";
    else {
      msg << Name(theSet) << " is part of the free basis (";

      VectorOf<Word> V = freeIsPartOfBasis->getAutomorphism( )
	.generatingImages();

      int VLen = V.length();
      FreeGroup F  = theSet.getParent().getFreePreimage();
      for( int i = 0; i < VLen; ++i ){
	msg << " ";
	F.printWord( msg, V[i] );
	if( i < VLen - 1)
	  msg << ",";
	else
	  msg << " ).";
      }
    }
    
    msg.send();
    adminTerminate();
  }
}


void FreeGeneralIsPartOfBasisProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is part of free basis problem"),
		  Text("Is") + Name( theSet )
		  + "is part of a free basis in"
		  + Name( Parent( theSet ) ) + "?",
		  helpID("FreeGeneralIsPartOfBasisProblem", theSet.getParent()),
		  "Part of basis ?"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), freeIsPartOfBasis.arcSlotID(), 100);

  pv.done();
}


// ----------------------- FreeGeneralIsPartOfBasis ------------------------ //


FreeGeneralIsPartOfBasis::FreeGeneralIsPartOfBasis
  (FreeGeneralIsPartOfBasisProblem& problemObject)
    : theSet( problemObject.getSetOfWords() ),
      theAnswer( dontknow ), theGroup( theSet.getParent().getFreePreimage() ),
      theAutomorphism( theGroup, theGroup ),
      GW( new GeneralWhitehead( theGroup, true ) ),
      theFileName( GW->getFileName() )
{
  GW->startComputation( theSet.getWords() );
  adminStart();
}


FreeGeneralIsPartOfBasis::~FreeGeneralIsPartOfBasis( )
{
  delete GW;
}


Map FreeGeneralIsPartOfBasis::getAutomorphism( ) const
{
#if SAFETY > 0
  if ( theAnswer != yes )
	 error("Called FreeGeneralIsPartOfBasis::getAutomorphism "
	       "when theAnswer != yes");
#endif
  return theAutomorphism;
}


void FreeGeneralIsPartOfBasis::takeControl( )
{
  if ( freeARCs() > 0 ) {
    
    if ( GW->continueComputation() ) {
      
      if( GW->extendsToFreeBasis() )
	theAnswer = yes;
      else
	theAnswer = no;
      
      if ( theAnswer == yes )
	theAutomorphism = GW->getAutomorphism();

      delete GW;
      GW = 0;

      adminTerminate();
      return;
    }
    
    usedOneARC();
  }
}
#line 1  "src/IsTrivialProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes IsTrivialChecker, IsTrivialProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "IsTrivialProblem.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
//#include "ORWordProblem.h"
#include "ORProblems.h"
#include "DecomposeInSubgroup.h"

// ------------------------ IsTrivialChecker ------------------------------- //


IsTrivialChecker::IsTrivialChecker( class SMFPGroup& group)
  : theGroup( group ),
    G( group.getFPGroup() ),
    checker( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false ),
    triedPreliminaryCheckings( false ),
    trivialDetails( group.getFPGroup() )
{ }

Trichotomy IsTrivialChecker::preliminaryCheckings()
{
  SetOf<Word> S = G.getRelators();
  int numOfGens = G.numberOfGenerators();


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Couple of fast checks                                             //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if( checker.hasTrivialPresentation() ) return yes;

  if( checker.numOfRelsLessNumOfGens() ) {
    explanation = "the number of relators is less than"
                  " the number of generators";
    return no;
  }


  Generator g;

  if( checker.existsGenWithExpSumZeroInEveryRelator(g) ) {
    
    ostrstream msg;
    msg << "the exponent sum of generator ";
    G.printWord(msg, Word(g));
    msg << " is equal to 0 in every relator of the group" << ends;

    explanation = msg.str();
    return no;
  }
  
  for( int i = 0; i < G.numberOfGenerators(); ++i ) {
    Integer gcd = abs( checker.GCDOfExpSumOfGen( Generator(i+1) ) );
    if( gcd > 1 ) {

      ostrstream msg; 
      msg << "the greatest common divisor of exponent sums of generator ";
      G.printWord(msg, Word(Generator(i+1)));
      msg << " in the group relations is " << gcd << ends;
      
      explanation = msg.str();
      return no;
    }
  }
  
  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use one relator                                                   //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.isOneRelator() ) {
    explanation = "the group is one relator";
    return ORProblems(G.numberOfGenerators(), gic.getOneRelator()).isTrivial();
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use small cancellation                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( !gic.haveMSC() ) {
    int lambda = G.cancellationLambda();
    if ( lambda == 0 || lambda >= 6 )
      gic.putHaveMSC( G, lambda );
  }
  
  if ( gic.haveMSC() ) {
    explanation = "the group is small cancellation";
    return gic.getMSC().isTrivial();
  }

  return dontknow;
}


Trichotomy IsTrivialChecker::isTrivial( )
{
  if( !triedPreliminaryCheckings ) {
    triedPreliminaryCheckings = true;
    Trichotomy prelResult = preliminaryCheckings();
    if ( prelResult != dontknow )
      return prelResult;
  }

  int numberOfGenerators = G.numberOfGenerators();


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the abelian quotient                                          //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( ! triedAbelianization && gic.haveCyclicDecomposition( ) ) {
	 
    triedAbelianization = true;
    
    // If the group itself is abelian, we get a definite answer;
    // otherwise, we only succeed when the image is non-trivial.
    
    if ( gic.isAbelian() == yes ) {
      explanation =
	FEData::Text("this follows from the canonical decomposition of")
   	+ FEData::Name(theGroup);
      return gic.getCyclicDecomposition().isTrivial();
    }
    else
      if ( gic.isNilpotent() == yes ) {
	explanation = "this follows from an inspection of the abelianization of the group";
	return gic.getCyclicDecomposition().isTrivial();
      }
      else
	if ( !gic.getCyclicDecomposition().isTrivial() ) {
	  explanation = "this follows from an inspection of the abelianization of the group";

	  return no;
	}
  } 


  if( theGroup.getCheckinType() != SMFPGroup::FP )
    return dontknow;
  
/* 
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use free-by-cyclic structure                                      //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.isFreeByCyclic() ) {
	 explanation = "by normal form in free-by-cyclic";
	 return ( gic.getFreeByCyclic().normalForm( w ).length() == 0 );
  }
*/

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the normal closure of the relators                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  // There are two cases:
    
  // 1) The group is finite, and a complete Cayley graph has been reported
  //    to the GIC.

  if ( gic.haveCompleteCayleyGraph() ) {
    // We get a definite answer.
    explanation = "by running through complete Cayley graph";

    bool header = true;
    SubgroupGraph SG( gic.getCompleteCayleyGraph() );
    for( int i = 0; i < numberOfGenerators; ++i ) {
      if( !SG.contains( Generator(i+1) ) ) {
	trivialDetails.printNonTrivialGeneratorDetails( 
          Generator(i+1), explanation, header
	);
	
	return no;
      }
      else if( gic.haveWordDecomposer() ) {
	trivialDetails.printTrivialGeneratorDetails(
	  Generator( i+1 ),
	  gic.getWordDecomposer().fullDecomposition( Generator(i+1) ),
	  header
	);
	header = false;
      }
    }

    return yes;
  }
    
  // 2) Not 1, so we run the word through the existing normal closure
  //    approximation.

  if ( gcm.normalClosure.exists() && gic.haveWordDecomposer() ) {

    bool haveResult = true;
    for( int i = 0; i < numberOfGenerators; ++i ) {
      if( gcm.normalClosure->isTrivial( Generator(i+1) ) != yes ) {
	haveResult = false;
	break;
      }
    }

    if( haveResult ) {
      explanation = "labels loop in normal closure approximation";

      bool header = true;
      for( int i = 0; i < numberOfGenerators; ++i ) {
	trivialDetails.printTrivialGeneratorDetails( 
	  Generator(i+1), 
	  gic.getWordDecomposer().fullDecomposition( Generator(i+1) ),
	  header
	);
	header = false;
      }

      return yes;
    }
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use complete KBMachine                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveConfluentKBMachine() ) {
    explanation = "according to the confluent rewrite system";
    for( int i = 0; i < numberOfGenerators; ++i ) {
      Word temp( Generator(i+1) );
      gic.getConfluentKBMachine().rewrite( temp );
      if( temp.length() > 0 )
	return no;
    }
    return yes;
  }

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use automatic structure                                           //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveAutomatic() ) {
    explanation = "according to the difference machine";
    for( int i = 0; i < numberOfGenerators; ++i ) {
      Word temp( Generator(i+1) );
      gic.getDiffMachine().rewrite( temp );
      if( temp.length() > 0 )
	return no;
    }
    return yes;
  }

  return dontknow;
}


// ----------------------- IsTrivialProblem -------------------------------- //


IsTrivialProblem::IsTrivialProblem(class SMFPGroup& G)
  : Supervisor( ! G.gic.haveFastWordProblem() ),
    theGroup( G ),
    theChecker( G ),
    theAnswer( dontknow ),
    normalClosure( *this, G.gcm().normalClosure ),
    abelianInvariants( *this, G.gcm().abelianInvariants ),
    kbSupervisor( *this, G.gcm().kbSupervisor ),
    agSupervisor( *this, G.gcm().agSupervisor ),
    nilpotentQuotients( *this, G.gcm().nilpotentQuotients ),
    nilpotentWPInQuotients(*this),
    genetic( *this )
{
  SetOf<Word> comms;
  int numOfGens = theGroup.getFPGroup().numberOfGenerators();
  for( int i = 0; i < numOfGens; ++i ) {
    Word gen = Word(Generator(i+1));
    comms |= gen;
  }
  nilpotentWPInQuotients->initialize(comms,&theGroup);
  genetic->init(G.getFPGroup(), comms, GeneticWPCM::SET_OF_WORDS);
  if ( ! displayInFE() ) adminStart();
  if( (theAnswer = theChecker.isTrivial()) != dontknow ) resultIsFast();
}


void IsTrivialProblem::takeControl( )
{
  if ( ! displayInFE() || freeARCs() > 0 ) {
    if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
      if (nilpotentWPInQuotients->isTrivial()==no){
	int retClass;
	theAnswer = nilpotentWPInQuotients->isTrivial(retClass);
	ostrstream msgTmp;
	msgTmp << ": because it's not trivial in lower central quotient of class " 
	       << retClass << ends;
	Chars explanation = msgTmp.str();
	LogMessage msg( *this, theGroup );
	msg << Name( theGroup ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "trivial" << explanation;
	msg.send();
	adminTerminate();     
	return;
      }
    if ( theAnswer != dontknow || 
	 ( theAnswer = theChecker.isTrivial() ) != dontknow ) {

      explanation = theChecker.getExplanation();
      LogMessage msg( *this, theGroup );
      msg << Name( theGroup ) << " is ";
      if ( theAnswer == no ) msg << "not ";
      msg << "trivial";

      if( explanation.length() > 0 )
	msg << ": " << explanation;

      msg << ".";
      msg.send();

      if( theChecker.haveDetails() ) {
	LogMessage msgLink( *this, theGroup );
	msgLink << Link(Chars("Click here to see a decomposition of "
			      "trivial generators of ") 
			+ Text(Name(theGroup))  
			+ Text(" as a product of conjugates of the relators."),
			"IsTrivialDetails", 
			theChecker.getDetailsFileName()
			);
	msgLink.send();
      }

      adminTerminate();
    } else
      if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
	
	Chars explanation = "according to the genetic algorithm";
	LogMessage msg( *this, theGroup );
	msg << Name( theGroup ) << " is trivial";
	msg << " : " << explanation << ".";
	msg.send();
	adminTerminate();
      }
    else
      usedOneARC();
  }
}


void IsTrivialProblem::start( )
{

}


void IsTrivialProblem::terminate( )
{

}


void IsTrivialProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Triviality problem"),
		  Text("Is") + Name( theGroup ) + "trivial ?",
		  helpID( "IsTrivialProblem", theGroup ), 
		  Text(Name(theGroup)) + "= 1 ?"
		  );

  
  pv.startItemGroup();

  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50
	  );


  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::ABELIAN
		     );
  
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(),
	  15
	  );


  pv.startItemGroup( CheckinType( theGroup ) != SMFPGroup::ABELIAN
		     );
  
  pv.add( Text("Is abelianization of") + Name( theGroup )
	  + Text("trivial ?"),
	  abelianInvariants.arcSlotID(),
	  15
	  );

  
  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::FP
		     );
 
  pv.add( Text("Enumerate normal closure of relators"),
	  normalClosure.arcSlotID(), 50);

  
  pv.add( Text("Seek a rewriting system for") + Name( theGroup ),
	  kbSupervisor.arcSlotID(),
	  50
	  );
  
  
  pv.add( Text("Seek a ShortLex automatic structure for")
	  + Name( theGroup ),
	  agSupervisor.arcSlotID(),
	  50
	  );

  pv.add( Text("Run genetic algorithm"), genetic.arcSlotID(), 50 );
  
  int upperBound, currentClass = 2;
  if (theGroup.gic.isNilpotent()==yes)
    upperBound = theGroup.gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theGroup.gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute lower central quotients for") + Name( theGroup ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theGroup ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );
  
  
  pv.done();
}
#line 1  "src/IsFreeProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class IsFreeProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "IsFreeProblem.h"
#include "OutMessages.h"
#include "viewStructure.h"


// ----------------------- ORIsFreeProblemARCer ---------------------------- //


void ORIsFreeProblemARCer::runComputation( )
{
  retValue = theORProblems->isFree();
}


void ORIsFreeProblemARCer::setArguments(ORProblems* anORProblems )
{
  theORProblems = anORProblems;
}


void ORIsFreeProblemARCer::writeResults( ostream& out ) 
{
  out < retValue;
}

void ORIsFreeProblemARCer::readResults( istream& in )
{
  in > retValue;
}


// ------------------------- ORIsFreeProblem ------------------------------- //


ORIsFreeProblem::ORIsFreeProblem( class SMFPGroup& G )
  : ComputationManager( true ),
    theGroup( G ), theORProblems( NULL ), arcer( *this )
{ 
  if( G.gic.isOneRelatorWithTorsion() )
    resultIsFast();
}


ORIsFreeProblem::~ORIsFreeProblem( )
{
  delete theORProblems;
  theORProblems = NULL;
}


void ORIsFreeProblem::takeControl( )
{
  if( fastResult() ) {
    
    LogMessage msg( *this, theGroup );
    msg << " One relator group with torsion is never free.";
    msg.send();

    adminTerminate();
    return;
  }

  if ( freeARCs() > 0 ) {
    if ( theORProblems == NULL ) {
      theORProblems =
	new ORProblems( theGroup.getFPGroup().numberOfGenerators(),
			theGroup.gic.getOneRelator() );
      arcer.setArguments(theORProblems);
    }
    else 
      if( arcer.takeControl() ) {
	
        bool answer = arcer.getRetValue();
	LogMessage msg( *this, theGroup );
	
	msg << Name( theGroup ) << " is ";
	if( !answer )
	  msg << "not ";
	msg << "free.";
	msg.send();
 	
	delete theORProblems;
	theORProblems = NULL;
	
	adminTerminate();
	return;
      }
      else
	usedOneARC();
  }
}


void ORIsFreeProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Freeness problem"),
		  Text("Is ") + Name( theGroup ) + Text(" free ?"),
		  helpID("ORIsFreeProblem", theGroup),
		  Text(Name(theGroup)) + "free ?"
		  );
  
  pv.startItemGroup();

  pv.add( Text("For the use of this problem"),
	  ThisARCSlotID(),
	  50
	  );
  
  pv.done();
}


// --------------------------- IsFreeChecker ------------------------------- //


IsFreeChecker::IsFreeChecker( class SMFPGroup& group)
  : theGroup( group ),
    G( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false )
{ }


Trichotomy IsFreeChecker::isFree( )
{
  if( gic.isFree() != dontknow )
    return gic.isFree();
  
  int numberOfGenerators = G.numberOfGenerators();
  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the abelian quotient                                          //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( ! triedAbelianization && gic.haveCyclicDecomposition( ) ) {
	 
    triedAbelianization = true;
    
    if ( gic.isAbelian() == yes ) {
      explanation =
	FEData::Text("this follows from the canonical decomposition of")
	+ FEData::Name(theGroup);
	
      return gic.getCyclicDecomposition().isFree();
    }
    else
      if ( gic.isNilpotent() == yes ) {
	explanation = Chars("this follows from an inspection of the abelianization of the group");
	return gic.getCyclicDecomposition().isFree();
      }
      else
	if ( !gic.getCyclicDecomposition().isFree() ) {
	  explanation = Chars("this follows from an inspection of the abelianization of the group");
	  return no;
	}
  }
  
  return dontknow;
}


// -------------------------- IsFreeProblem -------------------------------- //


IsFreeProblem::IsFreeProblem(class SMFPGroup& G)
  : Supervisor( G.gic.isFree() == dontknow ),
    theGroup( G ),
    theChecker( G ),
    abelianInvariants( *this, G.gcm().abelianInvariants )
{
  if( theGroup.gic.isAbelian() == yes || theGroup.gic.isNilpotent() == yes )
    if ( theGroup.gic.haveCyclicDecomposition() )
      resultIsFast();
}


void IsFreeProblem::takeControl( )
{
  Trichotomy answer = dontknow;
  if ( ( answer = theChecker.isFree() ) != dontknow ) {
    Chars explanation = theChecker.getExplanation();
    LogMessage msg( *this, theGroup );
    msg << Name( theGroup ) << " is ";
    if ( answer == no ) msg << "not ";

    Chars text;
    if( theGroup.gic.isAbelian() == yes )
      text = "free abelian";
    else
      if( theGroup.gic.isNilpotent() == yes )
	text = "free nilpotent";
      else
	text = "free";
    msg << text;
    
    if( explanation.length() > 0 )
      msg << ": " << explanation;
    
    msg << ".";
    msg.send();
    adminTerminate();
  }
}


void IsFreeProblem::start( ) { }


void IsFreeProblem::terminate( ) { }


void IsFreeProblem::viewStructure(ostream& ostr) const
{
  Chars text = Text("Is") + Name( theGroup );
  
  if( theGroup.gic.isAbelian() == yes )
    text = text + " free abelian ?";
  else
    if( theGroup.gic.isNilpotent() == yes )
      text = text + " free nilpotent ?";
    else
      text = text + " free ?";
  
  
  ProblemView pv( ostr, oid(), Text("Freeness problem"), Text(text),
		  helpID("IsFreeProblem", theGroup),
		  Text(Name(theGroup)) + "free ?");

  
  pv.startItemGroup();

  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::ABELIAN
		     );
  
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(),
	  15
	  );


  pv.startItemGroup( CheckinType( theGroup ) != SMFPGroup::ABELIAN
		     );
  
  pv.add( Text("Is abelianization of") + Name( theGroup )
	  + Text("free ?"),
	  abelianInvariants.arcSlotID(),
	  15
	  );

  
  pv.done();
}
#line 1  "src/IsFiniteProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class OrderProblem
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "GroupFastChecks.h"
#include "IsFiniteProblem.h"
#include "OutMessages.h"
#include "viewStructure.h"


// ------------------------- IsFiniteProblem ------------------------------- //
 

IsFiniteProblem::IsFiniteProblem(SMFPGroup& G)
  : Supervisor( true ),
    theGroup( G ),
    abelianRank( *this, theGroup.gcm().abelianRank ),
    kbSupervisor( *this, theGroup.gcm().kbSupervisor ),
    agSupervisor( *this, theGroup.gcm().agSupervisor ),
    theToddCoxeter( *this, theGroup.gcm().theToddCoxeter),
    ghToddCoxeter( *this, theGroup.gcm().ghToddCoxeter)
  
{
  if( theGroup.gic.isFinite() != dontknow
      || theGroup.gic.haveCyclicDecomposition() ) {
    resultIsFast();
    LogMessage(theGroup.gic.getFiniteMessage()).send();
    return;
  }

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Fast checks                                                       //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  FPGroup FP = theGroup.getFPGroup();
  GroupFastChecks checker(FP);

  if( checker.numOfRelsLessNumOfGens() ) {
    theGroup.gic.putIsFinite(false, "the number of relators is less than"
			     " the number of generators");
    resultIsFast();
    LogMessage(*this, theGroup.gic.getFiniteMessage(), 2).send();
    return;
  }


  Generator g;
  if( checker.existsGenWithExpSumZeroInEveryRelator(g) ) {
    
    ostrstream msg;
    msg << "the exponent sum of generator ";
    FP.printWord(msg, Word(g));
    msg << " is equal to 0 in every relator of the group" << ends;

    Chars explanation = msg.str();
    theGroup.gic.putIsFinite( false, explanation);
    resultIsFast();
    LogMessage(*this, theGroup.gic.getFiniteMessage(), 2).send();
    return;
  }
  

  // @am Why ??
  /*
  if ( theGroup.gic.haveOrder() ) 
    {
      ostrstream msg;
      msg << "according to Todd-Coxeter algorithm";
      Chars explanation = msg.str();
       
      theGroup.gic.putIsFinite( theGroup.gic.getOrder() != 0, explanation);
      resultIsFast();
      return;
    }
    */

}


void IsFiniteProblem::takeControl( )
{
  if ( theGroup.gic.isFinite() != dontknow ) {

    if( !fastResult() ) 
      LogMessage(*this, theGroup.gic.getFiniteMessage(), 2).send();

    adminTerminate();
  }
  else
    {
      if(theGroup.gic.haveCyclicDecomposition() ) {
	
	AbelianGroup& A = (AbelianGroup&)
	  (theGroup.gic.getCyclicDecomposition());

	Chars explanation 
	  = Text("this follows from the canonical decomposition of")
	  + Name(theGroup);

	if ( theGroup.gic.isAbelian() != yes ) {
	  explanation += " abelianized";
	  if( A.order() == 0 )
	    theGroup.gic.putIsFinite(false, explanation);
	}
	else 
	  theGroup.gic.putIsFinite(A.order() != 0, explanation);

	LogMessage(*this, theGroup.gic.getFiniteMessage(), 2).send();
      }
      
      if ( theGroup.gic.haveOrder() ) 
	{
	  ostrstream msg;
	  msg << "according to the Todd-Coxeter algorithm";
	  
	  Chars explanation = msg.str();
	  theGroup.gic.putIsFinite( theGroup.gic.getOrder() != 0, explanation);
	  
	  LogMessage(*this, theGroup.gic.getFiniteMessage(), 2).send();
	  adminTerminate();
	}
    }

  
}


void IsFiniteProblem::start( ) { }


void IsFiniteProblem::terminate( ) { }


void IsFiniteProblem::viewStructure(ostream& ostr) const
{
  Chars abelianText;
  if( theGroup.gic.isAbelian() == yes )
    abelianText = Text("Compute the torsion-free rank of") + Name( theGroup );
  else
    abelianText =
      Text("Compute the torsion-free rank of the abelianization of")
      + Name( theGroup );
  

  ProblemView pv( ostr, oid(), Text("Finiteness problem"),
		  Text("Is") + Name( theGroup ) + Text("finite ?"),
		  helpID("IsFiniteProblem", theGroup),
		  Text(Name(theGroup)) + "finite ?"
		  );

    
  pv.startItemGroup();
  
  pv.add( Text(abelianText),
	  abelianRank.arcSlotID(),
	  15
	  );

  
  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::FP
		     );
  
  pv.add( Text("Enumerate cosets")
	  + Name( theGroup ),
	  theToddCoxeter.arcSlotID(),
	  100
	  ); 
  
  pv.add( Text("GH Todd-Coxeter")
	  + Name( theGroup ),
	  ghToddCoxeter.arcSlotID(),
	  100
	  ); 

  pv.add( Text("Seek a rewriting system for") + Name( theGroup ),
	  kbSupervisor.arcSlotID(),
	  50
	  );
  
  pv.add( Text("Seek a ShortLex automatic structure for") + Name( theGroup ),
	  agSupervisor.arcSlotID(),
	  50
	  );

  
  pv.done();
}

#line 1  "src/ExtendToHomProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes ExtendToHomChecker, ExtendToHomProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "ExtendToHomProblem.h"
#include "SMHomomorphism.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "OneRelatorGroup.h"

// ------------------------ ExtendToHomChecker ----------------------------- //


ExtendToHomChecker::ExtendToHomChecker(ExtendToHomProblem& boss,
					class SMMap& map)
  : theMap( map ),
    theBoss(boss),
    theDomain( map.getDomain() ),
    theRange( map.getRange() ),
    M( map.getMap() ),
    G1( map.getDomain().getFPGroup() ),
    G2( map.getRange().getFPGroup() ),
    gic1( map.getDomain().gic ),
    gcm1( map.getDomain().gcm() ),
    gic2( map.getRange().gic ),
    gcm2( map.getRange().gcm() ),
    triedAbelianization( false ),
    theClass( map.getRange().gic.getNilpotentcyClass() ),
    relators( getAllRelators(theDomain) )
//    triedPreliminaryCheckings( false )
{ }


Trichotomy ExtendToHomChecker::preliminaryCheckings()
{
  int numOfGens1 = G1.numberOfGenerators();
  SetOf<Word> S1 = G1.getRelators();
  SetIterator<Word> I1(S1);

  int numOfGens2 = G2.numberOfGenerators();
//  SetOf<Word> S2 = G2.getRelators();
//  SetIterator<Word> I2(S2);

/*
  if( numOfGens1 == 0 || numOfGens2 == 0 )
    return yes;
*/

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Free Group:                                                       //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if( theDomain.getCheckinType() == SMFPGroup::FREE ) {
    explanation = FEData::Text("this follows from the fact that the given"
      " generators freely generate") + FEData::Name(theDomain);
    return yes;
  }

  if( theRange.getCheckinType() == SMFPGroup::FREE ) {
    
    FreeGroup F = theRange.getFreePreimage();

    // Check images of all relators

    for( I1.reset(); !I1.done();  I1.next() )
      if( F.wordProblem( M.imageOf(I1.value()) ) == no )
	return no;

    if( gic1.isAbelian() == yes || gic1.isNilpotent() == yes || 
	gic1.isFreeNilpotent() == yes) {

      // In case abelian or nilpotent group images of all generators
      // of G1 must be powers with the same root.
      
      Word image = M.generatingImages(0);
      Word root = image.initialSegment( image.length()/maximalRoot( image ) );
      
      for( int i = 1; i < numOfGens1; ++i ) {
	image = M.generatingImages(i);
	Word tmp = image.initialSegment( image.length()/maximalRoot( image ) );
	if( tmp != root )
	  return no;
      }
    }

    return yes;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Map to MSC Group:                                                 //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if( gic2.haveMSC() ) {
    
    // Check images of all relators first
    
    for( I1.reset(); !I1.done();  I1.next() )
      if( gic2.getMSC( ).wordProblem( M.imageOf(I1.value()) ) == no )
	return no;
    
    if( gic1.isAbelian() == yes || gic1.isNilpotent() == yes || 
	gic1.isFreeNilpotent() == yes) {
      
      // In case abelian or nilpotent group all images
      // of generators of G1 have to commute in G2
      
      VectorOf<Word> V = M.generatingImages();
      for( int i = 0; i < numOfGens1; ++i )
	for( int j = 0; j < numOfGens1; ++j )
	  if( i != j ) {
	    Word comm = ( Word(V[i]).inverse() * Word(V[j]).inverse()
			  * V[i] * V[j] );
	    if( gic2.getMSC( ).wordProblem(comm) == no )
	      return no;
	  }
    }
    
    return yes;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Map to OR Group:                                                  //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if( gic2.isOneRelator() ) {
    
    // ORWP should work swiftly in this case.
    OneRelatorGroup orwp( G2.namesOfGenerators(), gic2.getOneRelator() );

    // Check images of all relators first
    
    for( I1.reset(); !I1.done();  I1.next() ) {
      
      if( !orwp.wordProblem( M.imageOf(I1.value()).freelyReduce() ) ) 
	return no;
    }
    
    if( gic1.isAbelian() == yes || gic1.isNilpotent() == yes || 
	gic1.isFreeNilpotent() == yes) {
      
      // In case abelian or nilpotent group all images
      // of generators of G1 have to commute in G2
      
      VectorOf<Word> V = M.generatingImages();
      for( int i = 0; i < numOfGens1; ++i )
	for( int j = 0; j < numOfGens1; ++j )
	  if( i != j ) {
	    Word comm = ( Word(V[i]).inverse() * Word(V[j]).inverse()
			  * V[i] * V[j]);
	    if( !orwp.wordProblem( comm.freelyReduce() ) )
	      return no;
	  }
    }
    
    return yes;
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Abelian Group:                                                    //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if( gic1.isAbelian() == yes && gic2.isAbelian() == yes
      && S1.cardinality() == 0 ) // free abelian -> abelian
    return yes;
  
/*  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Nilpotent Group:                                                  //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if( gic1.isNilpotent() == yes )
    error( "Trichotomy ExtendToHomChecker::preliminaryCheckings() : "
	   "Nilpotent case is not implemented yet.");
*/  
  
  return dontknow;
}


SetOf<Word> ExtendToHomChecker::getAllRelators( class SMFPGroup& group ) const
{
  if( group.gic.isAbelian() == yes )
    return AbelianGroup( group.getFPGroup() ).getAllRelators();
  else
    return group.getFPGroup().getRelators();
}


bool ExtendToHomChecker::checkRelatorsInNilpotent() const
{
  // The target group has to be nilpotent.
  
  SetIterator<Word> I(relators);
  NilpotentGroup NG( gic2.getNilpotentGroupInited() );
  for( ; !I.done(); I.next() ) {
    if( !NG.wordProblem(I.value()) )
      return false;
  }
  
  int class1;
  if( (gic1.isNilpotent() == yes || gic1.isFreeNilpotent() == yes)&&
      (class1 = gic1.getNilpotentcyClass()) < theClass ) {
    CommutatorIterator I(G1.numberOfGenerators(), class1 + 1);
    for( ; !I.done(); I.next() ) {
      if( !NG.wordProblem(I.value()) )
	return false;
    }
  }

  return true;
}


Trichotomy ExtendToHomChecker::doesExtendToHom( )
{
/*
  if( !triedPreliminaryCheckings ) {
    triedPreliminaryCheckings = true;
    Trichotomy prelResult = preliminaryCheckings();
    if ( prelResult != dontknow )
      return prelResult;
  }
*/
  int numOfGens1 = G1.numberOfGenerators();
  SetOf<Word> S1 = G1.getRelators();
  SetIterator<Word> I1(S1);

  int numOfGens2 = G2.numberOfGenerators();
//  SetOf<Word> S2 = G2.getRelators();
//  SetIterator<Word> I2(S2);

  SetIterator<Word> I(relators);

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the abelian quotient                                          //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( ! triedAbelianization && gic2.haveCyclicDecomposition( ) ) {
    
    triedAbelianization = true;
    const AbelianGroup& theGroup = gic2.getCyclicDecomposition();
    
    // If the group itself is abelian, we get a definite answer;
    // otherwise, we only succeed when the all images are non-trivial.
    
    if ( gic2.isAbelian() == yes ) {

      bool answer = yes;
      for( I1.reset(); !I1.done(); I1.next() )
	if( !theGroup.isTrivial( M.imageOf(I1.value()) ) ) {
	  answer = no;
	  break;
	}
     
//      explanation = "the canonical decomposition";
      return answer;
    }

    bool answer = yes;
    for( I.reset(); !I.done(); I.next() )
      if( !theGroup.isTrivial( M.imageOf(I.value()) ) ) {
	answer = no;
	break;
      }
    
    if( !answer ) {
//      explanation = "by mapping into the abelianization";
      return answer;
    }
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Nilpotent case                                                    //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  /*  if( gic2.isNilpotent() == yes && gic2.haveNilpotentQuot(theClass) ){

    if( !bNilpStarted ) {
      bNilpStarted = true;
      arcer.setArguments(this);
    }
    else
      if( arcer.takeControl() )
	return arcer.getRetValue();
  }
  */
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use nilpotent quotients                                           //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////
  if (theBoss.nilpWPInQuotAnswer() != dontknow ){
    //    explanation = "because of computed nilpotent quotients";
    return theBoss.nilpWPInQuotAnswer();
  }
  if (theBoss.nilpWPAnswer() != dontknow ){
    //    explanation = "because of computed nilpotent structure";
    return theBoss.nilpWPAnswer();
  }


  if( theRange.getCheckinType() != SMFPGroup::FP )
    return dontknow;
  

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the normal closure of the relators                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  // There are two cases:
    
  // 1) The group is finite, and a complete Cayley graph has been reported
  //    to the GIC.

  if ( gic2.haveCompleteCayleyGraph() ) {

    // We get a definite answer.
//    explanation = "by running through complete Cayley graph";
    SubgroupGraph SG = gic2.getCompleteCayleyGraph();

    for( I.reset(); !I.done(); I.next() )
      if( !SG.contains( (M.imageOf(I.value())).freelyReduce() ) )
	return no;
    
    if( gic1.isNilpotent() == yes || gic1.isFreeNilpotent() == yes) {
      CommutatorIterator CI(numOfGens1, gic1.getNilpotentcyClass());
      for( CI.reset(); !CI.done(); CI.next() )
	if( !SG.contains( (M.imageOf(CI.value())).freelyReduce() ) )
	  return no;
    }
    
    return yes;
  }
    
  // 2) Not 1, so we run the word through the existing normal closure
  //    approximation.

  if ( gcm2.normalClosure.exists() ) {
    
    bool bAnswer = true;
    for( I.reset(); !I.done(); I.next() )
      if( (gcm2.normalClosure->
	    isTrivial( (M.imageOf(I.value())).freelyReduce() ) ) != yes ) {
	bAnswer = false;
	break;
      }
    
    if( bAnswer && (gic1.isNilpotent() == yes || 
		    gic1.isFreeNilpotent() == yes)) {
      CommutatorIterator CI(numOfGens1, gic1.getNilpotentcyClass());
      for( CI.reset(); !CI.done(); CI.next() )
	if( (gcm2.normalClosure->
	      isTrivial( (M.imageOf(CI.value())).freelyReduce() ) )!=yes ) {
	  bAnswer = false;
	  break;
	}
    }
    
    if( bAnswer ) {
//      explanation = "labels loop in normal closure approximation";
      return yes;
    }
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use complete KBMachine                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic2.haveConfluentKBMachine() ) {

//    explanation = "according to the confluent rewrite system";

    for( I.reset(); !I.done(); I.next() ) {
      Word temp( (M.imageOf(I.value())).freelyReduce() );
      gic2.getConfluentKBMachine().rewrite( temp );
      if( temp.length() != 0 )
	return no;
    }

    if( gic1.isNilpotent() == yes || gic1.isFreeNilpotent() == yes) {
      CommutatorIterator CI(numOfGens1, gic1.getNilpotentcyClass());
      for( CI.reset(); !CI.done(); CI.next() ) {
	Word temp( (M.imageOf(CI.value())).freelyReduce() );
	gic2.getConfluentKBMachine().rewrite( temp );
	if( temp.length() != 0 )
	  return no;
      }
    }

    return yes;
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use automatic structure                                           //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic2.haveAutomatic() ) {

//    explanation = "according to the difference machine";

    for( I.reset(); !I.done(); I.next() ) {
      Word temp( (M.imageOf(I.value())).freelyReduce() );
      gic2.getDiffMachine().rewrite( temp );
      if( temp.length() != 0 )
	return no;
    }
    
    if( gic1.isNilpotent() == yes || gic1.isFreeNilpotent() == yes) {
      CommutatorIterator CI(numOfGens1, gic1.getNilpotentcyClass());
      for( CI.reset(); !CI.done(); CI.next() ) {
	Word temp( (M.imageOf(CI.value())).freelyReduce() );
	gic2.getDiffMachine().rewrite( temp );
	if( temp.length() != 0 )
	  return no;
      }
    }
    
    return yes;
  }

  return dontknow;
}


// ----------------------- ExtendToHomProblem ------------------------------ //


ExtendToHomProblem::ExtendToHomProblem(class SMMap& map)
  : Supervisor( ! map.getRange().gic.haveFastWordProblem() ),
    theMap( map ),
    theDomain( map.getDomain() ),
    theRange( map.getRange() ),
    theChecker( *this, map ),
    theAnswer( dontknow ),
    normalClosure( *this, map.getRange().gcm().normalClosure ),
    abelianInvariants( *this, map.getRange().gcm().abelianInvariants ),
    kbSupervisor( *this, map.getRange().gcm().kbSupervisor ),
    agSupervisor( *this, map.getRange().gcm().agSupervisor ),
    nilpotentQuotients( *this, map.getRange().gcm().nilpotentQuotients ),
    nilpotentWPInQuotients(*this),
    computeBasis(*this, map.getRange().gcm().computeBasis),
    nilpotentWP(*this),
    genetic(*this),
    theClass( theRange.gic.getNilpotentcyClass() )
{
  SetIterator<Word> I(theDomain.getFPGroup().getRelators());
  SetOf<Word> comms;
  Map m = theMap.getMap();
  for( I.reset(); !I.done(); I.next() )
    comms |=  (m.imageOf(I.value())).freelyReduce();
  nilpotentWPInQuotients->initialize(comms,&theRange);
  nilpotentWP->initialize(comms,&theRange);
  genetic->init(theRange.getFPGroup(), comms, GeneticWPCM::SET_OF_WORDS);

  if ( ! displayInFE() ) adminStart();
  if( theMap.mic.doesExtendToHom() != dontknow )
    resultIsFast();
  if( (theAnswer = theChecker.preliminaryCheckings()) != dontknow )
    resultIsFast();
  if( theRange.gic.isAbelian() == yes 
      && theRange.gic.haveCyclicDecomposition() )
    resultIsFast();
}
  
Trichotomy ExtendToHomProblem::nilpWPAnswer( ) 
{
  if (nilpotentWP->state() == ComputationManager::TERMINATED){
    Trichotomy answer = nilpotentWP->isTrivial();
    return answer;
  }
  else
    return dontknow;
}

Trichotomy ExtendToHomProblem::nilpWPInQuotAnswer( ) 
{
  if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
    if (nilpotentWPInQuotients->isTrivial()==no)
      return no;
  return dontknow;
}

void ExtendToHomProblem::sendResultMessage( Chars explanation )
{
  if( explanation.length() == 0 )
    if( theAnswer == yes )
      explanation = "the images of the generators satisfy the given relations";
    else
      explanation = "the images of the generators do not"
	" satisfy the given relations";

  theMap.mic.putDoesExtendToHom(theAnswer, explanation);
  
  if( theAnswer == yes ) {
    
    Chars text = Text("The extension of") + Name(theMap) + 
      Text("to an homomorphism of") + Name( theDomain ) + "to"
      + Name( theRange );
      
    if( theDomain.oid() == theRange.oid() ) {
      SMHomomorphism* smo = new SMHomomorphism(theMap, text);
      ListOf<OID> dependencies( theMap.getDomain() );
      CheckinMessage( *smo, "", dependencies ).send();
    }
    else {
      SMHomomorphism2* smo = new SMHomomorphism2(theMap, text);
      ListOf<OID> dependencies( theMap.getDomain() );
      dependencies.append( theMap.getRange() );
      CheckinMessage( *smo, "", dependencies ).send();
    }
  }
}


void ExtendToHomProblem::takeControl( )
{
  if( theMap.mic.doesExtendToHom() != dontknow ) {
    LogMessage(theMap.mic.getExtendToHomMessage()).send();
    return;
  }

  if( theAnswer != dontknow ) {
    sendResultMessage(theChecker.getExplanation());
//    adminTerminate();
    return;
  }

  if ( !displayInFE() || freeARCs() > 0 ) {
    if ( ( theAnswer = theChecker.doesExtendToHom() ) != dontknow ) {
      sendResultMessage(theChecker.getExplanation());
      adminTerminate();
    }
    else if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
      explanation = "according to the genetic algorithm";
      sendResultMessage(explanation);
      adminTerminate();
    }    
    else
      usedOneARC();
  }
}


void ExtendToHomProblem::start( ) { }


void ExtendToHomProblem::terminate( ) { }


void ExtendToHomProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Extend to an homomorphism problem"),
		  Text("Does") + Name( theMap ) + "extend to an homomorphism ?",
		  helpID("ExtendToHomProblem", theMap.getDomain()),
		  "Ext to hom ?"
		  );

  
  pv.startItemGroup();

  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50
	  );


  pv.startItemGroup( );
  
  pv.add( Text("Compute abelian invariants of") + Name( theRange ),
	  abelianInvariants.arcSlotID(),
	  15
	  );


  pv.startItemGroup( CheckinType( theRange ) == SMFPGroup::NILPOTENT ||
		     CheckinType( theRange ) == SMFPGroup::FREE_NILPOTENT
		     );

  if ( ! theRange.gic.haveNilpotentGroupInited( ) ) {
    
    pv.add(Text("Compute a basis of ")
	   + Name(theRange),
	   computeBasis.arcSlotID(),
	   100
	   );
  }
  pv.add(Text("Solve using basis of ")
	 + Name( theRange ),
	 nilpotentWP.arcSlotID(),
	 100
	 );
 
  pv.startItemGroup( CheckinType( theRange ) == SMFPGroup::FP
		     );
 
  pv.add( Text("Enumerate normal closure of relators"),
	  normalClosure.arcSlotID(), 50);

  
  pv.add( Text("Seek a rewriting system for") + Name( theRange ),
	  kbSupervisor.arcSlotID(),
	  50
	  );
  
  pv.add( Text("Seek a ShortLex automatic structure for")
	  + Name( theRange ),
	  agSupervisor.arcSlotID(),
	  50
	  );

  pv.add( Text("Run genetic algorithm"), genetic.arcSlotID(), 50 );
  
  pv.startItemGroup( CheckinType(theRange ) == SMFPGroup::NILPOTENT ||
		     CheckinType(theRange ) == SMFPGroup::FREE_NILPOTENT ||
		     CheckinType(theRange ) == SMFPGroup::FP
		     );
  
  int upperBound, currentClass = 2;
  if (theRange.gic.isNilpotent()==yes || theRange.gic.isFreeNilpotent()==yes)
    upperBound = theRange.gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theRange.gic.haveNilpotentGroupInited( ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute the lower central quotients for") + Name( theRange ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theRange ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );
  
  pv.done();
}
#line 1  "src/IsAbelianProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes IsAbelianChecker, IsAbelianProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "IsAbelianProblem.h"
#include "CommutatorIterator.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "ORProblems.h"
#include "DecomposeInSubgroup.h"

// ------------------------- IsAbelianChecker ------------------------------ //


IsAbelianChecker::IsAbelianChecker(class SMFPGroup& group)
  : G( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false ),
    triedOneRelator( false ),
    abelianDetails( group.getFPGroup() )
{ }


Chars IsAbelianChecker::commutator( VectorOf<int> components )
{
  ostrstream msg;
  msg << "\\[";

  int len = components.length();
  for( int i = 0; i < len; ++i ) {
    G.printWord( msg, Word(Generator(components[i])) );
    if( i != len-1 )
      msg << ",";
  }

  msg << "\\]" << ends;
  return msg.str();
}

Trichotomy IsAbelianChecker::isAbelian( )
{
  int numOfGens = G.numberOfGenerators();

  if( (numOfGens - G.getRelators().cardinality()) > 1 ) {
    
    explanation = "the group contains non-abelian free subgroup"; 
    return no;
  }


  CommutatorIterator I(numOfGens, 2);

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use one relator with torsion                                      //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.isOneRelatorWithTorsion() ) {

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() )
      if( G.shortenByRelators(I.value()).length() != 0 ) {
	allTrivial = false;
	break;
      }

    explanation = "according to Dehn's algorithm (one relator with torsion) ";

    if( allTrivial ) 
      explanation += "all commutators are trivial";
    else 
      explanation += Chars("the commutator ") + commutator(I.components()) 
	+ " is not trivial";
    
    return allTrivial;
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use one relator                                                   //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////
  
  if ( gic.isOneRelator() ) {

    bool result = ORProblems( numOfGens, gic.getOneRelator() ).isAbelian();

    if( numOfGens <= 1)
      explanation = "number of generators of the one relator group is less"
	" than two";
    else if( numOfGens > 2)
      explanation = "number of generators of the one relator group is greater"
	" than two";
    else
      if( result ) 
	explanation = "all commutators are trivial";
      else {
	VectorOf<int> v(2);
	v[0] = 1; v[1] = 2;
	explanation = Chars("the commutator ") + commutator(v) 
	  + " is not trivial";
      }
    
    return result;
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use small cancellation                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveMSC() ) {

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() )
      if( !gic.getMSC().wordProblem(I.value()) ) {
	allTrivial = false;
	break;
      }
    
    explanation = "according to Dehn's algorithm (small cancellation) ";
    
    if( allTrivial ) 
      explanation += "all commutators are trivial";
    else 
      explanation += Chars("the commutator ") + commutator(I.components()) 
	+ " is not trivial";

    return allTrivial;
  }
  

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the normal closure of the relators                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  // There are two cases:
    
  // 1) The group is finite, and a complete Cayley graph has been reported
  //    to the GIC.

  if ( gic.haveCompleteCayleyGraph() ) {
    // We get a definite answer.


    explanation = "according to complete Cayley graph ";

    bool allTrivial = true;
    bool header = true;

    for( I.reset(); !I.done(); I.next() ) {

      if( !gic.getCompleteCayleyGraph().contains(I.value()) ) {

	explanation += 
	  Chars("the commutator ") + commutator(I.components()) 
	  + " is not trivial";
	
	abelianDetails.printNonTrivialCommutatorDetails( 
          commutator(I.components()), explanation, header 
	);

	allTrivial = false;
	break;
      }
      else if( gic.haveWordDecomposer() ) {
	abelianDetails.printTrivialCommutatorDetails(
	  commutator(I.components()), 
	  gic.getWordDecomposer().fullDecomposition(I.value()),
	  header
	);
	header = false;
      }
    }
    
    if( allTrivial ) 
      explanation += "all commutators are trivial";

    return allTrivial;
  }
    
  // 2) Not 1, so we run the word through the existing normal closure
  //    approximation.

  if ( gcm.normalClosure.exists() && gic.haveWordDecomposer() ) {

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() )
      if( gcm.normalClosure->isTrivial( I.value() ) != yes ) {
	allTrivial = false;
	break;
      }

    if( allTrivial ) {
      explanation = "according to normal closure approximation "
	"all commutators are trivial";

      bool header = true;
      for( I.reset(); !I.done(); I.next() ) {
	abelianDetails.printTrivialCommutatorDetails(
	  commutator(I.components()), 
	  gic.getWordDecomposer().fullDecomposition(I.value()),
	  header
	);
	header = false;
      }

      return yes;
    }
  }

  //@db need to replace abelianization by nilpotent quotients
  //@am abelianization is used in nilpotent groups: if abelianization
  // iz cyclic, then nilpotent group is abelian
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the abelian quotient                                          //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( ! triedAbelianization && gic.haveCyclicDecomposition( ) ) {
    
    triedAbelianization = true;
    
    if ( gic.isFreeNilpotent() == yes ||
	 gic.isNilpotent() == yes) {
      const AbelianGroup& ag = gic.getCyclicDecomposition( );
      if ((ag.rankOfFreeAbelianFactor() == 0 && ag.invariants().length() ==1) ||
	  (ag.rankOfFreeAbelianFactor() == 1 && ag.invariants().length() ==0)){
	gic.putActualNilpotencyClass(1);
      explanation = "this follows from the fact that it's abelianization is cyclic";
	return true;
      }
    }
  }
  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use complete KBMachine                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveConfluentKBMachine() ) {
    // Yay! But we have to ask the KBMachine for the answer

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() ) {
      Word temp( I.value() );
      gic.getConfluentKBMachine().rewrite( temp );
      if( temp.length() != 0 ) {
	allTrivial = false;
	break;
      }
    }

    explanation = "according to the confluent rewriting system ";

    if( allTrivial ) 
      explanation += "all commutators are trivial";
    else 
      explanation += Chars("the commutator ") + commutator(I.components()) 
	+ " is not trivial";
    
    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use automatic structure                                           //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveAutomatic() ) {
    // Yay! But we have to ask the DiffMachine for the answer

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() ) {
      Word temp( I.value() );
      gic.getDiffMachine().rewrite( temp );
      if( temp.length() != 0 ) {
	allTrivial = false;
	break;
      }
    }
    
    explanation = "according to the difference machine ";

    if( allTrivial ) 
      explanation += "all commutators are trivial";
    else 
      explanation += Chars("the commutator ") + commutator(I.components()) 
	+ " is not trivial";
    
    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use any nilpotent quotients                                       //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use (possibly partial) KBMachines                                 //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Give up for now                                                   //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  return dontknow;
}


// -------------------------- IsAbelianProblem ---------------------------- //


IsAbelianProblem::IsAbelianProblem(SMFPGroup& G)
  : Supervisor( ! G.gic.haveFastWordProblem() ),
    theGroup( G ),
    theChecker( G ),
    normalClosure( *this, G.gcm().normalClosure ),
    abelianInvariants( *this, G.gcm().abelianInvariants ),
    kbSupervisor( *this, G.gcm().kbSupervisor ),
    agSupervisor( *this, G.gcm().agSupervisor ),
    nilpotentQuotients(*this, G.gcm().nilpotentQuotients),
    nilpotentWPInQuotients(*this),
    computeBasis(*this, G.gcm().computeBasis),
    nilpotentWP(*this),
    genetic( *this )
{
  nilpotentWPInQuotients->initialize(2,&theGroup);
  nilpotentWP->initialize(2,&theGroup);

  SetOf<Word> comms;
  CommutatorIterator I(G.getFPGroup().numberOfGenerators(), 2);
  for( ; !I.done(); I.next() ) comms |= I.value();
  genetic->init(G.getFPGroup(), comms, GeneticWPCM::COMMUTATORS);
  
  if ( ! displayInFE() ) adminStart();
}


void IsAbelianProblem::takeControl( )
{
  Trichotomy theAnswer = dontknow;
  Chars explanation;

  if ( ! displayInFE() || freeARCs() > 0 ) {
    if (nilpotentWP->state() == ComputationManager::TERMINATED){
      theAnswer = nilpotentWP->isTrivial();
      explanation = ": according to an inspection of a basis, ";
      if( theAnswer ) 
	explanation += Chars("all commutators are trivial.");
      else 
	explanation += Chars("the commutator ") + 
	  theChecker.commutator(nilpotentWP->getCommutator())+ 
	  " is not trivial.";
      
    }
    if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
      if (nilpotentWPInQuotients->isTrivial()==no){
	int retClass;
	theAnswer = nilpotentWPInQuotients->isTrivial(retClass);
	ostrstream msgTmp;
	msgTmp << ": because the commutator " << 
	  theChecker.commutator(nilpotentWPInQuotients->getCommutator()) << 
	  " is not trivial in lower central quotient of class " 
	       << retClass << ends;
	explanation = msgTmp.str();
      }    
    if (theAnswer != dontknow){
 	LogMessage msg( *this, theGroup );
	msg << Name( theGroup ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "abelian" << explanation;
	msg.send();
	if( theAnswer == yes ) theGroup.gic.putIsAbelian();
	//if (theGroup.gic.isNilpotent())
	//theGroup.gic.putActualNilpotencyClass(1);
	adminTerminate();     
	return;     
    }

    if ( ( theAnswer = theChecker.isAbelian() ) != dontknow ) {

      LogMessage msg( *this, theGroup );
      msg << Name( theGroup ) << " is ";
      if ( theAnswer == no ) msg << "not ";
      msg << "abelian";

      explanation = theChecker.getExplanation();
      if( explanation.length() > 0 )
	msg << " : " << explanation;

      msg << ".";
      msg.send();

      if( theChecker.getDetailsFileName().length() > 0 ) {
	LogMessage msgLink( *this, theGroup );
	msgLink << Link(Chars("Click here to see a decomposition of "
			      "trivial commutators of ") 
			+ Text(Name(theGroup))  
			+ Text(" as a product of conjugates of the relators."),
			"IsAbelianDetails", 
			theChecker.getDetailsFileName()
			);
	msgLink.send();
      }

      if( theAnswer == yes ) theGroup.gic.putIsAbelian();
      //if (theGroup.gic.isNilpotent())
      //theGroup.gic.putActualNilpotencyClass(1);
      adminTerminate();
    } else
      
      if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
	
	LogMessage msg( *this, theGroup );
	msg << Name( theGroup ) << " is ";
	msg << "abelian";
	Chars explanation = "according to the genetic algorithm";
	msg << " : " << explanation << ".";
	msg.send();
	if( theAnswer == yes ) theGroup.gic.putIsAbelian();
	adminTerminate();
      }
    
      else
	usedOneARC();
  }
}


void IsAbelianProblem::start( ) { }


void IsAbelianProblem::terminate( ) { }


void IsAbelianProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is abelian problem"),
		  Text("Is") + Name( theGroup ) + "abelian ?",
		  helpID("IsAbelianProblem", theGroup),
		  Text(Name(theGroup)) + "abelian ?"
		  );
  

  pv.startItemGroup( CheckinType(theGroup) == SMFPGroup::FP ||
		     CheckinType(theGroup) == SMFPGroup::NILPOTENT
		     );
  
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);


  if ( (theGroup.gic.isFreeNilpotent() == yes ||
	theGroup.gic.isNilpotent() == yes) && 
       !theGroup.gic.haveCyclicDecomposition()) 
    
    pv.add(Text("Compute abelian invariants of") + Name(theGroup)
	   + "abelianized",
	   abelianInvariants.arcSlotID(),
	   15
	   );
  

  int upperBound, currentClass = 2;
  if (theGroup.gic.isNilpotent()==yes)
    upperBound = theGroup.gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theGroup.gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute lower central quotients for") + Name( theGroup ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theGroup ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );
 
  pv.startItemGroup( CheckinType(theGroup) == SMFPGroup::FP);
  
  pv.add(Text("Enumerate normal closure of relators"),
	 normalClosure.arcSlotID(), 50);
  
  pv.add(Text("Seek a rewriting system for") + Name(theGroup),
	 kbSupervisor.arcSlotID(),
	 50
	 );
  
  pv.add(Text("Seek a ShortLex automatic structure for")
	 + Name(theGroup),
	 agSupervisor.arcSlotID(),
	 50
	 );

  pv.add(Text("Run genetic algorithm"),
	 genetic.arcSlotID(), 50);

  pv.startItemGroup( CheckinType(theGroup) == SMFPGroup::NILPOTENT);

  if (!theGroup.gic.haveNilpotentQuotInited(theGroup.gic.getNilpotentcyClass()))
    pv.add(Text("Compute a basis of ") + Name( theGroup ),
	   computeBasis.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using a basis of")
	 + Name( theGroup ),
	 nilpotentWP.arcSlotID(),
	 50
	 );
  
  pv.done();
}
#line 1  "src/IsNilpotentProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class IsNilpotentProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "IsNilpotentProblem.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"


// -------------------------- IsNilpotentProblem --------------------------- //


IsNilpotentProblem::IsNilpotentProblem(SMFPGroup& G, int nilpClass)
  : Supervisor( true/*! G.gic.haveFastWordProblem()*/ ),
    theGroup( G ),
    theClass(nilpClass),
    theChecker( G, nilpClass+1 ),
    theArcer( *this ),
    normalClosure( *this, G.gcm().normalClosure ),
    //    abelianInvariants( *this, G.gcm().abelianInvariants ),
    kbSupervisor( *this, G.gcm().kbSupervisor ),
    agSupervisor( *this, G.gcm().agSupervisor ),
    nilpotentQuotients(*this, G.gcm().nilpotentQuotients),
    nilpotentWPInQuotients(*this)
{
  nilpotentWPInQuotients->initialize(theClass+1,&theGroup,theClass+1);
  if ( ! displayInFE() ) adminStart();
  theArcer.setArguments(&theChecker);
}


void IsNilpotentProblem::takeControl( )
{
  Trichotomy theAnswer = dontknow;
  Chars explanation;

  if ( ! displayInFE() || freeARCs() > 0 ) {
    if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
      if (nilpotentWPInQuotients->isTrivial()==no){
	int retClass;
	theAnswer = nilpotentWPInQuotients->isTrivial(retClass);
	ostrstream msgTmp;
	msgTmp << ": because there are not trivial commutators in lower central quotient of class " 
	       << retClass << ends;
	explanation = msgTmp.str();
      }    
    if (theAnswer != dontknow){
 	LogMessage msg( *this, theGroup );
	msg << Name( theGroup ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "nilpotent of class " << theClass << explanation;
	msg.send();
	if( theAnswer == yes ) theGroup.gic.putIsNilpotent(theClass);
	adminTerminate();     
	return;     
    }
    if( theArcer.takeControl() ) 
      if( (theAnswer = theArcer.getRetValue()) != dontknow ) {
	LogMessage msg( *this, theGroup );
	msg << Name( theGroup ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "nilpotent of class " << theClass;
	
	explanation = theChecker.getExplanation();
	if( explanation.length() > 0 )
	  msg << " : " << explanation;
	
	msg << ".";
	msg.send();
	if( theAnswer == yes ) theGroup.gic.putIsNilpotent(theClass);
	adminTerminate();
	return;
      }

    usedOneARC();
  }
}


void IsNilpotentProblem::viewStructure(ostream& ostr) const
{
  char classStr[10];
  sprintf(classStr,"%u",theClass);

  ProblemView pv( ostr, oid(), Text("Is nilpotent problem"),
		  Text("Is") + Name( theGroup ) + "nilpotent of class"
		  + classStr + "?",
		  helpID("IsNilpotentProblem", theGroup),
		  Text(Name(theGroup)) + "nilpotent ?"
		  );
  

  pv.startItemGroup( CheckinType(theGroup) == SMFPGroup::FP ||
		     CheckinType(theGroup) == SMFPGroup::SMALL_CANCELLATION ||
		     CheckinType(theGroup) == SMFPGroup::ONE_RELATOR ||
		     CheckinType(theGroup) == SMFPGroup::NILPOTENT
		     );
  
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  /*  @am we don't need abelianization here
      pv.add(Text("Compute abelian invariants of") + Name(theGroup)
	 + "abelianized",
	 abelianInvariants.arcSlotID(),
	 15
	 );
	 */
  pv.startItemGroup( CheckinType(theGroup) == SMFPGroup::FP ||
		     CheckinType(theGroup) == SMFPGroup::SMALL_CANCELLATION ||
		     CheckinType(theGroup) == SMFPGroup::ONE_RELATOR 
		     );
  pv.add(Text("Enumerate normal closure of relators"),
	 normalClosure.arcSlotID(), 50);
  
  pv.add(Text("Seek a rewriting system for") + Name(theGroup),
	 kbSupervisor.arcSlotID(),
	 50
	 );
    
  pv.add(Text("Seek a ShortLex automatic structure for")
	 + Name(theGroup),
	 agSupervisor.arcSlotID(),
	 50
	 );

  pv.startItemGroup(CheckinType( theGroup ) == SMFPGroup::FP
		     );

  int upperBound, currentClass = 2;
  if (theGroup.gic.isNilpotent()==yes)
    upperBound = theGroup.gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theGroup.gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute lower central quotients for") + Name( theGroup ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theGroup ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );
  pv.done();
}
#line 1  "src/SetOfWordsChecker.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class SetOfWordsChecker
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "SetOfWordsChecker.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "conversions.h"
#include "ShortenByRelators2.h"

// ----------------------- SetOfWordsChecker ------------------------------- //


SetOfWordsChecker::SetOfWordsChecker( const VectorOf<Word>& words,
				      const SMFPGroup& group )
  : theWords( words ),
    G( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false ),
    solutionAlgorithm( GIC::NONE ),
    keepDetails( false ),
    dehnsDetails( group.getFPGroup() ),
    wordsDecomposition( group.getFPGroup() )
{ 
  init();
}


SetOfWordsChecker::SetOfWordsChecker( const SetOf<Word>& words,
				      const class SMFPGroup& group )
  : G( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false ),
    solutionAlgorithm( GIC::NONE ),
    theWords( makeVectorOf(words) ),
    dehnsDetails( group.getFPGroup() ),
    wordsDecomposition( group.getFPGroup() )
{
  init();
}


SetOfWordsChecker::SetOfWordsChecker( const class SMFPGroup& group )
  : G( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false ),
    solutionAlgorithm( GIC::NONE ),
    dehnsDetails( group.getFPGroup() ),
    wordsDecomposition( group.getFPGroup() )
{ 
  init();
}

void SetOfWordsChecker::init()
{
  theTrivialWords = VectorOf<bool>( theWords.length() );
  for( int i = 0; i < theTrivialWords.length(); ++i ) {
    theTrivialWords[i] = false;
  }
}

void SetOfWordsChecker::replaceTheSet( const VectorOf<Word>& V) 
{
  theWords = V; 
  solutionAlgorithm = GIC::NONE;
  init();
}

void SetOfWordsChecker::replaceTheSet( const SetOf<Word>& words )
{
 replaceTheSet( makeVectorOf( words ) );
}

Trichotomy SetOfWordsChecker::isTrivial( )
{
  // We might consider something like storing a pointer to a member
  // function which does exactly one of the below, when we know that that
  // solves the problem. Trouble is, many of these are only theoretical
  // `solutions', and may not succeed in practice.

  int len = theWords.length();
  
  bool allTrivial = true;
  for( int i = 0; i < len; ++i )
    if( Word(theWords[i]).length() != 0 ) {
      allTrivial = false;
      break;
    }
  if ( allTrivial ) return yes;

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use one relator with torsion                                      //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.isOneRelatorWithTorsion() == true ) {

    bool allTrivial = true;
    for( int i = 0; i < len; ++i )
      if( G.shortenByRelators(theWords[i]).length() != 0 ) {
	allTrivial = false;
	break;
      }
    
    if( len == 1 ) 
      dehnsDetails.printDehnTransformationDetails( theWords[0] );

    explanation = 
      "this follows by using Dehn's algorithm (the word problem for "
      "one-relator groups with torsion can be solved by using Dehn's "
      "algorithm)";

    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use small cancellation                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveMSC() ) {

    bool allTrivial = true;
    for( int i = 0; i < len; ++i )
      if( !gic.getMSC().wordProblem(theWords[i]) ) {
	allTrivial = false;
	break;
      }

    if( len == 1 ) 
      dehnsDetails.printDehnTransformationDetails( theWords[0] );

    explanation = "by Dehn's algorithm (small cancellation)";
    return allTrivial;
  }
  

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use free-by-cyclic structure                                      //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.isFreeByCyclic() ) {

    bool allTrivial = true;
    for( int i = 0; i < len; ++i )
      if( gic.getFreeByCyclic().normalForm( theWords[i] ).length() > 0 ) {
	allTrivial = false;
	break;
      }

    explanation = "by normal form in free-by-cyclic";
    return allTrivial;
  }


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the normal closure of the relators                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  // There are two cases:
    
  // 1) The group is finite, and a complete Cayley graph has been reported
  //    to the GIC.

  if ( gic.haveCompleteCayleyGraph() ) {
    // We get a definite answer.

    explanation = "by running through complete Cayley graph";

    bool allTrivial = true;
    bool header = ! wordsDecomposition.haveDetails();
    for( int i = 0; i < len; ++i ) {
      if( !theTrivialWords[i] ) {
	if( !gic.getCompleteCayleyGraph().contains( theWords[i] ) ) {
	  if( keepDetails ) {
	    wordsDecomposition.printNonTrivialWordDetails( 
              theWords[i], explanation, header );
	    header = false;
	  }
	  allTrivial = false;
	  break;
	}
	else if( keepDetails && gic.haveWordDecomposer() ) {
	  wordsDecomposition.printTrivialWordDetails( 
	    theWords[i], 
	    gic.getWordDecomposer().fullDecomposition(theWords[i]),
	    header
	    );
	  header = false;
	}
      }
    } // for( int i = ...

    return allTrivial;
  }
    
  // 2) Not 1, so we run the word through the existing normal closure
  //    approximation.

  if ( !keepDetails && gcm.normalClosure.exists() 
       || keepDetails && gic.haveWordDecomposer() ) {

    bool allTrivial = true;
    bool header = ! wordsDecomposition.haveDetails();
    for( int i = 0; i < len; ++i ) {
      if( !theTrivialWords[i] ) {
	if( gcm.normalClosure->isTrivial( theWords[i] ) != yes ) {
	  allTrivial = false;
	  break;
	}
	else {
	  if( keepDetails && gic.haveWordDecomposer() ) {
	    wordsDecomposition.printTrivialWordDetails( 
	      theWords[i], 
	      gic.getWordDecomposer().fullDecomposition(theWords[i]),
	      header
	      );
	    header = false;
	  }
	  theTrivialWords[i] = true;
	}
      }
    } // for(...

    if( allTrivial ) {
      solutionAlgorithm = GIC::NORM_CLOSURE;

      if( theWords.length() == 1 ) {
	//	printReductionDetailsByNormalClosure();
      }

      explanation = "labels loop in normal closure approximation";
      return yes;
    }
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the abelian quotient                                          //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( ! triedAbelianization && gic.haveCyclicDecomposition( ) ) {
    
    triedAbelianization = true;
    
    // If the group itself is abelian, we get a definite answer;
    // otherwise, we only succeed when the images are non-trivial.
	 
    if ( gic.isAbelian() == yes ) {
      
      bool allTrivial = true;
      for( int i = 0; i < len; ++i )
	if( !gic.getCyclicDecomposition().isTrivial( theWords[i] ) ) {
	  allTrivial = false;
	  break;
	}

      solutionAlgorithm = GIC::AB_INV;
      explanation = "this follows from the canonical decomposition";
      return allTrivial;
    }
    else {

      bool allTrivial = true;
      for( int i = 0; i < len; ++i )
	if( !gic.getCyclicDecomposition().isTrivial( theWords[i] ) ) {
	  allTrivial = false;
	  break;
	}
 
      if ( !allTrivial ) {
	solutionAlgorithm = GIC::AB_INV;
	explanation = "this follows from an inspection of the abelianization of the group";
	return no;
      }
    }
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use complete KBMachine                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveConfluentKBMachine() ) {
    // Yay! But we have to ask the KBMachine for the answer

    bool allTrivial = true;
    for( int i = 0; i < len; ++i ) {
      Word temp( theWords[i] );
      gic.getConfluentKBMachine().rewrite( temp );
      if( temp.length() != 0 ) {
	allTrivial = false;
	break;
      }
    }
    solutionAlgorithm = GIC::REWR_SYSTEM;
    explanation = "according to the confluent rewriting system";
    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use automatic structure                                           //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveAutomatic() ) {
    // Yay! But we have to ask the DiffMachine for the answer

    bool allTrivial = true;
    for( int i = 0; i < len; ++i ) {
      Word temp( theWords[i] );
      gic.getDiffMachine().rewrite( temp );
      if( temp.length() != 0 ) {
	allTrivial = false;
	break;
      }
    }

    solutionAlgorithm = GIC::AUT_STRUCTURE;
    explanation = "according to the difference machine";
    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use any nilpotent quotients                                       //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use (possibly partial) KBMachines                                 //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Give up for now                                                   //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  return dontknow;
}
#line 1  "src/CommutatorsChecker.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes CommutatorsChecker,
//           CommutatorsCheckerARCer
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "CommutatorsChecker.h"
#include "CommutatorIterator.h"
#include "SMFPGroup.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"


// ------------------------- CommutatorsChecker ---------------------------- //


CommutatorsChecker::CommutatorsChecker( class SMFPGroup& group, int length)
  : G( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false ),
    theLength( length ),
    generators( group.getFPGroup().numberOfGenerators() )
{
  int genLen = generators.length();
  for( int i = 0; i < genLen; ++i )
    generators[i] = Word(Generator(i+1));
}


CommutatorsChecker::CommutatorsChecker( class SMFPGroup& group, int length,
					const VectorOf<Word>& V )
  : G( group.getFPGroup() ),
    gic( group.gic ),
    gcm( group.gcm() ),
    triedAbelianization( false ),
    theLength( length ),
    generators( V )
{ }


Trichotomy CommutatorsChecker::areTrivial( )
{
  // We might consider something like storing a pointer to a member
  // function which does exactly one of the below, when we know that that
  // solves the problem. Trouble is, many of these are only theoretical
  // `solutions', and may not succeed in practice.

  CommutatorIterator I(generators, theLength);

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use one relator with torsion                                      //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.isOneRelatorWithTorsion() == true ) {

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() )
      if( G.shortenByRelators(I.value()).length() != 0 ) {
	allTrivial = false;
	break;
      }

    explanation = "by Dehn's algorithm (one relator with torsion)";
    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use small cancellation                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveMSC() ) {

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() )
      if( !gic.getMSC().wordProblem(I.value()) ) {
	allTrivial = false;
	break;
      }
    
    explanation = "by Dehn's algorithm (small cancellation)";
    return allTrivial;
  }
  

  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the normal closure of the relators                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  // There are two cases:
    
  // 1) The group is finite, and a complete Cayley graph has been reported
  //    to the GIC.

  if ( gic.haveCompleteCayleyGraph() ) {
    // We get a definite answer.

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() )
      if( !gic.getCompleteCayleyGraph().contains(I.value()) ) {
	allTrivial = false;
	break;
      }

    explanation = "by running through complete Cayley graph";
    return allTrivial;
  }
    
  // 2) Not 1, so we run the word through the existing normal closure
  //    approximation.

  if ( gcm.normalClosure.exists() ) {

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() )
      if( gcm.normalClosure->isTrivial( I.value() ) != yes ) {
	allTrivial = false;
	break;
      }

    if( allTrivial ) {
      explanation = "labels loop in normal closure approximation";
      return yes;
    }
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use the abelian quotient                                          //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( ! triedAbelianization && gic.haveCyclicDecomposition( ) ) {
    
    triedAbelianization = true;
    
    // If the group itself is abelian, we get a definite answer;
    // otherwise, we only succeed when the images are non-trivial.
	 
    if ( gic.isAbelian() == yes ) {
      
      bool allTrivial = true;
      for( I.reset(); !I.done(); I.next() )
	if( !gic.getCyclicDecomposition().isTrivial( I.value() ) ) {
	  allTrivial = false;
	  break;
	}
      
      explanation = "this follows from the canonical decomposition";
      return allTrivial;
    }
    else {

      bool allTrivial = true;
      for( I.reset(); !I.done(); I.next() )
	if( !gic.getCyclicDecomposition().isTrivial( I.value() ) ) {
	  allTrivial = false;
	  break;
	}
 
      if ( !allTrivial ) {
	explanation = "this follows from an inspection of the abelianization of the group";

	return no;
      }
    }
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use complete KBMachine                                            //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveConfluentKBMachine() ) {
    // Yay! But we have to ask the KBMachine for the answer

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() ) {
      Word temp( I.value() );
      gic.getConfluentKBMachine().rewrite( temp );
      if( temp.length() != 0 ) {
	allTrivial = false;
	break;
      }
    }

    explanation = "according to the confluent rewrite system";
    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use automatic structure                                           //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  if ( gic.haveAutomatic() ) {
    // Yay! But we have to ask the DiffMachine for the answer

    bool allTrivial = true;
    for( I.reset(); !I.done(); I.next() ) {
      Word temp( I.value() );
      gic.getDiffMachine().rewrite( temp );
      if( temp.length() != 0 ) {
	allTrivial = false;
	break;
      }
    }
    
    explanation = "according to the difference machine";
    return allTrivial;
  }

  
  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use any nilpotent quotients                                       //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Use (possibly partial) KBMachines                                 //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////////////////
  //                                                                   //
  // Give up for now                                                   //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  return dontknow;
}


// ---------------------- CommutatorsCheckerARCer -------------------------- //


void CommutatorsCheckerARCer::setArguments(CommutatorsChecker* checker)
{
  theChecker = checker;
}


void CommutatorsCheckerARCer::runComputation( )
{
  retValue = theChecker->areTrivial();
}


void CommutatorsCheckerARCer::writeResults( ostream& out ) 
{
  out < (*theChecker) < retValue;
}


void CommutatorsCheckerARCer::readResults( istream& in )
{
  in > (*theChecker) > retValue;
}
#line 1  "src/OrderOfElt.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes OrderOfEltARCer, OrderOfElt
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "OrderOfElt.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "viewStructure.h"

//---------------------------------------------------------------------------//
//---------------------------- OrderOfEltInQuotients  -----------------------//
//---------------------------------------------------------------------------//


OrderOfEltInQuotients::OrderOfEltInQuotients(OrderOfElt& sup)
  : 
    currentClass(2),
    arcer( *this ),
    isStarted (false),
    theWord(sup.getWord()),
    theGroup(sup.getWord().getParent()),
    upperBound( 10 )
{
  adminStart();
}

void OrderOfEltInQuotients::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if (currentClass > upperBound ){
      adminTerminate();
      return;
    }   
    if ( ! isStarted && theGroup.gic.haveNilpotentQuotInited( currentClass ) ) {
      arcer.setArguments( theGroup.gic.getNilpotentQuotInited( currentClass ),
			  theWord.getWord());
      isStarted = true;
    }
    
    if ( isStarted ) 
      if (arcer.takeControl()){
	if (arcer.getOrder() == 0){
	  theWord.wic.putHaveOrder(0);
	  adminTerminate();
	  return;
	}
	currentClass++;
	isStarted = false;
    }
    usedOneARC();
  }
}

// -------------------------- OrderOfEltARCer ------------------------------ //


OrderOfEltARCer::OrderOfEltARCer( ComputationManager& boss,
				  const class SMFPGroup& group, 
				  const Word& word ) 
  : ARCer( boss ), 
    theGroup( group ),
    theWord( word ),
    current( word ),
    retValue( -1 ),
    power( 1 )
{ } 


void OrderOfEltARCer::runComputation( )
{
  while( true ) {

    SetOfWordsChecker theChecker(current, theGroup);
    Trichotomy answer = dontknow;

    if( (answer = theChecker.isTrivial()) != no ) {
      if( answer == yes )
	retValue = power;
      return;
    }

    ++power;
    current = (current * theWord).freelyReduce();
  }
}


void OrderOfEltARCer::writeResults( ostream& out ) 
{
  out < retValue < power;
}


void OrderOfEltARCer::readResults( istream& in )
{
  in > retValue > power;
}


// --------------------------- OrderOfElt --------------------------------- //


OrderOfElt::OrderOfElt( SMWord& word)
  : Supervisor( true /*! word.getParent().gic.haveFastWordProblem()*/ ),
    theWord( word ),
    theArcer( *this, word.getParent(), word.getWord() ),
    explanation(""),
    triedAbelianization( false ),
    normalClosure( *this, word.getParent().gcm().normalClosure ),
    abelianInvariants( *this, word.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, word.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, word.getParent().gcm().agSupervisor ),
    nilpotentQuotients(*this, word.getParent().gcm().nilpotentQuotients),
    orderOfEltInQuotients(*this)
{
  if (theWord.wic.haveOrder())
    resultIsFast();
  if ( !displayInFE() ) adminStart();
}


int OrderOfElt::getOrder( )
{
  if ( ! triedAbelianization && 
      theWord.getParent().gic.haveCyclicDecomposition( ) ) {
    
    triedAbelianization = true;
    if( theWord.getParent().gic.getCyclicDecomposition()
       .orderOfElt( theWord.getWord() ) == 0 ) {
      explanation = "this follows from an inspection of the abelianization of the group";
      return 0;
    }
  }
  
  if( theArcer.takeControl() ){
    return theArcer.getRetValue();
  }
  
  return -1;
}


void OrderOfElt::takeControl( )
{
  Integer theAnswer;
  
  if ( ! displayInFE() || freeARCs() > 0 ){
    if (theWord.wic.haveOrder())
      theAnswer = theWord.wic.getOrder();
    else
      theAnswer = getOrder();
    
    if ( theAnswer  != -1 ) {
      LogMessage msg( *this, theWord );
      msg << Name( theWord ) << " has ";
      if ( theAnswer == 0 ) 
	msg << "infinite order";
      else
	msg << "order " << theAnswer;
      
      if( explanation.length() > 0 )
	msg << " : " << explanation;
      msg << ".";
      msg.send();
      
      if (!fastResult())
	theWord.wic.putHaveOrder(theAnswer);
      adminTerminate();
    }
    else
      usedOneARC();
  }
}


void OrderOfElt::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is element of finite order problem"),
		  Text("Is") + Name( theWord ) + "of finite order in"
		  + Name( Parent( theWord ) ) + "?",
		  helpID("OrderOfElt", theWord.getParent()),
		  Text("Order") + Name(theWord)
		  );
  

  pv.startItemGroup();
  
  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	  abelianInvariants.arcSlotID(),
	  15
	  );

    
  pv.startItemGroup
    ( CheckinType( Parent( theWord ) ) != SMFPGroup::ABELIAN &&
      CheckinType( Parent( theWord ) ) != SMFPGroup::NILPOTENT
      );

  int currentClass = 2;
  while (currentClass <= 10 && 
	 theWord.getParent().gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;
  
  if (!(currentClass > 10) )
    pv.add(Text("Compute the nilpotent quotients for") + 
	   Name( theWord.getParent() ),
	   nilpotentQuotients.arcSlotID(),
	   100
	   );
  pv.add(Text("Compute the problem, using nilpotent quotients for")
	 + Name( theWord.getParent() ),
	 orderOfEltInQuotients.arcSlotID(),
	 100
	 );  

  pv.add( Text("Enumerate normal closure of relators of")
	  + Name( Parent( theWord ) ),
	  normalClosure.arcSlotID(), 50);
  
  pv.add( Text("Seek a rewriting system for") + Name( Parent( theWord ) ),
	  kbSupervisor.arcSlotID(),
	 50
	  );
    
  pv.add( Text("Seek a ShortLex automatic structure for")
	  + Name( Parent( theWord ) ),
	  agSupervisor.arcSlotID(),
	  50
	  );
  
  pv.done();
}
#line 1  "src/IsEltCentral.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class IsEltCentral
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "IsEltCentral.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "viewStructure.h"


//--------------------------- IsEltCentral ----------------------------------//


IsEltCentral::IsEltCentral(const SMWord& word)
  : Supervisor( ! word.getParent().gic.haveFastWordProblem() ),
    theWord( word ),
    theChecker( word.getParent() ),
    normalClosure( *this, word.getParent().gcm().normalClosure ),
    abelianInvariants( *this, word.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, word.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, word.getParent().gcm().agSupervisor ),
    computeBasis( *this, word.getParent().gcm().computeBasis ),
    nilpotentQuotients( *this, word.getParent().gcm().nilpotentQuotients ),
    nilpotentWPInQuotients(*this),
    nilpotentWP( *this ),
    genetic( *this )
{
  SetOf<Word> comms;
  int numOfGens = word.getParent().getFPGroup().numberOfGenerators();
  Word w = word.getWord();
  
  for( int i = 0; i < numOfGens; ++i ) {
    Word gen = Word(Generator(i+1));
    comms |= (w.inverse() * gen.inverse() * w * gen).freelyReduce();
  }

  theChecker.replaceTheSet(comms);
  nilpotentWP->initialize(comms,&theWord.getParent());
  nilpotentWPInQuotients->initialize(comms,&theWord.getParent());
  genetic->init(theWord.getParent().getFPGroup(), comms, 
		GeneticWPCM::COMMUTATORS);

  if ( !displayInFE() ) adminStart();
}


void IsEltCentral::takeControl( )
{
  Trichotomy theAnswer = dontknow;
  Chars explanation;
  if ( ! displayInFE() || freeARCs() > 0 )
    if (nilpotentWP->state() == ComputationManager::TERMINATED){
      explanation = ": this follows from an inspection of a basis." ;
      theAnswer = nilpotentWP->isTrivial();
    }
  if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
    if (nilpotentWPInQuotients->isTrivial()==no){
      int retClass;
      theAnswer = nilpotentWPInQuotients->isTrivial(retClass);
      ostrstream msgTmp;
      msgTmp << ": because it's not central in lower central quotient of class " 
	     << retClass << ends;
      explanation = msgTmp.str();
    }
  if (theAnswer != dontknow){
      LogMessage msg( *this, theWord );
      msg << Name( theWord ) << " is ";
      if ( theAnswer == no ) msg << "not ";
      msg << "central in " << Name( Parent( theWord ) ) << explanation;
      msg.send();
      adminTerminate();     
      return;    
  }
  if ( ( theAnswer = theChecker.isTrivial() ) != dontknow ) {
    
    LogMessage msg( *this, theWord );
    msg << Name( theWord ) << " is ";
    if ( theAnswer == no ) msg << "not ";
    msg << "central in " << Name( Parent( theWord ) );
    
    Chars explanation = theChecker.getExplanation();
    if( explanation.length() > 0 )
      msg << " : " << explanation;
    
    msg << ".";
    msg.send();

    if( theChecker.haveDetails() ) {
      LogMessage msgLink( *this, theWord );
      msgLink << Link( Chars("Click here to see details of centrality problem "
			     "for ")
		       + Text(Name(theWord)) + 
		       Text("."),
		       "WordProblemDetails", 
		       theChecker.getDecompositionFileName()
		       );
      msgLink.send();
    }
    
    
    adminTerminate();
  } 
  else if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
    
    Chars explanation = "according to the genetic algorithm";
    LogMessage msg( *this, theWord );
    msg << Name( theWord ) << " is central";
    msg << " : " << explanation << ".";
    msg.send();
    adminTerminate();
  }
 
  else if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
    
    Chars explanation = "according to the genetic algorithm";
    LogMessage msg( *this, theWord );
    msg << Name( theWord ) << " is central";
    msg << " : " << explanation << ".";
    msg.send();
    adminTerminate();
  }
  else
    usedOneARC();
}


void IsEltCentral::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is element central problem"),
		  Text("Is") + Name( theWord ) + "central in"
		  + Name( Parent( theWord ) ) + "?",
		  helpID("IsEltCentral",theWord.getParent()), 
		  Text(Name(theWord)) + "central ?"
		  );
  

  pv.startItemGroup();
  
  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	  abelianInvariants.arcSlotID(),
	  15
	  );

    
  pv.startItemGroup
    ( CheckinType( Parent( theWord ) ) != SMFPGroup::ABELIAN &&
      CheckinType( Parent( theWord ) ) != SMFPGroup::NILPOTENT &&
      CheckinType( Parent( theWord ) ) != SMFPGroup::FREE_NILPOTENT
      );

  pv.add( Text("Enumerate normal closure of relators of")
	  + Name( Parent( theWord ) ),
	  normalClosure.arcSlotID(), 50);
  
  pv.add( Text("Seek a rewriting system for") + Name( Parent( theWord ) ),
	  kbSupervisor.arcSlotID(),
	 50
	  );
    
  pv.add( Text("Seek a ShortLex automatic structure for")
	  + Name( Parent( theWord ) ),
	  agSupervisor.arcSlotID(),
	  50
	  );

  pv.add( Text("Run genetic algorithm"), genetic.arcSlotID(), 50 );

  pv.startItemGroup( CheckinType( Parent( theWord ) ) == SMFPGroup::NILPOTENT ||
		     CheckinType( Parent( theWord ) ) == SMFPGroup::FREE_NILPOTENT );

  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add(Text("Compute a basis of ") + Name( Parent( theWord ) ),
	   computeBasis.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using relation subgroup for")
	 + Name( Parent( theWord ) ),
	 nilpotentWP.arcSlotID(),
	 50
	 );
  
  pv.startItemGroup( CheckinType( Parent( theWord ) ) == SMFPGroup::NILPOTENT ||
		     CheckinType( Parent( theWord ) ) == SMFPGroup::FREE_NILPOTENT ||
		     CheckinType( Parent( theWord ) ) == SMFPGroup::FP
		     );
  int upperBound, currentClass = 2;
  if (theWord.getParent().gic.isNilpotent()==yes ||
      theWord.getParent().gic.isFreeNilpotent()==yes)
    upperBound = theWord.getParent().gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theWord.getParent().gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;

  if (!(currentClass > upperBound) )
    pv.add(Text("Compute lower central quotients for") + Name( Parent( theWord ) ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using lower central quotients of")
	 + Name( Parent( theWord ) ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );
 
  pv.done();
}
#line 1  "src/AreEltsEqual.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class AreEltsEqual
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "AreEltsEqual.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "viewStructure.h"


//--------------------------- AreEltsEqual ----------------------------------//


AreEltsEqual::AreEltsEqual(const SMWord& w1, const SMWord& w2)
  : Supervisor( ! w1.getParent().gic.haveFastWordProblem() ),
    word1( w1 ),
    word2( w2 ),
    theChecker( w1.getParent() ),
    normalClosure( *this, w1.getParent().gcm().normalClosure ),
    abelianInvariants( *this, w1.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, w1.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, w1.getParent().gcm().agSupervisor ),
    computeBasis( *this, w1.getParent().gcm().computeBasis ),
    nilpotentQuotients( *this, w1.getParent().gcm().nilpotentQuotients ),
    nilpotentWPInQuotients(*this),
    nilpotentWP( *this ),
    genetic( *this )
{
  Word checkWord( (w1.getWord() * w2.getWord().inverse()).freelyReduce());
  theChecker.replaceTheSet( checkWord );
  nilpotentWP->initialize(checkWord,&word1.getParent());
  nilpotentWPInQuotients->initialize(checkWord,&word1.getParent());
  genetic->init(word1.getParent().getFPGroup(), checkWord, GeneticWPCM::WORD);

  if ( !displayInFE() ) adminStart();
}


void AreEltsEqual::takeControl( )
{
  Trichotomy theAnswer = dontknow;
  Chars explanation;
  if ( ! displayInFE() || freeARCs() > 0 )
    if (nilpotentWP->state() == ComputationManager::TERMINATED){
      explanation = Chars(": this follows from an inspection of a basis of ") +
	Text(Name(word1.getParent()));
      theAnswer = nilpotentWP->isTrivial();
    }
    if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
      if (nilpotentWPInQuotients->isTrivial()==no){
	int retClass;
	theAnswer = nilpotentWPInQuotients->isTrivial(retClass);
	ostrstream msgTmp;
	msgTmp << ": their images are not equal in the lower central quotient of "
	       << Name(word1.getParent()) << "of class " 
	       << retClass << ends;
	explanation = msgTmp.str();
      }
    if (theAnswer != dontknow){
	LogMessage msg( *this, word1, word2 );
	msg << Name( word1 ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "equal to " << Name( word2 ) << explanation;
	msg.send();
	adminTerminate();     
	return;      
    }
    if ( ( theAnswer = theChecker.isTrivial() ) != dontknow ) {
      
      LogMessage msg( *this, word1, word2 );
      msg << "The elements "<< Name( word1 ) << " and " << Name( word2 ) << " are ";
      if ( theAnswer == no ) msg << "not ";
      msg << "equal";

      Chars explanation = theChecker.getExplanation();
      if( explanation.length() > 0 )
	msg << " : " << explanation;

      msg << ".";
      msg.send();
      adminTerminate();
    }
    else if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
      
      LogMessage msg( *this, word1, word2 );
      msg << "The elements "<< Name( word1 ) << " and " << Name( word2 ) 
	  << " are equal";
      Chars explanation = "according to the genetic algorithm";
      msg << " : " << explanation << ".";
      msg.send();
      adminTerminate();
    }
    else
      usedOneARC();
}


void AreEltsEqual::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Elements equality problem"),
		  Text("Is") + Name( word1 ) + "equal to"
		  + Name( word2 ) + "?",
		  helpID("AreEltsEqual", word1.getParent()),
		  Text(Name(word1)) + "=" + Name(word2) + "?"
		  );
  

  pv.startItemGroup();
  
  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.add( Text("Compute abelian invariants of") + Name( Parent( word1 ) ),
	  abelianInvariants.arcSlotID(),
	  15
	  );

    
  pv.startItemGroup
    ( CheckinType( Parent( word1 ) ) != SMFPGroup::ABELIAN &&
       CheckinType( Parent( word1 ) ) != SMFPGroup::NILPOTENT &&
      CheckinType( Parent( word1 ) ) != SMFPGroup::FREE_NILPOTENT
      );

  pv.add( Text("Enumerate normal closure of relators of")
	  + Name( Parent( word1 ) ),
	  normalClosure.arcSlotID(),
	  50);
  
  pv.add( Text("Seek a rewriting system for") + Name( Parent( word1 ) ),
	  kbSupervisor.arcSlotID(),
	  50
	  );
    
  pv.add( Text("Seek a ShortLex automatic structure for")
	  + Name( Parent( word1 ) ),
	  agSupervisor.arcSlotID(),
	  50
	  );

  pv.add(Text("Run genetic algorithm"), genetic.arcSlotID(), 50);

  pv.startItemGroup
    ( CheckinType( Parent( word1 ) ) == SMFPGroup::FP ||
      CheckinType( Parent( word1 ) ) == SMFPGroup::NILPOTENT ||
      CheckinType( Parent( word1 ) ) == SMFPGroup::FREE_NILPOTENT
      );
  
  int upperBound, currentClass = 2;
  if (word1.getParent().gic.isNilpotent()==yes ||
      word1.getParent().gic.isFreeNilpotent()==yes)
    upperBound = word1.getParent().gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 word1.getParent().gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;

  if (!(currentClass > upperBound) )
    pv.add(Text("Compute the lower central quotients for") + Name( Parent( word1 ) ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using lower central quotients of")
	 + Name( Parent( word1 ) ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );
  
  pv.startItemGroup( CheckinType( Parent( word1 ) ) == SMFPGroup::NILPOTENT ||
		     CheckinType( Parent( word1 ) ) == SMFPGroup::FREE_NILPOTENT
		     );
  
  if (!word1.getParent().gic.haveNilpotentGroupInited())
    pv.add(Text("Compute a basis of ") + Name( Parent( word1 ) ),
	   computeBasis.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using relation subgroup of ")
	 + Name( Parent( word1 ) ),
	 nilpotentWP.arcSlotID(),
	 50
	 );
  pv.done();
}
#line 1  "src/SubgroupProblems.C"
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes IsSGAbelian, IsSGCentral, IsSGNilpotent,
//           IsSGTrivial.
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "SubgroupProblems.h"
#include "SMSubgroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "Subgroup.h"
#include "conversions.h"
#include "PresentationsOfSubgroup.h"

//--------------------------- IsSGAbelian -----------------------------------//

IsSGAbelian::IsSGAbelian( SMSubgroup& S)
  : Supervisor( ! S.getParent().gic.haveFastWordProblem() ),
    theSubgroup( S ),
    theChecker( S.getParent() ),
    normalClosure( *this, S.getParent().gcm().normalClosure ),
    abelianInvariants( *this, S.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, S.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, S.getParent().gcm().agSupervisor ),
    nilpotentQuotients(*this, S.getParent().gcm().nilpotentQuotients),
    nilpotentWPInQuotients(*this),
    computeBasis(*this, S.getParent().gcm().computeBasis),
    nilpotentWP(*this),
    genetic(*this)
{
  if (theSubgroup.sic.isAbelian()!=dontknow)
    resultIsFast();
  else {
    SetOf<Word> comms;
    VectorOf<Word> gens = S.getSubgroup().generators();
    int gensLen = gens.length();
    
    for( int i = 0; i < gensLen-1; ++i ) 
      for( int j = i+1; j < gensLen; ++j ) {
	Word w1 = gens[i];
	Word w2 = gens[j];
	comms |= (w1.inverse() * w2.inverse() * w1 * w2).freelyReduce();
      }
    nilpotentWPInQuotients->initialize(comms,&(theSubgroup.getParent()));
    nilpotentWP->initialize(comms,&(theSubgroup.getParent()));
    genetic->init(theSubgroup.getParent().getFPGroup(), comms, 
		  GeneticWPCM::COMMUTATORS);
    theChecker.replaceTheSet(comms);
  }
  if ( !displayInFE() ) adminStart();
}


void IsSGAbelian::takeControl( )
{
  Chars explanation;
  if (!fastResult())
  if ( ! displayInFE() || freeARCs() > 0 ){
    if (nilpotentWP->state() == ComputationManager::TERMINATED){
      explanation = Chars(":this follows from an inspection of a basis of ")+
	Text(Name(theSubgroup.getParent())) ;
      theSubgroup.sic.putIsAbelian(nilpotentWP->isTrivial());
    }
    if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
      if (nilpotentWPInQuotients->isTrivial()==no){
	int retClass;
	theSubgroup.sic.putIsAbelian
	  (nilpotentWPInQuotients->isTrivial(retClass));
	ostrstream msgTmp;
	  msgTmp << ":the image of "<<Name(theSubgroup)<<" in the lower central quotient of "
		 << Name(theSubgroup.getParent()) << " of class "<< retClass
		 << " is not abelian"<<ends;
	explanation = msgTmp.str();
      }    

    Trichotomy theAnswer(dontknow);
    if ( ( theAnswer = theChecker.isTrivial() ) != dontknow ) {
      explanation = theChecker.getExplanation();
      theSubgroup.sic.putIsAbelian(theAnswer);
    }

    if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
      explanation = "according to the genetic algorithm";
      theSubgroup.sic.putIsAbelian(theAnswer);
    }
    
    usedOneARC();
  }
  if (theSubgroup.sic.isAbelian()!=dontknow){
    LogMessage msg( *this, theSubgroup );
    msg << Name( theSubgroup ) << " is ";
    if ( theSubgroup.sic.isAbelian() == no ) msg << "not ";
    msg << "abelian ";
    if (explanation.length() >0)
      msg << explanation;
    msg << ".";
    msg.send();
    adminTerminate();     
    return;
  }
}


void IsSGAbelian::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is subgroup abelian problem"),
		  Text("Is the subgroup ") + Name( theSubgroup ) + " of "
		  + Name( Parent( theSubgroup ) ) + " abelian?",
		  helpID("IsSGAbelian", theSubgroup.getParent()),
		  Text(Name(theSubgroup)) + "abelian ?"
		  );
  

  pv.startItemGroup();
  
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup ) ),
	 abelianInvariants.arcSlotID(),
	 15
	 );

    
  pv.startItemGroup
    ( CheckinType( Parent( theSubgroup ) ) != SMFPGroup::ABELIAN &&
      CheckinType( Parent( theSubgroup ) ) != SMFPGroup::NILPOTENT &&
      CheckinType( Parent( theSubgroup ) ) != SMFPGroup::FREE_NILPOTENT
      );

  pv.add( Text("Enumerate normal closure of relators of")
	  + Name( Parent( theSubgroup ) ),
	  normalClosure.arcSlotID(), 50);
  
  pv.add(Text("Seek a rewriting system for") + Name( Parent( theSubgroup ) ),
	 kbSupervisor.arcSlotID(),
	 50
	 );
    
  pv.add(Text("Seek a ShortLex automatic structure for")
	 + Name( Parent( theSubgroup ) ),
	 agSupervisor.arcSlotID(),
	 50
	 );
  
  pv.add( Text("Run genetic algorithm"), genetic.arcSlotID(), 50 );

  pv.startItemGroup( CheckinType(Parent( theSubgroup )) == SMFPGroup::FP ||
		     CheckinType(Parent( theSubgroup )) == SMFPGroup::NILPOTENT ||
		     CheckinType(Parent( theSubgroup )) == SMFPGroup::FREE_NILPOTENT 
		     );
  
  int upperBound, currentClass = 2;
  if (theSubgroup.getParent().gic.isNilpotent()==yes ||
      theSubgroup.getParent().gic.isFreeNilpotent()==yes)
    upperBound = theSubgroup.getParent().gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theSubgroup.getParent().gic.haveNilpotentQuotInited(currentClass ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute the lower central quotients for") + 
	   Name( theSubgroup.getParent() ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theSubgroup.getParent() ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );  

  pv.startItemGroup( CheckinType(Parent(theSubgroup)) == SMFPGroup::NILPOTENT ||
		     CheckinType(Parent(theSubgroup)) == SMFPGroup::FREE_NILPOTENT);
  
  if (!theSubgroup.getParent().gic.
      haveNilpotentGroupInited())
    pv.add(Text("Compute a basis of ") + 
	   Name(theSubgroup.getParent()),
	   computeBasis.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using basis of ")
	 + Name( theSubgroup.getParent() ),
	 nilpotentWP.arcSlotID(),
	 50
	 );
  pv.done();
}


// -------------------------- IsSGCentral ---------------------------------- //


IsSGCentral::IsSGCentral( SMSubgroup& S)
  : Supervisor( ! S.getParent().gic.haveFastWordProblem() ),
    theSubgroup( S ),
    theChecker( S.getParent() ),
    normalClosure( *this, S.getParent().gcm().normalClosure ),
    abelianInvariants( *this, S.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, S.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, S.getParent().gcm().agSupervisor ),
    nilpotentQuotients(*this, S.getParent().gcm().nilpotentQuotients),
    nilpotentWPInQuotients(*this),
    computeBasis(*this, S.getParent().gcm().computeBasis),
    nilpotentWP(*this),
    genetic(*this)
{
  if (theSubgroup.sic.isCentral() != dontknow)
    resultIsFast();
  else {
    SetOf<Word> comms;
    int numOfGens = S.getParent().getFPGroup().numberOfGenerators();
    VectorOf<Word> gens = S.getSubgroup().generators();
    int gensLen = gens.length();
    
    for( int i = 0; i < gensLen; ++i ) 
      for( int j = 0; j < numOfGens; ++j ) {
	Word w1 = gens[i];
	Word w2 = Word(Generator(j+1));
	comms |= (w1.inverse() * w2.inverse() * w1 * w2).freelyReduce();
      }
    nilpotentWPInQuotients->initialize(comms,&(theSubgroup.getParent()));
    nilpotentWP->initialize(comms,&(theSubgroup.getParent()));
    genetic->init(theSubgroup.getParent().getFPGroup(), comms, 
		  GeneticWPCM::COMMUTATORS);
    theChecker.replaceTheSet(comms);
  }
  if ( !displayInFE() ) adminStart();
}


void IsSGCentral::takeControl( )
{
  Chars explanation;
  if (!fastResult())
  if ( ! displayInFE() || freeARCs() > 0 ){
    if (nilpotentWP->state() == ComputationManager::TERMINATED){
      explanation = ":this follows from an inspection of a basis" ;
      theSubgroup.sic.putIsCentral(nilpotentWP->isTrivial());
    }
    if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
      if (nilpotentWPInQuotients->isTrivial()==no){
	int retClass;
	theSubgroup.sic.putIsCentral
	  (nilpotentWPInQuotients->isTrivial(retClass));
	ostrstream msgTmp;
	msgTmp << ":the image of "<<Name(theSubgroup)<<" in the lower central quotient of "
	       << Name(theSubgroup.getParent()) << " of class "<< retClass
	       << " is not central"<<ends;
	explanation = msgTmp.str();
      }    

    Trichotomy theAnswer(dontknow);
    if ( (  theAnswer = theChecker.isTrivial() ) != dontknow ) {
      theSubgroup.sic.putIsCentral(theAnswer);
      explanation = theChecker.getExplanation();
    }

    if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
      explanation = "according to the genetic algorithm";
      theSubgroup.sic.putIsCentral(theAnswer);
    }

    usedOneARC();
  }
  if (theSubgroup.sic.isCentral() != dontknow){
    LogMessage msg( *this, theSubgroup );
    msg << Name( theSubgroup ) << " is ";
    if (theSubgroup.sic.isCentral() == no ) msg << "not ";
    msg << "central in " <<  Name( theSubgroup.getParent() )<<" ";
    if (explanation.length() >0)
      msg << explanation;
    msg << ".";
    msg.send();
    adminTerminate();     
    return;     
  }
}

void IsSGCentral::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is subgroup central problem"),
		  Text("Is") + Name( theSubgroup ) + "central in"
		  + Name( Parent( theSubgroup ) ) + "?",
		  helpID("IsSGCentral",  theSubgroup.getParent()),
		  Text(Name(theSubgroup)) + "central ?"
		  );
  

  pv.startItemGroup();
  
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup ) ),
	 abelianInvariants.arcSlotID(),
	 15
	 );

    
  pv.startItemGroup
    ( CheckinType( Parent( theSubgroup ) ) != SMFPGroup::ABELIAN &&
      CheckinType( Parent( theSubgroup ) ) != SMFPGroup::NILPOTENT &&
      CheckinType( Parent( theSubgroup ) ) != SMFPGroup::FREE_NILPOTENT
      );

  pv.add( Text("Enumerate normal closure of relators of")
	  + Name( Parent( theSubgroup ) ),
	  normalClosure.arcSlotID(), 50);
  
  pv.add(Text("Seek a rewriting system for") + Name( Parent( theSubgroup ) ),
	 kbSupervisor.arcSlotID(),
	 50
	 );
    
  pv.add(Text("Seek a ShortLex automatic structure for")
	 + Name( Parent( theSubgroup ) ),
	 agSupervisor.arcSlotID(),
	 50
	 );

  pv.add( Text("Run genetic algorithm"), genetic.arcSlotID(), 50 );

  pv.startItemGroup( CheckinType(Parent( theSubgroup )) == SMFPGroup::FP ||
		     CheckinType(Parent( theSubgroup )) == SMFPGroup::NILPOTENT ||
		     CheckinType(Parent( theSubgroup )) == SMFPGroup::FREE_NILPOTENT
		     );
  
  int upperBound, currentClass = 2;
  if (theSubgroup.getParent().gic.isNilpotent()==yes ||
      theSubgroup.getParent().gic.isFreeNilpotent()==yes )
    upperBound = theSubgroup.getParent().gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theSubgroup.getParent().gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute the lower central quotients for") + 
	   Name( theSubgroup.getParent() ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theSubgroup.getParent() ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );  

  pv.startItemGroup( CheckinType(Parent(theSubgroup)) == SMFPGroup::NILPOTENT ||
		     CheckinType(Parent(theSubgroup)) == SMFPGroup::FREE_NILPOTENT);
  
  if (!theSubgroup.getParent().gic.
      haveNilpotentGroupInited())
    pv.add(Text("Compute a basis of ") + Name(theSubgroup.getParent()),
	   computeBasis.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using basis of")
	 + Name( theSubgroup.getParent() ),
	 nilpotentWP.arcSlotID(),
	 50
	 ); 
  pv.done();
}


// ---------------------------- IsSGNilpotent ------------------------------ //


IsSGNilpotent::IsSGNilpotent(const SMSubgroup& S, int nilpClass)
  : Supervisor( true/*! S.getParent().gic.haveFastWordProblem()*/ ),
    theSubgroup( S ),
    theClass(nilpClass),
    theChecker( S.getParent(), nilpClass+1, S.getSubgroup().generators() ),
    theArcer( *this ),
    normalClosure( *this, S.getParent().gcm().normalClosure ),
    abelianInvariants( *this, S.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, S.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, S.getParent().gcm().agSupervisor ),
    nilpotentQuotients(*this, S.getParent().gcm().nilpotentQuotients),
    nilpotentWPInQuotients(*this)
{
  nilpotentWPInQuotients->initialize(theClass+1,&(theSubgroup.getParent()),
				     theSubgroup.getSubgroup().generators(),
				     theClass+1);
  if ( ! displayInFE() ) adminStart();
  theArcer.setArguments(&theChecker);
}


void IsSGNilpotent::takeControl( )
{
  Trichotomy theAnswer = dontknow;
  Chars explanation;

  if ( ! displayInFE() || freeARCs() > 0 ) {
    if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
      if (nilpotentWPInQuotients->isTrivial()==no){
	int retClass;
	theAnswer = nilpotentWPInQuotients->isTrivial(retClass);
	ostrstream msgTmp;
	msgTmp << "because there are non trivial commutators in lower central quotient of class " 
	       << retClass << ends;
	explanation = msgTmp.str();
      }    
    if (theAnswer != dontknow){
 	LogMessage msg( *this, theSubgroup );
	msg << Name( theSubgroup ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "nilpotent of class " << theClass << explanation;
	msg.send();
	adminTerminate();     
	return;     
    }
    
    if( theArcer.takeControl() ) 

      if( (theAnswer = theArcer.getRetValue()) != dontknow ) {
	LogMessage msg( *this, theSubgroup );
	msg << Name( theSubgroup ) << " is ";
	if ( theAnswer == no ) msg << "not ";
	msg << "nilpotent of class " << theClass;
	
	explanation = theChecker.getExplanation();
	if( explanation.length() > 0 )
	  msg << " : " << explanation;
	
	msg << ".";
	msg.send();
	adminTerminate();
	return;
      }

    usedOneARC();
  }
}


void IsSGNilpotent::viewStructure(ostream& ostr) const
{
  char classStr[10];
  sprintf(classStr,"%u",theClass);

  ProblemView pv( ostr, oid(), Text("Is subgroup nilpotent problem"),
		  Text("Is") + Name( theSubgroup ) + "nilpotent of class"
		  + classStr + "?",
		  helpID("IsSGNilpotent", theSubgroup.getParent()),
		  Text(Name(theSubgroup)) + "nilpotent ?"
		  );
  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);
  
  pv.add(Text("Compute abelian invariants of") + Name(Parent( theSubgroup ))
	 + "abelianized",
	 abelianInvariants.arcSlotID(),
	 15
	 );  

  pv.startItemGroup
    ( CheckinType(Parent( theSubgroup )) == SMFPGroup::FP ||
      CheckinType(Parent( theSubgroup )) == SMFPGroup::SMALL_CANCELLATION ||
      CheckinType(Parent( theSubgroup )) == SMFPGroup::ONE_RELATOR
      );
  
  pv.add(Text("Enumerate normal closure of relators"),
	 normalClosure.arcSlotID(), 50);
  
  pv.add(Text("Seek a rewriting system for") + Name(Parent( theSubgroup )),
	 kbSupervisor.arcSlotID(),
	 50
	 );
    
  pv.add(Text("Seek a ShortLex automatic structure for")
	 + Name(Parent( theSubgroup )),
	 agSupervisor.arcSlotID(),
	 50
	 );

  pv.startItemGroup( CheckinType(Parent( theSubgroup )) == SMFPGroup::FP 
		     );
  
  int upperBound, currentClass = 2;
  if (theSubgroup.getParent().gic.isNilpotent()==yes ||
      theSubgroup.getParent().gic.isFreeNilpotent()==yes)
    upperBound = theSubgroup.getParent().gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theSubgroup.getParent().gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute the lower central quotients for") + 
	   Name( theSubgroup.getParent() ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theSubgroup.getParent() ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );  

  pv.done();
}


//--------------------------- IsSGTrivial -----------------------------------//


IsSGTrivial::IsSGTrivial( SMSubgroup& S)
  : Supervisor( ! S.getParent().gic.haveFastWordProblem() ),
    theSubgroup( S ),
    theChecker( S.getSubgroup().generators(), S.getParent() ),
    normalClosure( *this, S.getParent().gcm().normalClosure ),
    abelianInvariants( *this, S.getParent().gcm().abelianInvariants ),
    kbSupervisor( *this, S.getParent().gcm().kbSupervisor ),
    agSupervisor( *this, S.getParent().gcm().agSupervisor ),
    nilpotentQuotients(*this, S.getParent().gcm().nilpotentQuotients),
    nilpotentWPInQuotients(*this),
    computeBasis(*this, S.getParent().gcm().computeBasis),
    nilpotentWP(*this),
    genetic(*this)
{
  if (theSubgroup.sic.isTrivial() != dontknow)
    resultIsFast();
  else {
    nilpotentWPInQuotients->initialize(theSubgroup.getSubgroup().generators(),
				       &(theSubgroup.getParent()));
    nilpotentWP->initialize(theSubgroup.getSubgroup().generators(),
			    &(theSubgroup.getParent()));
    genetic->init( theSubgroup.getParent().getFPGroup(), 
		   makeSetOf(theSubgroup.getSubgroup().generators()),
		   GeneticWPCM::SET_OF_WORDS );
  }
  if ( ! displayInFE() ) adminStart();
}


void IsSGTrivial::takeControl( )
{
  Chars explanation;
  if (!fastResult())
    if ( ! displayInFE() || freeARCs() > 0 ){
      if (nilpotentWP->state() == ComputationManager::TERMINATED){
	explanation = Chars("all generators of ")+ Text(Name(theSubgroup)) +" are trivial";
	theSubgroup.sic.putIsTrivial(nilpotentWP->isTrivial());
      }
      if (nilpotentWPInQuotients->state() == ComputationManager::TERMINATED)
	if (nilpotentWPInQuotients->isTrivial()==no){
	  int retClass;
	  theSubgroup.sic.putIsTrivial
	    (nilpotentWPInQuotients->isTrivial(retClass));
	  ostrstream msgTmp;
	  msgTmp << "the image of "<<Name(theSubgroup)<<" in the lower central quotient of "
		 << Name(theSubgroup.getParent()) << " of class "<< retClass
		 << " is not trivial"<<ends;
     	  explanation = msgTmp.str();
	}    

      Trichotomy theAnswer(dontknow);
      if ( ( theAnswer = theChecker.isTrivial() ) != dontknow ) {
	theSubgroup.sic.putIsTrivial(theAnswer);
	explanation = theChecker.getExplanation();
      }
      
      if ( ( theAnswer = genetic->areTrivial() ) != dontknow ) {
	explanation = "according to the genetic algorithm";
	theSubgroup.sic.putIsTrivial(theAnswer);
      }

      usedOneARC();
    }
  if (theSubgroup.sic.isTrivial() != dontknow){
    LogMessage msg( *this, theSubgroup );
    msg << Name( theSubgroup ) << " is a ";
    if (theSubgroup.sic.isTrivial() == no ) msg << "non-";
    msg << "trivial subgroup of " <<  Name( Parent( theSubgroup ) );
    if (explanation.length() > 0)
      msg <<": " << explanation;
    msg << ".";
    msg.send();
    adminTerminate();     
    return;     
  }  
}


void IsSGTrivial::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Subgroup triviality problem"),
		  Text("Is the subgroup") + Name( theSubgroup ) + "of"
		  + Name( Parent( theSubgroup ) ) + "trivial ?",
		  helpID("IsSGTrivial",  theSubgroup.getParent()),
		  Text(Name(theSubgroup)) + "= 1 ?"
		  );
  

  pv.startItemGroup();
  
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);

  pv.add(Text("Compute abelian invariants of") + Name( Parent( theSubgroup ) ),
	 abelianInvariants.arcSlotID(),
	 15
	 );

/*
  pv.startItemGroup( CheckinType( Parent( theSubgroup ) ) ==
		     SMFPGroup::ONE_RELATOR);

  pv.add("Compute the Magnus breakdown", orwp.arcSlotID(), 50);
*/
    
  pv.startItemGroup
    ( CheckinType( Parent( theSubgroup ) ) != SMFPGroup::ABELIAN &&
      CheckinType( Parent( theSubgroup ) ) != SMFPGroup::NILPOTENT &&
      CheckinType( Parent( theSubgroup ) ) != SMFPGroup::FREE_NILPOTENT
      );

  pv.add(Text("Enumerate normal closure of relators"),
	 normalClosure.arcSlotID(), 50);
  
  pv.add(Text("Seek a rewriting system for") + Name( Parent( theSubgroup ) ),
	 kbSupervisor.arcSlotID(),
	 50
	 );
    
  pv.add(Text("Seek a ShortLex automatic structure for")
	 + Name( Parent( theSubgroup ) ),
	 agSupervisor.arcSlotID(),
	 50
	 );

  pv.add( Text("Run genetic algorithm"), genetic.arcSlotID(), 50 );
  
  pv.startItemGroup( CheckinType(Parent(theSubgroup)) == SMFPGroup::NILPOTENT ||
		     CheckinType(Parent(theSubgroup)) == SMFPGroup::FREE_NILPOTENT ||
		     CheckinType(Parent(theSubgroup)) == SMFPGroup::FP);
  
  int upperBound, currentClass = 2;
  if (theSubgroup.getParent().gic.isNilpotent()==yes ||
      theSubgroup.getParent().gic.isFreeNilpotent()==yes)
    upperBound = theSubgroup.getParent().gic.getNilpotentcyClass()-1;
  else
    upperBound = 10;
  
  while (currentClass <= upperBound && 
	 theSubgroup.getParent().gic.haveNilpotentQuotInited( currentClass ) )
    currentClass++;
  
  if (!(currentClass > upperBound) )
    pv.add(Text("Compute the lower central quotients for") + 
	   Name( theSubgroup.getParent() ),
	   nilpotentQuotients.arcSlotID(),
	   50
	   );
  pv.add(Text("Solve using lower central quotients of")
	 + Name( theSubgroup.getParent() ),
	 nilpotentWPInQuotients.arcSlotID(),
	 50
	 );  

  pv.startItemGroup( CheckinType(Parent(theSubgroup)) == SMFPGroup::NILPOTENT ||
		     CheckinType(Parent(theSubgroup)) == SMFPGroup::FREE_NILPOTENT);
  
  if (!theSubgroup.getParent().gic.
      haveNilpotentGroupInited())
    pv.add(Text("Compute a basis of ") + Name(theSubgroup.getParent()),
	   computeBasis.arcSlotID(),
	   50
	   );
  
  pv.add(Text("Solve using basis of")
	 + Name( theSubgroup.getParent() ),
	 nilpotentWP.arcSlotID(),
	 50
	 ); 
 
  pv.done();
}


// -------------------------- SGIndexProblem ------------------------------- //


SGIndexProblem::SGIndexProblem(SMSubgroup& subgroup)
  : Supervisor( ! subgroup.sic.haveIndex() ),
    theSubgroup( subgroup ),
    sgToddCoxeter(*this,subgroup.scm().sgToddCoxeter),
    sgGHToddCoxeter(*this,subgroup.scm().sgGHToddCoxeter)
{
  if ( theSubgroup.sic.haveIndex() ) resultIsFast();
}


void SGIndexProblem::takeControl( )
{

  if ( theSubgroup.sic.haveIndex() ) {
    int theIndex = theSubgroup.sic.index();

    LogMessage msg( *this, theSubgroup );
     msg << Name( theSubgroup ) << " has";
    
    if( theIndex == 0 )
      msg << " infinite index";
    else
      msg << " index " << theIndex;
    if( !fastResult() )
      msg << ": according to the Todd-Coxeter algorithm";
    msg << ".";
    msg.send();
    adminTerminate();
    return;
  }

}


void SGIndexProblem::viewStructure(ostream& ostr) const
{
 ProblemView pv( ostr, oid(), Text("Index of a subgroup problem"),
		  Text("Compute the index of") + Name( theSubgroup )
		  + Text("in") + Name( theSubgroup.getParent() ),
		  helpID("SGIndexProblem", theSubgroup.getParent()),
		  Text("Index") + Name(theSubgroup)
		  );
  


  pv.startItemGroup( CheckinType( theSubgroup.getParent() ) == SMFPGroup::FP
		     );
  pv.add(Text("Enumerate cosets"),
	 sgToddCoxeter.arcSlotID(),
	 100
	 );
 
  pv.add(Text("GH Todd-Coxeter"),
	 sgGHToddCoxeter.arcSlotID(),
	 100
	 );

  pv.done();
}


// ---------------------------SGPresentationProblem--------------------------//

// -------------------------------ApproxMethodARCer--------------------------//

void ApproxMethodARCer::setArguments( const FPGroup& g , 
				      const VectorOf<Word>& v )
{
  sg = Subgroup(g,v);
}

FPGroup ApproxMethodARCer::getRetValue( )
{
  return result;
}

Chars ApproxMethodARCer::tmpOutputFilename()
{
  return tmpOutput.getFileName();
}

void ApproxMethodARCer::runComputation( )
{
  PresentationsOfSubgroup PS( sg );
  result = PS.makePresentation(tmpOutput);
}

void ApproxMethodARCer::writeResults( ostream& out)
{
  out < result;
}

void ApproxMethodARCer::readResults( istream& in )
{
  in > result;
}


// -----------------------------ApproxMethod-------------------------------//

ApproxMethod::ApproxMethod(SGPresentationProblem& sgp)
  : ComputationManager( false ), 
    arcer(*this) , theSubgroup(sgp.getTestSubgroup()) , result() 
{
  arcer.setArguments(theSubgroup.getParent().getFPGroup(),
		     theSubgroup.getSubgroup().generators());
  adminStart();
}
    
FPGroup ApproxMethod::getResult()
{
  return result;
}

Chars ApproxMethod::tmpOutputFilename()
{
  return arcer.tmpOutputFilename();
}

void ApproxMethod::takeControl( )
{
  if ( freeARCs() > 0 )
    if (arcer.takeControl())
      {
	result = arcer.getRetValue();
	adminTerminate();
	return;
      }
    else
      usedOneARC();
}

void ApproxMethod::start( )
{
  
}

void ApproxMethod::terminate( )
{

}


// -----------------------------TCMethodARCer---------------------------//

void TCMethodARCer::setArguments( const FPGroup& g , 
				  const VectorOf<Word>& v,
				  const PermutationRepresentation& prep )
{
  sg = Subgroup(g,v);
  pr = prep;
}

FPGroup TCMethodARCer::getRetValue( )
{
  return result;
}

Chars TCMethodARCer::tmpOutputFilename()
{
  return tmpOutput.getFileName();
}

void TCMethodARCer::runComputation( )
{
  PresentationsOfSubgroup PS( sg );
  result = PS.makePresentationTC( pr, tmpOutput );
}

void TCMethodARCer::writeResults( ostream& out)
{
  out < result;
}

void TCMethodARCer::readResults( istream& in )
{
  in > result;
}


// -------------------------------TCMethod------------------------------//

TCMethod::TCMethod(SGPresentationProblem& sgp)
  : ComputationManager( false ), 
    arcer(*this) , theSubgroup(sgp.getTestSubgroup()) , result() , init(false)
{ 
  adminStart();
}
    
FPGroup TCMethod::getResult()
{
  return result;
}

Chars TCMethod::tmpOutputFilename()
{
  return arcer.tmpOutputFilename();
}

void TCMethod::takeControl( )
{
   if ( freeARCs() > 0 && theSubgroup.sic.haveSchreierTransvl() )
     {
       if( !init )
	 {
	   arcer.setArguments(theSubgroup.getParent().getFPGroup(),
			      theSubgroup.getSubgroup().generators(),
			      theSubgroup.sic.getSchreierTransvl());
	   init = true;
	 }
       
       if (arcer.takeControl())
	 {
	   result = arcer.getRetValue();
	   adminTerminate();
	   return;
	 }
       else
	 usedOneARC();
     }

}   
void TCMethod::start( )
{
  
}

void TCMethod::terminate( )
{

}


// ---------------------------SGPresentationProblem--------------------------//

SGPresentationProblem::SGPresentationProblem(SMSubgroup& s)
  : Supervisor( true ) ,
    theSubgroup( s ) ,
    done( false ) ,
    term( false ) ,
    am( *this ) ,
    tcm( *this ) ,
    sgToddCoxeter(*this,theSubgroup.scm().sgToddCoxeter)
{
  if( theSubgroup.sic.haveIndex() )
    { 
      int index = theSubgroup.sic.index();
      if ( !index ) resultIsFast();
    }   
  
  if ( ! displayInFE() ) adminStart();
}

SMSubgroup& SGPresentationProblem::getTestSubgroup()
{
  return theSubgroup;
}

void SGPresentationProblem::takeControl()
{
  if( theSubgroup.sic.haveIndex() && !term )
    { 
      int index = theSubgroup.sic.index();
      
      if ( !index )
	{
	  LogMessage msg3( *this, theSubgroup );
	  // @am explanation was changed
	  msg3 << Name( theSubgroup ) << " is of infinite index in " 
	       << Name( theSubgroup.getParent() ) << ". "
	       << "There is no finite RS-presentation for" 
	       <<  Name( theSubgroup ) <<".";
	  msg3.send();
	  
	  adminTerminate();
	  return;
	}
      
      term = true;
      
    }
  
  if( am->state() == ComputationManager::TERMINATED )
    {
      LogMessage msg1( *this, theSubgroup );
      
      msg1 << Name( theSubgroup ) << " has presentation according to approximation method:";
      msg1 << am->getResult(); 
      msg1.send();
      
      LogMessage msg2( *this, theSubgroup );
      msg2 << Link("Click here to see the steps of RS-process that uses approximation method" , "ApproximationsRS-process" , am->tmpOutputFilename() );
      msg2.send();
      
      adminTerminate();
      return;
    }
      
  if( tcm->state() == ComputationManager::TERMINATED )
    {
      LogMessage msg4( *this, theSubgroup );
      
      msg4 << Name( theSubgroup ) << " has presentation according to method that uses Todd-Coxeter procedure:";
      msg4 << tcm->getResult(); 
      msg4.send();
      
      LogMessage msg5( *this, theSubgroup );
      msg5 << Link("Click here to see the steps of RS-process that uses Todd-Coxeter procedure" , "ToddCoxeterRS-process" , tcm->tmpOutputFilename() );
      msg5.send();

      adminTerminate();
      return;
    }
}

void SGPresentationProblem::viewStructure( ostream& ostr ) const
{
  ProblemView pv( ostr, oid(), Text("Subgroup presentation problem"),
		  Text("Compute an RS-presentation for") + Name( theSubgroup )
		  + Text("in") + Name( theSubgroup.getParent() ),
		  helpID("SGPresentationProblem", theSubgroup.getParent()),
		  Text("Pres") + Name( theSubgroup )
		  );  
  
  pv.startItemGroup();
  
  pv.add(Text("Approximation method"), am.arcSlotID(), 45 );

  pv.add(Text("Compute Schreier transversal"), sgToddCoxeter.arcSlotID(), 45 );

  pv.add(Text("Method that uses Todd-Coxeter procedure"), tcm.arcSlotID(), 10 );
  
  pv.done();
  
}


void SGPresentationProblem::start( )
{
  
}

void SGPresentationProblem::terminate( )
{

}


//-----------------------------SGRewriteWordProblem-----------------------//

void RewriteWordARCer::setArguments(const PermutationRepresentation& pr1,const FreeGroup& K,const Word& w)
{
  theWord = w;
  F = K;
  pr =  new  PermutationRepresentation(pr1);
}

Word RewriteWordARCer::rewrite(const PermutationRepresentation& pr,const VectorOf<Word>& b,
			       const FreeGroup& F,const Word& w,bool flag)
{
  Word h;
  Word rw;
  FreeGroup K(b.length());
  
  if( flag )
    {
      tmpOutput << " "; 
      F.printWord(tmpOutput,w);
      tmpOutput << " = " << endl << " = ";
    }
  
  for( int i = 0 ; i < w.length() ; i++ )
    {
      Word h = pr.representativeOf(w.initialSegment(i));
      Word h1 = pr.representativeOf(w.initialSegment(i+1));
      Word s = h * w[i] * F.inverseOf(h1);
      s = s.freelyReduce();
      
      if( flag )
	{
      
	  if( rw.length() )
	    {
	      K.printWord(tmpOutput,rw);
	      tmpOutput << "  ";
	    }
	  
	  tmpOutput << " ( ";
	  F.printWord(tmpOutput,h);
	  tmpOutput << "   ";
	  F.printWord(tmpOutput,w[i]);
	  tmpOutput << "   ( ";
	  F.printWord(tmpOutput,h1);
	  tmpOutput << " )^-1 )   ";
      
	  if(h1.length())
	    {
	      F.printWord(tmpOutput,h1);
	      tmpOutput << "   ";
	    }
	  
	  if( i != w.length() - 1 )
	    F.printWord(tmpOutput,
			(w.initialSegment(i+1).inverse() * w).freelyReduce());
	  tmpOutput << " = " << endl << " = ";
	}
      
      int k = b.indexOf(s);
      
      Word g;
      
      if( k < 0 )
	{
	  s = F.inverseOf(s);
	  k = b.indexOf(s);
	  
	  if( k >= 0 )
	    g = Generator(-(k+1));
	  
	}
      else
	g = Generator(k+1);
      
      rw *= g;
    
      if( flag )
	{
	  K.printWord(tmpOutput,rw);
	  tmpOutput << "   ";
	  
	  if( h1.length() )
	    {
	      F.printWord(tmpOutput,h1);
	      tmpOutput << "   ";
	    }
	  
	  if( i != w.length() - 1 )
	    F.printWord(tmpOutput,
			(w.initialSegment(i+1).inverse() * w).freelyReduce());
	  if( i != w.length() - 1 )
	    tmpOutput << " = " << endl << " = ";
	}
    }
  
  if( flag )
    tmpOutput << endl;
  
  return rw;
}

Chars RewriteWordARCer::tmpOutputFilename()
{
  return tmpOutput.getFileName();
}

int RewriteWordARCer::getRank()
{
  return rank;
}

Word RewriteWordARCer::getRepresentative()
{
  return pr->representativeOf(theWord);
}



void RewriteWordARCer::runComputation( )
{
  VectorOf<Word> sch = pr->getRepresentatives();
  tmpOutput << "Schreier generators of the subgroup:" << endl;
  VectorOf<Word> basis;
  Word w;
  
  int count = 1;
  for( int i = 0 ; i < F.rank() ; i++ ) 
    {
      Word x = Generator(i+1);
      
      for( int r = 0 ; r < sch.length() ; r++ )
	{
	  Word u = sch[r];
	  Word tmp = pr->representativeOf(u * x);
	  w = u * x * F.inverseOf(tmp);
	  w = w.freelyReduce();
	  
	  if( w.length() ) 
	    {
	      basis.append(w);
	      tmpOutput << "x" << count << " = j(s" << r + 1 << ",";
	      F.printWord(tmpOutput,x);
	      tmpOutput << ") = ";
	      F.printWord(tmpOutput,w);
	      tmpOutput << endl;
	      count++;
	    }
	}
    }
  
  tmpOutput << endl << " The steps of the rewriting process:" << endl << endl;
  
  SGofFreeGroup H(F,basis);
  if(H.contains(theWord))
    {
      FreeGroup K(basis.length());
      
      tmpOutput << " ";
      F.printWord(tmpOutput,theWord);
      tmpOutput << "  is in the subgroup and can be expressed in terms of Schreier generators as follows: " << endl << " ";
      F.printWord(tmpOutput,theWord);
      tmpOutput << " = ";
      
      K.printWord(tmpOutput,rewrite(*pr,basis,F,theWord,0));
      tmpOutput << endl << endl;
      
      tmpOutput << " The process for expressing is as follows: " << endl;
      retValue = rewrite(*pr,basis,F,theWord,1);
    }  
  else
    retValue = Word();
  
  tmpOutput << end;
}

void RewriteWordARCer::writeResults( ostream& out ) 
{
  out < retValue;

}

void RewriteWordARCer::readResults( istream& in )
{
  in > retValue;

}


SGRewriteWordProblem::SGRewriteWordProblem(SMSubgroup& s,SMWord& w)
  : Supervisor( true ),
    theSubgroup( s ),
    theWord( w ),
    init( false ),
    arcer(*this),
    sgToddCoxeter(*this,theSubgroup.scm().sgToddCoxeter)
{
  if( theSubgroup.sic.haveIndex() )
    {
      int index = theSubgroup.sic.index();  
      if ( !index ) resultIsFast();
    }
  
  if( !theWord.getWord().length() )
    resultIsFast();
}


void SGRewriteWordProblem::takeControl( )
{
  if( !theWord.getWord().length() )
    {
      LogMessage msg1( *this, theSubgroup, theWord );
      msg1 << Name( theWord ) << " is trivial.";
      msg1.send();
      
      adminTerminate();
      return;
    }
  
  if ( theSubgroup.sic.haveIndex() ) 
    {
      int index = theSubgroup.sic.index();
      
      if( index == 0 )
	{
	  LogMessage msg2( *this, theSubgroup, theWord );
	  msg2 << Name( theSubgroup ) << " has infinite index , so we can't rewrite " << Name( theWord ) << ".";
	  msg2.send();
	  
	  adminTerminate();
	  return;
	}
      else
	{ 
	  if( !init )
	    {
	      arcer.setArguments(theSubgroup.sic.getSchreierTransvl(),
				 theSubgroup.getParent().getFreePreimage(),
				 theWord.getWord());
	      init = true;
	    }
	  
	  if(freeARCs() > 0)
	    if(arcer.takeControl())
	      {
		Word result = arcer.getRetValue();
		if( !arcer.getRepresentative().length() )
		  {
		    LogMessage msg3( *this, theSubgroup, theWord );
		    msg3 << Name( theWord );
		    msg3 << " in terms of Schreier generators: ";
		    
		    FreeGroup T(arcer.getRank());
		    T.printWord(msg3,result);
		    
		    msg3 << ".";
		    msg3.send();
		    
		    LogMessage msg4( *this, theSubgroup ,theWord );
		    msg4 << Link("Click here to see the Schreier generators" , "SchreierGens" , arcer.tmpOutputFilename() );
		    msg4.send();
     		  }
		else
		  {
		    LogMessage msg5( *this, theSubgroup, theWord );
		    msg5 << Name( theWord );
		    msg5 << " is not in ";
		    msg5 << Name( theSubgroup ) << "; its representative in the Schreier transversal is ";
		    
		    FreeGroup F(theSubgroup.getParent().getFreePreimage());
		    F.printWord(msg5,arcer.getRepresentative());
		    
		    msg5 << ".";

		    msg5.send();
		  }
		
		adminTerminate();
		return;
	      }
	    else
	      usedOneARC();
	}
    }
} 

void SGRewriteWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Rewriting of a word in terms of Schreier generators"),
		  Text("Rewrite a word in terms of Schreier generators ") + 
		  Name( theWord ) ,
		  helpID("SGRewriteWordProblem",theWord.getParent()),
		  Text("rewrite") + Name(theWord)
		  );
  pv.startItemGroup();
  
  if (!theSubgroup.sic.haveIndex())
    pv.add(Text("Enumerate cosets of")
	   + Name( theSubgroup ),
	   sgToddCoxeter.arcSlotID(),
	   100
	   );   
  
  pv.add(Text("Rewrite ")+Name( theWord ),
  	 ThisARCSlotID(),
  	 10);
  
  pv.done();
}
#line 1  "src/AbelianProblems.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

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
#include "GCD.h"
#include "SmithNormalForm.h"

// ----------------------- AbelianWordProblem ------------------------------ //

AbelianWordProblem::AbelianWordProblem(SMWord& w)
: Supervisor( true ),
  theWord( w ),
  abelianInvariants( *this, w.getParent().gcm().abelianInvariants )
{
  if ( w.getParent().gic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianWordProblem::takeControl( )
{
  if ( theWord.getParent().gic.haveCyclicDecomposition() ) {
    const AbelianGroup& A = theWord.getParent().gic.getCyclicDecomposition();
    LogMessage msg( *this, theWord );
    msg << "The canonical decomposition of " << Name( theWord ) << ": ";
    A.printWordInNewGens( msg, A.oldToNewGens( A.oldInAbelianForm( theWord.getWord() ) ) );
    msg << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the canonical decomposition of a word"),
		  Text("Compute the canonical decomposition of ")
		  + Name( theWord ),
		  helpID("AbelianWordProblem", theWord.getParent()),
		  "Can dec" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// ----------------------- AbelianSGWordProblem ---------------------------- //

AbelianSGWordProblem::AbelianSGWordProblem(SMSubgroup& S, SMWord& w)
  : Supervisor( true ),
    theSubgroup( S ),
    theWord( w ),
    abelianSGInvariants( *this, theSubgroup.scm().abelianSGInvariants )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) resultIsFast();
}

void AbelianSGWordProblem::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) {
    const AbelianSGPresentation& A = theSubgroup.sic.getSGPresentation();
    LogMessage msg( *this, theWord, theSubgroup );
    Word newWord;
    if(A.fromSGGensToSGPGens(theWord.getWord(), newWord)) {
      msg << Name(theWord) << " = ";
      A.printWordInNewGens(msg, A.oldToNewGens(A.oldInAbelianForm(newWord)));
    }
    else msg << Name(theWord) << " is not in " << Name(theSubgroup);
    msg << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianSGWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Rewrite a word in terms of a "
		  "canonical decomposition of a subgroup",
		  Text("Rewrite") + Name( theWord ) 
		  + Text("in terms of a canonical decomposition of") 
		  + Name( theSubgroup ),
		  helpID("AbelianSGWordProblem", theWord.getParent()),
		  "SG Can dec" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theSubgroup ),
	  abelianSGInvariants.arcSlotID(), 50 );
  pv.done();
}

// -------------------- AbelianQuotientWordProblem ----------------------- //

AbelianQuotientWordProblem::AbelianQuotientWordProblem(SMSubgroup& S, SMWord& w)
  : Supervisor( true ),
    theSubgroup( S ),
    theWord( w ),
    abelianInvariants( *this, theSubgroup.scm().abelianInvariantsOfFactor )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) resultIsFast();
}

void AbelianQuotientWordProblem::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    LogMessage msg( *this, theWord, theSubgroup );
    msg << Name(theWord) << Name(theSubgroup) << " = ";
    A.printWordInNewGens(msg, A.oldToNewGens(A.oldInAbelianForm(theWord.getWord())));
    msg << Name(theSubgroup) << ".";
    msg.send();
    FPGroup G = theWord.getParent().getFPGroup();
    File file(A.getFileName());
    file << "The canonical decomposition of " << Name( Parent( theSubgroup ) )
	 << "/" << Name( theSubgroup ) << ":" << endl << endl
	 << A << "." << endl << endl;
    LogMessage msg1(*this,theWord, theSubgroup);
    msg1 << Link(Chars( "Click here to see the canonical decomposition of ") 
		 + Text(Name( Parent( theSubgroup ) )) + Chars("/")  
		 + Text(Name( theSubgroup )),
		 "CyclicDecomposition", file.getFileName() );
    msg1.send();   
    adminTerminate();
  }
}

void AbelianQuotientWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Compute the canonical decomposition of a word",
		  Text("Compute the canonical decomposition of") 
		  + Name( theWord ) + Text("in terms of a canonical basis for")
		  + Name( Parent( theWord )) + "/" + Name( theSubgroup ),
		  helpID("AbelianSGFactorWordProblem", theWord.getParent()),
		  "Quo Can dec" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) )
	  + "/" + Name( theSubgroup ), abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// --------------------------- IsWordNthPower ------------------------------ //

IsWordNthPower::IsWordNthPower(SMWord& w, int p)
  : Supervisor( true ),
    thePower( p ),
    theWord( w ),
    theGroup( w.getParent() ),
    abelianInvariants( *this, theGroup.gcm().abelianInvariants )
{
  if( theGroup.gic.haveCyclicDecomposition() ) resultIsFast();
}

void IsWordNthPower::takeControl( )
{
  if ( theGroup.gic.haveCyclicDecomposition() ) {
    const AbelianGroup& A = theGroup.gic.getCyclicDecomposition();
    const AbelianWord& w = A.oldInAbelianForm( theWord.getWord() );
    VectorOf<Integer> inv = A.invariants();
    VectorOf<Integer> pow = A.oldToNewGens(w).getPowers();
    LogMessage msg( *this, theWord );
    bool ans = true;
    int r = A.rankOfFreeAbelianFactor();
    for(int i=0; i<r; i++) {
      if (pow[i]%thePower!=0) { ans = false; break; }
      else pow[i] = pow[i] / thePower;
    }
    if(ans) {
      for(int i=0; i<inv.length(); i++) {
	Integer p, q, d;
	d = GCD(thePower, inv[i], p, q);
	if( gcd(pow[i+r],inv[i]) % d != 0 ) { ans = false; break; }
	else pow[i+r] = (pow[i+r]/d * p) % (inv[i]/d);
      }
      msg << Name(theWord) << " = ( ";
      A.getFPGroup().printWord(msg, A.newToOldGens(AbelianWord(pow)).getWord());
      msg << " )^" << thePower << ".";
    } 
    else msg << Name(theWord) << " is not a " << thePower << "-th power.";
    msg.send();
    adminTerminate();
  }
}

void IsWordNthPower::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Rewrite w1 as an n-th power",
		  Text("Rewrite") + Name( theWord ) + Text("as an n-th power"),
		  helpID("IsWordNthPower", theGroup), "Rewrite as pow" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ), 
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// ----------------------- AbelianIsomProblem ------------------------------ //

AbelianIsomProblem::AbelianIsomProblem(SMFPGroup& g1, SMFPGroup& g2)
  : Supervisor( true ), G1( g1 ), G2( g2 ),
    abelianInvariants1( *this, g1.gcm().abelianInvariants ),
    abelianInvariants2( *this, g2.gcm().abelianInvariants )
{
  if ( G1.gic.haveCyclicDecomposition() && 
       G2.gic.haveCyclicDecomposition() ) resultIsFast();
}

void AbelianIsomProblem::takeControl( )
{
  if ( G1.gic.haveCyclicDecomposition() && G2.gic.haveCyclicDecomposition() ) {
    const AbelianGroup& A1 = G1.gic.getCyclicDecomposition();
    const AbelianGroup& A2 = G2.gic.getCyclicDecomposition();
    LogMessage msg( *this, G1, G2 );
    msg << Name( G1 ) << " and " << Name( G2 ) << " are ";
    if ( A1.rankOfFreeAbelianFactor() != A2.rankOfFreeAbelianFactor() )
      msg << "not isomorphic: they have different free ranks.";
    else if ( A1.invariants() != A2.invariants() )
      msg << "not isomorphic because their torsion subgroups differ.";
    else
      msg << "isomorphic because they have the same invariants.";    
    msg.send();
    adminTerminate();
  }
}


void AbelianIsomProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Abelian isomorphism problem"),
		  Text("Are") + Name(G1) + "and" + Name(G2) + "isomorphic?",
		  helpID("AbelianIsomProblem", G1), "Isomorphic ?" );
  pv.startItemGroup();
  if ( ! G1.gic.haveCyclicDecomposition() )
    pv.add( Text("Compute abelian invariants of") + Name( G1 ),
	    abelianInvariants1.arcSlotID(), 50 );
  if ( ! G2.gic.haveCyclicDecomposition() )
    pv.add( Text("Compute abelian invariants of") + Name( G2 ),
	    abelianInvariants2.arcSlotID(), 50 );
  pv.done();
}

//------------------- AbelianTorsionFreeRankProblem -------------------------//

AbelianTorsionFreeRankProblem::AbelianTorsionFreeRankProblem(SMFPGroup& G)
  : Supervisor( true ),
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
    msg << "The torsion-free rank of " << Name( theGroup ) 
	<< " is " << A.rankOfFreeAbelianFactor( )
	<< ": this follows from the canonical decomposition of "
	<< Name(theGroup) << "."  ;
    msg.send();
    adminTerminate();
  }
}

void AbelianTorsionFreeRankProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the torsion-free rank"),
		  Text("Find the torsion-free rank of") + Name( theGroup ),
		  helpID("TorsionFreeRankProblem", theGroup),
		  "T-f rank" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

//-------------- AbelianOrderOfTheTorsionSubgroupProblem --------------------//

AbelianOrderOfTheTorsionSubgroupProblem::AbelianOrderOfTheTorsionSubgroupProblem(SMFPGroup& G)
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
    msg << "The order of the torsion subgroup of" << Name(theGroup) << "is";
    if( sign(theOrder) == 0 ) msg << "infinite";
    else msg << theOrder;
    msg << ": this follows from the canonical decomposition of " 
	<< Name( theGroup ) << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianOrderOfTheTorsionSubgroupProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the order of the torsion subgroup"),
		  Text("Find the order of the torsion subgroup of ")
		  + Name( theGroup ), 
		  helpID("AbelianOrderOfTheTorsionSubgroupProblem", theGroup),
		  "Ord tor subgp");
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(), 50 );
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
    Chars explanation = 
      Text("this follows from the canonical decomposition of")+Name(theGroup);
    theGroup.gic.putHaveOrder(A.order(), explanation);    
    if( !fastResult() )
      LogMessage(*this, theGroup.gic.getOrderMessage(), 2).send();
    adminTerminate();
  }
}

void AbelianOrderProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the order"),
		  Text("Find the order of ") + Name( theGroup ),
		  helpID("AbelianOrderProblem", theGroup),
		  Text("Order") + Name(theGroup) );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(), 50);
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
    if( theOrder == 0 )  msg << "infinite";
    else msg << theOrder;
    msg << ": this follows from the canonical decomposition.";
    msg.send();
    adminTerminate();
  }
}

void AbelianOrderOfEltProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the order"),
		  Text("Find the order of ") + Name( theWord ),
		  helpID("AbelianOrderOfEltProblem", theWord.getParent()),
		  Text("Order") + Name(theWord) );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// --------------------- AbelianEqualityProblem ---------------------------- //

AbelianEqualityProblem::AbelianEqualityProblem(SMWord& w1, SMWord& w2)
  : Supervisor( true ), word1( w1 ), word2( w2 ),
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
    if( !answer ) msg << "not ";
    msg << "equal to " << Name( word2 )
	<< ": this follows from the canonical decompositions.";
    msg.send();
    adminTerminate();
  }
}

void AbelianEqualityProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Equality problem"),
		  Text("Is ") + Name( word1 ) + Text(" = ")
		  + Name( word2 ) + Text(" ?"), 
		  helpID("AbelianEqualityProblem", word1.getParent()),
		  Text(Name(word1)) + "=" + Name(word2) + "?" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( Parent( word1 ) ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// ------------------------ WordInSGOfAbelian ------------------------------ //

WordInSGOfAbelian::WordInSGOfAbelian( SMSubgroup& S, SMWord& w )
  : Supervisor( true ), theSubgroup( S ), theWord( w ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() )  resultIsFast();
}

void WordInSGOfAbelian::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    bool answer = A.isTrivial( theWord.getWord() );
    LogMessage msg( *this, theSubgroup, theWord );
    msg << Name( theWord ) << " is ";
    if( !answer )  msg << "not ";
    msg << "in " << Name( theSubgroup ) 
	<< " : this follows from the canonical decomposition of "
	<< Name( theWord ) << " in the factor group "
	<< Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup ) << "."; 
    msg.send();
    FPGroup G = theWord.getParent().getFPGroup();
    File file(A.getFileName());
    file << endl << endl << "The canonical decomposition of "
	 << Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup ) 
	 << ":" << endl << endl << A << "." << endl << endl
	 << "The canonical decomposition of the image of " << Name( theWord ) 
	 << " in the new generators of the factor group "
	 << Name( Parent( theSubgroup ) ) << "/"  << Name( theSubgroup ) 
	 << ":" << endl << endl; 
    AbelianWord abW = A.oldInAbelianForm(theWord.getWord());
    A.printWordInNewGens(file,A.oldToNewGens(abW));
    file << "." << endl;
    LogMessage msg1(*this,theWord, theSubgroup);
    msg1 << Link(Chars( "Click here to see the canonical decomposition of ") 
		 + Text(Name( theWord )) + Chars(" in the factor group ") 
		 + Text(Name( Parent( theSubgroup ) )) + Chars("/")  
		 + Text(Name( theSubgroup )),
		 "CyclicDecomposition", file.getFileName() );
    msg1.send();   
    adminTerminate();
  }
}

void WordInSGOfAbelian::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Subgroup membership problem"),
		  Text("Is ") + Name( theWord ) + Text(" in ")
		  + Name( theSubgroup ) + Text(" ?"), 
		  helpID("WordInSGOfAbelian",theWord.getParent()),
		  Text(Name(theWord)) + "in" + Name(theSubgroup) + "?" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ") + Name( Parent( theWord ) )
	  + Text("/") + Name( theSubgroup ), 
	  abelianInvariants.arcSlotID(), 50);
  pv.done();
}

// ---------------------- WordPowerInSGOfAbelian --------------------------- //

WordPowerInSGOfAbelian::WordPowerInSGOfAbelian( SMSubgroup& S, SMWord& w )
  : Supervisor( true ), theSubgroup( S ), theWord( w ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) resultIsFast();
}

void WordPowerInSGOfAbelian::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    Integer theOrder = A.orderOfElt(theWord.getWord());
    File file(A.getFileName());
    LogMessage msg( *this, theSubgroup, theWord );
    if( theOrder==0 ) msg << Name( theWord ) << " is not";
    else if( theOrder==1 ) msg << Name( theWord ) << " is";
    else msg << Name( theWord ) << "^" << theOrder << " is";
    msg << " in " << Name( theSubgroup ) 
	<< ": this follows from the canonical decomposition of " 
	<< Name( theWord ) << " in the factor group "
	<< Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup ) << "."; 
    msg.send();
    FPGroup G = theWord.getParent().getFPGroup();
    file << endl << endl << "The canonical decomposition of "
	 << Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup ) 
	 <<  ":" << endl << endl << A << "." << endl << endl;
    file << "The canonical decomposition of the image of "
	 << Name( theWord ) << " in the new generators of the factor group "
	 << Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup )
	 << ":" << endl << endl; 
    AbelianWord abW = A.oldInAbelianForm(theWord.getWord());
    A.printWordInNewGens(file,A.oldToNewGens(abW));
    file << "." << end;
    LogMessage msg1(*this,theWord, theSubgroup);
    msg1 << Link(Chars( "Click here to see the canonical decomposition of ") 
		 + Text(Name( theWord )) + Chars(" in the factor group ") 
		 + Text(Name( Parent( theSubgroup ) )) + Chars("/")  
		 + Text(Name( theSubgroup )),
		 "CyclicDecomposition",	 file.getFileName() );
    msg1.send();
    adminTerminate();
  }
}

void WordPowerInSGOfAbelian::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Subgroup membership problem",
		  Text("Is a power of") + Name( theWord ) + Text("in")
		  + Name( theSubgroup ) + Text("?"), 
		  helpID("WordPowerInSGOfAbelian",theWord.getParent()),
		  Text(Name(theWord)) + "power in" + Name(theSubgroup) + "?");
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ") + Name(Parent(theWord))
	  + Text("/") + Name(theSubgroup), abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// ---------------------- WordInSGBasisOfAbelian --------------------------- //

WordInSGBasisOfAbelian::WordInSGBasisOfAbelian( SMSubgroup& S, SMWord& w )
  : Supervisor( true ), 
  theSubgroup( S ), 
  theWord( w ),
  abelianSGInvariants( *this, theSubgroup.scm().abelianSGInvariants )
{
  if( theSubgroup.sic.haveCyclicDecomposition( ) ) resultIsFast();
}

void WordInSGBasisOfAbelian::takeControl( )
{
  if( theSubgroup.sic.haveCyclicDecomposition( ) ) {
    LogMessage msg(*this,theWord,theSubgroup);
    AbelianSGPresentation A = theSubgroup.sic.getSGPresentation();
    if( !theSubgroup.sic.havePrimaryDecomposition( ) ) {
      A.findPrimaryBasis();
      theSubgroup.sic.putHavePrimaryDecomposition(A);
    }
    Word newWord;
    if(!A.fromSGGensToSGPGens(theWord.getWord(),newWord))
      msg << Name(theWord) << " is not in ";
    else {
      msg << Name(theWord) << " is";
      if(!theSubgroup.sic.getSGPresentation().isPureCyclSubgroup(newWord))
	msg << " not";
      msg << " part of a canonical basis for ";
    } 
    msg << Name(theSubgroup) << ".";
    msg.send( );
    adminTerminate();
  }
}

void WordInSGBasisOfAbelian::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Is w1 part of a canonical basis for H1",
		  Text("Is") + Name( theWord )
		  + Text("part of a canonical basis for")
		  + Name( theSubgroup ) + Text("?"), 
		  helpID("WordInSGBasisOfAbelian", theSubgroup.getParent()),
		  Text(Name(theWord)) + "in" + Name(theSubgroup) + "basis?");
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ") + Name(theSubgroup),
	    abelianSGInvariants.arcSlotID(), 50 );
  pv.done();
}

// ------------------- AbelianIsSGEqualToTheGroup -------------------------- //

AbelianIsSGEqualToTheGroup::AbelianIsSGEqualToTheGroup( SMSubgroup& S )
  : Supervisor( true ),
    theSubgroup( S ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() )  resultIsFast();
}

void AbelianIsSGEqualToTheGroup::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    bool answer = A.isTrivial();
    LogMessage msg( *this, theSubgroup );
    msg << Name( theSubgroup ) << " is ";
    if( !answer ) msg << "not ";
    msg << "equal to " << Name( theSubgroup.getParent() )
	<< ": this follows by computing the abelian invariants of "
	<< Name( Parent( theSubgroup ) ) << "/" << Name( theSubgroup ) << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianIsSGEqualToTheGroup::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is a subgroup equal to a group ?"),
		  Text("Is ") + Name( theSubgroup ) + Text(" = ")
		  + Name( theSubgroup.getParent() ) + Text(" ?"),
		  helpID("AbelianIsSGEqualToTheGroup",theSubgroup.getParent()),
		  Text(Name(theSubgroup)) + "="
		  + Name(theSubgroup.getParent()) + "?" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ") + Name( Parent(theSubgroup) ) 
	  + Text("/") + Name( theSubgroup ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// ---------------------- AbelianSGIndexProblem ---------------------------- //

AbelianSGIndexProblem::AbelianSGIndexProblem( SMSubgroup& S )
  : Supervisor( true ),
    theSubgroup( S ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) resultIsFast();
}

void AbelianSGIndexProblem::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    Integer answer = A.order();
    LogMessage msg( *this, theSubgroup );
    msg << Name( theSubgroup ) << " has";
    if( answer == 0 ) msg << "infinite index";
    else msg << "index" << answer;
    msg << ": the canonical decomposition of " << Name(Parent(theSubgroup)) 
	<< "/" << Name( theSubgroup ) << "is" << A << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianSGIndexProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Index of a subgroup problem"),
		  Text("Compute the index of") + Name( theSubgroup )
		  + Text("in") + Name( theSubgroup.getParent() ),
		  helpID("AbelianSGIndexProblem", theSubgroup.getParent()),
		  Text("Index") + Name(theSubgroup) );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ")
	  + Name( Parent( theSubgroup ) ) + Text("/") + Name( theSubgroup ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// ---------------------------- AbelianSGMinGens --------------------------- //

AbelianSGMinGens::AbelianSGMinGens( SMSubgroup& S )
  : Supervisor( true ),
    theSubgroup( S ),
    theGroup( S.getParent() ),
    abelianSGInvariants( *this, theSubgroup.scm().abelianSGInvariants )
{
  if( theSubgroup.sic.haveCyclicDecomposition() ) resultIsFast();
}

void AbelianSGMinGens::takeControl( )
{
  if( theSubgroup.sic.haveCyclicDecomposition() ) {
    LogMessage msg(*this, theSubgroup);
    msg << "The minimal number of generators of "
	<< Name( theSubgroup ) << " is " 
	<< theSubgroup.sic.getSGPresentation().newToOldGens().length() 
	<< " : this follows from the canonical decomposition of "
	<< Name( theSubgroup ) << ".";
    msg.send();
    adminTerminate();   
  }
}

void AbelianSGMinGens::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Minimal Number of Generators",
		  Text("Compute the minimal number of generators of") 
		  + Name( theSubgroup ),
		  helpID("AbelianSGMinGens", theGroup), Text("Min Gens") );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ") + Name( theSubgroup ),
	  abelianSGInvariants.arcSlotID(), 50 );
  pv.done();
}

// -------------------- AbelianIsSGIsolatedProblem ------------------------- //

AbelianIsSGIsolatedProblem::AbelianIsSGIsolatedProblem( SMSubgroup& S )
  : Supervisor( true ),
    theSubgroup( S ),
    abelianInvariants( *this, S.scm().abelianInvariantsOfFactor )
{
  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) resultIsFast();
}

void AbelianIsSGIsolatedProblem::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    Integer answer = A.orderOfTheTorsionSubgroup();
    LogMessage msg( *this, theSubgroup );
    msg << Name( theSubgroup ) << " is";
    if( answer > 1 ) msg << " not";
    msg << " isolated : the canonical decomposition of" 
	<< Name(Parent(theSubgroup)) << "/" << Name( theSubgroup ) 
	<< "is" << A << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianIsSGIsolatedProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Isolation problem"),
		  Text("Is") + Name( theSubgroup ) + Text("isolated in") 
		  + Name( theSubgroup.getParent() ) + Text("?"),
		  helpID("AbelianIsSGIsolatedProblem",theSubgroup.getParent()),
		  Text("Is")+Text(Name(theSubgroup)) + Text("isolated?") );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ")
	  + Name( Parent( theSubgroup ) ) + Text("/") + Name( theSubgroup ),
	  abelianInvariants.arcSlotID(), 50 );
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
  if( subgroup1.sic.haveCyclicDecompositionOfFactor() ) resultIsFast();
}

void AbelianSGContainmentProblem::takeControl( )
{
  if ( subgroup1.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = subgroup1.sic.getCyclicDecompositionOfFactor();
    bool answer = yes;
    VectorOf<Word> V = subgroup2.getSubgroup().generators();
    for( int i = 0; i < V.length(); ++i )
      if( !A.isTrivial( V[i] ) ) { answer = no; break; }
    LogMessage msg( *this, subgroup1, subgroup2 );
    msg << Name( subgroup1 );
    if( !answer ) msg << " doesn't contain ";
    else msg << " contains ";
    msg << Name( subgroup2 )
	<< ": this follows by computing the abelian invariants of "
	<< Name( Parent( subgroup1 ) ) << "/" << Name( subgroup1 ) << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianSGContainmentProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Subgroup containment problem"),
		  Text("Does ") + Name( subgroup1 ) + Text(" contain ")
		  + Name( subgroup2 ) + Text(" ?"), 
		  helpID("AbelianSGContainmentProblem", 
			 subgroup1.getParent()),
		  Text(Name(subgroup1)) + "cont" + Name( subgroup2 ) + "?" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ") + Name( Parent(subgroup1) )
	  + Text("/") + Name(subgroup1), abelianInvariants.arcSlotID(), 50 );

  pv.done();
}

// ------------------- AbelianSGDirectFactorProblem ----------------------- //

AbelianSGDirectFactorProblem::AbelianSGDirectFactorProblem( SMSubgroup& S1, SMSubgroup& S2 )
  : Supervisor(true), sgDone1(false), sgDone2(false),
    subgroup1( S1 ), subgroup2( S2 ),
    abelianSGInvariants1( *this, subgroup1.scm().abelianSGInvariants ),
    abelianSGInvariants2( *this, subgroup2.scm().abelianSGInvariants )
{
  if( subgroup1.sic.haveCyclicDecomposition() ) sgDone1 = true;
  if( subgroup2.sic.haveCyclicDecomposition() ) sgDone2 = true;
  if( sgDone1 && sgDone2 ) resultIsFast();
}

void AbelianSGDirectFactorProblem::takeControl( )
{
  if( subgroup1.sic.haveCyclicDecomposition() &&
      subgroup2.sic.haveCyclicDecomposition() ) {
    AbelianSGPresentation A1 = subgroup1.sic.getSGPresentation();
    AbelianSGPresentation A2 = subgroup2.sic.getSGPresentation();
    if( !subgroup1.sic.havePrimaryDecomposition( ) ) {
	A1.findPrimaryBasis();
	subgroup1.sic.putHavePrimaryDecomposition(A1);
    }
    LogMessage msg(*this,subgroup1,subgroup2);
    VectorOf<AbelianWord> cyclicGens = subgroup2.sic.getSGPresentation().newToOldGens();
    Word tmpWord, newWord;
    bool result = true;
    for(int i=0; i<cyclicGens.length(); i++) {
      tmpWord = A2.fromSGPGensToSGGens(cyclicGens[i].getWord());
      if(!A1.fromSGGensToSGPGens(tmpWord,newWord)) {
	msg << Name(subgroup2) << " is not in " << Name(subgroup1) << ".";
	msg.send();
	adminTerminate();
	return;
      }
      else if(!A1.isPureCyclSubgroup(newWord)) { result = false; break; }
    }
    msg << Name(subgroup2) << " is";
    if(!result) msg << " not";
    msg << " a direct factor of " << Name(subgroup1) << ".";
    msg.send( );
    adminTerminate();
    return;
  }
}

void AbelianSGDirectFactorProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), "Subgroup direct factor problem",
		  Text("Is") + Name(subgroup2) + Text("a direct factor of")
		  + Name(subgroup1) + Text(" ?"), 
		  helpID("AbelianSGDirectFactorProblem",subgroup1.getParent()),
		  Text(Name(subgroup2)) + "factor" + Name(subgroup1) + "?" );
  pv.startItemGroup();
  if(!sgDone1)
    pv.add( Text("Compute abelian invariants of ") + Name( subgroup1 ),
	    abelianSGInvariants1.arcSlotID(), 50 );
  if(!sgDone2)
    pv.add( Text("Compute abelian invariants of ") + Name( subgroup2 ),
	    abelianSGInvariants2.arcSlotID(), 50 );
  pv.done();
}

// -------------------- AbelianPHeightOfEltProblem ------------------------- //

AbelianPHeightOfEltProblem::AbelianPHeightOfEltProblem(SMWord& w, int p)
  : Supervisor( true ),
    theWord( w ),
    abDone( false ),
    P (p),
    abelianInvariants( *this, w.getParent().gcm().abelianInvariants ),
    abelianPrimes( *this, w.getParent().gcm().abelianPrimes ),
    notPrime(false)
{
  if (!PrimeNumbers().isPrime(p)) { notPrime = true; resultIsFast(); }
  if ( w.getParent().gic.haveCyclicDecomposition() ) abDone = true;
  if ( w.getParent().gic.havePrimaryDecomposition() ) resultIsFast();
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
    Integer pHeight = theWord.getParent().gic.getCyclicDecomposition().
      pHeightOfElt(theWord.getWord(),P);
    LogMessage msg( *this, theWord );
    msg << "The " << P << "-height of " << Name( theWord ) << " is ";
    if (pHeight<0) msg << "infinite";
    else msg << pHeight ;
    msg << ".";
    msg.send();
    adminTerminate();
  }
}

void AbelianPHeightOfEltProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the p-height"),
		  Text("Find the p-height of ") + Name( theWord ),
		  helpID("AbelianPHeightOfEltProblem", theWord.getParent()),
		  Text("p-height") + Name(theWord) );
  pv.startItemGroup();
  if (!abDone)
    pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	    abelianInvariants.arcSlotID(), 50 );
  pv.add(Text("Find the primary decomposition of")+Name( Parent( theWord ) ),
	 abelianPrimes.arcSlotID(), 50);
  pv.done();
}

//-------------- AbelianComputeTorsionSubgroup --------------------//

AbelianComputeTorsionSubgroup::AbelianComputeTorsionSubgroup(SMFPGroup& G)
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
      msg << Name( theGroup ) 
	  << " does not have any non trivial torsion subgroup.";
      msg.send();
    }
    else {
      for (int i=0;i<torsionGens.length();i++)
	torsionGens[i]=cyclicGens[i+A.rankOfFreeAbelianFactor()].getWord();
      SGofFreeGroup tmpSG(theGroup.getFreePreimage(),torsionGens);
      msg << "Torsion subgroup of " << Name(theGroup ) << " is computed.";
      msg.send();
      SMObject* smo = new SMSubgroup( theGroup, tmpSG, Text("Torsion subgroup of") + Name( theGroup ) );
      ListOf<OID> dependencies( theGroup );
      CheckinMessage( *smo, "", dependencies ).send(); 
    }
    adminTerminate();
  }
}

void AbelianComputeTorsionSubgroup::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the torsion subgroup of "),
		  Text("Compute the torsion subgroup of ") + Name( theGroup ), 
		  helpID("AbelianComputeTorsionSubgroup", theGroup),
		  "Tor subgp" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.done();
}

// -------------------- AbelianEltPowerSubgrARCer ------------------------- //


void AbelianEltPowerSubgrARCer::setArguments
( const AbelianGroup& group, const Word& w, const VectorOf<Word>& sG)
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

// ----------------------- AbelianEltPowerSubgrComp ------------------------ //

AbelianEltPowerSubgrComp::AbelianEltPowerSubgrComp(AbelianEltPowerSubgr& sup)
  : theSupervisor( sup ),
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
	    << Name(theSupervisor.getWord()) << " belongs to " 
	    << Name( theSupervisor.getSubgroup()) 
	    << " and it is minimal positive possible.";
      }
      else msg << "No powers of " <<  Name( theSupervisor.getWord())
	       << " belongs to " <<  Name( theSupervisor.getSubgroup()) << ".";
      msg.send();  
      finished = true;
      adminTerminate();
      return;
    } 
    usedOneARC();
  }
}

// ---------------------- AbelianEltPowerSubgr ---------------------------- //

AbelianEltPowerSubgr::AbelianEltPowerSubgr( const SMSubgroup& smg, const SMWord& smw)
  : Supervisor( true ),
    theWord( smw ),
    theSubgroup( smg ),
    abelianEltPowerSubgr( *this ) 
{ }

void AbelianEltPowerSubgr::takeControl( )
{
  if (abelianEltPowerSubgr->isDone()) adminTerminate();
}

void AbelianEltPowerSubgr::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Compute the order of an element modulo a subgroup"),
		  Text("Compute the order of ")
		  + Name( theWord ) + Text("modulo")+ Name( theSubgroup ) ,
		  helpID("AbelianEltPowerSubgr", theSubgroup.getParent()),
		  "Pow in subgp" );
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
  if ( w.getParent().gic.haveCyclicDecomposition() ) {
    if ( w.getParent().gic.getCyclicDecomposition().orderOfElt(w.getWord())!=0)
      resultIsFast();
    else abDone = true;
  }
  else if ( w.getParent().gic.havePrimaryDecomposition() ) resultIsFast();
}

void AbelianPowerProblem::takeControl( )
{
  if ( theWord.getParent().gic.havePrimaryDecomposition() || fastResult() ) {
    LogMessage msg( *this, theWord );
    msg << Name( theWord );
    if ( theWord.getParent().gic.getCyclicDecomposition().orderOfElt( theWord.getWord() ) !=0){
      msg << " is of finite order, hence it is a proper power.";
    }
    else if (theWord.getParent().gic.getCyclicDecomposition().isEltProperPower( theWord.getWord() ))
      msg << " is of infinite order and it is a proper power.";
    else
      msg << " is of infinite order and it is not a proper power.";
    msg.send();
    adminTerminate();
  }
}

void AbelianPowerProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is element a proper power"),
		  Text("Find is the ") + Name( theWord ) 
		  + Text(" a proper power"),
		  helpID("AbelianPowerProblem", theWord.getParent()),
		  Text("Powr prob") );
  pv.startItemGroup();
  
  if (!abDone)
    pv.add( Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	    abelianInvariants.arcSlotID(), 50 );
  pv.add(Text("Find the primary docomposition of")+ Name( Parent( theWord ) ),
	 abelianPrimes.arcSlotID(), 50);
  pv.done();
}

// ------------------- AbelianInvariantsOfSGARCer ------------------------- //

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

// ------------------------ AbelianSGInvariants ---------------------------- //

AbelianSGInvariants::AbelianSGInvariants(SCM& scm)
  : theSCM( scm ),    
    presentationStarted( false ),
    arcer( *this )
{
  adminStart();
}

void AbelianSGInvariants::takeControl( )
{
  if (freeARCs()>0) {
    if (!presentationStarted) {
      arcer.setArguments( AbelianGroup(theSCM.getSubgroup().getParent().getFPGroup()), theSCM.getSubgroup().getSubgroup().generators());
      presentationStarted = true;   
    }
    if( arcer.takeControl() ) {
      theSCM.getSubgroup().sic.putHaveCyclicDecomposition( arcer.getSGCyclicDecomposition() );
      LogMessage msg(*this,theSCM.getSubgroup());
      msg << "The canonical decomposition of " << Name(theSCM.getSubgroup())
	  << " :" << arcer.getSGCyclicDecomposition() << ".";
      msg.send();       
      LogMessage msg1(*this,theSCM.getSubgroup());
      msg1 << Link(Chars( "Click here to see the generators of the canonical decomposition of ") + Text( Name(theSCM.getSubgroup()) ),
		   "CyclicDecomposition",
		   arcer.getSGCyclicDecomposition().getFileName() );
      msg1.send();  
      adminTerminate();
      return;
    } 
    usedOneARC();
  }
}

// ------------------- AbelianSGCyclicDecomposition -----------------------//

AbelianSGCyclicDecomposition::AbelianSGCyclicDecomposition(SMSubgroup& S)
  : Supervisor( true ),
    theSubgroup( S ),
    abelianSGInvariants( *this, S.scm().abelianSGInvariants )
{
  if( theSubgroup.sic.haveCyclicDecomposition( ) ) resultIsFast();
}


void AbelianSGCyclicDecomposition::takeControl( )
{
  if( fastResult() ) {
    LogMessage msg(*this, theSubgroup);
    msg << "The canonical decomposition of " << Name( theSubgroup ) << " :";
    msg << theSubgroup.sic.getSGPresentation(); 
    msg << ".";
    msg.send( );
    adminTerminate();
  }     
  else if ( theSubgroup.sic.haveCyclicDecomposition( ) ) adminTerminate();
}

void AbelianSGCyclicDecomposition::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Compute the canonical decomposition of a subgroup"),
		  Text("Compute the canonical decomposition of")
		  + Name( theSubgroup ),
		  helpID("AbelianSGCyclicDecomposition", 
			 theSubgroup.getParent()), "Can dec" );
  pv.startItemGroup();
  pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
	 abelianSGInvariants.arcSlotID(), 50);
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
       msg << "The primary decomposition of " 
	   << Name( theSupervisor.getSubgroup() ) << " :";
       arcer.getSGPrimaryDecomposition().printPrimaryDec(msg); 
       msg << ".";
       msg.send();       
       LogMessage msg1(*this,theSupervisor.getSubgroup());
       msg1 << Link(Chars( "Click here to see the generators of the primary decomposition of ") + Text(Name(theSupervisor.getSubgroup())),
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
    abelianSGInvariants( *this, S.scm().abelianSGInvariants ),
    abelianSGPrimes( *this )
{
  if( theSubgroup.sic.haveCyclicDecomposition( ) ) abSGDone = true;
  if( theSubgroup.sic.havePrimaryDecomposition( ) ) resultIsFast();
}

void AbelianSGPrimesDecomposition::takeControl( )
{
  if ( theSubgroup.sic.havePrimaryDecomposition( ) ){
    LogMessage msg(*this, theSubgroup);
    msg << "The primary decomposition of " << Name( theSubgroup ) << " :";
    theSubgroup.sic.getSGPresentation().printPrimaryDec(msg); 
    msg << ".";
    msg.send( );
    adminTerminate();
  }     
}

void AbelianSGPrimesDecomposition::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Compute the primary decomposition of a subgroup"),
		  Text("Compute the primary decomposition of")
		  + Name( theSubgroup ),
		  helpID("AbelianSGPrimesDecomposition", 
			 theSubgroup.getParent()), "Prime dec" );
  pv.startItemGroup();
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
    abelianSGInvariants( *this, S.scm().abelianSGInvariants )
{
  if( theSubgroup.sic.haveCyclicDecomposition( ) ) resultIsFast();
}

void AbelianSGOrder::takeControl( )
{
  if (theSubgroup.sic.haveCyclicDecomposition( )){
    Integer theOrder = theSubgroup.sic.getSGPresentation().order();
    LogMessage msg;
    msg << "The order of " << Name( theSubgroup ) << " is ";
    if( sign(theOrder) == 0 )  msg << "infinite";
    else {
      msg << theOrder;
      if (theOrder > 3){
	msg << "=";
	PrimeNumbers pn;
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
  ProblemView pv( ostr, oid(), Text("Compute the order of a subgroup"),
		  Text("Compute the order of") + Name( theSubgroup ),
		  helpID("AbelianSGOrder", theSubgroup.getParent()), "Order" );
  pv.startItemGroup();
  pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
	 abelianSGInvariants.arcSlotID(), 50);
  pv.done();
}

// ---------------------- AbelianMaximalRootARCer -------------------------- //

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

// --------------------- AbelianMaximalRootComp ---------------------------- //

AbelianMaximalRootComp::AbelianMaximalRootComp(AbelianMaximalRoot& sup)
  : theSupervisor( sup ),
    rootStarted( false ),
    A(*this)
{
  adminStart();
}

void AbelianMaximalRootComp::takeControl( )
{
  if (freeARCs()>0) {
    if (theSupervisor.getWord().getParent().gic.haveCyclicDecomposition())
      if (theSupervisor.getWord().getParent().gic.getCyclicDecomposition().orderOfElt(theSupervisor.getWord().getWord())!=0) {
	LogMessage msg(*this, theSupervisor.getWord());
	msg << Name( theSupervisor.getWord() ) 
	    << " is of finite order, so it does not have a maximal root.";
	msg.send();   
	theSupervisor.getWord().wic.putHaveMaximalRoot(Word(),0);
	adminTerminate();
	return;
      }   
    if (theSupervisor.getWord().getParent().gic.havePrimaryDecomposition()){
      if (!rootStarted) {
	A.setArguments( theSupervisor.getWord().getParent().gic.getCyclicDecomposition(), theSupervisor.getWord().getWord() );
	rootStarted = true;   
      }
      if( A.takeControl() ) {
	LogMessage msg(*this,theSupervisor.getWord());
	if (A.getPower() == 0)
	  msg << Name(theSupervisor.getWord() ) 
	      << " is of infinite order and it does not have a maximal root.";
	else {
	  msg << Name( theSupervisor.getWord() ) 
	      << " is of infinite order, it's maximal root is ";
	  theSupervisor.getWord().getParent().getFPGroup().printWord(msg,A.getRoot());
	  msg << " and " << A.getPower() 
	      << ordinalPostfix(A.getPower().as_long()) 
	      << " power of it is " << Name(theSupervisor.getWord()) << ".";
	}
	msg.send();  
	theSupervisor.getWord().wic.putHaveMaximalRoot(A.getRoot(), A.getPower());
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
  if (theWord.getParent().gic.haveCyclicDecomposition()) abDone = true;
  if( theWord.getParent().gic.havePrimaryDecomposition()) abPDone=true;  
  if( theWord.wic.isPower( )!=dontknow ) resultIsFast();
}

void AbelianMaximalRoot::takeControl( )
{
  if ( theWord.wic.isPower( )!=dontknow ){
    LogMessage msg(*this, theWord);
    Integer order = theWord.getParent().gic.getCyclicDecomposition().orderOfElt(theWord.getWord() );
    if (order!=0 )
      msg << Name(theWord ) 
	  << " is of finite order, so it does not have a maximal root.";
    else if (theWord.wic.isPower( )==no)
      msg << Name(theWord ) 
	  << " is of infinite order, it does not have a maximal root.";
    else {
      Word w;
      Integer expon;
      theWord.wic.getMaximalRoot(w,expon);
      msg << Name( theWord ) 
	  << " is of infinite order, it's maximal root is ";
      theWord.getParent().getFPGroup().printWord(msg,w);
      msg << " and " << expon << "-th power of it is " << Name(theWord );
      msg << ".";
    }
    msg.send();      
    adminTerminate();
  }
}

void AbelianMaximalRoot::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the maximal root of element"),
		  Text("Compute the maximal root of ") + Name( theWord ),
		  helpID("AbelianMaximalRoot", theWord.getParent()), 
		  "Max root" );
  pv.startItemGroup();
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	   abelianInvariants.arcSlotID(), 50);
  if (!abPDone)
    pv.add( Text("Find the primary decomposition of")
	    + Name( Parent( theWord ) ), abelianPrimes.arcSlotID(), 50);
  pv.add( Text("Compute the maximal root"), 
	  abelianMaximalRoot.arcSlotID(), 100);
  pv.done();
}

// ------------------- AbelianIsIsomorphicSG -----------------------//

AbelianIsIsomorphicSG::AbelianIsIsomorphicSG(SMSubgroup& S1,SMSubgroup& S2)
  : Supervisor( true ),
    theSubgroup1( S1 ),
    theSubgroup2( S2 ),
    sg1abDone( false ),
    sg2abDone( false ),
    abelianSG1Invariants( *this, S1.scm().abelianSGInvariants ),
    abelianSG2Invariants( *this, S2.scm().abelianSGInvariants )
{
  if( theSubgroup1.sic.haveCyclicDecomposition( ) ) sg1abDone = true;
  if( theSubgroup2.sic.haveCyclicDecomposition( ) ) sg2abDone = true;
  if (sg2abDone && sg1abDone) resultIsFast();
}


void AbelianIsIsomorphicSG::takeControl( )
{
  if( theSubgroup1.sic.haveCyclicDecomposition() &&
      theSubgroup2.sic.haveCyclicDecomposition() ) {
    LogMessage msg(*this,theSubgroup1,theSubgroup2);
    if (theSubgroup1.sic.getSGPresentation().isomorphicTo(theSubgroup2.sic.getSGPresentation()) )
       msg << Name( theSubgroup1 ) << " is isomorphic to " 
	   << Name( theSubgroup2 ) << ".";
    else
       msg << Name( theSubgroup1 ) << " is not isomorphic to " 
	   << Name( theSubgroup2 ) << ".";
    msg.send( );
    adminTerminate();
  }
}

void AbelianIsIsomorphicSG::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Isomorphism problem"),
                  Text("Is ") + Name( theSubgroup1 ) + 
                  Text(" isomorphic to ") +  Name( theSubgroup2 ),
		  helpID("AbelianIsIsomorphicSG", theSubgroup1.getParent()),
		  "Is isom" );
  pv.startItemGroup();
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
    abelianSGInvariants( *this, SG.scm().abelianSGInvariants )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) resultIsFast();
}

void AbelianTorsionFreeRankOfSG::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) {
    LogMessage msg( *this, theSubgroup );
    msg << "The torsion-free rank of " << Name( theSubgroup ) << " is "
	<< theSubgroup.sic.getSGPresentation().rankOfFreeAbelianFactor()
	<< ": this follows from the canonical decomposition of "
	<< Name(theSubgroup) << "."  ;
    msg.send();
    adminTerminate();
  }
}

void AbelianTorsionFreeRankOfSG::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the torsion-free rank of a subgroup"),
		  Text("Find the torsion-free rank of ") + Name( theSubgroup ),
		  helpID("AbelianTorsionFreeRankOfSG", theSubgroup.getParent()),
		  "T-f rank" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of subgroup ") + Name(theSubgroup),
	  abelianSGInvariants.arcSlotID(), 50 );
  pv.done();
}

//-------------- AbelianOrderOfTheTorsionSubgroupOfSG --------------------//

AbelianOrderOfTheTorsionSubgroupOfSG::AbelianOrderOfTheTorsionSubgroupOfSG(SMSubgroup& SG)
  : Supervisor( ! SG.sic.haveCyclicDecomposition() ),
    theSubgroup( SG ),
    abelianSGInvariants( *this, SG.scm().abelianSGInvariants )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) resultIsFast();
}


void AbelianOrderOfTheTorsionSubgroupOfSG::takeControl( )
{
  if ( theSubgroup.sic.haveCyclicDecomposition() ) {
    Integer theOrder = theSubgroup.sic.getSGPresentation().orderOfTheTorsionSubgroup();
    LogMessage msg( *this, theSubgroup );
    msg << "The order of the torsion subgroup of " << Name( theSubgroup )
	<< " is ";
    if( sign(theOrder) == 0 ) msg << "infinite";
    else {
      msg << theOrder;
      if (theOrder > 3){
	msg << "=";
	PrimeNumbers pn;
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
  ProblemView pv( ostr, oid(), Text("Compute the order of the torsion subgroup"),
		  Text("Compute the order of the torsion subgroup of ") 
		  + Name( theSubgroup ),
		  helpID("AbelianOrderOfTheTorsionSubgroupOfSG", 
			 theSubgroup.getParent()), "Ordr t-f subg" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of ") + Name( theSubgroup ),
	    abelianSGInvariants.arcSlotID(), 50 );
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
  if (theWord.getParent().gic.haveCyclicDecomposition()) abDone = true;
  if( theWord.getParent().gic.havePrimaryDecomposition()) resultIsFast();
}

void EltPrimeForm::takeControl( )
{
  if (theWord.getParent().gic.havePrimaryDecomposition( )){
    LogMessage msg(*this,theWord);
    msg << "The primary decomposition of " << Name( theWord ) << ": ";
    const AbelianGroup& A = theWord.getParent().gic.getCyclicDecomposition();
    A.printInPrimaryForm( msg, A.findEltPrimeForm(theWord.getWord()));
    msg <<".";
    msg.send();
    adminTerminate();
  }     
}

void EltPrimeForm::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the primary decomposition of an element"),
		  Text("Compute the primary decomposition of") + Name( theWord ),
		  helpID("EltPrimeForm", theWord.getParent()),
		  "Prime form" );
  pv.startItemGroup();
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord ) ),
	   abelianInvariants.arcSlotID(), 50);
  pv.add(Text("Find the primary decomposition of") + Name( Parent( theWord ) ),
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
    arcer( *this ),
    started( false )
{ }

void virtualFreeComp::takeControl( )
{
  if (freeARCs()>0) {
    if ( !started ) {
      arcer.setArguments( AbelianGroup(theSubgroup.getParent().getFPGroup()),
			  theSubgroup.getSubgroup().generators() );
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
      else {
	msg << "Found a virtual free complement of " 
	    << Name(theSubgroup) << ".";
	msg.send();
	SMObject* smo = new SMSubgroup( theSubgroup.getParent(), tmpSG,
	  Text("Virtual free complement of") + Name( theSubgroup ) );
	ListOf<OID> dependencies( theSubgroup.getParent() );
	CheckinMessage( *smo, "", dependencies ).send();
      }
      adminTerminate();
      return;
    } 
    usedOneARC();
  }
}

void virtualFreeComp::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find a virtual free complement of a subgroup."),
		  Text("Compute a virtual free complement of")
		  + Name( theSubgroup ),
		  helpID("virtualFreeComp", theSubgroup.getParent()),
		  "Virt comp" );
  pv.startItemGroup();
  pv.add(Text("Compute a virtual free complement of") + Name( theSubgroup ),
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
    arcer( *this ),
    started( false )
{ }

void SubgroupIsolator::takeControl( )
{
  if (freeARCs()>0){
    if ( !started ) {
      arcer.setArguments( AbelianGroup(theSubgroup.getParent().getFPGroup()), 
			  theSubgroup.getSubgroup().generators());
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
      else {
	msg << "Found the isolator of " << Name(theSubgroup ) << ".";
	msg.send();
	SMObject* smo = new SMSubgroup( theSubgroup.getParent(), tmpSG, 
	  Text("The isolator of") + Name( theSubgroup ) );
	ListOf<OID> dependencies( theSubgroup.getParent() );
	CheckinMessage( *smo, "", dependencies ).send(); 
      }
      adminTerminate();
      return;
    }
    usedOneARC();
  }
}

void SubgroupIsolator::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find subgroup isolator."),
		  Text("Compute the subgroup isolator of") + Name(theSubgroup),
		  helpID("SubgroupIsolator", theSubgroup.getParent()),
		  "Isolator" );
  pv.startItemGroup();
  pv.add(Text("Compute the subgroup isolator of") + Name( theSubgroup ), 
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
  for (int i=0; i<cyclicGens.length(); i++) {
    // transfer a cyclic generator to the group generators
    tmpWord = theSubgroup.sic.getSGPresentation().fromSGPGensToSGGens(cyclicGens[i].getWord());
    // If cyclic subgroup generated by the generator of
    // the cyclic decomposition of a subgroup not pure, than 
    // the whole subgroup is not pure.
    if (!theSubgroup.getParent().gic.getCyclicDecomposition().isPureCyclSubgroup(tmpWord)) return false;
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
    arcer(*this, sms)
{
  if (theSubgroup.getParent().gic.haveCyclicDecomposition())  abDone = true;
  if (theSubgroup.getParent().gic.havePrimaryDecomposition()) PDDone = true;
  if( theSubgroup.sic.haveCyclicDecomposition( ) ) sgAbDone = true;
  if( theSubgroup.sic.isPure( )!= dontknow ) resultIsFast();
}

void AbelianSGPurityProblem::takeControl( )
{
  if (freeARCs()>0) {
    if (theSubgroup.sic.haveCyclicDecomposition() && 
	theSubgroup.getParent().gic.havePrimaryDecomposition() )
      if(arcer.takeControl()) 
	theSubgroup.sic.putIsPure(arcer.isPureSubgroup());
    usedOneARC();
  }
  if (theSubgroup.sic.isPure( ) != dontknow ) {
    LogMessage msg(*this,theSubgroup);
    if (theSubgroup.sic.isPure( )== yes )
      msg << "The subgroup " << Name( theSubgroup ) << " is pure.";
    if (theSubgroup.sic.isPure( )== no )
      msg << "The subgroup " << Name( theSubgroup ) << " is not pure.";
    msg.send( );
    adminTerminate();
  }
}


void AbelianSGPurityProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is the subgroup pure"),
		  Text("Is the subgroup ") + Name(theSubgroup) + Text(" pure"),
		  helpID("AbelianSGPurityProblem", theSubgroup.getParent()),
		  "Pure" );
  pv.startItemGroup();
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + Name(Parent(theSubgroup)),
	   abelianInvariants.arcSlotID(), 50);
  if (!PDDone)
    pv.add(Text("Find the primary decomposition of") 
	   + Name( Parent( theSubgroup ) ), abelianPrimes.arcSlotID(), 50);
  if (!sgAbDone)
       pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
	      abelianSGInvariants.arcSlotID(), 50);
  pv.add(Text("For use of this problem"), ThisARCSlotID(), 50);
  pv.done();
}

// ------------------- AbelianDoesGensSummand -----------------------//

AbelianDoesGensSummand::AbelianDoesGensSummand(SMWord& smw)
  : Supervisor( true ),
    theWord( smw ),
    theGroup( smw.getParent() ),
    abDone( false ),
    abelianInvariants( *this, theGroup.gcm().abelianInvariants ),
    abelianPrimes( *this, theGroup.gcm().abelianPrimes )
{
  if (theGroup.gic.haveCyclicDecomposition())  abDone = true;
  if (theGroup.gic.havePrimaryDecomposition()) resultIsFast();
}

void AbelianDoesGensSummand::takeControl( )
{
  if( theGroup.gic.havePrimaryDecomposition() ) {
    bool result = theGroup.gic.getCyclicDecomposition().isPureCyclSubgroup(theWord.getWord());
    LogMessage msg(*this,theWord,theGroup);
    msg << Name(theWord);
    if(result) msg << " generates a direct summand.";
    else msg << " does not generate a direct summand.";
    msg.send( );
    adminTerminate();
  }
}


void AbelianDoesGensSummand::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Does the word generate a direct summand"),
		  Text("Does ") + Name( theWord ) 
		  + Text(" generate a direct summand."),
		  helpID("AbelianDoesGensSummand", theGroup),
		  "Gens direct sumnd?" );
  pv.startItemGroup();
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + Name( theGroup ),
	   abelianInvariants.arcSlotID(), 50);
  pv.add(Text("Find the primary decomposition of") + Name( theGroup ),
	 abelianPrimes.arcSlotID(), 50);
  pv.done();
}

// ------------------- AbelianSGGenedByWordPurityProblem --------------------//

AbelianSGGenedByWordPurityProblem::AbelianSGGenedByWordPurityProblem(SMWord& smw)
  : Supervisor( true ),
    theWord( smw ),
    theGroup( smw.getParent() ),
    abDone(false),
    abelianInvariants( *this, smw.getParent().gcm().abelianInvariants ),
    abelianPrimes( *this, smw.getParent().gcm().abelianPrimes )
{
  if ( theGroup.gic.haveCyclicDecomposition()) abDone = true;
  if ( theGroup.gic.havePrimaryDecomposition()) resultIsFast();
}


void AbelianSGGenedByWordPurityProblem::takeControl( )
{
  if( theGroup.gic.havePrimaryDecomposition() ) { 
    bool result = theGroup.gic.getCyclicDecomposition().isPureCyclSubgroup(theWord.getWord());
    LogMessage msg(*this,theWord,theGroup);
      msg << "The subgroup generated by " << Name( theWord );
      if (result) msg << " is pure.";
      else msg << " is not pure.";
    msg.send( );
    adminTerminate();
  }
}


void AbelianSGGenedByWordPurityProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Is the subgroup generated by a word pure"),
		  Text("Compute is the subgroup generated by ")
		  + Name( theWord ) + Text(" pure"),
		  helpID("AbelianSGGenedByWordPurityProblem", theGroup),
		  "Is SG pure" );
  pv.startItemGroup();
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + Name( theGroup ),
	   abelianInvariants.arcSlotID(), 50);
  pv.add(Text("Find the primary decomposition of") + Name( theGroup ),
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
 if( subgroup1.sic.haveCyclicDecompositionOfFactor() &&
     subgroup2.sic.haveCyclicDecompositionOfFactor() ) resultIsFast();
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
    for( int i=0; i < V1.length(); ++i ) if( !A2.isTrivial( V1[i] ) ) {
      answer = no;
      break;
    }
    for( int i=0; i < V2.length(); ++i ) if( !A1.isTrivial( V2[i] ) ) {
      answer = no;
      break;
    }
    LogMessage msg( *this, subgroup1, subgroup2 );
    msg << Name( subgroup1 );
    if( !answer ) msg << " is not equal to  ";
    else msg << " is equal to  ";
    msg << Name( subgroup2 ) << ".";
    msg.send();
    adminTerminate();
  }
}


void AbelianSGEqualityProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Subgroup equality problem"),
		  Text("Does ") + Name( subgroup1 ) + Text(" = ")
		  + Name( subgroup2 ) + Text(" ?"), 
		  helpID("AbelianSGEqualityProblem", subgroup1.getParent()),
		  Text(Name(subgroup1)) + "=" + Name( subgroup2 ) + "?" );
  pv.startItemGroup();
  if (!s1AbDone)
    pv.add( Text("Compute abelian invariants of ") + Name( Parent(subgroup1) )
	    + Text("/") + Name( subgroup1 ),
	    s1AbelianInvariantsOfFactor.arcSlotID(), 50 );
  if (!s2AbDone)
    pv.add( Text("Compute abelian invariants of ") + Name( Parent(subgroup1) )
	    + Text("/") + Name( subgroup2 ),
	    s2AbelianInvariantsOfFactor.arcSlotID(), 50 );
  pv.done();
}

// ------------------ IsAbelianWordPowerOfSecondArcer --------------------- //

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
  if (theWord1.getParent().gic.haveCyclicDecomposition()) abDone = true;
}


void IsAbelianWordPowerOfSecond::takeControl( )
{
  if (freeARCs()>0){
    if (theWord1.getParent().gic.haveCyclicDecomposition()){
      if (!arcerSet){
	arcer.setArguments(theWord1.getParent().gic.getCyclicDecomposition(),theWord1,theWord2); 
	arcerSet = true;
      }
      if (arcer.takeControl() ){
	int power = arcer.getPower();
	LogMessage msg( *this, theWord1, theWord2 );
	msg << Name(theWord1);
	char s[10];
	sprintf(s, "%d", power);
	if (!power) msg << " is not a power of " << Name(theWord2) << ".";
	else msg << " = " << Name(theWord2) << "^" << s << ".";
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
  ProblemView pv( ostr, oid(), Text("Is a word a power of another"),
		  Text("Is ") + Name(theWord1) + " a power of "
		  + Name(theWord2),
		  helpID("IsAbelianWordPowerOfSecond", theWord1.getParent()),
		  Text("Is") + Text(Name(theWord1)) + "a power of" 
		  + Text(Name(theWord2)) + "?" );
  pv.startItemGroup();
  if (!abDone)
    pv.add(Text("Compute abelian invariants of") + Name( Parent( theWord1 ) ),
	   abelianInvariants.arcSlotID(), 50);
  pv.add(Text("For use of this problem"), ThisARCSlotID(), 50);
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
  if (freeARCs()>0) {
    if (theMCM.getMap().getRange().gic.haveCyclicDecomposition()){
      if (!sgInvStarted) {
	arcer.setArguments( theMCM.getMap().getRange().gic.getCyclicDecomposition(), theMCM.getMap().getMap().generatingImages());
	sgInvStarted = true;   
      }
      if (!sgInvFinished) { 
	if(arcer.takeControl() ) {
	  abSG = new AbelianSGPresentation(arcer.getSGCyclicDecomposition());
	  sgInvFinished = true;
	} 
      }
      if (sgInvFinished && 
	  theMCM.getMap().getDomain().gic.haveCyclicDecomposition()) {
        AbelianGroup ab =
	  theMCM.getMap().getDomain().gic.getCyclicDecomposition();
        if (abSG->rankOfFreeAbelianFactor()!=ab.rankOfFreeAbelianFactor())
	  theMCM.getMap().mic.putIsMono(false);
	else if (abSG->invariants()!=ab.invariants())
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
  if (theMap.getRange().gic.haveCyclicDecomposition()) abRangeDone = true;
  if (theMap.getDomain().gic.haveCyclicDecomposition()) abDomainDone = true;
  if (theMap.mic.isMono()!=dontknow) resultIsFast();
}


void AbelianHomIsMono::takeControl( )
{
  if ( theMap.mic.isMono()!=dontknow ){
    LogMessage msg( *this, theMap );
    msg << Name(theMap);
    if (theMap.mic.isMono() == no) msg << " is not ";
    else msg << " is ";
    msg << "a monomorphism.";
    msg.send();    
    adminTerminate();
  }
}

void AbelianHomIsMono::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is a monomorphism"),
		  Text("Compute is ") + Name(theMap) + " a monomorphism. ",
		  helpID("AbelianHomIsMono", theMap.getDomain()),
		  Text("Is mono") + "?" );
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
    else usedOneARC();
}

// ------------------- AbelianHomIsEpi -----------------------//

AbelianHomIsEpi::AbelianHomIsEpi(SMMap& map)
  : Supervisor( true ),
	 theMap(map ),
         abelianHomIsEpi( *this, map.mcm().abelianHomIsEpi )
{
  if (theMap.mic.isEpi()!=dontknow) resultIsFast();
}


void AbelianHomIsEpi::takeControl( )
{
  if ( theMap.mic.isEpi()!=dontknow ) {
    LogMessage msg( *this, theMap );
    msg << Name(theMap);
    if (theMap.mic.isEpi() == no) msg << " is not ";
    else msg << " is ";
    msg << "an epimorphism.";
    msg.send();
    adminTerminate();
  }   
}

void AbelianHomIsEpi::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is an epimorphism"),
		  Text("Compute is the ") + Name(theMap) + " an epimorphism. ",
		  helpID("AbelianHomIsEpi", theMap.getDomain()),
		  Text("Is epi") + "?");
  pv.startItemGroup();
  pv.add(Text("For use of this problem"), abelianHomIsEpi.arcSlotID(), 50);
  pv.done();
}

// ------------------- AbelianHomIsAuto -----------------------//

AbelianHomIsAuto::AbelianHomIsAuto(SMHomomorphism& homo)
  : Supervisor( true ),
    theHomo(homo ),
    abelianHomIsEpi( *this, homo.mcm().abelianHomIsEpi )
{
  if (theHomo.mic.isEpi()!=dontknow) resultIsFast();
}


void AbelianHomIsAuto::takeControl( )
{
  if ( theHomo.mic.isEpi()!=dontknow ) {
    LogMessage msg( *this, theHomo );
    msg << Name(theHomo);
    if (theHomo.mic.isEpi() == no) msg << " is not ";
    else msg << " is ";
    msg << "an automorphism.";
    msg.send();    
    adminTerminate();
  }
}


void AbelianHomIsAuto::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is an automorphism"),
		  Text("Compute is the ")+Name(theHomo)+" an automorphism. ",
		  helpID("AbelianHomIsAuto", theHomo.getDomain()),
		  Text("Is auto") + "?" );
  pv.startItemGroup();
  pv.add(Text("Compute is ") + Name(theHomo) + Text(" an epimorphism") ,
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
  if (theHomo.getRange().gic.haveCyclicDecomposition()) abRangeDone = true;
  if (theHomo.getDomain().gic.haveCyclicDecomposition()) abDomainDone = true;
  if (theHomo.mic.isMono()!=dontknow && 
      theHomo.mic.isEpi()!=dontknow) resultIsFast();
}

void AbelianHomIsIso::takeControl( )
{
  if (theHomo.mic.isMono()!=dontknow && theHomo.mic.isEpi()!=dontknow) {
    LogMessage msg( *this, theHomo );
    msg << Name(theHomo);
    if (theHomo.mic.isMono() == yes && theHomo.mic.isEpi()==yes )
      msg << " is ";
    else msg << " is not ";
    msg << "an isomorphism.";
    msg.send();
    adminTerminate();
  }   
}

void AbelianHomIsIso::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is isomorphism"),
		  Text("Compute is the ") + Name(theHomo) + " an isomorphism. ",
		  helpID("AbelianHomIsIso", theHomo.getDomain()),
		  Text("Is iso") + "?");
  pv.startItemGroup();
  if (!abRangeDone)
     pv.add(Text("Compute abelian invariants of") + Name(theHomo.getRange()),
	    abelianInvariants1.arcSlotID(), 50);
  if (!abDomainDone)
     pv.add(Text("Compute abelian invariants of") + Name(theHomo.getDomain()),
	    abelianInvariants2.arcSlotID(), 50);
  if (theHomo.mic.isMono()==dontknow)  
    pv.add(Text("Compute is ")+ Name( theHomo) + Text(" a monomorphism") , 
	   abelianHomIsMono.arcSlotID(), 50);
  if (theHomo.mic.isEpi()==dontknow)  
    pv.add(Text("Compute is ")+ Name( theHomo) + Text(" an epimorphism") , 
	   abelianHomIsEpi.arcSlotID(), 50);
  pv.done();
}

// ---------------------- AbelianOrderOfAutoARCer ------------------------ //

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
  if (theAuto.mic.isEpi()!=yes) error("Don't know is it automorphism!!!");
}


void AbelianOrderOfAuto::takeControl( )
{
  if (freeARCs() > 0) {
    if (theAuto.getDomain().gic.haveCyclicDecomposition()){
      if (!started) {
	arcer.setArguments(theAuto.getDomain().gic.getCyclicDecomposition(),
			   theAuto.getMap().generatingImages());
	started = true;
      }
      if (arcer.takeControl()) {
	int result = arcer.getOrder();
	LogMessage msg( *this, theAuto );
	if(result < 0) msg << "Can't find the order of " << Name(theAuto);
	else msg << "The order of "<< Name(theAuto) << " is " << result << ".";
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
  ProblemView pv( ostr, oid(), Text("Compute the order of an automorphism"),
		  Text("Compute the order of ")+Name(theAuto),
		  helpID("AbelianOrderOfAuto", theAuto.getDomain()),
		  Text("order") );
  pv.startItemGroup();
  if (!theAuto.getDomain().gic.haveCyclicDecomposition() &&
      theAuto.getDomain().getFPGroup().getRelators().cardinality()!=0)
    pv.add( Text("Compute abelian invariants of") + Name(theAuto.getDomain()),
	    abelianInvariants.arcSlotID(), 50 );
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);
  pv.done();
}

// --------------------- AbelianInverseAutoARCer -------------------------- //


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
  if (theAuto.mic.isEpi()!=yes) error("Don't know is it automorphism!!!");
  else {
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
      SMHomomorphism* smo = new SMHomomorphism( theAuto.getDomain(), inverseMap, Text("The inverse of ") + Name(theAuto));
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
  ProblemView pv( ostr, oid(), Text("Find inverse of an automorphism"),
		  Text("Find inverse of ")+Name(theAuto),
		  helpID("AbelianInverseAuto", theAuto.getDomain()),
		  Text("inverse") );
  pv.startItemGroup();
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);
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
  if (theAuto.mic.isEpi() != yes) error("Don't know is it automorphism!!!");
  else {
    AbelianGroup a( theAuto.getDomain().getFPGroup() );
    arcer.setArguments( a , theAuto.getMap().generatingImages() );    
  }
}

void AbelianFixedPointsOfAutoProblem::takeControl()
{
  if (freeARCs() > 0) {
    if (arcer.takeControl()) {
      SMSubgroup* smo = new SMSubgroup( theAuto.getDomain(), SGofFreeGroup(theAuto.getDomain().getFreePreimage(),arcer.getResult()), Text("The subgroup of fixed points of ") + Name(theAuto) );
      ListOf<OID> dependencies( theAuto.getDomain() );
      CheckinMessage( *smo, "", dependencies ).send();
      adminTerminate();
      return;
    }
    usedOneARC();
  }
}

void AbelianFixedPointsOfAutoProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the subgroup of fixed points"),
		  Text("Compute the fixed points of ") + Name( theAuto ),
		  helpID("AbelianFixedPointsOfAutoProblem",theAuto.getDomain()),
		  Text("fixedPoints") );
  pv.startItemGroup();
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);
  pv.done();
}

// ---------------------- AbelianSGIntersectionARCer ----------------------- //

void AbelianSGIntersectionARCer::setArguments
( const AbelianGroup& g, const VectorOf<Word>& v1, const VectorOf<Word>& v2)
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
  if( !v1.length() || !v2.length() ) resultIsFast();
}

void AbelianSGIntersectionProblem::takeControl( )
{
  if( fastResult() ) {
    VectorOf<Word> v1 = subgroup1.getSubgroup().generators();
    VectorOf<Word> v2 = subgroup2.getSubgroup().generators();
    if( !v1.length() ) {
      LogMessage msg1( *this , subgroup1.getParent() );
      msg1 << Name( subgroup1 ) << " is trivial.";
      msg1.send();
    }
    if( !v2.length() ) {
      LogMessage msg2( *this , subgroup1.getParent() );
      msg2 << Name( subgroup2 ) << " is trivial.";
      msg2.send();
    }
    LogMessage msg3( *this , subgroup1.getParent() );
    msg3 << "Intersection of " << Name( subgroup1 ) << " and " 
	 << Name( subgroup2 ) << " is trivial.";
    msg3.send();
    adminTerminate();
    return;
  }
  if( subgroup1.getParent().gic.haveCyclicDecomposition() )  {
    if( !init )	{
      AbelianGroup A = subgroup1.getParent().gic.getCyclicDecomposition( );
      VectorOf<Word> v1 = subgroup1.getSubgroup().generators();
      VectorOf<Word> v2 = subgroup2.getSubgroup().generators();
      arcer.setArguments( A , v1 , v2 );
      init = true;
    }
    if ( freeARCs() > 0 ) 
      if( arcer.takeControl() ) {
	VectorOf<Word> result = arcer.getResult();
	LogMessage msg( *this, subgroup1.getParent() );
	if( !result.length() )
	  msg << "Intersection of " << Name( subgroup1 ) << " and " 
	      << Name( subgroup2 ) << " is trivial.";
	else {
	  AbelianGroup A(subgroup1.getParent().getFPGroup());
	  SGofFreeGroup S(subgroup1.getParent().getFreePreimage(),result); 
	  SMObject* smo = new SMSubgroup ( subgroup1.getParent(), S, Text("The intersection of") + Name( subgroup1 ) + "and" + Name( subgroup2 ) );
	  ListOf<OID> dependencies( subgroup1.getParent() );
	  CheckinMessage( *smo, "", dependencies ).send();
	  msg << Link("Click here to see the generators of intersection", 
		      "Intersection" , arcer.getFileName() );
	}
	msg.send();
	adminTerminate();
	return;
      }
      else usedOneARC();
  }
}

void AbelianSGIntersectionProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the intersection of two subgroups in abelian group"),
		  Text("Find the intersection of ") 
		  + Name( subgroup1 ) + Text(" and ") + Name( subgroup2 ) +
		  Text(" in ") + Name( subgroup1.getParent() ) ,
		  helpID("AbelianSGIntersectionProblem", subgroup1.getParent()) ,
		  Text("Intersection") );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name(subgroup1.getParent()),
	  abelianInvariants.arcSlotID(), 50 );
  pv.add( Text("For the use of this problem") + Name( subgroup1.getParent() ),
	  ThisARCSlotID(), 100 );
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

// ------------------- AbelianIntegralHomologyProblem ---------------------- //

AbelianIntegralHomologyProblem::AbelianIntegralHomologyProblem( class SMFPGroup& G, int n = 2 )
  : Supervisor( true ),
    init( false ),
    theGroup( G ),
    d( n ),
    abelianInvariants( *this , G.gcm().abelianInvariants ),
    arcer(*this)
{  }

void AbelianIntegralHomologyProblem::takeControl( )
{
  if( theGroup.gic.haveCyclicDecomposition() ) {
    if( !init ) {
      AbelianGroup A = theGroup.gic.getCyclicDecomposition( );
      arcer.setArguments( A , d );
      init = true;
    }
    if ( freeARCs() > 0 ) 
      if( arcer.takeControl() ) {
	AbelianGroup result = arcer.getResult();
	result.computeCyclicDecomposition();
	LogMessage msg( *this, theGroup );
	msg << "Dimension " << d << " integral homology of " 
	    << Name( theGroup ) << " : " << result;
	msg.send();
	adminTerminate();
	return;
      }
      else usedOneARC();
  }
}

void AbelianIntegralHomologyProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Integral homology for abelian group"),
		  Text("Compute the dimension") + d + "homology of"
		  + Name( theGroup ),
		  helpID("AbelianHomologyProblem", theGroup), "Homology" );
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(), 50 );
  pv.add( Text("For the use of this problem") + Name( theGroup ),
	  ThisARCSlotID(), 50 );
  pv.done();
}

// ------------------- IsAbelianHyperbolic -----------------------//

IsAbelianHyperbolic::IsAbelianHyperbolic(SMFPGroup& G)
  : Supervisor( true ),
    theGroup( G ),
    abelianInvariants( *this, theGroup.gcm().abelianInvariants )
{
  if(theGroup.gic.haveCyclicDecomposition() ||
     theGroup.gic.isFinite()==yes) resultIsFast();
}

void IsAbelianHyperbolic::takeControl()
{
  LogMessage msg(*this, theGroup);
  if(theGroup.gic.isFinite()==yes) {
    msg << Name(theGroup) << " is hyperbolic.";
    msg.send();
    adminTerminate();
  }
  else if(theGroup.gic.haveCyclicDecomposition()) {
    if(theGroup.gic.getCyclicDecomposition().rankOfFreeAbelianFactor()<2)
      msg << Name(theGroup) << " is hyperbolic.";
    else
      msg << Name(theGroup) << " is not hyperbolic.";
    msg.send();
    adminTerminate();
  }
}

void IsAbelianHyperbolic::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is the group hyperbolic"),
		  Text("Is") + Name( theGroup ) + "hyperbolic", 
		  helpID("IsAbelianHyperbolic", theGroup), "hyperbolic");
  pv.startItemGroup();
  pv.add(Text("Compute abelian invariants of") + Name( theGroup ),
	 abelianInvariants.arcSlotID(), 50);
  pv.done();
}

// ------------------- AbelianIsSGFinite -----------------------//

AbelianIsSGFinite::AbelianIsSGFinite(SMSubgroup& g)
  : Supervisor( true ),
    theGroup( g.getParent() ),
    theSubgroup( g ),
    abelianInvariants( *this, theGroup.gcm().abelianInvariants )
{
  if(theGroup.gic.haveCyclicDecomposition()) resultIsFast();
}

void AbelianIsSGFinite::takeControl( )
{
  if( theGroup.gic.haveCyclicDecomposition() ) {
    VectorOf<Word> v = theSubgroup.getSubgroup().generators();
    LogMessage msg(*this, theSubgroup);
    for(int i=0; i<v.length(); i++) 
      if(theGroup.gic.getCyclicDecomposition().orderOfElt(v[i])==0 ) {
	msg << Name(theSubgroup) << " is infinite: "; 
	theGroup.getFPGroup().printWord(msg, v[i]);
	msg << " is of infinite order.";
	msg.send();
	adminTerminate();
	return; 
      }
    msg << Name(theSubgroup) << " is finite.";
    msg.send();
    adminTerminate();
  }
}

void AbelianIsSGFinite::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is the subgroup finite"),
		  Text("Is") + Name( theSubgroup ) + "finite", 
		  helpID("AbelianIsSGFinite", theGroup), "finite");
  pv.startItemGroup();
  pv.add(Text("Compute abelian invariants of") + Name( theGroup ),
	 abelianInvariants.arcSlotID(), 50);
  pv.done();
}

// ------------------- AbelianIsSGFreeAbelian -----------------------//

AbelianIsSGFreeAbelian::AbelianIsSGFreeAbelian(SMSubgroup& g)
  : Supervisor( true ),
    theSubgroup( g ),
    abelianSGInvariants( *this, theSubgroup.scm().abelianSGInvariants )
{
  if(theSubgroup.sic.haveCyclicDecomposition()) resultIsFast();
}

void AbelianIsSGFreeAbelian::takeControl( )
{
  if( theSubgroup.sic.haveCyclicDecomposition() ) {
    LogMessage msg(*this , theSubgroup);
    if(theSubgroup.sic.getSGPresentation().isFree())
      msg <<  Name(theSubgroup) << " is free ableian: ";
    else
      msg <<  Name(theSubgroup) << " is not free ableian: ";
    msg << "this follows from the canonical edcomposition of "
	<< Name(theSubgroup);
    msg.send();
    adminTerminate();
  }
}

void AbelianIsSGFreeAbelian::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Is the subgroup free abelian"),
		  Text("Is") + Name( theSubgroup ) + "free abelian", 
		  helpID("AbelianIsSGFreeAbelian", theSubgroup.getParent()), 
		  "free" );
  pv.startItemGroup();
  pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
	 abelianSGInvariants.arcSlotID(), 50);
  pv.done();
}

// ------------------- AbelianIsSGHyperbolic -----------------------//

AbelianIsSGHyperbolic::AbelianIsSGHyperbolic(SMSubgroup& g)
  : Supervisor( true ),
    theSubgroup( g ),
    abelianSGInvariants( *this, theSubgroup.scm().abelianSGInvariants )
{
  if(theSubgroup.sic.haveCyclicDecomposition()) resultIsFast();
}

void AbelianIsSGHyperbolic::takeControl( )
{
  if( theSubgroup.sic.haveCyclicDecomposition() ) { 
    LogMessage msg(*this, theSubgroup);
    if(theSubgroup.sic.getSGPresentation().rankOfFreeAbelianFactor()<2)
      msg << Name(theSubgroup) << " is hyperbolic.";
    else
      msg << Name(theSubgroup) << " is not hyperbolic.";
    msg.send();
    adminTerminate();
  }
}

void AbelianIsSGHyperbolic::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Is the subgroup hyperbolic"),
		  Text("Is") + Name( theSubgroup ) + "hyperbolic", 
		  helpID("AbelianIsSGHyperbolic", theSubgroup.getParent()), 
		  "hyperbolic");
  pv.startItemGroup();
  pv.add(Text("Compute abelian invariants of") + Name( theSubgroup ),
	 abelianSGInvariants.arcSlotID(), 50);
  pv.done();
}

//-------------- AbelianComputeTorsionSubgroupOfSG --------------------//

AbelianComputeTorsionSubgroupOfSG::AbelianComputeTorsionSubgroupOfSG(SMSubgroup& g)
  : Supervisor( true ),
    theSubgroup( g ),
    abelianSGInvariants( *this, theSubgroup.scm().abelianSGInvariants )
{
  if(theSubgroup.sic.haveCyclicDecomposition()) resultIsFast();
}

void AbelianComputeTorsionSubgroupOfSG::takeControl( )
{
  if( theSubgroup.sic.haveCyclicDecomposition() ) {
    LogMessage msg( *this, theSubgroup );
    const AbelianSGPresentation& A = theSubgroup.sic.getSGPresentation();
    VectorOf<AbelianWord> cGens = A.newToOldGens();
    if(cGens.length()==A.rankOfFreeAbelianFactor()) {
      msg << Name(theSubgroup) << " does not have any non trivial torsion subgroup.";
      msg.send();
    }
    else {
      VectorOf<Word> tGens(cGens.length()-A.rankOfFreeAbelianFactor());
      for (int i=0; i<tGens.length(); i++)
	tGens[i] = A.fromSGPGensToSGGens(cGens[i+A.rankOfFreeAbelianFactor()].getWord());
      SGofFreeGroup tmpSG(theSubgroup.getParent().getFreePreimage(), tGens);
      msg << "Torsion subgroup of " << Name(theSubgroup ) << " is computed.";
      msg.send();
      SMObject* smo = new SMSubgroup(theSubgroup.getParent(), tmpSG, Text("Torsion subgroup of") + Name(theSubgroup));
      ListOf<OID> dependencies(theSubgroup);
      CheckinMessage( *smo, "", dependencies ).send(); 
    }
    adminTerminate();
  }
}

void AbelianComputeTorsionSubgroupOfSG::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), 
		  Text("Compute the torsion subgroup of the subgroup"),
		  Text("Compute the torsion subgroup of") + Name(theSubgroup), 
		  helpID("AbelianComputeTorsionSubgroupOfSG", 
			 theSubgroup.getParent()), "Tor subgp");
  pv.startItemGroup();
  pv.add( Text("Compute abelian invariants of") + Name( theSubgroup ),
	  abelianSGInvariants.arcSlotID(), 50 );
  pv.done();
}
#line 1  "src/SMMagnusBreakdown.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of class SMMagnusBreakdown
//
// Principal Author: Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//

#include "SMMagnusBreakdown.h"
#include "MagnusBreakdown.h"
#include "OutMessages.h"
#include "OneRelatorProblems.h"

static void printListOfGenerators( ostream& ostr, 
				   const ListOf<Generator>& lg, 
				   const FPGroup& G )
{
  ostr << "{ ";
  bool comma = false;
  for ( ListIterator< ListOf<Generator> > I = lg; !I.done(); I.next() ) {
    if( comma ) ostr << ", ";
    G.printWord( ostr, I.value() );
    comma = true;
  }
  ostr << " }";
}

SMMagnusBreakdown::SMMagnusBreakdown( const SMFPGroup& smo )
  : theGroup( smo )
{
  FPGroup FP = theGroup.getFPGroup();

  OneRelatorGroup G( FP.namesOfGenerators(), theGroup.gic.getOneRelator() );
  MagnusBreakdown M(G); 

  InvokingMessage message;

  message <<
    " defineMagnusBreakdown "
    " title {Define a generator(s)} "
    " descr { "
      "Specify a stable letter or two generators (separated by comma) with"
      " non-zero exponent sum or use the default button."
    "} "
    " zero ";
  printListOfGenerators(message, M.getGeneratorsWithZeroExpSum(), FP );
  message << " nonzero ";
  printListOfGenerators(message, M.getGeneratorsWithNonzeroExpSum(), FP );
  message << " default ";
  printListOfGenerators(message, M.getDefaultBreakdownGenerators(), FP );
  message << " message { " << oid().unwrap()  << ' ' << ACCEPT << " } ";
  message.send();
}

void SMMagnusBreakdown::readMessage( istream& istr )
{
  TagMessage tag;

  istr >> (int&)tag;

  if ( tag == ACCEPT ) {

    FPGroup FP = theGroup.getFPGroup();

    Chars err;
    VectorOf<Word> generators = FP.readVectorOfWords( istr, err );

    if ( err.length() > 0 ) {
      // ParseErrorMessage( name, err ).send();    
      Message message("Warning");
      message << "You entered incorrect generators.";
      message.send();
    }
    else {

      int numOfGens = generators.length();    
      if( numOfGens > 2 ) {
	Message message("Warning");
	message << "2 generators should be specified at most.";
	message.send();
      }
      else if( numOfGens == 0 ) {

	// register new object. 

	ORFindHNNPresentation *smo = new ORFindHNNPresentation( theGroup );

	SMObject *group  = (SMObject *)&theGroup;
	group->addDependent( *smo );
      }
      else {

	for( int i = 0; i < numOfGens; ++i ) {
	  if( generators[i].length() != 1 ) {
	    Message message("Warning");
	    message << "You should specify only generators, not words.";
	    message.send();
	    return;
	  }
	}

	Generator stableGen, accompGen;
	
	stableGen = abs(ord(generators[0][0]));
	if( numOfGens == 2 ) accompGen = abs(ord(generators[1][0]));
	
	OneRelatorGroup G( FP.namesOfGenerators(), theGroup.gic.getOneRelator() );
	MagnusBreakdown M(G); 
	SetOf<Generator> gensWithZeroExpSum = makeSetOf( M.getGeneratorsWithZeroExpSum() );
	SetOf<Generator> gensWithNonzeroExpSum = makeSetOf( M.getGeneratorsWithNonzeroExpSum() );
	
	if( numOfGens == 1 && !gensWithZeroExpSum.contains(stableGen)
	    && FP.numberOfGenerators() > 1 ) {
	  Message message("Warning");
	  message << "You should specify two generators with non-zero exponent sum.";
	  message.send();
	}
	else if( ( M.getGeneratorsWithZeroExpSum().contains( stableGen ) 
		   || M.getGeneratorsWithZeroExpSum().contains( accompGen ) )
		 && numOfGens == 2 ) {
	  Message message("Warning");
	  message << "You should specify only 1 generator with zero exponent sum.";
	  message.send();
	}
	else if( numOfGens == 2 && stableGen == accompGen ) {
	  Message message("Warning");
	  message << "You should specify different generators.";
	  message.send();
	}
	else {
	  // register new object. 
	  
	  ORFindHNNPresentation *smo;
	  
	  if( numOfGens == 1 ) 
	    smo = new ORFindHNNPresentation( theGroup, stableGen );
	  else
	    smo = new ORFindHNNPresentation( theGroup, stableGen, accompGen );
	  
	  // theGroup.addDependent( *smo );
	  SMObject *group  = (SMObject *)&theGroup;
	  group->addDependent( *smo );
	}
      }
    }
    
    // delete myself.
    TheObjects::remove( *this );
  }
}
#line 1  "src/OneRelatorProblems.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of one-relator problems.
//
// Principal Author: 
//
// Status: in progress
//
// Revision History:
//

#include "OutMessages.h"
#include "OneRelatorProblems.h"
#include "MagnusBreakdown.h"
#include "SMHomomorphism.h"
#include "SMWord.h"
#include "SMSubgroup.h"
#include "viewStructure.h"

FPGroup makePresentationOf( const OneRelatorGroup& G )
{
  return FPGroup( G.namesOfGenerators(), G.relator() );
}

void ORFindHNNPresentation::takeControl( )
{

  SMFPGroup *theGroup1 = (SMFPGroup *)&theGroup;
  SMFPGroup *group = theGroup1;

  FPGroup FP = theGroup.getFPGroup();
  Word relator =  theGroup.gic.getOneRelator();
  VectorOf<Chars> oldGeneratorsNames = FP.namesOfGenerators();

  OneRelatorGroup G( oldGeneratorsNames, relator );  
  ListOf<OID> dependencies;

  // Step I.. Simplify the one-relator presentation.

  // Step 1a. Seach for cyclically reduced relator of the group.

  if( relator.cyclicallyReduce().length() != relator.length() ) {
    relator = relator.cyclicallyReduce();
    LogMessage msg( *this, theGroup );
    msg << "Made cyclically reduced relator ";
    FP.printWord(msg, relator );
    msg << " of the group " << Name(theGroup) << ".";
    msg.send();
  }
  
  // Step 1b. Search for free part of group: 

  SetOf<Generator> relatorGens;
  for( int i = relator.length()-1; i >= 0; --i )
    relatorGens |= Generator( abs(ord(relator[i])) );
  if( numOfGens > 0 ) {
    // user can specify a generator not occured in the relator.
    relatorGens |= stableGen; 
  }
  
  int oldGen = FP.numberOfGenerators();
  int newGen =  relatorGens.cardinality();
  int freeGen = oldGen - newGen;

  if( freeGen > 0 ) {
    VectorOf<Word> imagesOldToNew( oldGen );
    VectorOf<Word> imagesNewToOld( newGen );
    VectorOf<Chars> newGeneratorsNames( newGen );
    VectorOf<Chars> freeGeneratorsNames( freeGen );
    for( int i = oldGen-1, f = freeGen, n = newGen ; i >= 0; --i ) {
      if( !relatorGens.contains( Generator(i+1) ) ) 
	freeGeneratorsNames[ --f ] = oldGeneratorsNames[i];
      else {
	imagesOldToNew[ i ] = Word( Generator(n) );
	imagesNewToOld[ --n ] = Word( Generator(i+1) );
	newGeneratorsNames[ n ]  = oldGeneratorsNames[i];
      }
    }

    // form factors of free product.
    
    FreeGroup F(freeGeneratorsNames);
    relator = relator.replaceGenerators( imagesOldToNew );
    G = OneRelatorGroup( newGeneratorsNames, relator );
    if( numOfGens > 0 )
      stableGen = imagesOldToNew[ ord(stableGen)-1 ][0];
    if( numOfGens > 1 ) 
      accompGen = imagesOldToNew[ ord(accompGen)-1 ][0]; 

    // Checkin non-free part of one-relator group.
    
    SMFPGroup *theGroup2 = 
      new SMFPGroup( makePresentationOf(G),
		     Text("The group of non-free part of one-relator group ") 
		     + Name( theGroup ),
		     SMFPGroup::ONE_RELATOR );

    CheckinMessage( *theGroup2, "" /*"G"*/, ListOf<OID>() ).send();

    group = theGroup2;

    // Make mappings between G1 and G2:
    FPGroup Gfp = makePresentationOf( G ); 
    Map mapOldToNew(FP, Gfp, imagesOldToNew);
    Map mapNewToOld( Gfp, FP, imagesNewToOld);

    dependencies = ListOf<OID>( *theGroup1 );
    dependencies.append( *theGroup2 );

    SMHomomorphism2 *smmap1 = 
      new SMHomomorphism2( *theGroup1, *theGroup2, mapOldToNew, 
			   Text("The restricting map of group ") 
			   + Name( *theGroup1 ) + " onto the group "
			   + Name( *theGroup2 ) 
			   );
    CheckinMessage( *smmap1, "", dependencies ).send();

    SMHomomorphism2 *smmap2 = 
      new SMHomomorphism2( *theGroup2, *theGroup1, mapNewToOld, 
			   Text("The embedding map of the group ") 
			   + Name( *theGroup2 ) + " into the group "
			   + Name( *theGroup1 ) 
			   );
    CheckinMessage( *smmap2, "", dependencies ).send();
  }

  // Step 2. Make a Magnus breakdown on given generators.

  MagnusBreakdown M(G);
  bool found = false;

  if( numOfGens == 0 || G.numberOfGenerators() == 1 ) {
    found = M.findHNNPresentation( );

    if( !found ) {
      int order = relator.length();
      
      if( order > 0 && FP.raiseToPower( Word(relator[0]), order ) != relator )
	error("Interal error: bad order of group "
	      "in ORFindHNNPresentation::takeControl");
      
      LogMessage msg( *this, *group );
      msg << "Magnus breakdown completed. The one-relator group " 
	  << Name(*group) << " is ";
      switch ( order ) {
      case 0:
	msg << "a cyclic infinite group" ;
	break;
      case 1:
	msg << "a trivial group";
	break;
      default: 
	msg << "a cyclic group of order " << order;
      }
      msg << '.';
      msg.send( );
      return;
    }
    
    stableGen = M.stableGenerator();
    numOfGens = 1;
    if( M.getExponentSumOf( stableGen ) != 0 ) {
      ++numOfGens;
      accompGen = M.accompGenerator();
    }
  }
  else if( numOfGens == 1 )
    found = M.findHNNPresentation( stableGen );
  else
    found = M.findHNNPresentation( stableGen, accompGen );

  // Put HNN presentation and its base group  into workspace.
   
  if( !found ) 
    error("Internal error in ORFindHNNPresentation::takeControl()");

  HNNExtOfORGroup H = M.getHNNPresentation();
    
  if( numOfGens == 2 ) {
    FPGroup FP2( M.getORGroup().namesOfGenerators(), 
		 M.embeddingOfORGroups().imageOf( M.getORGroup().relator() ) 
		 );

    // Make embedding.

    ostrstream heritage;
    heritage << "Embed " << Name(theGroup) 
	     << " in the group obtained by adjoining to " << Name(theGroup)
	     << " a "
	     << M.getExponentSumOf(accompGen) << "-th root of ";
    FP2.printWord( heritage, stableGen );
    heritage << " and set ";
    FP2.printWord( heritage, accompGen );
    heritage << " = ";
    FP2.printWord( heritage, 
	  M.embeddingOfORGroups().generatingImages( abs(ord(accompGen))-1 )
    );
    heritage << " " << ends;
	       
    SMFPGroup *theGroup3 = 
      new SMFPGroup( FP2, heritage.str(), SMFPGroup::ONE_RELATOR);
      
    CheckinMessage( *theGroup3, "", ListOf<OID>() ).send();

    dependencies = ListOf<OID>( *group );
    dependencies.append( *theGroup3 );

    SMHomomorphism2 *embedding = 
      new SMHomomorphism2( *group, *theGroup3, 
			   M.embeddingOfORGroups(),
			   Text(Name( *group )) + "is embedded in" +
			   Name( *theGroup3 ) + "via the mapping"
			   );
    CheckinMessage( *embedding, "", dependencies ).send();

    group = theGroup3;
  }

  // Check in HNN presentation and corresponding mappings.
    
  FPGroup OFP = group->getFPGroup();
  FPGroup HFP = H.getPresentation();

  ostrstream heritage;
  heritage << Name( *group ) << " can be expressed as an HNN presentation"
	   << " with stable letter t = ";
  OFP.printWord( heritage, stableGen );
/*@db 
  if( numOfGens == 2 ) {
    heritage << " and accompanying generator ";
    OFP.printWord( heritage, accompGen );
  }
*/
  heritage << ends;

  SMFPGroup *HNN = new SMFPGroup( HFP, heritage.str() );      
    
  CheckinMessage( *HNN, "" /*"H"*/, ListOf<OID>() ).send();

  // Forms  isomorphic mappings between an one-relator group and 
  // its HNN-presentation.

  dependencies = ListOf<OID>( *group );
  dependencies.append( *HNN );

  SMHomomorphism2 *ORG2HNN = 
    new SMHomomorphism2( *group, *HNN, M.toHNNPresentation(), 
			 Text("The isomorphism from") + Name( *group )
			 + "into" + Name( *HNN )
			 + Text("is defined by the mapping")
			 );
  CheckinMessage( *ORG2HNN, "", dependencies).send();


  SMHomomorphism2 *HNN2ORG = 
    new SMHomomorphism2( *HNN, *group, M.toORGroup(),
			 Text("The inverse isomorphism from") + Name( *HNN )
			 +"to" + Name( *group ) + Text("is defined by")
			 );
  CheckinMessage( *HNN2ORG, "", dependencies).send();


  // Check in an basis group of HNN-presentation and corresponding mappings:

  FPGroup BFP = makePresentationOf( H.basisGroup() );
    
  ostrstream heritage2;
  heritage2 << "The base group of the HNN extension " << Name( *HNN )
	    << " has presentation" << ends;
/*@db
  OFP.printWord( heritage2, stableGen );
  if( numOfGens == 2 ) {
    heritage2 << " and accompanying generator ";
    OFP.printWord( heritage2, accompGen );
  }
  heritage2 << " and presentation" << ends;
*/        
  SMFPGroup *basisGroup = 
    new SMFPGroup( BFP, heritage2.str(), SMFPGroup::ONE_RELATOR );

  CheckinMessage( *basisGroup, "" /*"B"*/, ListOf<OID>() ).send();
    
  dependencies = ListOf<OID>( *HNN );
  dependencies.append( *basisGroup );

  int numberOfNewGenerators = M.numberOfNewGenerators();
  VectorOf<Word> images( numberOfNewGenerators );
  for( int i = 0; i < numberOfNewGenerators-1 ; ++i )
    images[i] = Generator(i+1);
  Map hnn2basis( HFP, BFP, images );
 
/*@db
  SMHomomorphism2 *HNN2BASIS = 
    new SMHomomorphism2( *HNN, *basisGroup, hnn2basis, 
			 Text("The isomorphism from") + Name( *group )
			 + Text(", thought of a subgroup of") + Name( *HNN )
			 + ", to" + Name( *basisGroup )
			 + Text("is defined by")
			 );
  CheckinMessage( *HNN2BASIS, "", dependencies ).send();
*/    

  images.shrink( numberOfNewGenerators - 1 );
  Map basis2hnn( BFP, HFP, images );

  SMHomomorphism2 *BASIS2HNN = 
    new SMHomomorphism2( *basisGroup, *HNN, basis2hnn, 
			 Text("The monomorphism from")
			 + Name( *basisGroup )
			 + "into" + Name( *HNN )
			 + Text("is defined by")
			 );
  CheckinMessage( *BASIS2HNN, "", dependencies ).send();
}    




// ========================================================================= //


// ------------------------ ORWordProblemARCer ----------------------------- //


ORExtendedWordProblemARCer::ORExtendedWordProblemARCer( ComputationManager& boss )
  : ARCer( boss ), answer(dontknow)
{ } 


void ORExtendedWordProblemARCer::setArguments( const Word& relator,
					       const Word& testWord,
					       const VectorOf<Word>& target )
{
  theRelator  = relator;
  theTestWord = testWord;
  theTarget   = target;
}

void ORExtendedWordProblemARCer::runComputation( ) 
{
  Generator g1 = theRelator.maxOccurringGenerator();
  Generator g2 = theTestWord.maxOccurringGenerator();
  int numOfGens = max( abs(ord(g1)), abs(ord(g2)) );

  for(int i = 0; i < theTarget.length(); ++i ) {
    Word gen = theTarget[i];
    numOfGens = max( numOfGens, ord(gen.maxOccurringGenerator()) );
  }

  OneRelatorGroup G( numOfGens, theRelator );
  SubgroupOfOneRelatorGroup S( G, theTarget );
  
  answer = S.contains( theTestWord );
  if( answer == yes ) 
    theWordInBasis = S.rewriteFromGroupToSubgroupGenerators( theTestWord );
}

void  ORExtendedWordProblemARCer::writeResults( ostream& out )
{
  out < answer;
  if( answer == yes ) 
    out < ' ' < theWordInBasis;
}


void ORExtendedWordProblemARCer::readResults( istream& in )
{
  in > answer;
  if( answer == yes ) {
    retValue = true;
    in > theWordInBasis;
  }
  else if( answer == no )
    retValue = false;
  else 
    error( "Internal error in OneRelatorGroup::wordProblem() " );
}


// ----------------------- ORExtendedWordProblemCM ------------------------ //


ORExtendedWordProblemCM::ORExtendedWordProblemCM( ORExtendedWordProblem& PO )
: theRelator( PO.getTestWord().getParent().gic.getOneRelator() ),
  theTestWord( PO.getTestWord().getWord() ),
  theTarget( PO.getTarget().getSubgroup().generators() ),
  itIsExpressed( dontknow ),
  theWordInBasis( ),
  bStarted( false ),
  arcer( *this )
{
  adminStart();
}

void ORExtendedWordProblemCM::terminate( )
{
  arcer.terminate();
}

void ORExtendedWordProblemCM::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if( !bStarted ) {
      arcer.setArguments( theRelator, theTestWord, theTarget );
      bStarted = true;
    }
    if( arcer.takeControl() ) {
      itIsExpressed = arcer.getRetValue();
      theWordInBasis = arcer.wordInBasis();
      return;
    }
    else
      usedOneARC();
  }
}


// -------------------------- ORExtendedWordProblem ------------------------ //


ORExtendedWordProblem::ORExtendedWordProblem( SMSubgroup& target, SMWord& w )
  : Supervisor( true ),
    theWord( w ),
    theTarget( target ),
    theAnswer( dontknow ),
    theWordInBasis( ),
    orwp( *this )
{
  OneRelatorGroup G( w.getParent().getFPGroup().namesOfGenerators(), 
		     w.getParent().gic.getOneRelator() );
  SubgroupOfOneRelatorGroup S( G, target.getSubgroup().generators() );
  if( !S.isMagnusSubgroup() ) {
    resultIsFast();
    theAnswer = no;
    Message msg("Warning");
    msg << "The given subgroup is not a Magnus one.";
    msg.send();
  }
}

void ORExtendedWordProblem::takeControl( )
{
  if ( ( theAnswer = orwp->isExpressed() ) != dontknow ) {
    
    LogMessage msg( *this, theTarget, theWord );
    msg << Name( theWord ) << " is ";
    if ( theAnswer == no ) msg << "not ";
    msg << "expressed in generators of " << Name( theTarget );
    
    if( theAnswer == yes ) {
      theWordInBasis =  orwp->wordInBasis();
      msg << " : " << Name(theWord) << " = ";
      theTarget.getSubgroup().printWord( msg, theWordInBasis );
    }
    msg << ".";
    msg.send();
    adminTerminate();
  }
}


void ORExtendedWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Extended word problem"),
		  Text("Is") + Name( theWord ) + "expressed in basis of"
		  + Name( theTarget ) + "?",
		  helpID( "ExtendedWordProblem", theWord.getParent() ), 
		  Text(Name(theWord)) + " in " + Text(Name(theTarget)) + "?"
		  );
  

  pv.startItemGroup();
  
  pv.add(Text("For the use of this problem"), orwp.arcSlotID(), 50);

  pv.done();
}


void ORIsMagnusSubgroup::takeControl( )
{
  OneRelatorGroup G( theSubgroup.getParent().getFPGroup().namesOfGenerators(), 
		     theSubgroup.getParent().gic.getOneRelator() );
  SubgroupOfOneRelatorGroup S( G, theSubgroup.getSubgroup().generators() );

  bool answer = S.isMagnusSubgroup();

  LogMessage msg( theSubgroup );
  msg << Name(theSubgroup) << " is ";
  if( !answer )
    msg << "not ";
  msg << "a Magnus subgroup.";
  msg.send();  
}
#line 1  "src/HNNProblems.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents:  
// 
//
// Principal Author: Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//


#include "HNNProblems.h"
#include "SMSubgroup.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "FEData.h"
#include "viewStructure.h"
#include "HNNExtOfFreeGroup.h"


void MakeHNNExtOfFreeGroup::takeControl( )
{
  SGofFreeGroup S1 = subgroup1.getSubgroup( );
  SGofFreeGroup S2 = subgroup2.getSubgroup( );

  if( S1.generators().length() != S2.generators().length() ) {
    Message msg;
    msg << "The number of generators of the subgroups has to be equal.";
    msg.send();
    return;
  }
  
  // @dp The given name of stable letter must be checked.
  //     The letter must not belong to the free base group.

  FreeGroup F = subgroup1.getParent().getFPGroup().namesOfGenerators();
  HNNExtOfFreeGroup H(F, theStableLetter, S2, S1);

  // try to avoid HNN at all.
  //  SetOf<Word> relators;
  //  relators |= S1.generators()[0];
  //  relators |= S2.generators()[0];
  //  FPGroup G( 2, relators );

  FPGroup realH = H.getFPGroup();
  
  if( realH.getRelators().cardinality() == 1 )
    {
      SMFPGroup* smo = new SMFPGroup
	( realH, Text("User defined HNN-extension < ") + Text(theStableLetter) + "," + Name(Parent(subgroup1)) + "; " + Text(theStableLetter) + Text("^-1") + Name(subgroup2) + Text(theStableLetter) + " =" + Name(subgroup1) + ">" +
Text("which is actually is a one relator group"),
	  SMFPGroup::ONE_RELATOR
	  );
  
      SetIterator<Word> I(realH.getRelators());
      Word w;
      while( !I.done() )
	{
	  w = I.value();
	  I.next();
	}
      
      smo->gic.putIsOneRelator(w);

      ListOf<OID> dependencies;
      CheckinMessage( *smo, "", dependencies ).send();
    
      return;
    }
  
  SMFPGroup* smo = new SMFPGroup
    ( H.getFPGroup(),
      Text("User defined HNN-extension < ") + Text(theStableLetter) + ","
      + Name(Parent(subgroup1)) + "; "
      + Text(theStableLetter) + Text("^-1") + Name(subgroup2) + Text(theStableLetter) 
      + " =" + Name(subgroup1) + ">",
      SMFPGroup::HNN_FREE
      );
  
  smo->gic.putHaveHNNofFree(H);

  ListOf<OID> dependencies;
  CheckinMessage( *smo, "", dependencies ).send();
}


void HNNofFreeGroup_IsTrivial::takeControl( )
{
  bool answer = ( group.gic.getHNNofFree().isTrivial() == yes );
  
  LogMessage msg( *this, group );
  msg << Name( group ) << " is";
  if( !answer )
    msg << " not";
  msg << " trivial.";

  msg.send();
}

void HNNofFreeGroup_IsFinite::takeControl( )
{
  bool answer = ( group.gic.getHNNofFree().isFinite() == yes );
  
  LogMessage msg( *this, group );
  msg << Name( group ) << " is";
  if( !answer )
    msg << " not";
  msg << " finite.";

  msg.send();
}


void HNNofFreeGroup_IsAbelian::takeControl( )
{
  bool answer = ( group.gic.getHNNofFree().isAbelian() == yes );
  
  LogMessage msg( *this, group );
  msg << Name( group ) << " is";
  if( !answer )
    msg << " not";
  msg << " abelian.";

  msg.send();
}



void HNNofFreeGroup_ConjugacyProblem::takeControl( )
{
  const HNNExtOfFreeGroup& H = word1.getParent().gic.getHNNofFree();

  Word conjugator;

  Trichotomy answer = H.conjugacyProblem( word1.getWord(), word2.getWord(), conjugator );

  LogMessage msg( word1, word2 );

  if( answer == dontknow ) { 
    msg << "Magnus cannot determine whether " << Name( word1 ) << " and "
	<< Name( word2 ) << " are conjugate or not";
  }
  else {
    msg << Name( word1 ) << " and " << Name( word2 ) << " are";

    if( answer == no )
      msg << " not conjugate";
    else {
      msg << " conjugate: X " << Name( word1 ) << " x = "
	  << Name( word2 ) << ", where x = ";
      word1.getParent().getFPGroup().printWord( msg, conjugator );
    }
  }
  
  msg << ".";
  msg.send();
}

void HNNofFreeGroup_WordProblem::takeControl( )
{
  const HNNExtOfFreeGroup& H = word.getParent().gic.getHNNofFree();

  Trichotomy answer = H.wordProblem( word.getWord() );

  LogMessage msg( word );

  if( answer == dontknow ) { 
    msg << "Magnus cannot determine whether " << Name( word ) << " is trivial or not.";
  }
  else {
    msg << Name( word ) << " is";
    if( answer == no )
      msg << " not";
    // @am changed wording
    msg << " trivial: follows from the normal form.";
  }
  
  msg.send();
}



void HNNofFreeGroup_ReducedForm::takeControl( )
{

  const HNNExtOfFreeGroup& H = word.getParent().gic.getHNNofFree();

  Word reduced = H.reducedFormOf( word.getWord() );

  SMObject* smo = new SMWord( word.getParent(), reduced, 
			      Text("The reduced form of") + Name( word )
			      ); 
  ListOf<OID> dependencies( word.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void HNNofFreeGroup_NormalForm::takeControl( )
{

  const HNNExtOfFreeGroup& H = word.getParent().gic.getHNNofFree();

  Word normal = H.normalFormOf( word.getWord() );

  SMObject* smo = new SMWord( word.getParent(), normal, 
			      Text("The normal form of") + Name( word )
			      ); 
  ListOf<OID> dependencies( word.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}


void HNNofFreeGroup_CyclicallyReducedForm::takeControl( )
{

  const HNNExtOfFreeGroup& H = word.getParent().gic.getHNNofFree();

  Word conjugator;
  Word cycreduced = H.cyclicallyReducedFormOf( word.getWord(), conjugator );

  SMObject* smo = new SMWord( word.getParent(), cycreduced, 
			      Text("The cyclically reduced form of") + Name( word )
			      ); 
  ListOf<OID> dependencies( word.getParent() );
  CheckinMessage( *smo, "", dependencies ).send();
}

void HNNofFreeGroup_IsProperPower::takeControl( )
{
  const HNNExtOfFreeGroup& H = word.getParent().gic.getHNNofFree();

  Word root;
  int power;

  Trichotomy answer = H.maximalRoot( word.getWord(), root, power );

  LogMessage msg( word );

  if( answer == dontknow ) { 
    msg << "Magnus cannot determine whether " << Name( word ) 
      // @am 'a' added 
	<< " is a proper power or not.";
  }
  else {
    if( power > 1 ) {
      msg << Name( word ) << " is a proper power. " << Name( word ) 
	  << " is equal ";
      H.printWord( msg, root );
      msg << " in power " << power;
    }
    else {
      msg << Name( word ) << " is not a proper power";
      if( power == 0 ) 
	msg << ": maximal root does not exist";
    }    
    msg << ".";
  }
  
  msg.send();
}

void HNNofFreeGroup_MaximalRoot::takeControl( )
{
  const HNNExtOfFreeGroup& H = word.getParent().gic.getHNNofFree();

  Word root;
  int power;
  Trichotomy answer = H.maximalRoot( word.getWord(), root, power );

  if( answer == dontknow ) { 
    LogMessage msg( word );
    msg << "Magnus cannot extract the maximal root of " << Name( word ) << ".";
    msg.send();
  }
  else {

    if( power == 0 ) {

      LogMessage msg( word );
      msg << "The maximal root of " << Name( word ) << " does not exist.";
      msg.send();
    }
    else if( power == 1 ) {

      LogMessage msg( word );
      msg << "The maximal root of " << Name( word ) << " is " 
	  << Name( word ) << ".";
      msg.send();
    }
    else {
      
      SMObject* smo =
	new SMWord( word.getParent(), root,
		    Text("The maximal root of") + Name( word )
		    );
      
      ListOf<OID> dependencies( word.getParent() );
      CheckinMessage( *smo, "", dependencies ).send();
    }
  }

}

void HNNofFreeGroup_AreEqual::takeControl( )
{
  const HNNExtOfFreeGroup& H = word1.getParent().gic.getHNNofFree();

  Trichotomy answer = H.areEqual( word1.getWord(), word2.getWord() );

  LogMessage msg( word1, word2 );

  if( answer == dontknow ) { 
    msg << "Magnus cannot determine whether " << Name( word1 ) << " is equal to "
	<< Name( word2 ) << " or not";
;
  }
  else {
    msg << Name( word1 ) << " is";
    if( answer == no )
      msg << " not";
    msg << " equal to " << Name( word2 );
  }

  msg << ".";
  msg.send();
}

void HNNofFreeGroup_IsProperPowerOfSecond::takeControl( )
{
  const HNNExtOfFreeGroup& H = word1.getParent().gic.getHNNofFree();

  LogMessage msg( word1, word2 );

  msg << Name( word1 ) << " is";
  int power = 0;
  bool answer = H.isProperPowerOfSecond( word1.getWord(), word2.getWord(), power );
  if( !answer )
    msg << " not";
  msg << " a proper power of " << Name( word2 );
  if( answer ) 
    msg << " ( " << Name(word2) << "^" << power << " = " << Name(word1) << " )";
  msg << ".";
  msg.send();
}


void HNNofFreeGroup_IsSGTrivial::takeControl( )
{
  const HNNExtOfFreeGroup& H = subgroup.getParent().gic.getHNNofFree();
  
  bool answer = H.isSubgroupTrivial( subgroup.getSubgroup().generators() );

  LogMessage msg( subgroup );

  msg << Name( subgroup ) << " is";
  if( !answer ) 
    msg << " not";
  msg << " trivial.";

  msg.send();
}



void HNNofFreeGroup_IsSGAbelian::takeControl( )
{
  const HNNExtOfFreeGroup& H = subgroup.getParent().gic.getHNNofFree();
  

  bool answer = H.isSubgroupAbelian( subgroup.getSubgroup().generators() );

  LogMessage msg( subgroup );

  msg << Name( subgroup ) << " is";
  if( !answer ) 
    msg << " not";
  msg << " abelian in " << Name( subgroup.getParent() ) << ".";
  
  msg.send();
}

void CheckinHNNofFreeGroup::takeControl( )
{
  Message msg;
  msg << 
    "Check-in the base group and two of its subgroups, which must "
    "have the same number of generators. The generators of the two "
    "subgroups must be arranged so that the mapping which sends the n-th "
    "generator of the first subgroup to the n-th generator of the second "
    "subgroup defines an isomorphism between them. Then go to the Make "
    "menu. (Note: HNN-extensions are, as of now, only implemented "
    "when the base group is checked in as free.)";
  msg.send();
}



// -------------------- Freeness of HNN-extension ---------------------- //

/*
HNNofFreeIsFreeArcer::~HNNofFreeIsFreeArcer( ) 
{ 
  delete theGroup;
}

void HNNofFreeIsFreeArcer::setArguments( const HNNExtOfFreeGroup& H )
{
  theGroup = new HNNExtOfFreeGroup(H);
}

void HNNofFreeIsFreeArcer::runComputation( )
{
  theAnswer = theGroup->isFree();
}
 
void HNNofFreeIsFreeArcer::writeResults( ostream& ostr )
{
  ostr < theAnswer;
}

void HNNofFreeIsFreeArcer::readResults( istream& istr )
{
  istr > theAnswer;
}
*/

HNNofFreeIsFree::HNNofFreeIsFree( class SMFPGroup& F )
  : ComputationManager( true ), 
    theGroup( F ),
    theORProblems( 0 ),
    arcer( *this )
{
  if( !theGroup.gic.isOneRelator() ||
      theGroup.gic.isOneRelatorWithTorsion() )
    {
      resultIsFast();
      return;
    }
}

HNNofFreeIsFree::~HNNofFreeIsFree( )
{
  delete theORProblems;
  theORProblems = NULL;
}

void HNNofFreeIsFree::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is an HNN-extension of free group free"),
		  Text("Is an HNN-extension ") + Name( theGroup ) 
		  + Text(" of free group free"),
		  helpID("HNNofFreeFreeness", theGroup),
		  "Freeness"
		  );

  pv.startItemGroup();

  pv.add(Text("For use of this problem"), 
	 ThisARCSlotID(), 
	 50
	 );

  pv.done();
}

void HNNofFreeIsFree::takeControl( )
{
  if( fastResult() )
    {
      LogMessage msg( *this, theGroup );
      
      if( !theGroup.gic.isOneRelator() )
	msg << "There is no algorithm known at present which determines whether or not an HNN extension of free group is free.";
      else
	msg << Name( theGroup ) << " is one relator group with torsion, so it can't be free.";
      
      msg.send();
      adminTerminate();
      return;
    }
  
  if( freeARCs() > 0 ) {
    if( theORProblems == 0 ) 
      {
	theORProblems = 
	  new ORProblems( theGroup.getFPGroup().numberOfGenerators(),
			  theGroup.gic.getOneRelator() );
      
	arcer.setArguments(theORProblems);
      }
    else 
      if( arcer.takeControl() ) {
	
        bool answer = arcer.getRetValue();
	LogMessage msg( *this, theGroup );
	
	msg << Name( theGroup ) << " is ";
	
	if( !answer )
	  msg << "not ";
	
	msg << "free.";
	msg.send();
 	
	delete theORProblems;
	theORProblems = NULL;
	
	adminTerminate();
	return;
      }
      else
	usedOneARC();
  }
}


// ------- Freeness of an amalgamated product of free groups  -------- //

void APofFreeIsFreeArcer::setArguments( const AmalgProductOfFreeGroups& G )
{
  theGroup = new AmalgProductOfFreeGroups(G);
}

void APofFreeIsFreeArcer::runComputation( )
{
  theAnswer = theGroup->isFree();
}
 
void APofFreeIsFreeArcer::writeResults( ostream& ostr )
{
  ostr < theAnswer;
}

void APofFreeIsFreeArcer::readResults( istream& istr )
{
  istr > theAnswer;
}


APofFreeIsFree::APofFreeIsFree( class SMFPGroup& F )
  : ComputationManager( true ), arcer( *this ), theGroup( F )
{
  arcer.setArguments( theGroup.gic.getAPOfFree() );
}

void APofFreeIsFree::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is an amalgamated product of free groups free"),
		  Text("Is an amalgamated product ") + Name( theGroup ) 
		  + Text(" of free groups free"),
		  helpID("APofFreeFreeness", theGroup),
		  "Freeness"
		  );

  pv.startItemGroup();

  pv.add(Text("For use of this problem"), ThisARCSlotID(), 50);

  pv.done();
  
}

void APofFreeIsFree::takeControl( )
{
  if( freeARCs() > 0 ) {

    if( arcer.takeControl() ) {

      Trichotomy answer = arcer.answer();
      LogMessage msg( *this, theGroup );
      if( answer == dontknow ) {
	msg << "Magnus cannot determine whether " << Name(theGroup) 
	    << " is free or not.";
      }
      else {
	theGroup.gic.putIsFree( answer );
	msg << Name( theGroup ) << " is";
	if( answer == no ) 
	  msg << " not";
	msg << " free.";
      }
      msg.send();
      adminTerminate();
      return;
    }

    usedOneARC();
  }
}

// ------- Is an amalgamated product of free groups perfect -------- //

APofFreeIsPerfect::APofFreeIsPerfect( class SMFPGroup& F )
  : Supervisor( true ),  theGroup( F ), 
    abelianInvariants( *this, F.gcm().abelianInvariants )
{
  if( theGroup.gic.haveCyclicDecomposition() ) // || theGroup.gic.isTrivial() ) 
    resultIsFast();
}

void APofFreeIsPerfect::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is an amalgamated product of free groups perfect"),
		  Text("Is an amalgamated product ") + Name( theGroup ) 
		  + Text(" of free groups perfect"),
		  helpID("APofFreePerfect", theGroup),
		  "Perfect"
		  );

  pv.startItemGroup();

  // pv.add("For use of this problem", ThisARCSlotID(), 50);
  pv.add( Text("Compute canonical decomposition of") + Name( theGroup ),
	  abelianInvariants.arcSlotID(),
	  50
	  );

  pv.done();
  
}

void APofFreeIsPerfect::takeControl( )
{
  if( theGroup.gic.haveCyclicDecomposition() ) {
    AbelianGroup A = theGroup.gic.getCyclicDecomposition();
    Trichotomy answer = A.isTrivial();
    
    LogMessage msg( *this, theGroup );
    msg << Name( theGroup ) << " is";
    if( answer == no ) 
      msg << " not";
    msg << " perfect.";
    msg.send();
    
    adminTerminate();
    return;
  }
}


// ------- Homology of an amalgamated product of free groups  -------- //

void APofFreeHomologyArcer::setArguments( const AmalgProductOfFreeGroups& G ) 
{
  theGroup = new AmalgProductOfFreeGroups(G);
}

void APofFreeHomologyArcer::runComputation( )
{
  int r = theGroup->factor( LEFT_FACTOR ).rank();
  int s = theGroup->factor( RIGHT_FACTOR ).rank();
  int m = theGroup->subgroup( LEFT_FACTOR ).generators().length();
  VectorOf<Word> relators( m );
  for( int i = 0; i < m; ++i ) {
    relators[i] = theGroup->subgroup( LEFT_FACTOR ).generators()[i] 
                  * theGroup->subgroup( RIGHT_FACTOR ).generators()[i].inverse();
  }
  AbelianGroup F( r+s );
  F.computeCyclicDecomposition();
  AbelianGroup H( F.makeSubgroupPresentation(relators) );
  H.computeCyclicDecomposition();
  theRankOfH2 = H.rankOfFreeAbelianFactor();
}

void APofFreeHomologyArcer::writeResults( ostream& ostr )
{
  ostr < theRankOfH2;
}

void APofFreeHomologyArcer::readResults( istream& istr )
{
  istr > theRankOfH2;
}


APofFreeHomologyProblem::APofFreeHomologyProblem( class SMFPGroup& F )
  : Supervisor( true ), theGroup( F ), cyclicSubgroup(false),
    arcer( *this ), arcerStarted( false ),
    abelianInvariants( *this, F.gcm().abelianInvariants ), abDone( false )
{
  cyclicSubgroup = ( theGroup.gic.getAPOfFree().subgroup(LEFT_FACTOR)
		     .generators().length() <= 1 
		     );
  if( theGroup.gic.haveCyclicDecomposition() ) {
    abDone = true;
    if( cyclicSubgroup ) 
      resultIsFast();
  }  

  adminStart();
}

void APofFreeHomologyProblem::takeControl( )
{
  if( freeARCs() > 0 ) {

    if( theGroup.gic.haveCyclicDecomposition() ) {

      int H2 = -1;

      if( cyclicSubgroup ) {
	const AmalgProductOfFreeGroups& G = theGroup.gic.getAPOfFree();
	AbelianWord rel( G.numberOfGenerators(), 
			 G.subgroup( LEFT_FACTOR ).generators()[0]
			 );
	H2 = ( rel.isTrivial() ? 0 : 1 );
      }
      else {

	if( !arcerStarted ) {
	  arcer.setArguments( theGroup.gic.getAPOfFree() );
	  arcerStarted = true;
	}
	if( arcer.takeControl() ) {
	  H2 = arcer.rankOfH2();
	}
      }
      
      if( H2 != -1 ) {

	const AbelianGroup& A = theGroup.gic.getCyclicDecomposition();
	LogMessage msg( *this, theGroup );
	msg << "Homology of the group " << Name(theGroup) << ": "
	    << " H_1(" << Name(theGroup) << ") = " << A
	    << ", H_2(" << Name(theGroup) << ") = ";
	switch( H2 ) {
	case 0:  msg << '0'; break;
	case 1:  msg << 'Z'; break;
	default: msg << "Z^" << H2;
	}
	msg << ",  H_n(" << Name(theGroup) << ") = 0, n>2.";
	msg.send();
	adminTerminate();
	return;
      }

      usedOneARC();
    }
  }

}


void APofFreeHomologyProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Integral homology of an amalgamated product of free groups"),
		  Text("Compute integral homology of the amalgamated product ") 
		  + Name( theGroup ) + Text(" of free groups"),
		  helpID("APofFreeHomology", theGroup),
		  "Homology"
		  );

  pv.startItemGroup();

  pv.add( Text("For use of this problem"), ThisARCSlotID(), 50 );

  if( !abDone )
    pv.add( Text("Compute abelian invariants of") + Name( theGroup ),
	    abelianInvariants.arcSlotID(), 50
	    );


  pv.done();
  
}


#line 1  "src/NGSubgroupProblems.C"

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes
//
// Principal Author: Eugene Paderin
//
// Status: in progress
//
// Revision History:
//

#include "SMSubgroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "SGOfNilpotentGroup.h"
#include "NGSubgroupProblems.h"
#include "conversions.h"
// ----------------- SGOfNGinitPreimageARCer ------------------------ //


void SGOfNGinitPreimageARCer::runComputation( )
{
  theSNG->initPreimage(  );
}


void SGOfNGinitPreimageARCer::setArguments( const NilpotentGroup& NG ,
						   const VectorOf<Word>& gens)
{
  theSNG = new SGOfNilpotentGroup( NG, gens );
}


void SGOfNGinitPreimageARCer::writeResults( ostream& out ) 
{
  out < *theSNG;
}

void SGOfNGinitPreimageARCer::readResults( istream& in )
{
  in > *theSNG;
}
const SGOfNilpotentGroup& SGOfNGinitPreimageARCer::getSubgroup(  ) const
{
#if SAFETY > 0
  if( theSNG == 0 )
    error( "const SGOfNilpotentGroup& SGOfNGinitPreimageARCer::getSubgroup(  )"
	   " const : Subgroup was not initialized");
#endif
  return *theSNG;
}

// ------------------- SGOfNGinitPreimageProblem --------------------- //


SGOfNGinitPreimageProblem::SGOfNGinitPreimageProblem(SCM& scm)
  : theSCM( scm ),   
    theGIC(scm.getSubgroup().getParent().gic),
    arcer( *this )
{
  if (scm.getSubgroup().sic.haveSGOfNGPreimageInitialized())
    resultIsFast();
  if (theGIC.haveNilpotentGroupInited()){
    arcer.setArguments( theGIC.getNilpotentGroupInited() ,
			theSCM.getSubgroup().getSubgroup().generators());
  }else{
    VectorOf<Word> rel(theSCM.getSubgroup().getParent().
		       getFPGroup().getRelators().cardinality());
    SetIterator<Word> I(theSCM.getSubgroup().getParent().
			getFPGroup().getRelators());
    int k = 0;
    for( I.reset(); !I.done(); I.next() ){
      rel[k] = I.value();
      k++;
    }
    NilpotentGroup ng(theSCM.getSubgroup().getParent().
		      getFPGroup().namesOfGenerators(),
		      theGIC.getNilpotentcyClass(),rel);
    arcer.setArguments( ng,theSCM.getSubgroup().getSubgroup().generators());    
  }
  adminStart();
}


void SGOfNGinitPreimageProblem::takeControl( )
{
  if (freeARCs()>0){
    if (fastResult()){
      adminTerminate();
      return;
    }
    if(arcer.takeControl()){
      theSCM.getSubgroup().sic.
	putHaveSGOfNGPreimageInitialized(arcer.getSubgroup());
      adminTerminate();
      return;
    }
    usedOneARC();
  }
}

// ----------------- SGOfNGinitializeARCer ------------------------ //


void SGOfNGinitializeARCer::runComputation( )
{
  theSNG->initBasis(  );
}


void SGOfNGinitializeARCer::setArguments( const NilpotentGroup& NG ,
						   const VectorOf<Word>& gens)
{
  theSNG = new SGOfNilpotentGroup( NG, gens );
}


void SGOfNGinitializeARCer::writeResults( ostream& out ) 
{
  out < *theSNG;
}

void SGOfNGinitializeARCer::readResults( istream& in )
{
  in > *theSNG;
}
const SGOfNilpotentGroup& SGOfNGinitializeARCer::getSubgroup(  ) const
{
#if SAFETY > 0
  if( theSNG == 0 )
    error( "const SGOfNilpotentGroup& SGOfNGinitializeARCer::getSubgroup(  )"
	   " const : Subgroup was not initialized");
#endif
  return *theSNG;
}

// ------------------- SGOfNGinitializeProblem --------------------- //


SGOfNGinitializeProblem::SGOfNGinitializeProblem(SCM& scm)
  : theSCM( scm ),   
    theGIC(scm.getSubgroup().getParent().gic),
    haveParentInited(false),
    arcer( *this )
{
  if (scm.getSubgroup().sic.haveSGOfNGinitialized())
    resultIsFast();
  if (theGIC.haveNilpotentGroupInited()){
    haveParentInited = true;
    arcer.setArguments( theGIC.getNilpotentGroupInited() ,
			theSCM.getSubgroup().getSubgroup().generators());
  }else{
    VectorOf<Word> rel(theSCM.getSubgroup().getParent().
		       getFPGroup().getRelators().cardinality());
    SetIterator<Word> I(theSCM.getSubgroup().getParent().
			getFPGroup().getRelators());
    int k = 0;
    for( I.reset(); !I.done(); I.next() ){
      rel[k] = I.value();
      k++;
    }
    NilpotentGroup ng(theSCM.getSubgroup().getParent().
		      getFPGroup().namesOfGenerators(),
		      theGIC.getNilpotentcyClass(),rel);
    arcer.setArguments( ng,theSCM.getSubgroup().getSubgroup().generators());    
  }
  adminStart();
}


void SGOfNGinitializeProblem::takeControl( )
{
  if (freeARCs()>0){
    if (fastResult()){
      adminTerminate();
      return;
    }
    if(arcer.takeControl()){
      theSCM.getSubgroup().sic.putHaveSGOfNGinitialized(arcer.getSubgroup());
      if (!haveParentInited)
	theGIC.putHaveNilpotentGroupInited(arcer.getSubgroup().parentGroup(),
					  Chars());
      adminTerminate();
      return;
    }
    usedOneARC();
  }
}
//--------------------------------------------------------------------//
//-------------------- SGOfNGcomputeBasisProblem ---------------------//
//--------------------------------------------------------------------//


SGOfNGcomputeBasisProblem::SGOfNGcomputeBasisProblem(SMSubgroup& sg)
  : Supervisor( true ),
    theSubgroup( sg ),
    initializeSGOfNG( *this , sg.scm().initializeSGOfNG),
    computeBasis(*this,sg.getParent().gcm().computeBasis )
{
  if( theSubgroup.sic.haveSGOfNGinitialized() ) resultIsFast();
}


void SGOfNGcomputeBasisProblem::takeControl( )
{
  if ( theSubgroup.sic.haveSGOfNGinitialized()){
    File file;
    LogMessage msg( *this, theSubgroup);
    theSubgroup.sic.getSGOfNilpotentGroup().printBasis(file);
    file << end;
    Chars basisMsg;
    //    if (theSubgroup.getParent().gic.isFreeNilpotent() == yes)
    //      basisMsg = "Click here to view the Malcev basis for ";
    //else
      basisMsg = "Click here to view the basis for ";
     
    msg << Link( Text(basisMsg) + Name(theSubgroup) + ".",
		 "NGSubgroupBasis",
		 file.getFileName());
    msg.send();
    adminTerminate();
    return;
  }
}
void SGOfNGcomputeBasisProblem::start( ) { }


void SGOfNGcomputeBasisProblem::terminate( ) { }

void SGOfNGcomputeBasisProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute a basis"),
		  Text("Compute a basis for") + Name(theSubgroup),
		  helpID("SGOfNGcomputeBasisProblem", 
			 theSubgroup.getParent()),
		  "basis"
		  );
  pv.startItemGroup();
  if (!theSubgroup.getParent().gic.haveNilpotentGroupInited())
    pv.add(Text("Compute a basis of ")
	  + Name(theSubgroup.getParent()),
	   computeBasis.arcSlotID(), 100);
  pv.add(Text("For use of this problem"),initializeSGOfNG.arcSlotID(), 100);
  pv.done();
}

// ------------------- SGOfNGDecomposeWordARCer --------------------- //


void SGOfNGDecomposeWordARCer::runComputation( )
{
  isContain = theSNG->decompose(theWord,theDecomposition);
}


void SGOfNGDecomposeWordARCer::setArguments( const SGOfNilpotentGroup& SNG,
					     const PolyWord& w )
{
  theSNG = new SGOfNilpotentGroup(SNG);
  theWord = w;
}


void SGOfNGDecomposeWordARCer::writeResults( ostream& out ) 
{
  out < isContain < theDecomposition ;
}

void SGOfNGDecomposeWordARCer::readResults( istream& in )
{
  in > isContain > theDecomposition ;
}

// ---------------------- SGOfNGDecomposeWordProblem ---------------------- //


SGOfNGDecomposeWordProblem::SGOfNGDecomposeWordProblem(SMSubgroup& sg,const SMWord& word)
  : Supervisor( true ),
    theSubgroup( sg ),
    theWord( word ),
    theSIC( sg.sic ),
    started(false),
    arcer(*this),
    initializeSGOfNG( *this, sg.scm().initializeSGOfNG ),
    computeBasis( *this, sg.getParent().gcm().computeBasis ),
    ngDecomposeWord( *this, word.wcm().ngDecomposeWord )
{ 

}


void SGOfNGDecomposeWordProblem::takeControl( )
{
  if (freeARCs() > 0){
    if (theSIC.haveSGOfNGPreimageInitialized() 
	&& theWord.wic.haveCollectedForm()){
      if (!started){
	arcer.setArguments(theSIC.getSGOfNilpotentGroup(),
			   theWord.wic.getCollectedForm());
	started = true;
      }
      if (arcer.takeControl()){
	LogMessage msg( *this,theWord );
	if (arcer.contains()){
	  msg << "Decomposition of " << Name( theWord ) << 
	    " in terms of basis: " << 
	    theSIC.getSGOfNilpotentGroup().
	    asDecomposition(arcer.decomposition());
	}
	else {
	  msg << Name ( theWord ) << " is not in " << Name (theSubgroup);
	}
	
	msg.send();
	adminTerminate();
	return;
      }
    }
    usedOneARC();
  }
}


void SGOfNGDecomposeWordProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Decompose word in terms of a basis"),
		  Text("Decompose ") + Name( theWord ) + 
		  Text("in terms of a basis of ")+
		  Name(theSubgroup),
		  helpID("SGOfNGDecomposeWordProblem", theWord.getParent()),
		  Text("decomp")
		  );
  
  pv.startItemGroup();

  if (!theWord.getParent().gic.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of ")
	    + Name( theWord.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    );
  
  if (!theWord.wic.haveCollectedForm())
    pv.add( Text("Decompose ")
	    + Name( theWord ) + " in " + Name (theWord.getParent()),
	    ngDecomposeWord.arcSlotID(),
	    100
	    );

  if (!theSIC.haveSGOfNGinitialized())
    pv.add( Text("Compute a basis of ")
	    + Name( theSubgroup ),
	    initializeSGOfNG.arcSlotID(),
	    100
	    );  
  
  pv.add( Text("For the use of this problem"), ThisARCSlotID(), 50 );
  pv.done();
}
// ------------------- SGOfNGWordContainARCer --------------------- //


void SGOfNGWordContainARCer::runComputation( )
{
  SetIterator<Word> I( theTestWords );

  for( I.reset(); !I.done(); I.next() ) {
    if( !theSubgroup->contains(I.value()) ) {
      theAnswer = false;
      return;
    }
  }

  theAnswer = true;
}


void SGOfNGWordContainARCer::setArguments( const SGOfNilpotentGroup& subgroup,
					   const SetOf<Word>& testWords )
{
  theSubgroup = new SGOfNilpotentGroup(subgroup);
  theTestWords = testWords;
}


void SGOfNGWordContainARCer::writeResults( ostream& out ) 
{
  out < theAnswer;
}

void SGOfNGWordContainARCer::readResults( istream& in )
{
  in > theAnswer;
}

SGOfNGWordContain::SGOfNGWordContain( SGOfNGWordContainProblem& boss )
  : 
    theSubgroup( boss.getSubgroup() ),
    theTestWords( boss.getTestWord().getWord() ),
    arcer(*this), arcerStarted(false),
    theAnswer( dontknow )
{ 
  adminStart();
}

Trichotomy SGOfNGWordContain::answer( ) const 
{ 
  return theAnswer; 
}

void SGOfNGWordContain::takeControl( )
{
  if( freeARCs() > 0 ) {

    if( theSubgroup.sic.haveSGOfNGPreimageInitialized() ) {

      if( !arcerStarted ) {
	arcer.setArguments( theSubgroup.sic.getSGOfNilpotentGroup(),
			    theTestWords );
	arcerStarted = true;
      }

      if( arcer.takeControl() ) {
	theAnswer = arcer.answer();
	adminTerminate();
	return;
      }

    }
    usedOneARC();
  }
}

// ---------------------- SGOfNGWordContainProblem ----------------------- //


SGOfNGWordContainProblem::SGOfNGWordContainProblem(SMSubgroup& sg,const SMWord& word)
  : Supervisor( true ),
    theSubgroup( sg ),
    theTestWord( word ),
    initPreimageSGOfNG( *this, sg.scm().initPreimageSGOfNG ),
    sgContain( *this )
{ 
}


void SGOfNGWordContainProblem::takeControl( )
{
  if( sgContain->state() == ComputationManager::TERMINATED ) {

    bool answer = sgContain->answer();

    LogMessage msg( *this, theTestWord );
    msg << Name( theTestWord ) << ( answer ? " is " : " is not " )
	<< " in " << Name( theSubgroup )<< ".";
    msg.send();

    adminTerminate();
  }
}


void SGOfNGWordContainProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Containment problem"),
		  Text("Determine whether") + Name(theTestWord)
		    + Text("lies in") + Name(theSubgroup),
		  helpID("SGOfNGWordContainProblem", 
			 theSubgroup.getParent()),
		  Text(Name(theTestWord)) + Text("is in")
		    + Name(theSubgroup) + Text("?")
		  );
  
  pv.startItemGroup();

  pv.add( Text("For the use of this problem"), 
	  sgContain.arcSlotID(), 
	  50 
	  );

  if( !theSubgroup.sic.haveSGOfNGPreimageInitialized() ) {
    pv.add( Text("Compute a basis of the free preimage of ")+Name(theSubgroup),
	    initPreimageSGOfNG.arcSlotID(),
	    100
	    );  
  }

  pv.done();
}

// ----------------- SGOfNGcontainSubgroupARCer ------------------------ //


void SGOfNGcontainSubgroupARCer::runComputation( )
{
   answer = theSubgroup1->contains( theSubgroup2 );
}


void SGOfNGcontainSubgroupARCer::setArguments( const SGOfNilpotentGroup& SNG1 ,
						const VectorOf<Word>& SNG2)
{
  theSubgroup1 = new SGOfNilpotentGroup( SNG1 );
  theSubgroup2 =  SNG2;
}


void SGOfNGcontainSubgroupARCer::writeResults( ostream& out ) 
{
  out < answer;
}

void SGOfNGcontainSubgroupARCer::readResults( istream& in )
{
  in > answer;
}
bool SGOfNGcontainSubgroupARCer::getAnswer(  ) const
{
#if SAFETY > 0
  if( theSubgroup1 == 0 )
    error( "bool SGOfNGcontainSubgroupARCer::getAnswer(  ) const :"
	   "Subgroups was not initializad");
#endif
  return answer;
}

//---------------------------------------------------------------------------//
//-------------------- SGOfNGcontainSubgroupProblem ------------------------//
//---------------------------------------------------------------------------//

SGOfNGcontainSubgroupProblem::SGOfNGcontainSubgroupProblem(SMSubgroup& sg1,const SMSubgroup& sg2)
  : Supervisor( true ),
    theSubgroup1( sg1 ),
    theSubgroup2( sg2 ),
    initPreimageSGOfNG( *this , sg1.scm().initPreimageSGOfNG),
    started(false),
    arcer(*this)
{
  
}

void SGOfNGcontainSubgroupProblem::takeControl( )
{
  if (freeARCs()>0){
    
    if ( theSubgroup1.sic.haveSGOfNGPreimageInitialized()){
      if (!started){
	arcer.setArguments( theSubgroup1.sic.getSGOfNilpotentGroup(),
			    theSubgroup2.getSubgroup().generators());
	started = true;
      }
      if (arcer.takeControl()){
	LogMessage msg( *this, theSubgroup1,theSubgroup2 );
	msg << "The subgroup " << Name( theSubgroup1 );
	
	if( arcer.getAnswer() ) msg << " contains ";
	else msg << " does not contain ";
	
	msg << Name( theSubgroup2 ) << ".";
	msg.send();

	adminTerminate();
	return;
      }
    }
    usedOneARC();
  }
}
void SGOfNGcontainSubgroupProblem::start( ) { }


void SGOfNGcontainSubgroupProblem::terminate( ) { }

void SGOfNGcontainSubgroupProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Subgroup containment problem"),
		  Text("Determine whether") + Name(theSubgroup1 )
		  + Text("contains") + Name( theSubgroup2 ),
		  helpID("SGOfNGcontainSubgroupProblem", 
			 theSubgroup1.getParent()),
		  Text(Name(theSubgroup2)) + Text("is in") +
		  Name(theSubgroup1) + Text("?")
		  );
  
  
  pv.startItemGroup();
  pv.add( Text("Does") + Name(theSubgroup1 )
	  + Text("contain") + Name( theSubgroup2 ), ThisARCSlotID(), 100 );
  
  if( ! theSubgroup1.sic.haveSGOfNGPreimageInitialized() )
    pv.add( Text("Compute a basis of the free preimage of ")+ 
	    Name(theSubgroup1 ),
	    initPreimageSGOfNG.arcSlotID(), 100 );
  
  pv.done();
}
//---------------------------------------------------------------------------//
//-------------------- SGOfNGequalSubgroupProblem ------------------------//
//---------------------------------------------------------------------------//

SGOfNGequalSubgroupProblem::SGOfNGequalSubgroupProblem(SMSubgroup& sg1, SMSubgroup& sg2)
  : Supervisor( true ),
    theSubgroup1( sg1 ),
    theSubgroup2( sg2 ),
    initPreimageSGOfNG1( *this , sg1.scm().initPreimageSGOfNG),
    started1(false),
    arcer1(*this),
    initPreimageSGOfNG2( *this , sg2.scm().initPreimageSGOfNG),
    started2(false),
    arcer2(*this)
{
  
}


void SGOfNGequalSubgroupProblem::takeControl( )
{
  if (freeARCs()>0){
    Trichotomy s1conts2 = dontknow;
    Trichotomy s2conts1 = dontknow;

    if ( theSubgroup1.sic.haveSGOfNGPreimageInitialized()){
      if (!started1){
	arcer1.setArguments( theSubgroup1.sic.getSGOfNilpotentGroup(),
			    theSubgroup2.getSubgroup().generators());
	started1 = true;
      }
      if (s1conts2 == dontknow)
	if (arcer1.takeControl()){
	  s1conts2 = arcer1.getAnswer();
	}
    }
    if ( theSubgroup2.sic.haveSGOfNGPreimageInitialized()){
      if (!started2){
	arcer2.setArguments( theSubgroup2.sic.getSGOfNilpotentGroup(),
			    theSubgroup1.getSubgroup().generators());
	started2 = true;
      }
      if (s2conts1 == dontknow)
	if (arcer2.takeControl()){
	  s2conts1 = arcer2.getAnswer();
	}
    }	
    Trichotomy areEqual = dontknow;
    if (s1conts2 == no || s2conts1 == no)
      areEqual = no;
    if (s1conts2 == yes && s2conts1 == yes)
      areEqual = yes;
    if (areEqual != dontknow){
      LogMessage msg( *this, theSubgroup1, theSubgroup2 );
      msg << "The subgroup " << Name( theSubgroup1 ) << " is ";
      if( areEqual == no ) msg << "not ";
      msg << "equal to " << Name( theSubgroup2 ) << ".";
      msg.send();
      
      adminTerminate();
      return;
    }
    
    usedOneARC();
  }
}
void SGOfNGequalSubgroupProblem::start( ) { }


void SGOfNGequalSubgroupProblem::terminate( ) { }

void SGOfNGequalSubgroupProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Subgroup equality problem"),
		  Text("Determine whether") + Name(theSubgroup1 )
		  + Text("is equal to") + Name( theSubgroup2 ),
		  helpID("SGOfNGequalSubgroupProblem", 
			 theSubgroup1.getParent()),
		  Text(Name(theSubgroup1)) + Text("=") +
		  Name(theSubgroup2) + Text("?")
		  );
  
  pv.startItemGroup();

  pv.add( Text("Does") + Name(theSubgroup1 )
	  + Text("equal to") + Name( theSubgroup2 ), ThisARCSlotID(), 100 );
  
  if( ! theSubgroup1.sic.haveSGOfNGPreimageInitialized() )
    pv.add( Text("Compute a basis of the free preimage of ")+ 
	    Name(theSubgroup1 ),
	    initPreimageSGOfNG1.arcSlotID(), 100 );
  
  if( ! theSubgroup2.sic.haveSGOfNGPreimageInitialized() )
    pv.add( Text("Compute a basis of the free preimage of ")+ 
	    Name(theSubgroup2 ),
	    initPreimageSGOfNG2.arcSlotID(), 100 );  
  pv.done();
}
// ----------------- SGOfNGindexARCer ------------------------ //


void SGOfNGindexARCer::runComputation( )
{
  index = theSNG->index(  );
}


void SGOfNGindexARCer::setArguments(const SGOfNilpotentGroup& sfng)
{
  theSNG = new SGOfNilpotentGroup( sfng );
}


void SGOfNGindexARCer::writeResults( ostream& out ) 
{
  out < index;
}

void SGOfNGindexARCer::readResults( istream& in )
{
  in > index;
}
int SGOfNGindexARCer::getIndex(  ) const
{
#if SAFETY > 0
  if( theSNG == 0 )
    error( "int SGOfNGindexARCer::getIndex(  ) const :"
	   "Subgroup was not initialized");
#endif
  return index;
}

//---------------------------------------------------------------------------//
//-------------------- SGOfNGindexProblem ----------------------------------//
//---------------------------------------------------------------------------//


SGOfNGindexProblem::SGOfNGindexProblem(SMSubgroup& sg)
  : Supervisor( true ),
    theSubgroup( sg ),
    initPreimageSGOfNG( *this , sg.scm().initPreimageSGOfNG),
    started(false),
    arcer( *this )
{
  if (theSubgroup.sic.haveIndex()) resultIsFast();
}


void SGOfNGindexProblem::takeControl( )
{
  
  if (!fastResult())
    if (freeARCs()>0){
      if ( theSubgroup.sic.haveSGOfNGPreimageInitialized()){
	if (!started){
	  arcer.setArguments( theSubgroup.sic.getSGOfNilpotentGroup());
	  started = true;
	}
	if (arcer.takeControl())
	  theSubgroup.sic.putIndex(arcer.getIndex());
      }
      usedOneARC();
    }
  if (theSubgroup.sic.haveIndex()){
    LogMessage msg( *this, theSubgroup );
    if (!theSubgroup.sic.index())
      msg << "The index of " << Name(theSubgroup) << " in " << 
	Name(theSubgroup.getParent())<<" is infinite.";
    else
      msg << "The index of " <<  Name(theSubgroup ) << " in " <<
	Name(theSubgroup.getParent()) << " is " << theSubgroup.sic.index();
    msg.send();
    adminTerminate();    
  }
}
void SGOfNGindexProblem::start( ) { }


void SGOfNGindexProblem::terminate( ) { }

void SGOfNGindexProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Index of subgroup"),
		  Text("Compute the index of") + Name(theSubgroup) +
		  Text(" in ")+Name(theSubgroup.getParent()),
		  helpID("SGOfNGindexProblem", 
			 theSubgroup.getParent()),
		  "Index"
		  );
  pv.startItemGroup();
  pv.add(Text("Compute the index"),ThisARCSlotID(), 100);
  if( ! theSubgroup.sic.haveSGOfNGPreimageInitialized() )
    pv.add( Text("Compute a basis of the free preimage of ")+ 
	    Name(theSubgroup ),
	    initPreimageSGOfNG.arcSlotID(), 100 );

  pv.done();
}

//---------------------------------------------------------------------------//
//-------------------- SGOfNGhirschNumberProblem ----------------------------//
//---------------------------------------------------------------------------//


SGOfNGhirschNumberProblem::SGOfNGhirschNumberProblem(SMSubgroup& sg)
  : Supervisor( true ),
    theSubgroup( sg ),
    initializeSGOfNG( *this , sg.scm().initializeSGOfNG),
    started(false)
{
  if (theSubgroup.sic.haveSGOfNGinitialized()) resultIsFast();
}


void SGOfNGhirschNumberProblem::takeControl( )
{
  
  if (theSubgroup.sic.haveSGOfNGinitialized()){
    LogMessage msg( *this, theSubgroup );
    msg << Text("The Hirsch number of the subgroup ") << Name(theSubgroup) 
	<<" is "<<theSubgroup.sic.getSGOfNilpotentGroup().theHirschNumber()<<".";
    msg.send();
    adminTerminate();    
  }
}
void SGOfNGhirschNumberProblem::start( ) { }


void SGOfNGhirschNumberProblem::terminate( ) { }

void SGOfNGhirschNumberProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute the Hirsch number"),
		  Text("Compute the Hirsch number of") + Name(theSubgroup),
		  helpID("SGOfNGhirschNumberProblem",theSubgroup.getParent()),
		  Text("Hirsch num.") + Name(theSubgroup )
		  );
  pv.startItemGroup();
 
  if( ! theSubgroup.sic.haveSGOfNGinitialized() )
    pv.add( Text("Compute a basis of ")+ Name(theSubgroup ),
	    initializeSGOfNG.arcSlotID(), 100 );

  pv.done();
}

// ----------------- SGOfNGisNormalARCer ------------------------ //


void SGOfNGisNormalARCer::runComputation( )
{
  theAnswer = theSNG->isNormal();
}


void SGOfNGisNormalARCer::setArguments(const SGOfNilpotentGroup& sfng)
{
  theSNG = new SGOfNilpotentGroup( sfng );
}


void SGOfNGisNormalARCer::writeResults( ostream& out ) 
{
  out < theAnswer;
}

void SGOfNGisNormalARCer::readResults( istream& in )
{
  in > theAnswer;
}
bool SGOfNGisNormalARCer::answer(  ) const
{
#if SAFETY > 0
  if( theSNG == 0 )
    error( "int SGOfNGindexARCer::getIndex(  ) const :"
	   "Subgroup was not initialized");
#endif
  return theAnswer;
}

//---------------------------------------------------------------------------//
//-------------------- SGOfNGisNormalProblem --------------------------------//
//---------------------------------------------------------------------------//


SGOfNGisNormalProblem::SGOfNGisNormalProblem(SMSubgroup& sg)
  : Supervisor( true ),
    theSubgroup( sg ),
    initPreimageSGOfNG(*this,sg.scm().initPreimageSGOfNG ),
    started(false),
    arcer( *this )
{
  if (theSubgroup.sic.isNormal()!=dontknow) resultIsFast();
}


void SGOfNGisNormalProblem::takeControl( )
{
  
  if (!fastResult())
    if (freeARCs()>0){
      if ( theSubgroup.sic.haveSGOfNGPreimageInitialized ()){
	if (!started){
	  arcer.setArguments( theSubgroup.sic.getSGOfNilpotentGroup());
	  started = true;
	}
	if (arcer.takeControl())
	  theSubgroup.sic.putIsNormal(arcer.answer());
      }
      usedOneARC();
    }
  if (theSubgroup.sic.isNormal()!=dontknow){
    LogMessage msg( *this, theSubgroup );
    msg << Text( Name(theSubgroup) ) + "is ";
    if(!theSubgroup.sic.isNormal()) msg << "not ";
    msg << "normal in " << Name( theSubgroup.getParent() ) << ".";
    msg.send();
    adminTerminate();    
  }
}
void SGOfNGisNormalProblem::start( ) { }


void SGOfNGisNormalProblem::terminate( ) { }

void SGOfNGisNormalProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is") + Name(theSubgroup) 
		  + "normal in" + Name( Parent(theSubgroup) ) + "?",
		  Text("Is") + Name(theSubgroup) 
		  + "normal in" + Name( Parent(theSubgroup) ) + "?",
		  helpID("SGOfNGisNormalProblem", 
			 theSubgroup.getParent()),
		  "Is normal?"
		  );
  
  pv.startItemGroup();
  pv.add( Text("Compute is normal"), ThisARCSlotID(), 100 );

  if( ! theSubgroup.sic.haveSGOfNGPreimageInitialized () )
    pv.add( Text("Compute a basis of the free preimage of ")+ 
	    Name( theSubgroup ), 
	    initPreimageSGOfNG.arcSlotID(), 100 );

  pv.done();
}
// ----------------- SGOfNGcomputeNClassARCer ------------------------ //


void SGOfNGcomputeNClassARCer::runComputation( )
{
  theClass = theSNG->subgroupClass(  );
}


void SGOfNGcomputeNClassARCer::setArguments( const NilpotentGroup& NG ,
						   const VectorOf<Word>& gens)
{
  theSNG = new SGOfNilpotentGroup( NG, gens );
}


void SGOfNGcomputeNClassARCer::writeResults( ostream& out ) 
{
  out < theClass;
}

void SGOfNGcomputeNClassARCer::readResults( istream& in )
{
  in > theClass;
}
int SGOfNGcomputeNClassARCer::getClass(  ) const
{
#if SAFETY > 0
  if( theSNG == 0 )
    error( "int SGOfNGcomputeNClassARCer::class(  ) const :"
	   "Subgroup was not initializad");
#endif
  return theClass;
}
// ---------------------- SGOfNGcomputeNClassProblem ----------------------- //


SGOfNGcomputeNClassProblem::SGOfNGcomputeNClassProblem(SMSubgroup& sg)
  : Supervisor( true ),
    theSubgroup( sg ),
    theGIC( sg.getParent().gic ),
    computeBasis( *this, sg.getParent().gcm().computeBasis ),
    arcer(*this),
    started(false)
{ 
}


void SGOfNGcomputeNClassProblem::takeControl( )
{
  if (freeARCs() > 0){
    if (theGIC.haveNilpotentGroupInited()){
      if (!started){
	arcer.setArguments(theGIC.getNilpotentGroupInited(),
			   theSubgroup.getSubgroup().generators());
	started = true;
      }
      //arcer.runComputation();
      if (arcer.takeControl()){
	LogMessage msg( *this, theSubgroup);
	msg << Text("The class of the subgroup ") << Name(theSubgroup)  << " is " 
	    << arcer.getClass() << ".";
	msg.send();
	adminTerminate();
	return;
      }
    }
    usedOneARC();
  }
}


void SGOfNGcomputeNClassProblem::viewStructure(ostream& ostr) const
{
 ProblemView pv( ostr, oid(), Text("Compute the class"),
		  Text("Compute the class of")
		  + Name( theSubgroup ),
		  helpID("SGOfNGcomputeNClassProblem",theSubgroup.getParent()),
		  Text("class")
		  );
  
  pv.startItemGroup();

  if (!theGIC.haveNilpotentGroupInited())
    pv.add( Text("Compute a basis of")
	    + Name( theSubgroup.getParent() ),
	    computeBasis.arcSlotID(),
	    100
	    );
  
  pv.add( Text("Compute the class of")
	  + Name( theSubgroup ),
	  ThisARCSlotID(),
	  100
	  );
  pv.done();
}
// ----------------- SGOfNGPresentationARCer ------------------------ //

void SGOfNGPresentationARCer::runComputation( )
{
  *thePresentation = theSubgroup->makePresentation( );  
}


void SGOfNGPresentationARCer::
setArguments(const SGOfNilpotentGroup& presentation )
{
   theSubgroup = new SGOfNilpotentGroup(presentation);
   thePresentation = new PresentationForSNG(presentation.preimageBasis(), 
					    presentation.parentGroup().relators(), 
					    presentation.parentGroup().collector());
}

const PresentationForSNG& SGOfNGPresentationARCer::getPresentation() const
{
 #if SAFETY > 0
  if( theSubgroup == 0 )
    error( "const PresentationForSNG& SGOfNGPresentationARCer::getPresentation() const:"
	   "You have not initialized the presentation");
#endif
  return *thePresentation;
}

void SGOfNGPresentationARCer::writeResults( ostream& out ) 
{
  out < *thePresentation;
}

void SGOfNGPresentationARCer::readResults( istream& in )
{
  in > *thePresentation;
}

//---------------------------------------------------------------------------//
//-------------------- SGOfNGbuildPresentationProblem --------------------------//
//---------------------------------------------------------------------------//


SGOfNGbuildPresentationProblem::SGOfNGbuildPresentationProblem( SMSubgroup& subgroup)
  : Supervisor( true ),
    theSubgroup( subgroup ),
    arcer( *this),
    started(false),
    initPreimageSGOfNG(*this,subgroup.scm().initPreimageSGOfNG )
{
}


void SGOfNGbuildPresentationProblem::takeControl( )
{
  if (freeARCs() > 0){
    if (theSubgroup.sic.haveSGOfNGPreimageInitialized ()){
      if (!started){
	arcer.setArguments(theSubgroup.sic.getSGOfNilpotentGroup());    
	started = true;
      }
      if (arcer.takeControl() ){
	File file;
	arcer.getPresentation().print(file);
	LogMessage msg( *this, theSubgroup);
	msg << Link(Text("Click here to view the presentation of ") +
		    Name(theSubgroup) + "." ,
		    "FNGStructureConstants", file.getFileName());
	msg.send();
	adminTerminate();
	return;
      }
    }
    usedOneARC();
  }
}  


void SGOfNGbuildPresentationProblem::start( ) { }


void SGOfNGbuildPresentationProblem::terminate( ) { }

void SGOfNGbuildPresentationProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Compute a presentation"),
		  Text("Compute a presentation of")
		  + Name( theSubgroup ),
		  helpID("SGOfNGbuildPresentationProblem", theSubgroup.getParent()),
		  Text("Present. of") + Name( theSubgroup )
		  );

  pv.startItemGroup();


  if( ! theSubgroup.sic.haveSGOfNGPreimageInitialized() )
    pv.add( Text("Compute a basis of the free preimage of ")+ 
	    Name(theSubgroup ),
	    initPreimageSGOfNG.arcSlotID(), 100 );  

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 100);
  pv.done();
}

// ----------------- SGOfNGormalClosureARCer ------------------------ //

void SGOfNGnormalClosureARCer::runComputation( )
{
  nClosure = theSubgroup->normalClosureBasis( );  
}


void SGOfNGnormalClosureARCer::
setArguments(const SGOfNilpotentGroup& presentation )
{
   theSubgroup = new SGOfNilpotentGroup(presentation);
}

const VectorOf<Word>& SGOfNGnormalClosureARCer::normalClosure() const
{
 #if SAFETY > 0
  if( theSubgroup == 0 )
    error( "const VectorOf<Word>& SGOfNGormalClosureARCer::normalClosure() const:"
	   "You have not initialized the presentation");
#endif
  return nClosure;
}

void SGOfNGnormalClosureARCer::writeResults( ostream& out ) 
{
  out < nClosure;
}

void SGOfNGnormalClosureARCer::readResults( istream& in )
{
  in > nClosure;
}

//---------------------------------------------------------------------------//
//-------------------- SGOfNGnormalClosureProblem --------------------------//
//---------------------------------------------------------------------------//


SGOfNGnormalClosureProblem::SGOfNGnormalClosureProblem( SMSubgroup& subgroup)
  : Supervisor( true ),
    theSubgroup( subgroup ),
    started(false),
    initializeSGOfNG(*this,subgroup.scm().initializeSGOfNG ),
    arcer( *this)
{
}


void SGOfNGnormalClosureProblem::takeControl( )
{
  if (freeARCs() > 0){
    if (theSubgroup.sic.haveSGOfNGinitialized() ||
	theSubgroup.getParent().gic.isFreeNilpotent() == yes){
      if (!started){
	if (theSubgroup.sic.haveSGOfNGinitialized())
	  arcer.setArguments(theSubgroup.sic.getSGOfNilpotentGroup());
	else
	  arcer.setArguments
	    (SGOfNilpotentGroup(theSubgroup.getParent().gic.
				getNilpotentGroupInited(),
				theSubgroup.getSubgroup().generators())
	     );
	     
	started = true;
      }
      if (arcer.takeControl() ){
	File file;
	file << "Generators of the subgroup are:" << endl << endl;
	const VectorOf<Word>& gens = arcer.normalClosure();
	for(int i = 0; i < gens.length(); i++) {
	  if (gens[i]!=Word()){
	    theSubgroup.getParent().getFPGroup().printWord( file,gens[i] );
	    file << endl;
	  }
	}
	file << end;
	
	SGofFreeGroup F(theSubgroup.getParent().getFreePreimage(),
			arcer.normalClosure() );
	
	SMObject* smo = 
	  new SMSubgroup( theSubgroup.getParent(), F,
			  Text("The normal closure of") + Name( theSubgroup ),
			  Text("ncl(") + Name( theSubgroup ) +")" 
			  ); 
	
	ListOf<OID> dependencies( theSubgroup.getParent() );
	CheckinMessage( *smo, "", dependencies ).send();
	
	LogMessage msg( *this, *smo);
	msg << Link( Text("Click here to view the generators of ") 
		     + Name(*smo) + ".",
		     "NGNCgenerators",
		     file.getFileName());
	msg.send();
	adminTerminate();
	return;
      }
    }
 
    usedOneARC();
  }
}


void SGOfNGnormalClosureProblem::start( ) { }


void SGOfNGnormalClosureProblem::terminate( ) { }

void SGOfNGnormalClosureProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the normal closure"),
		  Text("Find the normal closure of")
		  + Name( theSubgroup ),
		  helpID("SGOfNGnormalClosureProblem", theSubgroup.getParent()),
		  Text("normal closure of") + Name( theSubgroup )
		  );

  pv.startItemGroup();
  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 100);
  
  pv.startItemGroup( CheckinType( Parent( theSubgroup ) ) == SMFPGroup::NILPOTENT);
  
  if (!theSubgroup.sic.haveSGOfNGinitialized())
    pv.add(Text("Compute a basis of ")
	   + Name(theSubgroup),
	   initializeSGOfNG.arcSlotID(), 100); 
  pv.done();
}
// ----------------- SGOfNGormalClosureGensARCer ------------------------ //

void SGOfNGnormalClosureGensARCer::runComputation( )
{
  nClosure = theSubgroup->normalClosureGens( );  
}


void SGOfNGnormalClosureGensARCer::
setArguments(const SGOfNilpotentGroup& presentation )
{
   theSubgroup = new SGOfNilpotentGroup(presentation);
}

const VectorOf<Word>& SGOfNGnormalClosureGensARCer::normalClosure() const
{
 #if SAFETY > 0
  if( theSubgroup == 0 )
    error( "const VectorOf<Word>& SGOfNGormalClosureGensARCer::normalClosure() const:"
	   "You have not initialized the presentation");
#endif
  return nClosure;
}

void SGOfNGnormalClosureGensARCer::writeResults( ostream& out ) 
{
  out < nClosure;
}

void SGOfNGnormalClosureGensARCer::readResults( istream& in )
{
  in > nClosure;
}

//---------------------------------------------------------------------------//
//-------------------- SGOfNGnormalClosureGensProblem ------------------------//
//---------------------------------------------------------------------------//


SGOfNGnormalClosureGensProblem::SGOfNGnormalClosureGensProblem( SMSubgroup& subgroup)
  : Supervisor( true ),
    theSubgroup( subgroup ),
    arcer( *this)
{
  NilpotentGroup ng(theSubgroup.getParent().
		    getFPGroup().namesOfGenerators(),
		    theSubgroup.getParent().gic.getNilpotentcyClass(),
		    makeVectorOf(theSubgroup.getParent().
				 getFPGroup().getRelators()));
  
  SGOfNilpotentGroup sng( ng,theSubgroup.getSubgroup().generators());  
  arcer.setArguments(sng);
}


void SGOfNGnormalClosureGensProblem::takeControl( )
{
  if (freeARCs() > 0){
    if (arcer.takeControl() ){
      VectorOf<Word> gens = arcer.normalClosure();
      
      File file;
      file << "Generators of the subgroup are:" << endl << endl;
      for(int i = 0; i < gens.length(); i++) {
	if (gens[i]!=Word()){
	  theSubgroup.getParent().getFPGroup().printWord( file,gens[i] );
	  file << endl;
	}
      }
      file << end;
      
      SGofFreeGroup F(theSubgroup.getParent().getFreePreimage(),gens );
      
      SMObject* smo = 
	new SMSubgroup( theSubgroup.getParent(), F,
			Text("The normal closure of") + Name( theSubgroup ),
			Text("ncl(") + Name( theSubgroup ) +")" 
			); 
      
      ListOf<OID> dependencies( theSubgroup.getParent() );
      CheckinMessage( *smo, "", dependencies ).send();
      
      LogMessage msg( *this, *smo);
      msg << Link( Text("Click here to view the generators of ") 
		   + Name(*smo) + ".",
		   "NGNCgenerators",
		   file.getFileName());
      msg.send();
      adminTerminate();
      return;
    }
    
    
    usedOneARC();
  }
}


void SGOfNGnormalClosureGensProblem::start( ) { }


void SGOfNGnormalClosureGensProblem::terminate( ) { }

void SGOfNGnormalClosureGensProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find the normal closure"),
		  Text("Find the normal closure of")
		  + Name( theSubgroup ),
		  helpID("SGOfNGnormalClosureGensProblem", 
			 theSubgroup.getParent()),
		  Text("norm clos")
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 100);

  pv.done();
}
#line 1  "src/TTProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes TTArcer, TTCM, TTProblem.
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "stdio.h"
#include "string.h"
#include "TTProblem.h"
#include "OutMessages.h"
#include "viewStructure.h"


// ------------------------------ TTArcer ---------------------------------- //


Chars TTArcer::getFileName( ) {

#if SAFETY > 0
  if( !ttp )
    error("Chars TTArcer::getFileName( ) : "
	  "The file has not been created.");
#endif
  
  return ttp->getFileName();
}
  

void TTArcer::setArguments( const FPGroup& G )
{
  theGroup = G;

  delete ttp;
  ttp = new TTP(G);
}

void TTArcer::writeResults( ostream& out ) 
{
  out < bHappy;
}


void TTArcer::readResults( istream& in )
{
  in > bHappy;
}


void TTArcer::runComputation( )
{
  if( !ttp->run() )
    bHappy = false;
}


// ----------------------------- KBSupervisor ------------------------------ //


TTCM::TTCM( class TTProblem& PO )
  : theGroup( PO.getGroup() ),
    arcer( *this ),
    inPos( 0 ),
    bInstructions( true )
{
  adminStart();
  in = fopen( arcer.getFileName(), "r" );
}

TTCM::~TTCM( ) 
{
  fclose(in);
}

void TTCM::start( )
{
  arcer.setArguments( theGroup.getFPGroup() );
}


void TTCM::takeControl( )
{
  // infinite process
  if ( freeARCs() > 0 ) {

    arcer.takeControl();
    usedOneARC();
    
    fseek(in,0,SEEK_END);
    long sz;
    if( ( sz = ftell(in)) > inPos ) {
      
      char s[1000];
      fseek(in,inPos,SEEK_SET);
      
      while( fgets(s,1024,in) != 0 ) {

	if( bInstructions ) 
	  if( strchr(s,'#') == 0 ) continue;
	  else 
	    bInstructions = false;
	
	int len = strlen(s);
	for( int i = 0; i < len; ++i )
	  if( s[i] == '#' ) s[i] = 'G';
	
	out << s << ready;
      }
      
      inPos = sz;
    }
  }
}


// -------------------------- TTProblem ----------------------------------- //


TTProblem::TTProblem(class SMFPGroup& G)
  : Supervisor( true ),
    theGroup( G ),
    tt( *this ),
    linkHasBeenSent( false )
{ 
  FPGroup G = theGroup.getFPGroup(); 
  SetOf<Word> S = G.getRelators();
  SetIterator<Word> I(S);
  
  for( int i = 0; i < G.numberOfGenerators(); ++i ) {
    
    for( I.reset(); !I.done(); I.next() )
      if( Word(I.value()).numberOfOccurrences( Generator(i+1) ) != 0 )
	break;
    
    if( I.done() ) {
      
      LogMessage msg( *this );
      msg << "The current version of the Tietze Trasformation Package does"
	  << " not allow us to have a free generator in a group presentation.";
      msg.send();
      resultIsFast();
      return;
    }
  }
  
  ostrstream ostr;
  
  int i;
  for( I.reset(), i = 0; !I.done(); I.next(), ++i ) {
    
    G.printWord(ostr, I.value());
    if( i < S.cardinality() - 1 )
      ostr << ',';
    else
      ostr << '!' << endl;
  }
  
  if( ostr.pcount() > 254 ) {

    LogMessage msg( *this );
    msg << "The current version of the Tietze Trasformation Package does"
	<< " not allow us to have an input line longer than 254 characters.";

    msg.send();
    resultIsFast();
    return;
  }
}


void TTProblem::takeControl( )
{
  if( fastResult() ) {
    adminTerminate();
    return;
  }

  if( !tt->isHappy() ) {
    
    LogMessage msg( *this );
    msg << "The Tietze Trasformation Package is unable to work with"
	<< " your presentation; probably it is to long for it.";
    
    msg.send();
    adminTerminate();
    return;
  }
  
  if( !linkHasBeenSent ) {
    
    linkHasBeenSent = true;
    
    LogMessage msg( *this );
    msg << Link(Chars("Click here to see other presentations for ")
		+ Text(Name( theGroup )), "TTProblem", 
		tt->getFileName(), true);
    msg << ".";
    msg.send();
  }
  
  // infinite process
}


void TTProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Presenations for the group"),
		  Text("Seek other presentations for") 
		  + Name( theGroup ),
		  helpID("TTProblem", theGroup),
		  "Present"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of this problem"), tt.arcSlotID(), 100);

  pv.done();
}
#line 1  "src/ToddCoxeter.C"
/*
 *   $Id:
 */
 
// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class ToddCoxeterARCer ToddCoxeter,
//           SGIndexToddCoxeter, SchreierTransversal, PermutationRepProblem,
//           WordRepresentativeProblem, WordRepresentativeProblem 

//
// Principal Author: Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//


#include "ToddCoxeter.h"
#include "CommutatorIterator.h"
#include "OutMessages.h"
#include "GCM.h"
#include "SMFPGroup.h"
#include "SMSubgroup.h"
#include "viewStructure.h"
#include "SubgroupContainmentProblem.h"


// ------------------- ToddCoxeterARCer --------------------- //

void ToddCoxeterARCer::runComputation( )
{
  retValue = enumerator.enumerate();
  permutationRepresentation = 
    new  PermutationRepresentation(enumerator.permutationRepresentation());
}

void ToddCoxeterARCer::writeResults( ostream& out ) 
{
  out < retValue;
  if (retValue !=0)  out < *permutationRepresentation;
}

void ToddCoxeterARCer::readResults( istream& in )
{
  in > retValue;
  if (retValue !=0) {
    permutationRepresentation = new PermutationRepresentation();
    in > *permutationRepresentation;
  }
}

const PermutationRepresentation& ToddCoxeterARCer::getTransversal()const
{
  if (permutationRepresentation == NULL)
    error("VectorOf<Word> getTransversal()const : transversal was not computed yet.");
  return *permutationRepresentation;
}

// -------------------------- ToddCoxeter ---------------------------- //

ToddCoxeter::ToddCoxeter(GCM& gcm)
  : theGCM( gcm ),
    theGroup( gcm.getSMFPGroup() )
{
  if (theGroup.gic.isNilpotent()==yes) {
    CommutatorIterator I(theGroup.getFPGroup().numberOfGenerators(),
			 theGroup.gic.getNilpotentcyClass()+1);
    SetOf<Word> newRelators = theGroup.getFPGroup().getRelators();
    for( I.reset(); !I.done(); I.next() ) newRelators |= I.value();
    FPGroup g(theGroup.getFPGroup().namesOfGenerators(), newRelators);
    arcer = new ToddCoxeterARCer(*this, g);
  }
  else if (theGroup.gic.isAbelian()==yes) {
    CommutatorIterator I(theGroup.getFPGroup().numberOfGenerators(), 2);
    SetOf<Word> newRelators = theGroup.getFPGroup().getRelators();
    for( I.reset(); !I.done(); I.next() ) newRelators |= I.value();
    FPGroup g(theGroup.getFPGroup().namesOfGenerators(), newRelators);
    arcer = new ToddCoxeterARCer(*this, g);
  }
  else 
    arcer = new ToddCoxeterARCer(*this, theGroup.getFPGroup());
  adminStart();
}

void ToddCoxeter::takeControl( )
{
  if ( freeARCs() > 0 )
    if( arcer->takeControl() ) {
      Chars explanation;
      // @am this is just for answer to be reasonable
      // because Todd-Coxeter checks for order of abelian
      // quotient first
      if (arcer->getRetValue())
	explanation = "according to the Todd-Coxeter algorithm";
      else
	explanation = Text("this follows from the canonical decomposition of")
	  + Name(theGroup);
      theGroup.gic.putHaveOrder( arcer->getRetValue(), explanation );
      if ( arcer->getRetValue() !=0 )
	theGroup.gic.putHaveSchreierTransvl(arcer->getTransversal());
      adminTerminate();
    }
    else
      usedOneARC();
}

// -------------------------- SGIndexToddCoxeter ------------------------- //

SGIndexToddCoxeter::SGIndexToddCoxeter(SCM& scm)
  : theSCM( scm ),
    theSubgroup( scm.getSubgroup() )
{
  if( theSubgroup.getParent().gic.isAbelian()==yes ) {
    CommutatorIterator I( theSubgroup.getParent().getFPGroup().
			  numberOfGenerators(), 2);
    SetOf<Word> newRelators =
      theSubgroup.getParent().getFPGroup().getRelators();
    for( I.reset(); !I.done(); I.next() ) newRelators |= I.value();
    FPGroup newGroup( theSubgroup.getParent().getFPGroup().namesOfGenerators(),
		      newRelators);
    arcer = new ToddCoxeterARCer( *this, newGroup, 
				  theSubgroup.getSubgroup().generators() );
  }
  else arcer = new ToddCoxeterARCer( *this, 
				     theSubgroup.getParent().getFPGroup(),
				     theSubgroup.getSubgroup().generators() );
  adminStart();
}


void SGIndexToddCoxeter::takeControl( )
{
  if ( freeARCs() > 0 )
    if( arcer->takeControl() ) {
      theSubgroup.sic.putIndex( arcer->getRetValue());
      if ( arcer->getRetValue() !=0 )
	theSubgroup.sic.putHaveSchreierTransvl(arcer->getTransversal());
      adminTerminate();
    }
    else
      usedOneARC();
}
// -------------------------- SchreierTransversal ------------------------- //

SchreierTransversal::SchreierTransversal(SMFPGroup& g)
  : Supervisor( true ),
    theObject( g ),
    theGroup( g ),
    theToddCoxeter(*this,g.gcm().theToddCoxeter)
{
  if ( haveSchreierTransvl() ) resultIsFast();
}

SchreierTransversal::SchreierTransversal(SMSubgroup& s)
  : Supervisor( true ),
    theObject( s ),
    theGroup( s.getParent() ),
    theToddCoxeter(*this,s.scm().sgToddCoxeter)
{
  if ( haveSchreierTransvl() ) resultIsFast();
}

void SchreierTransversal::takeControl( )
{
  if (isInfinite()) {
    LogMessage msg(*this,theObject);
    if (Chars(theObject.typeID())==Chars("SMFPGroup"))
      msg << Name(theObject) << " is infinite";
    else 
      msg << Name(theObject) << " has infinite index in " << Name(theGroup); 
    msg << ". Can't compute Schreier transversal.";
    msg.send();
    adminTerminate();
    return;    
  }
  if (haveSchreierTransvl() ) {
    File file;
    VectorOf<Word> transversal = getRepresentatives();
    FPGroup g = theGroup.getFPGroup();
    for (int i=0; i<transversal.length(); i++) {
      file << "s" << i+1 << " = ";
      g.printWord(file, transversal[i]);
      file << endl;
    }
    LogMessage msg(*this,theObject);
    if (Chars(theObject.typeID())==Chars("SMFPGroup"))
      msg << Link( Chars( "Click here to see Shreier representatives for ") 
		   + Text( Name(theObject) ) + Text("."), 
		   "SchreierTransversal", file.getFileName() );
    else
      msg << Link( Chars( "Click here to see Shreier transversal of ")
		   + Text( Name(theObject) ) + Text("."),
		   "SchreierTransversal", file.getFileName() );      
    msg.send();
    adminTerminate();
    return;
  }
}

bool SchreierTransversal::haveSchreierTransvl() const 
{
  if (Chars(theObject.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theObject).gic.haveSchreierTransvl();
  else
    return ((SMSubgroup& )theObject).sic.haveSchreierTransvl();
}
  
const VectorOf<Word>& SchreierTransversal::getRepresentatives() const
{
  if (Chars(theObject.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theObject).gic.getSchreierTransvl().getRepresentatives();
  else
    return ((SMSubgroup& )theObject).sic.getSchreierTransvl().getRepresentatives();    
}

bool SchreierTransversal::isInfinite() const
{
  if (Chars(theObject.typeID())==Chars("SMFPGroup"))
     return ((SMFPGroup& )theObject).gic.isFinite()==no;
  else 
    return ((SMSubgroup& )theObject).sic.index()==0;
}

void SchreierTransversal::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Schreier representatives"),
		  Text("Find Schreier representatives for") 
		  + Name( theObject ),
		  helpID("SchreierTransversal",theGroup),
		  Text("schr. repr.") + Name(theObject) );
  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::FP ||
		     CheckinType( theGroup ) == SMFPGroup::ABELIAN ||
		     CheckinType( theGroup ) == SMFPGroup::NILPOTENT );
  pv.add(Text("Enumerate cosets") + Name( theObject ),
	 theToddCoxeter.arcSlotID(), 100 );   
  pv.done();
}


// ---------------------- PermutationRepProblem ----------------------- //

PermutationRepProblem::PermutationRepProblem(SMFPGroup& g)
  : Supervisor( true ),
    theObject( g ),
    theGroup( g ),
    theToddCoxeter(*this, g.gcm().theToddCoxeter)
{
  if ( haveSchreierTransvl() ) resultIsFast();
}

PermutationRepProblem::PermutationRepProblem(SMSubgroup& s)
  : Supervisor( true ),
    theObject( s ),
    theGroup( s.getParent() ),
    theToddCoxeter(*this, s.scm().sgToddCoxeter)
{
  if ( haveSchreierTransvl() ) resultIsFast();
}

void PermutationRepProblem::takeControl( )
{
  if (isInfinite()) {
    LogMessage msg(*this,theObject);
    if (Chars(theObject.typeID())==Chars("SMFPGroup"))
      msg << Name(theObject) << " is infinite";
    else 
      msg << Name(theObject) << " has infinite index in " << Name(theGroup); 
    msg << ". Can't compute Schreier transversal.";
    msg.send();
    adminTerminate();
    return;    
  }
  if (haveSchreierTransvl() ) {
    File file;
    FPGroup g = theGroup.getFPGroup();
    getPermutations().printOn(g.namesOfGenerators(), file);
    LogMessage msg(*this, theObject);
    if (Chars(theObject.typeID())==Chars("SMFPGroup"))
      msg << Link(Chars( "Click here to see a permutation representation of ")
		  + Text( Name(theObject) ) + Text("."),
		  "PermutationRepresentation", file.getFileName() );
    else
      msg << Link( Chars( "Click here to see the presentation of ")
		   + Text( Name(theGroup) ) 
		   + Text(" by permutations on cosets of ") 
		   + Text( Name(theObject) ) + Text("."),
		   "PermutationRepresentation", file.getFileName() );     
    msg.send();
    adminTerminate();
    return;
  }
}

bool PermutationRepProblem::haveSchreierTransvl()const
{
  if (Chars(theObject.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theObject).gic.haveSchreierTransvl();
  else
    return ((SMSubgroup& )theObject).sic.haveSchreierTransvl();
}
  
const PermutationRepresentation& PermutationRepProblem::getPermutations()const
{
  if (Chars(theObject.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theObject).gic.getSchreierTransvl();
  else
    return ((SMSubgroup& )theObject).sic.getSchreierTransvl();    
}

bool PermutationRepProblem::isInfinite()const 
{
  if (Chars(theObject.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theObject).gic.isFinite()==no;
  else
    return ((SMSubgroup& )theObject).sic.index()==0;
}

void PermutationRepProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Permutation representation"),
		  Text("Find a permutation representation of") + 
		  Name( theObject ),
		  helpID("PermutationRepProblem",theGroup),
		  Text("perm.") + Name(theObject) );
  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::FP ||
		     CheckinType( theGroup ) == SMFPGroup::ABELIAN || 
		     CheckinType( theGroup ) == SMFPGroup::NILPOTENT );
  pv.add(Text("Enumerate cosets") + Name( theObject ), 
	 theToddCoxeter.arcSlotID(), 100);   
  pv.done();
}


// ------------------- WordRepresentativeARCer --------------------- //

void WordRepresentativeARCer::setArguments(const PermutationRepresentation& pr,
					   const Word& w)
{
  theWord = w;
  permutationRepresentation =  new  PermutationRepresentation(pr);
}

void WordRepresentativeARCer::runComputation( )
{
  retValue = permutationRepresentation->representativeOf(theWord);
}

void WordRepresentativeARCer::writeResults( ostream& out ) 
{
  out < retValue;
}

void WordRepresentativeARCer::readResults( istream& in )
{
  in > retValue;
}

// -------------------------- WordRepresentative -------------------------- //

WordRepresentative::WordRepresentative(class WordRepresentativeProblem& boss)
  : theParent( boss.getParent() ),
    theGroup( boss.getGroup() ),
    theTestWords(1),
    theWordsReps(1),
    theWordsRepsFound(1),
    checkedWord(0),
    arcer( *this ),
    arcerStarted( false )
{
  theTestWords[0] = boss.getWord().getWord();
  adminStart();
}

WordRepresentative::WordRepresentative(class SubgroupContainmentProblem& boss)
  : theParent( boss.getSubgroup() ),
    theGroup( boss.getTestWord().getParent() ),
    theTestWords(1),
    theWordsReps(1),
    theWordsRepsFound(1),
    checkedWord(0),
    arcer( *this ),
    arcerStarted( false )
{
  theTestWords[0] = boss.getTestWord().getWord();
  adminStart();
}

WordRepresentative::WordRepresentative(class IsSGNormal& boss)
  : theParent( boss.getSubgroup() ),
    theGroup( boss.getSubgroup().getParent() ),
    theTestWords( makeVectorOf(boss.getTestWords()) ),
    theWordsReps( boss.getTestWords().cardinality() ),
    theWordsRepsFound( boss.getTestWords().cardinality() ),
    checkedWord(0),
    arcer( *this ),
    arcerStarted( false )
{
  for( int i = 0; i < theWordsRepsFound.length(); ++i )
    theWordsRepsFound[i] = false;
  adminStart();
}

void WordRepresentative::takeControl( )
{
  if( isInfinite() ) {
    for( int j = 0; j < theWordsRepsFound.length(); ++j )
      theWordsRepsFound[j] = false;
    adminTerminate();
    return;    
  }
  if( freeARCs() > 0 && haveSchreierTransvl() ) {
    if ( !arcerStarted ){
      if( checkedWord == theTestWords.length() ) {
	adminTerminate();
	return;
      }
      arcer.setArguments( getPermutations(), theTestWords[checkedWord] );
      arcerStarted = true;
    }
    if( arcer.takeControl() ) {
      theWordsRepsFound[checkedWord] = yes;
      theWordsReps[checkedWord] = arcer.getRetValue();
      ++checkedWord;
      arcerStarted = false;
    }
    usedOneARC();
  } 
}

bool WordRepresentative::haveSchreierTransvl( ) const 
{
  if( Chars(theParent.typeID()) == Chars("SMFPGroup") )
    return ((SMFPGroup& )theParent).gic.haveSchreierTransvl();
  else
    return ((SMSubgroup& )theParent).sic.haveSchreierTransvl();
}

const PermutationRepresentation& WordRepresentative::getPermutations( ) const
{
  if( Chars(theParent.typeID()) == Chars("SMFPGroup") )
    return ((SMFPGroup& )theParent).gic.getSchreierTransvl();
  else
    return ((SMSubgroup& )theParent).sic.getSchreierTransvl();    
}

bool WordRepresentative::isInfinite( ) const
{
  if( Chars(theParent.typeID()) == Chars("SMFPGroup") )
    return ((SMFPGroup& )theParent).gic.isFinite()==no;
  else
    return ((SMSubgroup& )theParent).sic.index()==0;
}

// ---------------------- WordRepresentativeProblem ----------------------- //

WordRepresentativeProblem::WordRepresentativeProblem(SMWord& w)
  : Supervisor( true ),
    theParent( w.getParent() ),
    theGroup( w.getParent()),
    theWord( w ),
    theToddCoxeter( *this, w.getParent().gcm().theToddCoxeter ),
    theWordRepCM( *this )
{
  if( isInfinite() ) resultIsFast();
}

WordRepresentativeProblem::WordRepresentativeProblem(SMSubgroup& s,SMWord& w)
  : Supervisor( true ),
    theParent( s ),
    theGroup( w.getParent() ),
    theWord( w ),
    theToddCoxeter( *this, s.scm().sgToddCoxeter ),
    theWordRepCM( *this ) 
{
  if( isInfinite() ) resultIsFast(); 
}

void WordRepresentativeProblem::takeControl( )
{
  if( isInfinite() ){
    LogMessage msg( *this, theParent, theWord );
    msg << Name(theParent) << " is infinite. Can't find a reresentative for " 
	<< Name( theWord ) << ".";
    msg.send();
    adminTerminate();
    return;    
  }
  if( theWordRepCM->state() == ComputationManager::TERMINATED ) {
    LogMessage msg( *this, theParent, theWord );
    msg << "Schreier representative of " << Name( theWord );
    if( Chars(theParent.typeID()) == Chars("SMFPGroup") )
      msg << " modulo 1 is ";
    else
      msg << " modulo "<<  Name( theParent ) << " is ";
    theGroup.getFPGroup().printWord( msg, theWordRepCM->getWordsReps()[0] );
    msg << ".";
    msg.send();
    adminTerminate();
    return;	
  } 
}


bool WordRepresentativeProblem::haveSchreierTransvl()const {
  if (Chars(theParent.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theParent).gic.haveSchreierTransvl();
  else
    return ((SMSubgroup& )theParent).sic.haveSchreierTransvl();
}
  
/*
const PermutationRepresentation& 
WordRepresentativeProblem::getPermutations()const{
  if (Chars(theParent.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theParent).gic.getSchreierTransvl();
  else
    return ((SMSubgroup& )theParent).sic.getSchreierTransvl();    
}
*/

bool WordRepresentativeProblem::isInfinite()const{
  if (Chars(theParent.typeID())==Chars("SMFPGroup"))
    return ((SMFPGroup& )theParent).gic.isFinite()==no;
  else 
    return ((SMSubgroup& )theParent).sic.index()==0;
}

void WordRepresentativeProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Schreier representative of a word"),
		  Text("Find Schreier representative of ") + Name( theWord ),
		  helpID("WordRepresentativeProblem",theGroup),
		  Text("repr.") + Name(theWord) );
  pv.startItemGroup( CheckinType( theGroup ) == SMFPGroup::FP ||
		     CheckinType( theGroup ) == SMFPGroup::ABELIAN ||
		     CheckinType( theGroup ) == SMFPGroup::NILPOTENT );
  if( !haveSchreierTransvl() )
    pv.add( Text("Enumerate cosets of") + Name( theParent ),
	    theToddCoxeter.arcSlotID(), 100 );   
  pv.add( Text("Find a representative of ") + Name(theWord),
	  theWordRepCM.arcSlotID(), 50 );
  pv.done();
}
#line 1  "src/GeneticProblems.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes GeneticWPArcer, GeneticWPCM
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "GeneticProblems.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "GAWP.h"


// -------------------------- GeneticWPArcer ------------------------------- //


GeneticWPArcer::GeneticWPArcer( ComputationManager& boss )
  : ARCer( boss ), retValue(dontknow), file( 0 )
{ } 

void GeneticWPArcer::setArguments( const FPGroup& group, 
				   const SetOf<Word>& words, 
				   DetailType dt )
{
  theGroup = group;
  theWords = words;
  dtype = dt;
  if( dtype != NO_DETAILS ) 
    file = new File;
}

Chars GeneticWPArcer::getFileName() 
{
#if SAFETY > 0
  if( file == 0 )
    error("Chars GeneticWPArcer::getFileName( ) : "
	  "You chose not to keep details in a file.");
#endif

  return file->getFileName(); 
}

void GeneticWPArcer::runComputation( ) 
{
  GHNConfig c;
  GAWP2 gawp(theGroup,c);
  if( dtype == WORD ) {
    *file << "The algorithm tries to reduce length of the word genetically. "
      "The fitness value below is the lowest length produced on the "
      "current generation. " << endl << endl << ready;
  } else if ( dtype == SET_OF_WORDS ) {
    *file << "The algorithm tries to reduce length of the words genetically. "
      "The fitness value below is the lowest length produced on the "
      "current generation. " << endl << endl << ready;
  }  else if ( dtype == COMMUTATORS ) {
    *file << "The algorithm tries to reduce length of the commutators "
      "genetically. "
      "The fitness value below is the lowest length produced on the "
      "current generation. " << endl << endl << ready;
  } 
  
  SetIterator<Word> I(theWords);
  retValue = yes;
  for(; !I.done(); I.next() ) {

    if ( dtype != NO_DETAILS && dtype != WORD ) {
      *file << endl << "Solving the word problem for w = ";
      theGroup.printWord(*file, I.value());
      *file << endl << endl << ready;
    } 
    
    retValue = gawp.isTrivial(I.value(), file);
    if( retValue != yes ) break;
  }
}

void  GeneticWPArcer::writeResults( ostream& out )
{
  out < retValue;
}

void GeneticWPArcer::readResults( istream& in )
{
  in > retValue;
}


// ---------------------------- GeneticWPCM -------------------------------- //


GeneticWPCM::GeneticWPCM( Supervisor& boss )
: theBoss( boss ),
  tAreTrivial( dontknow ),
  bStarted( false ),
  bInited( false ),
  arcer( *this )
{
  adminStart();
}

void GeneticWPCM::terminate( )
{
  //arcer.terminate(); 
}

void GeneticWPCM::start( ) { }

void GeneticWPCM::init( const FPGroup& group, const SetOf<Word> words,
			DetailType dt )
{
  theGroup = group;
  theWords = words;
  dtype = dt;
  bInited = true;
}

void GeneticWPCM::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if( !bStarted ) {

    #if SAFETY > 0
      if( !bInited )
	error("void GeneticWPCM::takeControl( ) : "
	      "The computation manager was not initialized.");
    #endif  
    
      arcer.setArguments( theGroup, theWords, dtype );
      bStarted = true;
      if( dtype != NO_DETAILS ) {
	LogMessage msg( *this, theBoss );
	msg << Link("Click here to see details of the genetic algorithm 1.",
		    "GeneticWP", getFileName(), true);
	msg.send(); 
      }
    }
    if( arcer.takeControl() ) {
      tAreTrivial = arcer.getRetValue();
      adminTerminate();
      return;
    }
    else
      usedOneARC();
  }
}
#line 1  "src/GAEquations.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes 
//           GAEquationArcer, GAEquationCM, GAEquationProblem,
//           TwoCommArcer, TwoCommCM, TwoCommProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "GAEquations.h"
#include "TwoCommSolver.h"
#include "SMFPGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "ARCer.h"


// --------------------------- GAEquationArcer ----------------------------- //


void GAEquationArcer::setArguments( FreeGroup F, int n, Word w )
{
  theGroup = F;
  numOfVars = n;
  theWord = w;
}


void GAEquationArcer::runComputation( )
{
  GHNConfig c;
  GAEquationSolver solver(theGroup,numOfVars,c);
  solution = solver.getSolution(theWord,&file);
  retValue = true;
}


void GAEquationArcer::writeResults( ostream& out ) 
{
  out < retValue;
}


void GAEquationArcer::readResults( istream& in )
{
  in > retValue;
}


Map GAEquationArcer::getSolution( ) const
{ 
#if SAFETY > 0
  if ( retValue != yes )
    error( "Map GAEquationArcer::getSolution( ) const : "
	   "the solution has not been found yet.");
#endif

  Map s;
  return s;
  //return solution; 
}


// ----------------------------- GAEquationCM ------------------------------ //


GAEquationCM::GAEquationCM( class GAEquationProblem& PO )
  : theEquation( PO.getEquation() ),
    arcer( *this ),
    theAnswer( dontknow ),
    bStarted( false )
{
  adminStart();
  //filename = arcer.getFileName();
}


void GAEquationCM::start( )
{
  //arcer.setArguments(theEquation);
}


void GAEquationCM::takeControl( )
{
  if ( freeARCs() > 0 ) { 
    
    if( !bStarted ) {
      bStarted = true;
      arcer.setArguments(theEquation.getFreeGroup(), 
			 theEquation.numberOfVariables(),  
			 theEquation.getWord());
    }
    
    if( arcer.takeControl() ) {
      
      theAnswer = arcer.haveSolution();
      //solution = arcer.getSolution();
      //LogMessage msg( *this, theEquation );
      //msg << "Found solution: " << getSolution() << ".";
      //msg.send();
      adminTerminate();
    }
    else
      usedOneARC();
  }
}


Map GAEquationCM::getSolution( ) const
{ 
#if SAFETY > 0
  if ( theAnswer != yes )
    error( "Map GAEquationCM::getSolution( ) const : "
	   "the solution is not found yet.");
#endif

  return solution; 
}


// ------------------------- GAEquationProblem ----------------------------- //


GAEquationProblem::GAEquationProblem( SMEquation2& E )
  : Supervisor( true ),
    theEquation( E ),
    linkHasBeenSent( false ),
    GA( *this )
{ }


void GAEquationProblem::takeControl( )
{
  //if ( freeARCs() > 0 ) { 
 
    if( !linkHasBeenSent ) {
      
      linkHasBeenSent = true;
      
      LogMessage msg( *this, theEquation );
      msg << Link("Click here to obtain details of the computation.",
		  "GAEquationProblem", GA->getFileName(), true);
      msg.send();
    }
  
    //Trichotomy answer = GA->haveSolution();
    if ( GA->haveSolution() != dontknow ) {
      /*
      LogMessage msg( *this, theEquation );
      msg << "Found solution: " << GA->getSolution() << ".";
      msg.send();
      */
      adminTerminate();
      return;
    }
  
  
    if( GA->state() == ComputationManager::TERMINATED )
      adminTerminate();
    
    //}
}


void GAEquationProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find solution"),
		  Text("Find a solution for")
		  + Name( theEquation ), 
		  helpID( "GAEquations", theEquation.getParent() ),
		  "Solution"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of genetic algorithm"), GA.arcSlotID(), 100);

  pv.done();
}


// ----------------------------- TwoCommArcer ------------------------------ //


void TwoCommArcer::setArguments( FreeGroup F, Word w )
{
  theGroup = F;
  theWord = w;
}


void TwoCommArcer::runComputation( )
{
  TwoCommSolver solver(theGroup);
  Word x1,x2,y1,y2;
  solver.isProductOfTwoComms(theWord,x1,x2,y1,y2,&file);
  retValue = true;
}


void TwoCommArcer::writeResults( ostream& out ) 
{
  out < retValue;
}


void TwoCommArcer::readResults( istream& in )
{
  in > retValue;
}


// ------------------------------- TwoCommCM ------------------------------- //


TwoCommCM::TwoCommCM( class TwoCommProblem& PO )
  : theWord( PO.getWord() ),
    arcer( *this ),
    theAnswer( dontknow ),
    bStarted( false )
{
  adminStart();
}


void TwoCommCM::start( )
{ }


void TwoCommCM::takeControl( )
{
  if ( freeARCs() > 0 ) { 
    
    if( !bStarted ) {
      bStarted = true;
      arcer.setArguments( theWord.getParent().getFreePreimage(), 
			  theWord.getWord() );
    }
    
    if( arcer.takeControl() ) {
      theAnswer = arcer.haveSolution();
      adminTerminate();
    }
    else
      usedOneARC();
  }
}


// --------------------------- TwoCommProblem ------------------------------ //


TwoCommProblem::TwoCommProblem( SMWord& w )
  : Supervisor( true ),
    theWord( w ),
    linkHasBeenSent( false ),
    GA( *this )
{ }


void TwoCommProblem::takeControl( )
{
  //if ( freeARCs() > 0 ) { 
 
    if( !linkHasBeenSent ) {
      
      linkHasBeenSent = true;
      
      LogMessage msg( *this, theWord );
      msg << Link("Click here to obtain details of the computation.",
		  "TwoCommProblem", GA->getFileName(), true);
      msg.send();
    }
  
    if ( GA->haveSolution() != dontknow ) {
      adminTerminate();
      return;
    }
  
  
    if( GA->state() == ComputationManager::TERMINATED )
      adminTerminate();
    
    //}
}


void TwoCommProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Is product of two commutators ?"),
		  Text("Is") + Name( theWord ) 
		  + "product of two commutators ?", 
		  helpID( "TwoCommProblem", theWord.getParent() ),
		  "Two comms"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of genetic algorithm"), GA.arcSlotID(), 100);

  pv.done();
}
#line 1  "src/Rewrites.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of the RewritesARCe, CommutatorRewriteProblem, 
//           SquareRewriteProblem classes.
//
// Principal Authors: Denis Serbin
//
// Status: In progress
//
// Revision History:
//
// Special Notes:
//
//

#include "Rewrites.h"
#include "SMWord.h"
#include "SMFPGroup.h"
#include "OutMessages.h"


// ------------------------ RewritesARCer ------------------------- //


void RewritesARCer::setArguments( const FPGroup& gr,const Word& w, int ord )
{
  G = gr;
  theWord = w.freelyReduce();
  order = ord;
}

void RewritesARCer::runComputation( ) 
{
  if( order == 1 )
    retValue = G.productOfCommutators(theWord,file);
  
  if( order == 2 )
    retValue = G.productOfSquares(theWord,file);
}

void RewritesARCer::writeResults( ostream& out )
{
  out < retValue;
}


void RewritesARCer::readResults( istream& in )
{
  in > retValue;
}

// ------------------------ CommutatorRewriteProblem ----------------------- //


CommutatorRewriteProblem::CommutatorRewriteProblem( SMWord& word )
  : theWord( word ),
    ComputationManager( true),
    arcer( *this )
{
  if( !theWord.getWord().freelyReduce().length() || 
      theWord.getWord().allExponentSumsZero() )
    resultIsFast();
  else
    arcer.setArguments(theWord.getParent().getFPGroup(),theWord.getWord(),1);
}

void CommutatorRewriteProblem::takeControl( )
{
  if( !theWord.getWord().freelyReduce().length() )
    {
      LogMessage msg2( *this,theWord );
      msg2 << Name( theWord ) << " is trivial.";
      msg2.send();
      adminTerminate();
      return;
    }
  
  if( theWord.getWord().allExponentSumsZero() )
    {
      LogMessage msg1( *this,theWord );
      VectorOf<Chars> v = theWord.getParent().getFPGroup().namesOfGenerators();
      FreeGroup F(v);
      File f;
      
      msg1 << Name( theWord ) << " can be rewritten as: " << F.productOfCommutators(theWord.getWord(),f);
      msg1.send();
      
      LogMessage msg4( *this,theWord );
      msg4 << Link("Click here to see the steps of the rewriting process" , "Commutators" , f.getFileName() );
      msg4.send();      
      
      f << end;

      adminTerminate();
      return;
    }
  
  if ( freeARCs() > 0 ) 
    if( arcer.takeControl() ) 
      {
	Chars result = arcer.getRetValue();
	LogMessage msg( *this,theWord );
	
	if ( result == Chars("not in derived") )
	  msg << Name( theWord )  << " isn't in derived subgroup.";
	else
	  if ( result == Chars("trivial") )
	    msg << Name( theWord ) << " is trivial.";
	  else
	    {
	      msg << Name( theWord ) << " can be rewritten as:  " << result << " . ";
	      LogMessage msg3( *this,theWord );
	      msg3 << Link("Click here to see the steps of the rewriting process" , "Commutators" , arcer.getFileName() );
	      msg3.send();
	    }
	
	msg.send();
	
	adminTerminate();
	return;
      }
    else
      usedOneARC();
}

void CommutatorRewriteProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Rewriting of a word as a product of commutators"),
		  Text("Rewrite")+Name(theWord)+Text(" as a product of commutators"),
		  helpID("CommutatorRewriteProblem",theWord.getParent()),
		  Text("rewrite") + Name(theWord)
		  );
  pv.startItemGroup();
  
  pv.add(Text("Rewrite ")+Name( theWord )+Text(" as a product of commutators"),
  	 ThisARCSlotID(),
  	 50);
  
  pv.done();
}


// ------------------------ SquareRewriteProblem ----------------------- //


SquareRewriteProblem::SquareRewriteProblem( SMWord& word )
  : theWord( word ),
    ComputationManager( true),
    arcer( *this )
{
  if( !theWord.getWord().freelyReduce().length() )
    resultIsFast();
  else
    arcer.setArguments(theWord.getParent().getFPGroup(),theWord.getWord(),2);
}

void SquareRewriteProblem::takeControl( )
{
  if( !theWord.getWord().freelyReduce().length() )
    {
      LogMessage msg2( *this,theWord );
      msg2 << Name( theWord ) << " is trivial.";
      msg2.send();
      adminTerminate();
      return;
    }
  
  if( freeARCs() > 0 ) 
    if( arcer.takeControl() ) 
      {
	Chars result = arcer.getRetValue();
	LogMessage msg( *this,theWord );
	
	if ( result == Chars("not in square") )
	  msg << Name( theWord )  << " isn't in subgroup generated by squares.";
	else
	  if ( result == Chars("trivial") )
	    msg << Name( theWord ) << " is trivial.";
	  else
	   { 
	     msg << Name( theWord ) << " can be rewritten as:  " << result << " . ";
	     LogMessage msg3( *this,theWord );
	     msg3 << Link("Click here to see the steps of the rewriting process" , "Squares" , arcer.getFileName() );
	     msg3.send();
	   }
	
	msg.send();
	
	adminTerminate();
	return;
      }
    else
      usedOneARC();
}

void SquareRewriteProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Rewriting of a word as a product of squares"),
		  Text("Rewrite")+Name(theWord)+Text(" as a product of squares"),
		  helpID("SquareRewriteProblem",theWord.getParent()),
		  Text("rewrite") + Name(theWord)
		  );
  pv.startItemGroup();
  
  pv.add(Text("Rewrite ")+Name( theWord )+Text(" as a product of squares"),
  	 ThisARCSlotID(),
  	 50);
  
  pv.done();
}
#line 1  "src/SubgroupContainmentProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes 
// 
//
// Principal Author: Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//
//

#include "SubgroupContainmentProblem.h"
#include "SMSubgroup.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "SubgroupGraph.h"
#include "DecomposeInSubgroup.h"
#include "conversions.h"

// ------------------------- SubgroupContainment ---------------------- //

SubgroupContainment::SubgroupContainment( SubgroupContainmentProblem& boss )
  : // arcer( *this ), arcerStarted( false ),
    theSubgroup( boss.getSubgroup() ),
    theTestWords( 1 ),
    wordsToCheck( boss.getTestWord().getWord() ),
    theAnswer( dontknow ),
    wordsDecomposition( boss.getSubgroup().getParent().getFPGroup(),
			boss.getSubgroup().getSubgroup().generators() )
{
  theTestWords[0] = boss.getTestWord().getWord();

  adminStart();
}



SubgroupContainment::SubgroupContainment( IsSGNormal& boss )
  : theSubgroup( boss.getSubgroup() ),
    theTestWords( makeVectorOf(boss.getTestWords()) ),
    wordsToCheck( boss.getTestWords() ),
    theAnswer( dontknow ),
    wordsDecomposition( boss.getSubgroup().getParent().getFPGroup(),
			boss.getSubgroup().getSubgroup().generators() )
{
  adminStart();
}

Trichotomy SubgroupContainment::contains( ) const 
{
  return theAnswer;
}

// Find words which are belonged to the current approximation
// and remove them from the test set.
SetOf<Word> SubgroupContainment::reduceTestingSet( 
  const DecomposeInSubgroupOfFPGroup& D, SetOf<Word>& testingSet )
{
  SetOf<Word> result;
  SetIterator<Word> I(testingSet);

  for( I.reset(); !I.done(); I.next() ) {
    Word w = I.value();
    if( !D.contains( w ) ) {
      result |= w;
    }
    else {
      wordsDecomposition.printSubgroupElement( 
        w, D.fullDecomposition(w), !wordsDecomposition.haveDetails() 
      );
    }
  }

  return result;
}

void SubgroupContainment::takeControl( )
{
  if ( freeARCs() > 0 ) {

    if( theSubgroup.sic.haveWordDecomposer() ) {

      const DecomposeInSubgroupOfFPGroup& D = 
	theSubgroup.sic.getWordDecomposer();

      wordsToCheck = reduceTestingSet( D, wordsToCheck );

      if( wordsToCheck.cardinality() == 0 ) {
	theAnswer = yes;

	adminTerminate();
	return;
      }    
    }
    
    if( theSubgroup.sic.haveCompleteCayleyGraph() ) {

      const DecomposeInSubgroupOfFPGroup& D = 
	theSubgroup.sic.getWordDecomposer();

      wordsToCheck = reduceTestingSet( D, wordsToCheck );

      if( wordsToCheck.cardinality() == 0 ) {
	theAnswer = yes;
      }    
      theAnswer = no;

      adminTerminate();
      return;
    }
    
    usedOneARC();
  }
  
}


SubgroupContainmentProblem::SubgroupContainmentProblem( SMSubgroup& subgroup,
							SMWord& w )
  : Supervisor( true ),
    theSubgroup( subgroup ),
    theTestWord( w ),
    isWordInSubgroup( dontknow ),
    subgroupContain( *this ),
    normalClosure( *this, subgroup.scm().normalClosure ),
    abelianInvariants( *this, subgroup.scm().abelianInvariantsOfFactor ),
    sgNilpotentQuotients( *this, subgroup.scm().sgNilpotentQuotients ),
    scpInNilpotentQuotients( *this ),
    toddCoxeter( *this, subgroup.scm().sgToddCoxeter ),
    wordRepCM( *this )
{

  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    if( A.isTrivial(theTestWord.getWord()) == false ) {
      isWordInSubgroup = no;
      explanation = " it follows from canonical decomposition";
      resultIsFast();
    }
  }
  scpInNilpotentQuotients->initialize( theTestWord.getWord(), &theSubgroup );
}


void SubgroupContainmentProblem::takeControl( )
{
  if( isWordInSubgroup == dontknow ) {

    // Check abelian computation managers.
    if( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
      const AbelianGroup& A=theSubgroup.sic.getCyclicDecompositionOfFactor();
      if( A.isTrivial(theTestWord.getWord()) == false ) {
	isWordInSubgroup = no;
	explanation = " it follows from canonical decomposition";
      }
    }
    
    // Check nilpotent computation managers.
    if( scpInNilpotentQuotients->state() == ComputationManager::TERMINATED ){
      int retClass;
      Trichotomy theAnswer = scpInNilpotentQuotients->contains(retClass);
      
      if( theAnswer == no ){
	isWordInSubgroup = no;
	ostrstream msgTmp;
	msgTmp << " because it's not trivial in lower central quotient" 
	       << "of class " << retClass << ends;
	explanation = msgTmp.str();
	msgTmp.freeze(0);
      }
    }
    
    // Check Todd-Coxeter computation managers.
    if( wordRepCM->state() == ComputationManager::TERMINATED ) {
      
      if( !wordRepCM->isInfinite() ) {
	Word rep = wordRepCM->getWordsReps()[0];
	ostrstream msgTmp;
	msgTmp << " because Schreier representative of " << Name(theTestWord)
	       << " modulo " << Name( theSubgroup );
	if( rep.length() == 0 ) {
	  isWordInSubgroup = yes;
	  msgTmp << " is 1";
	}
	else {
	  isWordInSubgroup = no;
	  msgTmp << " is not 1";
	}
	explanation = msgTmp.str();
	msgTmp.freeze(0);
      }
    }
  }
  
  if( isWordInSubgroup != dontknow ) {
    
    // Send message to front end about found results and quit.
    LogMessage msg(*this, theTestWord, theSubgroup);
    msg << Name( theTestWord );
    if( isWordInSubgroup == yes ) 
      msg << " lies ";
    else
      msg << " does not lie ";
    msg << "in " << Name( theSubgroup ) 
	<< explanation << ".";
    msg.send();
    adminTerminate();
  }
  
  if( subgroupContain->state() == ComputationManager::TERMINATED ) {

    LogMessage msg( *this, theSubgroup, theTestWord );
    msg << Name(theTestWord)
	<< (subgroupContain->contains() == yes ? " lies " : " does not lie ")
	<< "in " << Name(theSubgroup) 
	<< " according to a normal closure approximation.";
    msg.send();
    
    if( subgroupContain->contains() == yes
	&& subgroupContain->haveDetails() ) {
      LogMessage msgLink( *this, theSubgroup, theTestWord );
      msgLink << Link( Chars("Click here to see a decomposition of ")
		       + Text(Name(theTestWord))
		       + Text(" in terms of subgroup generators and "
			      "as a product of conjugates of given relators."),
		       "SubgroupContainmentDetails", 
		       subgroupContain->getDecompositionFileName()
		       );
      msgLink.send();
    }
    
    adminTerminate();
  }
  
}

void SubgroupContainmentProblem::start( )
{
}

void SubgroupContainmentProblem::terminate( )
{
}


void SubgroupContainmentProblem::viewStructure(ostream& ostr) const
{

  ProblemView pv( ostr, oid(), Text("Subgroup containment problem"),
		  Text("Does") + Name( theSubgroup ) + "contain "
		  + Name( theTestWord ) + "?",
		  helpID( "none", theSubgroup.getParent() ), 
		  Text( "SubgroupContainment" )
		);

  pv.startItemGroup();
  
  //pv.add("For the use of this problem", ThisARCSlotID(), 50);

  pv.add(Text("For the use of this problem"), subgroupContain.arcSlotID(), 50);

  pv.add( Text("Normal closure approximation method") + Name(theSubgroup),
	  normalClosure.arcSlotID(),
	  50
	  );

  pv.add( Text("Compute abelian invariants of") + Name(theSubgroup),
	  abelianInvariants.arcSlotID(),
	  15
	  );

  pv.startItemGroup(CheckinType(Parent(theTestWord))==SMFPGroup::NILPOTENT
		 || CheckinType(Parent(theTestWord))==SMFPGroup::FREE_NILPOTENT
		 || CheckinType(Parent(theTestWord))==SMFPGroup::FP
  );

  SMFPGroup& G = theTestWord.getParent();
  int upperBound;
  int currentClass = 2;

  upperBound = getNilpotencyUpperBound( G );

  while( currentClass <= upperBound && 
	 theSubgroup.sic.haveSGNilpotentQuotInited( currentClass ) )
    currentClass++;     

  if( currentClass <= upperBound ) {
    
    pv.add( Text("Compute lower central quotients for") + Name( theSubgroup ),
	    sgNilpotentQuotients.arcSlotID(),
	    50
	    );
  }

  pv.add( Text("Solve using lower central quotients of") + Name(theSubgroup),
	  scpInNilpotentQuotients.arcSlotID(),
	  50
	  );
  
  pv.startItemGroup();

  pv.add( Text("Enumerate cosets by using Todd-Coxeter procedure"),
	  toddCoxeter.arcSlotID(),
	  40
	  );

  pv.add( Text("Method that uses Todd-Coxeter procedure"),
	  wordRepCM.arcSlotID(),
	  40
	  );

  pv.done();
}


// ============================= IsSGNormal ============================== //

IsSGNormal::IsSGNormal( SMSubgroup& subgroup )
  : Supervisor( true ),
    theSubgroup( subgroup ),
    theTestWords( makeTestWords(subgroup) ),
    isNormal( dontknow ),
    subgroupContain( *this ),
    normalClosure( *this, subgroup.scm().normalClosure ),
    abelianInvariants( *this, subgroup.scm().abelianInvariantsOfFactor ),
    sgNilpotentQuotients( *this, subgroup.scm().sgNilpotentQuotients ),
    scpInNilpotentQuotients( *this ),
    toddCoxeter( *this, subgroup.scm().sgToddCoxeter ),
    wordRepCM( *this )
{

  if( theSubgroup.sic.isNormal() != dontknow ) {
    resultIsFast();
    return;
  }

  if( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
    const AbelianGroup& A = theSubgroup.sic.getCyclicDecompositionOfFactor();
    SetIterator<Word> I( theTestWords );
    for( ; !I.done(); I.next() ) {
      Word w = I.value();
      if( A.isTrivial( w ) == false ) {
	isNormal = no;
	explanation = " it follows from canonical decomposition";
	resultIsFast();
	return;
      }
    }
  }
  scpInNilpotentQuotients->initialize( theTestWords, &theSubgroup );
}

SetOf<Word> IsSGNormal::makeTestWords( const SMSubgroup& H ) const
{
  FPGroup G = H.getParent().getFPGroup();
  VectorOf<Word> gens = H.getSubgroup().generators();

  SetOf<Word> result;
  Word w;
  for( int ii = 0; ii < gens.length(); ++ii ) {
    for( int j = 0; j < G.numberOfGenerators(); ++j ) {
      w = gens[ii].conjugateBy( Word(Generator(j+1)) );
      result |= w.freelyReduce();
      w = gens[ii].conjugateBy( Word(inv(Generator(j+1))) );
      result |= w.freelyReduce();
    }
  }

  return result;
}

void IsSGNormal::takeControl( )
{
  if( theSubgroup.sic.isNormal() != dontknow ) {
    // Send message to front end about found results and quit.
    LogMessage msg(*this, theSubgroup);
    msg << "The subgroup " << Name( theSubgroup ) << " is "
	<< ( theSubgroup.sic.isNormal() ? "" : "not " ) << "normal.";
    msg.send();
    adminTerminate();
  }
  
  if( isNormal == dontknow ) {

    // Check abelian computation managers.
    if( theSubgroup.sic.haveCyclicDecompositionOfFactor() ) {
      const AbelianGroup& A=theSubgroup.sic.getCyclicDecompositionOfFactor();
      SetIterator<Word> I( theTestWords );
      for( ; !I.done(); I.next() ) {
	Word w = I.value();
	if( A.isTrivial( w ) == false ) {
	  isNormal = no;
	  explanation = " it follows from canonical decomposition";
	  break;
	}
      }
    }
    
    // Check nilpotent computation managers.
    if( scpInNilpotentQuotients->state() == ComputationManager::TERMINATED ){
      int retClass;
      Trichotomy theAnswer = scpInNilpotentQuotients->contains(retClass);
      
      if( theAnswer == no ){
	isNormal = no;
	ostrstream msgTmp;
	msgTmp << " it follows from inspection of lower central quotient " 
	       << "of class " << retClass << ends;
	explanation = msgTmp.str();
	msgTmp.freeze(0);
      }
    }
    
    // Check Todd-Coxeter computation managers.
    if( wordRepCM->state() == ComputationManager::TERMINATED ) {
      
      if( !wordRepCM->isInfinite() ) {

	VectorOf<Word> reps = wordRepCM->getWordsReps();
	isNormal = yes;
	for( int ii = 0; ii < reps.length(); ++ii ) {
	  if( reps[ii].length() != 0 ) {
	    isNormal = no;
	    break;
	  }
	}
	explanation = 
	  "it follows from inspection of Schreier representatives system";
      }
    }

    if( subgroupContain->state() == ComputationManager::TERMINATED ) {
      explanation = "according to a normal closure approximation";
      isNormal = subgroupContain->contains();
    }
  }
  
  if( isNormal != dontknow ) {
    
    ((SMSubgroup&)theSubgroup).sic.putIsNormal( isNormal );
    // Send message to front end about found results and quit.
    LogMessage msg(*this, theSubgroup);
    msg << "The subgroup " << Name( theSubgroup ) << " is "
	<< ( isNormal ? "" : "not " ) << "normal " << explanation << ".";
    msg.send();
    adminTerminate();
  }
  
  
}

void IsSGNormal::start( )
{
}

void IsSGNormal::terminate( )
{
}


void IsSGNormal::viewStructure(ostream& ostr) const
{

  ProblemView pv( ostr, oid(), Text("Is normal problem"),
		  Text("Is") + Name( theSubgroup ) + "normal ?",
		  helpID( "none", theSubgroup.getParent() ), 
		  Text( "IsNormal" )
		);

  pv.startItemGroup();
  
  //pv.add("For the use of this problem", ThisARCSlotID(), 50);

  pv.add(Text("For the use of this problem"), subgroupContain.arcSlotID(), 50);

  pv.add( Text("Normal closure approximation method") + Name(theSubgroup),
	  normalClosure.arcSlotID(),
	  50
	  );

  pv.add( Text("Compute abelian invariants of") + Name(theSubgroup),
	  abelianInvariants.arcSlotID(),
	  15
	  );

  pv.startItemGroup(CheckinType(Parent(theSubgroup))==SMFPGroup::NILPOTENT
		 || CheckinType(Parent(theSubgroup))==SMFPGroup::FREE_NILPOTENT
		 || CheckinType(Parent(theSubgroup))==SMFPGroup::FP
  );

  SMFPGroup& G = theSubgroup.getParent();
  int upperBound;
  int currentClass = 2;

  upperBound = getNilpotencyUpperBound( G );

  while( currentClass <= upperBound && 
	 theSubgroup.sic.haveSGNilpotentQuotInited( currentClass ) )
    currentClass++;     

  if( currentClass <= upperBound ) {
    
    pv.add( Text("Compute lower central quotients for") + Name( theSubgroup ),
	    sgNilpotentQuotients.arcSlotID(),
	    50
	    );
  }

  pv.add( Text("Solve using lower central quotients of") + Name(theSubgroup),
	  scpInNilpotentQuotients.arcSlotID(),
	  50
	  );
  
  pv.startItemGroup();

  pv.add( Text("Enumerate cosets by using Todd-Coxeter procedure"),
	  toddCoxeter.arcSlotID(),
	  40
	  );

  pv.add( Text("Method that uses Todd-Coxeter procedure"),
	  wordRepCM.arcSlotID(),
	  40
	  );

  pv.done();
}

#line 1  "src/SGNilpotentQuotients.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes 
//            
//            
//            

// Principal Author: Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//


#include "SGNilpotentQuotients.h"
//#include "CommutatorIterator.h"
#include "SMFPGroup.h"
#include "SMSubgroup.h"
#include "SMWord.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "conversions.h"

// ---------------------- SCPNilpotentARCer --------------------------- //
SCPNilpotentARCer::SCPNilpotentARCer( ComputationManager& boss ) 
  : ARCer( boss ), 
    theSubgroup(0), 
    theAnswer( dontknow ) 
{ 
} 

SCPNilpotentARCer::~SCPNilpotentARCer( ) 
{
  delete theSubgroup; 
} 


void SCPNilpotentARCer::setArguments( const SGOfNilpotentGroup& subgroup,
				      const SetOf<Word>& testWords )
{
  if( theSubgroup )
    delete theSubgroup;
  theSubgroup = new SGOfNilpotentGroup(subgroup);
  theTestWords = testWords;
}


void SCPNilpotentARCer::runComputation( )
{
#if SAFETY > 0
  if( theSubgroup == 0 )
    error( " NilpotentSCPARCer::runComputation() : "
	   "The subgroup containment problem has not been started yet");
#endif

  if( !theSubgroup->preimageIsInitialized() )
    error("theSubgroup.preimageIsInitialized() is false.");

  SetIterator<Word> I( theTestWords );

  for( I.reset(); !I.done(); I.next() ) {
    if( !theSubgroup->contains(I.value()) ) {
      theAnswer = false;
      return;
    }
  }

  theAnswer = true;
}


void SCPNilpotentARCer::writeResults( ostream& out ) 
{
  out < theAnswer;
}


void SCPNilpotentARCer::readResults( istream& in )
{
  in > theAnswer;
}


Trichotomy SCPNilpotentARCer::contains() const
{
#if SAFETY > 0
  if( theSubgroup == 0 )
    error( "bool SCPNilpotentSCPARCer::contains() const : "
	   "The containment problem has not been computed yet");
#endif
  return theAnswer;
}


//---------------------------------------------------------------------------//
//---------------------------- NilpotentSCPInQuotients -----------------------//
//---------------------------------------------------------------------------//


SCPinNilpotentQuotients::SCPinNilpotentQuotients( Supervisor& boss )
: 
  theSubgroup( 0 ),
  theAnswer( dontknow ),
  currentClass(2),
  upperBound( -1 ), // delayed initialization
  arcer( *this ),
  arcerStarted( false )
{

}

int getNilpotencyUpperBound( const SMFPGroup& G )
{
  int upperBound = 10;
  if( G.gic.isNilpotent() == yes || G.gic.isFreeNilpotent() == yes )
    upperBound = G.gic.getNilpotentcyClass() - 1;
  return upperBound;
}


void SCPinNilpotentQuotients::initialize( const SetOf<Word>& testWords, 
					  const SMSubgroup* subgroup, 
					  int startClass)
{
  theTestWords = testWords;
  theSubgroup = subgroup;
  currentClass = startClass;
  upperBound = getNilpotencyUpperBound( subgroup->getParent() );

  adminStart();
}

Trichotomy SCPinNilpotentQuotients::contains( int& nClass ) const
{
  if( theAnswer != dontknow )
    nClass = currentClass;
  return theAnswer;
}

void SCPinNilpotentQuotients::takeControl( )
{
  if( freeARCs() > 0 ) {

    if( currentClass > upperBound ){
      adminTerminate();
      return;
    }   
    if( !arcerStarted 
	&& theSubgroup->sic.haveSGNilpotentQuotInited(currentClass) ) {
      arcer.setArguments( 
        theSubgroup->sic.getSGNilpotentQuotInited(currentClass), theTestWords
      );
      arcerStarted = true;
    }
    
    if( arcerStarted ) {
      if( arcer.takeControl() ){
	if( !arcer.contains() ){
	  theAnswer = no;
	  adminTerminate();
	  return;
	}
	currentClass++;
	arcerStarted = false;
      }
      usedOneARC();
    }

  } // if( freeARCs() > 0 )
}


// ---------------------- SGofNGinitPreimageARCer ------------------------- //

SGofNGinitPreimageARCer::SGofNGinitPreimageARCer( ComputationManager& boss )
  : ARCer( boss ), theSubgroup( 0 ) 
{ 
} 

SGofNGinitPreimageARCer::~SGofNGinitPreimageARCer( )
{
  if (theSubgroup) 
    delete theSubgroup; 
} 


void SGofNGinitPreimageARCer::setArguments( const NilpotentGroup& NG, 
					    const VectorOf<Word>& gens )
{
  if( theSubgroup )
    delete theSubgroup;
  // This ARCer can be called for several times, so we have to delete
  // init, if it exists.

  theSubgroup =  new SGOfNilpotentGroup( NG, gens );
}


void SGofNGinitPreimageARCer::runComputation( )
{
#if SAFETY > 0
  if( theSubgroup == 0 )
    error( "void SGofNGinitPreimageARCer::runComputation( ): "
	   "The subgroup in quotient has not been initialized");
#endif
  //  if( !theSubgroup->preimageIsInitialized() )
  theSubgroup->initPreimage();
}


void SGofNGinitPreimageARCer::writeResults( ostream& out ) 
{
  out < *theSubgroup;
}


void SGofNGinitPreimageARCer::readResults( istream& in )
{
  in > *theSubgroup;
}


const SGOfNilpotentGroup& SGofNGinitPreimageARCer::getSubgroup() const
{
#if SAFETY > 0
  if( theSubgroup == 0 )
    error( "SGOfNilpotentGroup& SGofNGinitPreimageARCer::getGroup() : "
	   "The quotient has not been computed yet");
#endif
  
  return *theSubgroup;
}


//---------------------------------------------------------------------------//
//------------------------ SGNilpotentQuotients -----------------------------//
//---------------------------------------------------------------------------//


SGNilpotentQuotients::SGNilpotentQuotients( class SCM& scm, bool internal )
  : ComputationManager( !internal ),
    theSCM( scm ),
    theSubgroup( scm.getSubgroup() ),
    arcer(*this),
    currentClass( 2 )
{
  upperBound = getNilpotencyUpperBound( theSubgroup.getParent() );
  
  if ( increaseCurrentClass() )
    setNewSubgroup();
  // Looking for computed quotients.

  adminStart();
}

void SGNilpotentQuotients::setNewSubgroup( ) 
{
  SMFPGroup& theGroup = theSubgroup.getParent();
  if( theGroup.gic.haveNilpotentQuotInited( currentClass ) ) {
    arcer.setArguments( theGroup.gic.getNilpotentQuotInited( currentClass ),
			theSCM.getSubgroup().getSubgroup().generators());
  }
  else {
    const FPGroup& G = theGroup.getFPGroup();
    NilpotentGroup NG( G.namesOfGenerators(), 
		       currentClass, //theGroup.gic.getNilpotentcyClass(),
		       makeVectorOf( G.getRelators() ) );
    
    arcer.setArguments( NG, theSubgroup.getSubgroup().generators() );
  }
}

SGNilpotentQuotients::~SGNilpotentQuotients( )
{
}


void SGNilpotentQuotients::takeControl( )
{
  if( freeARCs() > 0 ) {
    if( currentClass > upperBound ){
      adminTerminate();
      return;
    }
    
    if( arcer.takeControl() ) {
      theSubgroup.sic.putHaveSGNilpotentQuotInited( arcer.getSubgroup(), 
						    currentClass );
      LogMessage msg( *this,theSubgroup );
      msg << "Computed the subgroup " 
	  << " in the nilpotent quotient of class " << currentClass
	  << " of " << Name( theSubgroup.getParent() ) << ".";
      msg.send();     

      currentClass++;
      if( !increaseCurrentClass() ){
	adminTerminate();
	return;
      }

      // Compute preimage for next nilpotency class.
      setNewSubgroup();
    } 
    usedOneARC();
  }
}


void SGNilpotentQuotients::start( )
{

}


void SGNilpotentQuotients::terminate( ) { }

bool SGNilpotentQuotients::increaseCurrentClass()
{
  while( currentClass <= upperBound && 
	 theSubgroup.sic.haveSGNilpotentQuotInited( currentClass ) )
    currentClass++;     
  return !(currentClass > upperBound);
}

#line 1  "src/ACConjecture.C"
/*
 *   $Id: 
 */
 
// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes
//           ACConjectureARCer, ACConjectureProblem
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//

#include "ACConjecture.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "conversions.h"


// -------------------------- ACConjectureARCer ---------------------- //


ACConjectureARCer::ACConjectureARCer( ComputationManager& boss )
  : ARCer( boss )
{ } 

void ACConjectureARCer::setArguments( FPGroup fp )
{
  theGroup  =  fp;
}

void ACConjectureARCer::runComputation( ) 
{
  /*
  ACConfig( int pop = 50, int gen = MAXINT, int fit_scale = 1, 
	    int cross_prob = 70, int mut_prob = 85, int elit = true, 
	    int penalty = 0, int cross_type = 1,
	    int mut_app_prob = 30, int mut_insert_prob = 20,
	    int mut_delete_prob = 20,
	    int mut_change_prob = 10,
	    int mut_permute_prob = 20,
	    int use_sum_fit = 1,
	    int num_min = 1000,
	    int us_w = 1
	    );
  */
  ACConfig c( 50, 10000000, 1, 
	      85, 95, true, 
	      0,1,
	      70, 5,
	      10,
	      15,
	      0,
	      1,
	      5000,
	      1
	      );


  ACGA acga(theGroup,c);
  result = acga.transform( compFile, resultFile );
  

}

void  ACConjectureARCer::writeResults( ostream& out )
{
  out < result;
}

void ACConjectureARCer::readResults( istream& in )
{
  in > result;
}

// -------------------- ACConjectureProblem --------------------------- //


ACConjectureProblem::ACConjectureProblem( const SMVectorOfWords& t)
    : Supervisor( true ), 
      theTuple( t ), 
      arcer ( *this )
{
  if (t.getWords().length() != t.getParent().getFreePreimage().numberOfGenerators() ){
    Message msg("Warning");
    msg << "Number of words in the tuple must be equal to the number of the generators!";
    msg.send();
    resultIsFast();
  }  else{
    SetOf<Word> s = makeSetOf(theTuple.getWords());
    FPGroup g(theTuple.getParent().getFreePreimage().namesOfGenerators(),
	      s);
    arcer.setArguments( g );
  }
}

void ACConjectureProblem::start( )
{
  
  LogMessage msg( *this, theTuple );
  msg << Link("Click here to obtain details of the genetic algorithm.",
	      "ACConjecture", arcer.getComputationFileName(), true);
  msg.send();
  
  LogMessage msg1( *this, theTuple );
  msg1 << Link("Click here to see the fittest solutions of the genetic algorithm.",
	       "ACConjecture", 
	       arcer.getResultFileName(), true);
  msg1.send();
  
}

void ACConjectureProblem::takeControl( )
{

  if ( freeARCs() > 0 ) {
    if ( arcer.takeControl() ) {
      
      /*     LogMessage msg( *this, theTuple );
      msg << Name( theTuple );
      if ( arcer.getResult()  )
	msg << " satisfies ";
      else
	msg << " does not satisfy ";
      msg << " the Andrews-Curtis conjecture.";
      msg.send();
      */
      adminTerminate();
      return;
    }
    usedOneARC();
  }
}


void ACConjectureProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find Andrews-Curtis reduction"),
		  Text("Find Andrews-Curtis reduction of ") + 
		  Name( theTuple ),
		  helpID("ACConjectureProblem", theTuple.getParent()),
		  "AC"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of the problem"), ThisARCSlotID(), 1000);

  pv.done();
}


#line 1  "src/HToddCoxeter.C"
/*
 *   $Id:
 */
 
// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class HToddCoxeterARCer HToddCoxeter,
//           HSGIndexToddCoxeter

//
// Principal Author: Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//


#include "HToddCoxeter.h"
#include "OutMessages.h"
#include "SMFPGroup.h"
#include "SMSubgroup.h"
#include "CommutatorIterator.h"
#include "viewStructure.h"

// ------------------- HToddCoxeterARCer --------------------- //


void HToddCoxeterARCer::runComputation( )
{
  tc.start();
  
  //  Timer t(delay()*1000);
  while ( !tc.finished() ) {;}

  if ( tc.getState() == SUCCESS )
    success = true;
  
  theIndex = tc. getIndex();

}

void HToddCoxeterARCer::writeResults( ostream& out ) 
{
  out < success;
  out < theIndex;
}

void HToddCoxeterARCer::readResults( istream& in )
{
  in > success;
  in > theIndex;
}

// -------------------------- HToddCoxeter ---------------------------- //


HToddCoxeter::HToddCoxeter(GCM& boss)
  : theBoss( boss ),
    theGroup( boss.getSMFPGroup() )
{
  if (theGroup.gic.isNilpotent()==yes){
    CommutatorIterator I(theGroup.getFPGroup().numberOfGenerators(), 
			 theGroup.gic.getNilpotentcyClass()+1);
    SetOf<Word> newRelators = theGroup.getFPGroup().getRelators();
    for( I.reset(); !I.done(); I.next() ){
      newRelators |= I.value();
    }
    FPGroup g(theGroup.getFPGroup().namesOfGenerators(),
	      newRelators);
    arcer = new HToddCoxeterARCer(*this,g);
  }
  else
    arcer = new HToddCoxeterARCer(*this,theGroup.getFPGroup());

  adminStart();
}


void HToddCoxeter::takeControl( )
{
  if ( freeARCs() > 0 )

    if( arcer->takeControl() ) {
      if ( arcer->isSuccessful() ){
	Chars explanation;
	explanation = "according to the GH Todd-Coxeter algorithm";
	theGroup.gic.putHaveOrder( arcer->getIndex(),
				   explanation );
      } else
	LogMessage("GH Todd-Coxeter terminated because it has exhausted the memory limits.").send();

      adminTerminate();
    }
    else
      usedOneARC();
}


// -------------------------- HSGIndexToddCoxeter ------------------------- //

HSGIndexToddCoxeter::HSGIndexToddCoxeter(SCM& scm)
  : theSCM( scm ),
    theSubgroup( scm.getSubgroup() ),
    arcer(*this,scm.getSubgroup().getParent().getFPGroup(),
	  scm.getSubgroup().getSubgroup().generators())
  
{
  adminStart();
}


void HSGIndexToddCoxeter::takeControl( )
{
  if ( freeARCs() > 0 )
    
   if( arcer.takeControl() ) {
      if ( arcer.isSuccessful() ){
	Chars explanation;
	explanation = "according to the GH Todd-Coxeter algorithm";
	theSubgroup.sic.putIndex( arcer.getIndex() );
      } else
	LogMessage("GH Todd-Coxeter terminated because it has exhausted the memory limits.").send();
      
      adminTerminate();
    }
    else
      usedOneARC();
}
#line 1  "src/WhiteheadMinimal.C"
/*
 *   $Id:
 */
 
// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes
//        
// Principal Author: Alexei Miasnikov
//
// Status: in progress
//
// Revision History:
//

#include "WhiteheadMinimal.h"
#include "OutMessages.h"
#include "viewStructure.h"



// -------------------------- GAFindWhiteheadMinimalArcer ---------------------- //


GAFindWhiteheadMinimalArcer::GAFindWhiteheadMinimalArcer( ComputationManager& boss )
  : ARCer( boss )
{ } 

void GAFindWhiteheadMinimalArcer::setArguments( FreeGroup f,
						const VectorOf<Word>& v )
{
  theGroup  =  f;
  theTuple = v;
}

void GAFindWhiteheadMinimalArcer::runComputation( ) 
{
  ACConfig c( 50, 10000, 1, 
	      85, 95, 1, 
	      0, 1,
	      75, 5,15,5,0,0
	      );


 GAIsPartOfBasis wh_minimal(theGroup,c,theTuple);
 
 wh_minimal.isPartOfBasis( compFile,resultFile.getFileName() );

 theAuto = wh_minimal.getAutomorphism();

}

void  GAFindWhiteheadMinimalArcer::writeResults( ostream& out )
{
  out < theAuto;
}

void GAFindWhiteheadMinimalArcer::readResults( istream& in )
{
  in > theAuto;
}


// -------------------- FindWhiteheadMinimalProblem --------------------------- //


FindWhiteheadMinimalProblem::FindWhiteheadMinimalProblem(const SMVectorOfWords& w)
    : Supervisor( true ), 
      theTuple( w ), 
      arcer( *this ),
      linkHasBeenSent( false )
{ 
  arcer.setArguments( w.getParent().getFreePreimage(),
		      w.getWords());
}

void FindWhiteheadMinimalProblem::terminate( )
{
  LogMessage msg( *this, theTuple );
  msg << Link("Click here to see the best solution found by the genetic algorithm.",
	      "WhiteheadMinimal", 
	      arcer.getResultFileName());
  msg.send();
}
void FindWhiteheadMinimalProblem::takeControl( )
{
  
    if ( freeARCs() > 0 ) {
      
      if( !linkHasBeenSent ) {
	
	linkHasBeenSent = true;
	
	LogMessage msg( *this, theTuple );
	msg << Link("Click here to see details of the genetic algorithm.",
		    "WhiteheadMinimal", 
		    arcer.getComputationFileName(), true);
	msg.send();
	
      }
      
      if( arcer.takeControl() ) {
	
	adminTerminate();
	return;
      }
    else
      usedOneARC();
    }
}


void FindWhiteheadMinimalProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Find a Whitehead minimal"),
		  Text("Find a Whitehead minimal of ") + Name( theTuple )
		  +  + "?",
		  helpID("FindWhiteheadMinimalProblem", theTuple.getParent()),
		  "Wh min"
		  );

  pv.startItemGroup();

  // pv.add("For the use of the Whitehead algorithm", freeIsPartOfBasis.arcSlotID(), 100);
  pv.add(Text("For the use of the genetic algorithm"), ThisARCSlotID(), 100);

  pv.done();
}
#line 1  "src/FNWP.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes FNWPArcer, FNWPCM
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "FNWP.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "PowerSeriesWP.h"


// ----------------------------- FNWPArcer --------------------------------- //


FNWPArcer::FNWPArcer( ComputationManager& boss )
  : ARCer( boss ), retValue(dontknow)
{ } 

void FNWPArcer::setArguments( const FreeGroup& group,
			      int nClass,
			      const SetOf<Word>& words )
{
  theGroup = group;
  c = nClass;
  theWords = words;
}

void FNWPArcer::runComputation( ) 
{
  PowerSeriesWP pswp(theGroup,c);
  SetIterator<Word> I(theWords);
  for(; !I.done(); I.next() )
    if( !pswp.isTrivial(I.value()) ) {
      retValue = no;
      return;
    }
  retValue = yes;
}

void FNWPArcer::writeResults( ostream& out )
{
  out < retValue;
}

void FNWPArcer::readResults( istream& in )
{
  in > retValue;
}


// ------------------------------- FNWPCM ---------------------------------- //


FNWPCM::FNWPCM( Supervisor& boss )
: theBoss( boss ),
  tAreTrivial( dontknow ),
  bStarted( false ),
  bInited( false ),
  arcer( *this )
{
  adminStart();
}

void FNWPCM::terminate( ) { }

void FNWPCM::start( ) { }

void FNWPCM::init( const FreeGroup& group, 
		   int nClass, 
		   const SetOf<Word> words )
{
  theGroup = group;
  c = nClass;
  theWords = words;
  bInited = true;
}

void FNWPCM::takeControl( )
{
  if ( freeARCs() > 0 ) {
    if( !bStarted ) {

    #if SAFETY > 0
      if( !bInited )
	error("void FNWPCM::takeControl( ) : "
	      "The computation manager was not initialized.");
    #endif  
    
      arcer.setArguments( theGroup, c, theWords );
      bStarted = true;
    }
    if( arcer.takeControl() ) {
      tAreTrivial = arcer.getRetValue();
      adminTerminate();
      return;
    }
    else
      usedOneARC();
  }
}
#line 1  "src/EquationsInFPProblem.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of the EqSystemInAbelianARCe, EqSystemInAbelianCM,
//           EqSystemInFPProblem, EquationInAbelianCM, EquationInFPProblem 
//           classes.
//
// Principal Authors: Denis Serbin
//
// Status: In progress
//
// Revision History:
//
// Special Notes:
//
//

#include "EquationsInFPProblem.h"
#include "SMFPGroup.h"

//--------------------------EqSystemInAbelianARCer---------------------------//

void EqSystemInAbelianARCer::setArguments( const AbelianEquationsSolver& a )
{
  AES = a;
}
  
void EqSystemInAbelianARCer::runComputation( )
{
  AES.findSolutions(file);
  
  retValue = AES.haveSolutions();
}
 
void EqSystemInAbelianARCer::writeResults( ostream& out )
{
  out < retValue;
}

void EqSystemInAbelianARCer::readResults( istream& in )
{
  in > retValue;
}

//---------------------------EqSystemInAbelianCM-----------------------------//

EqSystemInAbelianCM::EqSystemInAbelianCM( class EqSystemInFPProblem& p )
  : theSystem( p.getSystem() ),
    answer( 0 ),
    arcer( *this )
{
  int numOfVar = theSystem.numberOfVariables();
  
  SetOf<Word> S = theSystem.getParent().getFPGroup().getRelators();
  SetIterator<Word> I(S);
  SetOf<Word> newS;
  
  while( !I.done() )
    {
      Word w = I.value();
      for( int j = 0 ; j < w.length() ; j++ )
	{
	  int p = Generator( w[j] ).hash();
	  if( p > 0 )
	    w[j] = Generator( p + numOfVar );
	  else
		w[j] = Generator( p - numOfVar );
	}
      
      newS.adjoinElement( w );
	  I.next();
    }
  
  FPGroup G( theSystem.getFreeGroup().namesOfGenerators() , newS );
  
  AbelianGroup A( G );
  
  VectorOf<Word> v = theSystem.getSystem();
  
  arcer.setArguments( AbelianEquationsSolver( A , v , numOfVar ) ); 

  adminStart();
}

void EqSystemInAbelianCM::takeControl( )
{
  if(freeARCs() > 0)
    if(arcer.takeControl())
      {
	theSystem.seic.putIsSolvedInAbelianization();
	theSystem.seic.putHaveSolutionsInAbelianization( arcer.getRetValue() );
	
	if( theSystem.getParent().getCheckinType( ) == SMFPGroup::ABELIAN )
	  {
	    theSystem.seic.putIsSolved();
	    theSystem.seic.putHaveSolutions( arcer.getRetValue() );
	  }
      }
    else
      usedOneARC();
}

//-------------------------EqSystemInFPProblem--------------------------//

EqSystemInFPProblem::EqSystemInFPProblem( SMEqSystem& s )
  : Supervisor( true ), 
    theSystem( s ),
    abelianCM( *this )
{
  if( theSystem.getParent().getCheckinType( ) == SMFPGroup::ABELIAN && 
      theSystem.seic.isSolvedInAbelianization() )
    resultIsFast();
}

void EqSystemInFPProblem::takeControl( )
{
  if( theSystem.seic.isSolvedInAbelianization() )
    {
      LogMessage msg1( *this, theSystem );
      
      if( theSystem.seic.haveSolutionsInAbelianization() )
	{
	  if( theSystem.getParent().getCheckinType( ) == SMFPGroup::ABELIAN )
	    {
	      msg1 << Name( theSystem );
	      msg1 << " has solutions.";
	      msg1.send();
	      
	      LogMessage msg2( *this, theSystem );
	      
	      msg2 << Link("Click here to see the solutions of the system" , "AbEqSysSol" , abelianCM->getFileName() );
	      msg2.send();
	    }
	  else
	    {
	      msg1 << "The algorithm can't give an exact answer.";
	      msg1.send();
	    }
	  
	  adminTerminate();
	  return;
	}
      else
	{
	  if( theSystem.getParent().getCheckinType( ) == SMFPGroup::ABELIAN )
	    {
	      msg1 << Name( theSystem );
	      msg1 << " has no solutions.";
	      msg1.send();
	      
	      LogMessage msg3( *this, theSystem );
	      
	      msg3 << Link("Click here to see the details" , "AbEqSysSol" , abelianCM->getFileName() );
	      msg3.send();
	    }
	  else
	    {
	      msg1 << Name( theSystem );
	      msg1 << " has no solutions because it cannot be solved in abelianization of ";
	      msg1 << Name( theSystem.getParent() ) << " .";
	      msg1.send();
	    }
	  
	  adminTerminate();
	  return;
	}
    }
}

void EqSystemInFPProblem::viewStructure( ostream& ostr ) const
{
  ProblemView pv( ostr, oid(), Text("Finding solutions for the system of equations"),
		  Text("Finding solutions of") + 
		  Name( theSystem ) ,
		  helpID("EquationsInAbelianProblem",theSystem.getParent()),
		  Text("solve") + Name(theSystem)
		  );
  
  pv.startItemGroup();
  
  if( theSystem.getParent().getCheckinType( ) == SMFPGroup::ABELIAN )
    pv.add(Text("For the use of the problem") , abelianCM.arcSlotID() , 50 );
  else
    pv.add(Text("Find solutions in abelianization") , abelianCM.arcSlotID() , 50 );
  
  pv.done();
}

//---------------------------EquationInAbelianCM-----------------------------//

EquationInAbelianCM::EquationInAbelianCM( class EquationInFPProblem& p )
  : theEquation( p.getEquation() ),
    answer( 0 ),
    arcer( *this )
{
  int numOfVar = theEquation.numberOfVariables();
  
  SetOf<Word> S = theEquation.getParent().getFPGroup().getRelators();
  SetIterator<Word> I(S);
  SetOf<Word> newS;
  
  while( !I.done() )
    {
      Word w = I.value();
      for( int j = 0 ; j < w.length() ; j++ )
	{
	  int p = Generator( w[j] ).hash();
	  if( p > 0 )
	    w[j] = Generator( p + numOfVar );
	  else
	    w[j] = Generator( p - numOfVar );
	}
      
      newS.adjoinElement( w );
      I.next();
    }
  
  FPGroup G( theEquation.getFreeGroup().namesOfGenerators() , newS );
  
  AbelianGroup A( G );
  
  VectorOf<Word> v;
  v.append( theEquation.getWord() );
  
  arcer.setArguments( AbelianEquationsSolver( A , v , numOfVar ) ); 

  adminStart();
}

void EquationInAbelianCM::takeControl( )
{
  if(freeARCs() > 0)
    if(arcer.takeControl())
      {
	theEquation.eic.putIsSolvedInAbelianization();
	theEquation.eic.putHaveSolutionsInAbelianization( arcer.getRetValue() );
      }
    else
      usedOneARC();
}

//---------------------------EquationInFPProblem-----------------------------//

EquationInFPProblem::EquationInFPProblem( SMEquation2& e )
  : Supervisor( true ), 
    theEquation( e ),
    abelianCM( *this )
{
  if( theEquation.getParent().getCheckinType( ) == SMFPGroup::ABELIAN && 
      theEquation.eic.isSolvedInAbelianization() )
    resultIsFast();
}

void EquationInFPProblem::takeControl( )
{
  if( theEquation.eic.isSolvedInAbelianization() )
    {
      LogMessage msg1( *this, theEquation );
      
      if( theEquation.eic.haveSolutionsInAbelianization() )
	{
	  if( theEquation.getParent().getCheckinType( ) == SMFPGroup::ABELIAN )
	    {
	      msg1 << Name( theEquation );
	      msg1 << " has solutions.";
	      msg1.send();
	      
	      LogMessage msg2( *this, theEquation );
	      
	      msg2 << Link("Click here to see solutions of the equation" , "AbEqSol" , abelianCM->getFileName() );
	      msg2.send();
	    }
	  else
	    {
	      msg1 << "The algorithm can't give an exact answer.";
	      msg1.send();
	    }
	  
	  adminTerminate();
	  return;
	}
      else
	{
	  if( theEquation.getParent().getCheckinType( ) == SMFPGroup::ABELIAN )
	    {
	      msg1 << Name( theEquation );
	      msg1 << " has no solutions.";
	      msg1.send();
	      
	      LogMessage msg3( *this, theEquation );
	      
	      msg3 << Link("Click here to see the details" , "AbEqSol" , abelianCM->getFileName() );
	      msg3.send();
	    }
	  else
	    {
	      msg1 << Name( theEquation );
	      msg1 << " has no solutions because it cannot be solved in abelianization of ";
	      msg1 << Name( theEquation.getParent() ) << " .";
	      msg1.send();
	    }
	  
	  adminTerminate();
	  return;
	}
    }
}

void EquationInFPProblem::viewStructure( ostream& ostr ) const
{
  ProblemView pv( ostr, oid(), Text("Finding solutions for the equation"),
		  Text("Finding solutions of") + 
		  Name( theEquation ) ,
		  helpID("EquationInAbelianProblem",theEquation.getParent()),
		  Text("solve") + Name( theEquation )
		  );
  
  pv.startItemGroup();
  
  if( theEquation.getParent().getCheckinType( ) == SMFPGroup::ABELIAN )
    pv.add(Text("For the use of the problem") , abelianCM.arcSlotID() , 50 );
  else
    pv.add(Text("Find solutions in abelianization") , abelianCM.arcSlotID() , 50 );
  
  pv.done();
}
#line 1  "src/KernelPresentation.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition of the FPNewPresentationARCer, 
//           FPNewPresentationProblem, FPImagePresentationARCer, 
//           FPImagePresentationCM, FPKernelPresentationARCer,
//           FPKernelPresentationCM, KBSupervisorCM,
//           FPKernelPresentationProblem
//           classes.
//
// Principal Authors: Denis Serbin
//
// Status: In progress
//
// Revision History:
//
// Special Notes:
//
//

#include "KernelPresentation.h"
#include "Supervisor.h"
#include "FEData.h"
#include "Word.h"
#include "FPGroup.h"
#include "CosetEnumerator.h"
#include "PresentationProblems.h"
#include "Chars.h"


//---------------------------FPNewPresentationARCer--------------------------//

void FPNewPresentationARCer::setArguments( const KBMachine& kb , 
					   const FPGroup& g , 
					   const VectorOf<Word>& y )
{
  G = g;
  newGens = y;
  N = NewPresentation( g , kb , y );
}

void FPNewPresentationARCer::runComputation( )
{
  CosetEnumerator ce( G , newGens );
  int ind = ce.enumerate();
  
  if( ind != 1 )
    {
      dntg = true;
      return;
    }
  
  retValue = N.getNewPresentation( );
}
 
void FPNewPresentationARCer::writeResults( ostream& out )
{
  out < retValue;
}

void FPNewPresentationARCer::readResults( istream& in )
{
  in > retValue;
}


// -----------------------FPNewPresentationProblem-------------------------- //

FPNewPresentationProblem::FPNewPresentationProblem( class SMVectorOfWords& v )
  : Supervisor( true ), 
    theVector( v ),
    init( false ),
    kbSupervisor( *this , theVector.getParent().gcm().kbSupervisor ),
    arcer( *this )
{ 
  //adminStart();
}

void FPNewPresentationProblem::takeControl( )
{
  if( freeARCs() > 0 && theVector.getParent().gic.haveConfluentKBMachine() )
    {
      if( !init )
	{
	  arcer.setArguments( theVector.getParent().gic.getConfluentKBMachine(),
			      theVector.getParent().getFPGroup(),
			      theVector.getWords() );
	  init = true;
	}
       
      if(arcer.takeControl())
	{
	  LogMessage msg( *this , theVector );
	  
	  if( arcer.doesntGenerate() )
	    msg << Name( theVector ) << " doesn't generate " 
		<< Name( theVector.getParent() ) << " . ";
	  else
	    {
	      /*  
		  msg << " New presentation for " << Name( theVector.getParent() )
		  << " on new generators " << Name( theVector ) << " : ";
		  msg << arcer.getRetValue();
	      */
	      SMFPGroup* smo = new SMFPGroup( arcer.getRetValue() , Text("") + Name( theVector.getParent() ) + Text("presented on generators given by") + Name( theVector ) );
	      
	      ListOf<OID> dependencies( theVector.getParent() );
	      CheckinMessage( *smo, "", dependencies ).send();
	    }
	  
	  msg.send();
	  
	  adminTerminate();
	  return;
	}
      else
	usedOneARC();
    }
}

void FPNewPresentationProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("New presentation problem"),
		  Text("Compute a presentation for") + Name( theVector.getParent() )
		  + Text("on generators given by") + Name( theVector ),
		  helpID("FPNewPresentationProblem", theVector.getParent()),
		  Text("NewPres") + Name( theVector )
		  );  
  
  pv.startItemGroup();
  
  pv.add(Text("Seek a rewriting system for") + Name( theVector.getParent() ),
	 kbSupervisor.arcSlotID(),
	 50
	 );

  pv.add(Text("For the use of this problem"), ThisARCSlotID(), 50);  
  
  pv.done();
}


//-------------------------FPImagePresentationARCer-------------------------//

void FPImagePresentationARCer::setArguments( const FPGroup& preimage, 
					     const FPGroup& image, 
					     const VectorOf<Word>& y )
{
  Im = ImageOfHom( preimage , image , y );
}
  
void FPImagePresentationARCer::runComputation( )
{
  retValue = Im.getImageOfHomo( newImages );
}

void FPImagePresentationARCer::writeResults( ostream& out )
{
  out < retValue < newImages;
}

void FPImagePresentationARCer::readResults( istream& in )
{
  in > retValue;
  in > newImages;
}


//---------------------------FPImagePresentationCM---------------------------//

FPImagePresentationCM::FPImagePresentationCM( class FPKernelPresentationProblem& kpp )
  : ComputationManager( false ), 
    theMap( kpp.getInitialHomo() ),
    G(),
    im(),
    arcer( *this )
{
  arcer.setArguments( theMap.getDomain().getFPGroup(),
		      theMap.getRange().getFPGroup(),
		      theMap.getMap().generatingImages() );  
  adminStart();
}

void FPImagePresentationCM::takeControl( )
{
  if ( freeARCs() > 0 )
    if (arcer.takeControl())
      {
	G = arcer.getRetValue();
	im = arcer.getVectorOfImages();
	
	adminTerminate();
	return;
      }
    else
      usedOneARC();
}


//-------------------------FPKernelPresentationARCer-------------------------//

void FPKernelPresentationARCer::setArguments( const KBMachine& kb , 
					      const FPGroup& preimage ,
					      const FPGroup& image ,
					      const VectorOf<Word>& y )
{
  K = KernelOfHom( preimage , image , kb , y );
}
  
void FPKernelPresentationARCer::runComputation( )
{
  retValue = K.getKernelPresentation( );
}
 
void FPKernelPresentationARCer::writeResults( ostream& out )
{
  out < retValue;
}

void FPKernelPresentationARCer::readResults( istream& in )
{
  in > retValue;
}


//---------------------------FPKernelPresentationCM--------------------------//

FPKernelPresentationCM::FPKernelPresentationCM( class FPKernelPresentationProblem& kpp )
  : ComputationManager( false ), 
    KPP( kpp ),
    result(),
    flag( false ),
    init( false ),
    arcer( *this )
{
  adminStart();
}

void FPKernelPresentationCM::takeControl( )
{
  if( flag )
    {
      if( !init )
	{
	  arcer.setArguments( KPP.getHomo().getRange().gic.getConfluentKBMachine(), 
			      KPP.getHomo().getDomain().getFPGroup(),
			      KPP.getHomo().getRange().getFPGroup(),
			      KPP.getHomo().getMap().generatingImages() );
	  init = true;
	}
      
      if ( freeARCs() > 0 )
	if (arcer.takeControl())
	  {
	    result = arcer.getRetValue();
	    
	    adminTerminate();
	    return;
	  }
	else
	  usedOneARC();
    }
}


//-----------------------------KBSupervisor----------------------------------//

KBSupervisorCM::KBSupervisorCM( class FPKernelPresentationProblem& kpp )
  : ComputationManager( false ), 
    KPP( kpp ),
    flag( false ),
    init( false ),
    arcer( *this )
{
  adminStart();
}

void KBSupervisorCM::takeControl( )
{
  if( flag )
    {
      if( !init )
	{
	  arcer.setArguments( KPP.getHomo().getRange().getFPGroup() );
	  init = true;
	}
      
      if ( freeARCs() > 0 )
	if (arcer.takeControl())
	  {
	    KPP.getHomo().getRange().gic.putHaveConfluentKBMachine( arcer.getKBMachine() );
	    
	    adminTerminate();
	    return;
	  }
	else
	  usedOneARC();
    }
}


// -----------------------FPKernelPresentationProblem----------------------- //

FPKernelPresentationProblem::FPKernelPresentationProblem( class SMHomomorphism2& m )
  : Supervisor( true ), 
    theMap( m ),
    theMap2( 0 ),
    kernel( *this ),
    image( *this ),
    kbs( *this ),
    foundImage( false ),
    foundKB( false ) 
{
  if ( ! displayInFE() ) adminStart();
}
  
void FPKernelPresentationProblem::takeControl( )
{
  if( image->state() == ComputationManager::TERMINATED && !foundImage )
    {
      LogMessage msg1( *this , theMap.getRange() );
	    
      msg1 << "Found the image of " << Name( theMap.getDomain() )
	  << " in " << Name( theMap.getRange() ) << " under " 
	  << Name( theMap ) << " . ";
      msg1.send();
      
      if( image->getImage() == theMap.getRange().getFPGroup() )
	{
	  LogMessage msg2( *this, theMap.getRange() );
	  
	  msg2 << "It coinsides with " << Name( theMap.getRange() ) << " . ";
	  msg2.send();
	
	  theMap2 = &theMap;
	}
      else
	{
	  FPGroup G = image->getImage();
	  VectorOf<Chars> genNames( G.numberOfGenerators() );
	  
	  for( int i1 = 0 ; i1 < genNames.length() ; i1++ )
	    genNames[i1] = Chars( "x" ) + Chars( i1 + 1 );
	  
	  FPGroup newG( genNames , G.getRelators() );
	  
	  SMFPGroup* smo1 = new SMFPGroup( newG , Text("The image of") + Name( theMap.getDomain() ) + " in " + Name( theMap.getRange() ) );
	  
	  ListOf<OID> dependencies1( theMap.getRange() );
	  CheckinMessage( *smo1, "", dependencies1 ).send();
	  
	  Map m( theMap.getDomain().getFPGroup() , image->getImage() ,
		 image->getVectorOfImages() );
	  
	  SMHomomorphism2* smo2 = new SMHomomorphism2( theMap.getDomain(), *smo1, m , Text("The restriction of") + Name( theMap ) + " to " + Name( *smo1 ) );
	  
	  ListOf<OID> dependencies2;
	  dependencies2.insert( theMap.getDomain().oid() , 0 );
	  dependencies2.insert( (*smo1).oid() , 0 );
	  CheckinMessage( *smo2, "", dependencies2 ).send();
	  
	  theMap2 = smo2;
	}
      
      foundImage = true;
      kbs->setFlag( true );
      
      //adminSuspend();
    }
  
  if( foundImage && theMap2->getRange().gic.haveConfluentKBMachine() &&
      !foundKB )
    {
      kernel->setFlag( true );
      foundKB = true;
    }
  
  if( kernel->state() == ComputationManager::TERMINATED )
    {
      /*
	LogMessage msg3( *this , theMap2.getRange() );
	
	msg3 << " Presentation for the kernel of " << Name( theMap2 )
	<< " : " << kernel.getResult();
	
	msg3.send();
      */
      
      SMFPGroup* smo3 = new SMFPGroup( kernel->getResult() , Text("The kernel of") + Name( *theMap2 ) );
      
      ListOf<OID> dependencies3( theMap2->getDomain() );
      CheckinMessage( *smo3, "", dependencies3 ).send();
      
      adminTerminate();
      return;
    }
}

void FPKernelPresentationProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Kernel presentation problem"),
		  Text("Compute a presentation for the kernel of") + Name( theMap ),
		  helpID("FPKernelPresentationProblem", theMap.getDomain()),
		  Text("KernelPresentation") + Name( theMap )
		  );  
  
  pv.startItemGroup();
 
  pv.add( Text("Computing the image of ") + Name( theMap.getDomain() ) , 
	  image.arcSlotID(),
	  50 
	  );
  
  pv.add(Text("Seek a rewriting system for the image of ") + Name( theMap.getDomain() ) + " in " + Name( theMap.getRange() ),
	 kbs.arcSlotID(),
	 50
	 );

  pv.add( Text("Computing the kernel of ") + Name( theMap ) , 
	  kernel.arcSlotID(),
	  50 
	  );
  
  pv.done();
}


  
#line 1  "src/RankOfSubgroup.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes RankOfSubgroup, RankOfSubgroupProblem, RankOfSubgroupARCer
//
// Principal Author: Alexander Ushakov
//
// Status: in progress
//
// Revision History:
//


#include "Set.h"
#include "RankOfSubgroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
//#include "GeneralWhitehead.h"
#include "SMSubgroup.h"

//################################################

ProperSubgroupEnumerator::ProperSubgroupEnumerator(const FreeGroup& F, const VectorOf<Word>& words) :
  theGroup( F ),
  theWords( words ),
  subgroup( 0 ),
  rank( theGroup.rank() )
{
  int t, cur, w;

  genOfGroup = VectorOf<Word>(rank);

  for( t=0 ; t<rank ; ++t ) 
    genOfGroup[t] = Word(Generator(t+1));

  //qVert - number of vertices
  qVert = 1;
  for( t=0 ; t<words.length() ; ++t ) 
    qVert += words[t].length()-1;

  setToVert  = new int[qVert];
  vertToWord = new int[qVert];
  vertToPos  = new int[qVert];

  for( t=0,cur=0,w=0 ; t<qVert ; ++t,++cur ) {
    setToVert[t] = UNKNOWN;
    vertToWord[t] = w;
    vertToPos[t]  = cur;
    if( cur==words[w].length()-1 ) {
      ++w;
      cur = 0;
    }
  }

  partition  = new int[qVert];
  _partition = new int*[qVert];
  for( t=0 ; t<qVert ; ++t ) {
    partition[t] = UNKNOWN;
    _partition[t] = new int[qVert];
  }
  lCells = new LC[qVert];

  nVert = 0;
  nSet = 0;
  qSet = 0;

  qWord = theWords.length();
  //generators of new subgroup
  genOfSG = VectorOf<Word>( qVert+qWord , theWords );
  buildSG( );
}


ProperSubgroupEnumerator::~ProperSubgroupEnumerator()
{
  for( int t=0 ; t<qVert ; ++t ) 
    delete []_partition[t];
  delete []_partition;

  delete []setToVert;
  delete []vertToWord;
  delete []vertToPos;
  delete []lCells;
  if( subgroup )
    delete subgroup;
}


bool ProperSubgroupEnumerator::getNextProperSubgroup(SGofFreeGroup& sg)
{
  while( nVert>=0 ) {

    if( nSet<=qSet ) {
      if( stepTo() ) {
	if( nVert==qVert ) {
  
	  /*
	  static int _t = 0;	  
	  file << _t << ": {";
	  for( int t=0; t<qVert; ++t) 
	    file << partition[t] << " , ";
	  file << "}" << endl;
	  ++_t;
	  */

	  VectorOf<Word> vt = subgroup->nielsenBasis( );
	  sg = SGofFreeGroup( theGroup , vt );

	  stepBack();
	  ++nSet;
	  return true;
	}
      }
    }
    else {
      stepBack();
    }
    ++nSet;
  }
  return false;
}


bool ProperSubgroupEnumerator::stepTo()
{
  int t;

  if( partition[nVert]!=UNKNOWN ) {

    //
    if( nSet>partition[nVert] ) {
      nSet = qSet;
      return false;
    }
    nSet = partition[nVert];

    // debug checkout
    if( setToVert[nSet]==UNKNOWN ) {
      file << "Error !!! " << endl;
      //error
    }

    // remember step
    for( t=0 ; t<qVert ; ++t ) 
      _partition[nVert][t] = partition[t];
    partition[nVert] = nSet;
    
    lCells[nVert].qSet = qSet;
    
    nSet = -1;
    ++nVert;
    return true;  
  }
  else {

    // 
    for( t=0 ; t<qVert ; ++t ) 
      _partition[nVert][t] = partition[t];

    //
    partition[nVert] = nSet;

    // check references to vertice
    for( t=0 ; t<qVert ; ++t) 
      if( partition[t]==nVert )
	partition[t] = nSet;
    
    lCells[nVert].qSet = qSet;
    if( nSet==qSet ) // mem. first vertice of set
      setToVert[qSet++] = nVert;
    else {
      // add new generator
      genOfSG[qWord++] = getWord(setToVert[nSet], nVert);
      buildSG( );
    }
    nSet = -1;
    ++nVert;

    if( !setRefs() || subgroup->contains(genOfGroup) ) {
      stepBack();
      return false;
    }

    return true;
  }
}


void ProperSubgroupEnumerator::stepBack()
{
  int t;
  --nVert;

  if( nVert>=0 ) {

    nSet = partition[nVert];

    for( t=0 ; t<qVert ; ++t ) 
      partition[t] = _partition[nVert][t];

    if( qSet > lCells[nVert].qSet ) 
      setToVert[qSet] = UNKNOWN;
    else 
      if( partition[nVert]==UNKNOWN ) {
	--qWord;
	buildSG( );
      }

    qSet = lCells[nVert].qSet;
  }
}


Word ProperSubgroupEnumerator::getWord(int x, int y)
{
  int t;
  int w1 = vertToWord[x];
  int w2 = vertToWord[y];
  int p1 = vertToPos[x];
  int p2 = vertToPos[y];
  VectorOf<Generator> vect(p1 + theWords[w2].length()-p2 );

  for( t=0 ; t<p1 ; ++t ) 
    vect[t] = theWords[w1][t];
  for( t=0 ; t<theWords[w2].length()-p2 ; ++t )
    vect[p1+t] = theWords[w2][p2+t];

  /*
  file << "getWord(" << x << "," << y << ")" << " -> " ;
  theGroup.printWord(file, Word(vect) );
  file << endl;
  */

  return Word(vect);
}


void ProperSubgroupEnumerator::buildSG( )
{
  if( subgroup )
    delete subgroup;

  subgroup = new SGofFreeGroup( theGroup , VectorOf<Word>(qWord, genOfSG) );
}


bool ProperSubgroupEnumerator::setRefs( )
{
  int t, t1, t2;
  Word w;
  int ns1;
  int ns2;

  for( t1=0 ; t1<qVert; ++t1 ) {
    ns1 = partition[t1];
    if( ns1==UNKNOWN || setToVert[ns1]==t1 ) {

      for( t2=t1+1 ; t2<qVert; ++t2 ) {
	ns2 = partition[t2];
	if( ns2==UNKNOWN || setToVert[ns2]==t2 ) {
	  
	  w = getWord(t1, t2);
	  if( subgroup->contains(w) ) {

	    // 1.	    
	    if( ns1!=UNKNOWN && ns2!=UNKNOWN )
	      return false;
	    
	    // 2.
	    if( ns1!=UNKNOWN ) {
	      partition[t2] = ns1;
	      for( t=t2 ; t<qVert ; ++t )
		if( partition[t]==t2 )
		  partition[t]=ns1;
	      continue;
	    }
	    
	    // 3.
	    if( ns2!=UNKNOWN ) {
	      file << "Waring !!! 'ns2!=UNKNOWN'" << endl;
	      continue;
	    }
	    
	    // 4. 
	    // ns1==UNKNOWN && ns2==UNKNOWN
	    partition[t2] = ns1;
	    for( t=t2 ; t<qVert ; ++t )
	      if( partition[t]==t2 )
		partition[t]=ns1;
	    
	  }
	}
      }
    }
  }

  return true;
}



// -------------------- RankOfSubgroupArcer --------------------------- //


RankOfSubgroupARCer::RankOfSubgroupARCer(ComputationManager& boss, 
		   const class SMFPGroup& group,  
		   const SGofFreeGroup& sg ) :
  ARCer ( boss ),
  theGroup ( group ),
  theSubgroup ( sg ),
  answer ( dontknow ),
  PSE ( new ProperSubgroupEnumerator( theGroup.getFreePreimage( ) , sg.generators() ) )
{
  
}

RankOfSubgroupARCer::~RankOfSubgroupARCer( )
{
  delete PSE;
}

void RankOfSubgroupARCer::runComputation()
{
  lookingup();
}


void RankOfSubgroupARCer::writeResults( ostream& out ) 
{
  out < answer;
}


void RankOfSubgroupARCer::readResults( istream& in )
{
  in > answer;
}


bool RankOfSubgroupARCer::isAFreeFactorIn( SGofFreeGroup& g ) const
  // check whether subgroup 'theSubgroup' is a free factor in subgroup g
  // subgroup g must contains subgroup 'theSubgroup'
{
  // rewrite subgroup g
  const VectorOf<Word> basis = g.nielsenBasis( );
  const SGofFreeGroup  nielsGroup(theGroup.getFreePreimage(), basis);

  // rewrite generators of 'theSubgroup' in a new generators of subgroup g
  int numGen = theSubgroup.generators().length( );
  VectorOf<Word> newGen( numGen );
  for( int t=0 ; t<numGen ; ++t ) 
    newGen[t] = expressWordInSubgroupGenerators(nielsGroup, theSubgroup.generators()[t]);

  FreeGroup      freeGroup( basis.length() );
  SGofFreeGroup sg(freeGroup, newGen);
  return sg.isAFreeFactor( );
}


void RankOfSubgroupARCer::printSG( ostream& file, const SGofFreeGroup& subgroup )
{
  VectorOf<Word> vect(subgroup.generators());

  file << "<< ";
  for( int t=0 ; t<vect.length() ; ++t ) {
    if( t )
      file << " , ";
    theGroup.getFreePreimage().printWord(file, vect[t]);
  }
  file << " >>" << endl;

  file << endl;
}


void RankOfSubgroupARCer::lookingup()
{
  SGofFreeGroup sg( theGroup.getFreePreimage() , VectorOf<Word>() );
  File file( getFileName() );
  SetOf<SGofFreeGroup> subgroups;

  while( PSE->getNextProperSubgroup( sg ) ) {

    if( subgroups.contains(sg) ) 
      continue;
    
    subgroups.adjoinElement(sg);  
    if( isAFreeFactorIn(sg) ) {
      static int _t = 0;
      ++_t;
      file << _t << " :  ";
      printSG( file , sg );
    }

  }

  answer = true;
}


// -------------------- RankOfSubgroupProblem --------------------------- //


RankOfSubgroupProblem::RankOfSubgroupProblem(const SMSubgroup& sg)
    : Supervisor( true ), 
      theSubgroup( sg ), 
      rankOfSubgroup( *this ),
      linkHasBeenSent( false )      
{ 

}
 
const SMSubgroup& RankOfSubgroupProblem::getSubgroup( ) const 
{ 
  return theSubgroup; 
}



void RankOfSubgroupProblem::takeControl( )
{
  if( !linkHasBeenSent ) {
  
    linkHasBeenSent = true;
    
    LogMessage msg( *this, theSubgroup );
    msg << Link( Text("Click here to see the rank of ") + Name( theSubgroup ),
		"RankOfSubgroup", rankOfSubgroup->getFileName(), true);
    msg.send();

  }

  Trichotomy answer = rankOfSubgroup->getAnswer();
  if ( rankOfSubgroup.exists() && answer!=dontknow) {
    
    LogMessage msg( *this, theSubgroup );
    msg.send();
    
    adminTerminate();
    return;
  }

}


void RankOfSubgroupProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Rank"),
		  Text("Compute rank of ") + Name( theSubgroup )
		  + " in "
		  + Name( Parent( theSubgroup ) ) + "?",
		  helpID("RankOfSubgroupProblem", theSubgroup.getParent()),
		  "Rank"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of the algorithm"), rankOfSubgroup.arcSlotID(), 100);

  pv.done();
}



// ------------------------ RankOfSubgroup ------------------------------ //


RankOfSubgroup::RankOfSubgroup(RankOfSubgroupProblem& problemObject)
    : theSubgroup( problemObject.getSubgroup() ),
      theGroup( theSubgroup.getParent().getFreePreimage() ),
      theARCer(*this, problemObject.getSubgroup().getParent(), problemObject.getSubgroup().getSubgroup())
{
  adminStart();
}


RankOfSubgroup::~RankOfSubgroup( )
{

}


void RankOfSubgroup::takeControl( )
{

  if ( freeARCs() > 0 ) {

    if ( theARCer.takeControl() ) {

      adminTerminate();
      return;
    }   

    usedOneARC();
  }

}
#line 1  "src/RankOfElt.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes RankOfElement, RankOfElementProblem
//
// Principal Author: Alexander Ushakov
//
// Status: in progress
//
// Revision History:
//

#include "Set.h"
#include "RankOfElt.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "SMSubgroup.h"



// -------------------- RankOfElementProblem --------------------------- //


RankOfElementProblem::RankOfElementProblem(const SMWord& w )
    : Supervisor( true ), 
      theWord( w ), 
      rankOfElement( *this ),
      linkHasBeenSent( false )      
{ 

}

 
const SMWord& RankOfElementProblem::getWord( ) const 
{ 
  return theWord; 
}


void RankOfElementProblem::takeControl( )
{
  if( !linkHasBeenSent ) {
  
    linkHasBeenSent = true;
    
    LogMessage msg( *this, theWord );
    msg << Link( Text("Click here to see the rank of ") + Name( theWord ),
		"RankOfSubgroup", rankOfElement->getFileName(), true);
    msg.send();

  }

  Trichotomy answer = rankOfElement->getAnswer();
  if ( rankOfElement.exists() && answer!=dontknow) {
    
    LogMessage msg( *this, theWord );
    msg.send();
    
    adminTerminate();
    return;
  }

}


void RankOfElementProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Rank"),
		  Text("Rank of ") + Name( theWord ),
		  helpID("RankOfElementProblem", theWord.getParent()),
		  "Rank"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of the algorithm"), rankOfElement.arcSlotID(), 100);

  pv.done();
}


// ------------------------ RankOfElement ------------------------------ //


VectorOf<Word> RankOfElement::constructVector( const Word& word )
{
  VectorOf<Word> result(1);
  result[0] = word;
  return result;
}


RankOfElement::RankOfElement(RankOfElementProblem& problemObject)
  : theWord( problemObject.getWord( ) ),
    theGroup( theWord.getParent().getFreePreimage() ),
    theARCer(*this, theWord.getParent(), SGofFreeGroup(theGroup , constructVector( theWord.getWord() )) )
{
  adminStart();
}


RankOfElement::~RankOfElement( )
{

}


void RankOfElement::takeControl( )
{
  
  if ( freeARCs() > 0 ) {
    
    if ( theARCer.takeControl() ) {
      
      adminTerminate();
      return;
    }   
    
    usedOneARC();
  }

}










#line 1  "src/IsWordAPE.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes IsWordAPE, IsWordAPEProblem, IsWordAPEARCer
//
// Principal Author: Alexander Ushakov
//
// Status: in progress
//
// Revision History:
//


#include "IsWordAPE.h"
#include "GeneralWhitehead.h"
#include "OutMessages.h"


int SetData< IsWordAPEARCer::pairSG_bool >::
hashElement( const IsWordAPEARCer::pairSG_bool& pair) const
{
  return pair.hash( );
}


// -------------------- IsWordAPEArcer --------------------------- //


IsWordAPEARCer::IsWordAPEARCer(ComputationManager& boss, 
			       const class FreeGroup& group,
			       const Word& word ) :
  ARCer ( boss ),
  theGroup ( group ),
  theWord ( word ),
  answer ( dontknow ),
  theSG ( 0 ),
  haveSG ( false )
{
  VectorOf<Word> v(1); 
  v[0] = word;
  PSE = new ProperSubgroupEnumerator( theGroup , v ); 
}


IsWordAPEARCer::~IsWordAPEARCer( )
{
  if (theSG) delete theSG;
  delete PSE;
}


void IsWordAPEARCer::runComputation()
{
  lookingup();
}


void IsWordAPEARCer::writeResults( ostream& out ) 
{
  out < answer ;
  out < " " ;
  out < haveSG;
  out < " ";
  if( haveSG ) out < theSG->generators();
}


void IsWordAPEARCer::readResults( istream& in )
{
  in > answer ;
  in > haveSG;
  if( haveSG ) {
    VectorOf<Word> v;
    in > v;
    theSG = new SGofFreeGroup( theGroup , v );
  }
}


void IsWordAPEARCer::printSG( ostream& file, const SGofFreeGroup& subgroup )
{
  VectorOf<Word> vect(subgroup.generators());

  file << "{ ";
  for( int t=0 ; t<vect.length() ; ++t ) {
    if( t )
      file << " , ";
    theGroup.printWord(file, vect[t]);
  }
  file << " }" << endl;

  file << endl;
}


void IsWordAPEARCer::lookingup()
{
  VectorOf<Word> v;
  SGofFreeGroup sg( theGroup , v );

  File file( getFileName() );

  // check for prime in a given free group
  GeneralWhitehead GW( theGroup , false );
  GW.startComputation( theWord );
  while ( !GW.continueComputation( ) );
  if( GW.extendsToFreeBasis() ) {
    file << "The word ";
    theGroup.printWord(file, theWord);
    file << " is primitive in the group " 
	 << theGroup
	 << endl;  
    answer = no;
    return;
  }

  file << "The word ";
  theGroup.printWord( file, theWord );
  file << " is not primitive in the group " 
       << theGroup
       << endl << endl;  
  
  SetOf<pairSG_bool> setSG;
  int count = 0;

  file << "New set of subgroups to check : " << endl << endl; 

  // enumerate subgroups
  while( PSE->getNextProperSubgroup( sg ) ) {

    ++count;
    bool cf = false;
    SetIterator<pairSG_bool> iterator(setSG);

    // check whether current subgroup contains in any subgroup getting earlier
    for( ;!iterator.done() ; iterator.next() ) { 

      if( iterator.value().getSG().contains(sg) ) {
	cf = true;
	break;
      }
      if( sg.contains(iterator.value().getSG( )) ) 
	setSG.removeElement( iterator.value() );

    }
    if( cf )
      continue;
    
    //write number of current subgroup & subgroup;
    static int _t = 0;
    ++_t;
    file << _t << " :  ";
    printSG( file , sg );

    setSG.adjoinElement( pairSG_bool(sg) );

    if( count>=period ) {
      count -= period;

      // check for primitive
      file << "Check whether the word is primitive in the list of subgroups " 
	   << "above:" 
	   << endl << endl;

      iterator = SetIterator<pairSG_bool> (setSG);
      for( ; !iterator.done() ; iterator.next() ) {
	if( iterator.value().getChecked() ) 
	  continue;

	setSG.removeElement(iterator.value());
	pairSG_bool pair(iterator.value().getSG( ), true);
	setSG.adjoinElement(pair);

	if( !checkForPrimitive( iterator.value().getSG() ) ) {
	  answer = no;
	  haveSG = true;
	  theSG = new SGofFreeGroup(iterator.value().getSG( ) );
	  return;
	}
	
      } //check for primitive
      
      file << "New set of subgroups to check : " << endl << endl; 

    } // (count > period)

  }
  
  SetIterator<pairSG_bool> iterator(setSG);
  file << "Check whether the word is primitive in the list of subgroups " 
       << "above:" 
       << endl << endl;
  while( !iterator.done() ) {

    if( iterator.value().getChecked( ) ) {
      iterator.next();
      continue;
    }
    if( !checkForPrimitive( iterator.value().getSG( ) ) ) {
      answer = no;
      haveSG = true;
      theSG = new SGofFreeGroup(iterator.value().getSG( ) );
      return;
    }
    iterator.next();
  }

  answer = yes;
}


bool IsWordAPEARCer::checkForPrimitive(SGofFreeGroup sg)
{ 
  bool result = false;

  // my free group
  const FreeGroup myGroup = theGroup;
  // checked word
  const Word word = theWord;
  // length of the word 
  const int length = word.length();

  // rebuild SG
  sg = SGofFreeGroup( myGroup , sg.nielsenBasis( ) );
  // rebuild word to SG
  Word newWord = expressWordInSubgroupGenerators( sg , word );
  // compute rank of the subgroup
  int rank = sg.rank( );
  
  FreeGroup freeGroup(rank);
  
  File file( getFileName( ) );

  //fast check
  {
    int i;
    int *exp = new int[rank];
    for( i=0 ; i<rank ; ++i ) exp[i] = 0;
    for( i=0 ; i<newWord.length() ; ++i )
      exp[ abs( newWord[i] )-1 ]++;

    for( i=0 ; i<rank ; ++i ) 
      if( exp[i]==1 )
	result = true;

    delete []exp;
  }

  // check
  if( !result ) {
    GeneralWhitehead GW( freeGroup , false );
    GW.startComputation( newWord );
    while ( !GW.continueComputation( ) );

    if ( GW.extendsToFreeBasis() ) result = true;
    else result = false;
  }

  // make note in file
  {
    int t;
    VectorOf<Word> v = sg.generators();
    file << "  Subgroup << ";
    for( t=0; t<v.length(); ++t ) {
      if( t ) file << " ; ";
      myGroup.printWord(file, v[t]);
    }
    file << " >> ";
    file << endl;
    
    file << "  The word expressed in the generators of the subgroup: '";
    freeGroup.printWord( file , newWord );
    file << "'" << endl;
    file << "  Result: ";
    if( !result ) file << "not" ;
    file << " primitive. " << endl << endl;
  }

  return result;
}


// -------------------- IsWordAPEProblem --------------------------- //


IsWordAPEProblem::IsWordAPEProblem(const SMWord& w)
    : Supervisor( true ), 
      theWord( w ), 
      isWordAPE( *this ),
      linkHasBeenSent( false )      
{ 

}
 

void IsWordAPEProblem::takeControl( )
{
  if( !linkHasBeenSent ) {
  
    linkHasBeenSent = true;
    
    LogMessage msg( *this, theWord );
    msg << Link("Click here to obtain details of the computation.",
		"IsWordAPE", isWordAPE->getFileName(), true);
    msg.send();

  }

  Trichotomy answer = isWordAPE->getAnswer();
  if ( isWordAPE.exists() && answer!=dontknow) {
    
    LogMessage msg( *this, theWord );
    if (answer == yes)
      {
	msg << Name(theWord) << " is almost primitive in "
	    << Name(theWord.getParent()) << ".";
      }
    else {
      if( isWordAPE->haveSG() ) {

	msg << Name(theWord) << " is not almost primitive in "
	    << Name(theWord.getParent()) << " because it is not primitive in "
	    << "the subgroup  { ";
	VectorOf<Word> v;
	SGofFreeGroup sg( theWord.getParent().getFreePreimage(), v);
	isWordAPE->getSG(sg);
	v = sg.generators();
	int n;
	for( n=0;n<v.length();++n ) {
	
	  if( n ) msg << " , ";
	  theWord.getParent().getFreePreimage().printWord(msg, v[n]);
	}
	msg << " } ";
      }
      else 
	msg << Name(theWord) << " is primitive in " << Name(theWord.getParent());
    }
    msg.send();
    
    adminTerminate();
    return;
  }

}


void IsWordAPEProblem::viewStructure(ostream& ostr) const
{
  ProblemView pv( ostr, oid(), Text("Almost primitive element problem"),
		  Text("Is") + Name( theWord )
		  + "almost primitive in"
		  + Name( Parent( theWord ) ) + "?",
		  helpID("IsWordAPEProblem", theWord.getParent()),
		  "APE ?"
		  );

  pv.startItemGroup();

  pv.add(Text("For the use of the algorithm"), isWordAPE.arcSlotID(), 100);

  pv.done();
}


// ------------------------ IsWordAPE ------------------------------ //


IsWordAPE::IsWordAPE(IsWordAPEProblem& problemObject)
    : theWord( problemObject.getWord() ),
      theGroup( theWord.getParent().getFreePreimage() ),
      theARCer(*this, theGroup, problemObject.getWord().getWord())
{
  adminStart();
}


IsWordAPE::~IsWordAPE( )
{

}


void IsWordAPE::takeControl( )
{

  if ( freeARCs() > 0 ) {

    if ( theARCer.takeControl() ) {

      adminTerminate();
      return;
    }   

    usedOneARC();
  }

}







#line 1  "src/GAConjProblemForORGroup.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes GAConjugacyForORGroupProblem, GAConjugacyForORGroupARCer, GAConjugacyForORGroup
//
// Principal Author: Alexander Ushakov
//
// Status: in progress
//
// Revision History:
//

#include "Set.h"
#include "GAConjProblemForORGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "SMSubgroup.h"
#include "ConjugacyProblem.h"


//---------------------------------------------------------------------------//
//--------------------- GAConjugacyForORGroupARCer --------------------------//
//---------------------------------------------------------------------------//


GAConjugacyForORGroupARCer::GAConjugacyForORGroupARCer( GAConjugacyForORGroup& boss ) : 
  ARCer( boss ), 
  theAnswer( dontknow ),
  theGroup( boss.getGroup( ) ),
  solver( 0 )
{

}

GAConjugacyForORGroupARCer::~GAConjugacyForORGroupARCer( )
{
  delete solver;
}

void GAConjugacyForORGroupARCer::setArguments( const OneRelatorGroup& G, const Word& u, const Word& v )
{
  theGroup = G;
  firstWord = u;
  secondWord = v;
  solver = new GAConjProblemForORGroupSolver( theGroup , firstWord , secondWord );
}

void GAConjugacyForORGroupARCer::runComputation( )
{
  if( solver->isConj( ) )
    theAnswer = yes;
  else
    theAnswer = no;
}

void GAConjugacyForORGroupARCer::writeResults( ostream& ostr )
{
  ostr < theAnswer;
}

void GAConjugacyForORGroupARCer::readResults( istream& istr )
{
  istr > theAnswer;
}


//---------------------------------------------------------------------------//
//----------------------- GAConjugacyForORGroup -----------------------------//
//---------------------------------------------------------------------------//


GAConjugacyForORGroup::GAConjugacyForORGroup( const ConjugacyProblem& CP)
  : ComputationManager( ), 
  arcer( *this ), 
  problem( CP ), 
  theAnswer( dontknow )
{
  arcer.setArguments( CP.firstWord().getParent().getFPGroup(),
		      CP.firstWord().getWord(), 
		      CP.secondWord().getWord() 
		      );

  adminStart();
}


OneRelatorGroup GAConjugacyForORGroup::getGroup( ) const 
{
  const SMWord& u = problem.firstWord( );

  int gens = u.getParent( ).getFPGroup( ).numberOfGenerators( );
  SetOf<Word> set = u.getParent( ).getFPGroup( ).getRelators( );

  SetIterator<Word> iterator( set );
  Word relator = iterator.value( );
  return OneRelatorGroup( gens , relator );
}


void GAConjugacyForORGroup::takeControl( )
{
  if ( freeARCs() > 0 ) {

    if( arcer.takeControl() ) {

      theAnswer = arcer.answer();
      adminTerminate();
      return;
    }

    usedOneARC();
  }

}


#line 1  "src/GAWordProblemForORGroup.C"
/*
 *   $Id: SA.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes GAWordForORGroupProblem, GAWordForORGroupARCer, GAWordForORGroup
//
// Principal Author: Alexander Ushakov
//
// Status: in progress
//
// Revision History:
//

#include "Set.h"
#include "GAWordProblemForORGroup.h"
#include "OutMessages.h"
#include "viewStructure.h"
#include "SMSubgroup.h"
#include "WordProblem.h"



//---------------------------------------------------------------------------//
//------------------------- GAWordForORGroupARCer ---------------------------//
//---------------------------------------------------------------------------//


GAWordForORGroupARCer::GAWordForORGroupARCer( GAWordForORGroup& boss ) : 
  ARCer( boss ), 
  theAnswer( dontknow ),
  theGroup( boss.getGroup( ) ),
  solver( 0 )
{

}

GAWordForORGroupARCer::~GAWordForORGroupARCer( )
{
  delete solver;
}

void GAWordForORGroupARCer::setArguments( const OneRelatorGroup& G, const Word& u )
{
  theGroup = G;
  theWord = u;
  solver = new GAConjProblemForORGroupSolver( theGroup , Word( ) , theWord, true , false );
}

void GAWordForORGroupARCer::runComputation( )
{
  if( solver->isConj( ) )
    theAnswer = yes;
  else
    theAnswer = no;
}

void GAWordForORGroupARCer::writeResults( ostream& ostr )
{
  ostr < theAnswer;
}

void GAWordForORGroupARCer::readResults( istream& istr )
{
  istr > theAnswer;
}


//---------------------------------------------------------------------------//
//----------------------- GAWordForORGroup -----------------------------//
//---------------------------------------------------------------------------//


GAWordForORGroup::GAWordForORGroup(WordProblem& CP)
  : ComputationManager( ), 
  arcer( *this ), 
  problem( CP ), 
  theAnswer( dontknow )
{
  arcer.setArguments( getGroup( ) , problem.getTestWord().getWord() );
  adminStart();
}


OneRelatorGroup GAWordForORGroup::getGroup( ) const 
{
  FPGroup g1 = problem.getTestWord( ).getParent().getFPGroup( );
  int gens = g1.numberOfGenerators( );
  SetOf< Word > rels = g1.getRelators( );
  SetIterator< Word > I( rels );

  return OneRelatorGroup( gens , I.value( ) );
}


void GAWordForORGroup::takeControl( )
{
  if ( freeARCs() > 0 ) {

    if( arcer.takeControl() ) {

      theAnswer = arcer.answer();
      adminTerminate();
      return;
    }

    usedOneARC();
  }

}





#line 1  "src/MakeRandomPresentation.C"
/*
 *   $Id:
 */
 
// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: 

//
// Principal Author: Alexei D. Miasnikov
//
// Status: in progress
//
// Revision History:
//


#include "MakeRandomPresentation.h"
#include "OutMessages.h"
#include "GCM.h"
#include "SMFPGroup.h"
#include "viewStructure.h"
#include "NormalRandomWord.h"
#include "FPGroup.h"

// ------------------- RandomPresentationARCer --------------------- //


void RandomPresentationARCer::runComputation( )
{
  // @am generate a random presentation
  UniformRandom ur;

  int nOfGens = ur.rand(2,maxGens);
  int nOfRels = ur.rand(2,maxRels);

  VectorOf<Chars> genNames(nOfGens);
  SetOf<Word> relts(nOfRels);
  
  for (int i=0;i<nOfGens;i++)
    genNames[i] = Chars("x") + Chars(i+1);

  for (int i=0;i<nOfRels;i++){
    NormalRandomWord nw(nOfGens, averageRels, ur.rand(0,averageRels/5),
			ur.rand(10,100000),ur.rand(10,100000));
    relts |= nw.word();
  }
  
  theGroup = FPGroup(genNames,relts);
}

void RandomPresentationARCer::writeResults( ostream& out ) 
{
  out < theGroup;
}

void RandomPresentationARCer::readResults( istream& in )
{
  in > theGroup;
}

// -------------------------- MakeRandomPresentation ----------------------- //

MakeRandomPresentation::MakeRandomPresentation(/*SMFPGroup& G*/)
  : Supervisor( true ),
    arcer( *this ),
    arcerStarted( false ),
    nOfPresentations( 1 )
  //theGroup( G )
{
  
}

void MakeRandomPresentation::takeControl( )
{
  if( freeARCs() ) {

    if ( !arcerStarted ){

      //      arcer.setArguments( getPermutations(), theTestWords[checkedWord] );
      arcerStarted = true;
    }
    
    if( arcer.takeControl() ) {
      
      SMFPGroup* smo
	= new SMFPGroup( arcer.getRetValue(),
			 Text("Random presentation"),
			 SMFPGroup::FP
			 );
      ListOf<OID> dependencies;
      CheckinMessage( *smo, "", dependencies ).send();
      
      if ((--nOfPresentations) == 0){
	adminTerminate();
	return;
      }
    }
    
    usedOneARC();
  } 

}

void MakeRandomPresentation::start(){
  nOfPresentations = getParameters().getValue("N");
  int maxG = getParameters().getValue("maxG");
  int maxR = getParameters().getValue("maxR");
  int averR = getParameters().getValue("averR");
  arcer.setArguments(maxG,maxR,averR);
}

void MakeRandomPresentation::viewStructure(ostream& ostr) const
{
   EnumertatorProblemView pv(oid(),"Make a random presentation",
			     Text("Make a random presentation" ) ,
			     "MakeRandomPresentation",
			     Text("Rand pres")
			     );
  
  pv.addParameter(EditInteger("N","Number of presentations ",0,1,1,10));
  pv.addParameter(EditInteger("maxG","Maximal number of generators ",0,2,1,100));
  pv.addParameter(EditInteger("maxR","Maximal number of relators ",0,2,1,100));
  pv.addParameter(EditInteger("averR","Average length of relators ",0,10,1,100));
 
  pv.addProblem( Subproblem("Sp1",ThisARCSlotID(),
			    Text("Generate random presentation"),100));
  
  
  submit(ostr, pv);
}
#line 1  "src/PreAbelianRepProblem.C"
#include "PreAbelianRepProblem.h"

void PreAbelianRepProblem::takeControl( )
{
  int i, j, k;
  for(i=0; i<m; i++) g[i] = Word(Generator(i+1));
  for(i=0; i<m; i++) x[i] = g[i];
  for(i=0; i<m; i++) z[i] = g[i];
  SetIterator<Word> I(G.getRelators()); 
  for(j=n-1; j>=0; j--,I.next()) y[j] = I.value();
  for(i=0; i<m; i++) for(j=0; j<n; j++)
    {
      a[i*n+j]=y[j].exponentSum(Generator(i+1));
    }
  f << "In the following, we use Nielsen transformation to obtain another set of generators <x_1,...,x_m> and another set of relators <R_1,...,R_n> such that" << endl;
  f << "    R_i = x_i^{d_i}*Q_i, for i = 1, ... , min(m,n)," << endl;  
  f << "    R_i = Q_i,           for i = min(m,n)+1,...,m," << endl;
  f << "where 0 < d_i | d_{i+1} and Q_i has zero exponent sum on each x_i." << endl;
  f << "The (i,j)-th elment of the matrix is the exponent sum of R_j on x_i" << endl; 
  show();
  for(i=0; i<m;)
    {
      if((k=findmin(i))==-1) break;
      j = k%n;
      k /= n;
      swap(i,j,'c');
      swap(i,k,'r');
      if(a[i*n+i]<0)
	{
	  for(j=i; j<n; j++) a[i*n+j] *= -1;
	  x[i] = x[i].inverse();
	  for(j=0; j<m; j++) z[j] = z[j].replaceGeneratorWithWord(Generator(i+1),g[i].inverse()); 
	}
      int b = 0;
      for(j=i+1; j<n; j++) b += plus(i,j,-a[i*n+j]/a[i*n+i],'c');
      for(j=i+1; j<m; j++) b += plus(i,j,-a[j*n+i]/a[i*n+i],'r');
      if(b==0) for(j=i+1; j<m; j++) for(k=i+1; k<n; k++) if(a[j*n+k]%a[i*n+i])
	{
	  plus(i, j, -1, 'r');
	  plus(i, k, a[j*n+k]/a[i*n+i], 'c');
	  b = 1; show();
	}
      if(b==0) i++;
    }
  VectorOf<Chars> name1 = G.namesOfGenerators();
  for(i=0; i<m; i++) name1[i] += "1"; 
  k = m<n?m:n;
  VectorOf<Word> w = y;
  for(j=0; j<k; j++) w[j] = pow(x[j], -a[j*n+j]) * w[j];
  LogMessage("New Generators: ").send();
  for(i=0; i<m; i++)
    {
      LogMessage msg;
      msg << name1[i];
      msg << " = ";
      G.printWord(msg, x[i].freelyReduce());
      msg.send();
    }
  for(i=0; i<m; i++)
    {
      LogMessage msg;
      msg << G.nameOfGenerator(i);
      msg << " = ";
      FPGroup(name1).printWord(msg, z[i].freelyReduce());
      msg.send();
    }
  LogMessage("New Relations: ").send();
  for(j=0; j<n; j++)
    {
      LogMessage msg;
      msg << "R" << j+1 << " = ";
      G.printWord(msg, y[j].freelyReduce());
      if(j<k)
	{
	  msg << " = " << name1[j] << "^" << a[j*n+j] << " * ";
	  FPGroup(name1).printWord(msg, w[j].replaceGenerators(z).freelyReduce());
	}
      msg.send();
    }
  LogMessage msg;
  msg << Link("Click here to see the intermediate steps: ", "PreAbelianRepProblem", f.getFileName());
  msg.send();
}

int PreAbelianRepProblem::findmin(int i)
{
  int j, k;
  k = i*n+i;
  for(j=k+1; j<m*n; j++) if(a[k]==0||a[j]&&abs(a[j])<abs(a[k])) k=j;
  return a[k]?k:-1;
}

int PreAbelianRepProblem::plus(int L1, int L2, int mult, char type) 
{
  if(mult==0||L1<0||L2<0) return 0;//unchanged
  if(type=='r')
    {
      if(L1>=m||L2>=m) return 0;
      x[L1] *= pow(x[L2], -mult);
      for(int i=0; i<m; i++) z[i] = z[i].replaceGeneratorWithWord(Generator(L1+1), g[L1]*pow(g[L2],mult));
      for(int i=0; i<n; i++) a[L2*n+i] += a[L1*n+i] * mult;
    }
  else if(type=='c')
    {
      if(L1>=n||L2>=n) return 0;
      y[L2] *= pow(y[L1], mult);
      for(int i=0; i<m; i++) a[i*n+L2] += a[i*n+L1] * mult;
    }
  show();
  return 1;//changed
}

void PreAbelianRepProblem::swap(int L1, int L2, char type)
{
  int i, j;
  if(L1==L2||L1<0||L2<0) return;
  if(type=='r')
    {
      if(L1>=m||L2>=m) return;
      Word w = x[L1];
      x[L1] = x[L2];
      x[L2] = w;
      VectorOf<Word> v = VectorOf<Word>(m);
      for(i=0; i<m; i++) v[i] = g[i];
      v[L1] = g[L2];
      v[L2] = g[L1];
      for(i=0; i<m; i++) z[i] = z[i].replaceGenerators(v);
      for(j=0; j<n; j++)
	{
	  i = a[L1*n+j];
	  a[L1*n+j] = a[L2*n+j];
	  a[L2*n+j] = i;
	}
    }
  else if(type=='c')
    {
      if(L1>=n||L2>=n) return;
      Word w = y[L1];
      y[L1] = y[L2];
      y[L2] = w;
      for(i=0; i<m; i++)
	{
	  j = a[i*n+L1];
	  a[i*n+L1] = a[i*n+L2];
	  a[i*n+L2] = j;
	}
    }
  show();
}

Word PreAbelianRepProblem::pow(const Word &w, int mult)
{
  Word result = Word();
  if(mult<0) 
    {
      for(int i=mult; i<0; i++) result *= w;
      return result.inverse();
    }
  else
    { 
      for(int i=0; i<mult; i++) result *= w;
      return result;
    }
}

void PreAbelianRepProblem::show(void)
{
  f << endl << "--- Step " << c++ << " ---";
  f << endl << "Generators: ";
  for(int i=0; i<m; i++) x[i] = x[i].freelyReduce();
  G.printVectorOfWords(f, x);
  f << endl << "Relations: ";
  for(int j=0; j<n; j++) y[j] = y[j].freelyReduce();
  G.printVectorOfWords(f, y);
  for(int i=0; i<m; i++)
    {
      f << endl;
      for(int j=0; j<n; j++) f << "    " << a[i*n+j];
    }
  f << endl;
}
