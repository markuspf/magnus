###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
pw1=Magnus.PolyWord()
pw2=Magnus.PolyWord()
g=Magnus.Generator(2)
vc=["x1","x2"]
ng=Magnus.NilpotentGroup(vc, 1, [w1,w2],0)
#####
a=Magnus.SubgroupBasis([w1,w2],ng);
a.initialize()     
x=a.cardinality() 
x=a.parentGroup() 
x=a.generators() 
x=a.isFull()
x=a.isNormalClosure() 
x=a.theHirschNumber() 
x=a.normalClosure() 
x=a.contains(w1) 
x=a.decomposeWord(pw1,pw2) 
x=a.asWords() 
x=a.asGroupWords() 
