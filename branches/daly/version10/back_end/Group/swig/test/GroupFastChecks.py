###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
fp=Magnus.FPGroup()
g=Magnus.Generator(3)
a=Magnus.GroupFastChecks(fp )
x=a.hasTrivialPresentation()
x=a.numOfRelsLessNumOfGens( )
x=a.existsGenWithExpSumZeroInEveryRelator( g)
x=a.getExpSumOfGen( g)
x=a.GCDOfExpSumOfGen(g )