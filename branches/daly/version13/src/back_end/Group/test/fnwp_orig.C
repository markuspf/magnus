// Copyright (C) 1994 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: 
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include "FreeGroup.h"
#include "Polynomial.h"

#define debug_fnwp


// Basic types

struct State { 
  
  State( int Coef = 1, int WPos = 0, int VPos = 0, int Power = 1) { 
    coef = Coef; wPos = WPos; vPos = VPos, power = Power; }
  
  int coef; int wPos; int vPos; int power; 
};


struct Stack{ 
  
  Stack( int c ) : stackLen(0) { states = new State[c]; }
  
  ~Stack( ) { delete [] states; }
  
  void put( State s ) { states[stackLen++] = s; }
  
  void get( State& s) 
  { 
    if( stackLen > 0 )
      s = states[--stackLen];
  }
  
  int length( ) { return stackLen; }
  
  State& state() { return states[stackLen - 1]; }
  
private:

  State* states;
  int stackLen;
};


bool wordProblem( const FreeGroup& F, int c, const Word& w )
{
  // Construct two possible types of monomials:
  // PM: (1 + x) and NM: ( 1 - x + x^2 - x^3 + ... x^c ) 

  int wLen = w.length();
  int numOfGens = F.numberOfGenerators();

  VectorOf<int> PM(c), NM(c);
  
  for( int i = 0; i < c; ++i ) {
    PM[i] = 0;
    if( odd(i+1) )
      NM[i] = -1;
    else
      NM[i] = 1;
  }
  PM[0] = 1;
  
  // Construct all monomials
  
  VectorOf<int>* M[wLen];
  for( int i = 0; i < wLen; ++i )
    if( ord(Generator(w[i])) > 0 )
      M[i] = &PM;
    else
      M[i] = &NM;

  // Enumerate all possible monomials and compute their coefficients.
  // They all are equal to 0 iff the word is trivial.

  VectorOf<int> v(c);
  for( int i = 0; i < c; ++i )
    v[i] = 0;

  // The main cycle

  while( true ) {
    
    // Construct the current monomial v

    int i = c-1;
    while( true ) {

      if( ++v[i] <= numOfGens )	break;
      v[i--] = 1;
      if( i < 0 ) return true;
    }

    for( i = 0; i < c; ++i )
      if( v[i] > 0 ) break;

    // Compute the sum of it's coefficients in M
    
    int sum = 0;
    Stack stack(c);
    State state(1, -1, i);
    bool checkPower = false;

    while( true ) {

      if( checkPower && v[state.vPos] == v[state.vPos + state.power] &&  
	  (*M[state.wPos])[state.power] != 0 ) {
	
	//state.vPos += state.power;
	++state.power;
      }
      else {

	++state.wPos;
	state.power = 1;

	for( ; state.wPos < wLen; ++state.wPos )
	  if( abs(ord(Generator(w[state.wPos]))) == v[state.vPos] )
	    break;
      }
      
      if( state.wPos == wLen) {

	if( stack.length() == 0 ) break;

	stack.get(state);
	checkPower = true;
      }
      else {

	stack.put(state);
	if( checkPower ) {
	  state.coef *= (*M[state.wPos])[state.power-1];
	  state.vPos += state.power;
	  state.power = 1;
	}
	else {
	  state.coef *= (*M[state.wPos])[0];
	  ++state.vPos;
	}
	checkPower = false;
	
	if( state.vPos >= c ) {
	  
	  sum += state.coef;
	  stack.get(state);
	}
      }
    }

  #ifdef debug_fnwp
    cout << "Monomial: " << v << "  Sum = " << sum << endl;
  #endif

    if( sum != 0 ) return false;
  }
}



int main ( )
{
  Polynomial<Integer> p;
  Map M;
  FreeGroup F;
  cout << "Enter a free group: ";
  cin >> F;
  cout << endl;
  
  int c;
  cout << "Enter the nilpotency class: ";
  cin >> c;
  cout << endl;

  while( 1 ) {
    
    Chars err;
    Word w;
    cout << "Enter a word: ";
    w = F.readWord(cin, err);
    cout << endl << endl;
    
    cout << "You entered: F = " << c << " " << F << endl;
    cout << "w = ";
    F.printWord(cout, w);
    cout << endl;
    
    bool result = wordProblem(F,c,w);
    
    cout << "The word w is ";
    if( !result) 
      cout << "NOT ";
    cout << "trivial." << endl << endl;
  }
}





