// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of class GANagata
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "GANagata.h"
#include "values.h"


// -------------------------- GANagataConfig ------------------------------- //


GANagataConfig::GANagataConfig( int popSize, int numOfGens, int crossChance, 
				int mutChance, ElitistSelectionType elitType, 
				bool fitScaling, int Report, int FitnessPrint,
				int PopulationPrint, int NumberOfVariables,
				Polynomial<Int2> aPolynomial, 
				int MaximalCoefficient, int MaximalPower,
				int MaximalNumberOfMonomials )
  : GAConfig( popSize, numOfGens, crossChance, mutChance, elitType, 
	      fitScaling, Report, FitnessPrint, PopulationPrint ),
    theNumberOfVariables( NumberOfVariables ),
    thePolynomial( aPolynomial ),
    theMaximalCoefficient( MaximalCoefficient ),
    theMaximalPower( MaximalPower ),
    theMaximalNumberOfMonomials( MaximalNumberOfMonomials )
{ }


bool GANagataConfig::setUserVariable( Chars varName, Chars value )
{
  istrstream in(value);
  int iValue;

  if( varName == Chars("NumberOfVariables") ) {
    in >> iValue;
    theNumberOfVariables = iValue;
  }

  else if( varName == Chars("Polynomial") ) {
    
    //cout << "Reading poly: " << value << endl;

    Polynomial<Int2> p;
    in >> p;
    thePolynomial = p;
  }

  else if( varName == Chars("MaximalCoefficient") ) {
    in >> iValue;
    theMaximalCoefficient = iValue;
  }

  else if( varName == Chars("MaximalPower") ) {
    in >> iValue;
    theMaximalPower = iValue;
  }

  else if( varName == Chars("MaximalNumberOfMonomials") ) {
    in >> iValue;
    theMaximalNumberOfMonomials = iValue;
  }
  
  else 
    return false;
  
  return true;
}
 

void GANagataConfig::printOn( ostream& ostr ) const
{
  GAConfig::printOn(ostr);

  ostr <<  "# Number of variables" << endl
       << "NumberOfVariables : " << theNumberOfVariables << endl << endl
       << "# Polynomial followed by comma" << endl
       << "Polynomial : " << thePolynomial << endl << endl
       << "*** Generating elementary auto: ***" << endl << endl
       << "# Maximal coefficient" << endl
       << "MaximalCoefficient : " << theMaximalCoefficient << endl << endl
       << "# Maximal Power" << endl
       << "MaximalPower : " << theMaximalPower << endl << endl
       << "# Maximal number of monomials" << endl
       << "MaximalNumberOfMonomials : " << theMaximalNumberOfMonomials 
       << endl << endl;
}


// ------------------------------ GANagata --------------------------------- //
  

GANagata::GANagata( const GANagataConfig& GAC )
  : GA( GAC ),
    out( 0 ),
    PMPC(r,
	 GAC.maximalCoefficient(),
	 GAC.maximalPower(),
	 GAC.numberOfVariables(),
	 GAC.maximalNumberOfMonomials())
{ }


GANagata::~GANagata( )
{ }


void GANagata::initPopulation( )
{
  
  Polynomial<Int2> r1(((const GANagataConfig&)gac).polynomial() );
  PMPolynomial p(PMPC,r1);

  for( int i = 0; i < popSize; ++i )
    pop[i] = p.clone();
}


int GANagata::fitness( const PM* pm )
{
  // The fitness is the highest degree minus one

  Polynomial<Int2> p = ((PMPolynomial*)pm)->getPolynomial();
  PolynomialIterator<Int2> I(p);
  int degree = 0;
  for( ; !I.done(); I.next() ) {
    Monomial<Int2> M = I.getValue();
    int s = 0;
    for( int i = 0; i < M.getNumberOfVariables(); ++i )
      s += abs(M.getPowerOfVariable(i));
    if( s > degree )
      degree = s;
  }
  
  return (degree-1);
}


void GANagata::init( )
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


bool GANagata::checkForSolution( Polynomial<Int2>& res )
{
  // print current results

  if( out && g % gac.report() == 0) {
    *out << "Generation: " << g << "   Fitness: " << minFit << endl;
  }

  if( minFit == 0 || g % gac.report() == 0 ) 
    cout << endl << "pop[minFitInd] : " << endl << *pop[minFitInd] 
	 << endl << endl;

  if( g % gac.fitnessPrint() == 0 ) {
    cout << "Fitness values: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << " " << fit[i];
    cout << endl << endl;
  }

  if( g % gac.populationPrint() == 0 ) {
    cout << "Population: " << endl;
    for( int i = 0; i < popSize; ++i )
      cout << i << ": " << endl << *pop[i] << endl;
  }
  
  // exit if found a solution

  if( minFit == 0 ) {
   // prepare and return found solution
    res = ((PMPolynomial*)pop[minFitInd])->getPolynomial();
    if( out ) {
      *out << "Solution: " << res;
      cout << endl << end;
    }
    return true;
  }
  return false;
}


Polynomial<Int2> GANagata::getSolution( ostream* o )
{
  out = o;
  Polynomial<Int2> res;
  init();
  initPopulation();
  int numOfGenerations = gac.numOfGenerations();
  for( g = 0; numOfGenerations < 0 || g < numOfGenerations; ++g ) {
    computeFitness();
    if( checkForSolution(res) ) return res;
    adjustFitness();
    doCrossover();
    doMutation();
    doReplacement();
  }
}

