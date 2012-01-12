%module Magnus

%{
#include "AbelianSGPresentation.h"
%}


class AbelianSGPresentation:public AbelianGroup{

public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Copy constructor provided by compiler


  AbelianSGPresentation( );

  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O                                              //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Inherited from AbelianGroup:

  Word fromSGPGensToSGGens(const Word& w)const;
   // Makes the map from presentation generators to
   // generators of parent group

  bool fromSGGensToSGPGens(const Word& w,Word& wInNew)const;
   // Makes the map from  generators of parent group to
   // presentation generators, if word w is in the subgroup it
   // returns true, and put the reflection in wInNew.
   // If word w does not belong to the subgroup, it returns false.

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  // Operators inhereted from AbelianGroup
 
 /*
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Debugging stuff                                  //
  //                                                   //
  ///////////////////////////////////////////////////////
protected:
private:
  friend  AbelianSGPresentation AbelianGroup::makeSubgroupPresentation
                                               (const VectorOf<Word>& vG) const;

  //Constructor is private because it have sence only for AbelianGroup
  // It accepts the following parameters:
  // parent - parent group of a subgroup, sgGens - generators od SG in terms of parent
  // thePresentation - the presentation for a subgroup as a group
  // sgPGens - generators of presentation epressed in generators of canonical 
  // decomposition of a parent group , makeF - true if you whant to make a files with 
  // generators, the same as for AbelianGroup
  AbelianSGPresentation(const AbelianGroup& parent,const VectorOf<Word>& sgGens,
                          const FPGroup& thePresentation,const DArray<Integer>& sgPGens,
                          bool makeF = false):
  DerivedObjectOf<AbelianGroup,AbelianSGPresentationRep>( new AbelianSGPresentationRep
                                                  (parent,sgGens,thePresentation,sgPGens, 
                                                   makeF) ) { }
  // Special wrapping constructor to wrap new representations (returned
  // by eg. delegated methods) and for base initialisation by derived
  // classes:
 
  AbelianSGPresentation( AbelianSGPresentationRep* newrep ) : 
                               DerivedObjectOf<AbelianGroup,AbelianSGPresentationRep>(newrep) { } 

 
#ifdef DEBUG
  friend int main(... );
#endif
*/

};

