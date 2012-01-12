// Copyright (C) 1997-1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of GeneticGraphWP class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//
//


#include "stdlib.h"
#include "values.h"
#include "GeneticGraphWP.h"
#include "Roulette.h"
#include "File.h"
#include "Integer.h"
#include "Set.h"


//#define debug_GeneticGraphWP


// ------------------------------- NCChunk --------------------------------- //


void NCChunkRep::deleteVertex( VertexType v)
{
  for( LabelType i = 0; i < valence; ++i ) {
    VertexType u = targetOfLabel(v,i);
    if( u != emptyTarget ) {
      int gen = labelToGenerator(i);
      table[ u * valence + generatorToLabel(-gen) ] = emptyTarget;
      table[ v * valence + i ] = emptyTarget;
    }
  }
}

bool NCChunkRep::isGarbage( VertexType v)
{
  for( LabelType i = 0; i < valence; ++i ) 
    if( targetOfLabel(v,i) != emptyTarget ) 
      return false;
  return true;
}

int NCChunkRep::power( VertexType v)
{
  int pow = 0;
  for( LabelType i = 0; i < valence; ++i ) 
    if( targetOfLabel(v,i) != emptyTarget ) 
      ++pow;
  return pow;
}

void NCChunkRep::moveVertex( VertexType source, VertexType target)
{
  for( LabelType i = 0; i < valence; ++i ) {
    VertexType u = targetOfLabel(source,i);
    if( u != emptyTarget ) {
      int gen = labelToGenerator(i);
      table[ u * valence + generatorToLabel(-gen) ] = target;
      table[ target * valence + i ] = u;
    }
    table[ source * valence + i ] = emptyTarget;
  }
}

void NCChunkRep::collectGarbage( )
{
  VertexType i,j = numberOfVertices-1;
  for( i = 0; i < numberOfVertices; ++i ) {

    bool bVertexFound = false;
    if( isGarbage(i) ) {

      while( j >0 ) {
	if( j <= i ) break;
	if( !isGarbage(j) ) { bVertexFound = true; break; }
	--j;
      }

      if( bVertexFound ) 
	moveVertex(j,i);
      else break;
    }
  }
  
  numberOfVertices = i;
  resize(i);
}

// used by qsort in updateDistanceTable()

struct SortVertex { long vertex,distance; }; 

int compareVertices(const void *p1, const void *p2)
{
  long d1 = ((SortVertex*)p1)->distance;
  long d2 = ((SortVertex*)p2)->distance;
  if( d1 < d2 ) 
    return -1;
  else if( d1 > d2 ) 
    return 1;
  else 
    return 0;
}

void NCChunkRep::updateDistanceTable( )
{
  if( numberOfVertices == 0 ) return;
  SetOf<Integer> S;
  S |= Integer(0);
  delete [] distance;
  distance = new long[numberOfVertices];
  for( int i = 1; i < numberOfVertices; ++i ) distance[i] = -1;
  distance[0] = 0;
  int dist = 0;

  while( S.cardinality() != 0 ) {
    
    ++dist;
    SetOf<Integer> S2;
    SetIterator<Integer> I(S);
    for( ; !I.done(); I.next() ) {
      VertexType v = I.value().as_long();
      for( LabelType i = 0; i < valence; ++i ) {
	VertexType t = targetOfLabel(v,i);
	if( t != emptyTarget && distance[t] == -1 ) {
	  distance[t] = dist;
	  S2 |= Integer(t);
	}
      }
    }
    S = S2;
  }
#ifdef debug_GeneticGraphWP
  cout << "Distances: " << endl;
  for( int i = 0; i < numberOfVertices; ++i ) 
    cout << i << ": " << distance[i] << endl;
  cout << endl;
#endif

}

void NCChunkRep::reduce( const Word& w)
{
  const int maxGraphSize = 1000;
  int toDelete = numberOfVertices - maxGraphSize;
  if( toDelete <= 0 ) return;

  updateDistanceTable();
  SortVertex sortVertex[numberOfVertices];
  for( int i = 0; i < numberOfVertices; ++i ) { 
    sortVertex[i].vertex = i; 
    sortVertex[i].distance = distance[i]; 
  }
  qsort(sortVertex, numberOfVertices, sizeof(SortVertex), &compareVertices);

#ifdef debug_GeneticGraphWP
  cout << "Sorted vertices: " << endl;
  for( VertexType i = 0; i < numberOfVertices; ++i ) 
    cout << sortVertex[i].vertex << endl;
  cout << endl;
#endif
  
  SetOf<Integer> wordVertices = getWordVertices(w);
  VertexType i;
  for( i = numberOfVertices-1; i > 0; ++i ) {
    
    if(toDelete <= 0 ) break;
    VertexType v = sortVertex[i].vertex;
    int pow = power(v);

  #ifdef debug_GeneticGraphWP
    if( pow == 1 )
      warn("Have a vertex of power 1 in the graph.");
  #endif

    if( pow == 2 && !wordVertices.contains(Integer(v)) ) {
      VertexType t[2];
      int j = 0;

      for( LabelType l = 0; l < valence; ++l ) { 
	VertexType target = targetOfLabel(v,l);
	if( target != emptyTarget ) 
	  t[j++] = target;
      }
      
      deleteVertex(v);
      --toDelete;

      for( j = 0; j < 2; ++j ) { 
	v = t[j];
	while( power(v) <= 1 ) 
	  for( LabelType l = 0; l < valence; ++l ) { 
	    VertexType target = targetOfLabel(v,l);
	    if( target != emptyTarget ) {
	      deleteVertex(v);
	      --toDelete;
	      v = target;
	      break;
	    }
	  }
      }
    }

  }

#ifdef debug_GeneticGraphWP
  if( i == 0 )
    warn("The process failed to reduce the graph");
#endif

  collectGarbage();
}

long NCChunkRep::getDistance( VertexType v ) const
{
#if SAFETY > 0
  if( distance == 0 || v >= numberOfVertices || v < 0 )
    error("void NCChunkRep::getDistance( VertexType v ) : "
	  "No data for v is available");
#endif
 
    return distance[v];
}

SetOf<Integer> NCChunkRep::getWordVertices( const Word& u) const
{
  VertexType target = 0;
  int i = 0;
  int uLen = u.length();
  SetOf<Integer> S;
  S |= Integer(target);
  while( i < uLen-1 ) {
    target = targetOfGenerator(target, ord(u[i++]));
    S |= Integer(target);
  }
  return S;
}

NCChunkRep* NCChunkRep::join(const NCChunkRep& C) const
{ 
  return new NCChunkRep(*SubgroupGraphRep::join(C));
}


// --------------------------- GeneticGraphWP ------------------------------ //


GeneticGraphWP::GeneticGraphWP( const FPGroup& G, const GHNConfig& config ) 
  : theGroup( G.namesOfGenerators() ), 
    cfg( config ), 
    relators( G.getRelators() )
{ }

Trichotomy GeneticGraphWP::isTrivial( const Word& u, ostream* out )
{
 
  //  if( out != 0 ) {
  //  (*out) << "The algorithm tries to reduce length of the word genetically. "
  //    "The fitness value below is the lowest length produced on the "
  //    "current generation. " << endl << endl << ready;
  //}
 
  w = u;
  wLen = w.length();
  int popSize = cfg.populationSize();
  NCChunk pop[popSize];
  NCChunk newPop[popSize];
  int fit[popSize];

  // create the original population
  SetOf<Word> S(relators);
  SetIterator<Word> I(S);
  S |= (w * I.value() * w.inverse()).freelyReduce();
  NCChunk SG(theGroup.rank(), S );

  for( int k = 0; k < popSize; ++k ) {
    pop[k] = SG;
    //newPop[k] = SG;
  }
  automataSize = max(100, 5 * SG.vertexCount());
  
  // the main loop

  int numOfGens = cfg.numOfGenerations();
  float crossRate = cfg.chanceOfCrossover();
  float mutRate = cfg.chanceOfMutation();
  int max, min, minInd, g;
 
  for( g = 0; g < numOfGens || numOfGens == -1; ++g ) {
    
    min = MAXINT; max = -MAXINT;  minInd = -1;
    
    // compute fitness values
    for( int x = 0; x < popSize; ++x ) {
	
      //pop[x].debugPrint(cout);
      fit[x] = fitness(pop[x]);
           
      if( fit[x] < min ) {
	min = fit[x];
	minInd = x;
      }
      if( fit[x] > max ) {
	max = fit[x];
      }
    }
    
    // print current results
    if( g < 100 || (g <1000 && g % 10 == 0) || ( g % 100 == 0 ) || min == 0 ) {
      if( out ) {
	*out << "Generation: " << g << "   Best Fitness: " << min << endl
	     << "The Fittest Automata has " 
	     << pop[minInd].vertexCount() << " vertices."
	     << endl << endl << ready;
      }
    }
   
    // exit if found a solution
    if( min == 0 ) {
      if( out )
	*out << "The word is trivial." << endl << end;
      
      // delete arrays and exit
      /*
      for( int x = 0; x < popSize; ++x ) {
	delete pop[x];
	delete newPop[x];
      }
      */
      return yes;
    }
    
    // make fitness values suitable for Roulette wheel selection
    int base = max + 1;
    for( int x = 0; x < popSize; ++x )
      fit[x] = base - fit[x];
    
    // fitness scaling
    if( cfg.haveFitnessScaling() )
      for( int x = 0; x < popSize; ++x )
	fit[x] = fit[x] * fit[x];
    
    // crossover
    RouletteWheel<int> wheel(popSize,fit);
    for( int x = 0; x < popSize; ++x ) {
      if( r.rand() <= crossRate ) {
	int i1 = wheel.GetIndex();
	int i2 = wheel.GetIndex();
	newPop[x] = crossover(pop[i1],pop[i2]);
      }
      else {
	newPop[x] = pop[x];
      }
    }


    // Roulette Wheel selection
    //RouletteWheel<int> wheel(popSize,fit);
   
    //for( int i = 0; i < popSize; ++i ) {
    //  newPop[i] = pop[wheel.GetIndex()];
    //}  


    // mutation
    for( int x = 0; x < popSize; ++x ) {
      if( r.rand() <= mutRate )
	newPop[x] = mutate(newPop[x]);
    }
     
    // elitist selection
    if( cfg.haveElitistSelection() ) {
      newPop[0] = pop[minInd];
    }
    
    // prepare for the next iteration
    for( int x = 0; x < popSize; ++x ) {
      pop[x] = newPop[x];
    }    
  }

  // delete arrays and exit
  /*
  for( int i = 0; i < popSize; ++i ) {
    delete pop[i];
    delete newPop[i];
  }
  */
  return dontknow;
}


int GeneticGraphWP::fitness( NCChunk& A ) const
{
  A.updateDistanceTable();
  NCChunk::VertexType target = 0;
  int i = 0;
  do {
    target = A.targetOfGenerator(target, ord(w[i++]));
#if SAFETY > 0
    if( target == -1 )
      error("int GeneticGraphWP::fitness( NCChunk& A ) const : "
	    "Cannot follow w in the graph.");
#endif
  } while( i < wLen );
  
  return A.getDistance(target);
  return 1;
}


NCChunk GeneticGraphWP::crossover( const NCChunk& A, const NCChunk& B ) const
{
  NCChunk C = A.join(B);
  C.reduce(w);
  return C;
}


NCChunk GeneticGraphWP::mutate( const NCChunk& A )
{
  const maxWordLen = 10;
  int len = 1 + int(r.rand() * maxWordLen);
  Word conj = randomWord(len);

  int numOfRels = relators.cardinality();
  SetIterator<Word> I(relators);
  int num = int(r.rand() * numOfRels);
  for( int x = 0; x < num; ++x ) I.next();
    
  Word res = conj * Word(I.value()) * conj.inverse();

  NCChunk C( A.join(NCChunk(theGroup.rank(), res)) );
  C.reduce(w);
  return C;
}

int GeneticGraphWP::randomGen( )
{
  int gen = int(r.rand() * theGroup.rank()) + 1;
  return (r.rand() <= 0.5) ? gen : -gen;
}


Word GeneticGraphWP::randomWord( int len )
{
  Word w;
  do { 
    VectorOf<Generator> v(len);
    for( int j = 0; j < len; ++j )
      v[j] = randomGen();
    w = Word(v).freelyReduce();
  } while( w.length() == 0 );
  
  return w;
}
