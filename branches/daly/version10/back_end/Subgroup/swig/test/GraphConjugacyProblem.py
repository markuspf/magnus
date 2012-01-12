###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
fg=Magnus.FPGroup()
sg=Magnus.SubgroupGraph(1, ws) 
###
a=Magnus.GraphConjugacyProblem( fg, w1, w2 )
a.startComputation( )
a.continueComputationFromSubgroupGraph( sg )
a.continueComputation( )
x=a.done( ) 
x=a.theNewGraphIsNeeded( ) 
x=a.getConjugator( ) 
