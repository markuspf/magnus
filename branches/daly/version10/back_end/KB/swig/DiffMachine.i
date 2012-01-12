%module Magnus

%{
#include "DiffMachine.h"
%}

class DiffMachine: public FSA{
// this should surely be derived from DFSA but I can't get the
// constructor to work except by doing this
//  friend class RKBPackage; 
//  typedef DFSARep::State State;
public:
  DiffMachine( );

  %name(DiffMachineFromVector) DiffMachine( const VectorOf<Chars> genNames );

  %name(DiffMachineFromVectorWord) DiffMachine( const VectorOf<Chars> genNames,const WordOrder & word_order );
  
  void addDifferencesEqn
       (const Word & w1,const Word & w2,const WordOrder & word_order);
  void closeUnderSubstrings(int mode,const WordOrder & word_order);
  void closeUnderInverses(const WordOrder & word_order);
  Bool rewrite(Word & w) const;
  %name(rewriteFromWordOrder) Bool rewrite(Word & w,const WordOrder & word_order) const;
  Word getDifference(State s) const ;
  GroupDFSA wordAcceptor(const WordOrder & word_order) const;
  void buildDifferenceMachine(DiffMachine & D,
   const SetOf<Word> & differences, const WordOrder & word_order);
  void rebuildDifferenceMachine(const WordOrder & word_order);

/*

protected:
  
  DiffMachine( DiffMachineRep * rep ) : FSA((FSARep *)rep) { }
  // Need to shadow inherited look() and change(), to get right ptr types.
  const DiffMachineRep *look() const { 
    return (DiffMachineRep*)FSA::look(); }
  DiffMachineRep *change() { 
    return (DiffMachineRep*)FSA::change(); }
*/
};
