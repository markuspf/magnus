###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
e=Magnus.Elt()
e1=Magnus.Elt()
e2=Magnus.Elt()
a=Magnus.Group();
b=Magnus.Group();
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
a!=b
print a
b.ReadMeFrom("xxwgfdg")
