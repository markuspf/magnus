// Contents: Declares class KripkeStructure 
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
 
#ifndef _KripkeStructure_h_
#define _KripkeStructure_h_

#include "Set.h"
#include "Vector.h"
#include "List.h"
#include "KState.h"


// --------------------------------- Token --------------------------------- //


struct Assigment {
  
  Assigment();
  Assigment( char C, int V ) : c( C ), v( V ) { }

  bool operator == ( const Assigment& a) const {
    return c == a.c && v == a.v;
  }

  char c;
  int v;
};


struct Token {
  
  Token(): expr( 0 ) { }
  ~Token() { delete expr; }
  Token(int v) : isExpr( false ), isChar( false ), i(v), expr( 0 ) { }
  Token(char v) : isExpr( false ), isChar( true ), c(v), expr( 0 ) { }
  Token(char qu, char va, ListOf<Token> pExp);
  Token( const Token& );

  Token& operator = ( const Token& );
  bool operator == ( const Token& t) const;
  
  friend ostream& operator << ( ostream& ostr, const Token& t )
  {
    if( t.isExpr )
      ostr << t.q << t.v << *t.expr;
    else if( t.isChar ) ostr << t.c; else ostr << t.i;

    return ostr;
  }

  void copy( const Token& t );

  bool isChar;
  char c;
  int i;
  
  bool isExpr;
  char q,v;
  ListOf<Token>* expr;
};


// ------------------------------ Expression ------------------------------- //


class Expression {

public:

  Expression ( VectorOf<int> lBound, VectorOf<int> uBound ); 

  Chars Expression::preprocess( Chars );
  // remove spaces, replace '<=','>=','<>' by a single token
  
  bool parseExpression( Chars expr, ListOf<Token>& pExp );
  // parses 'expr', sends result to 'pExp', return true iff successful

  bool evaluateParsedExpression( ListOf<Token> exp, KState state, 
				  ListOf<Assigment> A, int& result );
  // evaluates 'exp' for state 'state', sends result to 'result',
  // return true iff successful


private:

  int priority( char op );
  // return priority for operation 'op' or -1 if op is illegal

  int compute(char op, int i1, int i2);
  // computes op(i1,i2)

  // Data members

  int minV, maxV;
};


// ---------------------------- KripkeStructure ---------------------------- //


class KripkeStructure {

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Constructors:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  KripkeStructure( int NumOfAgents, VectorOf<int> LBound, 
		   VectorOf<int> UBound );

  ~KripkeStructure( ) { }

 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Accessors:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator << ( ostream& ostr, const KripkeStructure& KS )
  {
    ostr << KS.theSet << endl;
    return ostr;
  }

  int numOfStates( ) const { return theSet.cardinality(); }
  
  void narrowSet( Chars input );
  
  void acceptInitialStatement( Chars input );
  // Accept statement like Ki(a+b), i.e. give agent i some 
  // initial knowledge

  bool acceptStatement( Chars input );

  bool question( Chars input, bool& answer );

  
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void addFunction( int agent, Chars exp );
  // add knowledge 'exp' to 'agent' 

  bool KripkeEvaluate( Chars input, KState state, bool& res );
  // evaluates something like Ki^Kj(exp), result is in res,
  // returns true iff successful

  bool acceptSingleStatement( Chars input );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
 
  int numOfAgents;
  VectorOf<int> lBound;
  VectorOf<int> uBound;
  int numOfVars;
  SetOf<KState> theSet;
  Expression E;
};

#endif
