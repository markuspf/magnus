
%module Magnus

%{
#include "Trichotomy.h"
%}
//Trichotomy xxwtest1=dontknow;
%inline %{
    Trichotomy *new_NO(){
       return new Trichotomy(false);
    }
    Trichotomy *new_DONTKNOW(){
        return new Trichotomy(Trichotomy::dontknow());
    }

%}


///add defination of NO and DONTKNOW to Mangus.py manually.
class Trichotomy
{

public:

	///////////////////////////////////////////////////////
	//                                                   //
	//  Helper Classes                                   //
	//                                                   //
	///////////////////////////////////////////////////////

	enum TrichotomyValue { DONTKNOW = -1, NO, YES };

	class dontknow { }; // used as marker in ctor of global Trich consts

	// @stc it seems as though g++ 2.6.3 requires local classes declared before
	// first place of use in class definition

	///////////////////////////////////////////////////////
	//                                                   //
	//  Constructors                                     //
	//                                                   //
	///////////////////////////////////////////////////////
	%name(TrichotomyFromBool) Trichotomy( bool b );
	%name(TrichotomyFromVoid) Trichotomy( void* p );
//        %name(NO) Trichotomy(bool b=false);
//        %name(DONTKNOW) Trichotomy(const dontknow& value=xxwtest1);
//	%name(TrichotomyFromClass) Trichotomy( const dontknow& );


	// no default constructor

	// copy constructor provided by compiler

	///////////////////////////////////////////////////////
	//                                                   //
	//  Special Initialisation Constructors              //
	//                                                   //
	///////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//                                                   //
	//  Conversion Operators                             //
	//                                                   //
	///////////////////////////////////////////////////////
/*
#ifdef ALLOW_BOGUS_CONVERSIONS
	operator bool ( ) const
	{
		if (theValue == -1) error("Non-bool value of Trichotomy in"
			" Trichotomy::operator bool()");
		return bool(theValue);
	}

	operator int ( ) const
	{
		return bool(*this);
	}
#endif
*/

	///////////////////////////////////////////////////////
	//                                                   //
	//  Logical Operations                               //
	//                                                   //
	///////////////////////////////////////////////////////
/*
#ifdef ALLOW_BOGUS_CONVERSIONS
	Trichotomy operator ! ( ) const
	{
		if (undefined()) return dontknow();
		else return !bool(*this);
	}
  
	// the rest of these are implemented as global functions below
	// for dual type promotion

	///////////////////////////////////////////////////////
	//                                                   //
	//  Accessors                                        //
	//                                                   //
	///////////////////////////////////////////////////////

	bool defined( ) const { return theValue != -1; }
	// true iff *this != dontknow

	bool undefined( ) const { return !defined(); }
	// true iff *this == dontknow
#endif
*/

	///////////////////////////////////////////////////////
	//                                                   //
	//  IPC operators                                    //
	//                                                   //
	///////////////////////////////////////////////////////


    ///    friend ostream& operator << ( ostream& ostr, const Trichotomy& t )
       %addmethods{
             %name(YES) Trichotomy(){
                 return new Trichotomy(true);
             }

/*             %name(NO) Trichotomy(){
                 return new Trichotomy(false);
             }
             %name(DONTKNOW) Trichotomy(){
                 return new Trichotomy(Trichotomy:dontknow());
             }*/
             char* __str__() {
		strstream s;
		s << *self << '\0';
		return s.str();
             }

   ///	inline friend bool operator == ( const Trichotomy& t, const Trichotomy& u )
   ///inline bool operator != ( const Trichotomy& t, const Trichotomy& u )

           int __cmp__(const Trichotomy& w) {
         	// only makes sense for == and !=, not for > or <
		if ((*self)==w)
			return 0;
		return 1;
           }
         ///  operator int ( ) const
           int __int__(){
              return int(*self);
           }
           Trichotomy __and__(const Trichotomy& T) {
		return ((*self)&&T);
           }
           Trichotomy __or__(const Trichotomy& T) {
		return ((*self)&&T);
           }
           Trichotomy __not__() {
		return !(*self);
           }
        }

       // friend ostream& operator < ( ostream& ostr, const Trichotomy& t )

};


//------------------ related global operators ---------------------//
//extern const Trichotomy YES, NO, DONTKNOW, yes, no, dontknow;

/*
#ifdef ALLOW_BOGUS_CONVERSIONS
inline Trichotomy operator && ( const Trichotomy& t, const Trichotomy& u )
{
	if (t.undefined() || u.undefined()) return Trichotomy::dontknow();
	else return bool(t) && bool(u);
}

inline Trichotomy operator || ( const Trichotomy& t, const Trichotomy& u )
{
	if (t.undefined() || u.undefined()) return Trichotomy::dontknow();
	else return bool(t) || bool(u);
}
#endif
*/
