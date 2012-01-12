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

#ifndef _SMDATASETTVCOMPUTATIONPROBLEMS_H_
#define _SMDATASETTVCOMPUTATIONPROBLEMS_H_

#include "SMObject.h"
#include "SMDataSetComputation.h"
#include "SMDataSetComputationProblems.h"
#include "OneVariableDataSet.h"
#include "TwoVariableDataSet.h"

class SMDataSetTVDSRegressionAnalysis : public SMDataSetComputation{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

    SMDataSetTVDSRegressionAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetTVDSRegressionAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////
  virtual void viewStructure(ostream& ostr) const;
  virtual void sendResult( ) const {}
};

class TVDSReportIndependentParam {
public:
  TVDSReportIndependentParam() {m_Forms="";m_strSummaryGraphicalParam="";
        m_fitType=' ';m_meanMUType=' ';}
  ~TVDSReportIndependentParam() {}

public:
  Chars m_strSummaryGraphicalParam;
  char m_meanMUType,m_fitType;
  double m_meanMU0,m_meanHypothesisLevel,m_meanConfidenceLevel;
  double m_varHypothesisLevel,m_nonParametricLevel,m_fitLevel;
  bool m_equalAssumed;
  Chars m_Forms;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IO:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend IStreamPoll operator >> ( istream& istr, TVDSReportIndependentParam& param )
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
  void readNonParamatric(istream& istr, Chars& errMsg );
};

class TVDSReportRegressionParam {
public:
  TVDSReportRegressionParam() {m_Forms="";m_strSummaryGraphicalParam="";
                              }
  ~TVDSReportRegressionParam() {}

public:
  Chars m_strSummaryGraphicalParam;
  Chars m_Forms;
  double m_correlationConfidenceLevel,m_correlationHypothesisLevel,m_correlationHypothesisMU0;
  char m_correlationHypothesisMU0Type;
  char m_modelType;
  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IO:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend IStreamPoll operator >> ( istream& istr, TVDSReportRegressionParam& param )
  {
    Chars errMsg;
    param.readFrom(istr, errMsg);
    return IStreamPoll(istr, errMsg);
  }

private:
  void eatWhite( istream& istr, int& pos );
  void readFrom( istream& istr, Chars& errMsg );
  void readCorrelation(istream& istr, Chars& errMsg );
};

class TVDSAllPredictionParam {
public:
  TVDSAllPredictionParam() {}
  ~TVDSAllPredictionParam() {}

public:

  double m_SlopeConfidenceLevel,m_SlopeHypothesisLevel,
         m_SlopeHypothesisMU0,m_InterceptConfidenceLevel,
         m_InterceptHypothesisLevel,m_InterceptHypothesisMU0,
         m_ModelLevel,m_PredictLevel,m_PredictX0;
  char m_SlopeHypothesisMU0Type,m_InterceptHypothesisMU0Type;

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // IO:                                                                 //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  friend IStreamPoll operator >> ( istream& istr, TVDSAllPredictionParam& param )
  {
    Chars errMsg;
    param.readFrom(istr, errMsg);
    return IStreamPoll(istr, errMsg);
  }

private:
  void eatWhite( istream& istr, int& pos );
  void readFrom( istream& istr, Chars& errMsg );
  void readSlope(istream& istr, Chars& errMsg );
  void readIntercept(istream& istr, Chars& errMsg );
  void readPrediction(istream& istr, Chars& errMsg );
  void readModel(istream& istr, Chars& errMsg );
};


class DataSetTNDSFullRegressionResultSend {
  public:
     DataSetTNDSFullRegressionResultSend() {}
  public:
     void sendFullRegressionResult(OID oid,TwoVariableDataSet ds) const;
  private:
};

class DataSetNonParametricResultSend {
  public:
     DataSetNonParametricResultSend() {}
  public:
     void sendNonParametricResult(OID oid,TwoVariableDataSet ds,OneVariableDataSet ds1,OneVariableDataSet ds2,
                          double Level) const;
  private:
     void sendSmallNonParametricResult(OID oid,TwoVariableDataSet ds,int n1,int n2) const;
};

class DataSetResidualAnalysisResultSend {
  public:
     DataSetResidualAnalysisResultSend() {}
  public:
     void sendResidualAnalysisResult(OID oid,TwoVariableDataSet ds) const;
  private:
};

class DataSetCorrelationAnalysisResultSend {
  public:
     DataSetCorrelationAnalysisResultSend() {}
  public:
     void sendCorrelationConfidenceResult(OID oid,TwoVariableDataSet ds,double Level) const;
     void sendCorrelationHypothesisResult(OID oid,TwoVariableDataSet ds,double Level,double MU0,char MU0Type) const;
  private:
     double getP(double Z,char MU0Type) const;
};
class DataSetAlternateModelAnalysisResultSend {
  public:
     DataSetAlternateModelAnalysisResultSend() {}
  public:
     void sendAlternateModelResult(OID oid,TwoVariableDataSet ds,char Type) const;

  private:
     void sendExponentialModel(OID oid,TwoVariableDataSet ds) const;
     void sendPowerModel(OID oid,TwoVariableDataSet ds) const;
};

class SMDataSetNonParametricAnalysis : public SMDataSetComputation,DataSetNonParametricResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetNonParametricAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetNonParametricAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetNonParametricAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetNonParametricAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;
 public:
  double m_Level;
};

class SMDataSetRegressionGraphicAnalysis : public SMDataSetTVDSRegressionAnalysis
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetRegressionGraphicAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetTVDSRegressionAnalysis (ds,def,heritage){}
    SMDataSetRegressionGraphicAnalysis(SMBaseOfDataSet& ds) : SMDataSetTVDSRegressionAnalysis (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetRegressionGraphicAnalysis"; }
  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetRegressionGraphicAnalysis; }  // overrides SMObject

};

class SMDataSetRegressionFullAnalysis : public SMDataSetComputation
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetRegressionFullAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetRegressionFullAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetRegressionFullAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetRegressionFullAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

};

class SMDataSetResidualAnalysis : public SMDataSetTVDSRegressionAnalysis,
                                  DataSetResidualAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetResidualAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetTVDSRegressionAnalysis (ds,def,heritage){}
    SMDataSetResidualAnalysis(SMBaseOfDataSet& ds) : SMDataSetTVDSRegressionAnalysis (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetResidualAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetResidualAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

};
//----------------------------send the result of variance analysis------------//

class TVDSPredictionParametricAnalysisResultSend {
public:
    TVDSPredictionParametricAnalysisResultSend() {}
public:

protected:
  void sendHypotheisSlopeResult(OID oid,TwoVariableDataSet& ds,double Level,double MU0,char MU0Type) const;
  void sendConfidenceSlopeResult(OID oid,TwoVariableDataSet& ds,double Level) const;
  void sendHypotheisInterceptResult(OID oid,TwoVariableDataSet& ds,double Level,double MU0,char MU0Type) const;
  void sendConfidenceInterceptResult(OID oid,TwoVariableDataSet& ds,double Level) const;
  void sendModelFitResult(OID oid,TwoVariableDataSet& ds,double Level) const;
  void sendPredictionIntervalResult(OID oid,TwoVariableDataSet& ds,double Level,double X0) const;

private:

};

class SMDataSetPredictionAnalysis : public SMDataSetComputation,TVDSPredictionParametricAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetPredictionAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage,TVDSAllPredictionParam Param):
	    SMDataSetComputation (ds,def,heritage){m_Params=Param;}
    SMDataSetPredictionAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetPredictionAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetPredictionAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;
public:
  double m_Level;
  char m_Type;	 
  double m_MU0,m_PredictedPoint;	 
  char m_MU0Type;	 
  TVDSAllPredictionParam m_Params;
};

class SMDataSetAlternateModelAnalysis : public SMDataSetComputation,
                                      DataSetAlternateModelAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetAlternateModelAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetAlternateModelAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetAlternateModelAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetAlternateModelAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;

  public:
     char m_Type;

};

class SMDataSetRegressionCorrelationAnalysis : public SMDataSetComputation,
                                        DataSetCorrelationAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

	SMDataSetRegressionCorrelationAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage):
	    SMDataSetComputation (ds,def,heritage){}
    SMDataSetRegressionCorrelationAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetRegressionCorrelationAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetRegressionCorrelationAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;
 public:
  double m_Level;
  double m_MU0;
  char m_MU0Type;
  char m_Type;
};

class SMDataSetTVDSReportIndependentAnalysis : public SMDataSetComputation,DataSetNonParametricResultSend,
                                                DataSetCompareVarianceResultSend,DataSetCompareMeanResultSend,
                                                GoodnessOfFitAnalysisResultSend,DataSetFullAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

       SMDataSetTVDSReportIndependentAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage,TVDSReportIndependentParam params):
	    SMDataSetComputation (ds,def,heritage)
         {m_Params=params;}
    SMDataSetTVDSReportIndependentAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetTVDSReportIndependentAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetTVDSReportIndependentAnalysis; }  // overrides SMObject

  virtual void sendResult( ) const;
  virtual void viewStructure(ostream& ostr) const;
public:
  TVDSReportIndependentParam m_Params;
};

class SMDataSetTVDSRegressionReportAnalysis : public SMDataSetComputation,DataSetTNDSFullRegressionResultSend,
                                              DataSetResidualAnalysisResultSend,DataSetAlternateModelAnalysisResultSend,
                                              TVDSPredictionParametricAnalysisResultSend,DataSetCorrelationAnalysisResultSend
{
public:

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Constructors:                                                       //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

       SMDataSetTVDSRegressionReportAnalysis(SMBaseOfDataSet& ds, const Chars def, const Chars heritage,TVDSReportRegressionParam param):
	    SMDataSetComputation (ds,def,heritage)
            {m_Params=param;}
    SMDataSetTVDSRegressionReportAnalysis(SMBaseOfDataSet& ds) : SMDataSetComputation (ds)
	{ }
  

  /////////////////////////////////////////////////////////////////////////
  //                                                                     //
  // Front End Interfacing:                                              //
  //                                                                     //
  /////////////////////////////////////////////////////////////////////////

  static const char* type( ) { return "SMDataSetTVDSRegressionReportAnalysis"; }

  const char* typeID( ) const { return type(); }        // overrides SMObject

  const IconID iconID( ) const { return IconID::DataSetTVDSRegressionReportAnalysis; }  // overrides SMObject
  virtual void sendResult( ) const;
  virtual void viewStructure(ostream& ostr) const;

public:
  TVDSReportRegressionParam m_Params;
};

#endif
