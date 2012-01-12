// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMDataSetTVComputationProblems
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//

#include <fstream.h>
#include "SMDataSetTVComputationProblems.h"
#include "OutMessages.h"
#include "statsFunctions.h"
#include "math.h"
#include "List.h"
#include "TwoVariableDataSet.h"

void SMDataSetTVDSRegressionAnalysis::viewStructure(ostream& ostr) const
{
  TwoVariableDataSet ds(theDataSet.getSet());
  ostr
	 << "viewBuilder " << m_ComputationView << " viewParameters {viewID "
	 << oid()
   ////diliver the slope and intercept
         << " forms { " << ds.GetSlope() << " " << ds.GetIntercept() << "}"
	 << " heritage {"
	 << heritage()
	 << "}}";
}

void SMDataSetTVDSRegressionReportAnalysis::viewStructure(ostream& ostr) const
{
  TwoVariableDataSet ds(theDataSet.getSet());
  ostr
	 << "viewBuilder " << m_ComputationView << " viewParameters {viewID "
	 << oid()
   ////diliver the slope and intercept
         << " forms { "
             << " {" << m_Params.m_strSummaryGraphicalParam << "} "
             << " {" << ds.GetSlope() << " " << ds.GetIntercept() << "} "
         <<" } "
	 << " heritage {"
	 << heritage()
	 << "}}";
}

void DataSetNonParametricResultSend::sendNonParametricResult(OID oid,TwoVariableDataSet ds,OneVariableDataSet ds1,OneVariableDataSet ds2,
                          double Level) const{
    int m=ds1.cardinality();
    int n=ds2.cardinality();
    double Ux=ds.GetUx();
    double Uy=ds.GetUy();
    double U=fmin(Ux,Uy);
    double z=statsFunctions::NormInv(1-Level);
    double d=0;
    {  LogMessage msg( oid,0);
         msg <<  "THE NON PARAMETRIC TESTING";
         msg.send();}

    if(m>=12||n>=12){
      d=(m*n+1.0-z*sqrt(m*n*(m+n+1.0)/3.0))/2.0;
      Chars isSign="is";
      if(U>d)
         isSign="is not";
      {  LogMessage msg( oid,0);
         msg <<  " It "<< isSign<<" significant at a "
             << 100*Level << "% level.";
         msg.send();}
    }
    else
       sendSmallNonParametricResult(oid,ds,m,n);
}

void EatWhite( ifstream& istr){
  while( !istr.eof() ) {
    char ch = istr.peek();
    if( ch == ' ' || ch == '\t' || ch == '\n' ) {
      istr.get();
    }
    else
      break;
  }
} 

void DataSetNonParametricResultSend::sendSmallNonParametricResult(OID oid,TwoVariableDataSet ds,int n1,int n2) const{
    double Ux=ds.GetUx();
    double Uy=ds.GetUy();
    double U=fmin(Ux,Uy);

  {  LogMessage msg( oid,0);
     msg <<  "  The Wilcoxon-Mann-Whitney U value is " << U;
     msg.send();}

  {  LogMessage msg( oid,0);
     msg <<  "  The size of the first sample is " << n1;
     msg.send();}
  

  {  LogMessage msg( oid,0);
     msg <<  "  The size of the second sample is " << n2;
     msg.send();}

  {  LogMessage msg( oid,0);
     msg <<  "  The U value should be compared to ";
     msg.send();}
  int temp;
  if(n1<n2){
     temp=n1;
     n1=n2;
     n2=temp;
  }
  ifstream fp;
  fp.open("WMWData.dat");
  int row,column,d;
  double delta,alpha2,alpha1;
  while(!fp.eof()) {
    EatWhite(fp);
    fp >> row;
    EatWhite(fp);
    fp >> column;
    EatWhite(fp);
    fp >> d;
    EatWhite(fp);
    fp >> delta;
    EatWhite(fp);
    fp >> alpha2;
    EatWhite(fp);
    fp >> alpha1;
    EatWhite(fp);
    if(row==n1&& column==n2&&alpha2>=0.01) {
         LogMessage msg( oid,0);
         msg <<  "    A U value of " << d << " has a P-value of "
             << alpha2;
         msg.send();
    }
  }
  fp.close();
}

void SMDataSetNonParametricAnalysis::sendResult( ) const{
    TwoVariableDataSet ds(theDataSet.getSet());
    OneVariableDataSet ds1(theDataSet.getSet(),0);
    OneVariableDataSet ds2(theDataSet.getSet(),1);
    sendNonParametricResult(oid(),ds,ds1,ds2,m_Level);
}

void DataSetTNDSFullRegressionResultSend::sendFullRegressionResult(OID oid,TwoVariableDataSet ds) const {
    {  LogMessage msg( oid,0);
       msg <<  "           regression Diagnostics ";
       msg.send();}
    if(!ds.IsEqualSize()){
       LogMessage msg( oid,0);
       msg <<  "           Size of two datasets are not equal! ";
       msg.send();
       return;
    }
    {  LogMessage msg( oid,0);
       msg <<  "  The fitted regression line was Y= "
           << ds.GetSlope() << "X+" << ds.GetIntercept();
       msg.send();}
    {  LogMessage msg( oid,0);
       msg <<  "  The regression slope was " << ds.GetSlope();
       msg.send();}
    {  LogMessage msg( oid,0);
       msg <<  "  The regression intercept was " << ds.GetIntercept();
       msg.send();}
    {  LogMessage msg( oid,0);
       msg <<  "  A t-value for the slope was "
           << ds.GetTValue();
       msg.send();}
    {  LogMessage msg( oid,0);
       msg <<  "  The coefficient of determination was "
           << pow(ds.GetR(),2);
       msg.send();}
    {  LogMessage msg( oid,0);
       msg <<  "  The correlation coefficient was "
           << ds.GetR();
       msg.send();}    

}

void SMDataSetRegressionFullAnalysis::sendResult( ) const{
    TwoVariableDataSet ds(theDataSet.getSet());
    {  LogMessage msg( oid(),0);
       msg <<  "           regression Diagnostics ";
       msg.send();}
    if(!ds.IsEqualSize()){
       LogMessage msg( oid(),0);
       msg <<  "           Size of two datasets are not equal! ";
       msg.send();
       return;
    }
    {  LogMessage msg( oid(),0);
       msg <<  "  The fitted regression line was Y= "
           << ds.GetSlope() << "X+" << ds.GetIntercept();
       msg.send();}
    {  LogMessage msg( oid(),0);
       msg <<  "  The regression slope was " << ds.GetSlope();
       msg.send();}
    {  LogMessage msg( oid(),0);
       msg <<  " The regression intercept was " << ds.GetIntercept();
       msg.send();}
    {  LogMessage msg( oid(),0);
       msg <<  "    A t-value for the slope was "
           << ds.GetTValue();
       msg.send();}
    {  LogMessage msg( oid(),0);
       msg <<  "   The coefficient of determination was "
           << pow(ds.GetR(),2);
       msg.send();}
    {  LogMessage msg( oid(),0);
       msg <<  "  The correlation coefficient was "
           << ds.GetR();
       msg.send();}    
}

void SMDataSetPredictionAnalysis::sendResult( ) const{
    TwoVariableDataSet ds(theDataSet.getSet());
   switch(m_Type){
      case 'S':sendHypotheisSlopeResult(oid(),ds,m_Level,m_MU0,m_MU0Type);
             break;
      case 's':sendConfidenceSlopeResult(oid(),ds,m_Level);
             break;
      case 'I':sendHypotheisInterceptResult(oid(),ds,m_Level,m_MU0,m_MU0Type);
             break;
      case 'i':sendConfidenceInterceptResult(oid(),ds,m_Level);
             break;
      case 'M':sendModelFitResult(oid(),ds, m_Level);
             break;
      case 'P':sendPredictionIntervalResult(oid(),ds,m_Level,m_PredictedPoint);
             break;
      case 'A':
             sendHypotheisSlopeResult(oid(),ds,m_Params.m_SlopeConfidenceLevel,m_Params.m_SlopeHypothesisMU0,
                                          m_Params.m_SlopeHypothesisMU0Type);
             sendConfidenceSlopeResult(oid(),ds,m_Params.m_SlopeConfidenceLevel);
             sendHypotheisInterceptResult(oid(),ds,m_Params.m_InterceptHypothesisLevel,
                                     m_Params.m_InterceptHypothesisMU0,m_Params.m_InterceptHypothesisMU0Type);
             sendConfidenceInterceptResult(oid(),ds,m_Params.m_InterceptConfidenceLevel);
             sendModelFitResult(oid(),ds, m_Params.m_ModelLevel);
             sendPredictionIntervalResult(oid(),ds,m_Params.m_PredictLevel,m_Params.m_PredictX0);
             break;
   }
}


void SMDataSetTVDSRegressionReportAnalysis::sendResult( ) const{
    TwoVariableDataSet ds(theDataSet.getSet());
    {  LogMessage msg( oid());
         msg <<  " Two Variable Dataset Regression Report ";
         msg.send();}

    for (int i=0;i<m_Params.m_Forms.length();i++) {
     switch (m_Params.m_Forms[i]) {
        case 'F':
           sendFullRegressionResult(oid(),ds);
           break;
        case 'R':
           sendResidualAnalysisResult(oid(),ds);
           break;
        case 'c':
           sendCorrelationConfidenceResult(oid(),ds,m_Params.m_correlationConfidenceLevel);
           break;
        case 'C':
           sendCorrelationHypothesisResult(oid(),ds,m_Params.m_correlationHypothesisLevel,
                       m_Params.m_correlationHypothesisMU0,m_Params.m_correlationHypothesisMU0Type);
           break;
        case 'M':
           sendAlternateModelResult(oid(),ds,m_Params.m_modelType);
           break;
     }
  }

}

void TVDSPredictionParametricAnalysisResultSend::sendHypotheisSlopeResult(OID oid,TwoVariableDataSet& ds,
                         double Level,double MU0,char MU0Type) const {
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     double Z=(ds.GetSlope()-MU0)/ds.GetSb();
     int n=ds.GetSize();
     double P;
     Chars MuType;
     switch(MU0Type){
       case '1':  ///B != B0
         P=1-statsFunctions::TDis(fabs(Z),n-2);
         MuType="B \\u2260 "; break;
       case '2':  ///B > B0
         P=1-statsFunctions::TDis(Z,n-2);
         MuType="B > "; break;
       case '3':  ///B < B0
         P=1-statsFunctions::TDis(fabs(Z),n-2);
         MuType="B < "; break;
    }

     {  LogMessage msg( oid,0);
         msg <<  "   Testing Concerning Regression Slope";
         msg.send();}

     {  LogMessage msg( oid, 0);
        msg <<  "The null hypothesis was b=" 
           << MU0 <<".";
        msg.send();}

     {  LogMessage msg( oid, 0);
           msg <<   "      The alternative hypothesis was  ";
           msg.send();}

     {  LogMessage msg( oid, 0);
           msg << "         " << MuType << MU0;
           msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The regression slope was " << ds.GetSlope();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The regression intercept was " << ds.GetIntercept();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The correlation coefficient was " << ds.GetR();
         msg.send();}
     Chars isSign="significant";
     if(P>=Level)
        isSign="not significant";
     {  LogMessage msg( oid,0);
         msg <<  "  The test of the regression slope B against B0 was " << isSign 
               << " at a " << Level << " level of significance. The P-value was "<< P ;
         msg.send();}
}

void TVDSPredictionParametricAnalysisResultSend:: sendConfidenceSlopeResult(OID oid,TwoVariableDataSet& ds,double Level) const{
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     int n=ds.GetSize();
     double V1=ds.GetSlope()-statsFunctions::TInv(Level,n-2)*ds.GetSb();
     double V2=ds.GetSlope()+statsFunctions::TInv(Level,n-2)*ds.GetSb();

     {  LogMessage msg( oid,0);
         msg <<  "   Testing Concerning Regression Slope";
         msg.send();}
     
     {  LogMessage msg( oid,0);
         msg <<  "   The regression slope was " << ds.GetSlope();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The regression intercept was " << ds.GetIntercept();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The correlation coefficient was " << ds.GetR();
         msg.send();}
     {  LogMessage msg( oid,0);
         msg <<  "  An " << Level*100 << "% confidence interval for the regression slope B was";
         msg.send();}
     {  LogMessage msg( oid,0);
         msg <<  V1 << "<= B <=" << V2;
         msg.send();}

}
void TVDSPredictionParametricAnalysisResultSend:: sendHypotheisInterceptResult(OID oid,TwoVariableDataSet& ds,double Level,
     double MU0,char MU0Type) const{
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     double Z=(ds.GetIntercept()-MU0)/ds.GetSa();
     int n=ds.GetSize();
     double P;
     Chars MuType;
     switch(MU0Type){
       case '1':  ///B != B0
         P=1-statsFunctions::TDis(fabs(Z),n-2);
         MuType="a \\u2260 "; break;
       case '2':  ///B > B0
         P=1-statsFunctions::TDis(Z,n-2);
         MuType="a > "; break;
       case '3':  ///B < B0
         P=1-statsFunctions::TDis(fabs(Z),n-2);
         MuType="a < "; break;
    }

     {  LogMessage msg( oid,0);
         msg <<  "   Testing Concerning Regression Intercept ";
         msg.send();}

     {  LogMessage msg( oid, 0);
        msg <<  "The null hypothesis was a=" 
           << MU0 <<".";
        msg.send();}

     {  LogMessage msg( oid, 0);
           msg <<   "      The alternative hypothesis was  ";
           msg.send();}

     {  LogMessage msg( oid, 0);
           msg << "         " << MuType << MU0;
           msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The regression slope was " << ds.GetSlope();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The regression intercept was " << ds.GetIntercept();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The correlation coefficient was " << ds.GetR();
         msg.send();}
     Chars isSign="significant";
     if(P>=Level)
        isSign="not significant";
     {  LogMessage msg( oid,0);
         msg <<  "  The test of the regression intercept a against a0 was " << isSign 
               << " at a " << Level << " level of significance. The P-value was "<< P ;
         msg.send();}
}
void TVDSPredictionParametricAnalysisResultSend:: sendConfidenceInterceptResult(OID oid,TwoVariableDataSet& ds,double Level) const{
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     int n=ds.GetSize();
     double V1=ds.GetIntercept()-statsFunctions::TInv(Level,n-2)*ds.GetSa();
     double V2=ds.GetIntercept()+statsFunctions::TInv(Level,n-2)*ds.GetSa();

     {  LogMessage msg( oid,0);
         msg <<  "   Testing Concerning Regression Intercept";
         msg.send();}
     
     {  LogMessage msg( oid,0);
         msg <<  "   The regression slope was " << ds.GetSlope();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The regression intercept was " << ds.GetIntercept();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The correlation coefficient was " << ds.GetR();
         msg.send();}
     {  LogMessage msg( oid,0);
         msg <<  "  An " << Level*100 << "% confidence interval for the regression intercept a was";
         msg.send();}
     {  LogMessage msg( oid,0);
         msg <<  V1 << "<= a <=" << V2;
         msg.send();}
}

void TVDSPredictionParametricAnalysisResultSend:: sendModelFitResult(OID oid,TwoVariableDataSet& ds,double Level) const{
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     int n=ds.GetSize();
     double Z=ds.GetSlope()/ds.GetSb();
     double P=1-statsFunctions::TDis(fabs(Z),n-2);
     
     {  LogMessage msg( oid,0);
         msg <<  "   Test of Model Fit ";
         msg.send();}
     
     {  LogMessage msg( oid,0);
         msg <<  "   The regression slope was " << ds.GetSlope();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The regression intercept was " << ds.GetIntercept();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The correlation coefficient was " << ds.GetR();
         msg.send();}
     Chars isSign="significance";
     Chars isFit="is";
     if(P>=Level){
         isSign="not significance";
         isFit="is not";
     }  
     {  LogMessage msg( oid,0);
         msg <<  " The test of the adequacy of the linear model was " << isSign << " at a "
             << Level*100 << "% level of significance. The P-value was " << P;
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  " Therefore the model "<< isFit <<" a good fit.";
         msg.send();}

}
void TVDSPredictionParametricAnalysisResultSend:: sendPredictionIntervalResult(OID oid,TwoVariableDataSet& ds,double Level,double X0) const{
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     int n=ds.GetSize();
     double Ypred=ds.GetIntercept()+ds.GetSlope()*X0;

     double SUy=ds.GetSUy(X0);
     double Spred=ds.GetSpred(X0);
     double V1=Ypred-statsFunctions::TInv(Level,n-2)*SUy;
     double V2=Ypred+statsFunctions::TInv(Level,n-2)*SUy;
     double W1=Ypred-statsFunctions::TInv(Level,n-2)*Spred;
     double W2=Ypred+statsFunctions::TInv(Level,n-2)*Spred;

     {  LogMessage msg( oid,0);
         msg <<  "        Prediction Interval for a Linear Model ";
         msg.send();}
     
     {  LogMessage msg( oid,0);
         msg <<  "   The regression slope was " << ds.GetSlope();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The regression intercept was " << ds.GetIntercept();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The correlation coefficient was " << ds.GetR();
         msg.send();}
         
     {  LogMessage msg( oid,0);
         msg <<  "   The predicted point was " << X0;
         msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The predicted response at "<< X0 << " is " << Ypred;
         msg.send();}
         
         
     {  LogMessage msg( oid,0);
         msg <<  "  A " << Level*100 << "% confidence interval for the mean response at " << X0 << " is "
             <<  V1 << "<= Mean Response <= " << V2;
         msg.send();}
     {  LogMessage msg( oid,0);
         msg <<  "  A " << Level*100 << "% prediction interval for the response at " << X0 << " is "
             <<  W1 << "<= Response <= " << W2;
         msg.send();}

}


void SMDataSetTVDSReportIndependentAnalysis::viewStructure(ostream& ostr) const
{
  ostr
	 << "viewBuilder " << m_ComputationView << " viewParameters {viewID "
	 << oid()
         << " forms { " << m_Params.m_strSummaryGraphicalParam << "}"
	 << " heritage {"
	 << heritage()
	 << "}}";
}

void TVDSReportRegressionParam::eatWhite( istream& istr, int& pos ){
  while( !istr.eof() ) {
    char ch = istr.peek();
    if( ch == ' ' || ch == '\t' || ch == '\n' ) {
      ++pos;
      istr.get();
    }
    else
      break;
  }
} 

void TVDSReportRegressionParam::readFrom( istream& istr, Chars& errMsg )
{
  // We expect a list of numbers separated by commas and enclosed in
  // braces. { { F {} } { d{type cell width startpoint}} { g{type cell width startpoint}} 
  // { m{type MU0 level MUType }} { v{type MU0 level MUType}} { f{type level }} }
  //
  int pos = 0;
  double d;
  eatWhite(istr,pos);
  if( istr.eof() || istr.peek() != '{' ) {
    errMsg = " { expected ";
    return;
  }
  
  istr.get(); eatWhite(istr,pos);
  
  while( !istr.eof() && istr.peek() != '}'  ) {
     eatWhite(istr,pos);
     istr.get();   ///get first {
     char type; 
     istr >> type;
     eatWhite(istr,pos);
     istr.get();   ///read second {
     eatWhite(istr,pos);
     switch(type) {
       case 'F':
          m_Forms+='F';
          break;
       case 'G':
          m_strSummaryGraphicalParam+=" {G} ";
          m_Forms+='G';
          break;
       case 'R':
          m_strSummaryGraphicalParam+=" {R} ";
          m_Forms+='R';
          break;
       case 'C':
          readCorrelation(istr,errMsg);
          break;

       case 'M':
          istr >> m_modelType;
          m_Forms+='M';
          break;
     }
     eatWhite(istr,pos);
     istr.get();  ///read first }
     eatWhite(istr,pos);
     istr.get();  ///read second }
     eatWhite(istr,pos);
  }
}


void TVDSReportIndependentParam::eatWhite( istream& istr, int& pos )
{
  while( !istr.eof() ) {
    char ch = istr.peek();
    if( ch == ' ' || ch == '\t' || ch == '\n' ) {
      ++pos;
      istr.get();
    }
    else
      break;
  }
} 


// temporary simple version

void TVDSReportIndependentParam::readFrom( istream& istr, Chars& errMsg )
{
  // We expect a list of numbers separated by commas and enclosed in
  // braces. { { F {} } { d{type cell width startpoint}} { g{type cell width startpoint}} 
  // { m{type MU0 level MUType }} { v{type MU0 level MUType}} { f{type level }} }
  //
  int pos = 0;
  double d;
  eatWhite(istr,pos);
  if( istr.eof() || istr.peek() != '{' ) {
    errMsg = " { expected ";
    return;
  }
  
  istr.get(); eatWhite(istr,pos);
  
  while( !istr.eof() && istr.peek() != '}'  ) {
     eatWhite(istr,pos);
     istr.get();   ///get first {
     char type; 
     istr >> type;
     eatWhite(istr,pos);
     istr.get();   ///read second {
     eatWhite(istr,pos);
     switch(type) {
       case 'F':
          m_Forms+='F';
          break;
       case 'd': 
          readDataSummary(istr, errMsg);
          break;
       case 'g': 
          readGraphic( istr, errMsg);
          break;
       case 'm': 
          readMean(istr, errMsg);
          break;
       case 'v': 
          readVariance(istr, errMsg);
          break;
       case 'f': 
          readFitness(istr, errMsg);
          m_Forms+='f';
          break;
       case 'n': 
          readNonParamatric(istr, errMsg);
          break;
     }
     eatWhite(istr,pos);
     istr.get();  ///read first }
     eatWhite(istr,pos);
     istr.get();  ///read second }
     eatWhite(istr,pos);
  }
}
//F{ }
void TVDSReportIndependentParam::readFull(istream& istr, Chars& errMsg ){
}

// d{type cell width startpoint} 
void TVDSReportIndependentParam::readDataSummary(istream& istr, Chars& errMsg ){
  char c;
  Chars strSummaryParam="";
  while( !istr.eof() && istr.peek() != '}'  ) {
     istr >> c;
     if(c==',')
        c=' ';
     strSummaryParam=strSummaryParam +c;
  }
  m_strSummaryGraphicalParam=m_strSummaryGraphicalParam+" {"+strSummaryParam+ "} ";
}

//g{type cell width startpoint} 
void TVDSReportIndependentParam::readGraphic(istream& istr, Chars& errMsg ){
  char c;
  Chars strGraphicalParam="";
  while( !istr.eof() && istr.peek() != '}'  ) {
     istr >> c;
     if(c==',')
        c=' ';
     strGraphicalParam=strGraphicalParam +c;
  }
  m_strSummaryGraphicalParam=m_strSummaryGraphicalParam+" {"+strGraphicalParam+ "} ";
}

// m{type(H,C) MU0 level MUType } 
void TVDSReportIndependentParam::readMean(istream& istr, Chars& errMsg ){
  char MeanType;
  char equalAssumed;
  istr >> MeanType;
  if(MeanType=='C'){
      istr >> m_meanConfidenceLevel;
      m_Forms+='m';
  } else {
      istr >> m_meanMU0;
      istr >> m_meanHypothesisLevel;
      istr >> m_meanMUType;
      istr >> equalAssumed;
      m_equalAssumed=(equalAssumed=='T')? true : false;
      m_Forms+='M';
  }
}

//v{type(H,C) MU0 level MUType}
void TVDSReportIndependentParam::readVariance(istream& istr, Chars& errMsg ){
  char MeanType;
  istr >> MeanType;
  istr >> m_varHypothesisLevel;
  m_Forms+='V';
}

//f{type(1 2 3 ) level } 

void TVDSReportIndependentParam::readFitness(istream& istr, Chars& errMsg ){
  istr >> m_fitType;
  istr >> m_fitLevel;
}
void TVDSReportIndependentParam::readNonParamatric(istream& istr, Chars& errMsg ){
  istr >> m_nonParametricLevel;
  m_Forms+='n';
}

void TVDSReportRegressionParam::readCorrelation(istream& istr, Chars& errMsg ){
  char CorrelationType;
  istr >> CorrelationType;
  if(CorrelationType=='C'){
      istr >> m_correlationConfidenceLevel;
      m_Forms+='c';
  } else {
      istr >> m_correlationHypothesisMU0;
      istr >> m_correlationHypothesisLevel;
      istr >> m_correlationHypothesisMU0Type;
      m_Forms+='C';
  }
}
void SMDataSetTVDSReportIndependentAnalysis::sendResult( ) const{
    TwoVariableDataSet ds(theDataSet.getSet());
    OneVariableDataSet ds1(theDataSet.getSet(),0);
    OneVariableDataSet ds2(theDataSet.getSet(),1);

    {  LogMessage msg( oid());
         msg <<  " Data Set Independent Analysis Report ";
         msg.send();}
/*    {  LogMessage msg( oid());
         msg <<  " Debug Message: " 
         <<  "Summary Param: " << m_Params.m_strSummaryGraphicalParam 
         <<   "Fit type: " << m_Params.m_fitType
         <<   "mean Type " << m_Params.m_meanMUType
         <<   "mean MU0:" << m_Params.m_meanMU0
         <<   "Mean Level: " << m_Params.m_meanConfidenceLevel
         <<   "variance Level: " << m_Params.m_varHypothesisLevel
         <<   "Fit Level: "  << m_Params.m_fitLevel
         <<   "Non-Parametric Level: "  << m_Params.m_nonParametricLevel
         <<   "Forms: "  << m_Params.m_Forms;
         msg.send();}
*/
   for (int i=0;i<m_Params.m_Forms.length();i++) {
     switch (m_Params.m_Forms[i]) {
        case 'm':
           sendCompareMeanConfidenceResult(oid(),ds1,ds2,m_Params.m_meanConfidenceLevel);
           break;
        case 'M':
           sendCompareMeanHypothesisResult(oid(),ds1,ds2,m_Params.m_meanHypothesisLevel,m_Params.m_meanMU0,m_Params.m_meanMUType,
                                      m_Params.m_equalAssumed);
           break;
       case 'V':
           sendCompareVarianceHypothesisResult(oid(),ds1,ds2,m_Params.m_varHypothesisLevel);
           break;
       case 'f':
           switch (m_Params.m_fitType) {
              case '1':
                  sendNormalResult(oid(),ds1,m_Params.m_fitLevel);
                  sendNormalResult(oid(),ds2,m_Params.m_fitLevel);
                  break;
              case '2':
                  sendExpResult(oid(),ds1,m_Params.m_fitLevel);
                  sendExpResult(oid(),ds2,m_Params.m_fitLevel);
                  break;
              case '3':
                  sendUniformResult(oid(),ds1,m_Params.m_fitLevel);
                  sendUniformResult(oid(),ds2,m_Params.m_fitLevel);
                  break;
           }
           break;
       case 'F':
           sendFullAnaysisResult(oid(),ds1);
           sendFullAnaysisResult(oid(),ds2);
           break;
       case 'n':
           sendNonParametricResult(oid(),ds,ds1,ds2,m_Params.m_nonParametricLevel);
           break;

     }
  }
}

void SMDataSetResidualAnalysis::sendResult( ) const{
    TwoVariableDataSet ds(theDataSet.getSet());
    sendResidualAnalysisResult(oid(),ds);
}

void SMDataSetAlternateModelAnalysis::sendResult( ) const{
    TwoVariableDataSet ds(theDataSet.getSet());
    sendAlternateModelResult(oid(),ds,m_Type);
}

void SMDataSetRegressionCorrelationAnalysis::sendResult( ) const{
   TwoVariableDataSet ds(theDataSet.getSet());
   if(m_Type=='C')
      sendCorrelationConfidenceResult(oid(),ds,m_Level);
   else
      sendCorrelationHypothesisResult(oid(),ds,m_Level,m_MU0,m_MU0Type);
}


void DataSetResidualAnalysisResultSend::sendResidualAnalysisResult(
				   OID oid,TwoVariableDataSet ds) const {
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     {  LogMessage msg( oid,0);
         msg <<  "   Residual Analysis";
         msg.send();}  
     {  LogMessage msg( oid,0);
        msg <<  "  The fitted regression line was Y= "
            << ds.GetSlope() << "X+" << ds.GetIntercept();
       msg.send();}
     {  LogMessage msg( oid,0);
        msg <<  "   The coefficient of determination was "
            << pow(ds.GetR(),2);
       msg.send();}


    {  LogMessage msg( oid,0);
       msg <<  "   The correlation coefficient was "
           << ds.GetR();
       msg.send();}

}

void DataSetCorrelationAnalysisResultSend::sendCorrelationConfidenceResult(
                		 OID oid,TwoVariableDataSet ds,double Level) const{
     int n=ds.GetSize();;
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     ///calculate A1 and A2
     double Z1=statsFunctions::NormInv(Level/2.0);
     double FZ=log((1.0+ds.GetR())/(1.0-ds.GetR()))/2.0;
     double A=FZ-2.0*Z1/sqrt(n-3);
     double B=FZ+2.0*Z1/sqrt(n-3);
     double A1=(exp(A)-1.0)/(exp(A)+1.0);
     double A2=(exp(B)-1.0)/(exp(B)+1.0);
 
     {  LogMessage msg( oid,0);
         msg << "   Confidence Interval for a Correlation Coefficient";
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg << "  A " << 100*Level << "% confidence interval for the correlation coefficient is";
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg << "              " << A1 << "<= p <=" << A2 ;
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg << "  This was based on " << n-3 << " degrees of freedom";
         msg.send();} 

}

double DataSetCorrelationAnalysisResultSend::getP(double Z,char MU0Type) const {
   switch (MU0Type) {
     case '1': return 1-statsFunctions::Norm(fabs(Z));
     case '2': return 1-statsFunctions::Norm(Z);
     case '3': return statsFunctions::Norm(Z);
   }
   return 0;
}

void DataSetCorrelationAnalysisResultSend::sendCorrelationHypothesisResult(
                 OID oid,TwoVariableDataSet ds,double Level,double MU0,char MU0Type) const{
    if(!ds.IsEqualSize()){
         LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();
         return;
     }
    int n=ds.GetSize();
    if(n<4){
         LogMessage msg( oid,0);
         msg <<  " Measurement of dataset is less than 4!";
         msg.send();
         return;
     }
    Chars MuType;
    Chars isSign="significant";
    double FZ=log((1.0+ds.GetR())/(1.0-ds.GetR()))/2.0;
    double FZ1=log((1.0+MU0)/(1.0-MU0))/2.0;
    double Z=(FZ-FZ1)*sqrt(n-3.0);
    double P=getP(Z,MU0Type);
    if(P>=Level)
        isSign="not significant";
    switch (MU0Type) {
      case '1': MuType="MU \\u2260 "; break;
      case '2': MuType="MU > "; break;
      case '3': MuType="MU < "; break;
    }

     {  LogMessage msg( oid,0);
         msg <<  "   Hypothesis Test for a Correlation Coefficient";
         msg.send();} 

     {  LogMessage msg( oid,0);
         msg <<  "  The Null Hypothesis was mu = " << MU0;
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg <<  "  The Alternative Hypothesis was";
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg <<  "             " << MuType << MU0;
         msg.send();} 

     {  LogMessage msg( oid,0);
         msg <<  "  The Computed Correlation Coefficient was " << ds.GetR();
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg <<  "  The Computed Z-value was " << Z;
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg <<  "  The Computed P-value was " << P;
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg <<  "  This is " << isSign << " at " << 100*Level
             << "% level of significance against the given alternative.";
         msg.send();} 
     {  LogMessage msg( oid,0);
         msg <<  "  This was based on " << n-3 << " degrees of freedom.";
         msg.send();} 
}

void DataSetAlternateModelAnalysisResultSend::sendExponentialModel(
			OID oid,TwoVariableDataSet ds) const{
     double A=ds.GetLogA();
     double B=ds.GetLogB();
     {  LogMessage msg( oid,0);
         msg <<  "   EXPONENTIAL MODEL FIT";
         msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The fitted exponential model is "
             <<  "   Y=" << A << "*(" << B
             <<  "^X)";
         msg.send();}
   
     {  LogMessage msg( oid,0);
         msg <<  "   This had a coefficient of determination of "
             <<  "   R^2 = "<< ds.getQSqure(A,B) ;
         msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The corresponding linear model is "
             <<  "   Y= " << ds.GetIntercept()  << " + " << ds.GetSlope() << " * X ";
         msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The linear model had a coefficient of determination of "
             <<  "   R^2 = " << pow(ds.GetR(),2);
         msg.send();}
   
}

void DataSetAlternateModelAnalysisResultSend::sendPowerModel(
			OID oid,TwoVariableDataSet ds) const{

     double A=ds.GetLogLogA();
     double B=ds.GetLogLogB();
     {  LogMessage msg( oid,0);
         msg <<  "   POWER MODEL FIT";
         msg.send();}   

     {  LogMessage msg( oid,0);
         msg <<  "   The fitted power model is "
             <<  "   Y=" << A << "*(" << B
             <<  "^X)";
         msg.send();}
   
     {  LogMessage msg( oid,0);
         msg <<  "   This had a coefficient of determination of "
             <<  "   R^2 = "<< ds.getPSqure(A,B) ;
         msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The corresponding linear model is "
             <<  "   Y= " << ds.GetIntercept()  << " + " << ds.GetSlope() << " * X ";
         msg.send();}

     {  LogMessage msg( oid,0);
         msg <<  "   The linear model had a coefficient of determination of "
             <<  "   R^2 = " << pow(ds.GetR(),2);
         msg.send();}

}

void DataSetAlternateModelAnalysisResultSend::sendAlternateModelResult(
			OID oid,TwoVariableDataSet ds,char Type) const{
     if(!ds.IsEqualSize()){
        {  LogMessage msg( oid,0);
         msg <<  " Measurements of dataset are not equal!";
         msg.send();}
         return;
     }
     if(Type=='1') 
        sendExponentialModel(oid,ds);
     else
        sendPowerModel(oid,ds);
};

void TVDSAllPredictionParam::eatWhite( istream& istr, int& pos ){
  while( !istr.eof() ) {
    char ch = istr.peek();
    if( ch == ' ' || ch == '\t' || ch == '\n' ) {
      ++pos;
      istr.get();
    }
    else
      break;
  }
}
void TVDSAllPredictionParam::readFrom( istream& istr, Chars& errMsg ){
  // We expect a list of numbers separated by commas and enclosed in
  // braces. { { S{Type level MU0 Type }} { I{Type level MU0 Type }}
   //  {M {level } } {P {level X0} } }
  //
  int pos = 0;
  double d;
  eatWhite(istr,pos);
  if( istr.eof() || istr.peek() != '{' ) {
    errMsg = " { expected ";
    return;
  }
  
  istr.get(); eatWhite(istr,pos);
  
  while( !istr.eof() && istr.peek() != '}'  ) {
     eatWhite(istr,pos);
     istr.get();   ///get first {
     char type; 
     istr >> type;
     eatWhite(istr,pos);
     istr.get();   ///read second {
     eatWhite(istr,pos);
     switch(type) {
       case 'S':
          readSlope(istr,errMsg);
          break;
       case 'I':
          readIntercept(istr,errMsg);
          break;
       case 'P':
          readPrediction(istr,errMsg);
          break;
       case 'M':
          readModel(istr,errMsg);
          break;
     }
     eatWhite(istr,pos);
     istr.get();  ///read first }
     eatWhite(istr,pos);
     istr.get();  ///read second }
     eatWhite(istr,pos);
  }
}

void TVDSAllPredictionParam::readSlope(istream& istr, Chars& errMsg ){
  char Type;
  istr >> Type;
  if(Type=='C'){
      istr >> m_SlopeConfidenceLevel;
  } else {
      istr >> m_SlopeHypothesisLevel;
      istr >> m_SlopeHypothesisMU0;
      istr >> m_SlopeHypothesisMU0Type;
  }
}

void TVDSAllPredictionParam::readIntercept(istream& istr, Chars& errMsg ){
  char Type;
  istr >> Type;
  if(Type=='C'){
      istr >> m_InterceptConfidenceLevel;
  } else {
      istr >> m_InterceptHypothesisLevel;
      istr >> m_InterceptHypothesisMU0;
      istr >> m_InterceptHypothesisMU0Type;
  }
}

void TVDSAllPredictionParam::readPrediction(istream& istr, Chars& errMsg ){
      istr >> m_PredictLevel;
      istr >> m_PredictX0;
   
}

void TVDSAllPredictionParam::readModel(istream& istr, Chars& errMsg ){
      istr >> m_ModelLevel;
}
