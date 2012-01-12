###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
og=Magnus.OneRelatorGroup( 1, w1)
####
a=Magnus.GAConjProblemForORGroupSolver(og , w1, w2 ) 
x=a.isConj( ) 
x=a.getNumberOfIterations( ) 
x=a.getFileName( )  
x=a.rnd1(1)   
x=a.roulette( 1 , 1 ) 
y=2.3
x=a.rouletteFromInt(1 , y) 
x=a.greedyReduce( og, w1 ) 
x=a.oneGreedyReduce( og , w1 ) 
a.insert( w1, w2, 1) 

