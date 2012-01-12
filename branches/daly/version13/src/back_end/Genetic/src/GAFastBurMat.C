// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of classes BurauPoly, GAFastBurMat
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "GAFastBurMat.h"
#include "values.h"


// ----------------------------- BurauPoly --------------------------------- //


BurauPoly::BurauPoly( int coef ) 
  : n( 0 )
{ 
  c = new int[1];
  c[0] = coef;
}

BurauPoly::BurauPoly( int pn, int* pc )
{
  n = pn;
  int k = 2*n+1;
  c = new int[k];
  memcpy(c, pc, sizeof(int) * k); 
}

BurauPoly::BurauPoly( const BurauPoly& p )
{
  n = p.n;
  int k = 2*n+1;
  c = new int[k];
  memcpy(c, p.c, sizeof(int) * k);
}

BurauPoly::~BurauPoly( )
{
  delete [] c;
}

BurauPoly& BurauPoly::operator = ( const BurauPoly& p )
{
  delete [] c;
  n = p.n;
  int k = 2*n+1;
  c = new int[k];
  memcpy(c, p.c, sizeof(int) * k);
  return *this;
}

void BurauPoly::expandTo( int pn )
{
  int dif = abs(pn-n);
  int k = 2 * pn + 1;
  if( pn > n ) {
    int* t = new int[k];
    memset( t, 0, dif * sizeof(int) );
    memcpy( t + dif, c, (2*n+1) * sizeof(int) );
    memset( t+dif+(2*n+1), 0, dif * sizeof(int) );
    delete [] c;
    n = pn;
    c = t;
  }
}

void BurauPoly::shrink( ) 
{
  int k = 2*n+1;
  int i;
  for( i = 0; i < n; ++i )
    if( c[i] != 0 || c[k-1-i] != 0 )
      break;
  if( i == 0 ) 
    return;
  k = 2*(n-i)+1;
  int *t = new int[k];
  memcpy(t,c+i,k*sizeof(int));
  delete [] c;
  c = t;
  n -= i;
}

void BurauPoly::add( const BurauPoly& p )
{
  if( p.zero() ) return;
  expandTo(p.n);
  int dif = n - p.n;
  int k = 2 * p.n + 1;
  for( int i = 0; i < k; ++i )
    c[dif+i] += p.c[i];
  shrink();
}

void BurauPoly::sub( const BurauPoly& p )
{
  if( p.zero() ) return;
  expandTo(p.n);
  int dif = n - p.n;
  int k = 2 * p.n + 1;
  for( int i = 0; i < k; ++i )
    c[dif+i] -= p.c[i];
  shrink();
}

void BurauPoly::multByX( )
{
  if( zero() ) return;
  int k = 2 * n + 1;
  int *t = new int[k+2];
  memcpy( t, c, k * sizeof(int) );
  t[k] = 0;
  t[k+1] = 0;
  delete [] c;
  c = t;
  ++n;
  shrink();
}

void BurauPoly::multByIX( )
{
  if( zero() ) return;
  int k = 2 * n + 1;
  int *t = new int[k+2];
  memcpy( t+2, c, k * sizeof(int) );
  t[0] = 0;
  t[1] = 0;
  delete [] c;
  c = t;
  ++n; 
  shrink();
}

void BurauPoly::printOn( ostream& ostr ) const
{
  if( n == 0 ) {
    ostr << " " << c[0];
    return;
  }

  int j = 0;
  for( int i = 0; i < n; ++i ) {
    if( c[j] != 0 ) {
      ostr << " ";
      if( c[j] > 0 ) ostr << "+"; else ostr << "-";
      if( abs(c[j]) != 1 ) ostr << abs(c[j]);
      ostr << " x^" << n-i;  
    }
    ++j;
  }

  if( c[j] != 0 ) {
    ostr << " ";
    if( c[j] > 0 ) 
      ostr << "+";
    ostr << c[j];  
  }
  ++j;

  for( int i = 0; i < n; ++i ) {
    if( c[j] != 0 ) {
      ostr << " ";
      if( c[j] > 0 ) ostr << "+"; else ostr << "-";
      if( abs(c[j]) != 1 ) ostr << abs(c[j]);
      ostr << " x^-" << i+1;  
    }
    ++j;
  }
}

void BurauPoly::debugPrint( ostream& ostr ) const
{
  int k = 2*n+1;
  for( int i = 0; i < k; ++i )
    ostr << " " << c[i];
  ostr << endl;
}


// --------------------------- GAFastBurMat ------------------------------- //
  

GAFastBurMat::GAFastBurMat( const GAConfig& GAC, const FreeGroup& Br )
  : GA( GAC ),
    B( Br ),
    numOfGens( Br.numberOfGenerators() ),
    n( Br.numberOfGenerators() + 1 ),
    out( 0 ),
    PMDWC(r,numOfGens,52,Br,false)
{ }


GAFastBurMat::~GAFastBurMat( )
{ }


void GAFastBurMat::initPopulation( )
{
  PMDebornoyWord u(PMDWC);
  for( int i = 0; i < popSize; ++i )
    pop[i] = u.randomPM();
}

void GAFastBurMat::destroyMatrix( BurauPoly** R )
{
  for( int i = 0; i < n; ++i )
    delete [] R[i];
  delete [] R;
}

void GAFastBurMat::printMatrix( BurauPoly** R )
{
  for( int i = 0; i < n; ++i ) {
    cout << "( ";
    for( int j = 0; j < n; ++j ) {
      cout << R[i][j];
      if( j != n-1 )
	cout << ", ";
    }
    cout << " )" << endl << endl;
  }
}

BurauPoly** GAFastBurMat::productMatrix( const PM* pm )
{
  Word u = ((PMDebornoyWord*)pm)->getPMWord().getWord();
  BurauPoly **R = new BurauPoly*[n];
  for( int i = 0; i < n; ++i ) {
    R[i] = new BurauPoly[n];
    R[i][i] = 1;
  }

  int uLen = u.length();
  for( int i = 0; i < uLen; ++i ) {
    int g = ord(u[i]);
    if( g > 0 ) {

      // R = R * bgens[g-1];
      for( int k = 0; k < n; ++k ) {
	BurauPoly t = R[k][g-1];
	t.multByX();
	R[k][g-1].sub(t);
	R[k][g-1].add(R[k][g]);
	R[k][g] = t;
      }
    }
    else {

      // R = R * bgensI[-g-1];
      for( int k = 0; k < n; ++k ) {
	BurauPoly t = R[k][-g];
	t.multByIX();
	R[k][-g].sub(t);
	R[k][-g].add(R[k][-g-1]);
	R[k][-g-1] = t;
      }
    }
  }
  return R;
}


int GAFastBurMat::fitness( const PM* pm )
{
  /* 
  // The fitness is the number of non zeros beyound the main diagonal

  BurauPoly **R = productMatrix(pm);
  int count = 0;
  for( int k = 0; k < n; ++k )
    for( int l = 0; l < n; ++l )
      if( k != l && !R[k][l].zero() )
	++count;
  
  destroyMatrix(R);
  return count; 
  */

  // The fitness is the average power of non-diagonal elements + 1
  // If an element is equal to 0 then the fitness is 0

  BurauPoly **R = productMatrix(pm);
  int count = 0;
  for( int k = 0; k < n; ++k )
    for( int l = 0; l < n; ++l )
      if( k != l )
	if( !R[k][l].zero() )
	  count += R[k][l].degree() + 1;
  
  destroyMatrix(R);
  //count = count / (n*n-n);
  return count; 

}


void GAFastBurMat::penalizeFitness( )
{
  minFit = MAXINT; maxFit = 0; minFitInd = -1;
  const int threshhold = 52;
  //@@DSP int DSP[popSize];
  int P6[popSize], OP[popSize], RP[popSize], DP[popSize];

  for( int i = 0; i < popSize; ++i ) {
    Word u = ((PMDebornoyWord*)pop[i])->getPMWord().getWord();
    int uLen = u.length();
    if( uLen < threshhold ) fit[i] += threshhold-uLen;
    
    /*@@DSP
    DSP[i] = 0;
    for( int j = 0; j < uLen; ++j )  
      if( ord(u[j]) > 0 ) 
	++DSP[i];
      else
	--DSP[i];
    DSP[i] = abs(DSP[i]);
    fit[i] += DSP[i];
    */
    P6[i] = 0;
    for( int j = 0; j < uLen-2; ++j ) {
      int v1 = ord(u[j]);
      int v2 = ord(u[j+1]);
      int v3 = ord(u[j+2]);
      if( v1 == v3 && sign(v1) == sign(v2) ) 

	/* penalize cancellations in relations of length 6
 
	if( abs(v1)+1 == abs(v2) || abs(v1) == abs(v2)+1 ) {
	  if( j > 0 && ord(u[j-1]) == -v2 )
	    ++P6[i];
	  if( j < uLen-3 && ord(u[j+3]) == -v2 )
	    ++P6[i];
	}
	*/

	// penalize a mere occurence of relation of length 6
	++P6[i];
    }

    OP[i] = 0;
    for( int j = 0; j < uLen-1; ++j ) {
      int a1 = abs(ord(u[j]));
      int a2 = abs(ord(u[j+1]));
      if( abs(a1-a2) <= 1 ) ;
      else if( a1 < a2 )
	OP[i] += 1;
      else
	OP[i] += 2;
    }
    //if( OP[i] <= 4 ) OP[i] = 0;

    RP[i] = 0;
    int k;
    for( k = 0; k < uLen-4; ++k ) {

      if( ord(u[k]) == ord(u[k+1]) && ord(u[k]) == ord(u[k+2])
	  && ord(u[k]) == ord(u[k+3]) && ord(u[k]) == ord(u[k+4]) ) {
	k += 4;
	++RP[i];
      }
    }
    
    DP[i] = numOfGens;
    int gens[numOfGens];
    for( int j = 0; j < numOfGens; ++j ) 
      gens[j] = 0;

    for( int j = 0; j < uLen-1; ++j ) {
      int a = abs(ord(u[j]));
      gens[a-1] = 1;
    }
    for( int j = 0; j < numOfGens; ++j ) 
      DP[i] -= gens[j];
    DP[i] *= 5;

    fit[i] += 3 * (P6[i] + OP[i] + RP[i] + DP[i]);


    if( fit[i] < minFit ) {
      minFit = fit[i];
      minFitInd = i;
    }
    if( fit[i] > maxFit )
      maxFit = fit[i];
  }
  if( out && g % 10 == 0) {
    *out << "Generation: " << g << "   Penalized fitness: " << minFit << endl;
  }
  if( g % 100 == 0 ) {
    cout << "Penalized fitness values: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << fit[i];
    cout << endl << endl;

    /*@@DSP
    cout << "Derived sugroup penalties: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << DSP[i];
    cout << endl << endl;
    */

    cout << "Relations of length 6 penalties: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << P6[i];
    cout << endl << endl;
 
    cout << "Order penalties: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << OP[i];
    cout << endl << endl;

    cout << "Repetition penalties: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << RP[i];
    cout << endl << endl;

    cout << "Generator diversity penalties: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << DP[i];
    cout << endl << endl;
  }
}

void GAFastBurMat::init( )
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


bool GAFastBurMat::checkForSolution( Word& res )
{
  // print current results
  if( out && g % 10 == 0) {
    *out << "Generation: " << g << "   Fitness: " << minFit << endl;
  }
  if( minFit == 0 || g % 100 == 0 ) 
    cout << endl << "pop[minFitInd] : " << endl << *pop[minFitInd] 
	 << endl << endl;

  if( g % 500 == 0 ) {
    
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
/*
Word initWord(const Generator* p, int len) {
  VectorOf<Generator> v(len);
  for( int i = 0; i < len; i ++ ) v[i] = *p++;
  return Word(v);
}
*/
Word GAFastBurMat::getSolution( ostream* o )
{
  /*
  const Generator gl3[] = { 1, -2, 1, -2, -2, 1, 1, -2, -2, -5, 1, 1, 2, -4, 5, 1, 2, 1, -2, 4, 5, 1, 2, 1, -2, 4, -2, 1, -4, -2, 1, 1, 2, 1, -2, -4, 5, 4, 1, -2, 1, -4, -2, 1, 4, 1, 5, 4, -2, 1, -4, -2, -2, -4, 5, 2, 1, -2, -2, -2, -5, -3, 1, -5, 3, 1, -4, -2, 1, 1, 1, -2 };
  
  Word r = initWord(gl3, 72);
  */
  /*
  Chars errMsg;
  Word r = B.readWord(cin,errMsg);
  
  PMDebornoyWord pm(PMWDC,r);
  cout << "Matrix: " << endl;
  printMatrix( productMatrix( &pm ) ); 
  cout << endl;
  cout << "Fitness = " << fitness(&pm) << endl;
  return r;
  */
  

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

