///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "DFSA.h"
%}

class DFSA : public FSA {
typedef DFSARep::State State;
public:
// constructors for DFSA.
// Name is only needed for input/output, and may be set to "" (in which 
// case a non-empty but meaningless string will be assigned for output.

  DFSA( );

  %name(DFSAFromVector) DFSA(const VectorOf<Chars> & genNames) ;

  %name(DFSAFromCharVector)  DFSA(Chars name,const VectorOf<Chars> & genNames) ;

  %name(DFSAFromVectorInt)  DFSA( const VectorOf<Chars> & genNames, int numOfStates) ;

  %name(DFSAFromCharVectorInt)  DFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates) ;

  %name(DFSAFromVectorIntInt)  DFSA( const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings);

  %name(DFSAFromCharVectorIntInt)  DFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings) ;

  State target(State s,Generator g) const;
  int getNumStates() const;
/*
protected:
  DFSA( DFSARep * rep ) : FSA((FSARep *)rep) { }
  // Need to shadow inherited look() and change(), to get right ptr types.
  const DFSARep *look() const { return (DFSARep*)FSA::look(); }
  DFSARep *change() { return (DFSARep*)FSA::change(); }
*/
};

class GroupDFSA : public FSA {
// A GroupDFSA is a DFSA for which the alphabet is actually the set of
// all symbols of the listed alphabet together with their formal
// inverses. Transits are defined on symbols and their inverses,
// and words in those symbols with both positive and negative exponents
// may be read in.
// This class should be used if the symbols of the FSA are the generators
//(and their inverses) of a finitely generated group.

//friend class DiffMachineRep;
//typedef DFSARep::State State;

public:
// constructors for GroupDFSA.
  GroupDFSA( ) ;

  %name(GroupDFSAFromVector) GroupDFSA(const VectorOf<Chars> & genNames) ;

  %name(GroupDFSAFromWordOrder) GroupDFSA(const VectorOf<Chars> & genNames, const WordOrder & word_order) ;

  %name(GroupDFSAFromCharVector) GroupDFSA(Chars name,const VectorOf<Chars> & genNames) ;

  %name(GroupDFSAFromCharVectorWordOrder) GroupDFSA(Chars name,const VectorOf<Chars> & genNames,
                                             const WordOrder & word_order) ;

  %name(GroupDFSAFromCharVectorInt) GroupDFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates) ;

  %name(GroupDFSAFromVectorWordOrderInt) GroupDFSA( const VectorOf<Chars> & genNames, const WordOrder & word_order,
                                                             int numOfStates) ;

  %name(GroupDFSAFromVectorIntInt) GroupDFSA( const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings) ;

  %name(GroupDFSAFromVectorWordOrderIntInt) GroupDFSA( const VectorOf<Chars> & genNames, const WordOrder & word_order,
                                            int numOfStates, int numOfStrings) ;

  %name(GroupDFSAFromCharVectorIntInt) GroupDFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings) ;
  State target(State s,Generator g) const;
  int getNumStates() const ;
/*
protected:
  GroupDFSA( GroupDFSARep * rep ) : FSA((FSARep *)rep) { }
  // Need to shadow inherited look() and change(), to get right ptr types.
  const GroupDFSARep *look() const { return (GroupDFSARep*)FSA::look(); }
  GroupDFSARep *change() { return (GroupDFSARep*)FSA::change(); }
*/
};
