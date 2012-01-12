###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
gr=Magnus.Group()
gr1=Magnus.FGGroup(gr)
m=Magnus.Map()
#####
a=Magnus.Endomorphism( gr1) 
b=Magnus.EndomorphismFromWord( gr1, [w1,w2] ) 
c=Magnus.EndomorphismFromMap( m)
a.makeIdentity()
a.reduceGenImages() 
