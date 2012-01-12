###this is a test file by xuxw(xuxw76@yahoo.com)
###it's all right
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
a=Magnus.RelatorConjugate( ) 
b=Magnus.RelatorConjugateFromWord( w1, w2 )
x=a.relator
x=a.conjugator
print a
x=Magnus.conjugateBy( [a],w1 );
x=Magnus.RelatorConjugateFromString("edg")