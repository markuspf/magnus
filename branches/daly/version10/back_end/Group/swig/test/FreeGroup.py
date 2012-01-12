###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
g1=Magnus.Generator(3)
w1=Magnus.Word()
w2=Magnus.Word()
w3=Magnus.Word()
e=Magnus.Elt()
e1=Magnus.Elt()
e2=Magnus.Elt()

a=Magnus.FreeGroup(2)
a=Magnus.FreeGroupFromList(["dsgd","dfgfg"])
x=a.type( )
x=a.actualType()
x=a.numberOfGenerators( )
x=a.nameOfGenerator(1)
x=a.namesOfGenerators( )
x=a.rank( )
#x=a.wordProblem( w1)    ###attemp to take Generator Word on trivial word
x=a.conjugacyProblem( w1, w2 ,w3)
x=a.inCommutatorSG(w1)
x=a.isCommutator( w1, w2 ,w3)
x=a.productOfCommutators( w1, "xxw");
x=a.productOfSquares( w1, "xxw");
x=a.getN_thElement(1 )
x=a.numberOfElement( w1)
x=a.nielsenBasis([w1,w2])
x=a.isAutomorphism([w1,w2])
x=a.isInnerAutomorphism([w1,w2])
x=a.isIAAutomorphism([w1,w2])
x=a.inverseAutomorphism([w1,w2])
x=a.readWord("sddgfdg", "error")
a.printWord(w1)
print a
x=a.firstGenerator()
b=Magnus.FreeGroupFromString("<a,b,c>")
###fromFGGroup
x=a.type( )
x=a.numberOfGenerators( ) 
x=a.nameOfGenerator(1) 
#x=a.namesOfGeneratorsFromGenerator(g1)  ###out of index
x=a.namesOfGeneratorsNoParam( ) 
#x=a.eval(w1 )  ###core dumped
#x=a.wordProblem(w1 ) ###attemp to take Generator Word on trivial word
x=a.conjugacyProblem( w1, w2, w3) 
#a.closeUnderCyclicPermutations([w1,w2])  ###frozen 
x=a.readWord("<a>", "error") 
x=a.readSetOfWords("xxw", "error") 
x=a.readVectorOfWords("xxw", "error")
#a.printGenerator(g1)    ###core dumped
#a.printGenerators()   ###out of index
a.printWord(w1)
#a.printSetOfWords([w1,w2])  ###core dumped
#a.printVectorOfWords([w1,w2])  ###core dumped
###fromGroup
x=a.type( )
x=a.actualType( )
x=a.order( )
x=a.isTrivial( )
x=a.isFinite( )
x=a.isInfinite()
x=a.isAbelian( )
x=a.makeIdentity( )
x=a.isSyntacticIdentity( e)
#x=a.isTrivialElt(  e )   ###core dumped below
#x=a.areEqual( e1,  e2) 
#x=a.firstElt( ) 
#x=a.nextElt( e) 
#x=a.multiply( e1,  e2) 
#x=a.inverseOf( e) 
#x=a.raiseToPower( e, 1) 
#x=a.conjugateBy( e1,  e2) 
#x=a.commutator( e1,  e2) 
#x=a.setMultiplyFromSetSet([e1,e2], [e1,e2]) 
#x=a.setMultiplyFromEltSet( e, [e1,e2]) 
#x=a.setMultiplyFromSetElt([e1,e2],  e) 
#x=a.conjugateByFromSetSet([e1,e2], [e1,e2]) 
#x=a.conjugateByFromEltSet( e, [e1,e2]) 
#x=a.conjugateByFromSetElt([e1,e2],  e) 
#a.closeUnderInverses([e2,e1]) 
a.ReadMeFrom("<a,b,c>")