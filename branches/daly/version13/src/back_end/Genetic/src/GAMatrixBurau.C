// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of GAMatrixBurau class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "GAMatrixBurau.h"
#include "values.h"


// --------------------------- GAMatrixBurau ------------------------------- //
  

GAMatrixBurau::GAMatrixBurau( const GAConfig& GAC, const FreeGroup& Br )
  : GA( GAC ),
    B( Br ),
    numOfGens( Br.numberOfGenerators() ),
    n( Br.numberOfGenerators() + 1 ),
    out( 0 ),
    bgens( 0 ),
    bgensI( 0 ),
    PMDWC(r,numOfGens,44,Br,false)
{ 
  I = Matrix< Polynomial<Int2> >(n);
  for( int k = 0; k < n; ++k )
    for( int l = 0; l < n; ++l )
      if( k != l ) 
	I[k][l] = "0,";
      else
	I[k][l] = "1,";

  // Burau representation
  {
    bgens = new Matrix< Polynomial<Int2> >[n];
    for( int i = 0; i < numOfGens; ++i ) {
      bgens[i] = I;
      bgens[i][i][i] = "1 - x1,";
      bgens[i][i][i+1] = "x1,";
      bgens[i][i+1][i] = "1,";
      bgens[i][i+1][i+1] = "0,";
      
      //cout << "bgens[" << i << "] = " << endl << bgens[i] << endl;
    }
    
    bgensI = new Matrix< Polynomial<Int2> >[n];
    for( int i = 0; i < numOfGens; ++i ) {
      bgensI[i] = I;
      bgensI[i][i][i] = "0,";
      bgensI[i][i][i+1] = "1,";
      bgensI[i][i+1][i] = "x1^-1,";
      bgensI[i][i+1][i+1] = "1 - x1^-1,";
    }
  }
  
  /*
  // Twisted Burau representation

  // odd generators:
  {
    bgens = new Matrix< Polynomial<Int2> >[n];
    for( int i = 0; i < numOfGens; i+=2 ) {
      bgens[i] = I;
      bgens[i][i][i] = "1 + x1,";
      bgens[i][i][i+1] = "x1^2,";
      bgens[i][i+1][i] = "-1,";
      bgens[i][i+1][i+1] = "1 - x1,";
      
      //cout << "bgens[" << i << "] = " << endl << bgens[i] << endl;
    }
    
    bgensI = new Matrix< Polynomial<Int2> >[n];
    for( int i = 0; i < numOfGens; i+=2 ) {
      bgensI[i] = I;
      bgensI[i][i][i] = "1 - x1,";
      bgensI[i][i][i+1] = "-x1^2,";
      bgensI[i][i+1][i] = "1,";
      bgensI[i][i+1][i+1] = "1 + x1,";
    }
  }
  
  // even generators:
  {
    for( int i = 1; i < numOfGens; i+=2 ) {
      bgens[i] = I;
      bgens[i][i][i] = "1 + x1^-1,";
      bgens[i][i][i+1] = "x1^-2,";
      bgens[i][i+1][i] = "-1,";
      bgens[i][i+1][i+1] = "1 - x1^-1,";
      
      //cout << "bgens[" << i << "] = " << endl << bgens[i] << endl;
    }
    
    for( int i = 1; i < numOfGens; i+=2 ) {
      bgensI[i] = I;
      bgensI[i][i][i] = "1 - x1^-1,";
      bgensI[i][i][i+1] = "-x1^-2,";
      bgensI[i][i+1][i] = "1,";
      bgensI[i][i+1][i+1] = "1 + x1^-1,";
    }
  }
  */
}


GAMatrixBurau::~GAMatrixBurau( )
{
  delete [] bgens;
  delete [] bgensI;
}


void GAMatrixBurau::initPopulation( )
{
  PMDebornoyWord u(PMDWC);
  for( int i = 0; i < popSize; ++i )
    pop[i] = u.randomPM();
}


Matrix< Polynomial<Int2> > GAMatrixBurau::productMatrix( const PM* pm )
{
  Word u = ((PMDebornoyWord*)pm)->getPMWord().getWord();
  Matrix< Polynomial<Int2> > R(I);
  int uLen = u.length();
  for( int i = 0; i < uLen; ++i ) {
    int g = ord(u[i]);
    if( g > 0 ) {
      /*
      cout << "g = " << g << endl << flush; 
      cout << "R = " << R << endl << flush; 
      cout << "bgens[g-1] = " << bgens[g-1] << endl; 
      */
      R = R * bgens[g-1];
    }
    else {
      /*
      cout << "g = " << g << endl; 
      cout << "R = " << R << endl; 
      cout << "bgensI[-g-1] = " << bgensI[-g-1] << endl; 
      */
      R = R * bgensI[-g-1];
    }
  }
  return R;
}


int GAMatrixBurau::fitness( const PM* pm )
{
  Matrix< Polynomial<Int2> > R = productMatrix(pm);
  int count = 0;
  for( int k = 0; k < n; ++k )
    for( int l = 0; l < n; ++l )
      if( k != l && R[k][l] != Polynomial<Int2>("0,") )
	++count;
  return count; 
}


void GAMatrixBurau::penalizeFitness( )
{
  minFit = MAXINT; maxFit = 0; minFitInd = -1;
  const int threshhold = 44;

  for( int i = 0; i < popSize; ++i ) {
    Word u = ((PMDebornoyWord*)pop[i])->getPMWord().getWord();
    int uLen = u.length();
    if( uLen < threshhold ) fit[i] += threshhold-uLen;
    if( fit[i] < minFit ) {
      minFit = fit[i];
      minFitInd = i;
    }
    if( fit[i] > maxFit )
      maxFit = fit[i];
  }
  if( out && g % 1 == 0) {
    *out << "Generation: " << g << "   Penalized fitness: " << minFit << endl;
  }
  if( g % 10 == 0 ) {
    cout << "Penalized fitness values: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << fit[i];
    cout << endl << endl;
  }
}

void GAMatrixBurau::init( )
{
  /*
  if( out != 0 ) {
    (*out) << "The algorithm tries to substitute variables with genetically "
      "produced words to reduce the length of an equation. "
      "The fitness value below is the lowest length of the equation produced "
      "on the current generation. " << endl << endl << ready;
  }
  */
}


bool GAMatrixBurau::checkForSolution( Word& res )
{
  // print current results
  if( out && g % 1 == 0) {
    *out << "Generation: " << g << "   Fitness: " << minFit << endl;
  }
  if( minFit == 0 || g % 10 == 0 ) 
    cout << "pop[minFitInd] : " << endl << *pop[minFitInd] << endl;

  if( g % 10 == 0 ) {
    
    cout << "Fitness values: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << fit[i];
    cout << endl << endl;
    cout << "Population: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << i << ": " << endl << *pop[i] << endl;
  }
  // exit if found a solution
  if( minFit == 0 ) {
   // prepare and return found solution
    res = ((PMDebornoyWord*)pop[minFitInd])->getPMWord().getWord();
    if( out ) {
      *out << "Solution: ";
      B.printWord(*out,res);
      cout << endl << end;
    }
    return true;
  }
  return false;
}

Word initWord(const Generator* p, int len) {
  VectorOf<Generator> v(len);
  for( int i = 0; i < len; i ++ ) v[i] = *p++;
  return Word(v);
}

Word GAMatrixBurau::getSolution( ostream* o )
{
  /*
  const Generator gl3[] = { 1, -2, 1, -2, -2, 1, 1, -2, -2, -5, 1, 1, 2, -4, 5, 1, 2, 1, -2, 4, 5, 1, 2, 1, -2, 4, -2, 1, -4, -2, 1, 1, 2, 1, -2, -4, 5, 4, 1, -2, 1, -4, -2, 1, 4, 1, 5, 4, -2, 1, -4, -2, -2, -4, 5, 2, 1, -2, -2, -2, -5, -3, 1, -5, 3, 1, -4, -2, 1, 1, 1, -2 };
  
  Word r = initWord(gl3, 72);
  */

  Chars errMsg;
  Word r = B.readWord(cin,errMsg);
  
  PMDebornoyWord pm(PMDWC,r);
  cout << "Matrix: " << endl << productMatrix( &pm ) << endl;
  cout << "Fitness = " << fitness(&pm) << endl;
  return r;

  

  out = o;
  Word res;
  init();
  initPopulation();
  int numOfGenerations = gac.numOfGenerations();
  for( g = 0; numOfGenerations < 0 || g < numOfGenerations; ++g ) {
    computeFitness();
    if( checkForSolution(res) ) return res;

    penalizeFitness();

    adjustFitness();
    doCrossover();
    doMutation();
    doReplacement();
  }
}

