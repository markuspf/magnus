###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
g1=Magnus.FreeGroup(2)
g2=Magnus.FreeGroup(3)
sg=Magnus.SGofFreeGroup( g1, ws)
g=Magnus.Group()
###
a=Magnus.APwithOneRelator() 
b=Magnus.APwithOneRelatorFromWord(g1, g2, w1, w2) 
c=Magnus.APwithOneRelatorFromVetor(g1, g2,[w1],[w2]  ) 
d=Magnus.APwithOneRelatorFromSGofFreeGroup(sg, sg)
e=Magnus.APwithOneRelatorFromGroup( g)
x=a.type( )
x=a.conjugacyProblem(w1, w2, w1)
x=a.centralizerOf(w1) 
