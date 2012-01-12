///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "GAIsPartOfBasis.h"
%}

class GAIsPartOfBasis
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
 /////////////////////////////////////////////////////////////////////////
  GAIsPartOfBasis( const FreeGroup& G, const ACConfig& config,
		   const Word& w);

  %name(GAIsPartOfBasisFromVectorVector) GAIsPartOfBasis( const FreeGroup& G, const ACConfig& config,
		   const VectorOf<Word>& v);
  
  %name(GAIsPartOfBasisFromVector) GAIsPartOfBasis( const FreeGroup& G, const ACConfig& config,
		   const VectorOf<Word>&,const VectorOf<Word>&);
   
 
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Trichotomy transform( ostream& , int );

  bool isPartOfBasis(ostream& out1, ostream& out2);
  %name(isPartOfBasisFromChars) bool isPartOfBasis(ostream& out1, const Chars& out2_f_name);

  VectorOf<Word> getAutomorphism() const;

  const VectorOf<Word>& getCurrentBestWords()const;
  const VectorOf<Word>& getFixedWords() const ;

  int nOfGenerations() const ;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  int fitness( const Word& u,ostream& out = cout, bool print = false );
  
  Word randomWord( );
/*
  
private:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void initialize();

  void printWords(ostream& o, const VectorOf<Word>& v) const;

  Word mutate( const Word& u );
  
  int randomGen( );
  
  Word crossover1( const Word& w1,const Word& w2 );
  
  int genToInsertAfter( Generator g );
  
  int genToInsertBefore( Generator g );
  
  int genToInsert( Generator g1, Generator g2 );
  
  VectorOf<Word> computeAuto(const Word&);

  int computeFitness( const VectorOf<Word>& v ) const;

  int computeFitness1( const VectorOf<Word>& v ) const;

  int hamingDistance(const Word& w1,const Word& w2) const;

  int tupleHamingDistance(const VectorOf<Word>&,const VectorOf<Word>&)const;
  void permuteVector(VectorOf<Word>& v1, int move_to) const;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  FPGroup theGroup;
  ACConfig cfg;
  VectorOf<Word> theWords;

  VectorOf<Word> tmpWords;

  VectorOf<Word> bestWords;
  VectorOf<Word> fixedWords;

  Chars best_out_name;
  bool print_best_each_time;

  friend int main(...);
 
  UniformRandom r;
  int numberOfGens;
  int numberOfTrans;
  int threshold;
  
  VectorOf<Word> transformations;
  VectorOf<Word> transferImage;
  VectorOf<Word> theAuto;
  AssociationsOf<int,int> reduces;

  int g;
  int sav_first_max;

  Word finalResult;
  Word bestTransformation;
  int total_tr;
  int swithc_tr;
*/
};
