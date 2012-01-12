// Contents: The program solves problems using Kripke structure 
//
// Principal Authors: Dmitry Bormotov
//


#include "KripkeStructure.h"
#include "Set.h"
#include "Word.h"
#include "iostream.h"


void main( )
{
  const int numOfAgents = 2;

  // Enter information about the variables
  /*
  int numOfAgents;
  while( true ) {
    cout << "Enter the number of agents: ";
    cin >> numOfAgents;
    cout << endl;
    if( numOfAgents < 2 )
      cout << "The number of agents should be >= 2" << endl;
    else 
      break;
  }
  */
  int numOfVars;
  while( true ) {
    cout << "Enter the number of variables: ";
    cin >> numOfVars;
    cout << endl;
    if( numOfVars < 1 || numOfVars > 26 )
      cout << "The number of variables should be between 1 and 26" << endl;
    else 
      break;
  }

  VectorOf<int> lBound(numOfVars);
  VectorOf<int> uBound(numOfVars);
  bool goodNumbers;

  for( int i = 0; i < numOfVars; ++i ) {
    
    do {
      cout << "Enter the range of variable " << char('a' + i)
	   << " (lower bound <space> upper bound): ";
      cin >> lBound[i] >> uBound[i];
      cout << endl;
      if( !cin )
	cout << "Input error" << endl;

      goodNumbers = true;
      if( lBound[i] < 0 || uBound[i] < 0 ) {
	cout << "All values must be greater or equal than 0" << endl;
	goodNumbers = false;
      }
    } while( !cin || !goodNumbers );

  }

  // Main loop: enter a new command and execute it
  
  KripkeStructure KS( numOfAgents, lBound, uBound );
  cout << "The current set has " << KS.numOfStates() << " states. " 
       << "Type 'p;' to print it." << endl;

  while( true ) {

    cout << ">";
    char s[3000];
    char c;
    int si = 0;

    while( true ) {
      cin >> c;
      if( c == ';' ) break;
      if( cin.eof() ) return 0;
      s[si++] = c;
    }
    s[si++] = 0;

    //cout << endl;
    //if( !cin ) return 0;

    VectorOf<int> l,u;
    Expression E(l,u);
    Chars input = E.preprocess(Chars(s));
    if( input.length() == 0 ) continue;
    //cout << "input: " << input << endl;

    if( input[0] == 'p' ) {
      
      // print the current set
      cout << KS;

    } else if( input[0] == 'q' ) {
      
      // quit
      return 0;

    } else if( input[0] == '?' ) {
      
      // answer the question
      const char* t = input;
      Chars q(t+1); 
      bool answer;
      if( KS.question(q, answer) ) {
	if( answer )
	  cout << "YES" << endl;
	else
	  cout << "NO" << endl;
      }

    } else if( input[0] == '.' ) {     
      const char* t = input;
      KS.acceptInitialStatement(t+1);
    }

    else {
      
      int count = 0, inputLen = input.length();
      for( int i = 0; i < inputLen; ++i )
	if( input[i] == 'K' ) 
	  ++count;
      if( count >= 1 )
	KS.acceptStatement(input);
      else
	KS.narrowSet(input);

      cout << "The current set has " << KS.numOfStates() << " states. " 
	   << "Type 'p;' to print it." << endl;
    }
    
  }
  
}


