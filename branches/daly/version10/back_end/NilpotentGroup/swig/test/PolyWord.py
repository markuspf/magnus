
###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
l=Magnus.Letter()
w=Magnus.Word()
a=Magnus.PolyWord() 
aw=Magnus.AbelianWord()
b=Magnus.PolyWordFromLetter( l)  
b=Magnus.PolyWordFromWord( w)  
b=Magnus.PolyWordFromAbelianWord( aw)  
###b=Magnus.PolyWordFromPolyWordRep(PolyWordRep* rep)  
x=a.length()  
x=a.numberOfLetters()  
x=a.isEmpty()  
x=a.isCollected()  
x=a.firstLetter()  
x=a.lastLetter()   
a.debugInfo()  
x=a.toChars(["dsg","fdg"])  
a.freelyReduce()  
x=a.inverse()  
a.append(b)  
a.appendFromLetter(l)  
a.clear()
a.removeFirstLetter()  
x=a.raiseToPower(1)  	
x=a*b
print a
a.printOn()
x=Magnus.PolyWordCommutator(a,b)
x=Magnus.PolyWordCommutatorOfInverses(a,b)
x=Magnus.PolyWordEvaluateWordFromWord(w,[a]) 
x=Magnus.PolyWordEvaluateWord(a,[a]) 




