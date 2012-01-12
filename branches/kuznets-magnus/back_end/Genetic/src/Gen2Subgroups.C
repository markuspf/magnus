// Copyright (C) 1997 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of Gen2Subgroups class for Hanna Neumann conjecture.
//           Class consists of a pair of subgroups of the free group of rank numOfGens. 
//           Each subgroup is represented by the set of its generators. 
//           Genetic component consists of fitness function and fitness member using formula
//                (rank(A ^ B) - 1) - (rank A - 1)*(rank B - 1)
//           Mutate and crossover functions are implemented
//           
//
// Principal Author: Roman Kuznets
//
// Status: in progress
//
// Revision History:
//
//


#include "Gen2Subgroups.h"
#include "GlobalRoulette.h"
#include <string>


#undef DEBUG


//----------------------------- Gen2Subgroups ----------------------------------//

Gen2Subgroups::Gen2Subgroups( ) { }

SetOf<Word> Gen2Subgroups::createDihedralSubgroup( const int maxDihGr, const bool randomize )
// creates a kernel of homomorphism onto the dihedral group of order 2n. 
// n is randomly chosen between 2 and maxSymGr if `randomize' = true
// n is set to maxSymGr if `randomize' = false
// Randomness excluded
// Emptyness checks and reductions for generators of subgroup are omitted where superfluous 
{
  
  Word letA = Word(Generator(1));
  Word letB = Word(Generator(2));
  
  SetOf<Word> result;
  Word GeneratorWord;
  float op = globalUniformRandom.rand();
  
  int n;
  if ( randomize )
    {
      n = globalUniformRandom.rand( 2, maxDihGr );
    }
  else
    {
      n = maxDihGr;
    }
  cout << "Start creating a dihedral group..." <<  endl;
  
  result |= letA.power(2); // a^2
    
  for ( int i = 1; i < n; i++ ) { // for i=1 to n-1
    result |= letB.power(i) * letA * letB.power(n-i) * letA.inverse(); // b^i a b^(n-i) a^(-1)
    result |= letA * letB.power(i) * letA * letB.power(n-i); // a b^i a b^(n-i) 
    cout << "." ;
  }

  cout << endl;

  result |= letB.power(n); // b^n
  
  result |= letA * letB.power(n) * letA.inverse(); // a b^n a^(-1) 

  cout << "Completed creating a dihedral group with parameter n = " << n << endl;
    
  return result;
}

SetOf<Word> Gen2Subgroups::createSymmetricSubgroup( const int maxSymGr, const bool randomize )
// relators defining the symmetric group of degree n.
// n is randomly chosen between 2 and maxSymGr if `randomize' = true
// n is set to maxSymGr if `randomize' = false
// All other randomness excluded
// Emptyness checks and reductions for generators of subgroup are omitted where superfluous 
{

  Word letA = Word(Generator(1));
  Word letB = Word(Generator(2));
  
  SetOf<Word> result;
  Word generatorWord;

  int n;
  if ( randomize )
    {
      n = globalUniformRandom.rand( 2, maxSymGr );
    }
  else
    {
      n = maxSymGr;
    }
 
  cout << "Start creating a symmetric group..." << endl;

  Word w = letA.inverse() * letB * letA;

  Word b[n];

  for (int i = 0; i < n; i++ ) {
    b[i] = w;
    w = letA.inverse() * w * letA;
  }
    

  for ( int i = 0; i < n; i++ ) {
    cout << ".";
    generatorWord = b[i].power(2);
    generatorWord = generatorWord.freelyReduce();
    result |= generatorWord;
    for ( int j = 0; j < n; j ++ ) {
      if ( i != j ) {
	generatorWord = (b[i]*b[j]).power(3);
	generatorWord = generatorWord.freelyReduce();
	result |= generatorWord;
	for (int k = 0; k < n; k ++ ) {
	  if ( i != k && j != k ) {
	    generatorWord = (b[i]*b[j]*b[i]*b[k]).power(2);
	    generatorWord = generatorWord.freelyReduce();
	    result |= generatorWord;
	  }
	}
      }
    }
  }
  cout << endl;
  cout << "Completed creating a symmetric group with parameter n = " << n << endl;
  return result;
}

Gen2Subgroups Gen2Subgroups::createKarSol2Subgroups( const int n )
  // inspired by counterexample from Karass&Solitar 
  // both subgroups correspond to the given n.
  // Randomness excluded
  // Emptyness checks and reductions for generators of subgroup are omitted where superfluous 
{
  cout << "Start creating KarSol groups " << endl;

  Word letA = Word(Generator(1));
  Word letB = Word(Generator(2));
  
  SetOf<Word> group1;
  SetOf<Word> group2;

  Word word1 = letA; 
  Word word2 = letA * letB;

  for ( int i = 0; i < n+1; i++ ) {
    cout << ".";
    group1 |= word1;
    group2 |= word2;
    word1 = letB.inverse() * word1 * letB;
    word2 = letB.inverse() * word2 * letB;
  }
  cout << endl;
  cout << "Completed creating KarSol groups " << endl;
  Gen2Subgroups P( group1, group2 );
  cout << "Fitness = " << P.fitness() << endl;
  cout << "Rank 1 = " << P.rank1() 
       << "; rank 2 = " << P.rank2() << endl
       << "Rank of intersection is " << P.rankInt() << endl;
  return P;
}

void Gen2Subgroups::initialize( ) 
  // private function to be used only in constructors
  // initializes all the private members of an instance
{
  SubgroupGraph A(numOfGens, gensSG1);
  r1 = A.rank();
  //  cout << "Rank 1 = " << r1;
  SubgroupGraph B(numOfGens, gensSG2);
  r2 = B.rank();
  //  cout << "   Rank 2 = " << r2;
  SubgroupGraph I = A.intersection(B);
  rInt = I.rank();
  //  cout << "   Rank of intersection = " << rInt << endl;
  fitn = (rInt - 1) - (r1 - 1) * (r2 - 1);
  if ( fitn > 0 ) 
    {
      // including the time and date in ISO format into the name of the output file
      time_t ts = time(NULL);
      tm* timeStamp = localtime( &ts );
      std::ostringstream timeString; 
      timeString << "HNCCounterExample_";
      timeString << timeStamp->tm_year + 1900 << "_";
      if ( timeStamp->tm_mon < 9 ) 
	{
	  timeString << 0;
	}
      timeString << timeStamp->tm_mon + 1 << "_"; 
      if ( timeStamp->tm_mday < 10 ) 
	{
	  timeString << 0;
	}
      timeString << timeStamp->tm_mday << "T"; 
      if ( timeStamp->tm_hour < 10 ) 
	{
	  timeString << 0;
	}
      timeString << timeStamp->tm_hour << "_"; 
      if ( timeStamp->tm_min < 10 ) 
	{
	  timeString << 0;
	}
      timeString << timeStamp->tm_min << "_"; 
      if ( timeStamp->tm_sec < 10 ) 
	{
	  timeString << 0;
	}
      timeString << timeStamp->tm_sec; 


      // writing the counterexample into the file
      // HNCCounterExample_yyyy_mm_ddThh_mm_ss
      ofstream out( timeString.str().c_str() );
      if ( !out )
	{
	  cout << "A COUNTEREXAMPLE IS FOUND" << endl;
	  cerr << "Error opening file " << timeString.str() << endl;
	  cout << "Fitness is " << fitn << endl;
	  cout << "Rank of the 1st subgroup is " << rank1()
	       << "; rank of the 2nd subgroup is " << rank2() << endl
	       << "Rank of the intersection is " << rankInt() <<endl;
	  cout << "The pair of subgroups is" << endl;
	  cout << *this <<endl;
	  exit(1);
	}
      cout << endl << endl 
	   << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
      cout << "A COUNTEREXAMPLE IS FOUND" << endl;
      cout   << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
      cout << "The counterexample is saved in file " << timeString.str() << endl;
      out << "A COUNTEREXAMPLE TO HANNA NEUMANN CONJECTURE" << endl;
      cout << "Fitness is " << fitn << endl;
      out << "Fitness is " << fitn << endl;
      cout << "Rank of the 1st subgroup is " << rank1()
	   << "; rank of the 2nd subgroup is " << rank2() << endl
	   << "Rank of the intersection is " << rankInt() <<endl;
      out << "Rank of the 1st subgroup is " << rank1()
	   << "; rank of the 2nd subgroup is " << rank2() << endl
	   << "Rank of the intersection is " << rankInt() <<endl;
      out << "The pair of subgroups is" << endl;
      out << *this <<endl;
      out.close();
      cout << "The pair of subgroups is" << endl;
      cout << *this <<endl;
      exit(0);
    }
}
 
int Gen2Subgroups::rank1( ) const
{
  return r1;
}

int Gen2Subgroups::rank2( ) const
{
  return r2;
}

int Gen2Subgroups::rankInt( ) const
{
  return rInt;
}

int Gen2Subgroups::fitness( ) const
{
  return fitn;
}

Word Gen2Subgroups::surgeryOnWord( const Word w ) const
  // Surgery a la Baumslag. Word is not randomly changed letter by letter. 
  // Rather some editor-style cut-n-paste operations are randomly applied
{
  // cout << "Surgery on word " << w << " started " << endl;
  if ( w.isEmpty() ) {// the trivial word remains trivial
    // cout << "Surgery on word " << w << " completed; the word is empty; output is " << w  << endl;
    return w;
  }
  int split1 = globalUniformRandom.rand( 0, w.length() ); // randomly break the word into 3 parts:
  int split2 = globalUniformRandom.rand( 0, w.length() ); // [0..i) [i..j) [j..w.length())
  int i = ( split1 <= split2 ) ? split1 : split2; 
  int j = ( split1 <= split2 ) ? split2 : split1; 
  float op = globalUniformRandom.rand();
  if( op <= 0.5 ) { // radical surgery, chance 50%, part [i..j) is cut
    Word empWord = Word( );
    Word result = w.replaceSubword( i, j, empWord );
    result = result.freelyReduce();
    // cout << "Surgery on word " << w << " completed; radical surgery with i = " << i << ", j = " << j<< "; output is " << result << endl;
    return result;
  }
  else { // reconstructive surgery, chance 50%, the word is deconstructed and then reconstructed differently
    // in the current version w_1 w_2 w_3 is replaced by w_1 w_2 (w_3)^-1 w_1
    // here imagination should be used for reconstruction
    Word result = w.subword(0,i) * w.subword(i,j) * (w.subword(j,w.length())).inverse() * w.subword(0,i);
    result = result.freelyReduce();
    //cout << "Surgery on word " << w << " completed; reconstructive surgery with i = " 
    //  << i << ", j = " << j << "; output is " << result  << endl;
    return result;
  }
}
    
SetOf<Word> Gen2Subgroups::surgeryOnSubgroup( const SetOf<Word> S ) const
  // Surgery a la Baumslag. Each generator undergoes a surgery
  // Trivial generators are omitted.
  // If all generators are trivial, the mutation is unsuccessful and the original subgroup is returned.
{
  int card = S.cardinality();
  SetOf<Word> result( card );
  SetIterator<Word> I(S);
  for( int i = 0; i < card; ++i ) {
    Word w = I.value();
    w = surgeryOnWord( w );
    if ( ! w.isEmpty() ) {
      result |= w;
    }
  }
  if ( !result.isEmpty() ) 
    return result;
  else
    return S;
}
  
Gen2Subgroups Gen2Subgroups::surgery( ) const
  // Surgery a la Baumslag
{
  SetOf<Word> gens1 = surgeryOnSubgroup(gensSG1);
  SetOf<Word> gens2 = surgeryOnSubgroup(gensSG2);
  Gen2Subgroups result( gens1, gens2 );
  if ( ( result.r1 <= 1 ) || ( result.r2 <= 1 ) ) {
    cout << "Trivial result of surgery. Canceling surgery" << endl;
    return *this;
  }
  else 
    return result;
}


Word Gen2Subgroups::breedWords( const Word u, const Word v ) const
  // Breeding of two words u_1u_2 and v_1v_2 a la Baumslag
  // the function returns a sibling Word
  // Resulting word is fully reduced but can be empty.
{
  float op = globalUniformRandom.rand(); 
  int  uhalf = (int)(u.length()/2);
  int  vhalf = (int)(v.length()/2);
  if ( op <= 0.495 ) {  // Natural breeding, chance 99%
    // Either  sibling is u_1v_2, chance 49.5%
    Word result = u.subword( 0, uhalf ) * v.subword( vhalf, v.length() );
    result = result.freelyReduce();
    return result;
  }
  else if ( op <= 0.99 ) {
    // Or sibling  is v_1u_2, chance 49.5%
    Word result = v.subword( 0, vhalf ) * u.subword( uhalf, u.length() );
    result = result.freelyReduce();				  
    return result;
  }
  else { //verbal breeding, chance 1%
    // sibling is w(u,v)
    // for w = a^2 b^3 a^-5 b
    Word result = u.power( 2 ) * v.power( 3 ) * u.power( -5 ) * v;
    result = result.freelyReduce();				  
    return result;
  }
}

SetOf<Word> Gen2Subgroups::breedSubgroups( const SetOf<Word> S1, const SetOf<Word> S2 ) const
  // Breeding of two subgroups S1 and S2 a la Baumslag. The subgroups are represented by the set of generators each.
  // The function returns a sibling subgroup.
  // The generators of the 1st subgroup are mapped to the generators of the 2nd (in the round-robin manner if needed)
  // then each mapped pair is bred via breedWords.
{
  SetOf<Word> result;
  SetIterator<Word> iter1( S1 );
  SetIterator<Word> iter2( S2 );
  //
  for ( ; !iter1.done() ; iter1.next() ) { // main loop over all generators of the 1st set
    if ( iter2.done() ) // the 2nd set is iterated over in a round-robin manner if it is shorter than the 1st, 
      iter2 = SetIterator<Word>( S2 );
    Word siblingWord = breedWords( iter1.value(), iter2.value() );
    if ( !siblingWord.isEmpty() ) 
      result |= siblingWord;
  }
  //
  // if the resulting subgroup is trivial, the breeding is unsuccessful and we return the original 1st subgroup instead
  if ( result.isEmpty() ) 
    result = S1;
  return result;
}
   
Gen2Subgroups Gen2Subgroups::breed( const Gen2Subgroups P ) const
   // breeding a la Baumslag. Generators of 1st subgroup of *this are bred with generators of the 1st subgroup of P.
   // similarly for the 2nd subgroups
{
  //  cout << "breeding started on " << *this << " and " << P << endl;
  SetOf<Word> generatorsBred1 = breedSubgroups( generatorsSG1(), P.generatorsSG1() );
  SetOf<Word> generatorsBred2 = breedSubgroups( generatorsSG2(), P.generatorsSG2() );
  //  cout << "breeding completed" << endl;
  Gen2Subgroups result( generatorsBred1, generatorsBred2 );
  if ( ( result.r1 <= 1 ) || ( result.r2 <= 1 ) ) {
    cout << "Trivial result of breeding. Repeating breeding" << endl;
    return breed( P );
  }
  else
    return result;
}

SetOf<Word> Gen2Subgroups::randomSubgroup( ) const
{
  SetOf<Word> S;
  int numOfSGens = maxCard/2;//int(r.rand() * (maxCard-1)) + 2;
  for( int g = 0; g < numOfSGens; ++g )
    S |= randomWord();

  SubgroupGraph A(numOfGens, S);
  if( A.rank() <= 1 ) return randomSubgroup( );
  return S;
}

Gen2Subgroups Gen2Subgroups::random2Subgroups( ) const
{
  SetOf<Word> S1, S2;
  S1 = randomSubgroup( );
  S2 = randomSubgroup( );
  return Gen2Subgroups(S1, S2);
}

Word Gen2Subgroups::randomWord( ) const
{
  Word w;
  do { 
    int vLen =  int(globalUniformRandom.rand() * maxWordLen) + 1;
    //int vLen = maxWordLen/2;
    VectorOf<Generator> v(vLen);
    for( int i = 0; i < vLen; ++i )
      v[i] = randomGen();
    w = Word(v).freelyReduce();
  } while( w.length() == 0 );
  
  return w;
}


int Gen2Subgroups::randomGen( ) const
{
  int gen = int(globalUniformRandom.rand() * numOfGens) + 1;
  return (globalUniformRandom.rand() <= 0.5) ? gen : -gen;
}


int Gen2Subgroups::maxWordLen = 15;
int Gen2Subgroups::maxCard = 30;
int Gen2Subgroups::numOfGens = 5;

