%module Magnus

%{
#include "MagnusBreakdown.h"

%}

class MagnusBreakdown
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Constructors:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  MagnusBreakdown( const OneRelatorGroup& G );
    
  // No default constructor, but it can be easily arranged by
  // combining default FPGroup constructor and previsious one.

  // Copy constructor and operator = provided by compiler
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Accessors:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  OneRelatorGroup getORGroup() const;
  // Returns a current node of Magnus breakdown.

  HNNExtOfORGroup getHNNPresentation() const;
  // The caller should call `findHNNPresentation' before 
  // and checks a presentation to be existed, 
  // otherwise this function causes error.

  int numberOfUsedGenerators() const ;

  Generator stableGenerator() const ;

  bool hasAccompGenerator() const ;

  Generator accompGenerator() const;

  ListOf<Generator> getGeneratorsWithZeroExpSum() const ;
  
  ListOf<Generator> getGeneratorsWithNonzeroExpSum() const ;

  ListOf<Generator> getDefaultBreakdownGenerators() const;

  int getExponentSumOf( const Generator& g ) const ;

  Word rewriteWordInOldGenerators( const Word& w ) const; 

  Word rewriteWordInNewGenerators( const Word& w ) const;

  Automorphism embeddingOfORGroups( ) const ;

  Map toHNNPresentation( ) const ;

  Map toORGroup( ) const ;

  int numberOfOldGenerators( ) const ;
  int numberOfNewGenerators( ) const ;

  VectorOf<Range> getSubscriptsTable() const ;
  
  Generator adjoinSubscript(const Generator& oldGen, int subscript) const;
  Generator extractSubscript(const Generator& newGen, int& subscript) const;
  // @dp NOTE: these procedures work with the bigger one-relator group.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Start/continue:                                                    //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Trichotomy hasHNNPresentation() const ;
  // `no' means the node is a (terminal) list. 
  // `dontknow' means the node is not investigated yet.

  bool findHNNPresentation();
  // Does one step of Magnus breakdown.

  %name(findHNNPresentationFromGenerator) bool findHNNPresentation( const Generator& stableGen );

  %name(findHNNPresentationFromGeneratorGenerator)  bool findHNNPresentation( const Generator& stableGen, 
			    const Generator& accompGen );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O routines:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
///  void printOn( ostream& ostr ) const;

///undefined   MagnusBreakdown *readFrom( istream& istr ) const;

///  void debugPrint( ostream& ostr ) const;
    %addmethods{
/* undefined	char* printOn() const {
		strstream *s=new strstream();
		self->printOn(*s);
		(*s) << '\0' << flush;
		return s->str();
	}*/
	char* debugPrint() const {
		strstream *s=new strstream();
		self->debugPrint(*s);
		(*s) << '\0' << flush;
		return s->str();
	}

    }

/*
protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Protected functions:                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void makeHNNPresentation( );
  bool chooseStableGenerator( );
  void makeSubscriptsTable( );
  void makeEmbedding();
  void makeTranslationTables();


  // Data members:

  // fixed data members:

  OneRelatorGroup	theORGroup;
  int			theNumberOfOldGenerators;
  ListOf<Generator>	theGeneratorsWithZeroExpSum;
  ListOf<Generator>	theGeneratorsWithNonzeroExpSum;
  VectorOf<int>		exponentsOfOldGenerators;

  
  // volatile data members:

  OneRelatorGroup	theLargerORGroup;
  HNNExtOfORGroup	theHNNPresentation;
  bool			haveHNNPresentation;
  Automorphism		theEmbeddingOfORGroups;
  Map			mapToHNN;
  Map			mapToORGroup;
  SuperGen		stable, accomp;
  Generator		stableNewGen;
  int			theNumberOfNewGenerators;
  VectorOf<Chars>	theNamesOfNewGenerators;
  VectorOf<Range>	subscriptsOfGenerator;
  VectorOf<int>		toNewGens;
  VectorOf<int>		toOldGens;
*/
};
/*
ProductOfRelatorConjugates liftUpProduct( 
  const ProductOfRelatorConjugates& mProduct, 
  const MagnusBreakdown& M, const OneRelatorGroup& G 
);


// --------- Inline members of MagnusBreakdown ------------------ //

inline HNNExtOfORGroup MagnusBreakdown::getHNNPresentation() const 
{
#if SAFETY > 0
  if( !haveHNNPresentation )
    error("Called MagnusBreakdown::getHNNPresentation() "
	  "when HNN presentation does not exist.");
#endif
  return theHNNPresentation;
}

inline Generator MagnusBreakdown::accompGenerator() const 
{
#if SAFETY > 0
  if( stable.exponent() == 0 )
    error("MagnusBreakdown::accompGenerator(): Calling for "
	  "an accompanying generator when it is not defined.");
#endif
  return accomp.generator();
}

#endif
*/


