///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
#include "Letter.h"
%}
struct Letter {

  // constructors

  Letter() ;
  // Default constructor makes fictive letter

  %name(LetterFromGenerator) Letter(Generator g, int pow = 1) ;

  %name(LetterFromInt) Letter(int g, int pow = 1) ;

  // collects letters
  void collectWith (Generator g) ;

  // conversion to Word

 // void printOn(ostream& os) const ;

  void invert() ;
  // Inverts the letter. Do not confuse with inv() global function.

  int shortLexIndex() const;
  // Returns generator's ShortLex index. Does not work with power==0.
/*
  // IPC write
  friend ostream& operator < (ostream& os, const Letter& let) {
    os < let.gen < let.power;
  }

  // IPC read
  friend istream& operator > (istream& is, Letter& let) {
    is > let.gen > let.power;
  }
*/
  // Data
  Generator gen;
  int power;
   %addmethods{
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}

	int __cmp__(const Letter& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
///  operator Word() const ;
        Word Word(){
           Word w;
           w=Word(*self);
           return w;
        }
 // void printOn(ostream& os) const ;
      	char* printOn() const {
		strstream *s=new strstream();
		self->printOn(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
  }

};

// Global functions
// inversion
///Letter inv( const Letter& let) ;
///use invert




