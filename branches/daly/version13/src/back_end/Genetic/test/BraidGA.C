// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test.
//
// Principal Author: Dmitry Bormotov
//
// Status:
//
// Revision History:
//


#include <values.h>
#include "global.h"
#include "Map.h"
#include "Config.h"
#include "RandomNumbers.h"
#include "Roulette.h"
#include "GAEquationSolver.h"
#include "EquationParser.h"
#include "File.h"
#include "BraidGroupToolkit.h"

int main ( )
{
  SetOf<Map> BS, Stab;

  {
    // Reading basic solutions
    
    ifstream in("../bs.gp");
    if( !in ) 
      error("cannot open bs.gp");
    
    char s[3001], c;
    SetOf<Map> BS;
    Chars errMsg;
    
    VectorOf<Chars> v(4);
    v[0] = "a1"; v[1] = "a2"; v[2] = "a3"; v[3] = "a4"; 
    FreeGroup R(v);
    
    v[0] = "y1"; v[1] = "y2"; v[2] = "y3"; v[3] = "y4"; 
    FreeGroup D(v);
    
    while( !in.eof() ) {
      
      in.get(s, 3000, '\n');
      in.get(c);
      if( strlen(s) == 0 ) continue;
      //cout << "111" << endl << s << endl << "222" << endl << endl;
      
      Word w;
      istrstream sin(s);
      VectorOf<Word> im(4);
      int n = 0;
      
      while( !sin.eof() && n < 4 ) {
	sin >> c;
	if( c == '-' ) {
	  sin >> c;
	  if( c != '>' ) continue;
	  //cout << "n = " << n << endl;
	  im[n++] = R.readWord(sin, errMsg);
	}
      }
      
      BS |= Map(D,R,im);
    }
    
    cout << "Cardinality of BS: " << BS.cardinality() << endl;
    //cout << "BS: " << endl << BS << endl;
  }

  {
    // Reading stabilizer
    
    ifstream in("../st.gp");
    if( !in ) 
      error("cannot open st.gp");
    
    char s[3001], c;
    Chars errMsg;
    
    VectorOf<Chars> v(8);
    v[0] = "y1"; v[1] = "y2"; v[2] = "y3"; v[3] = "y4"; 
    v[4] = "a1"; v[5] = "a2"; v[6] = "a3"; v[7] = "a4"; 
    FreeGroup R(v);

    VectorOf<Chars> u(4);
    u[0] = "y1"; u[1] = "y2"; u[2] = "y3"; u[3] = "y4"; 
    FreeGroup D(u);
    
    while( !in.eof() ) {
      
      in.get(s, 3000, '\n');
      in.get(c);
      if( strlen(s) == 0 ) continue;
      //cout << "111" << endl << s << endl << "222" << endl << endl;
      
      Word w;
      istrstream sin(s);
      VectorOf<Word> im(4);
      int n = 0;
      
      while( !sin.eof() && n < 4 ) {
	sin >> c;
	if( c == '-' ) {
	  sin >> c;
	  if( c != '>' ) continue;
	  //cout << "n = " << n << endl;
	  im[n++] = R.readWord(sin, errMsg);
	}
      }
      
      Stab |= Map(D,R,im);
    }
    
    cout << "Cardinality of Stab: " << Stab.cardinality() << endl;
    //cout << "Stab: " << endl << Stab << endl;
  }



}

