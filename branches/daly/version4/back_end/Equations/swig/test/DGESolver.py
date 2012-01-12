###this is a test file by xuxw(xuxw76@yahoo.com)
###it's all right
import Magnus
w=Magnus.Word()
F=Magnus.FreeGroup(2)
m=Magnus.Map()
a=Magnus.DGESolver( F, ["df","fdg"], w )
x=a.getSolution( w, m)
