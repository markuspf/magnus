// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Declaration of class SMDataSetComputationProblems
//
// Principal Author: xiaowei xu
//
// Status: in progress
//
// Revision History:
//

#ifndef _SMDATASETCOMPUTATIONPROBLEMS_H_
#define _SMDATASETCOMPUTATIONPROBLEMS_H_

#include "SMDataSetComputation.h"
#include "OneVariableDataSet.h"
//---------------------------------------------------------------------------//
//---------------------------- SMDataSetFullAnalysis ---------------------------------------//
//---------------------------------------------------------------------------//
# define CLASSMAXSIZE 8

struct DataSetItem{
     double lower,upper,frequency;
};

class DataSetClass{
  public:
    DataSetClass(OneVariableDataSet& ds):theSet(ds){}
    void init();
    void adjust();
    int GetSize();
    DataSetItem operator [ ] ( int i ) const ;
  private:
    OneVariableDataSet& theSet;
    DataSetItem dcSet[CLASSMAXSIZE];
};

//----------------------------send the result of mean analysis------------//

class MeanAnalysisResultSend {
public:
    MeanAnalysisResultSend() {}
public:

protected:
  void sendMeanHyporesult(OID oid,OneVariableDataSet& ds,double Level,double MU0,char MU0Type) const;
  void sendMeanConfidence(OID oid,OneVariableDataSet& ds,double Level) const;
private:
  double getE(OneVariableDataSet& ds,double Level) const;
  double getP(OneVariableDataSet& ds,double Z,char MU0Type) const;
};

//----------------------------send the result of variance analysis------------//

class VarianceAnalysisResultSend {
public:
    VarianceAnalysisResultSend() {}
public:

protected:
  void sendVarianceConfidenceResult(OID oid,OneVariableDataSet& ds,double Level) const;
  void sendVarianceHypoResult(OID oid,OneVariableDataSet& ds,double Level,double MU0,char MU0Type) const;

private:
  double getIntervalMin(OneVariableDataSet& ds,double Level) const;
  double getIntervalMax(OneVariableDataSet& ds,double Level) const;
  double getP(OneVariableDataSet& ds,double Z,char MU0Type) const;
};

//----------------------------send the result of goodness of fit analysis------//

class GoodnessOfFitAnalysisResultSend {
public:
    GoodnessOfFitAnalysisResultSend() {}
public:

protected:
   void sendNormalResult(OID oid,OneVariableDataSet& ds,double Level) const;
   void sendExpResult(OID oid,OneVariableDataSet& ds,double Level) const;
   void sendUniformResult(OID oid,OneVariableDataSet& ds,double Level) const;

private:
   double getP(int N,double Z,char Type) const;
   double getExpZ(OneVariableDataSet& ds,DataSetClass& dc) const;
   double getUnifZ(OneVariableDataSet& ds,DataSetClass& dc) const;
};

//----------------------------send the result of full analysis------//

class DataSetFullAnalysisResultSend {
public:
    DataSetFullAnalysisResultSend() {}
public:

protected:
   void sendFullAnaysisResult(OID oid,OneVariableDataSet& ds) const;

private:
};


//-----------------------------data summary tools------------------------//
class SMDataSetDataSummary : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetDataSummary(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetDataSummary(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetDataSummary"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetDataSummary; }  // overrides SMObject

};

class SMDataSetFullAnalysis : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetFullAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetFullAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetFullAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetFullAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

  void fullAnalysisResult() const;           // overrides SMObject

};

class SMDataSetGraphicAnalysis : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetGraphicAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetGraphicAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetGraphicAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetGraphicAnalysis; }  // overrides SMObject

};

class SMDataSetGoodnessOfFit : public SMDataSetComputation,GoodnessOfFitAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetGoodnessOfFit(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetGoodnessOfFit(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetGoodnessOfFit"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetGoodnessOfFit; }  // overrides SMObject

  virtual void sendResult( ) const;

  double m_Level;
  char m_Type;	 
  int m_setNum;
private:

};


class SMDataSetMeanAnalysis : public SMDataSetComputation,MeanAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetMeanAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetMeanAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetMeanAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetMeanAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

  double m_Level;
  char m_Type;	 
  double m_MU0;	 
  char m_MU0Type;	 

private:
//  double getE(OneVariableDataSet& ds) const;
//  double getP(OneVariableDataSet& ds,double Z) const;
//  void sendHyporesult() const;
};

class SMDataSetVarianceAnalysis : public SMDataSetComputation,VarianceAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetVarianceAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetVarianceAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetVarianceAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetVarianceAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;
  double m_Level;
  char m_Type;	 
  double m_MU0;	 
  char m_MU0Type;
private:
};

class DataSetCompareMeanResultSend {
  public:
     DataSetCompareMeanResultSend() {}
  public:
     void sendCompareMeanConfidenceResult(OID oid,OneVariableDataSet ds1,OneVariableDataSet ds2, double Level) const;
     void sendCompareMeanHypothesisResult(OID oid,OneVariableDataSet ds1,OneVariableDataSet ds2,
                          double Level,double MU0,char MU0Type,bool EqualAssumed) const;
  private:
     double GetP(double Z,int N1,int N2,int V1,int V2,char MU0Type,bool EqualAssumed) const;    
};

class DataSetCompareVarianceResultSend {
  public:
     DataSetCompareVarianceResultSend() {}
  public:
     void sendCompareVarianceHypothesisResult(OID oid,OneVariableDataSet ds1,OneVariableDataSet ds2,double Level) const;
  private:
     double GetP(double Z,int N1,int N2,int V1,int V2,char MU0Type,bool EqualAssumed) const;    
};

class SMDataSetCompareMeanAnalysis : public SMDataSetComputation, DataSetCompareMeanResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetCompareMeanAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetCompareMeanAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetCompareMeanAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetCompareMeanAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

  double m_Level;
  char m_Type;	 
  double m_MU0;	 
  char m_MU0Type;
  bool m_EqualAssumed;
private:
};

class SMDataSetCompareVarianceAnalysis : public SMDataSetComputation,DataSetCompareVarianceResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetCompareVarianceAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetCompareVarianceAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetCompareVarianceAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetCompareVarianceAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

  double m_Level;
  char m_Type;
private:

};

class OVDSReportParam {
public:
  OVDSReportParam() {m_Forms="";m_strSummaryGraphicalParam="";
        m_fitType=' ';m_meanMUType=' ';m_varMUType=' ';}
  ~OVDSReportParam() {}

public:
  Chars m_strSummaryGraphicalParam;
  char m_meanMUType,m_varMUType,m_fitType;
  double m_meanMU0,m_meanHypothesisLevel,m_meanConfidenceLevel;
  double m_varMU0,m_varHypothesisLevel,m_varConfidenceLevel,m_fitLevel;
  Chars m_Forms;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IO:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend IStreamPoll operator >> ( istream& istr, OVDSReportParam& param )
  {
    Chars errMsg;
    param.readFrom(istr, errMsg);
    return IStreamPoll(istr, errMsg);
  }

private:
  void eatWhite( istream& istr, int& pos );
  void readFrom( istream& istr, Chars& errMsg );
  void readFull(istream& istr, Chars& errMsg );
  void readDataSummary(istream& istr, Chars& errMsg );
  void readGraphic(istream& istr, Chars& errMsg );
  void readMean(istream& istr, Chars& errMsg );
  void readVariance(istream& istr, Chars& errMsg );
  void readFitness(istream& istr, Chars& errMsg );
};

class SMOVDSReportAnalysis : public SMDataSetComputation,MeanAnalysisResultSend,VarianceAnalysisResultSend,
                          GoodnessOfFitAnalysisResultSend,DataSetFullAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMOVDSReportAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage,
          const OVDSReportParam params): SMDataSetComputation (ds,def,heritage)
         {m_Params=params;}
    SMOVDSReportAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  
  
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "OVDSReportAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::OVDSReportAnalysis; }  // overrides SMObject
  virtual void viewStructure(ostream& ostr) const;
  virtual void sendResult( ) const;
public:
  OVDSReportParam m_Params;

private:
};

#endif
