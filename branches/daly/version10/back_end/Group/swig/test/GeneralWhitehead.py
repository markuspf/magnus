###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
g=Magnus.FreeGroup(2)
ws=[w1,w2]
a=Magnus.GeneralWhitehead( g )
a.startComputation( ws )
a.startComputationFromWord( w1 )
x=a.continueComputation( )
x=a.done( ) 
x=a.extendsToFreeBasis( )
x=a.getAutomorphism( )
x=a.getFileName( )

  
