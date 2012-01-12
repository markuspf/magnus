%module Magnus

%{
#include "Elt.h"
%}

class Elt{
public:

///////////////////////////////////////////////////////////////////////
//                                                                   //
// Constructors:                                                     //
//                                                                   //
///////////////////////////////////////////////////////////////////////

  // copy constructor provided by compiler

  Elt( );
  // @stc this is a tentative way of giving Elt a default constructor

  // destructor provided by compiler

///////////////////////////////////////////////////////////////////////
//                                                                   //
// Basic operators:                                                  //
//                                                                   //
///////////////////////////////////////////////////////////////////////

//  int operator == ( const Elt& e ) const ;

//  int operator != ( const Elt& e ) const ;
///////////////////////////////////////////////////////////////////////
//                                                                   //
// Representation and type stuff:                                    //
//                                                                   //
///////////////////////////////////////////////////////////////////////

  Type type( );

  Type actualType( ) const ;

  int hash( ) const ;

///////////////////////////////////////////////////////////////////////
//                                                                   //
// Mathematical methods:                                             //
//                                                                   //
///////////////////////////////////////////////////////////////////////

//  Elt operator * ( const Elt& e ) const
//  Elt operator *= ( const Elt& e ) 

  Elt raiseToPower( Integer power ) const;

  Elt inverse() const ;

//  friend Elt inverse( const Elt& e );
  // to make it more convenient to take the inverse of something which
  // can be converted to an Elt

  Elt conjugateBy( const Elt& e ) const ;

//  friend Elt commutator( const Elt&, const Elt& );

  // The paradigm for output:
  // The Rep of the root of a hierarchy defines
  // virtual void printOn(ostream&) const;
  // The root defines a friend << which
  // calls printOn. Subclasses of Rep override printOn.
  // No other << or printOn's are needed.

//  friend ostream& operator << ( ostream&, const Elt& );

//  void debugPrint( ostream& o ) const; // mixed pseudo-virtual
  %addmethods{
      	char* debugPrint() const {
		strstream *s=new strstream();
		self->debugPrint(*s);
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

  friend ostream& operator < ( ostream& ostr, const Elt& e )
  {
    e.look()->write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, Elt& e )
  {
    e.change()->read(istr);
    return istr;
  }


protected:

  // Shadow representation accessors to get representations of the
  // right type in the members of this class:

  const EltRep* look( ) const {
	 return (const EltRep*)GenericObject::look();
  }
  EltRep* enhance( ) const {
	 return (EltRep*)GenericObject::enhance();
  }
  EltRep* change( ) {
	 return (EltRep*)GenericObject::change();
  }

  Elt( EltRep* p ) : GenericObject(p) { }
*/
   %addmethods {

      //  int operator == ( const Elt& e ) const;
      //  int operator != ( const Elt& e ) const;
	int __cmp__(const Elt& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
      //  Elt operator * ( const Elt& e ) const;
	Elt __mul__(const Elt& w) {
		return (*self)*w;
	}
      //  Elt inverse() const;

      //  friend Elt inverse( const Elt& e );

	Elt __invert__() {
		return self->inverse();
	}
      ///  friend ostream& operator << ( ostream&, const Elt& );
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}

   }
}

//  friend Elt commutator( const Elt&, const Elt& );
%name(EltCommutator) Elt commutator( const Elt&, const Elt& );
