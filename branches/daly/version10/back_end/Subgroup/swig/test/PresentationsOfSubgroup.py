###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
g=Magnus.Group()
g1=Magnus.Generator(3)
gg=Magnus.FGGroup(g)
sg=Magnus.Subgroup( gg)
p=Magnus.PermutationRepresentation()
###
a=Magnus.PresentationsOfSubgroup(sg) 
x=a.makePresentation("dgfg") 
x=a.makePresentationTC(p,"xxw") 
x=a.makePurePresentationTC(p) 
x=a.rewriteWord( p ,w1) 


