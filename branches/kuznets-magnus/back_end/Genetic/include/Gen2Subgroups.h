// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Definition of Gen2Subgroups class
//
// Principal Author: Roman Kuznets
//
// Status: in progress
//
// Revision History:
//


#ifndef _Gen2Subgroups_H_
#define _Gen2Subgroups_H_

#include "SubgroupGraph.h"
//#include "RandomNumbers.h"
//#include "GlobalRandom.h"
//#include <utility>

//------------------------------ Gen2Subgroups ---------------------------------//


class Gen2Subgroups
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  Gen2Subgroups( );
  
  Gen2Subgroups( const SetOf<Word>& generatorsSG1, const SetOf<Word>& generatorsSG2 ) : 
    gensSG1( generatorsSG1 ), gensSG2( generatorsSG2 )
    {
      initialize();
    }
 
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SetOf<Word> generatorsSG1( ) const { return gensSG1; }

  SetOf<Word> generatorsSG2( ) const { return gensSG2; }

  int fitness( ) const;

  int rank1( ) const;

  int rank2( ) const;

  int rankInt( ) const;

  Gen2Subgroups surgery( ) const;
  // Surgery a la Baumslag

  Gen2Subgroups breed( const Gen2Subgroups P ) const;
  // breeding a la Baumslag. Generators of 1st subgroup of *this are bred with generators of the 1st subgroup of P.
  // similarly for the 2nd subgroups

  //  Gen2Subgroups crossfertilize( const Gen2Subgroups& S ) const;

  static SetOf<Word> createDihedralSubgroup( const int maxDihGr, const bool randomize );
  // creates a kernel of homomorphism onto the dihedral group of order 2n. n is chosen randomly from 1 to maxDihGr
  // Emptyness checks and reductions for generators of subgroup are omitted where superfluous 

  static SetOf<Word> createSymmetricSubgroup( const int MaxSymGr, const bool randomize );
  // relators defining the symmetric group of degree n. n is randomly chosen between 1 and MaxSymGr
  // Emptyness checks and reductions for generators of subgroup are omitted where superfluous 

  static Gen2Subgroups createKarSol2Subgroups( const int n );
  // inspired by counterexample from Karass&Solitar 
  // both subgroups correspond to the given n.
  // Emptyness checks and reductions for generators of subgroup are omitted where superfluous 

  Gen2Subgroups random2Subgroups( ) const;

  int largerFitness( const Gen2Subgroups P1, const Gen2Subgroups P2 ) const {
    return ( P1.fitness() > P2.fitness() ) ? 1 : 0;
  }
  // comparison function used for sorting std::vector<Gen2Subgroups> in descending order of fitness
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  
  friend ostream& operator << ( ostream& ostr, const Gen2Subgroups& S )
  {
    ostr << "First subgroup's generators are" << endl
         << S.gensSG1 << endl
         << "Second subgroup's generators are" << endl
         << S.gensSG2 << endl;
    return ostr;
  }

  /*
  friend istream& operator >> ( istream& istr, Gen2Config& C )
  {
    C.readFrom(istr);
    return istr;
  }
  */
  


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void initialize( );
  // computes ranks of the subgroups, rank of their intersection, and fitness

  Word randomWord( ) const;
  
  int randomGen( ) const;

  SetOf<Word> randomSubgroup( ) const;
  
  Word surgeryOnWord( const Word ) const;
  // Surgery a la Baumslag. Word is not randomly changed letter by letter. 
  // Rather some editor-style cut-n-paste operations are randomly applied  

  SetOf<Word> surgeryOnSubgroup( const SetOf<Word> ) const;
  // Surgery a la Baumslag. Each generator undergoes a surgery
  // A subgroup is represented by the set of its generators

  Word breedWords( const Word, const Word ) const;
  // Breeding of two words u_1u_2 and v_1v_2 a la Baumslag
  // the function returns a sibling Word
  // Resulting word is fully reduced but can be empty.

  SetOf<Word> breedSubgroups( const SetOf<Word>, const SetOf<Word> ) const; 
  // Breeding of two subgroups S1 and S2 a la Baumslag. The subgroups are represented by the set of generators each.
  // The function returns a sibling subgroup.
  // The generators of the 1st subgroup are mapped to the generators of the 2nd (in the round-robin manner if needed)
  // then each mapped pair is bred via breedWords.
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private data members:                                               //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  SetOf<Word> gensSG1, gensSG2; // subgroup generators for both subgroups
  static int maxCard;
  static int maxWordLen;
  static int numOfGens; // rank of the ambient free group

private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private data member:                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  int fitn;
  int r1, r2, rInt;
};

#endif

