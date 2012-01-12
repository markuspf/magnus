###this is a test file by xuxw(xuxw76@yahoo.com)
##it's all right except NilpotentCollector class
import Magnus
w=Magnus.Word()
pw=Magnus.PolyWord()
b=Magnus.NGCollector(1,1)
g=Magnus.Generator(3)
bc=Magnus.BasicCommutators(1, 1)
####it's an abstract class
a=Magnus.NilpotentCollector(1,1)
b=Magnus.NilpotentCollectorFromBasicCommutators(bc)
a.initialize()
x=a.make(1,1);
###  x=a.clone() const=0;
x=a.commutators()  
###virtual NilpotentCollector::Type type() const=0;
x=a.isInitialized()  
a.collectingProcess(  pw )  
x=a.collect( w)  
x=a.collectFromPolyWord( pw)  
x=a.collectLetter(pw, g)  
x=a.multiply(pw, pw)  
x=a.raiseToPower(pw, 2)  
x=a.inverse(pw)  
x=a.weightOf( w)  
x=a.weightOfFromPolyWord(pw)  
a.writeIPC()  
a.readIPC("dgfd")
######
c=Magnus.NGCollectorFromNGCollector(b);
b.initialize()  
x=b.commutators()  
x=b.type()  
x=b.isInitialized()  
b.collectingProcess( pw )  
x=b.collect( w)  
x=b.collectFromPolyWord(pw)  
x=b.collectLetter(pw, g)  
x=b.multiply(pw, pw)  
x=b.raiseToPower(pw, 1)  
x=b.inverse(pw)  
x=b.weightOf( w)  
x=b.weightOfFromPolyWord(pw)  
b.writeIPC()