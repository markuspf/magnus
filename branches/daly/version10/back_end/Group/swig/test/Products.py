###this is a test file by xuxw(xuxw76@yahoo.com)
###it's all right
import Magnus
fp1=Magnus.FPGroup()
fp2=Magnus.FPGroup()
a=Magnus.FreeProduct( fp1, fp2);
x=a.mapFromG1()
x=a.mapFromG2()
x=a.mapToG1()
x=a.mapToG2()


b=Magnus.DirectProduct( fp1, fp2);
x=b.mapFromG1()
x=b.mapFromG2()
x=b.mapToG1()
x=b.mapToG2()


