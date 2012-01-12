###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
fg=Magnus.FreeGroup(2)
c=Magnus.GHNConfig()
###
a=Magnus.GAEquationSolver( fg, 2, c )
x=a.getSolution( w1 )
###
a=Magnus.GraphicEquationSolver( fg, "dsg", w1,c)
x=a.getSolution(w1)
