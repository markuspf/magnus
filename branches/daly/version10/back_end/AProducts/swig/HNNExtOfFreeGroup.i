%module Magnus

%{
#include "HNNExtOfFreeGroup.h"

%}

class HNNExtOfFreeGroup:public HNNExtension

{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Constructors:                                                      //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  HNNExtOfFreeGroup( const FreeGroup& F, 
		     const Chars& nameOfStableLetter,
		     const SGofFreeGroup& subgroupA,
		     const SGofFreeGroup& subgroupB );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Accessors:                                                         //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  // inherited  FPGroup getFPGroup() const { return look()->getFPGroup(); }

  // const FGGroup& getBasisGroup( ) const;
  // Overrides pseudo-virtual `HNNExtension::getBasisGroup()'.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  //  Methods and operators which deal with the group:                   //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  // Overrides pseudo-virtual functions from FGGroup:
  // 
  // int order( ) const; // pseudo-virtual
  // 
  // Trichotomy isFree( ) const { } // pseudo-virtual
  // *** Time consuming algorithm ! ***

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

  Trichotomy isProperPower( const Word& w ) const;
  // It can always determine a result for cyclic subgroups case only.
  // General case is partially supported, it can return `dontknow'.

  // Trichotomy maximalRoot( const Word& w, Word& maxRoot, int& maxPower ) const;
  // overrides pseudo-virtual HNNExtension::maximalRoot().
  // It can determines a result for cyclic subgroups case only.
  // General case is partially supported, it can returns `dontknow'.

  Trichotomy conjugacyProblem( const Word& w, const Word& u, Word& conjugator ) const;

  bool isProperPowerOfSecond( const Word& w, const Word& u, int k ) const ;
  // Returns `true' iff it exists a number k>1 such that w^k = u,
  // `false' otherwise.

 ///Inherited From HNNExtension

##   Word compose( const VectorOf<Word>& V ) ;
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

  friend ostream& operator < ( ostream& ostr, const HNNExtOfFreeGroup& G )
  {
    G.look()->write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, HNNExtOfFreeGroup& G )
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

  HNNExtOfFreeGroup( HNNExtOfFreeGroupRep *newrep ) 
    : DerivedObjectOf<HNNExtension, HNNExtOfFreeGroupRep> ( newrep ) { }
*/

};
