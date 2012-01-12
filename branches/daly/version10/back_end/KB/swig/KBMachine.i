%module Magnus

%{
#include "KBMachine.h"
%}

class KBMachine {
// this should surely be derived from GroupDFSA but I can't get the
// constructor to work except by doing this
//  friend class RKBPackage;
public:
  KBMachine( );

  %name(KBMachineFromVector) KBMachine( const VectorOf<Chars> & genNames ) ;

  %name(KBMachineFromVectorWordOrder) KBMachine( const VectorOf<Chars> & genNames,const WordOrder & word_order ) ;

  %name(KBMachineFromVectorWordOrderInt) KBMachine( const VectorOf<Chars> & genNames,const WordOrder & word_order,
          int numOfRules);

  %name(KBMachineFromVectorWordOrderIntInt) KBMachine( const VectorOf<Chars> & genNames,const WordOrder & word_order,
        int numOfRules, int numOfStates);

//  void oldFormatPrintOn(ostream & str=cout) const;
  // for use with old code, which should one day be updated.

  Bool rewrite(Word & w) const;

  //Bool finiteLanguage() const { return look()->finiteLanguage(); }

  //Integer sizeLanguage() const { return look()->sizeLanguage(); }
/*
protected:
  KBMachine( KBMachineRep * rep ) : FSA((FSARep *)rep) { }
  // Need to shadow inherited look() and change(), to get right ptr types.
  const KBMachineRep *look() const { 
    return (KBMachineRep*)FSA::look(); }
  KBMachineRep *change() { 
    return (KBMachineRep*)FSA::change(); }
*/

  %addmethods{
//  void oldFormatPrintOn(ostream & str=cout) const;
    	char* oldFormatPrintOn() const {
		strstream *s=new strstream();
		self->oldFormatPrintOn(*s);
		(*s) << '\0' << flush;
		return s->str();
	}
  }
};

