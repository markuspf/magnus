// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of class GA
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "values.h"
#include "GA.h"
#include "Roulette.h"


// ---------------------------------- GA ----------------------------------- //


GA::GA( const GAConfig& GAC ) 
  : gac( GAC ),
    popSize( GAC.populationSize() ),
    bFirstGen( true )
{ 
  pop = new PM*[popSize];
  for( int i = 0; i < popSize; ++i )
    pop[i] = 0;
  fit = new int[popSize];
  copyFit = new int[popSize];
  newPop = new PM*[popSize];
}


GA::~GA( )
{
  delete [] newPop;
  delete [] fit;
  delete [] copyFit;
  for( int i = 0; i < popSize; ++i )
    delete pop[i];
  delete [] pop;
}


/* sample of run()

  initPopulation();
  int numOfGens = config.numberOfGenerations();
  for( int g = 0; numOfGens < 0 || g < numOfGens; ++g ) {
    computeFitness();
    adjustFitness();
    doCrossover();
    doMutation();
    doReplacement();
  }

*/

void GA::computeFitness()
{ 
  minFit = MAXINT; maxFit = 0; minFitInd = -1;

  for( int i = 0; i < popSize; ++i ) {
    fit[i] = fitness(pop[i]);
      
    if( fit[i] < minFit ) {
      minFit = fit[i];
      minFitInd = i;
    }
    if( fit[i] > maxFit )
      maxFit = fit[i];
  }
}


void GA::adjustFitness()
{
  memcpy( copyFit, fit, popSize * sizeof(int) );

  // make fitness values suitable for Roulette wheel selection
  int base = maxFit + 1;
  for( int i = 0; i < popSize; ++i )
    fit[i] = base - fit[i];
  
  // fitness scaling
  if( gac.haveFitnessScaling() )
    for( int i = 0; i < popSize; ++i )
      fit[i] = fit[i] * fit[i];
}


void GA::doCrossover( )
{
  RouletteWheel<int> wheel(popSize,fit);
  double crossRate = gac.chanceOfCrossover();

  for( int i = 0; i < popSize; ++i )
    if( r.rand() <= crossRate ) {
      int i1 = wheel.GetIndex();
      int i2 = wheel.GetIndex();
      newPop[i] = pop[i1]->crossover(pop[i2]);
    }
    else
      newPop[i] = pop[i]->clone();
}


void GA::doMutation( )
{
  double mutRate = gac.chanceOfMutation();
  for( int i = 0; i < popSize; ++i )
    if( r.rand() <= mutRate ) {
      PM* pm = newPop[i]->mutate();
      delete newPop[i];
      newPop[i] = pm;
    }
}


void GA::doReplacement( )
{
  // elitist selection
  if( gac.elitistSelectionType() == GAConfig::SIMPLE ) {
    delete newPop[0];
    newPop[0] = pop[minFitInd];
    pop[minFitInd] = 0;
  }

  // strong elitist selection
  else if( gac.elitistSelectionType() == GAConfig::STRONG )
    for( int c = 0; c < 5; ++c ) {
      int m = MAXINT, mind = -1;
      for( int i = 0; i < popSize; ++i ) {
	if( copyFit[i] <= m ) {
	  m = copyFit[i];
	  mind = i;
	}
      }
      delete newPop[c];
      newPop[c] = pop[mind];
      pop[mind] = 0;
      copyFit[mind] = MAXINT;
    }
  
  // prepare for the next iteration
  for( int i = 0; i < popSize; ++i ) {
    delete pop[i];
    pop[i] = newPop[i];
  }    
}



