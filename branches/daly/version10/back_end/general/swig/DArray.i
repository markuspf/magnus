///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "DArray.h"
%}
%define LIST(DARRAY,TYPE)
%{
typedef DArray<TYPE> DARRAY;
%}

class DARRAY{

public:


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  DARRAY( int n = 0 );
  // Default constructor makes empty square DArray  of size n.

  %name(DARRAY ## FromInt) DARRAY( int h, int w);
  // constructor makes DArray of size h x w.

  // Copy constructor, operator=, and destructor supplied by compiler.
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Operators:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

//  bool operator == ( const DArray& DA ) const;

//  VectorOf<R> operator []( int i ) const { return look()->getRow(i); }

///  MatrixRow< R >& operator [] ( int i ) { return change()->operator [](i); 

   %addmethods{
    	int __cmp__(const DARRAY& w){

		if ((*self)==w)
			return 0;
		return 1;
	}
	VectorOf<TYPE> __getitem__(int i) {
		return (*self)[i];
	}
   }

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  VectorOf<TYPE> getRow( int i ) const ;
  // Returns  i's row

  VectorOf<TYPE> getCol( int i ) const ;
  // Returns  i's column
 
  void assignCol( int i, const VectorOf<TYPE>& vc );
  // Assigns i's column with vc.

 // Changes height and width. Puts data in up left corner.
  void reSize(int newHeight,int newWidth);

 // Functions below are inserts, corresponding to function names, before(after) 'col'
 // 'colsNum' columns(rows). 
  void colInsBefore(int col,int colsNum);
  void colInsAfter(int col,int colsNum);
  void rowInsBefore(int row,int rowsNum);
  void rowInsAfter(int row,int rowsNum);

 // These methods deletes number of rows(columns) before, after or between
 // the columns. If reAlloc = true - memory realocates for array, if not
 // the bounds are changed, but fisicaly memory does not realocating.
  void colDelBefore(int col,int colsNum,bool reAlloc = TRUE);
  void colDelAfter(int col,int colsNum, bool reAlloc =TRUE);
  void colDelRange(int col1,int col2, bool reAlloc =TRUE);
  void rowDelBefore(int row,int rowsNum,bool reAlloc = TRUE);
  void rowDelAfter(int row,int rowsNum, bool reAlloc =TRUE);
  void rowDelRange(int row1,int row2, bool reAlloc =TRUE);

  int width( ) const;
    
  int height( ) const ;

  // Index begins from 0, so the last column or row number differs from the 
  // height or width
  int lastCol( ) const ;
    
  int lastRow( ) const;
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // I/O:                                                                //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
/*
  friend IStreamPoll operator >> ( istream& istr, DArray& M );
 
  inline friend ostream& operator << ( ostream& o, const DArray& v );
*/

   %addmethods{
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
        void ReadFromString(istream& istr){
              istr >> (*self);
        }
  }

/*
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < <R>( ostream& ostr, const DArray& DA );
  
  friend istream& operator > <R>( istream& istr, DArray& DA);


  protected :
	DArray( DArrayRep<R>* newrep ) : ObjectOf<DArrayRep <R> >(newrep) { }
*/

};

%enddef

LIST(DArrayInteger,Integer)
LIST(DArrayRational,Rational)
LIST(DArraydouble,double)