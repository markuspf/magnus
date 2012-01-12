// Contents: Defines class KripkeStructure
//
// Principal Author: Dmitry Bormotov 
//
// Status: in progress
//
// Revision History:
//


#include "KripkeStructure.h"


// --------------------------------- Token --------------------------------- //


Token::Token(char qu, char va, ListOf<Token> pExp)
  : q( qu ), 
    v( va ),
    expr( 0 ),
    isExpr( true )
{
  expr = new ListOf<Token>(pExp);
}

Token::Token( const Token& t )
{
  copy(t);
}

Token& Token::operator = ( const Token& t )
{
  if( isExpr ) delete expr;
  copy(t);
  return *this;
}

bool Token::operator == ( const Token& t) const {
  if( isExpr && t.isExpr )
    return q == t.q && v == t.v && *expr == *t.expr;
  else if( isChar && t.isChar ) return c == t.c; 
  else if( !isChar && !t.isChar ) return i == t.i;
  else return false;
}

void Token::copy( const Token& t )
{
  if( t.isExpr ) {
    q = t.q;
    v = t.v;
    expr = new ListOf<Token>(*t.expr);
    isExpr = true;
  }
  else {
    isChar = t.isChar;
    c = t.c;
    i = t.i;
    isExpr = false;
    expr = 0;
  }
}


// ------------------------------ Expression ------------------------------- //


Expression::Expression( VectorOf<int> lBound, VectorOf<int> uBound  )
{
  int len = lBound.length();
  if( len == 0 ) return;

  minV = lBound[0];
  for( int i = 1; i < len; ++i )
    if( lBound[i] < minV ) 
      minV = lBound[i];

  maxV = uBound[0];
  for( int i = 1; i < len; ++i )
    if( uBound[i] > maxV ) 
      maxV = uBound[i];
}

bool Expression::parseExpression( Chars expr, ListOf<Token>& pExp )
{
  StackOf<Token> theStack;
  ListOf<Token> result;
  char s[3000];
  int eLen = expr.length(); 
  for( int i = 0; i < eLen; ++i)
    s[i] = expr[i];
  s[eLen] = 0;

  istrstream in(s);
  char c;
  
  while( true ) {

    if( !in.get(c) ) break;

    if( isdigit(c) ) {
      int n = 0;
      in.putback(c);
      while( true ) {
	if( !in.get(c) ) break;
	if( !isdigit(c) ) { in.putback(c); break; }
	n = n * 10 + (c - '0');
      }
      result.append(n); 
    }

    else if( c == 'E' || c == 'A' ) {
      char q = c,v;
      if( !in.get(v) ) return false;
      if( !isalpha(v) ) return false; 
      if( !in.get(c) ) return false;
      if( c != '(' ) return false; 
      char s[3000];
      int si = -1, count = 1;
      s[++si] = c;

      while( in.get(c) ) {
	s[++si] = c;
	if( c == '(' ) ++count;
	else if( c == ')' ) --count;
	if( count == 0 ) break;
      }
      s[++si] = 0;
      if( count != 0 ) return false;
      Chars expr(s);
      ListOf<Token> pExp;
      if( !parseExpression( expr, pExp ) ) return false;
      Token t(q,v,pExp);
      result.append(t);
    }

    else if( isalpha(c) ) 
      result.append(c);

    else if( c == '(' )
      theStack.push(Token(c));

    else if ( c == ')' ) {
      while( theStack.isntEmpty() ) {
	char t = theStack.pop().c;
	if( t == '(' ) 
	  break;
	else
	  result.append(t);
      }
    }

    else { // operation
      int pri = priority(c);
      if( pri < 0 ) return false;
      while( theStack.isntEmpty() ) {
	char t = theStack.pop().c;
	if( priority(t) >= pri )
	  result.append(t);
	else { 
	  theStack.push(Token(t));
	  break;
	}
      }
      theStack.push(Token(c));
    }
    
  }
  
  while( theStack.isntEmpty() )
    result.append(theStack.pop().c);
  
  pExp = result;

  //cout << "Parsed exp: " << pExp << endl;

  return true;
}

int Expression::priority( char op )
{
  switch( op ) {
  case '&' :
  case '|' :
  case '^' :
  case '_' :
    return 1;
  case '<' :
  case '>' :
  case '=' :
  case '[' :
  case ']' :
  case '!' :
    return 2;
  case '+' :
  case '-' :
    return 3;
  case '*' :
  case '/' :
  case '%' :
  case '#' :
    return 4;
  default:
    return -1;
  }
}

bool Expression::evaluateParsedExpression( ListOf<Token> exp, KState state, 
					   ListOf<Assigment> A, int& result )
{
  StackOf<Token> theStack;
  ListIterator< ListOf<Token> > I(exp);

  for( ; !I.done(); I.next() ) {

    if( I.value().isExpr ) {
	
      int i;
      for( i = minV; i <= maxV; ++i ) {
	ListOf<Assigment> l(A);
	l.append( Assigment(I.value().v, i) );
	int res;
	if( !evaluateParsedExpression( *I.value().expr, state, l, res) )
	  return false;
	if( I.value().q == 'E' && res == 1 ) {
	  theStack.push( Token( 1 ) );
	  break;
	}
	else if( I.value().q == 'A' && res == 0 ) {
	  theStack.push( Token( 0 ) );
	  break;
	}
      }
	
      if( i > maxV )
	if( I.value().q == 'E' )
	  theStack.push( Token( 0 ) );
	else if( I.value().q == 'A' )
	  theStack.push( Token( 1 ) );
    }

    else if( !I.value().isChar )
      theStack.push( Token( I.value().i ) );

    else if( isalpha(I.value().c) ) {
      int n = I.value().c - 'a';
      if( n < 0 || n >= state.getNumOfValues() ) {
	
	int ALen = A.length();
	bool bFound = false;
	for( int i = 0; i < ALen; ++i )
	  if( A[i].c == I.value().c ) {
	    theStack.push(Token(A[i].v));
	    bFound = true;
	    break;
	  }
	if( !bFound ) return false;
      }
      else 
	theStack.push(Token(state[n]));
    }

    else {
      int i1,i2;
      if( theStack.isEmpty() ) return false;
      i2 = theStack.pop().i;
      if( I.value().c != '^' ) {
	if( theStack.isEmpty() ) return false;
	i1 = theStack.pop().i;
      }
      else 
	i1 = i2;

      theStack.push( Token(compute(I.value().c, i1, i2)) );
    }
  }

  result = theStack.pop().i;
  if( theStack.isEmpty() ) 
    return true;
  else
    return false;
}

int Expression::compute(char op, int i1, int i2)
{
  switch( op ) {
  case '&': {
    if( i1 < 0 || i1 > 1 || i2 < 0 || i2 > 1 )
      error("int Expression::compute( ) : "
	    "Illegal arguments for logical operation");
    return (i1 == 1 && i2 == 1 ) ? 1 : 0;
  }
  case '|': {
    if( i1 < 0 || i1 > 1 || i2 < 0 || i2 > 1 )
      error("int Expression::compute( ) : "
	    "Illegal arguments for logical operation");
    return (i1 == 1 || i2 == 1 ) ? 1 : 0;
  }
  case '_': {
    if( i1 < 0 || i1 > 1 || i2 < 0 || i2 > 1 )
      error("int Expression::compute( ) : "
	    "Illegal arguments for logical operation");
    return (i1 == 1 && i2 == 0 ) ? 0 : 1;
  }
  case '^': {
    if( i1 < 0 || i1 > 1 )
      error("int Expression::compute( ) : "
	    "Illegal arguments for logical operation");
    return (i1 == 0) ? 1 : 0;
  }

  case '<':
    return i1<i2 ? 1 : 0;
  case '>':
    return i1>i2 ? 1 : 0;
  case '=':
    return i1==i2 ? 1 : 0;
  case '!':
    return i1!=i2 ? 1 : 0;
  case '[':
    return i1<=i2 ? 1 : 0;
  case ']':
    return i1>=i2 ? 1 : 0;

  case '+':
    return i1+i2;
  case '-': {
    int r = i1-i2;
    return (r >= 0) ? r : 0;
  }

  case '*':
    return i1*i2;
  case '/':
    return i1/i2;
  case '%':
    return i1%i2;
  case '#': {
    int res = 1;
    for( int y = 0; y < i2; ++y )
      res *= i1;
    return res;
  }

  default:
    error("int Expression::compute(char op, int i1, int i2) : "
	  "Illegal operation");
  }
}

Chars Expression::preprocess( Chars s)
{
  char t[3000], r[3000];

  // Eat spaces
  int sLen = strlen(s), j = -1;
  for( int i = 0; i < sLen; ++i )
    if( !isspace(s[i]) )
      t[++j] = s[i];
  t[++j] = 0;

  int i = 0, tLen = strlen(t);
  j = -1;

  while( i < tLen ) {

    if( t[i] == '<' && i+1 < tLen && t[i+1] == '=' ) 
      { r[++j] = '['; ++i; }
    else if( t[i] == '>' && i+1 < tLen && t[i+1] == '=' ) 
      { r[++j] = ']'; ++i; }
    else if( t[i] == '<' && i+1 < tLen && t[i+1] == '>' ) 
      { r[++j] = '!'; ++i; }
    else if( t[i] == '-' && i+1 < tLen && t[i+1] == '>' ) 
      { r[++j] = '_'; ++i; }
    else if( t[i] == '*' && i+1 < tLen && t[i+1] == '*' ) 
      { r[++j] = '#'; ++i; }
    else r[++j] = t[i];

    ++i;
  }

  r[++j] = 0;
  return r;
}

 
// ---------------------------- KripkeStructure ---------------------------- //


KripkeStructure::KripkeStructure( int NumOfAgents, VectorOf<int> LBound, 
				  VectorOf<int> UBound )
  : numOfAgents( NumOfAgents ), 
    lBound( LBound),
    uBound( UBound),
    numOfVars( LBound.length() ),
    E( LBound, UBound )
{
  // Enumerate all possible values for the variables and add them to the set

  theSet = SetOf<KState>();

  int v[numOfVars];
  for( int j1 = 0; j1 < numOfVars; ++j1 )
    v[j1] = lBound[j1];
  --v[numOfVars-1];
  
  bool endEnumeration = false;
  bool ready;
  int cur;
  
  while( !endEnumeration ) {
    
    cur = numOfVars - 1;
    do {
      ready = true;
      if( ++v[cur] > uBound[cur] ) {
	v[cur] = lBound[cur];
	if( --cur < 0 ) endEnumeration = true;
	ready = false;
      }
    } while( !ready && !endEnumeration);
    
    if( !endEnumeration ) {
      theSet |= KState(numOfVars,v); 
      //cout << "New addition " << KState(numOfVars,v) << endl;
      //cout << "Cardinality: " << theSet.cardinality() << endl;
    }
  }
  
}
    
void KripkeStructure::narrowSet( Chars input )
{
  ListOf<Token> pExp;
  if( !E.parseExpression(input, pExp) ) {
    cout << "narrowSet() reports input error : cannot parse the expression" 
	 << endl;
    return;
  }
  
  SetOf<KState> newSet;
  SetIterator<KState> I(theSet);
  for( ; !I.done(); I.next() ) {
    int result;
    ListOf<Assigment> l;
    if( !E.evaluateParsedExpression( pExp, I.value(), l, result ) ) {
      cout << "narrowSet() reports input error: "
	   << "cannot evaluate the expression" << endl;
      return;
    }
    
    if( result < 0 || result > 1 ) {
      cout << "Input error: expected boolean expression" << endl;
      return;
    }

    KState s(I.value());
    if( result == 1 )
      newSet |= s;
  }
  
  theSet = newSet;
}

void KripkeStructure::acceptInitialStatement( Chars input )
{
  
  const char* s = input;

  if( s[0] != 'K' ) {
    cout << "Input error: K expected";
    return;
  }
  int a = s[1] - '1';
  if( a < 0 || a >= numOfAgents ) {
    cout << "Input error: agent number expected";
    return;
  }

  addFunction(a, Chars(s+2));
}

void KripkeStructure::addFunction(int agent, Chars exp)
{
  ListOf<Token> pExp;
  if( !E.parseExpression(exp, pExp) ) {
    cout << "Input error: cannot parse the expression" << endl;
    return;
  }
  
  SetOf<KState> newSet;
  SetIterator<KState> I(theSet);
  for( ; !I.done(); I.next() ) {
    int result;
    ListOf<Assigment> l;
    if( !E.evaluateParsedExpression( pExp, I.value(), l, result ) ) {
      cout << "Input error: cannot evaluate the expression" << endl;
      return;
    }
    
    KState s(I.value());
    s.getAgentList( agent ).append( result );
    newSet |= s;
  }
  
  theSet = newSet;
}


bool KripkeStructure::acceptStatement( Chars input )
{
  int count = 0, inputLen = input.length();
  for( int x = 0; x < inputLen; ++x )
    if( input[x] == 'K' ) 
      ++count;
  if( count == 1 )
    return acceptSingleStatement(input);

  const char* s = input;
  bool knows;
  int j = 1;

  if( s[0] == 'K' ) 
    knows = true;
  else if( s[0] == '^' && s[1] == 'K' ) {
    knows = false;
    j = 2;
  }
  else {
    cout << "Input error: 'K' or '^K' expected";
    return false;
  }
  int a = s[j] - '1';
  if( a < 0 || a >= numOfAgents ) {
    cout << "Input error: agent number expected";
    return false;
  }

  SetOf<KState> S(theSet), Y, N;

  while( S.cardinality() != 0 ) {
    
    SetOf<KState> Cur;
    SetIterator<KState> I(S);

    KState st(I.value());
    ListOf<int>& l = st.getAgentList(a);
    int lLen = l.length();
    if( lLen == 0 ) {
      cout << "You must give agent " << a+1 
	   << " some initial knowledge first." << endl;
      return false;
    }
    
    for( I.reset(); !I.done(); I.next() ) {
      
      KState st2(I.value());
      ListOf<int>& l2 = st2.getAgentList(a);
      bool bEqual = true;
      
      for( int v = 0; v < lLen; ++v )
	if( l[v] != l2[v] ) {
	  bEqual = false;
	  break;
	}
      
      if( bEqual )
	Cur |= I.value();
    }

    SetIterator<KState> J(Cur);
    for( ; !J.done(); J.next() )
      S.removeElement(J.value());

    bool success = true;
    for( J.reset(); !J.done(); J.next() ) {
      bool result;
      if( !KripkeEvaluate( Chars(s+j+1), J.value(), result ) )
	return false;
      if( !result ) {
	success = false;
	break;
      }
    }
    if( success ) Y |= Cur; else N |= Cur;
  }
  
  if( knows ) theSet = Y; else theSet = N;
  return true;
}

bool KripkeStructure::acceptSingleStatement( Chars input )
{
  bool result;
  SetOf<KState> S;
  SetIterator<KState> I(theSet);
  for( ; !I.done(); I.next() ) {
    if( !KripkeEvaluate( input, I.value(), result ) )
      return false;
    if( result) 
      S |= I.value();
  }
  theSet = S;
  return true;
}

bool KripkeStructure::KripkeEvaluate( Chars input, KState state, bool& res )
{
  const char* s = input;
  bool knows;
  int j = 1;

  if( s[0] == 'K' ) 
    knows = true;
  else if( s[0] == '^' && s[1] == 'K' ) {
    knows = false;
    j = 2;
  }
  else {
    cout << "Input error: 'K' or '^K' expected";
    return false;
  }
  int a = s[j] - '1';
  if( a < 0 || a >= numOfAgents ) {
    cout << "Input error: agent number expected";
    return false;
  }

  SetOf<KState> S(theSet), Y, N;
    
  SetOf<KState> Cur;
  SetIterator<KState> I(S);

  KState st(state);
  ListOf<int>& l = st.getAgentList(a);
  int lLen = l.length();
  if( lLen == 0 ) {
    cout << "You must give agent " << a+1
	 << " some initial knowledge first." << endl;
    return false;
  }
 
  for( I.reset(); !I.done(); I.next() ) {

    KState st2(I.value());
    ListOf<int>& l2 = st2.getAgentList(a);
    bool bEqual = true;

    for( int v = 0; v < lLen; ++v )
      if( l[v] != l2[v] ) {
	bEqual = false;
	break;
      }

    if( bEqual )
      Cur |= I.value();
  }
 
  SetIterator<KState> J(Cur);
  bool result;

  if( s[j+1] == '^' || s[j+1] == 'K' ) {

    for( J.reset(); !J.done(); J.next() ) {
      if( !KripkeEvaluate( Chars(s+j+1), J.value(), result ) )
	return false;
      if( knows && !result || !knows && result ) {
	res = false;
	return true;
      }
    }
    
    res = true;
    return true;

  } else if( s[j+1] != '(' ) {
    cout << "KripkeEvaluate() reports input error: "
	 << "expected (" << endl;
    return false;

  } else {
    
    ListOf<Token> pExp;
    if( !E.parseExpression( Chars(s+j+1), pExp ) ) {
      cout << "KripkeEvaluate() reports input error "
	   << ": cannot parse the expression" 
	   << endl;
      return false;
    }

    bool bFirst = true;
    result = true;
    int theAns;

    for( J.reset(); !J.done(); J.next() ) {
      int ans;
      ListOf<Assigment> l;
      if( !E.evaluateParsedExpression( pExp, J.value(), l, ans ) ) {
	cout << "KripkeEvaluate() reports input error: "
	     << "cannot evaluate the expression" << endl;
	return false;
      }
      
      if( bFirst ) { theAns = ans; bFirst = false; }
      else if( theAns != ans ) { result = false; break; }		
    }
    
    if( knows == result ) res = true; else res = false;
    return true;
  }
}


bool KripkeStructure::question( Chars input, bool& answer )
{
  SetOf<KState> oldSet = theSet, newSet;
  if( !acceptStatement(input) ) 
    return false;
  newSet = theSet;
  theSet = oldSet;

  if( newSet.cardinality() == theSet.cardinality() )
    answer = true;
  else
    answer = false;

  return true;
}
