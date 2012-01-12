####This is a test firl for DArray
##it's all right
import Magnus
a=Magnus.DArrayInteger()
b=Magnus.DArrayIntegerFromInt(3,5)
c=Magnus.DArrayRationalFromInt(3,5)
d=Magnus.DArraydoubleFromInt(3,5) 
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
