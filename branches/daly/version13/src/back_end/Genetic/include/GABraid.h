// Copyright (C) 1999 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of class PMQESol
//
// Principal Author: Dmitry Bormotov
//
// Status: in progress
//
// Description:
//
// Revision History:
//


#ifndef _GABraid_H_
#define _GABraid_H_


#include "PMWord.h"
#include "PMArray.h"
#include "GA.h"
#include "FreeGroup.h"


// ------------------------------- Triple ---------------------------------- //


struct Triple
{
  Triple(int i = 0, int j = 0, int k = 0, int l = 0) 
    : x(i), y(j), z(k), w(l) { }

  bool operator == (const Triple& t) const 
    { return x == t.x && y == t.y && z == t.z && w == t.w; }

  bool operator != (const Triple& t) const 
    { return !(*this == t); }

  friend ostream& operator << ( ostream& out, const Triple& t ) {
    out << " (" << t.x << "," << t.y << "," << t.z << "," << t.w << ")";
  }

  int x,y,z,w;
};


//---------------------------- PMB2ComConfig ------------------------------- //


class PMB2ComConfig
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  PMB2ComConfig( UniformRandom& R,  
		 int MaxNumOfComms,
		 FreeGroup sc,
		 int NumOfVars,
		 VectorOf<Word> Gens
		) 
    : r( R ),
      maxNumOfComms( MaxNumOfComms ),
      SC( sc ),
      numOfVars( NumOfVars ),
      numOfConsts( sc.numberOfGenerators() - NumOfVars ),
      gens( Gens )
    { 
      FreeGroup F(numOfConsts);
      VectorOf<Word> v(SC.numberOfGenerators());
      for( int i = numOfVars; i < v.length(); ++i )
	v[i] = Word(Generator(i+1-numOfVars));
      for( int i = 0; i < numOfVars; ++i )
	v[i] = gens[i];
      M = Map(SC,F,v);
    }

  // copy constructor, destructor and operators =, == supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  UniformRandom& rng( ) const { return r; }

  int getMaxNumOfComms( ) const { return maxNumOfComms; }
 
  int getNumOfVars( ) const { return numOfVars; }

  int getNumOfConsts( ) const { return numOfConsts; }

  FreeGroup getSC( ) const { return SC; }

  Map getM( ) const { return M; }

 
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  UniformRandom& r;
  int maxNumOfComms, numOfVars, numOfConsts;
  FreeGroup SC;
  VectorOf<Word> gens;
  Map M;
};


// ------------------------------- PMB2Com --------------------------------- //


class PMB2Com : public PM 
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PMB2Com( const PMB2ComConfig& CFG, int NumOfComms, Triple* T );

  PMB2Com( const PMB2ComConfig& CFG );

  PMB2Com( const PMB2Com& pm );
  
  ~PMB2Com( );
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* clone( ) const { return new PMB2Com(*this); }


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden hook functions:                                           //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PM* crossover( const PM* );

  PM* mutate( );

  PM* randomPM( ); 

  void printOn( ostream& ostr ) const;

  bool operator == ( const PM& p ) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  PMB2Com& operator = ( const PMB2Com& pm );

  Word productInOriginalGroup( const PMB2Com& pm ) const;


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Triple randomTriple( );

  Word productInOriginalGroup( int tLen, Triple* t ) const;

  bool productIsTrivial( int tLen, Triple* t) const;


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const PMB2ComConfig& cfg;
  Triple* comms;
  int numOfComms;
};


// --------------------------- GABurauSolver ------------------------------- //


class GABurauSolver : public GA
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GABurauSolver( const GAConfig& GAC, const FreeGroup& G, int NumOfVars, 
		 const FreeGroup& B, VectorOf<Word> BGens, 
		 const FreeGroup& sc );
  
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Overriden abstract functions:                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void initPopulation( );

  int fitness( const PM* pm );
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Map getSolution( const Word& u, ostream* o = NULL );
  // returns a solution for equation u found by genetic algorithm


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
  
  void reworkEquation( );
  
  void init( );
    
  Word PMtoG( const PM* pm, int k );

  bool checkForSolution( Map& res );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FreeGroup theGroup, B, SC;
  int numOfVars, numOfGens, numOfConsts, g;
  bool keepDetails;
  VectorOf<Word> bGens;
  Map M,MB;
  Word w;
  ostream* out;
  PMB2ComConfig PMCC;
  PMArrayConfig PMAC;
};


#endif
