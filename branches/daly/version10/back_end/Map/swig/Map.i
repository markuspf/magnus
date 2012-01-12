%module Magnus

%{
#include "Map.h"
%}

class Map{

public:

  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // Constructors                                                   //
  //                                                                //
  ////////////////////////////////////////////////////////////////////

  // @stc note the complete lack of consistency checks in the
  // constructors

  // no default constructor: domain and range groups must be given

  Map( );
  // copy constructor supplied by compiler

  %name(MapFromFGGroups) Map( const FGGroup& domain,
       const FGGroup& range );
  // to construct a map with unspecified images;
  // by default, becomes the trivial map, i.e.
  // the map mapping everything to the identity;
  // Word's default constructor takes care of initialising
  // the generating images to the identity.

  %name(MapFromFGGroupsAndVector) Map( const FGGroup& domain,
       const FGGroup& range,
       const VectorOf<Word>& generatingImages );
  // to construct a map, given a domain and range group, and images
  // for the generators of the domain
  // @stc this should do some fancy stuff to deal with cases when
  // too few or too many generating images are given (both could be
  // legal with appropriate semantics)

  // destructor supplied by compiler

  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // Standard Operators                                             //
  //                                                                //
  ////////////////////////////////////////////////////////////////////

//  bool operator == ( const Map& m ) const ;

  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // Accessors / Modifiers                                          //
  //                                                                //
  ////////////////////////////////////////////////////////////////////

  Type type( );

  // @stc it is assumed that domain, range and generatingImages
  // in their entirety are modified infrequently, hence seperate
  // members are provided to (re)set them
  // individual generatingImages might be modified more often;
  // hence, for convenience, an intermediate assignable result is
  // constructed when taking the image of a generator

  const FGGroup& domain( ) const;
  // to poll the domain of definition of the map
  // not for modification

  const FGGroup& range( ) const;
  // to poll the range of definition of the map
  // not for modification

  void setDomain( const FGGroup& g );
  // to modify domain group; if the number of generators of g is less
  // than the number of stored generating images, the extra ones are
  // thrown away; if it is greater, the additional generating images
  // are uninitialised
  // @stc more fancy facilities should be provided to specify how one
  // wants to reuse the old images.

  void setRange( const FGGroup& g );
  // to set of modify range group
  // @stc this should check the images for validity

  const VectorOf<Word>& generatingImages( ) const;
  // to poll the defining images of the map
  // not for modification

  %name(generatingImagesFromInt) Word generatingImages( int i ) const;
  // to poll the i'th defining image (0-based indexing)
  // not for modification
  // @stc this cannot be made to return a const ref, as
  // theGeneratingImages[i] is not an lvalue and this would be a
  // a dangling reference
  // when changing the MapRep this could change

  %name(generatingImagesFromGenerator) Word generatingImages( const Generator& g ) const;
  void setGeneratingImages( const VectorOf<Word> gi );
  // to set or modify generating images

  %name(setGeneratingImagesFromWord) void setGeneratingImages( int i, const Word& e  );
  // to assign to the i-th (0-based) generating image

  %name(setGeneratingImagesFromGenerator) void setGeneratingImages( const Generator& g, const Word& w );
	
  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // Mapping methods                                                //
  //                                                                //
  ////////////////////////////////////////////////////////////////////

  // computing images:
  // formally:

  Word imageOf( const Word& w ) const;
  // takes a formal word in the generators of domain and evaluates its
  // `image' in range; if the images are defined via formal words,
  // then the result is also a formal word.
  // @stc this bears discussion -- should a seperate image evaluater
  // also be provided?

  %name(imageOfFromGenerator) Word imageOf( const Generator& g ) const;
  // image of generator g; not an lvalue

  // computing images:
  // evaluated:

  Elt evalImage( const Word& w ) const;
  // to compute the image, using the range's operations

  Elt postEvalImage( const Word& w ) const ;
  // to compute an image formally, the evaluate it in the range

  // operations on maps:
//  friend Map composition( const Map& secondMap,
		//	  const Map& firstMap );

   
  // map-theoretic composition

  Map& composeAfter( const Map& firstMap );
  // equivalent to as *this = composition(*this,firstMap);
  // @stc tricky one to name: not to confuse the order of composition

//  Map operator | ( const Map& secondMap );
  
  // @stc this is a tentative operator abbreviation for
  // composition -- its intuitive iterpretation should be a pipe,
  // hence composition is left to right; its advantage is to permit
  // syntactically light concatenation: fcomp = f1 | f2 | f3;
  // In the same line of thought an Word | Map is provided below
  // @stc not a global to avoid most kinds of unintended conversions

  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // Mapping properties                                             //
  //                                                                //
  ////////////////////////////////////////////////////////////////////

  // @stc values cast to Trichotomy before returning to keep warnings
  // silent

  Trichotomy extendsToHom( ) const;

  void putIsMono( ) const;

  Trichotomy isMono( ) const;

  void putIsEpi( ) const;
 
  Trichotomy isEpi( ) const;
 
  Trichotomy isIso( ) const;
  Trichotomy isTrivialMap( ) const; // @stc impl. tmp.

  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // I/O                                                            //
  //                                                                //
  ////////////////////////////////////////////////////////////////////

//  void printOn(ostream& ostr) const;
 
  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // Debugging stuff                                                //
  //                                                                //
  ////////////////////////////////////////////////////////////////////
 
#ifdef DEBUG

  //friend int main( );

  bool consistent( ) ;

#endif

  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // I/O                                                            //
  //                                                                //
  ////////////////////////////////////////////////////////////////////

//  friend IStreamPoll operator >> ( istream&, Map& );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
/*
  friend ostream& operator < ( ostream& ostr, const Map& M )
  {
    M.look()->write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, Map& M)
  {
    M.change()->read(istr);
    return istr;
  }


  ////////////////////////////////////////////////////////////////////
  //                                                                //
  // Representation access methods                                  //
  //                                                                //
  ////////////////////////////////////////////////////////////////////
 
protected:
 
  // Special wrapping constructor to wrap new representations (returned
  // by eg. delegated methods) and for base initialisation by derived
  // classes:
 
  Map( MapRep* newrep ) : DerivedObjectOf<GenericObject,MapRep>(newrep) { }
*/
   %addmethods {
//  bool operator == ( const Map& m ) const ;
	int __cmp__(const Map& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
///ostream& operator << ( ostream&, const Map& );
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
 // void printOn(ostream& ostr) const;
        char* printOn() const {
		strstream *s=new strstream();
		self->printOn(*s);
		(*s) << '\0' << flush;
		return s->str(); 
       }
   }
};

//  friend IStreamPoll operator >> ( istream&, Map& );
%inline %{
	Map * MapFromString(char* p) {
		strstream s;
		Map* F=new Map();
		s << p << '\0' << flush;
		Chars msg=s >> *F;
		if (msg.length()>0) {
			PyErr_SetString(PyExc_Exception,msg);
			PyErr_Print();
			return NULL;
		}
		return F;
	}
%}

//------------------ Related global functions --------------------//


