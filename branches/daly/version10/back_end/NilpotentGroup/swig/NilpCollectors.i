%module Magnus

%{
#include "NilpCollectors.h"
%}


class CollectorToTheLeft:public NilpotentCollector
{

public:

  //------------------------------------------------------------------------
  //   Constructors and initializers
  //------------------------------------------------------------------------

  CollectorToTheLeft(int numgen, int nilclass, bool initialize = true);

  %name(CollectorToTheLeftFromBasicCommutators) CollectorToTheLeft(const BasicCommutators& bc);

  
  //copy constructor, destructor and assignment operator provided by compiler

//  virtual NilpotentCollector::Type type() const ;
  // the collector type

//  virtual void collectingProcess( PolyWord& pw ) const;
  // collects the given PolyWord. The result is stored in the same PolyWord.

//  virtual NilpotentCollector * clone() const ;

  // method to clone the object

};



//==========================================================================
//=================== class CollectorFromTheLeft =============================
//==========================================================================

class CollectorFromTheLeft :public NilpotentCollector
{

public:

  //------------------------------------------------------------------------
  //   Constructors and initializers
  //------------------------------------------------------------------------

  CollectorFromTheLeft(int numgen, int nilclass, bool initialize = true);
  %name(CollectorFromTheLeftFromBasicCommutators) CollectorFromTheLeft(const BasicCommutators& bc);

  
  //copy constructor, destructor and assignment operator provided by compiler

//  virtual NilpotentCollector::Type type() const ;
  // the collector type

//  virtual void collectingProcess( PolyWord& pw ) const;
  // collects the given PolyWord. The result is stored in the same PolyWord.

//  virtual NilpotentCollector * clone() const ;
};




