%module Magnus

%{
#include "HavasTC.h"
%}

enum TC_STATE {NSTARTED, RUNNING, SUCCESS, NOMEM};

class HavasTC {
public:

  HavasTC(const FPGroup& group); 
  // Constructor for order of a group problem
  %name(HavasTCFromVector) HavasTC(const FPGroup& group,const VectorOf<Word>& subgroup);
  // Constructor for index of a subgroup problem


  ~HavasTC();
  /////////////////////////////////////////////////////////////////////// 
  //                                                                   //
  // Methods                                                           //
  //                                                                   //
  ///////////////////////////////////////////////////////////////////////

  void start();

  bool finished();

  int getIndex()const;

  TC_STATE getState() const;

 /// void printWord( ostream& ostr, const Word& w ) const;

  void setWorkspace(int w); 
/*
private:
  bool isInitSegment(Chars str, Chars seg ) const;
  
  int theIndex;
  BlackBox tc;
  FPGroup theGroup;
  VectorOf<Word> theSubgroup;

  TC_STATE theState;
  int theWorkspace;
*/
  %addmethods{
      	char* printWord(const Word& w) const {
		strstream *s=new strstream();
		self->printWord(*s,w);
		(*s) << '\0' << flush;
		return s->str();
	}
   }
};
































































