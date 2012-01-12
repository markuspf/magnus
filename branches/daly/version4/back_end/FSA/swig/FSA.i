///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "FSA.h"
%}

class FSA {
public:
  Bool accepts(Word w) const ;
  Bool rejectsInState(Word w, int& state) const ;
  Bool nextAcceptedWord(Word& w) const ;
//Bool nextAcceptedWord(Word w, int*& history) const = 0; // bad arg
  void minimize() ;
  Integer sizeLanguage() const ;
  Bool finiteLanguage() const ;
  
  void readFrom(istream &str = cin) ;
//  void printOn(ostream &str = cout) const ;
  void setName(const Chars & name) ;
  int getNumStates() const ;

//  int operator == ( const FSA & F ) const;

//  int operator != ( const FSA & F ) const ;

  %addmethods {
        FSA(){
        }
	int __cmp__(const FSA& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
      	char* printOn() const {
		strstream *s=new strstream();
		self->printOn(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
   }
  
/*
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const FSA& fsa )
  {
    fsa.look()->write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, FSA& fsa )
  {
    fsa.change()->read(istr);
    return istr;
  }


protected:
  typedef ObjectOf<FSARep> R;
  FSA( FSARep *p ) : R(p) {}
*/
};

