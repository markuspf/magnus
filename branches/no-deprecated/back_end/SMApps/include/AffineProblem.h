// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Definition of class AffineProblem
//
// Principal Author: Nathan Penton
//
// Status: in progress
//
// Revision History:
//


#ifndef _AFFINEPROBLEM_H_
#define _AFFINEPROBLEM_H_

#include "Supervisor.h"
#include "Vector.h"
#include "Affine.h" 
#include "OutMessages.h"

class GrobnerIPC {
	public:
	void set(const vector<polynomial> &grobner_basis,int deg){grobner = grobner_basis;
						poly_deg = deg;}
	vector<polynomial> get_basis() const
	{ return grobner; }
	private:
	int poly_deg;
	vector<polynomial> grobner;
	friend ostream& operator < ( ostream& ostr, const GrobnerIPC &G);
	friend istream& operator > ( istream& istr, GrobnerIPC &G);
};

ostream& operator < ( ostream& ostr, const GrobnerIPC &G);
istream& operator > ( istream& istr, GrobnerIPC &G);

//---------------------------------------------------------------------------//
//------------------------- AffineProblem -----------------------------------//
//---------------------------------------------------------------------------//

class AffineARCer : public ARCer
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  AffineARCer( ComputationManager& );

  ~AffineARCer( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void setArguments( const SMFPGroup &G, int degree);

  int getDimension() const {return dimension;};
  vector<polynomial> getGrobnerBasis() const {return grobner_basis;};

  // Inherited from ARCer:
  // bool takeControl( );


  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IPC interaction members:                                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void runComputation( );

  void writeResults( ostream& );

  void readResults( istream& );

  
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  vector<polynomial> grobner_basis;
  FPGroup theGroup;
  GrobnerIPC grobner_ipc;
  int deg;
  int dimension;
};


class AffineProblem : public ComputationManager
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  AffineProblem( const class AffineSupervisor& );
   
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  bool isSolved() const { return solved; }

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Control:                                                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void takeControl( );         // overrides ComputationManager

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // State Transitions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void start( ) { }              // overrides ComputationManager

  void terminate( ) { }          // overrides ComputationManager

  
private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMFPGroup& theGroup;
  int theDimension;
  int theDegree;
  AffineARCer AffineArcer;
  bool arcerStarted;
  bool solved;
};





class AffineSupervisor : public Supervisor
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  AffineSupervisor(class SMFPGroup& G, int d = 2 );

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Accessors:                                                          //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMFPGroup& group() const { return theGroup; }

  int dimension( ) const { return theDimension; }
 
  int degree( ) const { return theDegree; }

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void viewStructure(ostream& ostr) const;  // overrides SMObject

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Control:                                                            //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void takeControl( );         // overrides ComputationManager

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // State Transitions:                                                  //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  void start( ) { }           // overrides ComputationManager

  void terminate( ) { }       // overrides ComputationManager


private:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Data Members:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  SMFPGroup& theGroup;
  int theDimension;
  int theDegree;

  Subordinate<AffineSupervisor, AffineProblem> affine;
};

#endif


