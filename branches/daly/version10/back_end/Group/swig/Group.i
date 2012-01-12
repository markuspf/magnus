%module Magnus

%{
#include "Group.h"
%}


class Group {
  
public:

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Constructors                                     //
  //                                                   //
  ///////////////////////////////////////////////////////

  // No default constructor for pseudo-abstract class.
  // Copy constructor and destructor supplied by compiler.
  
  ///////////////////////////////////////////////////////
  //                                                   //
  //  Operators                                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  // Operator= supplied by compiler.


  ///////////////////////////////////////////////////////
  //                                                   //
  //  Accessors                                        //
  //                                                   //
  ///////////////////////////////////////////////////////

  Type type( );

  Type actualType( ) const;

  ///////////////////////////////////////////////////////
  //                                                   //
  //  Group structure methods                          //
  //                                                   //
  ///////////////////////////////////////////////////////

  int order( ) const; 
  // Returns the order of this group, or 0 if this is infinite,
  // or -1 if no answer can be determined.

  Trichotomy isTrivial( ) const ;
  // pseudo-virtual

  Trichotomy isFinite( ) const ;
  // pseudo-virtual

  Trichotomy isInfinite( ) const ;
  // pseudo-virtual

  Trichotomy isAbelian( ) const ;
  // pseudo-virtual


  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with group elements           //
  //                                                   //
  ///////////////////////////////////////////////////////


  Elt makeIdentity( ) const ;
  // Returns an object which this group treats as a
  // syntactic representation of its identity element.

  Bool isSyntacticIdentity(const Elt& e) const ;
  // Says whether this group considers e to be a syntactic representation
  // of its identity element. This does not try to solve the word problem,
  // or apply such a solution if it exists.

  Trichotomy isTrivialElt( const Elt& e ) const;
  // Attempts to determine whether e is or represents the identity in
  // this group

  Trichotomy areEqual(const Elt& e1, const Elt& e2) const ;
  // Tries to decide whether e1 and e2 represent the same element
  // of this group.

  Elt firstElt( ) const ;
  // Returns the first Elt in a fixed, pre-determined sequence of Elts
  // which maps onto, but not necessarily 1-1, the set of this group's
  // elements.

  Elt nextElt(const Elt& e) const ;
  // Returns the element which comes after e in the sequence described
  // above. Guaranteed defined only on Elts returned by firstElt or
  // nextElt, since the sequence may not contain all Elts.

  Elt multiply(const Elt& e1, const Elt& e2) const ;
  // Returns a representation of the product of e1 and e2 in this group.

  Elt inverseOf(const Elt& e) const ;
  // Returns a representation of the inverse of e in this group.

  Elt raiseToPower(const Elt& e, int n) const ;
  // Returns a representation of e^n in this group, for any int n.

  Elt conjugateBy(const Elt& e1, const Elt& e2) const ;
  // Returns a representation of e2^-1 e1 e2 in this group.

  Elt commutator(const Elt& e1, const Elt& e2) const ;
  // Returns a representation of e1^-1 e2^-1 e1 e2 in this group.


  ///////////////////////////////////////////////////////
  //                                                   //
  //  Methods which deal with sets of group elements   //
  //                                                   //
  ///////////////////////////////////////////////////////


  %name(setMultiplyFromSetSet) SetOf<Elt> setMultiply(const SetOf<Elt>& S1, const SetOf<Elt>& S2) const;
  %name(setMultiplyFromEltSet) SetOf<Elt> setMultiply(const Elt& e, const SetOf<Elt>& S) const;
  %name(setMultiplyFromSetElt) SetOf<Elt> setMultiply(const SetOf<Elt>& S, const Elt& e) const;
  // These return elementwise products, multiplying in the order of the args.


  %name(conjugateByFromSetSet) SetOf<Elt> conjugateBy(const SetOf<Elt>& S1, const SetOf<Elt>& S2) const;
  %name(conjugateByFromEltSet) SetOf<Elt> conjugateBy(const Elt& e, const SetOf<Elt>& S) const;
  %name(conjugateByFromSetElt) SetOf<Elt> conjugateBy(const SetOf<Elt>& S, const Elt& e) const;
  // By convention, conjugateBy(S1, S2) = { s2^-1 s1 s2 : s_i in S_i }.

  void closeUnderInverses(SetOf<Elt>& S) const;
  // Alters S so that it is closed under (group element) inverses.
   

  ///////////////////////////////////////////////////////
  //                                                   //
  //  I/O                                              //
  //                                                   //
  ///////////////////////////////////////////////////////


  // @stc outdated: (valid only for ostream & istream)
  // The paradigm for I/O:
  //
  // The Rep of the root of a hierarchy H defines
  //
  // virtual void printOn(ostream&) const;
  // virtual Rep* readFrom(istream&, Chars&) const;
  //
  // The root defines
  //
  // friend ostream& operator << ( ostream&, const H& );
  // friend IStreamPoll operator >> ( istream&, H& );
  //
  // which delegate to printOn and readFrom, resp.
  // Subclasses of Rep override printOn/readFrom.
  // readFrom returns NULL when there is a parse error;
  // in this case, >> does not change H.

  // the old i/o way :
/*
  friend ostream& operator << ( ostream&, const Group& );
  
  friend IStreamPoll operator >> ( istream&, Group& );

 /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC tools:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend ostream& operator < ( ostream& ostr, const Group& G )
  {
    G.look()->write(ostr);
    return ostr;
  }
  
  friend istream& operator > ( istream& istr, Group& G )
  {
    G.change()->read(istr);
    return istr;
  }



  ///////////////////////////////////////////////////////
  //                                                   //
  //  Representation access methods                    //
  //                                                   //
  ///////////////////////////////////////////////////////


protected:

  // Shadow representation accessors to get representations of the
  // right type in the members of this class:

  const GroupRep* look( ) const {
	 return (const GroupRep*)GenericObject::look();
  }
  GroupRep* enhance( ) const {
	 return (GroupRep*)GenericObject::enhance();
  }
  GroupRep* change( ) {
	 return (GroupRep*)GenericObject::change();
  }

  // Special wrapping constructor to wrap new representations (returned
  // by eg. delegated methods) and for base initialisation by derived
  // classes:

  Group( GroupRep* newrep ) : GenericObject(newrep) { }


  ///////////////////////////////////////////////////////
  //                                                   //
  //  Debugging stuff                                  //
  //                                                   //
  ///////////////////////////////////////////////////////

#ifdef DEBUG

private:
  
  //friend int main( );
  friend void debugPrint(ostream&, const Group& g);

public:

  bool consistent( ) const { return look()->consistent(); } // pseudo-virtual

#endif
*/
  
    %addmethods {
        Group(){
        }
       //int operator == ( const Group& G ) const;
       // int operator != ( const Group& G ) const;
	int __cmp__(const Group& w) {
	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
	}
 
      ///   friend ostream& operator << ( ostream&, const Group& );
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}
 ////         friend IStreamPoll operator >> ( istream&, Group& );
         void ReadMeFrom(istream& i){
            i>>*self;
         }
   }
};

