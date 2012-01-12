
#include "FPGroup.h"
#include "global.h"


//#include <values.h>

//#include "GHNConfig.h"
//#include "RandomNumbers.h"
//#include "Roulette.h"
//#include "GASubgroup.h"
//#include "Timer.h"


//#include "FGGroupRep.h"

//#include "List.h"



#include <iostream>
//#include <Integer.h>
//#include "Map.h"
//#include "AbelianGroup.h"
#include "Polynomial.h"
#include "Int2.h"
#include "Word.h"
#include "DArray.h"

int main(int argc, char* argv[])
{
  Polynomial<Int2> p1;
	Polynomial<Integer> p2;
	DArrayParser<Word> parser(cin);

				
  Generator x(1);
  Generator y(2);
  Word w(x);
  w *= y;
  //w < "x = y^123";
  SetOf<Word> s(w);
  FPGroup G(2, s);
  //Chars errMsg = cin >> G;

   //cout << "hello world" << endl;
//int x =5; 

   //FPGroup G();
   //G() ="<x,y;X y^2 x=y^321>";
   //WordEnumeratorProblem(G);




//VectorOf<Chars> w= 5;
//SetOf<Word> x = 4;

 
  
/*
  if ( errMsg.length() > 0 ) {
    cout << errMsg << endl << endl;
    return 1;
  }

  cout << "\n\nYou entered:\n";
  cout << G << endl << endl;
*/
	
//enumerate words of G here!!
}







