

%module Magnus

%{

#include "TurnerProperSubgroupEnumerator.h"

%}

class ProperSubgroupEnumerator
{
public:
  ProperSubgroupEnumerator(const FreeGroup& F, const VectorOf<Word>& words);
//undefined  %name(ProperSubgroupEnumeratorFromWord) ProperSubgroupEnumerator(const FreeGroup& F, const Word& word);

 ~ProperSubgroupEnumerator();

//undefined  %name(ProperSubgroupEnumeratorFromProperSubgroupEnumerator) ProperSubgroupEnumerator(const ProperSubgroupEnumerator& PSE);
//  int operator == (const ProperSubgroupEnumerator& PSE);
  // hidden

  bool getNextProperSubgroup(SGofFreeGroup& sg);

  Chars getFileName() ;
/*//undefined  %addmethods{
	int __cmp__(const ProperSubgroupEnumerator& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
  }
*/
/*
private:
  const VectorOf<Word> theWords;
  const FreeGroup theGroup;

  VectorOf<Word> genOfGroup;
  // generators of free group

  const int rank;
  // rank of the FreeGroup

  bool stepTo();
  void stepBack();
  bool setRefs();
  void buildSG( );
  Word getWord(int x, int y);

  File file;
  // debug file

  SGofFreeGroup *subgroup;
  // current subgroup
  VectorOf<Word> genOfSG;
  // generators of current subgroup
  int qWord;
  // generator quantity


  int nVert;
  // num of current vertice
  int nSet;
  // num of current set

  int *partition;
  int **_partition;
  // partition 

  int qSet;
  // set quantity 
  int qVert;
  // vertice quantity

  int *setToVert;
  int *vertToWord;
  int *vertToPos;

  struct LC {
    int qSet;
  }* lCells;

  enum { UNKNOWN=-1 };
*/
};


