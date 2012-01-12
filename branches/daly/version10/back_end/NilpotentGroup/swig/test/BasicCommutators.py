###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
g=Magnus.Generator(3)
l=Magnus.Letter() 
a=Magnus.BasicCommutators(1, 1)
a.initialize()     
x=a.theHirschNumber() 
x=a.numberOfGenerators() 
x=a.nilpotencyClass()
x=a.numberOfWeight(1) 
x=a.theFirstOfWeight(1) 
x=a.isInitialized()
x=a.wordForm()   
x=a.weightOf(1)  
x=a.leftIndexOf(1)  
x=a.rightIndexOf(1)  
x=a.commutatorName(1)  
x=a.commutatorNames() 
x=a.commutatorIsBasic(1, 1) 
x=a.findBC(1, 1) 
x=a.generatorsCommute(1, 1)  
x=a.commuteLetters(l, l) 
x=a.findCommutationRelation( l, l) 
x=a.decomposeLetter(l) 
a.printOn(["fdg","dsg"])
####
w1=Magnus.Word()
pw=Magnus.PolyWord()
b=Magnus.NGWordForms(a)
x=b.toWord(1)  
x=b.toWordFromPolyWord(pw)  
x=b.toWordFromWord(w1)  
x=b.commutatorStructure(1)  
x=b.asCommutatorWord(pw)  
x=b.asCommutatorWordFromLetter(l)  
x=b.asBracketedWord(1, ["dg","dfg"])  
x=b.asBracketedWordFromPolyWord(pw,["dg","dfg"])  













