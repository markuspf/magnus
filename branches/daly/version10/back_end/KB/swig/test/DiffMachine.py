###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
wo1=Magnus.WordOrder()
wo2=Magnus.WordOrder()
a=Magnus.DiffMachine( )
b=Magnus.DiffMachineFromVector(["xs","gfg"])
c=Magnus.DiffMachineFromVectorWord(["xs","gfg"],wo1);
b.addDifferencesEqn(w1,w2,wo1);
b.closeUnderSubstrings(1,wo1);
b.closeUnderInverses(wo1);
b.rewrite(w1)
b.rewriteFromWordOrder(w1,wo1)
###b.getDifference(State s) const ;
###typedef DFSARep::State State;
###GroupDFSA wordAcceptor(wo1) const;
b.buildDifferenceMachine(a,[w1] ,wo1);
b.rebuildDifferenceMachine(wo1);
