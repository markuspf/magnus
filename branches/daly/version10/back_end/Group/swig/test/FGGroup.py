###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
g=Magnus.Group()
g1=Magnus.Generator(3)
w1=Magnus.Word()
w2=Magnus.Word()
a=Magnus.FGGroup(g)
x=a.type( );
x=a.numberOfGenerators( ) 
x=a.nameOfGenerator(1) 
x=a.namesOfGeneratorsFromGenerator(g1)  
x=a.namesOfGeneratorsNoParam( ) 
x=a.eval(w1 ) 
x=a.wordProblem(w1 ) 
x=a.conjugacyProblem( w1, w2) 
a.closeUnderCyclicPermutations([w1,w2]) 
x=a.readWord("xxw", "error") 
x=a.readSetOfWords("xxw", "error") 
x=a.readVectorOfWords("xxw", "error") 
a.printGenerator(g1)
a.printGenerators()
a.printWord(w1)
a.printSetOfWords([w1,w2])
a.printVectorOfWords([w1,w2])
###abstract b=Magnus.FGGroupFromString("dgfdg")
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
x=a.isTrivialElt(  e ) 
x=a.areEqual( e1,  e2) 
x=a.firstElt( ) 
x=a.nextElt( e) 
x=a.multiply( e1,  e2) 
x=a.inverseOf( e) 
x=a.raiseToPower( e, 1) 
x=a.conjugateBy( e1,  e2) 
x=a.commutator( e1,  e2) 
x=a.setMultiplyFromSetSet([e1,e2], [e1,e2]) 
x=a.setMultiplyFromEltSet( e, [e1,e2]) 
x=a.setMultiplyFromSetElt([e1,e2],  e) 
x=a.conjugateByFromSetSet([e1,e2], [e1,e2]) 
x=a.conjugateByFromEltSet( e, [e1,e2]) 
x=a.conjugateByFromSetElt([e1,e2],  e) 
a.closeUnderInverses([e2,e1]) 
a.ReadMeFrom("xxwgfdg")