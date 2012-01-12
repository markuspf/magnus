###this is a test file by xuxw(xuxw76@yahoo.com)
### it's all right
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
g=Magnus.FPGroup()
ag=Magnus.AbelianGroup( g)
a=Magnus.AbelianInfinitenessProblem( g)
a.startComputation( )
x=a.continueComputation( ) 
x=a.done( )
x=a.isInfinite( ) 
