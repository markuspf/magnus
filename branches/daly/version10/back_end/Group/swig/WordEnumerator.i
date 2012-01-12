%module Magnus

%{
#include "WordEnumerator.h"
%}

class WordEnumerator
{
public:
  WordEnumerator( const FreeGroup& f, int n = 2);
  
  void reset(int c = 0);
  void next();
  
  VectorOf<Word> getWords();
/*    
private:
  FreeGroup theGroup;
  VectorEnumerator ve;
*/
};
