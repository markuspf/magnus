###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
g1=Magnus.Generator(3)
g1=Magnus.Generator(4)
w1=Magnus.Word()
w2=Magnus.Word()
G=Magnus.FreeGroup(2)
a=Magnus.QEqnSolutionsInFreeGroup(  G,   w1,1) 
a.startComputation() 
a.continueComputation() 
a.disableBuildingRegStab() 
a.enableBuildingRegStab() 
a.disableBuildingSolutions() 
a.enableBuildingSolutions() 
x=a.isComputationDone()   
x=a.isComputationStarted()   
x=a.haveNewSolution()    
x=a.nextSolution()  
x=a.haveNewStabGenerator()    
x=a.nextStabGenerator()  
x=a.isBuildingSolutionsEnabled()   
x=a.isBuildingRegStabEnabled()    
x=a.getSolution() 
x=a.getStabGenerator()  
x=a.surfaceForm() 
x=a.toSurfaceForm() 
x=a.group()   
x=a.equation()    
x=a.numberOfVariables()   
x=a.numberOfGenerators()    
x=a.numberOfConstants()    
x=a.rank()   
###VectorPtrOf<Endomorphism> basicSolutions()   
###VectorPtrOf<Automorphism> regStabGenerators()   
x=a.prefixMap()   
###EquationStatus getProcessStatus()   
x=a.eliminator()   
## struct EquationStatus
##  {
##    public:
##    EquationStatus() 
##
##    // copy contsructor, operator=, destructor supplied by compiler.
##
##    // data members:
##
##    bool enableBuildingSolutions 
##    bool enableBuildingRegStab 
##    int verticesToPass 
##    int verticesPassed 
##    int loopsToPass 
##    int loopsPassed 
##    long totalLengthOfLoops 
##  } 
print a
x=a.checkEquation(G,w1, 1) 
