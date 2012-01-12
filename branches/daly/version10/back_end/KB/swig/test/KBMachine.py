###this is a test file by xuxw(xuxw76@yahoo.com)
### all right but comment functions
import Magnus
w=Magnus.Word()
wo1=Magnus.WordOrder()
a=Magnus.KBMachine( )
b=Magnus.KBMachineFromVector(["ds","dg"])
c=Magnus.KBMachineFromVectorWordOrder(["asf","sfdf"],wo1) ;
d=Magnus.KBMachineFromVectorWordOrderInt(["asf","sfdf"],wo1,2)
##e=Magnus.KBMachineFromVectorWordOrderIntInt(["asf","sfdf"],wo1,2,1)
c.rewrite(w)
c.oldFormatPrintOn()