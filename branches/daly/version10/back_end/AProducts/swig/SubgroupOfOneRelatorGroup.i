%module Magnus

%{
#include "APwithOneRelator.h"

%}

/*
class ORGSubgroup {

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors / Destructor                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  ORGSubgroup( const VectorOf<Word>& gens );

  virtual ~ORGSubgroup( );


  virtual ORGSubgroup *copy( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors / Modifiers                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  virtual const OneRelatorGroup& parentGroup( ) const;

  const VectorOf<Word>& generators() const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with subgroup structure       //
  //                                                   //
  ///////////////////////////////////////////////////////

  bool isMagnusSubgroup( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////

  virtual bool contains( const Word& w ) const;

  virtual Word rewriteFromGroupToSubgroupGenerators( const Word& w ) const ;

  Word rewriteFromSubgroupToGroupGenerators( const Word& w ) const;

  bool lexicallyContains( const Word& w ) const;
  %name(lexicallyContainsFromWordWord) bool lexicallyContains( const Word& w, Word& wInBasis ) const;

//  virtual bool operator == ( const ORGSubgroup& S ) const;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O routines:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

///  friend ostream& operator << ( ostream& ostr, const ORGSubgroup& S ) 
 %addmethods{

	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
	int __cmp__(const ORGSubgroup& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
 }



  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const ORGSubgroup& S ) {
    S.write( ostr );
    return ostr;
  }

  friend istream& operator > ( istream& istr, ORGSubgroup& S ) {
    S.read( istr );
    return istr;
  }

protected:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual void write( ostream& ostr ) const;
  virtual void read( istream& istr );

private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O routines:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  virtual void printOn( ostream& ostr ) const;

  // data members:

  VectorOf<Word> theGenerators;

};
*/

class SubgroupOfOneRelatorGroup 
{

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////


  SubgroupOfOneRelatorGroup( const OneRelatorGroup& G,
			     const VectorOf<Word>& gens );

  ORGSubgroup *copy( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors / Modifiers                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  const OneRelatorGroup& parentGroup() const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with subgroup structure       //
  //                                                   //
  ///////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////

  bool contains( const Word& w ) const;
  // Overrides pure-virtual ORGSubgroup::contains().

  %name(containsFromBool) bool contains( const Word& w, bool keepDetails ) const;

  Word rewriteFromGroupToSubgroupGenerators( const Word& w ) const;
  // Overrides pure-virtual ORGSubgroup::rewriteFromGroupToSubgroupGenerators().

  ProductOfRelatorConjugates getIdentityProductDecomposition( const Word& w ) const;

  SubgroupOfOneRelatorGroup join( const SubgroupOfOneRelatorGroup& sg ) const;
  
  // Inherited from ORGSubgroup: 
  //   bool lexicallyContains( const Word& w ) const;
  //   bool lexicallyContains( const Word& w, Word& wInBasis ) const;

  
//  bool operator == ( const SubgroupOfOneRelatorGroup& S ) const ;
  // Overrides virtual ORGSubgroup::operator==().

 %addmethods{

	int __cmp__(const SubgroupOfOneRelatorGroup& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
 }

///Inherited From ORGSubgroup

  const VectorOf<Word>& generators() const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with subgroup structure       //
  //                                                   //
  ///////////////////////////////////////////////////////

  bool isMagnusSubgroup( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////



  Word rewriteFromSubgroupToGroupGenerators( const Word& w ) const;

  bool lexicallyContains( const Word& w ) const;
  %name(lexicallyContainsFromWordWord) bool lexicallyContains( const Word& w, Word& wInBasis ) const;

//  virtual bool operator == ( const ORGSubgroup& S ) const;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O routines:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

///  friend ostream& operator << ( ostream& ostr, const ORGSubgroup& S ) 
 %addmethods{

	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
 }


/*
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O routines:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void write( ostream& ostr ) const;
  // Overrides virtual ORGSubgroup::write().
  void read( istream& istr ); 
  // Overrides virtual ORGSubgroup::read().
 
private:

  // data members:

  OneRelatorGroup theParentGroup;
  
  //
  // 
  //

  struct ContainmentProblemData 
  {
    Word wordInParentGroupGenerators;
    bool inSubgroup;
    Word wordInSubgroupGenerators;
    ProductOfRelatorConjugates idProductDeco;
    bool keepDetails;
    
    ContainmentProblemData() 
      : wordInParentGroupGenerators( ), inSubgroup( true ),
	wordInSubgroupGenerators( ), idProductDeco( ),
	keepDetails( false ) { }
    
    ContainmentProblemData( const Word& w, bool f, const Word& u, bool keep_details,
			    const ProductOfRelatorConjugates& prodDeco ) 
      : wordInParentGroupGenerators(w),	inSubgroup(f),
	wordInSubgroupGenerators(u), idProductDeco( prodDeco ),
	keepDetails( keep_details ) { }
  };
  
  ContainmentProblemData containsResult;

  void setContainmentProblemData( const Word& w, bool in_subgroup, 
				  const Word& wInBasis, bool keepDetails, 
				  const ProductOfRelatorConjugates& product ) const;

  bool subgroupsDecomposition(const Word& w, 
			      const SubgroupOfOneRelatorGroup& T,
			      const SubgroupOfOneRelatorGroup& A,
			      Word& t_part, Word& a_part,
			      const bool keepDetails, 
			      ProductOfRelatorConjugates& prodDeco ) const;
  
  // define theParentGroup as HNN-extension of G_0 group.
  // define subgroup T (theGenerators) in G as HNN-extension of T_0 in G_0 

  bool pureContainmentProblem( const Word& u, bool keepDetails = false ) const;

  class MagnusBreakdown buildMagnusBreakdown( const Word& w, 
    SetOf<Generator>& subGens ) const;
*/
};


class SubgroupOfORGroupWithTorsion 
{

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////


  SubgroupOfORGroupWithTorsion( const OneRelatorGroupWithTorsion& G,
				const VectorOf<Word>& gens );

  ORGSubgroup *copy( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors / Modifiers                            //
  //                                                   //
  ///////////////////////////////////////////////////////

  const OneRelatorGroup& parentGroup() const ;
  // Overrides pure-virtual ORGSUbgroup::parentGroup().

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with subgroup structure       //
  //                                                   //
  ///////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////

  bool contains( const Word& w ) const;
  // Overrides virtual ORGSubgroup::contains().

  Word rewriteFromGroupToSubgroupGenerators( const Word& w ) const;
  // Overrides pure-virtual ORGSubgroup::rewriteFromGroupToSubgroupGenerators().

//  bool operator == ( const SubgroupOfORGroupWithTorsion& S ) const ;
  // Overrides virtual ORGSubgroup::operator==().
 %addmethods{

	int __cmp__(const SubgroupOfORGroupWithTorsion& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
 }
///Inherited From ORGSubgroup

  const VectorOf<Word>& generators() const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with subgroup structure       //
  //                                                   //
  ///////////////////////////////////////////////////////

  bool isMagnusSubgroup( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////


  Word rewriteFromSubgroupToGroupGenerators( const Word& w ) const;

  bool lexicallyContains( const Word& w ) const;
  %name(lexicallyContainsFromWordWord) bool lexicallyContains( const Word& w, Word& wInBasis ) const;

//  virtual bool operator == ( const ORGSubgroup& S ) const;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O routines:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

///  friend ostream& operator << ( ostream& ostr, const ORGSubgroup& S ) 
 %addmethods{

	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
 }


/*
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O routines:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void write( ostream& ostr ) const;
  void read( istream& istr );

  // data members:

  bool contains( const Word& w, Word& wInBasis ) const;

  OneRelatorGroupWithTorsion theParentGroup;
*/
};
