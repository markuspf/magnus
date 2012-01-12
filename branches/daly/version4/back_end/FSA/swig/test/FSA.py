###this is a test file by xuxw(xuxw76@yahoo.com)
###this is a abstract class
import Magnus
w1=Magnus.Word()
a=Magnus.FSA()
b=Magnus.FSA()
a.accepts(w1);
a.rejectsInState(w1,1)
a.nextAcceptedWord(w1)
a.minimize()
a.sizeLanguage()
a.finiteLanguage()
a.readFrom("xxw")
a.printOn()
a.setName("dfgf")
a.getNumStates()
a!=b
