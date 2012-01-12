%module Magnus

%{
#include "Products.h"
%} 
class FreeProduct
{
public:
  enum CreatError { NONE, DOUBLE_GENS };
  FreeProduct( const FPGroup& g1, const FPGroup& g2);

  // Maps
  virtual Map mapFromG1() const;
  virtual Map mapFromG2() const;
  virtual Map mapToG1() const;
  virtual Map mapToG2() const;
  
  //  CreateError getCreateStatus() const { return theStatus; }

//  operator FPGroup( ) const;
/*
protected:
  void createGenerators();
  void createRelators();

  VectorOf<Chars> theGenerators;
  SetOf<Word> theRelators;

  FPGroup G1;
  FPGroup G2;

  // Creation status
  CreatError theStatus;
private:
  FreeProduct( const FreeProduct&);
  FreeProduct& operator = ( const FreeProduct& );
  bool gensInitialized;
*/
};


class DirectProduct
{
 public:
   DirectProduct( const FPGroup& g1, const FPGroup& g2);
  //inherited from FreeProduct
  // Maps
  virtual Map mapFromG1() const;
  virtual Map mapFromG2() const;
  virtual Map mapToG1() const;
  virtual Map mapToG2() const;
  
  //  CreateError getCreateStatus() const { return theStatus; }

//  operator FPGroup( ) const;

/*         
 private:
  DirectProduct( const DirectProduct&);
  DirectProduct& operator = ( const DirectProduct& );
*/
};


