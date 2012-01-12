###this is a test file by xuxw(xuxw76@yahoo.com)
###it's all right
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
c=Magnus.GHNConfig(  )
g=Magnus.FPGroup()
###
a=Magnus.GAWP( g, c) 
x=a.isTrivial(w1 )
####
a=Magnus.GAWP2( g, c)
x=a.isTrivial(w1,)
