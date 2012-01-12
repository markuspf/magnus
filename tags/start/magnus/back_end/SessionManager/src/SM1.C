#line 1  "src/ComputationManager.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class ComputationManager
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "ComputationManager.h"
#include "OutMessages.h"
#include "SMFPGroup.h"

#define DEBUG_CLIENTS

//---------------------------------------------------------------------------//
//------------------------ ComputationManager -------------------------------//
//---------------------------------------------------------------------------//


ComputationManager::ComputationManager( bool display_in_fe )
  : SMObject( true ),
    ResourceManager( oid() ),
    amFast( false ),
    showMe( display_in_fe ),
    wasStalled( false ),
    isCrashed( false ),
    theState( UNSTARTED )
{ }


ComputationManager::~ComputationManager( )
{

}


ComputationManager::State ComputationManager::state( ) const
{
  return theState;
}


const IconID ComputationManager::iconID( ) const
{
  return IconID::problem;
}


void ComputationManager::viewStructure(ostream& ostr) const
{
  // Blank provided here to save the nuisance of supplying it
  // in derivatives, many of which do not have `viewStructures'.
}


const char* ComputationManager::typeID( ) const
{
  return "ComputationManager";
}


void ComputationManager::printProperties(ostream& ostr) const
{
  // Blank provided here to save the nuisance of supplying it
  // in derivatives, most of which do not have `properties'.
}


void ComputationManager::printDefinition(ostream& ostr) const
{
  // Blank provided here to save the nuisance of supplying it
  // in derivatives, most of which do not have `definitions'.
}


void ComputationManager::resultIsFast( )
{
  amFast = true;
  showMe = false;
}


void ComputationManager::printGlobalMessageTemplates(ostream& ostr)
{
  ostr
	 << "init_view_req_msg_ {${viewID} " << VIEW_REQUEST
	 << " ${ARCSlotID}}" << endl

	 << "init_arc_msg_ {${viewID} " << ARC_UPDATE
	 << " ${ARCSlotID} ${value}}" << endl

	 << "init_start_msg_ {${viewID} " << START << '}' << endl

	 << "init_suspend_msg_ {${viewID} " << SUSPEND << '}' << endl

	 << "init_resume_msg_ {${viewID} " << RESUME << '}' << endl

	 << "init_terminate_msg_ {${viewID} " << TERMINATE << '}' << endl

	 << "init_parameter_msg_ {${viewID} " << PARAMETER 
	 << " ${value} {EOF_PARAM} }" << endl;
}


void ComputationManager::readMessage(istream& istr)
{
  MessageTag tag;
  
  istr >> (int&)tag;

  if ( tag == ARC_UPDATE ) {
	 
	 int asi, arcs;
	 
	 istr >> asi;
	 istr >> arcs;
	 
	 acceptAllocation( oid(), ARCSlotID(asi), ARC(arcs), true );
	 
  } else if ( tag == START ) {
	 adminStart();
  } else if ( tag == SUSPEND ) {
	 adminSuspend();
  } else if ( tag == RESUME ) {
	 adminResume();
  } else if ( tag == TERMINATE ) {
	 adminTerminate();
  }
}

void ComputationManager::attachClient( const SMObject& client ) { 
  theClients |= client.oid(); 
}

void ComputationManager::detachClient( const SMObject& client ) {  
  theClients -= client.oid(); 
}

SetOf<OID> ComputationManager::getClients( ) const { 
  return theClients; 
}

bool ComputationManager::checkForStall( )
{
  // Definition of `stalled': A CM X which is displayed in the FE, is
  // in state RUNNING, and for which no CM (including X), which has
  // ever had ARCs with benefactor X, has any free ARCs now.

  // We can check the first two conditions here. For the last, we must
  // ask TheObjects, since it is the only one (so far) who knows where
  // all of the CMs are. TheObjects asks the ResourceManager of each
  // CM whether it works for X; it can answer, because (so far) it
  // never throws out zeroed Resource records.

  if ( displayInFE() && theState == RUNNING ) {

	 if ( TheObjects::isStalled( *this ) ) {

		if ( ! wasStalled )
		  StateTransition( StateTransition::STALLED, *this ).send();
		wasStalled = true;
		return true;

	 } else {  // Any state other than RUNNING overrides in the FE.

		if ( wasStalled )
		  StateTransition( StateTransition::RESUME, *this ).send();
		wasStalled = false;
	 }
  }
  return false;
}


void ComputationManager::adminStart( )
{
  theState = RUNNING;

  liquefy();


  if ( displayInFE() )
	 StateTransition( StateTransition::START, oid() ).send();

  // @am 
  start();
}


void ComputationManager::adminSuspend( )
{
  theState = SUSPENDED;

  freeze();

  suspend();

  if ( displayInFE() )
	 StateTransition( StateTransition::SUSPEND, oid() ).send();
}


void ComputationManager::adminResume( )
{
  theState = RUNNING;

  liquefy();

  resume();

  if ( displayInFE() )
    StateTransition( StateTransition::RESUME, oid() ).send();
}


void ComputationManager::adminTerminate( )
{
  //@db
  if( theState == TERMINATED ) return;
  
  theState = TERMINATED;

  freeze();

  terminate();

  terminateArcers();

  if ( displayInFE() )
    StateTransition( StateTransition::TERMINATE, oid() ).send();
}


void ComputationManager::hireArcer( ARCer* arcer )
{
  theArcers.append(arcer);
}


void ComputationManager::terminateArcers( )
{
  int len = theArcers.length();
  for( int i = 0; i < len; ++i )
    theArcers[i]->terminate();
}


Chars ComputationManager::helpID( const Chars& problemName, 
				  const SMFPGroup& parent ) const
{
  return problemName + "::" + parent.getCheckinTypeStr();
}


void ComputationManager::setCrashState( ) 
{ 
  isCrashed = true; 
  adminTerminate();
}
#line 1  "src/FEData.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of mixin class FEData
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "FEData.h"
#include "SMObject.h"


//---------------------------------------------------------------------------//
//-------------------------- FEData::(Reps) ---------------------------------//
//---------------------------------------------------------------------------//


FEData::KeyRep::KeyRep(int i)
  : index( i ), text( 0 ), isConstant( true ), subExpression( 0 )
{ }


FEData::KeyRep::KeyRep(int i, const char* t)
  : index( i ), text( t ), isConstant( false ), subExpression( 0 )
{ }


FEData::KeyRep::KeyRep(OID oid, const char* t)
  : index( oid.unwrap() ), text( t ), isConstant( true ), subExpression( 0 )
{ }


FEData::KeyRep::KeyRep(ExpressionRep* se, const char* t)
  : text( t ), subExpression( se )
{ }


void FEData::KeyRep::printOn(ostream& ostr) const
{
  if ( subExpression ) {
	 ostr << "$A(";
	 subExpression->printOn( ostr );
	 ostr << ',' << text << ')';
  } else if ( isConstant ) {
	 if ( text == 0 )
		ostr << index;
	 else
		ostr << "$A(" << index << ',' << text << ')';
  } else {
	 if ( text == 0 )
		ostr << "$x" << index;
	 else
		ostr << "$A($x" << index << ',' << text << ')';
  }
}


FEData::JoinRep::JoinRep(FEData::ExpressionRep* a1,
								 FEData::ExpressionRep* a2,
								 const char* j
								 )
  : lhs(a1), rhs(a2), junctor(j)
{ }


FEData::JoinRep::~JoinRep( )
{
  lhs->getHence();
  rhs->getHence();
}
	 

void FEData::JoinRep::printOn(ostream& ostr) const
{
  ostr << '(';
  lhs->printOn( ostr );
  ostr << junctor;
  rhs->printOn( ostr );
  ostr << ')';
}


FEData::NotRep::NotRep(FEData::ExpressionRep* a) : expr(a) { }


FEData::NotRep::~NotRep( )
{
 expr->getHence();
}


void FEData::NotRep::printOn(ostream& ostr) const
{
  ostr << '!';
  expr->printOn( ostr );
}


FEData::NameRep::NameRep(FEData::ExpressionRep* a) : expr(a) { }


FEData::NameRep::~NameRep( )
{
 expr->getHence();
}


void FEData::NameRep::printOn(ostream& ostr) const
{
  ostr << "$A(";
  expr->printOn( ostr );
  ostr << ",name)";
}


//---------------------------------------------------------------------------//
//------------------------ FEData::Expression -------------------------------//
//---------------------------------------------------------------------------//


ostream& operator << ( ostream& ostr, const FEData::Expression& E)
{
  E.theRep->printOn( ostr );
  return ostr;
}


FEData::Expression
FEData::Expression::join(const FEData::Expression& E, const char* junctor) const
{
  return
	 Expression( new JoinRep( theRep->gimme(), E.theRep->gimme(), junctor ) );
}


//---------------------------------------------------------------------------//
//------------------------- FEData::DataPair --------------------------------//
//---------------------------------------------------------------------------//


FEData::DataPair& FEData::DataPair::operator = (const FEData::DataPair& dp)
{
  theKey = dp.theKey;
  theDatum = dp.theDatum;
}


ostream& operator << ( ostream& ostr, const FEData::DataPair& dp)
{
  ostr << "{ key {" << dp.theKey << "} datum {" << dp.theDatum << "}} ";
  return ostr;
}


//---------------------------------------------------------------------------//
//--------------------------- FEData::Text ----------------------------------//
//---------------------------------------------------------------------------//


FEData::Text::Text(const char* t)
{
  ostrstr << t;
}


FEData::Text::Text(const Expression& E)
{
  ostrstr << E;
}
     

FEData::Text::operator Chars( ) const
{
  ostrstream& o = (ostrstream&)ostrstr;  // Cast away const-ness.
  o << ends;
  Chars result( o.str() );
  o.freeze(0);
  return result;
}


FEData::Text& FEData::Text::operator + (const FEData::Text& t)
{
  ostrstr << ' ' << t;
  return *this;
}


FEData::Text& FEData::Text::operator + (const FEData::Expression& E)
{
  ostrstr << ' ' << E;
  return *this;
}


ostream& operator << (ostream& ostr, const FEData::Text& t)
{
  ostrstream& o = (ostrstream&)t.ostrstr;  // Cast away const-ness.
  o << ends;
  ostr << o.str();
  o.freeze(0);
  return ostr;
}


//--------------------------------------------------------------------------//
//-------------------------- FEData::Name ----------------------------------//
//--------------------------------------------------------------------------//

FEData::Name::Name(int i)
  : Expression( new NameRep( Object( i ).theRep->gimme() ) )
{ }


FEData::Name::Name(OID o)
  : Expression( new NameRep( Object( o ).theRep->gimme() ) )
{ }


FEData::Name::Name(const FEData::Expression& E)
  : Expression( new NameRep( E.theRep->gimme() ) )
{ }


// ------------------------- FEData::Link ---------------------------------- //


FEData::Link::Link( const Chars& text, const Chars& problemName,
		    const Chars& fileName, bool isDynamic )
  : Text( Chars("@") + text + "@" + problemName + "@" + fileName + "@"
	  + (isDynamic ? "1" : "0") + "@" )
{ }
#line 1  "src/GCM.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class GCM
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "GCM.h"
#include "SMFPGroup.h"


//---------------------------------------------------------------------------//
//-------------------------------- GCM --------------------------------------//
//---------------------------------------------------------------------------//


GCM::GCM( class SMFPGroup& G )
  : Supervisor( false ),
    normalClosure( *this ),
    abelianInvariants( *this ),
    abelianPrimes( *this ),
    abelianRank( *this ),
    kbSupervisor( *this ),
    agSupervisor( *this ),
    theToddCoxeter( *this ),
    ghToddCoxeter( *this ),
    nilpotentQuotients( *this ),
    computeBasis( *this ),
    theSMFPGroup( G ),
    didFastChecks( false )
{
  adminStart();
}



void GCM::takeControl( )
{
  if ( ! didFastChecks ) {
    
    didFastChecks = true;
    
    // Cheat and do some quickie checks here without reporting ARC consumption.

    if( theSMFPGroup.getCheckinType() != SMFPGroup::ABELIAN &&
	theSMFPGroup.getCheckinType() != SMFPGroup::NILPOTENT ) {
    
      SetOf<Word> relators = theSMFPGroup.getFPGroup().getRelators();
      
      if ( relators.cardinality() == 1 ) {
	
	SetIterator<Word> I( relators );
	Word r = I.value().cyclicallyReduce();
	if ( r.isProperPower() )
	  theSMFPGroup.gic.putIsOneRelatorWithTorsion( r );
	else
	  theSMFPGroup.gic.putIsOneRelator( r );
      }
    }
  }
}


void GCM::start( ) { }

void GCM::terminate( ) { }

void GCM::viewStructure(ostream& ostr) const { }

#line 1  "src/Menu.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of classes Ctor<n>, Menu<n>
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//
// * 11/10/96 EP added stuff for SMVectorOfWords
//
// * 96/10/09 @dp added ReadFreeGetNextN_thElementItem::construct(SMObject*,
//     Chars&,istream&).
//   96/10/10 @dp added ReadTerminalSegmentItem::construct(SMObject*,Chars&,
//     istream&).
//   96/10/10 @dp added ReadSegmentOfWordItem::construct(SMObject*,Chars&,
//     istream&).
//   void Menu::DefineInt2::print(ostream& ostr) const
//   //@db temporary implementation


#include "Chars.h"
#include "Menu.h"
#include "ObjectSmith.h"
#include "SessionManager.h"
#include "TheObjects.h"
#include "OutMessages.h"
#include "EquationParser.h"
#include "EqSystemParser.h"
#include "Permutation.h"

// Includes required for special cases:

#include "TheObjects.h"
#include "SMFPGroup.h"
#include "SMWord.h"
#include "SMEquation.h"
#include "SMSubgroup.h"
#include "SMMagnusBreakdown.h"
#include "SMMap.h"
#include "fastProblems.h"
#include "HomologyProblem.h"
#include "Type.h"
#include "PackagesData.h"
#include "ObjectFactory.h"

//---------------------------------------------------------------------------//
//------------------------------ Ctor<n> ------------------------------------//
//---------------------------------------------------------------------------//


void Ctor::newDependent(SMObject* smo, OID oid) const
{
  //@db smo->addDependent( oid );
  // addDependent() is now called by CheckinMessage 
}


SMObject* Ctor::get(int oid) const
{
  return TheObjects::get( OID(oid) );
}


void CtorAux0::readFrom(istream& istr) const {
  Chars name;
  SMObject* smo = construct( name, istr );
  if ( smo ) {
    if ( smo->displayInFE() ) {
      ListOf<OID> dependencies;
      CheckinMessage( *smo, name, dependencies ).send();
    }
  }
}


void CtorAux1::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  Chars name;
  SMObject* smo = construct( arg1, name, istr );
  if ( smo ) {
    newDependent( arg1, *smo );
    if ( smo->displayInFE() ) {
      ListOf<OID> dependencies( *arg1 );
      CheckinMessage( *smo, name, dependencies ).send();
    }
    //@rn Factor out somewhere
  }
}


void CtorAux2::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  istr >> i;
  SMObject* arg2 = get( i );
  if ( arg2 == 0 ) return;
  Chars name;
  SMObject* smo;
  if ( transposeARGS ) smo = construct( arg2, arg1, name, istr );
  else smo = construct( arg1, arg2, name, istr );
  if ( smo ) {
    newDependent( arg1, *smo );
    newDependent( arg2, *smo );
    if ( smo->displayInFE() ) {
      ListOf<OID> dependencies( *arg1 );
      dependencies.append( *arg2 );
      CheckinMessage( *smo, name, dependencies ).send();
    }
    //@rn Factor out somewhere
  }
}


void CtorAux3::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  istr >> i;
  SMObject* arg2 = get( i );
  if ( arg2 == 0 ) return;
  istr >> i;
  SMObject* arg3 = get( i );
  if ( arg3 == 0 ) return;
  Chars name;
  SMObject* smo = construct( arg1, arg2, arg3, name, istr );
  if ( smo ) {
    newDependent( arg1, *smo );
    newDependent( arg2, *smo );
    newDependent( arg3, *smo );
    if ( smo->displayInFE() ) {
      ListOf<OID> dependencies( *arg1 );
      dependencies.append( *arg2 );
      dependencies.append( *arg3 );
      CheckinMessage( *smo, name, dependencies ).send();
    }
    //@rn Factor out somewhere
  }
}


void CtorAux4::readFrom(istream& istr) const
{
  int i;
  istr >> i;
  SMObject* arg1 = get( i );
  if ( arg1 == 0 ) return;
  istr >> i;
  SMObject* arg2 = get( i );
  if ( arg2 == 0 ) return;
  istr >> i;
  SMObject* arg3 = get( i );
  if ( arg3 == 0 ) return;
  istr >> i;
  SMObject* arg4 = get( i );
  if ( arg4 == 0 ) return;
  Chars name;
  SMObject* smo = construct( arg1, arg2, arg3, arg4, name, istr );
  if ( smo ) {
    newDependent( arg1, *smo );
    newDependent( arg2, *smo );
    newDependent( arg3, *smo );
    newDependent( arg4, *smo );
    if ( smo->displayInFE() ) {
      ListOf<OID> dependencies( *arg1 );
      dependencies.append( *arg2 );
      dependencies.append( *arg3 );
      dependencies.append( *arg4 );
      CheckinMessage( *smo, name, dependencies ).send();
    }
    //@rn Factor out somewhere
  }
}



//---------------------------------------------------------------------------//
//------------------------------- Ctor --------------------------------------//
//--------------------------- special cases ---------------------------------//


SMObject* ReadSMFPGroup::construct(Chars& name, istream& istr) const
{
  istr >> name;
  FPGroup G;
  Chars err = istr >> G;
  if ( err.length() == 0 ) {
    if( G.getRelators().cardinality() == 0 )
      return new SMFPGroup( G, "User defined free group", SMFPGroup::FREE );
    if ( G.getRelators().cardinality() == 1 )
      return new SMFPGroup( G, "User defined one relator group",
			    SMFPGroup::ONE_RELATOR );
    return new SMFPGroup( G, "User defined finitely presented group" );
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject* ReadSMFreeGroup::construct(Chars& name, istream& istr) const
{
  istr >> name;
  FreeGroup G;
  Chars err = istr >> G;
  if ( err.length() == 0 ) {
    return new SMFPGroup( FPGroup( G.namesOfGenerators() ),
			  "User defined free group", SMFPGroup::FREE );
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject* ReadSMAbelianGroup::construct(Chars& name, istream& istr) const
{
  istr >> name;
  FPGroup G;
  Chars err = istr >> G;
  if ( err.length() == 0 ) {
    SMFPGroup* smo = new SMFPGroup( G, "User defined abelian group", 
				    SMFPGroup::ABELIAN );
    smo->gic.putIsAbelian();
    return smo;
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject* ReadSMNilpotentGroup::construct(Chars& name, istream& istr) const
{
  istr >> name;
  int theClass;
  istr >> theClass;
  FPGroup G;
  Chars err = istr >> G;
  if ( err.length() == 0 ) {
    if( G.getRelators().cardinality() > 0 ){
      SMFPGroup* smo = new SMFPGroup( G, "User defined nilpotent group", 
				      SMFPGroup::NILPOTENT );
      smo->gic.putIsNilpotent( theClass );
      return smo;
    } else {
      SMFPGroup* smo = new SMFPGroup( FPGroup(G.namesOfGenerators()),
				      "User defined free nilpotent group",
				      SMFPGroup::FREE_NILPOTENT );
      FreeGroup FG(G.namesOfGenerators());
      smo->gic.putIsFreeNilpotent( FG, theClass );
      return smo;
    }
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}

SMObject* ReadSMFreeNilpotentGroup::construct(Chars& name, istream& istr) const
{
  istr >> name;
  int theClass;
  istr >> theClass;
  FreeGroup G;
  Chars err = istr >> G;
  if ( err.length() == 0 ) {
    SMFPGroup* smo = new SMFPGroup( FPGroup(G.namesOfGenerators()),
				    "User defined free nilpotent group",
				    SMFPGroup::FREE_NILPOTENT );
    smo->gic.putIsFreeNilpotent( G, theClass );
    return smo;
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}

SMObject* ReadSMORGroup::construct(Chars& name, istream& istr) const
{
  istr >> name;
  FPGroup G;
  Chars err = istr >> G;
  if ( err.length() == 0 ) {
    if ( G.getRelators().cardinality() == 1 ) {
      SetIterator<Word> I = G.getRelators();
      Word r = I.value().cyclicallyReduce();
      if( r.isProperPower() ) 
	return new SMFPGroup( G, "User defined one relator group with torsion",
			      SMFPGroup::ONE_RELATOR_WITH_TORSION );
      else return new SMFPGroup( G, "User defined one relator group",
				 SMFPGroup::ONE_RELATOR );
    } else {
      ParseErrorMessage(name,"0 {the presentation is not one relator}").send();
      return 0;		
    }
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject* ReadSMSmallCancGroup::construct(Chars& name, istream& istr) const
{
  istr >> name;
  FPGroup G;
  Chars err = istr >> G;
  if ( err.length() == 0 ) {
    int lambda = G.cancellationLambda();
    if ( lambda < 6 && lambda != 0 ) {
      ostrstream o;
      o << "0 {This group is only C'(1/" << lambda << ").}" << ends;
      ParseErrorMessage( name, o.str() ).send();
      o.freeze(0);
      //@rn If message is queued, `freeze' allows str to be deleted.
      return 0;
    } else {
      ostrstream o;		
      if ( lambda == 0 ) o << "User defined C'(0) group" << ends;
      else o << "User defined C'(1/" << lambda << ") group" << ends;
      SMFPGroup* smo = new SMFPGroup(G,o.str(),SMFPGroup::SMALL_CANCELLATION);
      o.freeze(0);
      smo->gic.putHaveMSC( G, lambda );
      return smo;
    }
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}

SMObject* ReadSMPermutation::construct( Chars& name, istream& istr ) const
{
  Permutation p;
  istr >> name;
  Chars errMsg = istr >> p;
  if( errMsg.length() == 0 )
    return new SMPermutation( p, "User defined permutation" );
  else {
    ParseErrorMessage( name, Chars(0) + " {" + errMsg + ".}" ).send();
    //    ParseErrorMessage( name, Chars(P.cursorPos()) 
    //+ " {" + errMsg + ".}" ).send();
    return 0;
  }
}

SMObject* ReadSMWord::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FGGroup G = parent.getFPGroup();
  SetOf<OID> S = TheObjects::dependencyClosure(parent);
  SetOf<OID> O;
  SetIterator<OID> I(S);
  for(; !I.done(); I.next()) {
    SMObject* smo = TheObjects::get(OID(I.value()));
    if( Chars(smo->typeID()) == Chars("SMWord")) O |= I.value();
  }
  VectorOf<OID> oids = makeVectorOf(O);
  int newNamesLen = oids.length();
  VectorOf<Chars> newNames(newNamesLen);
  for( int i = 0; i < newNamesLen; ++i )
    newNames[i] = TheObjects::name(oids[i]);
  VectorOf<Chars> names = concatenate(G.namesOfGenerators(), newNames);
  FreeGroup F(names);
  Word w;
  Chars err;
  w = F.readWord(istr, err);
  if ( err.length() == 0 ) {
    int numOfOldGens = G.numberOfGenerators();
    int vLen = names.length();
    VectorOf<Word> v(vLen);
    for( int i = 0; i < numOfOldGens; ++i )  v[i] = Word(Generator(i+1));
    for( int i = numOfOldGens; i < vLen; ++i )
      v[i] = ((SMWord*)(TheObjects::get(oids[i-numOfOldGens])))->getWord();
    Map M(F,G,v);
    w = M.imageOf(w);
    return new SMWord( parent,w,Text("User defined word in the generators of")
		       + Name( parent ) );
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject*
ReadSMEquation::construct(SMObject* a1, Chars& name, istream& istr) const
{
  //@rn This expects the equation to be quadratic.
  
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FGGroup G = parent.getFPGroup();
  EquationParser P( istr );
  VectorOf<Chars> newNames;
  Chars err;
  Word E = P.parseEquation( G.namesOfGenerators(), newNames, err );  
  if ( err.length() == 0 ) {

    // Check for quadraticness

    int numOfVar = newNames.length() - G.numberOfGenerators();
    for( int i = 0; i < numOfVar; ++i ) {
      int count = E.numberOfOccurrences( Generator( i + 1 ) );
      if ( count != 2 && count != 0 ) {
	ParseErrorMessage( name, "0 {The equation is not quadratic}" ).send();
	return 0;
      }
    }
    return new SMEquation( parent, FreeGroup( newNames ), E, numOfVar,
			   Text("User defined equation in") + Name( parent ) );
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}

SMObject*
ReadSMEquation2::construct(SMObject* a1, Chars& name, istream& istr) const
{
  //@rn This expects the equation to be quadratic.
  
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FGGroup G = parent.getFPGroup();
  EquationParser P( istr );
  VectorOf<Chars> newNames;
  Chars err;
  Word E = P.parseEquation( G.namesOfGenerators(), newNames, err );  
  if ( err.length() == 0 ) {
    int numOfVar = newNames.length() - G.numberOfGenerators();
    return new SMEquation2( parent, FreeGroup( newNames ), E, numOfVar,
			    Text("User defined equation in") + Name(parent) );
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}

SMObject*
ReadSMEqSystem::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FGGroup G = parent.getFPGroup();
  EqSystemParser ESP( istr );
  VectorOf<Chars> newNames;
  Chars err;
  VectorOf<Word> system = ESP.parseEqSystem( G.namesOfGenerators(), newNames, err );
  if ( err.length() == 0 ) {
      //cout << "no err";
    int numOfVar = newNames.length() - G.numberOfGenerators();
    return new SMEqSystem( parent, FreeGroup( newNames ), system, numOfVar,
			   Text("User defined system of equations in") 
			   + Name( parent ) );
  } else {
      //cout << "err";
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}

SMObject*
ReadSMSubgroup::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FPGroup G = parent.getFPGroup();
  Chars err;
  VectorOf<Word> generators = G.readVectorOfWords(istr, err);
  if ( err.length() == 0 ) {
    return new SMSubgroup( parent, 
			   SGofFreeGroup(parent.getFreePreimage(), generators),
			   Text("User defined subgroup of") + Name( parent ) );
  } else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject*
ReadSMSetOfWords::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FPGroup G = parent.getFPGroup();
  Chars err;
  SetOf<Word> generators = G.readSetOfWords(istr, err);
  if ( err.length() == 0 ) 
    return new SMSetOfWords( parent, generators, Text("User defined set of words in") + Name( parent ) );
  else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}

SMObject*
ReadSMVectorOfWords::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FPGroup G = parent.getFPGroup();
  Chars err;
  VectorOf<Word> generators = G.readVectorOfWords(istr, err);
  if ( err.length() == 0 )
    return new SMVectorOfWords( parent, generators, Text("User defined tuple of words in") + Name( parent ) );
  else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject* ReadSMMap::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMFPGroup& domain = *(SMFPGroup*)a1;
  istr >> name;
  FPGroup G = domain.getFPGroup();
  Map m( G, G );
  Chars err = istr >> m;
  if ( err.length() == 0 )
    return new SMMap( domain, domain, m,
		      Text("User defined map from the generators of")
		      + Name( domain ) + "to" + Name( domain ) );
  else {
    ParseErrorMessage( name, err ).send();    
    return 0;
  }
}

SMObject* ReadSMMap2::construct(SMObject* a1, SMObject* a2,
				Chars& name, istream& istr) const
{
  SMFPGroup& domain = *(SMFPGroup*)a1;
  SMFPGroup& range = *(SMFPGroup*)a2;
  istr >> name;
  FPGroup G = domain.getFPGroup();
  FPGroup H = range.getFPGroup();
  Map m( G, H );
  Chars err = istr >> m;
  if ( err.length() == 0 )
    return new SMMap( domain, range, m,
		      Text("User defined map from the generators of")
		      + Name( domain ) + "to" + Name( range ) );
    
  else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject* ReadSymmetricGroup::construct(Chars& name, istream& istr) const
{
  int n;
  istr >> n;
  SymmetricGroupFactory S(n);
  return S.factory();
}


SMObject*
ReadSMMagnusBreakdown::construct(SMObject* a1, Chars& name, istream& istr)const
{
  SMFPGroup& G = *(SMFPGroup *)a1;
  return new SMMagnusBreakdown( G );
}


SMObject*
ReadPowerOfMapItem::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int p;
  istr >> p;
  return new PowerOfMap( *(SMMap*)a1, p );
}


SMObject*
ReadPHeight::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int p;
  istr >> p;
  return new AbelianPHeightOfEltProblem( *(SMWord*)a1, p );
}

SMObject*
ReadHomologyItem::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int d;
  istr >> d;
  return new HomologySupervisor( *(SMFPGroup*)a1, d );
}

SMObject*
ReadHomologyItem1::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int d;
  istr >> d;
  return new AbelianIntegralHomologyProblem( *(SMFPGroup*)a1, d );
}

SMObject*
ReadAutEnumItem::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int p;
  istr >> p;
  return new AutEnumerator( *(SMFPGroup*)a1, p );
}


SMObject*
ReadFinAutEnumItem::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int p;
  istr >> p;
  return new AutEnumerator( *(SMFPGroup*)a1, p ,true);
}

SMObject*
ReadInitialSegmentItem::construct(SMObject* a1,Chars& name,istream& istr) const
{
  int p;
  istr >> p;
  return new InitialSegmentOfWord( *(SMWord*)a1, p );
}

SMObject*
ReadTerminalSegmentItem::construct(SMObject* a1,Chars& name,istream& istr)const
{
  int p;
  istr >> p;
  return new TerminalSegmentOfWord( *(SMWord*)a1, p );
}

SMObject*
ReadSegmentOfWordItem::construct(SMObject* a1, Chars& name, istream& istr)const
{
  int start;
  istr >> start;
  int length  = 1;
  // @dp TO BE INSERTED: istr >> length;

  SMWord *smo = (SMWord*)a1;
  if( start > smo->getWord().length() ) {
    Message message("Warning");
    message << "The starting position should be less than " 
	    << smo->getWord().length()+1 << ".";
    message.send();
    return 0;
  }
  return new SegmentOfWord( *(SMWord*)a1, start, length);
}


SMObject*
ReadMakeNilpotentQuotientItem::construct(SMObject* a1, Chars& name,
					 istream& istr) const
{
  int p;
  istr >> p;
  return new MakeNilpotentQuotient( *(SMFPGroup*)a1, p );
}


SMObject*
ReadMakeQuotientItem::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMFPGroup& parent = *(SMFPGroup*)a1;
  istr >> name;
  FPGroup G = parent.getFPGroup();
  Chars err;
  SetOf<Word> relators = G.readSetOfWords(istr, err);
  
  if ( err.length() == 0 )
    return new MakeQuotient( *(SMFPGroup*)a1, relators );
  else {
    ParseErrorMessage( name, err ).send();
    return 0;
  }
}


SMObject*
ReadFreeGetN_thElementItem::construct(SMObject* a1, Chars& name, 
				      istream& istr) const
{
  int p;
  istr >> p;
  return new FreeGetN_thElement( *(SMFPGroup*)a1, p );
}

SMObject*
ReadFreeGetNextN_thElementItem::construct(SMObject* a1, Chars& name, 
					  istream& istr) const
{
  int p;
  istr >> p;
  return new FreeGetNextN_thElement( *(SMWord*)a1, p );
}


SMObject*
ReadIsNilpotentProblemItem::construct(SMObject* a1, Chars& name,
				      istream& istr) const
{
  int p;
  istr >> p;
  return new IsNilpotentProblem( *(SMFPGroup*)a1, p );
}


SMObject*
ReadIsSGNilpotentItem::construct(SMObject* a1, Chars& name, istream& istr)const
{
  int p;
  istr >> p;
  return new IsSGNilpotent( *(SMSubgroup*)a1, p );
}

SMObject*
ReadLCStermProblem::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int p;
  istr >> p;
  return new NGLCStermGensProblem( *(SMFPGroup*)a1, p );
}

SMObject* 
ReadNthPower::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int p;
  istr >> p;
  return new IsWordNthPower( *(SMWord*)a1, p );
}

SMObject* 
ReadNormalApproximation::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int p;
  istr >> p;
  return new NormalApproximationProblem( *(SMSubgroup*)a1, p );
}

// ----------------- Package Management ------------------- //

SMObject*
ReadPackage::construct(Chars& name, istream& istr) const
{
  Chars tname;
  Chars command;
  int checkin_selected;
  Type::notype n;
  Type object_selected(n);
  istr > tname > command > checkin_selected;
  object_selected.readMeFrom( istr );
  int nOfParam;
  istr > nOfParam;
  ListOf<ParamRecord> pList;
  for (int i=0;i<nOfParam;i++){
    Chars pname;
    istr > pname;
    Chars ptype;
    istr > ptype;
    ParamRecord::PARAM_TYPE pt;
    if (ptype == Chars("Integer")) pt = ParamRecord::INT;
    else if (ptype == Chars("String")) pt = ParamRecord::STRING;
    else pt = ParamRecord::BOOL;
    pList.append( ParamRecord( pname, pt) );
  }
  return new AddPackage(tname,command,checkin_selected,object_selected,pList);
}

SMObject*
ReadEditPackage::construct(Chars& name, istream& istr) const
{
  Type::notype n;
  Type id(n);
  istr >> id;
  int i;
  istr >> i;
  Chars c;
  istr >> c;
  return new EditPackage( id,i, c );
}

SMObject*
ReadGroupPackageID::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMFPGroup*)a1, id );
}

SMObject*
ReadWordPackageID::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMWord*)a1, id );
}

SMObject*
ReadSubgroupPackageID::construct(SMObject* a1, Chars& name, istream& istr)const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMSubgroup*)a1, id );
}

SMObject*
ReadMapPackageID::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMMap*)a1, id );
}

SMObject*
ReadHomoPackageID::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMHomomorphism*)a1, id );
}

SMObject*
ReadWordWordPackageID::construct(SMObject* a1,SMObject* a2,
				 Chars& name, istream& istr) const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMWord*)a1, *(SMWord*)a2, id );
}

SMObject*
ReadSubgroupSubgroupPackageID::construct(SMObject* a1,SMObject* a2,
					 Chars& name, istream& istr) const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMSubgroup*)a1, *(SMSubgroup*)a2, id );
}

SMObject*
ReadSubgroupWordPackageID::construct(SMObject* a1,SMObject* a2,
				     Chars& name, istream& istr) const
{
  int id;
  istr >> id;
  return new RunPackage( *(SMSubgroup*)a1, *(SMWord*)a2, id );
}


//---------------------------------------------------------------------------//
//-------------------------- Menu::Action -----------------------------------//
//---------------------------------------------------------------------------//


void Menu::DefineFPGroup::print(ostream& ostr) const
{
  ostr << "_defineFPGroup_ {Define a finitely presented group}";
}


void Menu::DefineFreeGroup::print(ostream& ostr) const
{
  ostr << "_defineFreeGroup_ {Define a free group}";
}


void Menu::DefineAbelianGroup::print(ostream& ostr) const
{
  ostr << "_defineAbelGroup_ {Define an abelian group}";
}


void Menu::DefineNilpotentGroup::print(ostream& ostr) const
{
  ostr << "_defineNilpotentGroup_ {Define a nilpotent group}";
}

void Menu::DefineFreeNilpotentGroup::print(ostream& ostr) const
{
  ostr << "_defineFreeNilpotentGroup_ {Define a free nilpotent group}";
}

void Menu::DefineSmallCancGroup::print(ostream& ostr) const
{
  ostr << "_defineSCGroup_ {Define a small cancellation group}";
}


void Menu::DefineORGroup::print(ostream& ostr) const
{
  ostr << "_defineORGroup_ {Define an one-relator group}";
}

void Menu::DefinePermutation::print(ostream& ostr) const
{
  ostr << "_definePermutation_ {Define a permutation}";
}

void Menu::DefineWord::print(ostream& ostr) const
{
  ostr
	 << "_defineWord_ {Define a word in the generators of " << Name( 1 ) << '}';
}

void Menu::DefineEquation::print(ostream& ostr) const
{
  ostr
	 << "_defineEquation_ {Define an equation in " << Name( 1 ) << '}';
}

void Menu::DefineEquation2::print(ostream& ostr) const
{
  ostr
	 << "_defineEquation_ {Define an equation in " << Name( 1 ) << '}';
}

void Menu::DefineEqSystem::print(ostream& ostr) const
{
  ostr
    << "_defineSystemOfEquations_ {Define a system of equations in " << Name( 1 ) << '}';
}

void Menu::DefineSubgroup::print(ostream& ostr) const
{
  ostr
	 << "_defineSubgroup_ {Define a subgroup of " << Name( 1 ) << '}';
}

void Menu::DefineSetOfWords::print(ostream& ostr) const
{
  ostr
    << "_defineSetOfWords_ {Define a set of words in "
    << Name( 1 ) << '}';
}

void Menu::DefineVectorOfWords::print(ostream& ostr) const
{
  ostr
    << "_defineVectorOfWords_ {Define a tuple of words in "
    << Name( 1 ) << '}';
}


void Menu::DefineMap::print(ostream& ostr) const
{
  ostr << "_defineMap_ "<< Oid( 1 ) << " " << Oid( 1 );
}


void Menu::DefineMap2::print(ostream& ostr) const
{
  ostr << "_defineMap_ " << Oid( 1 ) << " " << Oid( 2 );
}


void Menu::DefineInverseMap2::print(ostream& ostr) const
{
  ostr << "_defineMap_ " << Oid( 2 ) << " " << Oid( 1 );
}


void Menu::DefineInt::print(ostream& ostr) const
{
  ostr
	 << "_defineInt_ {" << prompt << "} " << lower << ' ';
  if ( suppliedUpper )
	 ostr << upper;
  else
	 ostr << "{}";
}

void Menu::DefinePackage::print(ostream& ostr) const
{
  ostr << "_definePackage_ {Add new package } ";
  Packages::printObjectNames( ostr );
  Packages::printCheckinTypes( ostr );
}

void Menu::DefineEditPackage::print(ostream& ostr) const
{
  ostr << "_editPackage_ { Edit packages } ";
  Packages::printCheckinTypes( ostr );
  Packages::printObjectFileNames( ostr );
}

void Menu::PackageID::print(ostream& ostr) const
{
  ostr
    << "_packageID_ {package id " << "} " << theID;
}

void Menu::DefineInt2::print(ostream& ostr) const
{

  ostr << "_defineInt_ {" << prompt[0] << "} " << boundInt[0].lowerBound << ' ';
  if ( boundInt[0].haveUpperBound )
    ostr << boundInt[0].upperBound;
  else
	 ostr << "{}";

  return;

  //  for( int i = 0; i < 2; ++i ) {
  //
  //    cerr << "prompt[" << i << "] = " << prompt[i] << endl; // @dp debug print.
  //    int i = 0;
  //
  //    ostr << "_defineInt_ {" << prompt[i] << "} " << boundInt[i].lowerBound << ' ';
  //    if ( boundInt[i].haveUpperBound )
  //      ostr << boundInt[i].upperBound;
  //    else
  //      ostr << "{}";
  //    ostr << " ; ";
  //  }

}


void Menu::DefineSetOfRelators::print(ostream& ostr) const
{
  ostr << "_defineSetOfRelators_ {Define a set of words}";
}


//---------------------------------------------------------------------------//
//------------------------------ Menu<n> ------------------------------------//
//---------------------------------------------------------------------------//


Menu::Menu(Menu::MenuKind mk) : state( INIT )
{
  if ( mk == CHECKIN )
    cout << "menu_defn_ menu checkin signature {";
  else if( mk == TOOLS )
    cout << "menu_defn_ menu tools signature {";
  else if( mk == MAKE ) 
    cout << "menu_defn_ menu make signature {";
  else if( mk == DATABASE )
    cout << "menu_defn_ menu database signature {";
  else if( mk == PACKAGES )
    cout << "menu_defn_ menu packages signature {";
  else if( mk == TESTING )
    cout << "menu_defn_ menu testing signature {";
}


void Menu::startItemGroup( )
{
  if ( state == DONE ) error("Continued writing to closed menu");
  if ( state == ITEM_GROUP ) cout << "}} ";  // Close previous
  cout << "{1 {";
  state = ITEM_GROUP;
}


void Menu::startItemGroup(const Expression& E)
{
  if ( state == DONE ) error("Continued writing to closed menu");
  if ( state == ITEM_GROUP ) cout << "}} ";  // Close previous
  cout << "{{" << E << "} {";
  state = ITEM_GROUP;
}


void Menu::separator( )
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");
  cout << " s ";
}


void Menu::done( )
{
  if ( state == DONE ) error("Continued writing to closed menu");
  cout << "}}}" << endl << flush;
  state = DONE;
}


void Menu::add(Ctor* ctor, const Text& text, const Action& action)
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  cout << "{{" << text << "} {";
  action.print( cout );
  cout << "} {"
  
  // Do return message:

		 << SessionManager::getObjectSmithOid() << ' '
    
		 << ObjectSmith::registerCallBack( ctor )
  
		 << "}} ";
}


void Menu::addDisabled(const Text& text)
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  // @db comment out the next line to hide disabled menu items
  cout << "{{" << text << "} {} {}} ";
}


void Menu::startCascade(const Text& text)
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  cout << "{c {" << text << "} {";
}


void Menu::closeCascade( )
{
  if ( state != ITEM_GROUP )
	 error("Tried to add item to menu without starting itemGroup first");

  cout << "}} ";
}
#line 1  "src/ObjectSmith.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class ObjectSmith
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "ObjectSmith.h"


//---------------------------------------------------------------------------//
//---------------------------- ObjectSmith ----------------------------------//
//---------------------------------------------------------------------------//


int ObjectSmith::registerCallBack(Ctor* ctor)
{
  if ( freeCallBack - theCallBacks >= theCallBacksLen ) {
	 Ctor** temp = new (Ctor*)[theCallBacksLen + jumpSize];
	 for( int i = 0; i < theCallBacksLen; ++i )
		temp[i] = theCallBacks[i];
	 delete [] theCallBacks;
	 theCallBacks = temp;
	 freeCallBack = theCallBacks + theCallBacksLen;
	 theCallBacksLen += jumpSize;
  }

  *freeCallBack = ctor;
  return freeCallBack++ - theCallBacks;
}


void ObjectSmith::readMessage(istream& istr)
{
  int i;
  istr >> i;
  if ( i >= 0 && i < theCallBacksLen )
	 theCallBacks[i]->readFrom(istr);

  //@rn else perhaps warn
}


//---------------------------------------------------------------------------//
//---------------------------- ObjectSmith ----------------------------------//
//----------------------- static initializations ----------------------------//

int ObjectSmith::theCallBacksLen = 0;
Ctor** ObjectSmith::theCallBacks = 0;
Ctor** ObjectSmith::freeCallBack = 0;

// gcc 2.6.3 wets itself if I try anything more, so the rest of the
// init is done in ObjectSmith::registerCallBack.
#line 1  "src/OutMessages.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class OutMessage, and derivatives
//           ARCUpdate, CheckinMessage, FEDataUpdate, LogMessage,
//           ParseErrorMessage, StateTransition, Warning
//
// Principal Author: Roger Needham
//
// Status: useable
//
// Revision History:
//
// 01/25/98 @am added ParseParamErrorMessage class. It reports to
// the front end about parsing errors in parameters.

#include "SMObject.h"
#include "OutMessages.h"
#include "TheObjects.h"
#include "Timer.h"
#include "Set.h"
#include "TheObjects.h"
#include "ComputationManager.h"

//---------------------------------------------------------------------------//
//--------------------------- OutMessage ------------------------------------//
//---------------------------------------------------------------------------//

void OutMessage::send( ) const
{
  print( cout );
  cout << endl << flush;
}


//---------------------------------------------------------------------------//
//--------------------------- LogMessage ------------------------------------//
//---------------------------------------------------------------------------//


LogMessage::LogMessage(int message_level)
  : num_addressees( 0 ), level( message_level ),
    oids( 0 )
{ }


LogMessage::LogMessage(OID o, int message_level)
  : num_addressees( 1 ), level( message_level ), 
    oids( 1 )
{
  oids[0] = o;
}


LogMessage::LogMessage(OID o1, OID o2, int message_level)
  : num_addressees( 2 ), level( message_level ),
    oids( 2 )
{
  oids[0] = o1;
  oids[1] = o2;
}
	  

LogMessage::LogMessage(OID o1, OID o2, OID o3, int message_level)
  : num_addressees( 3 ), level( message_level ),
    oids( 3 )
{
  oids[0] = o1;
  oids[1] = o2;
  oids[2] = o3;
}


LogMessage::LogMessage(const char* msg, int message_level)
  : num_addressees( 0 ), level( message_level ),
    oids( 0 )
{
  *this << msg;
}


LogMessage::LogMessage(OID o, const char* msg, int message_level)
  : num_addressees( 1 ), level( message_level ),
    oids(1)
{
  oids[0] = o; 
  *this << msg;
}

void LogMessage::print(ostream& ostr) const
{
  ostrstream& o = (ostrstream&)*this;  // Cast away constness

  o << ends;
  ostr << "post_to_log_ {";

  SetOf<OID> bosses;
  for( int i = 0; i < num_addressees; ++i ) {
    OID o = oids[i];
    SMObject *worker = TheObjects::get( o );
    SetOf<OID> wbosses;
    if( worker->isComputationManager() ) 
      wbosses = ((ComputationManager *)worker)->getClients();
    else 
      wbosses |= o;
    bosses |= wbosses;
  }

  SetIterator<OID> I(bosses);
  for( ; !I.done(); I.next() ) {
	 ostr << I.value() << ' ';
  }

  ostr << "} {" << o.str() << "} " << level;
  o.freeze(0);
  
  //@rn could check for bogus characters, brace balance, etc.
}


//---------------------------------------------------------------------------//
//------------------------ ParseErrorMessage --------------------------------//
//---------------------------------------------------------------------------//


ParseErrorMessage::ParseErrorMessage(const Chars name, const Chars diagnosis)
  : theName( name ), theDiagnosis( diagnosis )
{ }


void ParseErrorMessage::print(ostream& ostr) const
{
  ostr
	 << "syntax_error_in_defn_ name {"
	 << theName
	 << "} errMesg {"
	 << theDiagnosis
	 << '}';
}


//---------------------------------------------------------------------------//
//------------------------ PackageInfoMessage     ---------------------------//
//---------------------------------------------------------------------------//


PackageInfoMessage::PackageInfoMessage(const Chars& name, 
				       const Chars& command,
				       const Chars& ch_type, 
				       const Chars& obj)  :
  theChType( ch_type ),
  theName( name ),
  theObject( obj ),
  theCommand( command )
{ }


void PackageInfoMessage::print(ostream& ostr) const
{
  ostr
    << "view_package_info_  "
    << " { " << theName << " } "
    << " { " << theCommand << " } "
    << " { " << theChType << " } "
    << " { " << theObject << " } ";\
}


//---------------------------------------------------------------------------//
//------------------------ ParseParamErrorMessage ---------------------------//
//---------------------------------------------------------------------------//


ParseParamErrorMessage::ParseParamErrorMessage(OID o,const Chars& name, 
					       const Chars& type,
					       const Chars& msg)
  : theOID( o ), theName( name ), theType( type  ), theMsg( msg )
{ }


void ParseParamErrorMessage::print(ostream& ostr) const
{
  ostr
    << "syntax_error_in_param_ OID {"
    << theOID 
    << "} name {"
    << theName
    << "} type {"
    << theType
    << "} errMesg {"
    << theMsg
    << '}';
}

//---------------------------------------------------------------------------//
//------------------------- CheckinMessage ----------------------------------//
//---------------------------------------------------------------------------//

//@rn This might just as well take a vector of dependencies.

CheckinMessage::CheckinMessage(const SMObject& smo,
			       const Chars name,
			       const ListOf<OID> dependencies,
			       bool bPrintDefinition
			       )
  : theSMO( smo ),
    theName( name ),
    theDependencies( dependencies ),
    BPrintDefinition( bPrintDefinition )
{ }


void CheckinMessage::print(ostream& ostr) const  // overrides OutMessage
{
  //@db report dependents to TheObjects
  {
    ListIterator< ListOf<OID> > I( theDependencies );
    while ( ! I.done() ) {
      SMObject* smo  = TheObjects::get(OID(I.value()));
      smo->addDependent( theSMO.oid() );
      I.next();
    }
  }

  ostr << "create_ objectID " << theSMO.oid()
       << " typeID " << theSMO.typeID()
       << " iconID " << theSMO.iconID()
       << " name {" << theName
       << "} dependencies {";
  
  ListIterator< ListOf<OID> > I( theDependencies );
  while ( ! I.done() ) {
    ostr << ' ' << I.value();
    I.next();
  }
  
  ostr << "} properties {";
  
  theSMO.printProperties( ostr );

  ostr << "} viewStructure {";
  
  theSMO.viewStructure( ostr );
  
  ostr << '}';
  
  if ( theName.length() == 0 ) {

    // By convention, when an object is created internally, it is not
    // given a name. This is how we know to echo the definition.
    //@rn Shouldn't we always print defn, for double-check?
    
    ostr << " definition {";
    if( BPrintDefinition )
      theSMO.printDefinition( ostr );
    ostr << '}';
  }
}


//---------------------------------------------------------------------------//
//-------------------------- FEDataUpdate -----------------------------------//
//---------------------------------------------------------------------------//

void FEDataUpdate::print(ostream& ostr) const  // overrides OutMessage
{
  ostr << "new_data_ key {" << theKey << "} datum {" << theDatum << '}';
}


//---------------------------------------------------------------------------//
//------------------------- StateTransition ---------------------------------//
//---------------------------------------------------------------------------//

void StateTransition::print(ostream& ostr) const
{
  if ( kind == START )
	 ostr << "confirm_start_";
  else if ( kind == SUSPEND )
	 ostr << "confirm_suspend_";
  else if ( kind == RESUME )
	 ostr << "confirm_resume_";
  else if ( kind == TERMINATE )
	 ostr << "terminate_problem_";
  else if ( kind == STALLED )
	 ostr << "problem_stalled_";

  ostr << " viewID " << oid;
}


//---------------------------------------------------------------------------//
//---------------------------- ARCUpdate ------------------------------------//
//---------------------------------------------------------------------------//

void ARCUpdate::print(ostream& ostr) const
{
  ostr
	 << "arc_update_ viewID " << oid
	 << " ARCSlotID " << asi
	 << " value " << value;
}


//---------------------------------------------------------------------------//
//----------------------------- Warning -------------------------------------//
//---------------------------------------------------------------------------//

void Warning::print(ostream& ostr) const
{
  // Partial safeguard against repeated warnings from within a loop:

  static Timer timer( 0 );   // First call should go out

  if ( timer.expired() ) {

	 ostrstream& o = (ostrstream&)*this;  // Cast away constness

	 o << ends;
	 ostr << "warn_ {" << o.str() << '}';

	 timer.reset( 1000 );
  }
}


//----------------------------- Message -------------------------------------//


void Message::print(ostream& ostr) const
{
  ostrstream& o = (ostrstream&)*this;  // Cast away constness
  o << ends;
  ostr << "tk_dialog .d {" << theTitle << "} {" << o.str() << "} warning 0 OK";
//  ostr "alert {" << o.str() << '}';
}


//----------------------------- InvokingMessage ------------------------------//


void InvokingMessage::print(ostream& ostr) const
{
  ostrstream& o = (ostrstream&)*this;  // Cast away constness
  o << ends;
  ostr << o.str();
}


#line 1  "src/ResourceManager.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class ResourceManager
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "ResourceManager.h"
#include "OutMessages.h"
#include "TheObjects.h"
#include "SMObject.h"
#include "ComputationManager.h"


#define debug_RM


//---------------------------------------------------------------------------//
//------------------------- ResourceManager ---------------------------------//
//---------------------------------------------------------------------------//


ResourceManager::ResourceManager(OID oid) :
  toUse(NULL),
  lastDrawnFrom(NULL),
  theOID(oid)
{ }


ResourceManager::~ResourceManager( )
{
  delete toUse;
}


OID ResourceManager::getOID( ) const
{
  return theOID;
}


ARC ResourceManager::freeARCs( ) const
{
  // Of course we could keep a running total, instead of computing it
  // each time.

  ARC result = ZeroARC();
  Resources* p = toUse;
  while ( p ) {
	 if ( isLiquid( p->benefactor ) )
		result += p->theARCs;
	 p = p->next;
  }
  return result;
}


bool ResourceManager::workingFor(OID oid) const
{
  // See ComputationManager::checkForStall(). This checks whether this
  // CM has ever had ARCs with benefactor oid, and has any free ARCs now.

  if ( freeARCs() == 0 ) return false;

  Resources* p = toUse;
  while ( p ) {
	 if ( p->benefactor == oid ) return true;
	 p = p->next;
  }
  return false;
}


bool ResourceManager::isNeeded() const
{
  Resources* p = toUse;
  while ( p ) {

    SMObject* smo = TheObjects::get(p->benefactor);
    
    if ( (smo) && smo->isComputationManager() ) {

      ComputationManager* CM = (ComputationManager*)smo;
      if( CM->state() == ComputationManager::RUNNING ||
	  CM->state() == ComputationManager::SUSPENDED )
	return true;
    }

    p = p->next;
  }

  return false;
}


void ResourceManager::freeze( )
{
  // For possible future use. Now, we tell whether a CM is frozen by
  // looking at its state.
}


void ResourceManager::liquefy( )
{
  // For possible future use. Now, we tell whether a CM is frozen by
  // looking at its state.
}


void ResourceManager::allocate(ResourceManager& recipient, ARC arcs)
{
  // Decide which of our Resources to take ARCs from:

  ARC give;
  ARC amount_left = arcs;
  Resources* p = toUse;

#ifdef debug_RM
  cerr
	 << "\n\n" << theOID << " is about to allocate " << arcs
	 << " in " << recipient.theOID << endl;
#endif

  while ( p ) {

	 // Don't fool around yet. Just grab the first ARCs we see.

#ifdef debug_RM
	 cerr
		<< "Looking at Resources:\nFrom: " << p->benefactor
		<< "\nASI: " << p->theARCSlotID
		<< "\nARCs: " << p->theARCs
		<< "\nState: ";
	 if ( isLiquid( p->benefactor ) ) cerr << "Liquid\n\n";
	 else  cerr << "Frozen\n\n";
#endif

	 if ( isLiquid( p->benefactor ) ) {

		if ( p->theARCs >= amount_left ) {
		  p->theARCs -= amount_left;
		  give = amount_left;
		  amount_left = ZeroARC();
		} else {
		  amount_left -= p->theARCs;
		  give = p->theARCs;
		  p->theARCs = ZeroARC();
		}

		// Note that we do not remove zeroed Resources, since they are
		// liable to be added to later.

		// Tell the recipient:

		recipient.acceptAllocation(p->benefactor, p->theARCSlotID, give);

		//@rn  In fact, need to distinguish between ARCSlotID-wise
		//@rn  benefactor, and the freeze-wise benefactor...

		if ( amount_left == 0 ) break;
		p = p->next;
	 }
  }

  if ( amount_left != 0 ) {
	 Warning msg;
	 msg << "ResourceManager::allocate tried to transfer too much; ignoring";
	 msg.send();
  }
}


void ResourceManager::acceptAllocation(OID benefactor,
													ARCSlotID asi,
													ARC arcs,
													bool overrides
													)
{
#ifdef debug_RM
  cerr << theOID << " is accepting " << arcs << " ARCs from "
		 << benefactor << " for ARCSlotID " << asi;
  if ( overrides ) cerr << " (overrides)";
  cerr << endl << endl;
#endif

  ARC change = arcs;

  // See if we already have Resources from the benefactor.

  Resources* p = toUse;
  while ( p ) {
	 if ( p->benefactor == benefactor && p->theARCSlotID == asi ) break;
	 p = p->next;
  }

  if ( p ) {  // Have one

	 if ( overrides ) {
		change -= p->theARCs;
		p->theARCs = arcs;
	 } else
		p->theARCs += arcs;

  } else {  // Make new Resources

	 toUse = new Resources(benefactor, asi, arcs, toUse);
  }
}


void ResourceManager::usedARCs(ARC arcs)
{
#ifdef debug_RM
  Resources* q = toUse;
  cerr << "\nHere are all Resources in ResourceManager::usedARCs, for "
		 << theOID << ':' << endl << endl;
  while ( q ) {
	 cerr
		<< "From: " << q->benefactor
		<< "\nASI: " << q->theARCSlotID
		<< "\nARCs: " << q->theARCs
		<< "\nState: ";
	 if ( isLiquid( q->benefactor ) )
		cerr << "Liquid\n\n";
	 else
		cerr << "Frozen\n\n";

	 q = q->next;
  }
#endif

  if ( freeARCs() >= arcs ) { // Internal check

	 ARC used;
	 ARC amount_left = arcs;

#ifdef debug_RM
	 cerr << "\n\n" << theOID << " is about to use " << arcs << endl;
#endif

	 do {
		do {
		  if ( lastDrawnFrom )
			 lastDrawnFrom = lastDrawnFrom->next;
		  if ( ! lastDrawnFrom )
			 lastDrawnFrom = toUse;
		} while (
					! isLiquid( lastDrawnFrom->benefactor ) ||
					lastDrawnFrom->theARCs == 0
					); // Should not loop

#ifdef debug_RM
		cerr
		  << "Looking at Resources:\nFrom: " << lastDrawnFrom->benefactor
		  << "\nASI: " << lastDrawnFrom->theARCSlotID
		  << "\nARCs: " << lastDrawnFrom->theARCs
		  << "\nState: ";
		if ( isLiquid( lastDrawnFrom->benefactor ) )
		  cerr << "Liquid\n\n";
		else
		  cerr << "Frozen\n\n";
#endif

		if ( lastDrawnFrom->theARCs >= amount_left ) {
		  used = amount_left;
		  lastDrawnFrom->theARCs -= used;
		  amount_left = ZeroARC();
		} else {
		  used = lastDrawnFrom->theARCs;
		  amount_left -= used;
		  lastDrawnFrom->theARCs = ZeroARC();
		}

		// Send ARC update

		ARCUpdate update(lastDrawnFrom->benefactor,
							  lastDrawnFrom->theARCSlotID,
							  used
							  );

		update.send();

	 } while ( amount_left > 0 ); // Should not loop

  } else {
    //@rn Let's not crash just yet, but just warn.

    if( ((ComputationManager*)this)->crashed() ) return; 
    //@db in case of crash let supervisor worry about warnings
    
    Warning msg;
    msg << "CM " << theOID << " tried to draw " << arcs
	<< " ARCs, when it has only " << freeARCs() << " free; ignoring";
    msg.send();
  }
}


void ResourceManager::usedOneARC( )
{
  usedARCs( OneARC() );
}


bool ResourceManager::isLiquid(OID oid) const
{
  return TheObjects::isRunning( oid );
}
#line 1  "src/SMFPGroup.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMFPGroup
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "SMFPGroup.h"
#include "viewStructure.h"


//---------------------------------------------------------------------------//
//--------------------------- SMFPGroup -------------------------------------//
//---------------------------------------------------------------------------//

SMFPGroup::SMFPGroup(const FPGroup& G, const Chars heritage, Checkin_Type t,
		     Chars def)
  : AlgebraicObject( heritage ),
         theGroup( G ),
	 theFreePreimage( G.namesOfGenerators() ),
	 gic( oid() ),
	 theGCM( new GCM( *this ) ),
	 checkin_type( t ),
         definition( def )
{
  addDependent( *theGCM );
}


void SMFPGroup::printProperties(ostream& ostr) const
{
  ostr << DataPair( CheckinType( *this ), checkin_type )
       << DataPair( Oid( *this ), Object( *this ) );
}


void SMFPGroup::viewStructure(ostream& ostr) const // overrides SMObject
{
  ObjectView( ostr, oid(), heritage() );
}


void SMFPGroup::printDefinition(ostream& ostr) const // overrides SMObject
{
  //@rn  Problem for things like abelian and nilpotent presentations, since
  //@rn  the commutators are not stored as relators.
  
  if( definition.length() == 0) {
    ostr << theGroup;
  } else {
    ostr << definition;
  }
}  


Chars SMFPGroup::getCheckinTypeStr( ) const
{
  switch ( checkin_type ) {
    
  case FP :                 return "FP";
  case FREE :               return "FREE";
  case ABELIAN :            return "ABELIAN";
  case NILPOTENT :          return "NILPOTENT";
  case SMALL_CANCELLATION : return "SMALL_CANCELLATION";
  case ONE_RELATOR :        return "ONE_RELATOR";
  case ONE_RELATOR_WITH_TORSION : return "ONE_RELATOR_WITH_TORSION";
  case AP_FREE :            return "AP_FREE";
  case AP_FREE_CYCLIC :     return "AP_FREE_CYCLIC";
  case FREE_NILPOTENT :     return "FREE_NILPOTENT";
  case HNN_FREE :           return "HNN_FREE";
  default:    error("Chars SMFPGroup::getCheckinTypeStr( ) const : "
	    "unknown chekin type.");
  }
}

void SMFPGroup::readMessage(istream& istr)
{
  MessageTag tag;
  
  istr >> (int&)tag;

 

  if ( tag == GENS_MAP_REQUEST ) {
    Chars msg;
    int cursorPos = 0;
    int oid2;
    Chars out;

    istr >> oid2;
    istr > msg;
 
    if ( getFPGroup().numberOfGenerators() > 0) {
      VectorOf<Chars> v = getFPGroup().namesOfGenerators();
      
      for (int i=0;i<v.length()-1;i++) {
	out += Chars(" ") + v[i] + " ->  ,\\n";
      }
      out +=  Chars(" ") + v[v.length()-1] + " ->  \\n";
      cursorPos =  v[0].length() + 5;
    }
    cout << "defineMapDialog {Define a map from " << Name( *this ) << " to "
	 << Name( OID(oid2) ) << "} " << '{' << msg << "} "<< "{} " << "\"{\\n"
	 << out  << "}\" " << "{} " << cursorPos; 
    cout << endl << flush;
  }
}
#line 1  "src/SMMap.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMMap
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "SMMap.h"

// ------------------------------- MCM ------------------------------------- //


MCM::MCM( class SMMap& M )
  : Supervisor( false ),
    theMap( M ),
    abelianHomIsMono( *this ),
    abelianHomIsEpi( *this )
{
  adminStart();
}


void MCM::takeControl( )
{
  if ( ! didFastChecks ) {
    
    didFastChecks = true;
    
    // Cheat and do some quickie checks here without reporting ARC consumption.
  }
}

//---------------------------------------------------------------------------//
//------------------------------ SMMap --------------------------------------//
//---------------------------------------------------------------------------//

SMMap::SMMap(SMFPGroup& G, Map m, const Chars heritage)
  : AlgebraicObject( heritage ),
	 theMap( m ),
	 theDomain( G ),
	 theRange( G ),
	 mic( oid() ),
         theMCM( new MCM( *this ) )
{
  addDependent( *theMCM );
}

SMMap::SMMap(SMFPGroup& G, SMFPGroup& H, Map m, const Chars heritage)
  : AlgebraicObject( heritage ),
    theMap( m ),
    theDomain( G ),
    theRange( H ),
    mic( oid() ),
    theMCM( new MCM( *this ) )
{
  addDependent( *theMCM );
}

SMMap::SMMap(SMFPGroup& G)
: AlgebraicObject(""), theMap( *((Map*)0) ), theDomain( G ), theRange( G ),
    mic( oid() ),theMCM( new MCM( *this ) )
{
  addDependent( *theMCM );
} 

SMMap::SMMap(SMFPGroup& G, SMFPGroup& H)
: AlgebraicObject(""), theMap( *((Map*)0) ), theDomain( G ), theRange( H ),
  mic( oid() ),theMCM( new MCM( *this ) )
{
  addDependent( *theMCM );
} 

void SMMap::printProperties(ostream& ostr) const
{
  ostr << DataPair( Domain( *this ), Object( theDomain ) )
		 << DataPair( Range( *this ), Object( theRange ) );
}


void SMMap::viewStructure(ostream& ostr) const
{
  ostr
	 << "viewBuilder objectView viewParameters {viewID "
	 << oid()
	 << " heritage {"
	 << heritage()
	 << "}}";
}


void SMMap::printDefinition(ostream& ostr) const
{
  ostr << "{ ";
  theMap.printOn( ostr );
  ostr << " } ";
}

#line 1  "src/SMObject.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMObject, and helper IconID
//
// Principal Author: Roger Needham
//
// Status: useable
//
// Revision History:
//


#include <string.h>
#include "SMObject.h"
#include "TheObjects.h"


//---------------------------------------------------------------------------//
//---------------------------- SMObject -------------------------------------//
//---------------------------------------------------------------------------//


SMObject::SMObject(bool is_cm)
  : theOid( TheObjects::reserveOid() ), isCM( is_cm )
{
  TheObjects::enroll( this );
}



//---------------------------------------------------------------------------//
//------------------------------ IconID -------------------------------------//
//---------------------------------------------------------------------------//

bool IconID::operator == ( const IconID& iid ) const
{
  return strcmp( theName, iid.theName ) == 0;
}


bool IconID::operator != ( const IconID& iid ) const
{
  return strcmp( theName, iid.theName ) != 0;
}


//---------------------------------------------------------------------------//
//------------------------------ IconID -------------------------------------//
//----------------------- static initializations ----------------------------//

const IconID IconID::group = IconID("group");
const IconID IconID::subgroup = IconID("subgroup");
const IconID IconID::SetOfWords = IconID("SetOfWords");
const IconID IconID::VectorOfWords = IconID("VectorOfWords");
const IconID IconID::elt = IconID("elt");
const IconID IconID::equation = IconID("equation");
const IconID IconID::systemOfEquations = IconID("systemOfEquations");
const IconID IconID::map = IconID("map");
const IconID IconID::homomorphism = IconID("homomorphism");
const IconID IconID::permutation = IconID("permutation");
const IconID IconID::enumerator_problem = IconID("enumerator_problem");
const IconID IconID::enumerator_object = IconID("enumerator_object");
const IconID IconID::list_object = IconID("list_object");
const IconID IconID::problem = IconID("problem");
const IconID IconID::none = IconID("none");
const IconID IconID::do_not_display = IconID("do_not_display");
#line 1  "src/SMWord.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMWord
//
// Principal Author: Roger Needham, Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//


#include "SMWord.h"

// ---------------------- WIC ----------------------------------------------//
WIC::WIC(OID word_oid) :
  wordOID( word_oid )
  //,
  //  ispower( dontknow ),
  //  is_trivial( dontknow ),
  //  collected( NULL ),
  //  order( -1 )
{
}

WIC::~WIC( ) {
  //  if(collected != NULL) delete collected;
}

//============================================================
//
// Property: IsTrivial
//
//============================================================

Trichotomy WIC::isTrivial() const 
{
  return haveProperty( IsTrivialProperty::type() );
}

// Yes if word is trivial, no if it is not.
Trichotomy WIC::isTrivial(Chars& expl)
{
  Trichotomy havep = haveProperty( IsTrivialProperty::type() );

  if( havep == yes ) 
    setKnownProperty( IsTrivialProperty(expl) );
  else if( havep == no ) 
    setHaveNotProperty( IsTrivialProperty::type(), expl ); //???

  return havep;
  // Old code:
  //  expl = explOfIsTrivial;
  //  return is_trivial;
}

// Yes if word is trivial, no if it is not.
void WIC::putIsTrivial(bool istrivial, const Chars& expl)
{
  if( istrivial )
    setKnownProperty( IsTrivialProperty(expl) );
  else 
    setHaveNotProperty( IsTrivialProperty::type(), expl );  

  if( istrivial )
    setKnownProperty( WordOrderProperty(1) );

  //  is_trivial = istrivial;
  //  explOfIsTrivial = expl;
  //  if (is_trivial)
  //    order = 1;
}

//============================================================
//
// Property: Have Order 
//
//============================================================

bool WIC::haveOrder( ) const 
{ 
  return haveProperty( WordOrderProperty::type() ) == yes;
}

void WIC::putHaveOrder (const Integer& o) 
{
  setKnownProperty( WordOrderProperty(o) );
  if( o == 1 ) {
    setKnownProperty( IsTrivialProperty(" because it's order is 1") );
  }
  else {
    setHaveNotProperty( IsTrivialProperty::type(),
			( o > 0
			  ? " because it's order is bigger, than 1"
			  : " because it's order is infinite"
			)
		      );

  }
}

Integer WIC::getOrder( ) const 
{
  if( !haveOrder() ) 
    return -1;

  return ((const WordOrderProperty *)
	  getKnownProperty( WordOrderProperty::type() ) ) -> value();
}

//============================================================
//
// Property: Maximal Root
//
//============================================================

// True if maximal root problem, was solved
Trichotomy WIC::isPower() const
{
  return haveProperty( MaximalRootProperty::type() );
}

// True if word has the maximal root
bool WIC::haveMaximalRoot() const
{ 
  return haveProperty( MaximalRootProperty::type() ) == yes;
}

// Puts the solution of maximal root problem
void WIC::putHaveMaximalRoot(const Word& root, const Integer& power)
{
  if( power == 0 ) {
    setHaveNotProperty( MaximalRootProperty::type(), "" );
  }
  else {
    setKnownProperty( MaximalRootProperty (root)  );
    setKnownProperty( MaximalPowerProperty(power) );
  }
}


void WIC::getMaximalRoot(Word& root, Integer& power) const
{
#if SAFETY > 0
  {
    Trichotomy havep = haveProperty(MaximalRootProperty::type());
    if( havep == dontknow ) {
      error( "void getMaximalRoot(Word&, Integer&) const : "
	     "maximal root was not computed yet." );
    }
    if( havep == no ) {
      error( "void getMaximalRoot(Word&, Integer&) const : "
	     "Word is not a power." );
    }
  }
#endif

  root = ((const MaximalRootProperty*)
	  getKnownProperty( MaximalRootProperty::type() ) ) -> value();

  power = ((const MaximalPowerProperty*)
	   getKnownProperty( MaximalPowerProperty::type() ) ) -> value();
}

Integer WIC::getMaximalExponent() const 
{ 
  return ((const MaximalPowerProperty *)
	  getKnownProperty( MaximalPowerProperty::type())) -> value();
}

//============================================================
//
// Property: Collected Form
//
//============================================================


// True if have cannonical decomposition in nilpotent
bool WIC::haveCollectedForm() const 
{
  return haveProperty( CollectedFormProperty::type() ) == yes;
  //return collected != NULL; 
}

void WIC::putHaveCollectedForm( const PolyWord& p )
{
  if( haveCollectedForm() )
    error("WIC::putHaveCollectedForm(): collected form already computed");

  setKnownProperty( CollectedFormProperty(p) );
}

const PolyWord& WIC::getCollectedForm() const
{
  if( !haveCollectedForm() )
    error("WIC::getCollectedForm(): collected form not computed");

  return ((const CollectedFormProperty *)
	  getKnownProperty(CollectedFormProperty::type())) -> value();
}

//---------------------------------------------------------------------------//
//-------------------- Static members of new properties ---------------------//
//---------------------------------------------------------------------------//

const PropertyType IsTrivialProperty::theTypeID = GenericProperty::unique();
const PropertyType WordOrderProperty::theTypeID = GenericProperty::unique();
const PropertyType MaximalRootProperty::theTypeID = GenericProperty::unique();
const PropertyType MaximalPowerProperty::theTypeID = GenericProperty::unique();
const PropertyType CollectedFormProperty::theTypeID=GenericProperty::unique();

//---------------------------------------------------------------------------//
//---------------------------- SMWord ---------------------------------------//
//---------------------------------------------------------------------------//

SMWord::SMWord(SMFPGroup& G, const Word w, const Chars heritage)
  : AlgebraicObject( heritage ),
    wic( oid() ),
    theGroup( G ),
    theWord( w ),
    theWCM( new WCM( *this ) )
{
  addDependent( *theWCM );
}


void SMWord::printProperties(ostream& ostr) const
{
  ostr << DataPair( Parent( oid() ), Object( theGroup ) );
}


void SMWord::viewStructure(ostream& ostr) const
{
  ostr
	 << "viewBuilder objectView viewParameters {viewID "
	 << oid()
	 << " heritage {"
	 << heritage()
	 << "}}";
}


void SMWord::printDefinition(ostream& ostr) const
{
  theGroup.getFPGroup().printWord( ostr, theWord );
}
// ------------------------------- WCM ------------------------------------- //


WCM::WCM( class SMWord& W )
  : Supervisor( false ),
    ngDecomposeWord(*this),
    theWord( W )
{
  adminStart();
}


void WCM::takeControl( )
{
  if ( ! didFastChecks ) {
    
    didFastChecks = true;
    
    // Cheat and do some quickie checks here without reporting ARC consumption.
  }
}
#line 1  "src/SessionManager.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class SessionManager
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//
 


//#include "MagnusHome.h"
#include "SessionManager.h"
#include "TheObjects.h"
#include "ComputationManager.h"
#include "Timer.h"
#include "DatabaseManager.h"
#include "RandomDefinitionsGenerator.h"

extern DatabaseManager TheDatabaseManager;

// Need this to init SessionManager::magicCookie :

//@db system porting

#ifdef BSD
extern "C" int getpid( );
#elif defined(SYSV)
extern "C" long int getpid( );
#endif



//---------------------------------------------------------------------------//
//---------------------------- Buffers --------------------------------------//
//---------------------------------------------------------------------------//

// Helper class for entries in a chainable list of char* :

struct Buffers {
  Buffers( ) : buffers( new char*[size] ), index(0), next(0) { }
  ~Buffers( ) {
    for( int i = 0; i < index; ++i ) delete [] buffers[i];
    delete [] buffers;
    delete next;
  }
  Buffers* store(char* p) {
    buffers[index++] = p;
    if ( index == size ) {
      next = new Buffers;
      return next;
    }
    else
      return this;
  }
  void dump(char* p, int bufferSize) {
    for( int i = 0; i < index; ++i ) {
      for( int j = 0; j < bufferSize - 1; ++j )   // Skip \0's
        *p++ = buffers[i][j];
    }
    if ( next != 0 ) next->dump(p, bufferSize);
  }
  static const int size = 128;
  char** buffers;
  int index;
  Buffers* next;
};


//---------------------------------------------------------------------------//
//------------------------- SessionManager ----------------------------------//
//---------------------------------------------------------------------------//


void SessionManager::start( )
{
  initializeFrontEnd();

  while ( 1 ) {

	 readMessages();

	 TheObjects::passControl();

	 takeControl();
  }
}


void SessionManager::readMessages( )
{
  while ( areMessages() ) {

	 // To protect the message stream, we read each message (which consists
	 // of a single line) into a char* and make an istrstream of that.

	 // Get the message:

	 const int bufferSize = 1024;
	 char buffer[bufferSize];        // Don't whip the heap for small messages
	 char* realBuffer = buffer;

	 cin.getline(buffer, bufferSize);
  
	 if ( cin.fail() ) {

		if ( (int)(*buffer) == 0 ) return;
		// When debugging we might redirect input from a file, which results
		// in a fail state when we get to its end. In this case, *buffer == 0

		Buffers* moreBuffers = new Buffers;
		Buffers* curBuffers = moreBuffers;
		int bufferCount = 1;

		do {
		  if ( cin.bad() )
			 error("The front end message stream has gone bad");
		  //@rn Take better emergency action
  
		  cin.clear();
		  char* tmp = new char[bufferSize];
		  cin.getline(tmp, bufferSize);
		  curBuffers = curBuffers->store(tmp);
		  ++bufferCount;
		} while ( cin.fail() );

		realBuffer = new char[bufferSize * bufferCount];
		for( int i = 0; i < bufferSize; ++i ) realBuffer[i] = buffer[i];
		moreBuffers->dump(realBuffer + bufferSize - 1, bufferSize); // Stomp \0
		delete moreBuffers;
	 }

	 // We politely ignore blank messages from the FE:

	 if ( (int)(*buffer) == 0 ) return;

    #ifdef DEBUG
	   cerr << "--> " << realBuffer << endl << flush;
    #endif

	 istrstream message(realBuffer);

	 // Process the raw message

	 int recipient;
	 message >> recipient;  //@rn Check for stream going bad

	 if ( recipient < 0 ) {
		// Special case
	  
		if ( recipient == sessionManagerOid )
		  readMessage( message );
		else if ( recipient == objectSmithOid )
		  ObjectSmith::readMessage( message );
		else if ( recipient == databaseManagerOid )
		  TheDatabaseManager.readMessage( message );
		else if ( recipient == randomDefnGeneratorOid ){
		  RandomDefinitionsGenerator::readMessage( message );
		}
		else
		  //@rn Warn of misdirected message
		  ;

	 } else {

		SMObject* smo = TheObjects::get(recipient);
		if ( smo ) {
		  smo->readMessage( message );
		} else {
		  //@rn Warn of misdirected message
		}
	 }

    // Clean up

	 if ( realBuffer != buffer ) delete realBuffer;
	 // Don't do this sooner! message is still using it.
  }
}



bool SessionManager::areMessages( )
{
  static Timer timer( 200 );

  if ( ! timer.expired() ) return false;

  timer.reset( 200 );

  // Can you BELIEVE this kludgery?
  // Of course, main sets stdin to non-blocking.

  int ch = getchar();
  if ( ch == EOF ) {
	 clearerr(stdin);
	 return 0;
  } else {
	 ungetc(ch, stdin);
	 return 1;
  }
}



void SessionManager::readMessage(istream& istr)
{
  MessageTag tag;

  istr >> (int&)tag;

  if ( tag == DELETE ) {

	 // Read list of OIDs to delete. Cumulate the dependency closure.

	 SetOf<OID> deleted;

	 int len, oid;

	 istr >> len;

	 while ( len-- ) {
		istr >> oid;
		deleted |= TheObjects::remove( OID( oid ) );
	 }

	 // Confirm the deletions.

	 cout << "confirm_delete_";

	 SetIterator<OID> I( deleted );
	 while ( !I.done() ) {
		cout << ' ' << I.value();
		I.next();
	 }

	 cout << endl << flush;

  } else if ( tag == QUIT ) {

    //@rn Implement

    if( !quitRequested ) {
      quitRequested = true;
      TheDatabaseManager.forceToFinish();
    }

  //@db experiment
 
  } else if ( tag == SET_NAME ) {
    
    int oid;
    Chars name;
    
    istr >> oid >> name;
    TheObjects::setName(oid,name);

  } else {

	 //@rn Warn of bad tag

  }

  
}

void SessionManager::takeControl( )
{
  TheDatabaseManager.takeControl();

  if( quitRequested ) {

    static bool userWarned = false;

    if( TheDatabaseManager.state() == DatabaseManager::STOPPED ) {

      for( int j = 0; j < TheObjects::theObjectsLength; ++j ) {
	SMObject* smo = TheObjects::theObjects[j];
	if ( smo != 0 && smo->isComputationManager()
	     && ((ComputationManager*)smo)->state()
	     == ComputationManager::RUNNING )
	  ((ComputationManager*)smo)->adminTerminate();
      }
      cout << "fakeQuit" << endl;

    }
    else {

      if( !userWarned ){
	Message msg;
	msg << 
	  "The database is modified. Please save the changes on the disk or "
	  "discard them by closing the database. After that the quitting "
	  "will be completed.";
	msg.send(); 
	userWarned = true;
      }

      if( TheDatabaseManager.state() != DatabaseManager::STOPPING ) {
	quitRequested = false;
      }
    }
  }

}


void SessionManager::initializeFrontEnd( )
{
  //------------------------------ Greeting ---------------------------------//

  cout << "magic_cookie_ " << magicCookie << endl;


  //-------------------------- Global Templates -----------------------------//

  cout
	 << "init_quit_msg_ {" << sessionManagerOid << ' ' << QUIT << '}' << endl

	 << "init_delete_msg_ {" << sessionManagerOid << ' ' << DELETE << '}' << endl

	 << "init_setName_msg_ {" << sessionManagerOid << ' ' << SET_NAME << '}' << endl;

  ComputationManager::printGlobalMessageTemplates( cout );
  SMFPGroup::printGlobalMessageTemplates( cout );
  DatabaseManager::printGlobalMessageTemplate( cout );

  //----------------------------- Type Names --------------------------------//

  cout
	 << "type_name_ {finitely presented group} G " << SMFPGroup::type() << endl
	 << "type_name_ {word} w " << SMWord::type() << endl
	 << "type_name_ {subgroup} H " << SMSubgroup::type() << endl
	 << "type_name_ {map} m " << SMMap::type() << endl
	 << "type_name_ {equation} E " << SMEquation::type() << endl
	 ;


  //------------------------------- Menus -----------------------------------//

  ObjectSmith::outputCheckinMenuDefns( cout );
  ObjectSmith::outputToolsMenuDefns( cout );
  ObjectSmith::outputMakeMenuDefns( cout );
  ObjectSmith::outputPackagesMenuDefns( cout );
  ObjectSmith::outputTestingMenuDefns( cout );

  //----------------------- Properties Registration  ------------------------//

  registerProperties();
  
  //--------------------------- Init Complete -------------------------------//
  
  cout << "boot_done_" << endl << flush;
  
}


//---------------------------------------------------------------------------//
//------------------------- SessionManager ----------------------------------//
//--------------------- static initializations ------------------------------//

const long SessionManager::magicCookie = getpid();
bool SessionManager::quitRequested = false;
#line 1  "src/Supervisor.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of mixin class Supervisor and helpers
//
// Principal Author: Roger Needham, Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "Supervisor.h"
#include "OutMessages.h"


//---------------------------------------------------------------------------//
//--------------------------- Subordinate -----------------------------------//
//---------------------------------------------------------------------------//


SubordinateBase::SubordinateBase(Supervisor& boss)
  : asi( boss.hire( this ) )
{ }


void SubordinateBase::addDependent( SMObject& smo, OID oid ) const
{
  smo.addDependent( oid );
}


MirrorSubordinate::MirrorSubordinate(Supervisor& boss, SubordinateBase& worker)
  : SubordinateBase( boss ),
    theBoss( boss ),
    mirror( worker ),
    firstAccessToWorker( true )
{
/*
  //@@db
  ComputationManager* pWorker;
  if( pWorker = worker.getWorker() )
    pWorker->adminStart();
*/
}


void MirrorSubordinate::acceptAllocation(OID oid, ARCSlotID asi, ARC arc,
					 bool overrides)
{
  mirror.getBoss().forwardAllocation(oid,mirror.arcSlotID(),asi,arc,overrides);

  ComputationManager *newWorker = mirror.getWorker();
  if( firstAccessToWorker && newWorker ) {
    newWorker->attachClient( theBoss );
    firstAccessToWorker = false;
  }
}
 
ComputationManager* MirrorSubordinate::getWorker( ) const 
{ 
  return mirror.getWorker(); 
}

void MirrorSubordinate::deleteWorker() 
{ 
  if( exists() ) {
    getWorker()->detachClient( theBoss );
  }
}

void MirrorSubordinate::terminateWorker( )
{
  ComputationManager* worker = mirror.getWorker();
  if( worker )
    if( !worker->isNeeded() ) {
      worker->adminTerminate();
      mirror.deleteWorker();
    }
}


//---------------------------------------------------------------------------//
//--------------------------- Supervisor ------------------------------------//
//---------------------------------------------------------------------------//


void Supervisor::forwardAllocation(OID oid, ARCSlotID to, ARCSlotID from,
				   ARC arc, bool overrides)
{
  SubordinateBase* recipient = dereference( to );

  if ( recipient ) {
	 recipient->acceptAllocation( oid, from, arc, overrides );
  }
}


SubordinateBase* Supervisor::dereference(ARCSlotID asi) const
{
  int i = asi.unwrap();

  return ( theWorkers.bound( i ) ? theWorkers.valueOf( i ) : 0 );
}


ARCSlotID Supervisor::hire( SubordinateBase* worker )
{
  theWorkers.bind( ARCSlotID_Counter, worker );

  return ARCSlotID( ARCSlotID_Counter++ );
}


void Supervisor::readMessage(istream& istr)
{
  MessageTag tag;
  
  istr >> (int&)tag;

 

  if ( tag == ARC_UPDATE ) {
	 
	 int asi, arc;
	 
	 istr >> asi;
	 istr >> arc;

	 if ( asi == 0 )
		acceptAllocation( oid(), ARCSlotID(asi), ARC(arc), true );
	 else
		forwardAllocation( oid(), ARCSlotID(asi), ARCSlotID(asi), ARC(arc), true );
	 
  } else if ( tag == START ) {
	 adminStart();
  } else if ( tag == SUSPEND ) {
	 adminSuspend();
  } else if ( tag == RESUME ) {
	 adminResume();
  } else if ( tag == TERMINATE ) {
         adminTerminate();
  } else if ( tag == PARAMETER ) {
         theParameters->read( istr );
  }
}


void Supervisor::adminTerminate( )
{
  if( crashed() ) {

    LogMessage msg( *this );
    Chars message = getCrashMessage();
    
    if( message != Chars("") )
      msg << message;
    else
      msg << "The problem " << "\\\"" << Name(*this)
	  << "\\\"" << " crashed; it should not affect any other "
	  << "algorithm in magnus.";
    
    msg.send();
  }
  
  ComputationManager::adminTerminate();

  AssociationsIterator<int,SubordinateBase*> I(theWorkers);
  for( ; !I.done(); I.next() ) {
    I.value()->terminateWorker();
/*
    Computation1Manager* pWorker = I.value()->getWorker();
    if( pWorker != 0 )
      pWorker->adminTerminate();
*/      
  }
  detachClient( *this );
}


void Supervisor::supervise( )
{
  AssociationsIterator<int,SubordinateBase*> I(theWorkers);
  bool allFinished = true;

  for( ; !I.done(); I.next() ) {
    ComputationManager* pWorker = I.value()->getWorker();
    if( pWorker ) {

      if( pWorker->state() != ComputationManager::TERMINATED )
	allFinished = false;
	
      if( pWorker->crashed() && !theCrashedWorkers.bound(I.key()) ) {

	theCrashedWorkers.bind(I.key(), true);
	
	LogMessage msg( *this );
	Chars message = pWorker->getCrashMessage();

	if( message != Chars("") )
	  msg << message;
	else
	  msg << "The computation manager " << "\\\"" 
	      << SubProblemName( *this, I.value()->arcSlotID( ) ) 
	      << "\\\"" << " crashed; it should not affect any other "
	      << "algorithm in magnus.";

	msg.send();
      }
    }
    else allFinished = false;
  }
  
  takeControl();

  /*@db temporary disabled

  if( this->state() != ComputationManager::TERMINATED && allFinished ) {
    LogMessage msg( *this );
    msg << "The currently implemented algorithms could not solve"
	<< " the problem.";
    msg.send();
    adminTerminate();
  }
  */
}
#line 1  "src/TheObjects.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class TheObjects
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


//@rn  It is still not clear (to me) how this should manage passing control
//@rn  to CMs. Eschewing premature optimization, the eggregiously clumsy code
//@rn  below is only semantically correct, which is all that we need just
//@rn  now.


#include "SMObject.h"
#include "ComputationManager.h"
#include "TheObjects.h"
#include "Supervisor.h"
//#include "unistd.h"


// #define debug_TheObjects


//---------------------------------------------------------------------------//
//--------------------------- TheObjects ------------------------------------//
//---------------------------------------------------------------------------//


bool TheObjects::isRunning(OID oid)
{
  SMObject* smo = TheObjects::get( oid );

  return
	 ( smo && smo->isComputationManager()
		&& ((ComputationManager*)smo)->state() == ComputationManager::RUNNING
		);
}


bool TheObjects::isStalled(OID oid)
{
  // See ComputationManager::checkForStall(). This checks whether any
  // CM which has ever had ARCs with benefactor oid, has any free ARCs now.

  for( int j = 0; j < theObjectsLength; ++j ) {
	 SMObject* smo = theObjects[j];
	 if ( smo != 0 &&
	      smo->isComputationManager() &&
	      ((ComputationManager*)smo)->workingFor( oid ) 
	      )
	   return false;
  }
  return true;
}


OID TheObjects::reserveOid( )
{
  // Maybe need to initialize:

  if ( theObjectsLength == 0 ) resize( minimumLength );

  // Look for an empty spot

  int i = 0;
  while ( i < theObjectsLength && theObjects[i] != 0 ) ++i;
  if ( i == theObjectsLength ) resize( i++ << 1 );

  return OID(i);
}


void TheObjects::enroll(SMObject* newObject)
{
  int oid = (newObject->oid()).unwrap();
  theObjects[oid] = newObject;
}


SetOf<OID> TheObjects::dependencyClosure(OID oid)
{
  SetOf<OID> result;
  int i = oid.unwrap();

  if ( i >= 0 && i < theObjectsLength && theObjects[i] ) {

	 result.adjoinElement( oid );
	 if  (theObjects[oid.unwrap()]->isComputationManager() )
	      return result;

	 ListIterator< ListOf<OID> > I( theObjects[i]->getDependents() );
	 while ( ! I.done() ) {
		result |= TheObjects::dependencyClosure( I.value() );
		I.next();
	 }
  }
  return result;
}


SetOf<OID> TheObjects::remove(OID oid)
{
  SetOf<OID> toDelete = TheObjects::dependencyClosure( oid );
  
#ifdef debug_TheObjects
  cerr << "\nabout to delete " << toDelete.cardinality()
       << " objects" << endl << endl << flush;
#endif
  
  SetIterator<OID> I( toDelete );
  
  while ( ! I.done() ) {
    int i = I.value().unwrap();
    
    if ( i >= 0 && i < theObjectsLength && theObjects[i] ) {
      
#ifdef debug_TheObjects
      cerr << "\ndeleting " << i << endl << endl << flush;
#endif
      
      //@db
      SMObject* smo = theObjects[i];
      if( smo->isComputationManager() ) {
	ComputationManager* CM = (ComputationManager*)smo;
	if( CM->state() == ComputationManager::RUNNING ||
	    CM->state() == ComputationManager::SUSPENDED )
	  CM->adminTerminate();
      }

      delete theObjects[i];
      theObjects[i] = 0;
      if( theNames->bound(i) )
	theNames->unbind(i);
    }
    
    I.next();
  }
  
  return toDelete;
}


SMObject* TheObjects::get(OID oid)
{
  int i = oid.unwrap();
  return ( i >= 0 && i < theObjectsLength ? theObjects[i] : 0 );
}


void TheObjects::passControl( )
{
  //@rn  Terrible, temporary hackery: A CM may have a fastResult(), which
  //@rn  means it should get control immediately, and then be removed.
  //@rn  Here we find all of these.

  for( int j = 0; j < theObjectsLength; ++j ) {
	 SMObject* smo = theObjects[j];
	 if ( smo != 0 &&
	      smo->isComputationManager() &&
	      ((ComputationManager*)smo)->fastResult()
	      ) {
	   ((ComputationManager*)smo)->takeControl();
	   remove( j );
	 }
  }
  
  
  int i = CMToGetControl;
  int count = theObjectsLength;
  
  while ( count-- ) {
	 if ( i >= theObjectsLength ) i = 0;
	 SMObject* smo = theObjects[i];
	 if ( smo != 0 &&
	      smo->isComputationManager() &&
	      ((ComputationManager*)smo)->state() 
	      == ComputationManager::RUNNING
	      ) 
	   {
	     ComputationManager* cm = (ComputationManager*)smo;
	     if ( !(cm->checkForStall()) ) {
	       
	       if( cm->isSupervisor() )  
		 ((Supervisor*)cm)->supervise();
	       else
		 cm->takeControl();
	       break;
	     }
	   }
	 ++i;
  }
  CMToGetControl = i + 1;
  
  //@dp Magnus should sleep, do not eat CPU time, when there are no CMs
  //    with allocated ARCs.

  static int passesHandledBeforeSleeping = 10;
  //  bool runningCMs = false;

  if(passesHandledBeforeSleeping >= 0 ) 
    --passesHandledBeforeSleeping;

  for( int j = 0; j < theObjectsLength; ++j ) {
    SMObject* smo = theObjects[j];
    if ( smo != 0
	 && smo->isComputationManager()
	 && ((ComputationManager*)smo)->state() == ComputationManager::RUNNING
	 && ((ComputationManager*)smo)->freeARCs() > 0 
	 ) {
      //runningCMs = true;
      passesHandledBeforeSleeping = 10;
      break;
    }
  }
  
  if( passesHandledBeforeSleeping < 0 /*!runningCMs*/  )
  #ifdef SYSV
    sleep(1); // @db: don't have usleep under Solaris
  #else    
    usleep(200);
  #endif   
}


void TheObjects::resize(int newLength)
{
  SMObject** temp = new SMObject*[newLength];
  for( int i = 0; i < theObjectsLength; ++i )
	 temp[i] = theObjects[i];
  for( int i = theObjectsLength; i < newLength; ++i )
	 temp[i] = 0;

  theObjectsLength = newLength;
  delete [] theObjects;
  theObjects = temp;
}


void TheObjects::setName(OID oid, Chars name)
{
  theNames->bind(oid,name);
}


Chars TheObjects::name(OID oid)
{
  if( theNames->bound(oid) )
    return theNames->valueOf(oid);
  else
    return "";
}


//---------------------------------------------------------------------------//
//---------------------------- TheObjects -----------------------------------//
//----------------------- static initializations ----------------------------//


int TheObjects::theObjectsLength = 0;

SMObject** TheObjects::theObjects = 0;

int TheObjects::CMToGetControl = 0;

AssociationsOf<OID,Chars>* TheObjects::theNames = new AssociationsOf<OID,Chars>(); 
#line 1  "src/viewStructure.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declarations of FE interface classes which define
//           viewStructures
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//


#include "viewStructure.h"


//---------------------------------------------------------------------------//
//--------------------------- ObjectView ------------------------------------//
//---------------------------------------------------------------------------//


ObjectView::ObjectView(ostream& ostr, OID oid, Chars heritage)
{
  ostr
	 << "viewBuilder objectView viewParameters {viewID "
	 << oid
	 << " heritage {"
	 << heritage
	 << "}}";
}


//---------------------------------------------------------------------------//
//--------------------------- ProblemView -----------------------------------//
//---------------------------------------------------------------------------//


ProblemView::ProblemView(ostream& o, OID oid, const Text& title,
    const Text& banner, const char* helpID, const Chars abbreviation)
  : state( INIT ), ostr( o )
{
  ostr
	 << "viewBuilder problemView viewParameters {viewID "
	 << oid
	 << " viewAbbreviation {"
	 << abbreviation
	 << "} viewTitle {"
	 << title
	 << "} viewBanner {"
	 << banner
	 << "} totalARCs {ARCSlotID -1 value 0} helpID " << helpID
	 // The canonical ARCSlotID for totalARCs is -1.
	 << " subproblems {";
}


void ProblemView::startItemGroup( )
{
  if ( state == DONE ) error("Continued writing to closed view");
  if ( state == ITEM_GROUP ) ostr << "}} ";  // Close previous
  ostr << "{1 {";
  state = ITEM_GROUP;
}


void ProblemView::startItemGroup(const Expression& E)
{
  if ( state == DONE ) error("Continued writing to closed menu");
  if ( state == ITEM_GROUP ) ostr << "}} ";  // Close previous
  ostr << "{{" << E << "} {";
  state = ITEM_GROUP;
}


void ProblemView::add(const Text& name, ARCSlotID asi, int value, bool enabled)
{
  ostr
	 << "{ARCSlotID " << asi
	 << " name {" << name
	 << "} state ";
  if ( enabled )
	 ostr << '1';
  else
	 ostr << '0';
  ostr
	 << " value " << value
	 << " helpID foo "       //@rn No real helpID yet.
	 << "} ";
}


void ProblemView::done( )
{
  if ( state == DONE ) error("Continued writing to closed menu");
  ostr << "}}}}" << flush;
  state = DONE;
}

// ------------------------ EnumertatorProblemView  ------------------------//

void EnumertatorProblemView::done(ostream& ostr) const {
  ostr
    << "viewBuilder EnumertatorProblemView viewParameters {viewID "
    << theOID
    << " viewAbbreviation {"
    << theAbbreviation
    << "} viewTitle {"
    << theTitle
    << "} viewBanner {"
    << theBanner
    << "} totalARCs {ARCSlotID -1 value 0} helpID " << theHelpID
    // The canonical ARCSlotID for totalARCs is -1.
    << " subproblems ";   
  theSubproblems.submit( ostr );
  ostr << " parameters ";
  theParameters.submit( ostr );
  ostr << " } " << flush;
  
  
}



#line 1  "src/ARCer.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class ARCer
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
// * 7/96 Dmitry B. made porting to gcc 2.7.2.
//


#include "ARCer.h"

#define DELETE_IPC_FILES


// ----------------------------- ARCer ------------------------------------- //


ARCer::ARCer( ComputationManager& boss, const int delay )
  : bInterrupted( false ),
    theDelay( delay ),
    pid( -1 ),
    theBoss( boss ),
    startDir( MagnusHome::magnusHome()+"/back_end/SessionManager/test/bin" )
{
  boss.hireArcer(this);
}


void ARCer::terminate( )
{
  if( pid < 0 ) return;
  kill(-pid, SIGKILL);
  #ifdef BSD    
    while( waitpid(-pid, 0, WNOHANG) > 0 );
  #endif
  pid = -1;
  bInterrupted = false; 
}


bool ARCer::takeControl( )
{
  if( !bInterrupted ) {
    
    if ( ( pid = fork() ) == 0 ) {
      
      setpgid(0,0);
      runComputation();
      
      char sPid[100];
      sprintf(sPid, "%d", getpid());
      ofstream out(MagnusTmp::magnusTmp()+"/results."+sPid, ios::out | ios::trunc );
      if( !out ) 
	error("Cannot open an output file.");
      
      writeResults(out);
      
      out.flush();
      _exit(0);
    }
    
    if( pid < 0 )
      error("fork failed.");
    
    kill(-pid, SIGSTOP);
  }
  
  kill(-pid, SIGCONT);
  sleep(theDelay);
  kill(-pid, SIGSTOP);
  bInterrupted = true;
  
  if( kill(pid, 0) != 0 ) {
    
    bInterrupted = false;
    
    //@db system porting

  #ifdef BSD    
    int status;
    wait(&status);
  #endif
    
    // get results of computation
    
    char sPid[100];
    sprintf(sPid, "/results.%d", pid);
    ifstream in(MagnusTmp::magnusTmp()+sPid, ios::in | ios::nocreate );

    if( !in ) {
      //error("Cannot open an input file.");
      
      theBoss.setCrashState(); 
      return false;
    }
    
    readResults(in);
    
    //@db Here we can delete the result file,
    //    but I want to keep it for a while.
    
    return true;
  }
  
  return false;
}


// ------------------------- ExternalARCer --------------------------------- //


ExternalARCer::ExternalARCer( ComputationManager& boss, const char* cmdline,
			      const int delay )
  : ARCer( boss, delay ),
    theCmdLine( cmdline )
{
  argFN = tempnam(MagnusTmp::magnusTmp(),"arg");
  resultFN = tempnam(MagnusTmp::magnusTmp(),"res");
}

void ExternalARCer::runComputation( )
{
  const char *cmdline = theCmdLine + " < " + argFN + " > " + resultFN; 
  execl("/bin/sh", "sh", "-c", cmdline, 0);

#if SAFETY > 0
  error("void ExternaARCer::runComputation( ): execl() failed.");
#endif    
}
  

bool ExternalARCer::takeControl( )
{
  if( !bInterrupted ) {

    ofstream out(argFN, ios::out | ios::trunc );

#if SAFETY > 0
    if( !out ) 
      error("bool ExternalARCer::takeControl( ): Cannot open an output file.");
#endif    

    writeArguments(out);
    
    if ( ( pid = fork() ) == 0 ) {
      setpgid(0,0);
      runComputation();
    }

#if SAFETY > 0
    if( pid < 0 )
      error("bool ExternaARCer::takeControl( ) : fork failed.");
#endif    
    
    kill(-pid, SIGSTOP);
  }
  
  kill(-pid, SIGCONT);
  sleep(theDelay);
  kill(-pid, SIGSTOP);
  bInterrupted = true;
  
  if( kill(pid, 0) != 0 ) {
    
    bInterrupted = false;
    
    //@db system porting

  #ifdef BSD    
    int status;
    wait(&status);
  #endif
    
    // get results of the computation

    ifstream in(resultFN, ios::in | ios::nocreate );

    if( !in ) {
      // error("bool ExternalARCer::takeControl( ): "
      // "Cannot open an input file.");

      theBoss.setCrashState(); 
      return false;
    }

    readResults(in);

    // delete the IPC files

#ifdef DELETE_IPC_FILES
    unlink(argFN);
    unlink(resultFN);
#endif
    
    return true;
  }
  
  return false;
}


// @db The following class is an experimental version of new ARCer
// class and included here just because of the NormalClosure.

// ----------------------------- ARCer2 ------------------------------------ //


ARCer2::ARCer2( ComputationManager& boss, const int delay )
  : ARCer(boss, delay),
    state(FINISHED),
    in(0)
{ }


void ARCer2::terminate( )
{
  if( pid < 0 ) return;
  kill(-pid, SIGKILL);
  #ifdef BSD    
    while( waitpid(-pid, 0, WNOHANG) > 0 );
  #endif
  pid = -1;
  state = FINISHED; 
}


bool ARCer2::takeControl( )
{
  if( state == FINISHED ) {
    
    state = RUNNING;

    if ( ( pid = fork() ) == 0 ) {
      
      setpgid(0,0);
      runComputation();
      
      char sPid[100];
      sprintf(sPid, "%d", getpid());
      ofstream out(MagnusTmp::magnusTmp()+"/results."+sPid, 
		   ios::out | ios::trunc );
      if( !out ) 
	error("Cannot open an output file.");
      
      writeResults(out);
      
      out.flush();
      _exit(0);
    }
    
    if( pid < 0 )
      error("fork failed.");
    
    kill(-pid, SIGSTOP);
  }
  
  if( state == RUNNING ) {

    kill(-pid, SIGCONT);
    sleep(theDelay);
    kill(-pid, SIGSTOP);
    
    if( kill(pid, 0) != 0 ) {
      
      state = READING;
      
      //@db system porting
      
    #ifdef BSD    
      int status;
      wait(&status);
    #endif
    }
  }   
   
  if( state == READING ) {
    
    // get results of computation
    
    if( !in ) {
      
      char sPid[100];
      sprintf(sPid, "/results.%d", pid);
      in = new ifstream(MagnusTmp::magnusTmp()+sPid, ios::in | ios::nocreate );
      if( !(*in) )
	error("Cannot open an input file.");
    }
    
    if( readResults2(*in) ) {
      
      //@db Here we can delete the result file,
      //    but I want to keep it for a while.
      
      state = FINISHED;
      delete in;
      in = 0;
      return true;
    }
  }
  
  return false;
}
#line 1  "src/MIC.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1996 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class MIC
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//

#include "MIC.h"
#include "OutMessages.h"


//------------------------------ MIC ----------------------------------------//


MIC::MIC(OID map_oid) :
  mapOID( map_oid )//,
  //extendToHom( dontknow ),
  //homIsMono( dontknow ),
  //homIsEpi( dontknow )
{ }


MIC::~MIC( ) { }

/*
void MIC::putDoesExtendToHom( bool doesExtend, Chars explanation )
{
  if( extendToHom != dontknow ) return;
  
  extendToHom = doesExtend;
  extendToHomExplanation = explanation;

  ostrstream msg;
  
  msg << Name( mapOID );
  if ( !doesExtend )
    msg << " doesn't extend";
  else
    msg << " extends";

  msg << " to an homomorphism";

  if( extendToHomExplanation.length() > 0 )
    msg << ": " << extendToHomExplanation;
  msg << "." << ends;
  
  extendToHomMessage = msg.str();
  
  LogMessage(mapOID, extendToHomMessage).send();
}
*/

/////////////

Trichotomy MIC::doesExtendToHom( ) const
{ 
  return haveProperty( ExtendToHomProperty::type() );
  //return extendToHom; 
}

void MIC::putDoesExtendToHom( bool doesExtend, Chars explanation )
{
  if( doesExtendToHom() != dontknow ) return;
  putProperty( ExtendToHomProperty(explanation), doesExtend );
  LogMessage(mapOID, getExtendToHomMessage() ).send();
}


Chars MIC::getExtendToHomExplanation( ) const 
{ 
  return getDescription( ExtendToHomProperty::type() );
  //return extendToHomExplanation; 
}

Chars MIC::getExtendToHomMessage( ) const 
{ 
  ostrstream msg;
  
  msg << Name( mapOID );
  if ( doesExtendToHom() != yes )
    msg << " doesn't extend";
  else
    msg << " extends";

  msg << " to an homomorphism";

  if( getExtendToHomExplanation().length() > 0 )
    msg << ": " << getExtendToHomExplanation();
  msg << "." << ends;
  
  Chars extendToHomMessage = msg.str();
  msg.freeze(0);

  return extendToHomMessage; 
}

void MIC::putIsMono(bool ismono)
{
  putProperty( HomIsMonoProperty(), ismono );
  //homIsMono = ismono;
}

void MIC::putIsEpi(bool isepi)
{
  putProperty( HomIsEpiProperty(), isepi );
  //homIsEpi = isepi;
}

Trichotomy MIC::isMono() const 
{
  return haveProperty( HomIsMonoProperty::type() );
  //return homIsMono;
}

Trichotomy MIC::isEpi() const 
{
  return haveProperty( HomIsEpiProperty::type() );
  //return homIsEpi;
}

const PropertyType HomIsMonoProperty  :: theTypeID = GenericProperty::unique();
const PropertyType HomIsEpiProperty   :: theTypeID = GenericProperty::unique();
const PropertyType ExtendToHomProperty:: theTypeID = GenericProperty::unique();
#line 1  "src/SMSetOfWords.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMSetOfWords
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "SMSetOfWords.h"


//---------------------------- SMSetOfWords ---------------------------------//


SMSetOfWords::SMSetOfWords( SMFPGroup& G, const SetOf<Word>& V,
			    const Chars heritage)
  : AlgebraicObject( heritage ),
    theGroup( G ),
    theWords( V )
{ }


void SMSetOfWords::printProperties(ostream& ostr) const
{
  ostr << DataPair( Parent( *this ), Object( theGroup ) );
}


void SMSetOfWords::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}


void SMSetOfWords::printDefinition(ostream& ostr) const
{
  theGroup.getFPGroup().printSetOfWords( ostr, theWords );
}




#line 1  "src/SMVectorOfWords.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMVectorOfWords
//
// Principal Author: Eugene Paderin
//
// Status: in progress
//
// Revision History:
//


#include "SMVectorOfWords.h"


//---------------------------- SMVectorOfWords ---------------------------------//


SMVectorOfWords::SMVectorOfWords( SMFPGroup& G, const VectorOf<Word>& V,
			    const Chars heritage)
  : AlgebraicObject( heritage ),
    theGroup( G ),
    theWords( V )
{ }


void SMVectorOfWords::printProperties(ostream& ostr) const
{
  ostr << DataPair( Parent( *this ), Object( theGroup ) );
}


void SMVectorOfWords::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}


void SMVectorOfWords::printDefinition(ostream& ostr) const
{
  theGroup.getFPGroup().printVectorOfWords( ostr, theWords );
}

#line 1  "src/SMList.C"
/*
 *   $Id: 
 */

// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMList, SMListIterator
//
// Principal Author: Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//

#include "SMEnumerator.h"
#include "SMSubgroup.h"
#include "Subgroup.h"
#include "Timer.h"
#include "AutoEnumerator.h"
#include "SMList.h"
#include "SMListIterator.h"
#include "MagnusHome.h"
#include <sys/mman.h>
#include <fcntl.h>

//--------------------------------------------------------------------------//
//------------------------------- SMList  ----------------------------------//
//--------------------------------------------------------------------------//

template <class T>
SMList<T>::SMList( EnumeratorProblem<T>& P, const Chars heritage )
  : AlgebraicObject( heritage ),
    theParent( P.getParentObject() ),
    theData( P.getData() )
{
  

  // Check the parent type 
  Chars theParentType(  theParent.typeID() );
  
  if ( theParentType != Chars(SMFPGroup::type()) && 
       theParentType != Chars(SMSubgroup::type()) )
    error("SMList::SMList(SMFPGroup& , const Chars, Chars , bool ) :"
	  "wrong type of the parent object.");

  theLic = new LIC( oid(), theData.numberOfElements() );
}

template <class T>
SMList<T>::SMList( const SMFPGroup& ao, const SMListData& d, const Chars heritage )
  : AlgebraicObject( heritage ),
    theParent( ao ),
    theData( d )
{
  
  // Check the parent type 
  Chars theParentType(  theParent.typeID() );
  
  if ( theParentType != Chars(SMFPGroup::type()) && 
       theParentType != Chars(SMSubgroup::type()) )
    error("SMList::SMList(AlgebraicObject& ao, const Chars& f, const Chars heritage  ) :"
	  "wrong type of the parent object.");  

  theLic = new LIC( oid(), theData.numberOfElements() );
}

   
template <class T>
SMList<T>::~SMList() 
{
    delete theLic;
}

template <class T>
void SMList<T>::printProperties(ostream& ostr) const
{
  ostr << DataPair( Parent( oid() ), Object( theParent ) );
}


template <class T>
void SMList<T>::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}

template <class T>
void SMList<T>::printDefinition(ostream& ostr) const
{

}

template <class T>
const SMFPGroup& SMList<T>::getGroup() const
{
  if (Chars(  theParent.typeID() ) == Chars(SMFPGroup::type()) )
    return (SMFPGroup& )theParent;
  else
    return (SMFPGroup& )((SMSubgroup& )theParent).getParent();
  
}








// ------------------------------- LIC ------------------------------------- //


LIC::LIC(OID list_oid, int num) :
  listOID( list_oid ),
  number_of_elements( num ),
  wpStatus( NULL ),
  abStatus( NULL ),
  centStatus( NULL )
{
  wpStatus = new status_type( num );
  abStatus = new status_type( num );
  centStatus = new status_type( num );
}


LIC::~LIC( )
{
  if ( wpStatus != NULL )
    delete wpStatus;
  if ( abStatus != NULL )
    delete abStatus;
  if ( centStatus != NULL )
    delete centStatus;}

///////////////////////////////////////////////////////////////
//                                                           //
//  Is Trivial                                               //
//                                                           //
///////////////////////////////////////////////////////////////


bool LIC::hasTrivialStatusBuffer( ) const
{
    if ( wpStatus != NULL )
         return true;
    else
         return false;
}

// pseudo constant function
Trichotomy LIC::getTrivialStatus( int i, GIC::AlgorithmID& al, Chars& e) const
{
  Trichotomy ret = wpStatus->getStatus( i, al );
  if ( ret != dontknow )
    e = explanation( al );
  return ret;
}

void LIC::setTrivialStatus( int i, Trichotomy is_trivial,GIC::AlgorithmID al )
{
  wpStatus->setStatus( i, is_trivial, al );
}

bool LIC::IsTrivialChecked() const
{
  return wpStatus->isChecked();
}

///////////////////////////////////////////////////////////////
//                                                           //
//  Is Abelian                                               //
//                                                           //
///////////////////////////////////////////////////////////////

bool LIC::hasAbelianStatusBuffer( ) const
{
    if ( abStatus != NULL )
      return true;
    else
      return false;
}

Trichotomy LIC::getAbelianStatus( int i, GIC::AlgorithmID& al, Chars& e) const
{
  Trichotomy ret = abStatus->getStatus( i, al );
  if ( ret != dontknow )
    e = explanation( al );
  return ret;
}

void LIC::setAbelianStatus( int i, Trichotomy is_abelian,GIC::AlgorithmID al )
{
  abStatus->setStatus( i, is_abelian, al );
}

bool LIC::IsAbelianChecked() const
{
  return abStatus->isChecked();
}


///////////////////////////////////////////////////////////////
//                                                           //
//  Is Central                                               //
//                                                           //
///////////////////////////////////////////////////////////////

bool LIC::hasIsCentralStatusBuffer( ) const
{
    if ( centStatus != NULL )
      return true;
    else
      return false;
}

Trichotomy LIC::getIsCentralStatus( int i, GIC::AlgorithmID& al, Chars& e) const
{
  Trichotomy ret = centStatus->getStatus( i, al );
  if ( ret != dontknow )
    e = explanation( al );
  return ret;
}

void LIC::setIsCentralStatus( int i, Trichotomy is_central,GIC::AlgorithmID al )
{
  centStatus->setStatus( i, is_central, al );
}

bool LIC::IsCentralChecked() const
{
  return centStatus->isChecked();
}



Chars LIC::explanation( GIC::AlgorithmID a) const
{
  Chars ret = "";
  if ( a == GIC::NORM_CLOSURE ) {
    ret =  "labels loop in normal closure approximation";
    
  } else if ( a == GIC::AB_INV ) {
    //    if ( theList->getGroup().gic.isAbelian() == yes ) 
    //  ret =  "this follows from the canonical decomposition";
    // else
      ret =  "this follows from an inspection of the "
	"abelianization of the group";   

  } else if ( a == GIC::REWR_SYSTEM ) {
    ret =  "according to the confluent rewriting system";

  } else if ( a == GIC::AUT_STRUCTURE ) {
    ret = "according to the difference machine";
    
 } else if ( a == GIC::GENETIC ) {
    ret =  "according to the genetic algorithm";

 } else if ( a == GIC::FREE ) {
    ret = "";
    
 } else {
   error("Chars SMListWPPrinResultArcer::explanation( GIC::AlgorithmID a) :"
	 " improper algorithm ID.");
 }
  if ( ret.length() > 0 )
    ret +=".";
  return ret;
}


// -------------------- LIC::status_type -------------------- //


LIC::status_type::status_type( int s ):
  size( s ),
  status_buffer( NULL )
{
#if ( SAFETY > 0 )
  if ( status_buffer != NULL )
    error("LIC::status_type::status_type( int s ):"
	  " buffer has been initialized already");
#endif

  char* tmpFileName = tempnam(MagnusTmp::magnusTmp(),NULL);
  
  status_file = open(tmpFileName,O_RDWR | O_CREAT, S_IWUSR | S_IRUSR  );

  if ( status_file < 0 )
    error("LIC::status_type::status_type( int s ):"
	  "error couldn't open a file.");


  int c = 0;
  char s[1];
  s[0] = '\0';
  for ( int i = 0;i<size ;i++)
    c += write(status_file,s,1);

  free(tmpFileName);
  
  status_buffer = (char*)mmap(0, size, 
			      PROT_READ | PROT_WRITE, MAP_SHARED, 
			      status_file, 0); 
    
  if ( status_buffer < 0 )
    error("LIC::status_type::status_type( int s ) :"
	  "error couldn't map the file.");
}

LIC::status_type::~status_type()
{
  if ( status_buffer != NULL ) {
    munmap(status_buffer, size );
    close( status_file );
  }
}

Trichotomy LIC::status_type::getStatus( int i, GIC::AlgorithmID& al ) const
{
#if ( SAFETY > 0 )
  if ( i < 0 || i >= size ) 
    error(" LIC::status_type::getStatus( int, GIC::AlgorithmID&, Chars& ) :"
	  " index out of bounds");
#endif
  if ( status_buffer[i] == '\0' )
    return dontknow;
  if ( status_buffer[i] > 0 ){
    al = GIC::AlgorithmID( status_buffer[i] );
    return yes;
  } else {
    al = GIC::AlgorithmID( -status_buffer[i] );
    return no;
  }  
}
void LIC::status_type::setStatus( int i, Trichotomy t, 
				  GIC::AlgorithmID al  )
{
 
  if ( t == dontknow )
    return;
  
#if ( SAFETY > 0 )
  if ( i < 0 || i >= size ) 
    error("LIC::status_type::setStatus( ... ):"
	  " index out of bounds");
#endif
  if ( status_buffer[i] == '\0' ){
    char flag = (char )al;
    if ( t == no )
      flag = -flag;
    status_buffer[i] = flag;  
  }  
}
  
bool LIC::status_type::isChecked() const
{
  for (int i = 0; i< size; i++)
    if (status_buffer[i] == '\0' )
      return false;

  return true; 
}
template class SMList<Word>;
template class SMList<Subgroup>;
template class SMList<Map>;

SMListType<Word> SMList<Word>::theType;
SMListType<Map> SMList<Map>::theType;
SMListType<Subgroup> SMList<Subgroup>::theType;

EnProbType<Word> EnumeratorProblem<Word>::theType;
EnProbType<Map> EnumeratorProblem<Map>::theType;
EnProbType<Subgroup> EnumeratorProblem<Subgroup>::theType;
EnProbType<NoType> EnumeratorProblem<NoType>::theType;
#line 1  "src/SMEnumerator.C"
/*
 *   $Id: 
 */

// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMEquation
//
// Principal Author: Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//


#include "SMEnumerator.h"
#include "SMSubgroup.h"
#include "Subgroup.h"
#include "Timer.h"
#include "AutoEnumerator.h"
#include "MagnusHome.h"
#include "SMListIterator.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

bool SUSPEND_SIGNAL_RECEIVED = false;

#ifdef BSD
extern "C"
void suspend_signal_handler(int sig, int code, struct sigcontext* scp,
			     char* addr)
#else
extern "C"
void suspend_signal_handler(int sig)
#endif

{
  if (sig == SIGUSR1)
    SUSPEND_SIGNAL_RECEIVED = true;
}

const Type NoType::theNoType = Type( Type::unique() );


//--------------------------------------------------------------------------//
//--------------------------- SMListData  ----------------------------------//
//--------------------------------------------------------------------------//

SMListData::SMListData( ): number_of_elements(0), mode( -1 ) 
{ 
  // get name for the data file
  char* tmpFileName = tempnam(MagnusTmp::magnusTmp(),NULL);
  dataFileName = Chars( tmpFileName );
  
  // makes an empty color file for front end maybe needs to be changed
  //const char* destColor = Chars( dataFileName ) + Chars(".color");
  Chars destColor = Chars( tmpFileName ) + Chars(".color");
  ofstream outColor( destColor );
  outColor << "#0000000%000@" << flush;
  outColor.close();
  free(tmpFileName);

  // get name for the referencies file
  tmpFileName = tempnam(MagnusTmp::magnusTmp(),NULL);
  refFileName = Chars( tmpFileName );
  free(tmpFileName);
  

}

// operator for adding elements
SMListData& SMListData::operator << (const WriteEnumeratorElement& e)
{
#if SAFETY > 0
  if ( mode != ios::out )
    error("SMListData& operator << (const WriteEnumeratorElement& e) :"
	  " was not opened for read acces.");
#endif    
  number_of_elements++;
  
  dataFile << number_of_elements << ": ";
  // save begining position of an element
  char f_pos[10];
  long beg_pos = dataFile.tellg();
  sprintf( f_pos, "%.10d",beg_pos );
  refFile << f_pos << flush;

  // write element
  dataFile << e;

  // save element length
  char e_size[5];
  sprintf(e_size, "%.5d",dataFile.tellg() - beg_pos );
  refFile << e_size << flush;

  dataFile << flush;
  return *this;
}

SMListData&  SMListData::operator << (const Chars& c)
{
#if SAFETY > 0
  if ( mode != ios::out )
    error("SMListData& operator << (const Chars& c) :"
	  " was not opened for read acces.");
#endif    

  dataFile << c;
  return *this;
}


  // returns i-th element as a character. 
  // Iterator must parse it to the corresponding data type
Chars SMListData::getElementOf( int i )
{
  int beg_pos = getBegPosOf( i );
  int size = getLengthOf( i );

  Chars ret;
  
  char* el = new char[size + 1];

  dataFile.seekg( beg_pos );
  dataFile.read( el, size );

  el[size] = '\0';
  ret = Chars( el );
  delete [] el;
  return ret;   
}


  // appends a list to the end 
void SMListData::append( SMListData& sd )
{
#if SAFETY > 0
  if ( mode != ios::out )
    error( "SMListData::append( ... ) : "
	   " was not opened for write access." );  
#endif   
  int n = sd.numberOfElements();
  for (int i = 0;i < n; i++ ){
    number_of_elements++;
    
    dataFile << number_of_elements << ": ";
    // save begining position of an element
    char f_pos[10];
    long beg_pos = dataFile.tellg();
    sprintf( f_pos, "%.10d",beg_pos );
    refFile << f_pos << flush;
    
    // write element
    dataFile << sd.getElementOf( i );
    
    // save element length
    char e_size[5];
    sprintf(e_size, "%.5d",dataFile.tellg() - beg_pos );
    refFile << e_size << flush;

  }
}

void SMListData::openFiles( int m )
{
  mode = m;
  dataFile.open(dataFileName,mode);
#if SAFETY > 0
  if( !dataFile )
      error( "SMListData::openFiles( int ) : "
	     "Cannot open the data file." );
#endif    

  refFile.open(refFileName,mode);
#if SAFETY > 0
  if( !refFile )
      error( "SMListData::openFiles( int ) : "
	     "Cannot open the referencies file." );
#endif 
}

void SMListData::setWriteMode()
{
  closeCurrentMode();
  openFiles( ios::out );
}
void SMListData::setReadMode()
{
  closeCurrentMode();
  openFiles( ios::in );  
}
void SMListData::closeCurrentMode()
{
  dataFile.close();
  refFile.close();
  mode = -1;
}

SMListData::SMListData( const SMListData& sd ):
  number_of_elements( sd.number_of_elements ), 
  mode( -1 ),
  dataFileName( sd.dataFileName ),
  refFileName( sd.refFileName )
{
  
}


void SMListData::doHardCopy( const SMListData& sd )
{
  closeCurrentMode();

  number_of_elements = sd.number_of_elements;

  // copy the file with enumerations
  char* tmpFileName = tempnam(MagnusTmp::magnusTmp(),NULL);
  Chars fName = sd.dataFileName;
  const char* tmpStr = fName;
  int pid;
  
  if ( ( pid = fork() ) == 0 ) {
    setpgid(0,0);
    
    if ( execlp( "cp", "",tmpStr ,tmpFileName, NULL) == -1 )
      error("SMListData( const SMListData& sd, int m = ios::in ):"
	    " can't create a copy of the data file.");
    
    _exit(0);
  }
  
  while( kill(pid, 0) == 0 ) {
    sleep(100); 
  }
  
#ifdef BSD    
  int status;
  wait(&status);
#endif
  
  // make an empty color file
  //const char* destColor = Chars( tmpFileName ) + Chars(".color");
  Chars destColor = Chars( tmpFileName ) + Chars(".color");
  ofstream outColor( destColor );
  outColor << "#0000000%000@" << flush;
  outColor.close();
  
  // save the name of the data file
  dataFileName = Chars( tmpFileName );
  free( tmpFileName );
  // Copy references file
  tmpFileName = tempnam(MagnusTmp::magnusTmp(),NULL);
  fName = sd.refFileName;
  tmpStr = fName;
  
  if ( ( pid = fork() ) == 0 ) {
    setpgid(0,0);
    
    if ( execlp( "cp", "",tmpStr ,tmpFileName, NULL) == -1 )
      error("SMListData( const SMListData& sd, int m = ios::in ):"
	    " can't create a copy of the offset file.");
    
    _exit(0);
  }
  
  while( kill(pid, 0) == 0 ) { sleep(10); }
  
#ifdef BSD    
  status;
  wait(&status);
#endif
  // save the name of the offset file
  refFileName = Chars( tmpFileName );
  free( tmpFileName );
}
  
  // gives offset of the i-th element 
int SMListData::getBegPosOf( int i)
{
#if SAFETY > 0
  if ( mode != ios::in )
    error( "SMListData::getBegPosOf( int ) : "
	   " was not opened for read access." );      
  if( i < 0 || i >= number_of_elements )
    error( "SMListData::getBegPosOf( int ) : "
	   " wrong index." );
#endif    
  
  refFile.seekg( i*10 + i*5 );
  char str_pos[11]; 
  refFile.read( str_pos, 10 );
  str_pos[10] = '\0';
  int f_pos = atoi( str_pos );
  return f_pos;
}


  // returns the length of an i-th element
int SMListData::getLengthOf( int i)
{
#if SAFETY > 0
  if ( mode != ios::in )
    error( "SMListData::getLengthOf( int ) : "
	   " was not opened for read access." );      
  if( i < 0 || i >= number_of_elements )
    error( "SMListData::getLengthOf( int ) : "
	   " wrong index." );
#endif    
  
  refFile.seekg( i*10 + i*5 + 10 );
  char str_pos[6];  
  refFile.read( str_pos, 5 );
  str_pos[5] = '\0';
  int size = atoi( str_pos );
  return size;
}


//--------------------------------------------------------------------------//
//----------------------- EnumeratorARCer ----------------------------------//
//--------------------------------------------------------------------------//

void EnumeratorARCer::runComputation( )
{
  SUSPEND_SIGNAL_RECEIVED = false;
  if (signal( SIGUSR1, suspend_signal_handler) == SIG_ERR)
    error("void EnumeratorARCer::runComputation( ) :"
	  "Can`t catch signal");
  enumerate();
}

bool EnumeratorARCer::submitSignalRecieved()const
{
  return  SUSPEND_SIGNAL_RECEIVED;
}

void EnumeratorARCer::submitSignal( )
{
  if( pid < 0 ) return;
  if (bInterrupted == true)
    kill(-pid, SIGCONT);
  kill(-pid, SIGUSR1);

}
/*
//---------------------------------------------------------------------------//
//-------------------------- SMEnumerator -----------------------------------//
//---------------------------------------------------------------------------//

template <class T> 
SMEnumerator<T>::SMEnumerator(AlgebraicObject& P, const Chars heritage)
  : AlgebraicObject( heritage ),
    theParent( P )

{ }

template <class T> 
SMEnumerator<T>::~SMEnumerator() 
{
}
template <class T> 
void SMEnumerator<T>::printProperties(ostream& ostr) const
{
  ostr << DataPair( Parent( oid() ), Object( theParent ) );
}


template <class T> 
void SMEnumerator<T>::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}

template <class T> 
void SMEnumerator<T>::printDefinition(ostream& ostr) const
{

}

template class SMEnumerator<Word>;
template class SMEnumerator<Map>;
template class SMEnumerator<NoType>;
*/
//---------------------------------------------------------------------------//
//-------------------------- EnumeratorProblem ------------------------------//
//---------------------------------------------------------------------------//



#line 1  "src/ViewContents.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: 
//           
//
// Principal Author: Alexei Myasnikov
//
// Status: in progress
//
// Revision History:
//


#include "values.h"
#include "ViewContents.h"
#include "conversions.h"
#include "OutMessages.h"
#include "WordEnumerator.h"
#include "EquationParser.h"

// ----------------------------- EditInteger  -----------------------------//
void EditInteger::submit(ostream& ostr)const {
  ostr << "{ " << type() << " " <<  name() << " \"" << theText << "\" " 
       << changeable << " " << defaultValue << " " << minValue << " " << maxValue << " }"; 
}

// ----------------------------- EditInteger  -----------------------------//
void EditBool::submit(ostream& ostr)const {
  ostr << "{ " << type() << " " <<  name() << " \"" << theText << "\" " 
       << changeable << " " << defaultValue << " }"; 
}

// ----------------------------- EditText  -----------------------------//
void EditText::submit(ostream& ostr)const {
  ostr << "{ " << type() << " " <<  name() << " \"" << theText << "\" " 
       << changeable << " \"" << defaultValue << "\" " << " }"; 
}

void  EditText::setValue( istream& is, Chars& errMsg, Chars name ) { 
  //Chars c;
    is > defaultValue;
    /*   defaultValue = "";
    char ch;
    for (int i = 0;i<c.length();i++) {
      ch = c[i];
      if ( ch == '\377' )
	defaultValue += ' ';
      else
	defaultValue += ch;
    }
    */
}
// ----------------------------- EditWord  -----------------------------//
void EditWord::setValue( istream& is, Chars& errMsg,  Chars name){
  EditText::setValue(is,name);
  
  //PatternParser p( defaultValue );
  istrstream istr( defaultValue );
  
  EquationParser p( istr );
  Chars m;
  VectorOf<Chars> newNames;
  theWord = p.parseEquation(  VectorOf<Chars>(),newNames,m );
  
  if (m.length() > 0) {
    bool start = false;
    errMsg = theText + Chars(": "); 
    for (int i=0;i<m.length();i++){
      if (m[i] == '}') break;
      if ( start ) errMsg += m[i];
      if (m[i] == '{') start = true;
    }
    
    theWord = Generator(1);
  }

  if (theWord.cyclicallyReduce().length() == 0)
    errMsg = theText + Chars(": the generic word can not be trivial "
			     "in the cyclically reduced form."); 
			     
}

// ----------------------------- GroupWindow  -----------------------------//

GroupWindow::GroupWindow(const GroupWindow& gw): 
  ViewContent(gw), children(NULL), last(NULL)        
{
  
  ViewContentCell* from = gw.children;
  while (from){
    ViewContentCell* tmpC = new ViewContentCell(from->content());
    if (children == NULL)
      last = children = tmpC;
    else {
      last->setNext(tmpC);
      last = last->next();
    }
    
    from = from->next();
  }
}
  


GroupWindow::~GroupWindow() {
  ViewContentCell* from = children;
  while (from){
    ViewContentCell* next = from->next();
    delete from;
    from = next;
  }    
}

void GroupWindow::submit(ostream& ostr) const {
  ostr << "{ " << type() << " " << name() << properties()  ;
  ViewContentCell* from = children;
  while (from){
    ostr << " ";
    from->content().submit(ostr);
    from = from->next();
  }
  ostr << " }" ;
}

void GroupWindow::add(const ViewContent& c){
  ViewContentCell* addNew = new ViewContentCell(c);
  if (children == NULL){
    last = children = addNew;
    return;
  }
  last->setNext(addNew);
  last = addNew;
}

const ViewContent* GroupWindow::contentOf( const Chars& n ) const
 
{
  if (name() == n){
    return this;
  }
  
  ViewContentCell* from = children;
  while (from){
    const ViewContent* tmp = from->content().contentOf(n);
      if ( tmp ){
      return tmp;
    }
    from = from->next();
  }    
  return NULL;
}

// ----------------------------- RadioButtonGroup  --------------------------//

void RadioButtonGroup::add(const ViewContent& c){
  if (strcmp(c.type(), "RadioButton") !=0 )
    error("Wrong type of parameter");
  if ( selButton.length() == 0)
    selButton = c.name();

  GroupWindow::add(c);
}




// ----------------------------- Subproblem  -----------------------------//
void Subproblem::submit(ostream& ostr)const {
  ostr << "{ " << type() << " " << name() << " "<< numOfARCs 
       << " {" << Text(description) << "} " <<  theASI << " helpID foo }"; 
}


// ----------------------------- ParameterStructure -------------------------//

void ParameterStructure::read( istream& istr )
{
  bool haveErrors = false;
  Chars type = "";
  Chars name;
  istr >> type;
  
  while ( type != Chars("{EOF_PARAM}")) { 
    
    istr >> name; 
    if (type == Chars("int") || type == Chars("RadioButtonGroup")
	|| type == Chars("text") || type == Chars("bool")) {
      Chars errMsg;
      theParameters.setValue(istr,errMsg, name);
      if (errMsg.length() > 0 ) {
	ParseParamErrorMessage(bossOID,name,type,errMsg).send();
	haveErrors = true;
      }
    } else
      error("void ParameterStructure::read( istream& istr ) : "
	    "unexpected type of a parameter");
    istr >> type;
  }
  
  if (!haveErrors)
    ParseParamOk(bossOID).send();
}

#line 1  "src/SMEqSystem.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition of the SEIC, SMEqSystem classes.
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


#include "SMEqSystem.h"


//---------------------------------------------------------------------------//
//-------------------------------- SEIC -------------------------------------//
//---------------------------------------------------------------------------//


SEIC::SEIC(OID eqs_oid)
  : eqSystemOID( eqs_oid ),
    solvedInAbelian( false ),
    haveSolInAbelian( false ),
    solved( false ),
    haveSol( false )
{ }

SEIC::~SEIC( )
{ }


//---------------------------------------------------------------------------//
//----------------------------- SMEqSystem ----------------------------------//
//---------------------------------------------------------------------------//

SMEqSystem::SMEqSystem(SMFPGroup& G, FreeGroup F, const VectorOf<Word> v, 
		       int nvar, const Chars heritage)
  : AlgebraicObject( heritage ),
    theGroup( G ),
    theFreeGroup( F ),
    theSystem( v ),
    numOfVar( nvar ),
    seic( oid() )
{ }

void SMEqSystem::printProperties(ostream& ostr) const
{
  ostr << DataPair( Parent( oid() ), Object( theGroup ) );
}


void SMEqSystem::viewStructure(ostream& ostr) const
{
  ostr
    << "viewBuilder objectView viewParameters {viewID "
    << oid()
    << " heritage {"
    << heritage()
    << "}}";
}


void SMEqSystem::printDefinition(ostream& ostr) const
{
  theGroup.getFPGroup().printVectorOfWords( ostr, theSystem );
}
#line 1  "src/SMPermutation.C"
/*
 *   $Id: SM1.C 4 2005-01-07 00:28:20Z daly $
 */

// Copyright (C) 2000 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMPermutation
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#include "SMPermutation.h"


// --------------------------- SMPermutation ------------------------------- //

SMPermutation::SMPermutation(Permutation perm, const Chars heritage)
  : AlgebraicObject( heritage ),
    thePermutation( perm )
{ }


void SMPermutation::printProperties(ostream& ostr) const
{
  ostr << DataPair( Oid( *this ), Object( *this ) );
}


void SMPermutation::viewStructure(ostream& ostr) const
{
  ostr << "viewBuilder objectView viewParameters {viewID "
       << oid()
       << " heritage {"
       << heritage()
       << "}}";
}


void SMPermutation::printDefinition(ostream& ostr) const
{
  ostr << thePermutation;
}

