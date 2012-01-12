###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
pw=Magnus.PolyWord()
ng=Magnus.NGCollector(1,1)
g=Magnus.Generator(3)
m=Magnus.MalcevSet()
###abstract class
###a=Magnus.NilpotentPresentation()
###b=Magnus.NilpotentPresentationFromNGCollector(ng,[w1,w2])
###x=a.isBuilt()
###a.build()
###x=a.makeGroup() 
###a.read("dgfdg")
###a.printOn()
###a.write()
####
a=Magnus.PresentationForNG(ng,[w1,w2]);
##inherited from NilpotentPresentation
x=a.isBuilt()
a.build()
x=a.makeGroup() 
a.read("dgfdg")
a.printOn()
a.write()
###
##inherited from NilpotentPresentation
a=Magnus.PresentationForSNG(m,[w1,w2],ng);
x=a.isBuilt()
a.build()
x=a.makeGroup() 
a.read("dgfdg")
a.printOn()
a.write()
