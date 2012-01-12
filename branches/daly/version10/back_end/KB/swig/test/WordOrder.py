###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
g1=Magnus.Generator(3)
g2=Magnus.Generator(4)
w1=Magnus.Word()
w2=Magnus.Word()
a=Magnus.WordOrder()
b=Magnus.WordOrderFromChars("dsg")
c=Magnus.WordOrderFromCharsInt("sdf",2)
d=Magnus.WordOrderFromCharsVector("sdfdg",[1,2,3])
e=Magnus.WordOrderFromCharsVectorVector("dgf",[1,2,3],[1,2,3]);
c.signature(w1,w2)
c.signatureFromGenerator(g1,g2)
c.signatureFromInt(1,2)
c.balancedEquationFromRelator(w1,w2,w2)
c.historyBound([w1,w2])
##WordOrder::
##DiffHistoryRep * buildDiffHistoryRep() const ;
##c.buildDiffHistoryRepFromState()  DiffHistoryRep * buildDiffHistoryRep(State d,int g,int h) const ;
##DiffHistoryRep * update (const DiffHistoryRep & dh,State d,int g,int h,const Word & wd) const;
##c.reduction(const DiffHistoryRep & dh,int g,int h)
##c.possibleReduction(const AheadInfoRep & ai,int g)
##c.updateFromAheadInfoRep  AheadInfoRep *  update(const AheadInfoRep & ai,int g) const ;
##DiffHistoryVtxRep * buildDiffHistoryVtxRep() const ;
##%name(buildDiffHistoryVtxRepFromState) DiffHistoryVtxRep * buildDiffHistoryVtxRep(State d,int g,int h);
##%name(updateFromDiffHistoryVtxRep) DiffHistoryVtxRep * update 
##(const DiffHistoryVtxRep & dh,State d,int g,int h,DiffHistoryVtx * ptr) const;
##%name(reductionFromDiffHistoryVtxRep) Bool reduction(const DiffHistoryVtxRep & dh,int g,int h) const;
##Bool possibleReductionAhead(const DiffHistoryVtxRep & dh,int g) const;
c.getOrderType()
c.getNumSymbols()
c.getSymbolIndex(1)
c.getWeight(1)
c.getWeightFromWord(w1)
c.getSymbol(1)
c.getPosition(g1)
c.selfInverse(g1)
c.inverse(w1)
