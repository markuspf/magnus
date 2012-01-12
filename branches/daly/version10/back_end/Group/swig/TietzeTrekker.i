%module Magnus

%{
#include "TietzeTrekker.h"
%}

class TietzeTrekker {

public:

  TietzeTrekker(const FPGroup& G);
  // Fire up Frank Rimlinger's TietzeTrek with the presentation of G.

  %name(TietzeTrekkerFromVector) TietzeTrekker(const VectorOf<Chars>& genNames, const SetOf<Word>& relators);
  // Fire up Frank Rimlinger's TietzeTrek with the given `presentation'.

  //@rn Need a destructor, which tells TietzeTrek to quit.

  Bool sanityCheck( );
  // Says whether this TietzeTrekker thinks its initialization was successful.

  Bool getFactoid(Bool queuePresentations, long giveUpLimit = 100);
  // The first argument says whether to save any presentations
  // which are found before the next factoid. Saved ones are later returned
  // by getPresentation().
  // The second argument specifies the number of lines to read from the
  // BlackBox in search of a factoid before giving up and returning.
  // The return value says whether a new factoid was discovered.

  // The following indicate what facts have been discovered so far by 0
  // or more calls to getFactoid (FALSE means "don't know (yet)"):

  Bool knownToBeTrivial( )  ;
  Bool knownToBeCyclic( ) ;
  Bool knownToHaveFreeFactor( );
  Bool knownToBeFinite( ) ;
  Bool knownToBeFree( );

  int getOrder( );
  // Returns the order of the group in `standard' format: -1 == DONTKNOW,
  // 0 == INFINITE, otherwise actual order. If the answer is -1, you can
  // call getFactoid (again) for another throw of the dice.

  int getRank( );
  // Returns the rank of the group AFTER knownToBeFree() has returned TRUE.
  // It is a fatal error if this is called when !knownToBeFree().

  FPGroup getPresentation( );
  // Returns another presentation of the finitely presented group
  // (representation) used to instantiate this TietzeTrekker, in the form
  // of a FPGroup.

/*
private:

  enum { TRIVIAL = 1, CYCLIC = 2, HAS_FREE_FACTOR = 4, FINITE = 8, FREE = 16 };

  // Data members;

  ListOf<FPGroup> presentations;
  // Presentations queued by getFactoid.

  BlackBox TietzeTrek;  // The BlackBox we're wrapping.

  int flags;            // Flag what we know with values from above enum.
  int order;            // Store order if we know it.
  int rank;             // Store free rank if we know it.

  int status;           // Initialization status: 1 == good, 0 == bad.


  // Private methods:

  void initialize(const VectorOf<Chars>& genNames, const SetOf<Word>& relators);
  // Does the actual work of construction.

  void printWord(ostream& ostr, const Word& w);
  // A special word printer, to work around the ineptitudes in Frank's API.

  void printGenerator(ostream& ostr, int g);
  // Prints g as [a-zA-Z].
*/
};
