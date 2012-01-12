// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Informal test: [w1 x w2, g] = 1
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
#include <sys/time.h>
#include <unistd.h>
#include "Matrix.h"
#include "Polynomial.h"

UniformRandom r;

double diffTime(timeval t2, timeval t1)
{
  double result;
  long s = t2.tv_sec - t1.tv_sec;
  result = (t2.tv_usec - t1.tv_usec) * 0.000001 + s;
  //if( t2.tv_usec < t1.tv_usec ) --result;
  return result;
}

/*
int main ( )
{
  timeval t1,t2;
  for( int i = 0; i < 10; ++i ) {
    gettimeofday(&t1, NULL);
    sleep(2);
    gettimeofday(&t2, NULL);
    cout << "t1 = " << t1.tv_sec << "," << t1.tv_usec << endl
	 << "t2 = " << t2.tv_sec << "," << t2.tv_usec << endl
	 << "diff: " << diffTime(t2,t1) << endl;
  }
}
*/

int randomGen( int rank )
{
  int gen = int(r.rand() * rank) + 1;
  return (r.rand() <= 0.5) ? gen : -gen;
}
/*
Word getEquation( int len )
{
  VectorOf<Generator> v(len);
  int i;
  for( i = 0; i < len/2; ++i )
    v[i] = Generator(2);
  for( ; i < len; ++i )
    v[i] = Generator(3);

  return Word(v);
}
*/

Word getEquation( int len )
{
  VectorOf<Generator> v(len);
  v[0] = randomGen(2);
  for( int i = 1; i < len; ++i ) {
    int gen;
    do {
      gen = randomGen(2);
    } while( gen == -ord(v[i-1]) );
    v[i] = gen;
  }

  for( int i = 0; i < len; ++i )
    if( ord(v[i]) < 0 ) v[i] = ord(v[i]) - 1;
    else v[i] = ord(v[i]) + 1;

  return Word(v);
}


int main ( )
{
  Matrix< Polynomial<Integer> > I(4);
  Polynomial<Integer> p("1-x1,");

  const int numOfExp = 10;

  Chars errMsg;
  GHNConfig c;
  //cout << c << endl;

  ifstream in("../eqs.cfg");
  in >> c;
  cout << c << endl;
 
  int len;
  cout << "Enter the length: ";
  cin >> len;
  cout << endl;

  VectorOf<Chars> names(3);
  names[0] = "x";
  names[1] = "a";
  names[2] = "b";
  FreeGroup G(names);
  int numOfVars = 1;
  
  double t[numOfExp];
  int g[numOfExp], l[numOfExp];

  for( int k = 0; k < numOfExp; ++k ) { 
    
    Word w = getEquation(2*len);
    cout << "The word: " ;
    G.printWord(cout, w);
    cout << endl;
    
    Word w1 = w.initialSegment(len).inverse();
    Word w2 = w.terminalSegment(len).inverse();
    /*
    cout << "w1: " ;
    G.printWord(cout, w1);
    cout << endl;
    
    cout << "w2: " ;
    G.printWord(cout, w2);
    cout << endl;
    */
    
    w = w1 * Word(Generator(1)) * w2;
    Word g1;
    do { 
      g1 = getEquation(len);
    } while ((w1 * g1 * w2).freelyReduce().length() < 3 * len );
   
    Word com = w.inverse() * g1.inverse() * w * g1;
    
    GAEquationSolver solver(G,numOfVars,c);
    timeval t1,t2;
    gettimeofday(&t1, NULL);
    int gs;
    Map solution = solver.getSolution(com,&cout,gs);
    gettimeofday(&t2, NULL);
    t[k] = diffTime(t2,t1);
    g[k] = gs;

    cout << "The solution is " << solution << endl << endl;
    cout << "Experiment number " << k+1 << ":" << endl; 
    //cout << "Time1: " << t1.tv_sec << "," << t1.tv_usec << endl;
    //cout << "Time2: " << t2.tv_sec << "," << t2.tv_usec << endl;
    cout << "Number of generations: " << gs << endl;
    cout << "Time: " << t[k] << endl << endl;
    int sLen = solution.generatingImages(0).freelyReduce().length();
    l[k] = sLen;
    cout << "Length of the solution: " << sLen << endl << endl;

    /*
    cout << "Checking:  Length ... " << sLen ;
    if( sLen == len*3 )
      cout << " Correct" << endl << endl;
    else {
      cout << " Incorrect" << endl;
    }
    */
  }
  
  cout << endl << endl << "Length: " << len*3 << endl;
  double avg = 0, aG = 0;
  for( int k = 0; k < numOfExp; ++k ) {
    cout << "Generations: " << g[k] << "    Time: " << t[k] 
	 << "    Length: " << l[k] << endl;
    avg += t[k];
    aG += g[k];
  }
  avg = avg / numOfExp;
  aG = aG / numOfExp;
  cout << "Average number of generatons: " << aG << endl;
  cout << "Average time: " << avg << endl;
  cout << "Average time per generation: " << avg/aG << endl;
}

