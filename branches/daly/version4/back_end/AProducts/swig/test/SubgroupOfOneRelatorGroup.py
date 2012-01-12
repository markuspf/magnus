###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
#####
w1=Magnus.Word()
w2=Magnus.Word()
t=Magnus.OneRelatorGroupWithTorsion( 1, w1);
###
a=Magnus.ORGSubgroup( [w1,w2])
b=a.copy( ) 
x=a.parentGroup( )
x=a.generators()  
x=a.isMagnusSubgroup( )  
x=a.contains( w1 )  
x=a.rewriteFromGroupToSubgroupGenerators(w1 )  
x=a.rewriteFromSubgroupToGroupGenerators( w1 )  
x=a.lexicallyContains(  w1 )  
x=a.lexicallyContainsFromWordWord(  w1, w2 )  
print a
a!=b
#####
a=Magnus.SubgroupOfOneRelatorGroup( og,[w1]);
b=a.copy( )  
x=a.parentGroup()  
x=a.contains(  w1 )  
x=a.containsFromBool(  w1, 0 )  
x=a.rewriteFromGroupToSubgroupGenerators(  w1 )  
x=a.getIdentityProductDecomposition(  w1 )  
x=a.join(b )  
a!=b
###Inherited From ORGSubgroup
b=a.copy( ) 
x=a.parentGroup( )
x=a.generators()  
x=a.isMagnusSubgroup( )  
x=a.contains( w1 )  
x=a.rewriteFromGroupToSubgroupGenerators(w1 )  
x=a.rewriteFromSubgroupToGroupGenerators( w1 )  
x=a.lexicallyContains(  w1 )  
x=a.lexicallyContainsFromWordWord(  w1, w2 )  
print a
a!=b
#####
a=Magnus.SubgroupOfORGroupWithTorsion( t,	[w1] )
b=a.copy( )  
x=a.parentGroup()  
x=a.contains(  w1 )  
x=a.rewriteFromGroupToSubgroupGenerators(  w1 )  
a!=b
###Inherited From ORGSubgroup
b=a.copy( ) 
x=a.parentGroup( )
x=a.generators()  
x=a.isMagnusSubgroup( )  
x=a.contains( w1 )  
x=a.rewriteFromGroupToSubgroupGenerators(w1 )  
x=a.rewriteFromSubgroupToGroupGenerators( w1 )  
x=a.lexicallyContains(  w1 )  
x=a.lexicallyContainsFromWordWord(  w1, w2 )  
print a
a!=b