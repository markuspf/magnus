%module Magnus

%{
#include "HNNExtension.h"
%}

class HNNExtension :public FGGroup

{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Constructors:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // No default constructor for pseudo-abstract class.
  // Copy constructor, operator=, and destructor supplied by compiler.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Types:                                                             //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

//  typedef HNNExtensionRep::NumberOfSubgroup NumberOfSubgroup;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Accessors:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  const FPGroup& getFPGroup() const ;
  // Returns the presentation of the group.

  const FGGroup& getBasisGroup( ) const ;
  // Returns the basis group of the HNN-extension. 

  Generator stableLetter( ) const ;
  // Returns a stable letter.

  //  Due the fact that the implementations of subgroups are different, 
  //  we hide this through next interfacing members.
  // Subgroup getSubgroup( NumberOfSubgroup subgrp ) const;
  // virtual getGeneratorOfSubgroup( const NumberOfSubgroup subgrp, int gen ) = 0;
  // virtual getNumberOfGeneratorsInSubgroup( const NumberOfSubgroup subgrp ) = 0;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods and operators which deal with the group:                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  // Overrides pseudo-virtual functions from FGGroup:
  // 
  // Trichotomy isTrivial( ) const; // pseudo-virtual
  // Trichotomy isFinite( ) const; // pseudo-virtual
  // Trichotomy isInfinite( ) const; // pseudo-virtual
  // Trichotomy isAbelian( ) const; // pseudo-virtual

  Trichotomy isFree( ) const ;
  // Returns YES if this group is free on its generators, NO if not,
  // and DONTKNOW if no answer can be determined.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods and operators which deal with subgroups:                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool isSubgroupTrivial( const VectorOf<Word>& vec ) const ;

  bool isSubgroupAbelian( const VectorOf<Word>& vec ) const ;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods which deal with group elements:                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // Trichotomy wordProblem( const Word& w ) const;
  // Overrides pseudo-virtual function.

  int lengthOf( const Word& w ) const ;

  Word reducedFormOf( const Word& w ) const ;
  // Returns a reduced form of the given word.

  Word normalFormOf( const Word& w ) const ;
  // Returns a normal form of the given word.

  // Public data members:

  //@dp++ static Word Conjugator;

  Word cyclicallyReducedFormOf( const Word& w, Word conjugator ) const;
    //@dp++ = HNNExtension::Conjugator) const 
  // Returns a cyclially reduced form of the given word.
  
  VectorOf<Word> decompositionOf( const Word& w ) const ;
  
  VectorOf<Word> reducedDecompositionOf( const Word& w ) const ;
  
  VectorOf<Word> normalDecompositionOf( const Word& w ) const ;
  
  VectorOf<Word> cyclicallyReducedDecompositionOf( const Word& w, 
						   Word& conjugator) const ;

///undefined  Word compose( const VectorOf<Word>& V ) ;
  
  virtual Trichotomy  maximalRoot( const Word& w, Word& maxRoot, int& maxPower ) const;
  // Returns `yes' if maxPower > 1, and `no' if maxPower == 1.
  // Returns `dontknow' if a result cannot be determined.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

//  void printDecomposition( ostream& ostr, const VectorOf<Word> deco ) const ;

  %addmethods{
        HNNExtension( ){
        }

      	char* printDecomposition( const VectorOf<Word> deco ) const {
		strstream *s=new strstream();
		self->printDecomposition(*s,deco);
		(*s) << '\0' << flush;
		return s->str();
	}
   
  }

/*
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const HNNExtension& G )
  {
    G.look()->write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, HNNExtension& G )
  {
    G.change()->read(istr);
    return istr;
  }

protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Protected functions:                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  HNNExtension( HNNExtensionRep *newrep ) 
    : DerivedObjectOf<FGGroup,HNNExtensionRep> ( newrep ) { }

private:
  
  HNNExtension( ); // Hidden, not to be implemented.
*/
};

/*
%inline %{
	HNNExtension * HNNExtensionFromString(char* p) {
		strstream s;
		HNNExtension* F=new HNNExtension();
		s << p << '\0' << flush;
		Chars msg=s >> *F;
		if (msg.length()>0) {
			PyErr_SetString(PyExc_Exception,msg);
			PyErr_Print();
			return NULL;
		}
		return F;
	}
%}
*/