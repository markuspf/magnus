
%module Magnus

%{
#include "WordOrder.h"

%}
// @sr for now this file needs to be independent of DFSARep.
// So for now we'll typedef State to be int, but that's only temporary.
// That's because a declaration of SetOf<DiffHistory> is necessary in Set.C
// (for each of the various DiffHistory types)
// and that can't be dependent on the FSA hierarchy.

class WordOrder{
public:
//constructor
  WordOrder() ;

  %name(WordOrderFromChars) WordOrder(const Chars & oType);

  %name(WordOrderFromCharsInt)  WordOrder(const Chars & oType, int numOfSymbols);

  %name(WordOrderFromCharsVector)  WordOrder(const Chars & oType,const VectorOf<int> & v);
// v is interpreted as specifying an order of generators for shortlex,
// but as weights for the weighted orders.
// To specify both orders and weights for the weighted orders, two
// vectors must be given as arguments

// The next constructor cannot be used for ShortLex
  %name(WordOrderFromCharsVectorVector) WordOrder(const Chars & oType,const VectorOf<int> & o,const VectorOf<int> & w);

  int signature(const Word & w1,const Word & w2) const;
  // return 1 if w1<w2, -1 if w1>w2, 0 if w1=w2
    
  %name(signatureFromGenerator) int signature(Generator g,Generator h) const ;
  // return 1 if g<h, -1 if g>h and 0 if g=h
  %name(signatureFromInt) int signature(int i,int j) const ;
  // returns signature(g,h), where g is the i-th and h the j-th generator in
  // the order.

  void balancedEquationFromRelator(const Word & w,Word & lhs,Word & rhs) const;
  // sets lhs equal to w1, rhs to w2, where w1>=w2 in the word order
  // w=w1*w2^-1 and w1 is as short as possible such that this can happen.

  int historyBound(const VectorOf<Word> & diffs) const;
  DiffHistoryRep * buildDiffHistoryRep() const ;
  %name(buildDiffHistoryRepFromState) DiffHistoryRep * buildDiffHistoryRep(State d,int g,int h) const ;
// build a difference history for the word difference g^-1h, attached
// to state d of the difference machine, bearing in mind the mysterious bound M,
// which might mean the history becomes trivial.

  DiffHistoryRep * update 
     (const DiffHistoryRep & dh,State d,int g,int h,const Word & wd) const;
// Given a difference history dh associated with pairs of words w,u,
// construct the difference history associated with pairs wg,uh and the
// state d of the difference machine (which is of course the target
// of the state for dh under the pair of symbols (g,h)

  Bool reduction(const DiffHistoryRep & dh,int g,int h) const;
// returns YES if the difference history contains a pair w,u of words
// such that wg=_G uh and uh < wg in the word order.

  Bool possibleReduction(const AheadInfoRep & ai,int g) const ;

  %name(updateFromAheadInfoRep) AheadInfoRep *  update(const AheadInfoRep & ai,int g) const ;


  DiffHistoryVtxRep * buildDiffHistoryVtxRep() const ;
  %name(buildDiffHistoryVtxRepFromState) DiffHistoryVtxRep * buildDiffHistoryVtxRep(State d,int g,int h);
// build a difference history for the word difference g^-1h

  %name(updateFromDiffHistoryVtxRep) DiffHistoryVtxRep * update 
     (const DiffHistoryVtxRep & dh,State d,int g,int h,DiffHistoryVtx * ptr) const;
// Given a DiffHistoryVtx * dhp associated with pairs of words w,u, 
// construct the difference history associated with pairs wg,uh

  %name(reductionFromDiffHistoryVtxRep) Bool reduction(const DiffHistoryVtxRep & dh,int g,int h) const;
// returns YES if the difference history contains a pair w,u of words
// such that wg=_G uh and uh < wg in the word order.

  Bool possibleReductionAhead(const DiffHistoryVtxRep & dh,int g) const;

  Chars getOrderType() const ;
  int getNumSymbols() const ;
  int getSymbolIndex(int i) const;
// i takes values from 1 to getNumSymbols() below
  Generator getWeight(int i) const;
  %name(getWeightFromWord) int getWeight(const Word & w) const ;
  Generator getSymbol(int i) const ;
// getPosition(0 is the inverse of getSymbol()
  int getPosition(Generator g) const ;
  int selfInverse(Generator g) const ;
  Word inverse(const Word & w) const ;
/*
protected:
  typedef ObjectOf<WordOrderRep> R;
  WordOrder( WordOrderRep *p ) : R(p) {}
*/
};

