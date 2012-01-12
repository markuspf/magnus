// Contents: Defines class KState
//
// Principal Author: Dmitry Bormotov 
//
// Status: in progress
//
// Revision History:
//


#include "KState.h"


// -------------------------------- KState --------------------------------- //


KState::KState( int NumOfValues, int Values[] )
  : numOfValues( NumOfValues ),
    values( 0 ),
    knowledge( 0 )
{ 
  values = new int[numOfValues];
  for( int i = 0; i < numOfValues; ++i )
    values[i] = Values[i];
  knowledge = new ListOf<int>*[numOfValues];
  for( int i = 0; i < numOfValues; ++i )
    knowledge[i] = new ListOf<int>;
}

KState::~KState( )
{
  delete [] values;
  for( int i = 0; i < numOfValues; ++i )
    delete knowledge[i];
  delete [] knowledge;
}

void KState::printOn( ostream& ostr) const
{
  ostr << "(";
  for( int i = 0; i < numOfValues-1; ++i )
    cout << values[i] << ",";
  cout << values[numOfValues-1] << ") ";

  // print agents' knowledge

  cout << "   ";
  for( int i = 0; i < numOfValues; ++i ) {
    int listLen = knowledge[i]->length();
    if( listLen != 0 ) {
      cout << "A" << i+1 << ": ";
      for( int j = 0; j < listLen; ++j )
	cout << (*knowledge[i])[j] << " ";
    }
  }
  //cout << endl;
}

bool KState::operator == ( const KState& s ) const
{
  if( numOfValues != s.numOfValues ) return false;

  for( int i = 0; i < numOfValues; ++i )
    if( values[i] != s.values[i] )
      return false;

  return true;
}

KState& KState::operator = ( const KState& s )
{
  delete [] values;
  for( int i = 0; i < numOfValues; ++i )
    delete knowledge[i];
  delete [] knowledge;
  numOfValues = s.numOfValues;
  values = new int[numOfValues];

  for( int i = 0; i < numOfValues; ++i )
    values[i] = s.values[i];
 
  knowledge = new ListOf<int>* [numOfValues];
  for( int i = 0; i < numOfValues; ++i )
     knowledge[i] = new ListOf<int>(*s.knowledge[i]);

  return *this;
}

KState::KState( const KState& s)
{
  numOfValues = s.numOfValues;

  values = new int[numOfValues];
  for( int i = 0; i < numOfValues; ++i )
    values[i] = s.values[i];

  knowledge = new ListOf<int>* [numOfValues];
  for( int i = 0; i < numOfValues; ++i )
     knowledge[i] = new ListOf<int>(*s.knowledge[i]);
}

int KState::operator [] ( int ind ) const
{
  if (ind >= 0 && ind < numOfValues )
    return values[ind];
  else
    error("int KState::operator [] ( int ind ) const: "
	  "Index is out of bounds");
}

ListOf<int>& KState::getAgentList( int agent ) const
{
  if( agent < 0 || agent > numOfValues )
    error("ListOf<int>& KState::getAgentList( int agent ) const : "
	  "the agent number is out of bounds");
  else
    return *(knowledge[agent]);
}
