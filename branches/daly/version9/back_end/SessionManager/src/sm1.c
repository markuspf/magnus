#line 1  "src/ComputationManager.C"
/*
 *   $Id: ComputationManager.C,v 1.9 1998/07/11 18:24:12 alex Exp $
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



void ComputationManager::setCrashState( ) 
{ 
  isCrashed = true; 
  adminTerminate();
}
#line 1  "src/FEData.C"
/*
 *   $Id: FEData.C,v 1.4 1997/06/23 18:50:19 bormotov Exp $
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


void FEData::SubjectRep::printOn(ostream& ostr) const
{
  ostr << "$subject==\"" << subject << "\"";
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


//--------------------------------------------------------------------------//
//-------------------------- FEData::Subject -------------------------------//
//--------------------------------------------------------------------------//

FEData::Subject::Subject( Chars sub )
  : Expression( new SubjectRep( sub ) )
{ }


// ------------------------- FEData::Link ---------------------------------- //


FEData::Link::Link( const Chars& text, const Chars& problemName,
		    const Chars& fileName, bool isDynamic )
  : Text( Chars("@") + text + "@" + problemName + "@" + fileName + "@"
	  + (isDynamic ? "1" : "0") + "@" )
{ }
#line 1  "src/DCM.C"
/*
 *   $Id: DCM.C,v 1.9 2001/10/31 19:36:37 alex Exp $
 */

// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementation of class DCM
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#include "DCM.h"
#include "SMBaseOfDataSet.h"


//---------------------------------------------------------------------------//
//-------------------------------- DCM --------------------------------------//
//---------------------------------------------------------------------------//


DCM::DCM( class SMBaseOfDataSet& D )
  : Supervisor( false ),
    theSMDataSet( D )
{
  adminStart();
}



void DCM::takeControl( )
{
}


void DCM::start( ) { }

void DCM::terminate( ) { }

void DCM::viewStructure(ostream& ostr) const { }

#line 1  "src/Menu.C"
/*
 *   $Id: Menu.C,v 1.33 2001/03/01 00:06:56 alex Exp $
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


#include "Chars.h"
#include "Menu.h"
#include "ObjectSmith.h"
#include "SessionManager.h"
#include "TheObjects.h"
#include "OutMessages.h"

// Includes required for special cases:

#include "TheObjects.h"
#include "Type.h"
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
  smo->sendResult();
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
  if ( transposeARGS )
	 smo = construct( arg2, arg1, name, istr );
  else
	 smo = construct( arg1, arg2, name, istr );
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

// ---------------------------- STATISTICS ------------------------------------
SMObject*
ReadSMSimpleMenu::construct( Chars& name, istream& istr) const
{
  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {

    return new SMBaseOfDataSet( S, Text("User defined data set") );
    
  } else {
    
    ParseErrorMessage( name, err ).send();
    
    return 0;
  }
}

//add by xuxw
SMObject*
ReadSMOneVariableDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;
  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined One Variable Data Set", SMBaseOfDataSet::ONEVARIABLE );

	 smo->dic.putIsOneVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject*
ReadSMTwoVariableDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Two Variable Data Set", SMBaseOfDataSet::TWOVARIABLE );

	 smo->dic.putIsTwoVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}


SMObject*
ReadSMMultiVariableDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Multi Variable Data Set", SMBaseOfDataSet::MULTIVARIABLE );

//	 smo->gic.putIsMultiVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject*
ReadSMCrossClassifiedDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Cross Classified DataSet", SMBaseOfDataSet::CROSSCLASSIFIED );

//	 smo->gic.putIsCrossClassifiedDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject*
ReadSMTimeSeriesDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Time Series Data Set", SMBaseOfDataSet::TIMESERIES );

//	 smo->gic.putIsTimeSeriesDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}


SMObject*
ReadSMQualityControlDataSet::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Quality Control Data Set", SMBaseOfDataSet::QUALITYCONTROL );

//	 smo->gic.putIsQualityControlDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}



SMObject*
ReadSMProbabilityDistribution::construct( Chars& name, istream& istr) const
{

  istr >> name;

  DataSet S;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined Probability Distribution ", SMBaseOfDataSet::PROBABILITY );

//	 smo->gic.putIsProbabilityDistribution();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}


SMObject* ReadSMConstructSampleFromDataSet::construct(SMObject* a1, Chars& name, istream& istr) const
{
  int size;
  DataSet S;
  istr >> name;
  Chars err = istr >> S;

  if ( err.length() == 0 ) {
	 
	 SMBaseOfDataSet* smo
		= new SMBaseOfDataSet( S, "User defined One Sample Data Set", SMBaseOfDataSet::ONEVARIABLE );

	 smo->dic.putIsOneVariableDataSet();

	 return smo;

  } else {

	 ParseErrorMessage( name, err ).send();

	 return 0;
  }
}

SMObject* ReadSMDataSummary::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  return new SMDataSetDataSummary( parent, "summaryView", "DataSet Data Summary");
}

SMObject* ReadSMFullAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  return new SMDataSetFullAnalysis( parent, "resultView", "DataSet Full Analysis");
}

SMObject* ReadSMGraphicAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  return new SMDataSetGraphicAnalysis( parent, "graphicView", "DataSet Graphical Analysis");
}

SMObject* ReadSMGoodnessOfFit::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  istr >> Type;
  istr >> level;
  SMDataSetGoodnessOfFit* smo
		= new SMDataSetGoodnessOfFit( parent, "resultView", "DataSet Fit Analysis");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  return smo;

}

SMObject* ReadSMMeanAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  double MU0;
  char MU0Type;
  istr >> Type;
  istr >> level;
  if(Type=='H') {
     istr >> MU0;
     istr >> MU0Type;
  }
  SMDataSetMeanAnalysis* smo
		= new SMDataSetMeanAnalysis( parent, "resultView", "DataSet Mean Analysis");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  smo->m_MU0=MU0;	 
  smo->m_MU0Type=MU0Type;	 
  return smo;

}

SMObject* ReadSMVarianceAnalysis::construct(SMObject* a1, Chars& name, istream& istr) const
{
  SMBaseOfDataSet& parent = *(SMBaseOfDataSet*)a1;
  char Type;
  double level;
  double MU0;
  char MU0Type;
  istr >> Type;
  istr >> level;
  if(Type=='H') {
     istr >> MU0;
     istr >> MU0Type;
  }
  SMDataSetVarianceAnalysis* smo
		= new SMDataSetVarianceAnalysis( parent, "resultView", "DataSet Variance Analysis");
  smo->m_Level=level;	 
  smo->m_Type=Type;	 
  smo->m_MU0=MU0;	 
  smo->m_MU0Type=MU0Type;	 
  return smo;

}

// ---------------------------- CALCULUS --------------------------------- //

//---------------------------------------------------------------------------//
//-------------------------- Menu::Action -----------------------------------//
//---------------------------------------------------------------------------//

// -------------------------- STATISTICS --------------------------------- //


void Menu::DefineDataSet::print(ostream& ostr) const
{
  ostr << "_defineDataSet_ {Define a data set}";
}


void Menu::DefineOneVariableDataSet::print(ostream& ostr) const
{
  ostr << "_defineOneVariableDataSet_ {Define a one variable data set}";
}

void Menu::DefineTwoVariableDataSet::print(ostream& ostr) const
{
  ostr << "_defineTwoVariableDataSet_ {Define a two variable data set}";
}

void Menu::DefineMultiVariableDataSet::print(ostream& ostr) const
{
  ostr << "_defineMultiVariableDataSet_ {Define a multi variable data set}";
}

void Menu::DefineCrossClassifiedDataSet::print(ostream& ostr) const
{
  ostr << "_defineCrossClassifiedDataSet_ {Define a cross classified data set}";
}

void Menu::DefineTimeSeriesDataSet::print(ostream& ostr) const
{
  ostr << "_defineTimeSeriesDataSet_ {Define a time series data set}";
}

void Menu::DefineQualityControlDataSet::print(ostream& ostr) const
{
  ostr << "_defineQualityControlDataSet_ {Define a quality control data set}";
}

void Menu::DefineProbabilityDistribution::print(ostream& ostr) const
{
  ostr << "_defineProbabilityDistribution_ {Define a probability distribution}";
}

void Menu::showDataSetHelp::print(ostream& ostr) const
{
   ostr << "showHelp {DescriptionDataSet::Description Of Data Set}  ";
};

void Menu::DefineDescriptionOfDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {DescriptionDataSet::Description Of Data Set} ";
}

void Menu::DefineConstructSampleFromDataSet::print(ostream& ostr) const
{
   ostr << "_constructSampleFromDataSet_ {Construct Random Sample from Data Set}";
}

void Menu::DefineDescriptionOfOneVariableDataSet::print(ostream& ostr) const
{
   ostr << "showHelp {DescriptionOfOneVariableDataSet::Description of One Variable Data Set}  ";
}

void Menu::DefineDataSetDataSummary::print(ostream& ostr) const
{
  ostr << "_defineDataSetDataSummary_ {Data Summary a Data Set}";
}

void Menu::DefineDataSetFullAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetFullAnalysis_ {Full Analysis a Data Set}";
}

void Menu::DefineDataSetGraphicAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetGraphicAnalysis_ {Graphic Analysis a Data Set}";
}

void Menu::DefineDataSetGoodnessOfFit::print(ostream& ostr) const
{
  ostr << "_defineDataSetGoodnessOfFit_ {Fit Analysis a Data Set}";
}

void Menu::DefineDataSetMeanAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetMeanAnalysis_ {Mean Analysis a Data Set}";
}

void Menu::DefineDataSetVarianceAnalysis::print(ostream& ostr) const
{
  ostr << "_defineDataSetVarianceAnalysis_ {Variance Analysis a Data Set}";
}



void Menu::DefineDescriptionOfFullAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {fullAnalysis::How Full Analysis Works}  ";
}

void Menu::DefineDescriptionOfDataSummary ::print(ostream& ostr) const
{
   ostr << "showHelp {dataSummary::How Data Summary Works}  ";
}
void Menu::DefineDescriptionOfGraphicAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {graphicAnalysis::How Graphic Analysis Works}  ";
}
void Menu::DefineDescriptionOfMeanAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {meanAnalysis::How Mean Analysis Works}  ";
}
void Menu::DefineDescriptionOfVarianceAnalysis::print(ostream& ostr) const
{
   ostr << "showHelp {varianceAnalysis::How Variance Analysis Works}  ";
}
void Menu::DefineDescriptionOfGoodnessOfFit::print(ostream& ostr) const
{
   ostr << "showHelp {goodnessOfFit::How Goodness Of Fit Works}  ";
}


// ---------------------------- CALCULUS --------------------------------- //


// ----------------------------------------------------------------------- //


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
  else if( mk == HELP )
    cout << "menu_defn_ menu help signature {";
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

// add menus without communication between tcl and SessionManager.
//such as help menus.

void Menu::addSimple(const Text& text,const Action& action)
{
  add(ReadSMSimpleMenu().copy(),text,action);
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
 *   $Id: ObjectSmith.C,v 1.1.1.1 1995/11/20 17:55:28 rogern Exp $
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
 *   $Id: OutMessages.C,v 1.9 2000/02/10 00:12:09 bormotov Exp $
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

#define STATISTICS_LOG_MESSAGE
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
  #ifndef STATISTICS_LOG_MESSAGE
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
  #else
     for( int i = 0; i < num_addressees; ++i ) 
        ostr << oids[i] << " ";
  #endif
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
    << " { " << theObject << " } ";
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
    
    ostr << " definition { ";
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
 *   $Id: ResourceManager.C,v 1.3 1997/06/23 18:50:20 bormotov Exp $
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
#line 1  "src/SessionManager.C"
/*
 *   $Id: SessionManager.C,v 1.13 2001/03/01 00:07:10 alex Exp $
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
//  SMFPGroup::printGlobalMessageTemplates( cout );
  DatabaseManager::printGlobalMessageTemplate( cout );

  //----------------------------- Type Names --------------------------------//
/*
  cout
	 << "type_name_ {finitely presented group} G " << SMFPGroup::type() << endl
	 << "type_name_ {word} w " << SMWord::type() << endl
	 << "type_name_ {subgroup} H " << SMSubgroup::type() << endl
	 << "type_name_ {map} m " << SMMap::type() << endl
	 << "type_name_ {equation} E " << SMEquation::type() << endl
	 ;
*/

  //------------------------------- Menus -----------------------------------//

  ObjectSmith::outputCheckinMenuDefns( cout );
  ObjectSmith::outputToolsMenuDefns( cout );
  ObjectSmith::outputMakeMenuDefns( cout );
  ObjectSmith::outputPackagesMenuDefns( cout );
  ObjectSmith::outputTestingMenuDefns( cout );
  ObjectSmith::outputHelpMenuDefns( cout );

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
 *   $Id: Supervisor.C,v 1.13 1999/11/23 20:32:57 bormotov Exp $
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
 *   $Id: TheObjects.C,v 1.10 2000/02/28 22:00:47 bormotov Exp $
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
 *   $Id: viewStructure.C,v 1.3 1998/02/23 17:19:06 alex Exp $
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
// ------------------------ GraphicView  ------------------------//
GraphicView::GraphicView(ostream& o, OID oid, const Text& title,
    const Text& banner, const char* helpID, const Chars abbreviation)
  : ostr( o )
{
  ostr
	 << "viewBuilder graphicView viewParameters {viewID "
	 << oid
	 << " viewAbbreviation {"
	 << abbreviation
	 << "} viewTitle {"
	 << title
	 << "} viewBanner {"
	 << banner
	 << "} totalARCs {ARCSlotID -1 value 0} helpID " << helpID
	 // The canonical ARCSlotID for totalARCs is -1.
	 << " subproblems {{{";
}

void GraphicView::done( )
{
  ostr << "}}}}" << flush;
}

// ------------------------ SummaryView  ------------------------//
SummaryView::SummaryView(ostream& o, OID oid, const Text& title,
    const Text& banner, const char* helpID, const Chars abbreviation)
  : ostr( o )
{
  ostr
	 << "viewBuilder summaryView viewParameters {viewID "
	 << oid
	 << " viewAbbreviation {"
	 << abbreviation
	 << "} viewTitle {"
	 << title
	 << "} viewBanner {"
	 << banner
	 << "} totalARCs {ARCSlotID -1 value 0} helpID " << helpID
	 // The canonical ARCSlotID for totalARCs is -1.
	 << " subproblems {{{";
}

void SummaryView::done( )
{
  ostr << "}}}}" << flush;
}


// ------------------------ ResultView  ------------------------//

ResultView::ResultView(ostream& o, OID oid, const Text& title,
    const Text& banner, const char* helpID, const Chars abbreviation)
  : ostr( o ),m_oid(oid)
{
  ostr
	 << "viewBuilder resultView viewParameters {viewID "
	 << oid
	 << " viewAbbreviation {"
	 << abbreviation
	 << "} viewTitle {"
	 << title
	 << "} viewBanner {"
	 << banner
	 << "} totalARCs {ARCSlotID -1 value 0} helpID " << helpID
	 // The canonical ARCSlotID for totalARCs is -1.
	 << " subproblems {{{";
}

void ResultView::done( )
{
  ostr << "}}}}" << flush;
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
 *   $Id: ARCer.C,v 1.6 1997/06/23 18:50:17 bormotov Exp $
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
#line 1  "src/ViewContents.C"
/*
 *   $Id: ViewContents.C,v 1.4 2000/09/21 21:18:00 alex Exp $
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

#line 1  "src/SMObject.C"
/*
 *   $Id: SMObject.C,v 1.7 2000/09/26 20:38:33 bormotov Exp $
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
//const IconID IconID::DataSetComputation = IconID("DataSetComputation");
const IconID IconID::DataSetDataSummary = IconID("DataSetDataSummary");
const IconID IconID::DataSetFullAnalysis = IconID("DataSetFullAnalysis");
const IconID IconID::DataSetGraphicAnalysis = IconID("DataSetGraphicAnalysis");
const IconID IconID::DataSetGoodnessOfFit = IconID("DataSetGoodnessOfFit");
const IconID IconID::DataSetMeanAnalysis = IconID("DataSetMeanAnalysis");
const IconID IconID::DataSetVarianceAnalysis = IconID("DataSetVarianceAnalysis");
const IconID IconID::DataSet = IconID("DataSet");
const IconID IconID::Polynomial = IconID("Polynomial");
const IconID IconID::none = IconID("none");
const IconID IconID::do_not_display = IconID("do_not_display");
#line 1  "src/ConstructSampleFromDataSet.C"
/*
 *   $Id: ConstructSampleFromDataSet.C,v 1.35 2001/03/01 00:04:04 alex Exp $
 */
 
// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Implementations of Construct Sample From Data Set Problem
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:


#include "ConstructSampleFromDataSet.h"
#include "OutMessages.h"
#include "conversions.h"
#include "OneVariableDataSet.h"


//---------------------------------------------------------------------------//
//--------------------- Full Data Analysis ----------------------------------//
//---------------------------------------------------------------------------//


void ConstructSampleFromDataSet::takeControl( )
{
  LogMessage msg( "Construct Sample From Data Set :");
  OneVariableDataSet ds(theDataSet.getSet());
  msg << "Unimplemented! ";
  msg.send();
}
