###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
a=Magnus.MatrixInteger(3)
b=Magnus.MatrixIntegerFromInt(2, 3)
a1=Magnus.MatrixRational(3)
b1=Magnus.MatrixRationalFromInt(2, 3)
a2=Magnus.Matrixdouble(3)
b2=Magnus.MatrixdoubleFromInt(2, 3)
c=a*b
c=a+b
c=a-b
###Inherited from DArray  
a!=b
#b[2][2]
#### MatrixRow< R >& operator [] ( int i ) { return change()->operator [](i); 
b.assignCol(3,[1,2,3]);
b.reSize(5,6)
b.colInsBefore(1,1)
b.colInsAfter(1,1)
b.rowInsBefore(1,1)
b.rowInsAfter(1,1)
b.colDelBefore(1,1)
b.colDelAfter(1,1)
b.colDelRange(1,1)
b.rowDelBefore(1,1)
b.rowDelAfter(1,1)
b.rowDelRange(1,1)
b.width( )
b.height( )
b.lastCol( )
b.lastRow( )
print b
#b.ReadFromString("3454efg")
