// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
//
// Contents: Declarations of helper classes for specifying menus
//
// Principal Author: Roger Needham
//
// Status: in progress
//
// Revision History:
//
// * 10/11/96 EP added stuff for SMVectorOfWords
//
//  96/10/09 @dp added struct ReadFreeGetNextN_thElementItem.
//  96/10/10 @dp added struct ReadTerminalSegmentItem, ReadSegmentOfWordItem.
//  96/10/10 @dp added struct DefineInt2, BoundedInteger.
//


#ifndef _MENU_H_
#define _MENU_H_


#include "FEData.h"
#include "SMObject.h"

//@rn These includes are required by the gcc 2.6.3 bug workaround,
//@rn discussed below:

#include "SMBaseOfDataSet.h"
#include "ConstructSampleFromDataSet.h"
#include "SMDataSetComputation.h"
#include "SMDataSetComputationProblems.h"
#include "SMDataSetTVComputationProblems.h"
#include "SMRandomDataSet.h"
#include "SMCrossDatasetProblems.h"

//---------------------------------------------------------------------------//
//------------------------------ Ctor<n> ------------------------------------//
//---------------------------------------------------------------------------//

// A programmer may specify an end user menu item by supplying a
// `constructor wrapper', i.e., an appropriate derivative of class Ctor.
// These arrange, in cooperation with class ObjectSmith, for a callback
// when the menu item is selected. The callback reads the arguments of
// the item, and creates the required derivative of SMObject. The
// constructor wrapper does strong compile-time type checking of the
// arguments against the SMObject to be created.

// Templates cannot be class members (think about it), so these have
// to hang out in the global namespace.

struct Ctor
{
  virtual void readFrom(istream& istr) const = 0;
  // This does all input, and construction and storage of objects.

  virtual Ctor* copy( ) const = 0;
  // Used for storing an object in a derived class as a Ctor*.
  // This is necessary in order to keep the right virtual table.

protected:

  // With these here, only Ctor need be a friend of TheObjects,
  // SMObject, resp.:

  SMObject* get(int oid) const;
  // Returns the value of TheObjects::get( oid ).

  void newDependent(SMObject* smo, OID oid) const;
  // Tells smo that oid depends on it.
};


// The classes CtorAux<n> are not strictly necessary; they factor
// common code out of the Ctor<n> templates, to mitigate the effects of
// combinatorial explosion.
// The `readFrom' member knows how many oid's to read. It gets the
// corresponding SMObject*'s from TheObjects, and passes them to
// `construct' for the actual construction and storage of the new object.
// In special cases, e.g., when a parse error is possible, `construct' may
// return 0.


struct CtorAux0 : public Ctor
{
  void readFrom(istream& istr) const;            // overrides Ctor

  virtual SMObject* construct(Chars&, istream&) const = 0;
};


struct CtorAux1 : public Ctor
{
  void readFrom(istream& istr) const;            // overrides Ctor

  virtual SMObject* construct(SMObject*, Chars&, istream&) const = 0;
};


struct CtorAux2 : public Ctor
{
  CtorAux2(bool transpose = false) : transposeARGS( transpose ) { }

  void readFrom(istream& istr) const;            // overrides Ctor

  virtual SMObject* construct(SMObject*, SMObject*, Chars&, istream&) const = 0;

  bool transposeARGS;
  // Marks whether to reverse the order of the arguments.
};


struct CtorAux3 : public Ctor
{
  void readFrom(istream& istr) const;            // overrides Ctor

  virtual SMObject* construct(SMObject*, SMObject*, SMObject*, 
			      Chars&, istream&) const = 0;
};


struct CtorAux4 : public Ctor
{
  void readFrom(istream& istr) const;            // overrides Ctor

  virtual SMObject* construct(SMObject*, SMObject*, SMObject*, SMObject*, 
			      Chars&, istream&) const = 0;
};


// The following are the Ctor's to be passed as arguments to the Menu
// objects. Their purpose is to do the casting from mere SMObject*'s to
// objects of precisely the right type, and to call the right
// constructor.
// Each derives from an intermediate CtorArgs<n>, so that the classes
// Menu<n> below can check the types of the arguments.

struct CtorArgs0 : public CtorAux0 { };

template <class T>
struct Ctor0 : public CtorArgs0
{
  Ctor* copy( ) const { return new Ctor0; }          // overrides Ctor

private:

  SMObject* construct(Chars&, istream&) const {      // overrides CtorAux1
	 return new T( );
  }
  // The istream& is used only by specializations of this function.
};


template <class A1> struct CtorArgs1 : public CtorAux1 { };

template <class T, class A1>
struct Ctor1 : public CtorArgs1<A1>
{
  Ctor* copy( ) const { return new Ctor1; }          // overrides Ctor

private:

  SMObject* construct(SMObject* a1, Chars&, istream&) const {
	 return new T( *((A1*)a1) );
  }
  // overrides CtorAux1
  // The istream& is used only by specializations of this function.
};


template <class A1, class A2> struct CtorArgs2 : public CtorAux2
{
  CtorArgs2(bool transpose = false) : CtorAux2( transpose ) { }
};

template <class T, class A1, class A2>
struct Ctor2 : public CtorArgs2<A1,A2>
{
  Ctor2(bool transpose = false) : CtorArgs2<A1,A2>( transpose ) { }

  Ctor* copy( ) const { return new Ctor2( transposeARGS ); } // overrides Ctor

private:

  SMObject* construct(SMObject* a1, SMObject* a2, Chars&, istream&) const {
	 return new T( *((A1*)a1), *((A2*)a2) );
  }
  // overrides CtorAux2
  // The istream& is used only by specializations of this function.
};


template <class A1, class A2, class A3> struct CtorArgs3 : public CtorAux3 { };

template <class T, class A1, class A2, class A3>
struct Ctor3 : public CtorArgs3<A1,A2,A3>
{
  Ctor* copy( ) const { return new Ctor3; } // overrides Ctor

private:

  SMObject* construct(SMObject* a1, SMObject* a2, SMObject* a3, 
		      Chars&, istream&) const 
  {
    return new T( *((A1*)a1), *((A2*)a2), *((A3*)a3) );
  }
  // overrides CtorAux3
  // The istream& is used only by specializations of this function.
};


template <class A1, class A2, class A3, class A4> struct CtorArgs4 :
  public CtorAux4 { };

template <class T, class A1, class A2, class A3, class A4>
struct Ctor4 : public CtorArgs4<A1,A2,A3,A4>
{
  Ctor* copy( ) const { return new Ctor4; } // overrides Ctor

private:

  SMObject* construct(SMObject* a1, SMObject* a2, SMObject* a3, SMObject* a4, 
		      Chars&, istream&) const 
  {
    return new T( *((A1*)a1), *((A2*)a2), *((A3*)a3), *((A4*)a4) );
  }
  // overrides CtorAux4
  // The istream& is used only by specializations of this function.
};

// At this point we would appear to have a satisfactory situation. The
// most common case, where a T has a constructor which takes arguments of
// types A1, A2, ... is handled automatically. Special cases are easily
// handled by specializing the member `Ctor<n>::construct' to use the
// istream&. In fact, the work of reading the `standard case' arguments
// is already done by CtorAux<n> even in special cases.
// 
// But no.
// 
// A sporadic gcc 2.6.3 bug sometimes prevents specializing a member
// function of a template class. In the perhaps overly naive hope that
// gcc will someday do templates (almost) right, we hack a workaround
// which mimics as closely as possible what we really want to do:
// 
// For each and every special case, derive a subclass of the needed
// instance of Ctor<n>. Override `construct', with the function body
// being exactly what the specialization's would have been.
// 
// An unfortunate consequence is that Ctor<n> will still try to
// instantiate the default version of `construct'. It cannot, because T
// does not have the right constructor. Indeed, the resulting compiler
// error is what this whole scheme is *designed* to cause! So, we hack
// further by putting dummy ctors in the classes T. Now I have to puke.


//---------------------------------------------------------------------------//
//------------------------------- Ctor --------------------------------------//
//--------------------------- special cases ---------------------------------//




// ---------------------------- STATISTICS --------------------------------- //

///for simple menus, such as help 

struct ReadSMSimpleMenu : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMSimpleMenu; }             // overrides Ctor0
  SMObject* construct(Chars&, istream&) const; // overrides Ctor0
};

struct ReadSMOneVariableDataSet : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMOneVariableDataSet; }       // overrides Ctor1
  SMObject* construct(Chars&, istream&) const; // overrides Ctor1
};


struct ReadSMTwoVariableDataSet : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMTwoVariableDataSet; }       // overrides Ctor1
  SMObject* construct(Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMMultiVariableDataSet : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMMultiVariableDataSet; }       // overrides Ctor1
  SMObject* construct(Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMCrossClassifiedDataSet : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCrossClassifiedDataSet; }       // overrides Ctor1
  SMObject* construct(Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMTimeSeriesDataSet : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMTimeSeriesDataSet; }       // overrides Ctor1
  SMObject* construct(Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMQualityControlDataSet : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMQualityControlDataSet; }       // overrides Ctor1
  SMObject* construct(Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMProbabilityDistribution : public Ctor0<SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMProbabilityDistribution; }       // overrides Ctor0
  SMObject* construct(Chars&, istream&) const; // overrides Ctor0
};

struct ReadSMConstructSampleFromDataSet : public Ctor1<SMRandomDataSet,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMConstructSampleFromDataSet; }     // overrides Ctor0
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor0
};

struct ReadSMDataSummary : public Ctor1<SMDataSetDataSummary,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMDataSummary; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMFullAnalysis : public Ctor1<SMDataSetFullAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMFullAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMGraphicAnalysis : public Ctor1<SMDataSetGraphicAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMGraphicAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMRegressionGraphicAnalysis : public Ctor1<SMDataSetGraphicAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMRegressionGraphicAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMGoodnessOfFit : public Ctor1<SMDataSetGoodnessOfFit,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMGoodnessOfFit; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMMeanAnalysis : public Ctor1<SMDataSetMeanAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMMeanAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};


struct ReadSMVarianceAnalysis : public Ctor1<SMDataSetVarianceAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMVarianceAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMOVDSReportAnalysis : public Ctor1<SMOVDSReportAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMOVDSReportAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};
//------------------------------ Two Variable Analysis Tools ---------------------------------------//
struct ReadSMCompareMeanAnalysis : public Ctor1<SMDataSetCompareMeanAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCompareMeanAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};


struct ReadSMCompareVarianceAnalysis : public Ctor1<SMDataSetCompareVarianceAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCompareVarianceAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMNonParametricAnalysis : public Ctor1<SMDataSetNonParametricAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMNonParametricAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMRegressionFullAnalysis : public Ctor1<SMDataSetRegressionFullAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMRegressionFullAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMResidualAnalysis : public Ctor1<SMDataSetResidualAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMResidualAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMPredictionAnalysis : public Ctor1<SMDataSetPredictionAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMPredictionAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMAlternateModelAnalysis : public Ctor1<SMDataSetAlternateModelAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMAlternateModelAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMRegressionCorrelationAnalysis : public Ctor1<SMDataSetRegressionCorrelationAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMRegressionCorrelationAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMTVDSReportIndependent : public Ctor1<SMDataSetTVDSReportIndependentAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMTVDSReportIndependent; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMTVDSRegressionReportAnalysis : public Ctor1<SMDataSetTVDSRegressionReportAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMTVDSRegressionReportAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

//------------------------------ Cross Dataset Analysis Tools ---------------------------//
struct ReadSMCrossConfidenceOne : public Ctor1<SMCrossDataSetConfidenceOne,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCrossConfidenceOne; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMCrossHypothesisOne : public Ctor1<SMCrossDataSetHypothesisOne,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCrossHypothesisOne; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMCrossConfidenceTwo : public Ctor1<SMCrossDataSetConfidenceTwo,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCrossConfidenceTwo; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMCrossHypothesisTwo : public Ctor1<SMCrossDataSetHypothesisTwo,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCrossHypothesisTwo; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMCrossTableAnalysis : public Ctor1<SMCrossDataSetTableAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCrossTableAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
};

struct ReadSMCrossFollowUpAnalysis : public Ctor1<SMCrossDataSetFollowUpAnalysis,SMBaseOfDataSet>, protected FEData
{
  Ctor* copy( ) const { return new ReadSMCrossFollowUpAnalysis; }             // overrides Ctor1
  SMObject* construct(SMObject* a1, Chars&, istream&) const; // overrides Ctor1
}; 
//---------------------------------------------------------------------------//
//------------------------------ Menu ---------------------------------------//
//---------------------------------------------------------------------------//


class Menu : public FEData
{
public:

  enum MenuKind { CHECKIN, TOOLS, MAKE, PACKAGES, DATABASE, TESTING, HELP };

  void startItemGroup( );
  void startItemGroup(const Expression& condition);

  void startCascade(const Text& text);
  void closeCascade( );

  void addDisabled(const Text& text);

  void separator( );

  void done( );

  //-------------------------------------------------------------------------//
  //---------------------------- Action -------------------------------------//
  //-------------------------------------------------------------------------//

  struct Action
  {
	 virtual void print(ostream& ostr) const { }
  };

  
  // -------------------------- STATISTICS --------------------------------- //


  struct DefineDataSet : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  //add by xuxw
  struct DefineOneVariableDataSet : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineTwoVariableDataSet : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineMultiVariableDataSet : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineCrossClassifiedDataSet : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineTimeSeriesDataSet : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineQualityControlDataSet : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineProbabilityDistribution : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct showDataSetHelp : public Action
  {
    void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineDescriptionOfDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineConstructSampleFromDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  
  struct DefineDescriptionOfOneVariableDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineDataSetDataSummary : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDataSetFullAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDataSetGraphicAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDataSetRegressionGraphicAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDataSetGoodnessOfFit : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct  DefineDataSetGoodnessOfFit2: public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };


  struct DefineDataSetMeanAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  
  struct DefineDescriptionOfFullAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineDescriptionOfDataSummary : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDescriptionOfGraphicAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDescriptionOfMeanAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDescriptionOfVarianceAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };
  struct DefineDescriptionOfGoodnessOfFit : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineDataSetVarianceAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineDataSetCompareMeanAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDataSetCompareVarianceAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 
 struct DefineDataSetOVDSReportAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

//------------how variable regression analysis tools----------------//

 struct DefineDataSetNonParametricAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDataSetRegressionFullAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDataSetResidualAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDataSetPredictionAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDataSetAlternateModelAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDataSetRegressionCorrelationAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDataSetTVDSReportIndependent : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 


 struct DefineDataSetTVDSRegressionReportAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDescriptionOfTwoVariableDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  }; 

 struct DefineDescriptionOfCompareMeanDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };


 struct DefineDescriptionOfCompareVarianceDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };


 struct DefineDescriptionOfNonParametricTestDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDescriptionOfFullRegressionDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDescriptionOfGraphicalRegressionDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDescriptionOfResidualAnalysisDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDescriptionOfPredictionParameterTestDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDescriptionOfAlternateModelDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDescriptionOfCorrelationAnalysisDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineDescriptionOfTVDSIndependentReportDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };


 struct DefineDescriptionOfTVDSRegressionReportDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

//----------------cross dataset analysis tools--------////
 struct DefineCrossDataSetConfidenceOne : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineCrossDataSetHypothesisOne : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineCrossDataSetConfidenceTwo : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineCrossDataSetHypothesisTwo : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineCrossDataSetTableAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

 struct DefineCrossDataSetFollowUpAnalysis : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

  struct DefineDescriptionOfCrossDataSet : public Action
  {
	 void print(ostream& ostr) const;      // overrides Action
  };

public:
  void addSimple(const Text& text,const Action& action);
protected:

  Menu(MenuKind);

  void add(Ctor* ctor, const Text& text, const Action& action);

private:

  enum StateType { INIT, ITEM_GROUP, DONE };

  StateType state;
};


//@rn We inline all of the Menu<n> member functions, probably
//@rn inappropriately, to avoid having to do explicit instantiation.
//@rn This can be improved when gcc supports template repositories.


class Menu0 : public Menu
{
public:

  Menu0(MenuKind mk) : Menu( mk ) {
	 cout << "} itemGroups {";
  }

  void add(const CtorArgs0& ctor,
			  const Text& text,
			  const Action& action = Action()
			  )
  {
	 Menu::add( ctor.copy(), text, action );
  }

};


template <class A1>
class Menu1 : public Menu
{
public:

  Menu1(MenuKind mk) : Menu( mk ) {
	 cout << A1::type();
	 cout << "} itemGroups {";
  }

  void add(const CtorArgs1<A1>& ctor,
			  const Text& text,
			  const Action& action = Action()
			  )
  {
	 Menu::add( ctor.copy(), text, action );
  }

};


template <class A1, class A2>
class Menu2 : public Menu
{
public:

  Menu2(MenuKind mk) : Menu( mk ) {
	 cout << A1::type() << ' ' << A2::type();
	 cout << "} itemGroups {";
  }

  void add(const CtorArgs2<A1,A2>& ctor,
			  const Text& text,
			  const Action& action = Action()
			  )
  {
	 Menu::add( ctor.copy(), text, action );
  }

};


template <class A1, class A2, class A3>
class Menu3 : public Menu
{
public:

  Menu3(MenuKind mk) : Menu( mk )
  {
    cout << A1::type() << ' ' << A2::type() << ' ' << A3::type();
    cout << "} itemGroups {";
  }

  void add( const CtorArgs3<A1,A2,A3>& ctor,
	    const Text& text,
	    const Action& action = Action()
	    )
  {
    Menu::add( ctor.copy(), text, action );
  }

};


template <class A1, class A2, class A3, class A4>
class Menu4 : public Menu
{
public:

  Menu4(MenuKind mk) : Menu( mk )
  {
    cout << A1::type() << ' ' << A2::type() << ' ' << A3::type() << ' '
	 << A4::type();
    cout << "} itemGroups {";
  }

  void add( const CtorArgs4<A1,A2,A3,A4>& ctor,
	    const Text& text,
	    const Action& action = Action()
	    )
  {
    Menu::add( ctor.copy(), text, action );
  }

};


#endif
