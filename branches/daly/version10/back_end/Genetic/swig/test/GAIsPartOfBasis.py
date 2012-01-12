###this is a test file by xuxw(xuxw76@yahoo.com)
###it's all right but comment
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
ws=[w1,w2]
fg=Magnus.FreeGroup(2)
c=Magnus.ACConfig( )
###
a=Magnus.GAIsPartOfBasisFromVectorVector( fg, c,ws)
b=Magnus.GAIsPartOfBasisFromVector(fg,c,ws,ws) 
b=Magnus.GAIsPartOfBasis( fg, c,w1) 
### xxwundo x=a.transform( ostream& , int ) 
### xxwundo x=a.isPartOfBasis(ostream& out1, ostream& out2) 
### xxwundo x=a.isPartOfBasisFromChars(ostream& out1, "sdgf") 
x=a.getAutomorphism()  
x=a.getCurrentBestWords() 
x=a.getFixedWords()  
x=a.nOfGenerations()  
x=a.fitness( w1) 
x=a.randomWord( ) 
