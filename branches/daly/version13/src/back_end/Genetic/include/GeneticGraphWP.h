// Copyright (C) 1997-1998 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of GeneticGraphWP class
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Revision History:
//


#ifndef _GeneticGraphWP_H_
#define _GeneticGraphWP_H_

#include "RandomNumbers.h"
#include "FPGroup.h"
#include "FreeGroup.h"
#include "Config.h"
#include "SubgroupGraph.h"


// ------------------------------- NCChunk --------------------------------- //


class NCChunkRep : public SubgroupGraphRep
{
public:

  NCChunkRep( int rank, SetOf<Word> S ) 
    : SubgroupGraphRep( rank,S ), distance(0) { }

  NCChunkRep( const NCChunkRep& G ) 
    : SubgroupGraphRep( G ), distance(0) { }

  NCChunkRep( const SubgroupGraphRep& G ) 
    : SubgroupGraphRep( G ), distance(0) { }

  ~NCChunkRep( ) { delete [] distance; }

  void deleteVertex( VertexType v);  

  void collectGarbage( );

  void updateDistanceTable( );

  void reduce( const Word& );

  long getDistance( VertexType v ) const;

  NCChunkRep* join(const NCChunkRep& C) const;

  NCChunkRep* clone( ) const { return new NCChunkRep( *this ); }
  // Standard clone.

private:

  bool isGarbage( VertexType v);
  void moveVertex( VertexType source, VertexType target);
  int power( VertexType v);
  SetOf<Integer> getWordVertices( const Word& ) const;

  // Data members:
  
  long* distance;
};


class NCChunk : public DerivedObjectOf<SubgroupGraph,NCChunkRep> 
{
public:
  
  NCChunk( int rank = 0, SetOf<Word> S = SetOf<Word>() ) : 
    DerivedObjectOf<SubgroupGraph,NCChunkRep>( new NCChunkRep(rank,S) ) { }
  /*
  NCChunk( const SubgroupGraph& G ) : 
    DerivedObjectOf<SubgroupGraph,NCChunkRep>( new NCChunkRep(G.enhance()) ) 
  { }
  */
  void deleteVertex( VertexType v )  
  {
    change()->deleteVertex(v);
  }

  void collectGarbage( )  
  {
    change()->collectGarbage();
  }

  void updateDistanceTable( )  
  {
    change()->updateDistanceTable();
  }

  void reduce( const Word& w )  
  {
    change()->reduce(w);
  }

  long getDistance( VertexType v ) const
  {
    return look()->getDistance(v);
  }

  NCChunk join(const NCChunk& C) const {
    return NCChunk( look()->join( *(C.look()) ) );
  }
    
private:

  NCChunk(NCChunkRep* rep) : DerivedObjectOf<SubgroupGraph,NCChunkRep>(rep) { }

};


// --------------------------- GeneticGraphWP ------------------------------ //


class GeneticGraphWP
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GeneticGraphWP( const FPGroup& G, const GHNConfig& config );
  
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Trichotomy isTrivial( const Word& u, ostream* out = NULL );
  // returns yes if the genetic algorithm could prove that u
  // is trivial; returns dontknow after computing all generations;
  // if out is not NULL it keeps details of the computation


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  int fitness( NCChunk& ) const;
  
  NCChunk mutate( const NCChunk& );

  NCChunk crossover( const NCChunk&, const NCChunk& ) const;

  int randomGen( );
  
  Word randomWord( int ); 
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FreeGroup theGroup;
  GHNConfig cfg;
  SetOf<Word> relators;
  Word w;
  int wLen;
  int automataSize;
  UniformRandom r;
};

#endif
