###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
og=Magnus.OneRelatorGroup( 1, w1)
r=Magnus.RelatorConjugate()
sg=Magnus.SubgroupOfOneRelatorGroup( og,[w1]);
###
a=Magnus.HNNExtOfORGroup( og,"sdg",sg, sg);
b=Magnus.HNNExtOfORGroup( og,"sdg",sg, sg);
x=a.wordProblem( w1 )  
x=a.wordProblemFromBool( w1, 0, [r])  
x=a.reducedDecompositionOf( w1, 0,[r])  
###Inherited From HNNExtOfORGroupGeneric
x=a.subgroup(1)  
x=a.basisGroup( )  
x=a.getPresentation( )  
x=a.nameOfStableGenerator( )  
x=a.wordProblemFromWord( w1)  
x=a.lengthOf( w1)  
x=a.lengthOfFromVector( [w1] )  
x=a.decompositionOf( w1)  
x=a.cyclicallyReducedDecompositionOf( w1, w2)
x=a.mappingFromSubgroup( 1, w1 )  
x=a.mappingDecompositionOf( 0,w1, w2, w1)
x=a.stableLetter( )  
print a
a!=b

