###this is a test file by xuxw(xuxw76@yahoo.com)
### 
import Magnus
w1=Magnus.Word()
w2=Magnus.Word()
wo=Magnus.WordOrder()
WA=Magnus.GroupDFSA()
GM=Magnus.GenMult()
g1=Magnus.Generator(3)
D=Magnus.DFSA()
Diff=Magnus.DiffMachine()
a=Magnus.KBmagPackageFromAllParam(["23","dg"],[w1,w2],wo,1)
b=Magnus.KBmagPackageFromWordOrder(["23","dg"],[w1,w2],wo)
c=Magnus.KBmagPackage(["23","dg"],[w1,w2])
b.sanityCheck()
b.autgroup()
b.gpaxioms()
b.gpcheckmult()
b.gpgenmult()
b.gpmakefsa()
b.gpwa()
b.kbprog ()
b.getName()
b.isProvedAutomatic( )
b.isProvedConfluent( )
b.getGeneratorName(g1)
b.wordAcceptor();
b.generalMultiplier();
b.differenceMachine(2)
b.setWordAcceptor(WA)
b.setGeneralMultiplier(GM)
b.setDifferenceMachine(Diff,1)
###GroupDFSARep wordAcceptorRep();
###GenMultRep generalMultiplierRep();
###DiffMachineRep differenceMachineRep(int i);
###%name(setWordAcceptorFromGroupDFSARep) void setWordAcceptor(const GroupDFSARep & WARep);
###%name(setGeneralMultiplierFromGenMultRep) void setGeneralMultiplier(const GenMultRep & GMRep);
###%name(setDifferenceMachineFromDiffMachineRep) void setDifferenceMachine(const DiffMachineRep & DiffRep,int i);
b.minimize(D)
b.minimizeFromGroupDFSA(WA)
###%name(minimizeFromDFSARep)  Bool minimize(DFSARep & D);
###%name(minimizeFromGroupDFSARep)  Bool minimize(GroupDFSARep & D);
b.gpcomp(WA,WA,WA);
###%name(gpcompFromGroupDFSARep)  Bool gpcomp(GroupDFSARep & D1,GroupDFSARep & D2,GroupDFSARep & D3);
