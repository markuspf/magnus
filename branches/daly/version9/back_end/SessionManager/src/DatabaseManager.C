// Copyright (C) 1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class DataBaseState
//
// Principal Author: Dmitry Pechkin
//
// Status: in progress
//
// Revision History:
//


//
//  
// 

#include "OutMessages.h"
#include "DatabaseManager.h"
#include "TheObjects.h"
#include "AlgebraicObject.h"
#define DEBUG_DATABASE 1

// forward declaration
extern DatabaseManager TheDatabaseManager;
extern DatabaseObjectSmith TheDatabaseObjectSmith;


void printOn( ostream& ostr, const DB2FE_MESSAGE& msg )
{
  switch( msg ) {
  case NO_MESSAGE: ostr << "NO_MESSAGE"; break;
  case MSG_NEW_FILENAME: ostr << "MSG_NEW_FILENAME"; break;
  case MSG_NEW_DATABASE: ostr << "MSG_NEW_DATABASE"; break;
  case MSG_OPEN_DATABASE: ostr << "MSG_OPEN_DATABASE"; break;
  case MSG_SAVE_DATABASE: ostr << "MSG_SAVE_DATABASE"; break;
  case MSG_SAVE_DATABASE_AS: ostr << "MSG_SAVE_DATABASE_AS"; break;
  case MSG_SAVE_ON_CLOSE: ostr << "MSG_SAVE_ON_CLOSE"; break;
  case MSG_CLOSE_DATABASE: ostr << "MSG_CLOSE_DATABASE"; break;
  case MSG_ADD_OBJECTS: ostr << "MSG_ADD_OBJECTS"; break;
  case MSG_GET_OBJECTS: ostr << "MSG_GET_OBJECTS"; break;
  default: ostr << "unknown message to FE";
  }
}

void printOn( ostream& ostr, const FE2DB_MESSAGE& msg ) 
{
  switch( msg ) {
  case DB_NO_EVENT: ostr << "DB_NO_EVENT"; break;
  case DB_NEW: ostr << "DB_NEW"; break;
  case DB_OPEN: ostr << "DB_OPEN"; break;
  case DB_SAVE: ostr << "DB_SAVE"; break;
  case DB_SAVE_AS: ostr << "DB_SAVE_AS"; break;
  case DB_CLOSE: ostr << "DB_CLOSE"; break;
  case DB_ADD_OBJECTS: ostr << "DB_ADD_OBJECTS"; break;
  case DB_GET_OBJECTS: ostr << "DB_GET_OBJECTS"; break;
  case DB_SELECT_OBJECTS: ostr << "DB_SELECT_OBJECTS"; break;
  case DB_OBJECT_DEFINITION: ostr << "DB_OBJECT_DEFINITION"; break;
  case DB_CANCEL: ostr << "DB_CANCEL"; break;
  case DB_SUCCESS: ostr << "DB_SUCCESS"; break;
  case DB_FAILURE: ostr << "DB_FAILURE"; break;
  case DB_YES: ostr << "DB_YES"; break;
  case DB_NO: ostr << "DB_NO"; break;
  default: ostr << "Unknown message FE2DB";
  }
}

ostream& operator<<( ostream& ostr, const DBEvent& event )
{
  ostr << '<';
  printOn( ostr, event.key);
  ostr << ':' << event.str << '>';
  return ostr;
}


///////////////////////////////////////////////////////////////////
//
//   States of Database
//
///////////////////////////////////////////////////////////////////

DBState::DBState( const DBState& state )
  : theNextState( copyState(state.theNextState) )
{ }

const DBState& DBState::operator=( const DBState& state )
{
  if( &state != this ) {
    delete theNextState;
    theNextState = copyState( state.theNextState );
  }
  return *this;
}

DBState::~DBState( )
{ 
  // We do not enter into infinite recursion because we should stop
  // on a null pointer somewhere inside.
  delete theNextState; // calling ~State().
}

IntermediateState::IntermediateState( const IntermediateState& state )
  : theFailure( copyState(state.theFailure) ),
    theSecondState( copyState(state.theSecondState) )
{
  // do nothing.
}

const IntermediateState& IntermediateState::operator=( 
  const IntermediateState& state )
{
  if( &state != this ) {
    DBState::operator=( state );
    delete theFailure;
    delete theSecondState;
    theFailure     =  copyState( state.theFailure );
    theSecondState =  copyState( state.theSecondState );
  }
  return *this;
}

IntermediateState::~IntermediateState() 
{ 
  // We do not enter into infinite recursion because we should stop
  // on a null pointer somewhere inside.
  delete theFailure;     // calling virtual ~State().
  delete theSecondState; // calling virtual ~DBState().
}

void IntermediateState::handleFailure() 
{
  if( theFailure ) {
    theNextState = theFailure;
    theFailure = 0;
  }
}

void IntermediateState::useSecondState( DBState* state ) 
{
  if( theSecondState ) {
    theNextState = theSecondState;
    theSecondState = 0;
    delete state;
  }
  else {
    theNextState = state;
  }
}

///////////////////////////////////////////////////////////////////////
//
//     User defined states
//
///////////////////////////////////////////////////////////////////////

void DatabaseClosed::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_NEW:
    theNextState = new DatabaseCreating( new DatabaseClosed() ); 
    break;
  case DB_OPEN:
    theNextState = new DatabaseOpening( new DatabaseClosed() );
    break;
  case DB_SAVE:
  case DB_SAVE_AS:
  case DB_CLOSE:
  case DB_ADD_OBJECTS:
  case DB_GET_OBJECTS: {
    Message msg;
    msg << "The database closed -- operation is not permitted. "
	<< "Try to create new or open existing database.";
    msg.send();
    break;
  }
  default:
    error("unknown state in DatabaseClosed::handleEvent(const DBEvent&)");
  }
}


void DatabaseSaved::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_NEW:
    theNextState = new DatabaseCreating( new DatabaseSaved() );
    break;
  case DB_OPEN:
    theNextState = new DatabaseOpening( new DatabaseSaved() );
    break;
  case DB_CLOSE:
    theNextState = new DatabaseClosed(); 
    break;
  case DB_SAVE: // do nothing;
    break;
  case DB_SAVE_AS:
    theNextState = new DatabaseSavingAs( new DatabaseSaved() );
    break;
  case DB_GET_OBJECTS:
    theNextState = new DatabaseGettingObjects( 
                     new DatabaseSaved(), // on failure
                     new DatabaseSaved()  // on success
                   );
    break;
  case DB_ADD_OBJECTS:
    theNextState = new DatabaseAddingObjects( new DatabaseSaved() );
    break;
  default:
    error("unknown state in DatabaseSaved::handleEvent(const DBEvent&)");
  }
}

void DatabaseModified::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_NEW:
    theNextState = new DatabaseClosing( 
                    new DatabaseModified(), // on failure
                    new DatabaseCreating( new DatabaseModified() ) //on success
                   );
    break;
  case DB_OPEN:
    theNextState = new DatabaseClosing( 
                     new DatabaseModified(), // on failure
                     new DatabaseOpening( new DatabaseModified() ) //on success
                   );
    break;
  case DB_CLOSE:
    theNextState = new DatabaseClosing( new DatabaseModified(),
					new DatabaseClosed() );
    break;
  case DB_SAVE:
    theNextState = new DatabaseSaving( new DatabaseModified() );
    break;
  case DB_SAVE_AS:
    theNextState = new DatabaseSavingAs( new DatabaseModified() );
    break;
  case DB_GET_OBJECTS: 
    theNextState = new DatabaseGettingObjects( new DatabaseModified() );
    break;
  case DB_ADD_OBJECTS:
    theNextState = new DatabaseAddingObjects( new DatabaseModified() );
    break;
  default:
    error("unknown state in DatabaseModified::handleEvent(const DBEvent&)");
  }
}

// Intermediate states' handlers.

void DatabaseCreating::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_SUCCESS: { 
    LogMessage msg;
    msg << "Created the database " << event.str << ".";
    msg.send();
    TheDatabaseManager.setFileName(event.str);
    TheDatabaseManager.postMessage(MSG_NEW_DATABASE);
    useSecondState( new DatabaseSaved() );
    break;
  }
  case DB_FAILURE: { 
    Message msg;
    msg << "Could not create the database " << event.str << ".";
    msg.send();
    handleFailure();
    break;
  }
  case DB_CANCEL:
    handleFailure();
    break;
  default:
    error("unknown state in DatabaseCreating::handleEvent(const DBEvent&)");
    handleFailure();
  }
}

void DatabaseOpening::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_SUCCESS: { 
    LogMessage msg;
    msg << "Open the database " << event.str << ".";
    msg.send();
    theNextState = new DatabaseSaved();
    break;
  }
  case DB_FAILURE: { 
    Message msg;
    msg << "The database was not opened -- " << event.str << ".";
    msg.send();
    handleFailure();
    break;
  }
  case DB_CANCEL:
    handleFailure();
    break;
  default:
    error("unknown state in DatabaseOpening::handleEvent(const DBEvent&)");
    handleFailure();
  }
}

void DatabaseSavingAs::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_SUCCESS: { 
    LogMessage msg;
    msg << "The database was saved in " << event.str << ".";
    msg.send();
    TheDatabaseManager.setFileName(event.str);

    theNextState = new DatabaseSaved();
    break;
  }
  case DB_CANCEL: { 
    handleFailure();
    break;
  }
  case DB_FAILURE: { 
    Message msg;
    msg << "The database was not opened -- " << event.str << ".";
    msg.send();
    handleFailure();
    break;
  }
  default:
    error("unknown state in DatabaseSaving::handleEvent(const DBEvent&)");
    handleFailure();
  }
}

void DatabaseSaving::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_SUCCESS: { 
    LogMessage msg;
    msg << "The database was saved in " << event.str << ".";
    msg.send();
    TheDatabaseManager.setFileName(event.str);
    useSecondState( new DatabaseSaved() );
    break;
  }
  case DB_FAILURE: { 
    Message msg;
    msg << "The database was not opened -- " << event.str << ".";
    msg.send();
    handleFailure();
    break;
  }
  default:
    error("unknown state in DatabaseSaving::handleEvent(const DBEvent&)");
    handleFailure();
  }
}

void DatabaseClosing::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_YES:
    theNextState = new DatabaseSaving( theFailure, theSecondState );
    theFailure = theSecondState = 0;
    break;
  case DB_NO: {
    LogMessage msg;
    msg << "The changes in database were not saved.";
    msg.send();
    useSecondState( 0 );
    break;
  }
  case DB_CANCEL:
    handleFailure();
    break;
  default:
    error("unknown state in DatabaseClosing::handleEvent(const DBEvent&)");
    handleFailure();
  }
}

void DatabaseAddingObjects::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_SUCCESS: { 
    LogMessage msg;
    msg << "Update the database with new experiment" << event.str << ".";
    msg.send();
    theNextState = new DatabaseModified();
    break;
  }
  case DB_CANCEL:
    handleFailure();
    break;
  default:
    error("unknown state in DatabaseAddingObjects::handleEvent(DBEvent&)");
    handleFailure();
  }
}

void DatabaseGettingObjects::handleEvent( const DBEvent& event )
{
  switch( event.key ) {
  case DB_SUCCESS: { 
    useSecondState( 0 );
    break;
  }
  case DB_CANCEL:
    handleFailure();
    break;
  default:
    error("unknown state in DatabaseGettingObjects::handleEvent(DBEvent&)");
    handleFailure();
  }
}


/////////////////////////////////////////////////////////////////////////
//
//  DatabaseManager class.
//
/////////////////////////////////////////////////////////////////////////


DatabaseManager::DatabaseManager( ) 
  : theCurrentState( new DatabaseClosed() ),
    theFileName(), theExternalState(WORKING), event()
{
}

DatabaseManager::~DatabaseManager( ) 
{
  delete theCurrentState;
}

void DatabaseManager::printGlobalMessageTemplate( ostream& ostr )
{
  const int oid = -3; //HACK: SessionManager::databaseManagerOid;
  ostr 
    << "init_db_new_msg_ {"     << oid << ' ' << DB_NEW     << '}' << endl
    << "init_db_open_msg_ {"    << oid << ' ' << DB_OPEN    << '}' << endl
    << "init_db_save_msg_ {"    << oid << ' ' << DB_SAVE    << '}' << endl
    << "init_db_save_as_msg_ {" << oid << ' ' << DB_SAVE_AS << '}' << endl
    << "init_db_close_msg_ {"   << oid << ' ' << DB_CLOSE   << '}' << endl
    << "init_db_cancel_msg_ {"  << oid << ' ' << DB_CANCEL  << '}' << endl
    << "init_db_success_msg_ {" << oid << ' ' << DB_SUCCESS << '}' << endl
    << "init_db_failure_msg_ {" << oid << ' ' << DB_FAILURE << '}' << endl
    << "init_db_yes_msg_ {"     << oid << ' ' << DB_YES     << '}' << endl
    << "init_db_no_msg_ {"      << oid << ' ' << DB_NO      << '}' << endl
    << "init_db_get_exp_msg_ {" << oid << ' ' << DB_GET_OBJECTS << '}' << endl
    << "init_db_add_exp_msg_ {" << oid << ' ' << DB_ADD_OBJECTS << '}' << endl
    << "init_db_select_objects_msg_ {" << oid << ' ' << DB_SELECT_OBJECTS 
                                       << '}' << endl
    << "init_db_object_definition_msg_ {" << oid << ' ' << DB_OBJECT_DEFINITION
                                       << '}' << endl;


  printDatabaseObjectCategories( ostr );
}

void DatabaseManager::postMessage( const DB2FE_MESSAGE message ) const
{
  InvokingMessage msg;
  switch( message ) {
  case NO_MESSAGE:
    return;
  case MSG_NEW_FILENAME:
    msg << "new_filename_";
    break;
  case MSG_NEW_DATABASE:
    msg << "new_database_ {" << theFileName << "}";
    break;
  case MSG_OPEN_DATABASE:
    msg << "open_database_";
    break;
  case MSG_SAVE_DATABASE:
    msg << "save_database_";
    break;
  case MSG_SAVE_DATABASE_AS:
    msg << "save_database_as_";
    break;
  case MSG_CLOSE_DATABASE:
    msg << "close_database_";
    break;
  case MSG_SAVE_ON_CLOSE:
    msg << "save_on_close_";
    break;
  case MSG_ADD_OBJECTS:
    msg << "db_add_experiment_";
    break;
  case MSG_GET_OBJECTS:
    msg << "db_get_experiment_";
    break;
  default:
    error( "Unknown message in DatabaseManager::postMessage(...)" );
  }
  msg.send();
}

void DatabaseManager::setFileName( const Chars& filename ) 
{
  theFileName = filename;
}

Chars DatabaseManager::getSecondaryFileName( ) const 
{
  Chars sec_name;
  sec_name = theFileName + ".sec";
  return sec_name;
}

void DatabaseManager::forceToFinish() 
{ 
  theExternalState = STOPPING; 
}

#ifdef DEBUG_DATABASE
ofstream dbg("/home/users/pechkin6/.magnus/db_events", ios::app);
#endif

void DatabaseManager::takeControl( )
{
  if( theExternalState != STOPPED ) {

    if( theExternalState == STOPPING && theCurrentState->isFinal() ) {
      theExternalState = STOPPED;
      return;
    }

    if( event.key != DB_NO_EVENT ) {

#ifdef DEBUG_DATABASE
      static bool start = true;
      if( start ) {
	dbg << "\n\nStarting a session....\n" << endl;
	start = false;
      }

      dbg << "current state = " << *theCurrentState 
	  << ", event = " << event << endl;
#endif

      theCurrentState -> handleEvent( event );
      const DBState *nextState = theCurrentState -> nextState();
      if( nextState ) {
	DBState *oldState = theCurrentState;
	theCurrentState = nextState -> clone();
	delete oldState;
      }
      postMessage( theCurrentState -> initMessage( ) );
    }
  }
  event = DBEvent();
}


void DatabaseManager::readMessage( istream& istr )
{
  int k;
  istr >> k;

  event.key = (FE2DB_MESSAGE) k; 
  event.str = Chars();

  if( event.key == DB_SELECT_OBJECTS ) {

    InvokingMessage msg;
    msg << "db_select_objects_details_ {";

    int n; 
    istr >> n;

    for(int i = 0; i < n; ++i ) {
      int oid;
      istr >> oid;

      AlgebraicObject *object = (AlgebraicObject*) TheObjects::get( oid );

      msg << " { " << oid << ' ' 
	  << DatabaseObjectCategory::getSMObjectCategory( object ) << ' ';

      InformationCenter *ic = object->infoCenter();
      if( !ic ) {
	msg << "{}";
      }
      else {
	ic->write( msg, true );
      }

      msg << " }";
    }

    msg << " }";
    msg.send();

    event.key = DB_NO_EVENT;
    return;

  } else if ( event.key == DB_OBJECT_DEFINITION ) {

    TheDatabaseObjectSmith.checkinObjectFromDatabase( istr );

    event.key = DB_NO_EVENT;
    return;
  }

  char ch = ' ';

  while( !istr.eof() && (ch = istr.get()) == ' '  )
    ; 

  if( ch != ' ' && ch != '\n' && ch != EOF ) event.str = ch;

  while( !istr.eof() && (ch = istr.get()) != '\n'  && ch != EOF  )
    event.str += ch;
}

#define QUALIFY(c) (DatabaseObjectCategory::c)
#define FORCECATEGORY(c) ((DatabaseObjectCategory::Category)c)
#define CATEGORY(c) DatabaseObjectCategory::str( QUALIFY(c) ) << ' ' \
                     << QUALIFY(c)
 
void DatabaseManager::printDatabaseObjectCategories(ostream& ostr) 
{

}



//////////////////////////////////////////////////////////////
//
//  Class DatabaseObjectCategory
//
//////////////////////////////////////////////////////////////


Chars DatabaseObjectCategory::str(Category category) 
{
  Chars res;
  switch( category ) {
       default: res = "";
  }
  return res;
} 

#define SAMETYPES(a,b) (!strcmp(a,b))
#define CATEGORYSTR(c) (DatabaseObjectCategory::str(QUALIFY(c))) 

Chars DatabaseObjectCategory::getSMObjectCategory( SMObject *smo ) 
{
  const char * type = smo->typeID(); 
  Chars res;
  return Chars("{") + res + Chars("}");
}

//////////////////////////////////////////////////////////////
//
//  Class DatabaseObjectSmith
//
//////////////////////////////////////////////////////////////

void DatabaseObjectSmith::reset( )
{

  if( theExperiment.length() > 0 ) {
    int total = 0;
    ostrstream names;
    for( int i = 0; i < theExperiment.length(); ++i ) {
      if( theExperiment[i] ) {
	if( total )
	  names << ", ";
	names << FEData::Name( *theExperiment[i] );
	++total;
      }
    }
    names << ends;

    LogMessage msg;
    msg << total << " objects loaded from database";
    if( total ) {
      msg << ":  " << names.str();  
      names.freeze(0);
    }
    msg << ".";
    msg.send();
  }

  theExperiment.shrink(0); 
}

void DatabaseObjectSmith::checkinObjectDefinition( istream& istr )
{
  istr >> theObjectName; 
  theObjectName = decodeSpaces(theObjectName);
  
  DatabaseObjectCategory theObjectCategory;
  istr >> theObjectCategory;
  
  SMObject *smo = 0;
  
  switch( theObjectCategory.id() ) {
  default:
    ;
  }

  theExperiment.append( smo );
  if( smo ) {
    CheckinMessage( *smo, "" /*theObjectName*/, dependencies ).send();
  }
  dependencies = ListOf<OID>();
}

void DatabaseObjectSmith::checkinObjectProperties( istream& istr )
{
  AlgebraicObject *current = 
    (AlgebraicObject *)theExperiment[ theExperiment.length()-1 ];

  if( current && current->infoCenter() ) {
    current->infoCenter()->read( istr, true );
  }
}

void DatabaseObjectSmith::checkinObjectFromDatabase( istream& istr )
{
  enum {OBJECTS_STOP = 0, CHECKIN_OBJECT = 1, LOAD_PROPERTIES = 2};
  int command;
  istr >> command;

  switch( command ) {
  case OBJECTS_STOP: {
    reset();
    break;
  } 
  case CHECKIN_OBJECT: {
    checkinObjectDefinition(istr);
    break;
  }
  case LOAD_PROPERTIES: {
    checkinObjectProperties(istr);
    break;
  }
  default:
    error("DatabaseObjectSmith::checkinObjectFromDatabase(): unknown command");
  }

  if( theError.length() > 0 ) {
    LogMessage msg;
    msg << theError;
    msg.send();
    theError = Chars();
  }

}


//---------------------------------------------------------------------------//
//----------------------- Static data ---------------------------------------//
//---------------------------------------------------------------------------//

DatabaseManager TheDatabaseManager;
DatabaseObjectSmith TheDatabaseObjectSmith;
