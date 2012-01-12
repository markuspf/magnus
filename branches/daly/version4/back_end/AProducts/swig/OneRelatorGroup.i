%module Magnus

%{
#include "OneRelatorGroup.h"

%}


class OneRelatorGroup : public FGGroup
{

public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Constructors:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  OneRelatorGroup( int ngens, const Word& relator );
  // To construct a group of given number of unnamed generators and
  // a given relator.
  
  %name(OneRelatorGroupFromVector) OneRelatorGroup( const VectorOf<Chars>& gennames, const Word& relator );
  // To construct a group with named generators, a given relator.  

  %name(OneRelatorGroupFromFPGroup)  OneRelatorGroup( const FPGroup& G );
  
  // Copy constructor, operator=, and destructor supplied by compiler.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Accessors:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Word relator( ) const;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods and operators which deal with the group:                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods and operators which deal with subgroups:                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods which deal with group elements:                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Trichotomy wordProblem( const Word& w ) const;


  %name(wordProblemFromBool) Trichotomy wordProblem( const Word& w, bool keepDetails, 
    ProductOfRelatorConjugates& productOfRelatorConjugates ) const;

  // Trichotomy wordProblem( const Word& w ) const;
  // Overrides pseudo-virtual FGGroup::wordProblem().

///Inherited From FGGroup


/*
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const OneRelatorGroup& G )
  {
    G.look()->write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, OneRelatorGroup& G )
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

  OneRelatorGroup( OneRelatorGroupRep *newrep ) 
    : DerivedObjectOf<FGGroup, OneRelatorGroupRep> ( newrep ) { }
*/
};


/////////////////////////////////////////////////////////////////////////// 

class EnumeratorOfConsequences 
{
public:
  EnumeratorOfConsequences( const OneRelatorGroup& G );
  // Create enumerator of all relator's consequences for the given group.

  Word word() const;
  // It returns a consequence of the relator.

  ProductOfRelatorConjugates tuple() const;
  // It return a consequence is presented as decomposition of conjugates
  // of the given relator.

//  void reset();
  // Resets the enumerator.

  bool done() const;
  // Returns false iff all consequences are enumerated.

  void advance();
  // It advances enumeration.

/* 
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const EnumeratorOfConsequences& ce )
  {
    ostr 
      < ce.theCurrentWordNumber 
      < ce.theLastComputedWordNumber
      < ce.theCurrentWord
      < ce.theCurrentProduct
      < ce.theRelator
      < ce.theGroup;
      
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, EnumeratorOfConsequences& ce )
  {
    
    istr 
      > ce.theCurrentWordNumber 
      > ce.theLastComputedWordNumber
      > ce.theCurrentWord
      > ce.theCurrentProduct
      > ce.theRelator
      > ce.theGroup;
  
  return istr;
  }

private:

  void generate( ) const;
  // Generate new consequence.

  Integer theCurrentWordNumber;
  Integer theLastComputedWordNumber;
  Word theCurrentWord;
  ProductOfRelatorConjugates theCurrentProduct;
  Word theRelator;
  FreeGroup theGroup;
*/
};


%inline %{
	OneRelatorGroup * OneRelatorGroupFromString(char* p) {
		strstream s;
                FPGroup* fg=new FPGroup();
		OneRelatorGroup* F=new OneRelatorGroup(*fg);
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
