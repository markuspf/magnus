%module Magnus

%{
#include "HNNExtOfORGroup.h"

%}
/*  abstract class
class HNNExtOfORGroupGeneric
{

public:

  enum NumberOfSubgroup { A = 0, B = 1 };
  enum Pinch { UP, DOWN };

//  Constructors: 

  HNNExtOfORGroupGeneric( const OneRelatorGroup& G, const Chars& stableGenName,
			  const ORGSubgroup& A, const ORGSubgroup& B);
  // stable generator has biggest order in constructing group = lastGenOf(G)+1.

  
  HNNExtOfORGroupGeneric( const HNNExtOfORGroupGeneric& H );

  virtual ~HNNExtOfORGroupGeneric( );

  HNNExtOfORGroupGeneric& operator=( const HNNExtOfORGroupGeneric& H );


//  Accessors/modifiers:

  virtual const ORGSubgroup& subgroup(NumberOfSubgroup i) const ;

  const OneRelatorGroup& basisGroup( ) const ;

  FPGroup getPresentation( ) const;

  Chars nameOfStableGenerator( ) const ;

//  Methods which deal with group elements:

  virtual Trichotomy wordProblem( const Word& w ) const = 0;

  int lengthOf( const Word& w ) const;
  int lengthOf( const VectorOf<Word>& deco ) const;

  VectorOf<Word> decompositionOf( const Word& w ) const;
  VectorOf<Word> reducedDecompositionOf( const Word& w ) const;
  VectorOf<Word> normalDecompositionOf( const Word& w ) const;

  VectorOf<Word> cyclicallyReducedDecompositionOf( const Word& w, Word& conj )
    const;
  // It returns a decomposition w1 of w and a conjugator `conj' such that
  // w1 = w ^ conj.

  //  static Word compose( const VectorOf<Word>& deco );

  Word mappingFromSubgroup( NumberOfSubgroup subgrp, const Word& w ) const;

  ProductOfRelatorConjugates mappingDecompositionOf( 
    const NumberOfSubgroup& S, const Word& w, const Word& wInSBasis, 
    const Word& tail ) const;

  Generator stableLetter( ) const ;


//  Standard comparison operators:  

//  virtual bool operator == ( const HNNExtOfORGroupGeneric& G ) const;

//  friend inline ostream& operator << ( ostream& ostr, const HNNExtOfORGroupGeneric& H ) 


protected:

  struct PinchStruct {
    Pinch type;
    int number;
    Word word;
  };

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Debugging stuff                                  //
  //                                                   //
  ///////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O routines:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  void printOn( ostream& ostr ) const;

  void printDecomposition( ostream& ostr, const VectorOf<Word>& deco ) const;

  //  virtual HNNExtOfORGroupGeneric* readFrom( istream& istr, 
  //                                            Chars& errMesg ) const;

  virtual void debugPrint( ostream& ostr ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  IPC tools:                                       //
  //                                                   //
  ///////////////////////////////////////////////////////

  virtual void write( ostream& ostr ) const;
  virtual void read( istream& istr );

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Private helper stuff:                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  int powerOfStableGen(int component, const VectorOf<Word>& deco ) const;

  bool suspectPinch(int component, const VectorOf<Word>& deco ) const;

  bool abelianizationTest( const PinchStruct& pinch, 
			   const VectorOf<int>& powersOfGens,
			   const VectorOf<bool>& subgroupGens ) const;

  PinchStruct formPinch(int component, const VectorOf<Word>& deco) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Debugging stuff                                  //
  //                                                   //
  ///////////////////////////////////////////////////////

  int unusedGenerators( const Word& test, VectorOf<Word>& toNewGens,
			VectorOf<Word>& toOldGens ) const;

  void init( const HNNExtOfORGroupGeneric& H );

  // Data members:

  OneRelatorGroup *theBasisGroup;
  Chars theNameOfStableGenerator;
  ORGSubgroup *subgroups[2];  
//  virtual bool operator == ( const HNNExtOfORGroupGeneric& G ) const;

//  friend inline ostream& operator << ( ostream& ostr, const HNNExtOfORGroupGeneric& H ) 

   %addmethods{
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}

	int __cmp__(const Word& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
  }
};

*/
class HNNExtOfORGroup 
{

public:

  enum NumberOfSubgroup { A = 0, B = 1 };
// enum Pinch { UP, DOWN };
//  Constructors: 

  HNNExtOfORGroup( const OneRelatorGroup& G,
		   const Chars& stableGenName,
		   const SubgroupOfOneRelatorGroup& A,
		   const SubgroupOfOneRelatorGroup& B);
  // stable generator has biggest order in constructing group = lastGenOf(G)+1.

//  Accessors/modifiers:

  // SubgroupOfOneRelatorGroup subgroup(int i) const { return subgroups[i]; }
  // OneRelatorGroup getBasisGroup( ) const { return theBasisGroup; }
  // FPGroup getPresentation( ) const;
  // Chars nameOfStableGenerator( ) const { return theNameOfStableGenerator; }

//  Methods which deal with group elements:

  Trichotomy wordProblem( const Word& test ) const;

  %name(wordProblemFromBool) Trichotomy wordProblem( const Word& test, bool keepDetails,
    ProductOfRelatorConjugates& productOfRelatorConjugates ) const;

  //  int lengthOf( const Word& w ) const;
  //  int lengthOf( const VectorOf<Word>& deco ) const;
  //  VectorOf<Word> decompositionOf( const Word& w ) const;
  //  VectorOf<Word> reducedDecompositionOf( const Word& w ) const;
  //  VectorOf<Word> normalDecompositionOf( const Word& w ) const;
  //  VectorOf<Word> cyclicallyReducedDecompositionOf( const Word& w, 
  //                                                   Word& conj ) const;
  //  Word mappingFromSubgroup( int subgrp, const Word& w ) const;

  VectorOf<Word> reducedDecompositionOf( const Word& w, bool keepDetails,
    ProductOfRelatorConjugates& productOfRelatorConjugates ) const;


//  Standard comparison operators:  

  //  bool operator == ( const HNNExtOfORGroup& G ) const;


  ///////////////////////////////////////////////////////
  //                                                   //
  //  Debugging stuff                                  //
  //                                                   //
  ///////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O routines:                                    //
  //                                                   //
  ///////////////////////////////////////////////////////

  //  void printOn( ostream& ostr ) const;
  //  void printDecomposition( ostream& ostr, const VectorOf<Word>& deco )
  //    const;
  //  HNNExtOfORGroup* readFrom( istream& istr, Chars& errMesg ) const;
  //  void debugPrint( ostream& ostr ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  IPC tools:                                       //
  //                                                   //
  ///////////////////////////////////////////////////////

  //  void write( ostream& ostr ) const;
  //  void read( istream& istr );

//Inherited from HNNExtOfORGroupGeneric
//  HNNExtOfORGroupGeneric& operator=( const HNNExtOfORGroupGeneric& H );


//  Accessors/modifiers:

  virtual const ORGSubgroup& subgroup(NumberOfSubgroup i) const ;

  const OneRelatorGroup& basisGroup( ) const ;

  FPGroup getPresentation( ) const;

  Chars nameOfStableGenerator( ) const;

//  Methods which deal with group elements:

  %name(wordProblemFromWord) virtual Trichotomy wordProblem( const Word& w ) const;

  int lengthOf( const Word& w ) const;
  %name(lengthOfFromVector) int lengthOf( const VectorOf<Word>& deco ) const;

  VectorOf<Word> decompositionOf( const Word& w ) const;
////  VectorOf<Word> reducedDecompositionOf( const Word& w ) const;
//  %name(reducedDecompositionOfFromWord) VectorOf<Word> reducedDecompositionOf( const Word& w ) const;
//  VectorOf<Word> normalDecompositionOf( const Word& w ) const;

  VectorOf<Word> cyclicallyReducedDecompositionOf( const Word& w, Word& conj )
    const;
  // It returns a decomposition w1 of w and a conjugator `conj' such that
  // w1 = w ^ conj.

  //  static Word compose( const VectorOf<Word>& deco );

  Word mappingFromSubgroup( NumberOfSubgroup subgrp, const Word& w ) const;

  ProductOfRelatorConjugates mappingDecompositionOf( 
    const NumberOfSubgroup& S, const Word& w, const Word& wInSBasis, 
    const Word& tail ) const;

  Generator stableLetter( ) const ;

//  Standard comparison operators:  

//  virtual bool operator == ( const HNNExtOfORGroupGeneric& G ) const;

//  friend inline ostream& operator << ( ostream& ostr, const HNNExtOfORGroupGeneric& H ) ;
    %addmethods{


	int __cmp__(const HNNExtOfORGroupGeneric& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}

	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}

    }


/*
private:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Private helper stuff:                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  //  int powerOfStableGen(int component, const VectorOf<Word>& deco ) const;
  //  bool suspectPinch(int component, const VectorOf<Word>& deco ) const;
  //  bool abelianizationTest( const Pinch& pinch, 
  //                           const VectorOf<int>& powersOfGens,
  //			       const VectorOf<bool>& subgroupGens ) const;
  //  Pinch formPinch(int component, const VectorOf<Word>& deco) const;


protected:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Debugging stuff                                  //
  //                                                   //
  ///////////////////////////////////////////////////////

  //  Generator stableLetter( ) const;

  int unusedGenerators( const Word& test, VectorOf<Word>& toNewGens,
			VectorOf<Word>& toOldGens ) const;

 

protected:

  // Data members:

  //  OneRelatorGroup theBasisGroup;
  //  Chars theNameOfStableGenerator;
  //  VectorPtrOf<SubgroupOfOneRelatorGroup> subgroups;
  //  static const int A = 0; // number of subgroup A.
  //  static const int B = 1; // number of subgroup B.
*/

};


