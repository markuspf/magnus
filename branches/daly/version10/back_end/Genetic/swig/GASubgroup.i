///created by xiaowei xu
////xuxw76@yahoo.com
///refer to word.i to see how swig works
%module Magnus
%{
  #include "GASubgroup.h"
%}

class GASubgroup
{

public:
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  GASubgroup( );
  
  %name(GASubgroupFromSet) GASubgroup( const SetOf<Word>& generators );
 
  // copy constructor supplied by compiler.
  
  // destructor supplied by compiler.


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SetOf<Word> generators( ) const;

  int fitness( const GASubgroup& S ) const;

  GASubgroup mutate( ) const;

  GASubgroup crossover( const GASubgroup& S ) const;

  GASubgroup randomSubgroup( ) const;
      

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // OI:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  /*
  friend ostream& operator << ( ostream& ostr, const GASubgroup& S )
  {
    ostr << S.gens << endl;
    return ostr;
  }

  
  friend istream& operator >> ( istream& istr, GHNConfig& C )
  {
    C.readFrom(istr);
    return istr;
  }
  */
   %addmethods{
	char* __str__() {
		strstream s;
		s << (*self) << '\0' << flush;
		return s.str();
	}

  }
/*
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Private functions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  Word randomWord( ) const;
  
  int randomGen( ) const;

  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  
  SetOf<Word> gens; // subgroup generators
  static int maxCard;
  static int maxWordLen;
  static int numOfGens;
  static UniformRandom r;
*/
};

