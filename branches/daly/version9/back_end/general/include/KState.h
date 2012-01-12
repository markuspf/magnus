// Contents: Declares class KState
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
 
#ifndef _KState_h_
#define _KState_h_

#include "Stack.h"


// -------------------------------- KState --------------------------------- //


// This class is used by class KripkeStructure only

class KState {

public:
  
  KState( ) { }

  KState( int NumOfValues, int Values[] );

  KState( const KState& );
  
  ~KState( );

  friend ostream& operator << ( ostream& ostr, const KState& s )
  {
    s.printOn(ostr);
    return ostr;
  }

  bool operator == ( const KState& ) const;
 
  KState& operator = ( const KState& s );
   
  int operator [ ] ( int ind ) const;

  ListOf<int>& getAgentList( int agent ) const;
  // returns the reference to the list of knowledge of 'agent' 

  int getNumOfValues( ) const { return numOfValues; }

  // IPC tools (temporary)  

  friend ostream& operator < ( ostream& ostr, const KState& )
  {
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, KState& )
  {
    return istr;
  }

private:

  void printOn( ostream& ostr) const;

  // Data members

  int numOfValues;
  int* values;
  ListOf<int> **knowledge; 
  // knowlegde[i] is a list of known functions for agent i  
};

#endif
