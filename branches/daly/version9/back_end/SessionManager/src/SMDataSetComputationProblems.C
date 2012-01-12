// Copyright (C) 1995 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.

// Contents: Implementation of class SMDataSetComputationProblems
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//


#include "SMDataSetComputationProblems.h"
#include "OutMessages.h"
#include "statsFunctions.h"
#include "math.h"
#include "List.h"
void DataSetClass::init(){
      for(int i=0;i<CLASSMAXSIZE;i++){
        if(i==0)
          dcSet[i].lower=theSet.min();
        else
          dcSet[i].lower=dcSet[i-1].upper;   ///to be continue
        dcSet[i].upper=theSet.min()+(theSet.max()-theSet.min())*(i+1.0)/CLASSMAXSIZE+0.0001;  ///avoid small lost
        dcSet[i].frequency=theSet.GetNum(dcSet[i].lower,dcSet[i].upper);
     }
}

void DataSetClass::adjust(){
     int i;
     ///combine
     for(i=0;i<CLASSMAXSIZE-1;i++){
        if(dcSet[i].frequency<5){
           dcSet[i+1].lower=dcSet[i].lower;
           dcSet[i+1].frequency+=dcSet[i].frequency;
           dcSet[i].frequency=0;
        }
     }
   ///combine the uppermost
     if(dcSet[CLASSMAXSIZE-1].frequency<5){
        for(i=CLASSMAXSIZE-2;i>=0;i--){
          if(dcSet[i].frequency>0)
             break;        
        }
        dcSet[i].upper=dcSet[CLASSMAXSIZE-1].upper;
        dcSet[i].frequency+=dcSet[CLASSMAXSIZE-1].frequency;
        dcSet[CLASSMAXSIZE-1].frequency=0;
    }
}
int DataSetClass::GetSize(){
     int num=0;
     for(int i=0;i<CLASSMAXSIZE;i++)
        if(dcSet[i].frequency>0)
           num++;
     return num;
}

DataSetItem DataSetClass::operator [ ] ( int index ) const {
     int num=-1;
     for(int i=0;i<CLASSMAXSIZE;i++){
        if(dcSet[i].frequency>0)
           num++;
        if(num==index)
          return dcSet[i];
     }
}


void SMDataSetFullAnalysis::sendResult( ) const{
  fullAnalysisResult();
}

Chars MakeNSpaceChars(int n) {
  Chars rst=" ";
  for(int i=1;i<n;i++)
    rst+=" ";
  return rst;
}

int GetLengthOfLog(Chars& text) {
  return text.length();
}

void FixLengthOfLog(LogMessage& msg,Chars text){
  int length=12;
  Chars strApp=MakeNSpaceChars(length-GetLengthOfLog(text));
  msg  << strApp;
}

void SMDataSetFullAnalysis::fullAnalysisResult() const{
  int Dimension=theDataSet.getSet().getDimension();
  OneVariableDataSet ds1(theDataSet.getSet(),0);
  {LogMessage msg( oid(), 0);
         msg <<  "   FULL DATA ANALYSIS FOR " << theDataSet.getName();
         msg.send();}
   
  if(Dimension==2){
	 LogMessage msg( oid(), 0);
         msg <<  "   First Sample";
         msg << "                           Second Sample";
         msg.send();
  }

  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Mean=","FurtherAnylysis","helpdvi/meanAnalysis" ) << "              "<< Chars(ds1.mean()); 
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.mean()));
               msg <<" Mean=" << "                   " << Chars(ds2.mean());
     }
     msg.send();}

  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Variance=","FurtherAnylysis",
       "helpdvi/varianceAnalysis" ) << "            " << Chars(ds1.deviation());
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.deviation()));
               msg <<" Variance=" << "               " << Chars(ds2.deviation());
     }
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Standard  Deviation=","FurtherAnylysis",
  		    "helpdvi/standardDeviationAnalysis" ) << "     " << Chars(ds1.standardDeviation());
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.standardDeviation()));
               msg << " Standard Deviation="<< "     "  << Chars(ds2.standardDeviation());
     }
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Range=  ","FurtherAnylysis",
  		     "helpdvi/rangeAnalysis" )<< "             "  << Chars(ds1.range()); 
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.range()));
               msg << " Range=" << "                  " << Chars(ds2.range());
     }
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Median=  ","FurtherAnylysis",
  		    "helpdvi/medianAnalysis" ) << "            " << Chars(ds1.median()); 
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.median()));
               msg << " Median=" << "                 " << Chars(ds2.median());
     }
      msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(" First   Quartile=","FurtherAnylysis",
  		     "helpdvi/firstQuartileAnalysis" )  <<"        "<< Chars(ds1.firstQuartile()); 
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.firstQuartile()));
               msg << " First Quartile=" << "         " << Chars(ds2.firstQuartile());
     }
      msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Third  Quartile=","FurtherAnylysis",
  		      "helpdvi/thirdQuartileAnalysis" ) << "        " << Chars(ds1.thirdQuartile()); 
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.thirdQuartile()));
               msg <<" Third Quartile=" << "         " << Chars(ds2.thirdQuartile());
     }
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Coefficient of Skewness=","FurtherAnylysis",
  		     "helpdvi/coefficientSkewnessAnalysis" ) << "  "<< Chars(ds1.coefficientSkewness()); 
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.coefficientSkewness()));
               msg << " Coefficient of Skewness=" << Chars(ds2.coefficientSkewness());
     }
     msg.send();}
  {  LogMessage msg( oid(), 0);
     msg <<  Link(" Number  of  Measurements=","FurtherAnylysis",
  		    "helpdvi/numberOfMeasurement" ) << ds1.cardinality(); 
     if(Dimension==2){
               OneVariableDataSet ds2(theDataSet.getSet(),1);
               FixLengthOfLog(msg,Chars(ds1.cardinality()));
               msg <<" Number of Measurements=" << " "<<  ds2.cardinality();
     }
     msg.send();}

}

void MeanAnalysisResultSend::sendMeanHyporesult(OID oid,OneVariableDataSet& ds,double Level,double MU0,char MU0Type) const{
  double XBar=ds.mean();
  double E=getE(ds,Level);
  double Z;
  Z=sqrt(ds.cardinality())*(XBar-MU0)/ds.standardDeviation();
  double P=getP(ds,Z,MU0Type);
  Chars isLarge="large";
  Chars ZT="Z";
  Chars MuType;
  if (ds.cardinality()<30){
     isLarge="small"; 
     ZT="T";
  }
  Chars isSignificant="is not significant";
  if(P<Level)
    isSignificant="is significant";

  {  LogMessage msg( oid, 0);
     msg <<  "      HYPOTHESIS TEST FOR THE MEAN";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The null hypothesis was " ;
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "       Mu=" <<  MU0;
     msg.send();}

   switch (MU0Type) {
     case '1': MuType="Mu \\u2260 "; break;
     case '2': MuType="Mu > "; break;
     case '3': MuType="Mu < "; break;
   }
  {  LogMessage msg( oid, 0);
     msg <<   "The alternative hypothesis was  ";
	 msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "       " << MuType << MU0;
	 msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The computed mean was " << XBar
         << " and the computed standard deviation was " << ds.standardDeviation()
         << " based on " << ds.cardinality()  << " Measurements.";
     msg.send();}

  
  {  LogMessage msg( oid, 0);
     msg <<  "The computed " << ZT << " value was " << Z << ".";
	 msg.send();}
  {
     LogMessage msg( oid, 0);
     msg <<  "The computed P-value was " 
         << P << ".";
	    msg.send();
  }
 {
     LogMessage msg( oid, 0);
     msg <<  "This " << isSignificant << " at the " 
         << 100* Level << "% level of significance.";
     msg.send();
  }
 {
     LogMessage msg( oid, 0);
     msg <<  "The results are significant at any p-value level " 
         << "less than " << P << ".";
     msg.send();
  }
 {
     LogMessage msg( oid, 0);
     msg <<  "This was a " 
         << isLarge << " sample procedure.";
     msg.send();
 }

}
void MeanAnalysisResultSend::sendMeanConfidence(OID oid,OneVariableDataSet& ds,double Level) const{
  double XBar=ds.mean();
  double E=getE(ds,Level);
  double Z;
  if(ds.cardinality()<30)
      Z=statsFunctions::TInv(0.5+Level/2,ds.cardinality()-1);
  else
      Z=statsFunctions::NormInv(0.5+Level/2);
  double T=0.1;
  Chars isLarge="large";
  {  LogMessage msg( oid, 0);
     msg <<  "           CONFIDENCE INTERVAL FOR THE MEAN";
             
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "A " << Level*100 <<"% "
		 <<"confidence interval for the mean is";
	 msg.send();}
  {  LogMessage msg( oid, 0);
     msg << "       " << XBar - fabs(E) << " <= u <= "
         << XBar + fabs(E)`;
	 msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "       Equivalently ";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "the population mean is estimated as " << XBar
         << " with a " << Level*100 <<"% confidence that it lies within "
         << "plus or minus " << E <<".";
	 msg.send();}


  if(ds.cardinality()<30)
	  isLarge="small";
  {  LogMessage msg( oid, 0);
     msg <<  "This is a " << isLarge 
		 <<  " sample procedure. The appropriate confidence coefficient is "
         << Z << ".";
	 msg.send();}
  if(ds.cardinality()<30){
	    LogMessage msg( oid, 0);
        msg <<  "This is based on " << ds.cardinality()-1 
		 <<  " degrees of freedom. The symmetry should be checked to"
         << " insure the appropriateness of the small sample procedure.";
	    msg.send();
  }

}


void SMDataSetMeanAnalysis::sendResult() const{
  OneVariableDataSet ds(theDataSet.getSet());
  if(m_Type=='H')
     sendMeanHyporesult(oid(),ds,m_Level,m_MU0,m_MU0Type);
  else 
     sendMeanConfidence(oid(),ds,m_Level);
}

double MeanAnalysisResultSend::getE(OneVariableDataSet& ds,double Level) const{
	int size=ds.cardinality();
	if(size>=30){
             return statsFunctions::NormInv(0.5+Level/2)*ds.standardDeviation()/sqrt(size);
	}else {
		return statsFunctions::TInv(0.5+Level/2,size-1)*ds.standardDeviation()/sqrt(size);
	}
}

double MeanAnalysisResultSend::getP(OneVariableDataSet& ds,double Z,char MU0Type) const{
  int size=ds.cardinality();
  if(size>=30)
    switch(MU0Type){
       case '1':  ///MU != MU0
         return 1-statsFunctions::Norm(fabs(Z));
    
       case '2':  ///MU > MU0
          return 1-statsFunctions::Norm(Z);

       case '3':  ///MU < MU0
          return statsFunctions::Norm(Z);
    }
  else
    switch(MU0Type){
       case '1':  ///MU != MU0
         return 1-statsFunctions::TDis(fabs(Z),size-1);
    
       case '2':  ///MU > MU0
          return 1-statsFunctions::TDis(Z,size-1);

       case '3':  ///MU < MU0
          return statsFunctions::TDis(Z,size-1);
    }

  return 0.00;
}

void VarianceAnalysisResultSend::sendVarianceHypoResult(OID oid,OneVariableDataSet& ds,double Level,double MU0,char MU0Type) const{
  double XBar=ds.mean();
  double Z;
  Chars ZT;
  int N=ds.cardinality();
  if(N>=30)
    Z=sqrt(N*2.0)*(ds.standardDeviation()-MU0)/MU0;
  else
    Z=(N-1)*pow(ds.standardDeviation(),2)/pow(MU0,2);
  double P=getP(ds,Z,MU0Type);
  Chars isLarge="large";
  if (ds.cardinality()<30){
     isLarge="small";
     ZT="Chi";
  }
  Chars isSignificant="is not significant";
  if(P<Level)
    isSignificant="is significant";

  {  LogMessage msg( oid, 0);
     msg <<  "     HYPOTHESIS TEST CONCERNING A SINGLE VARIANCE";
              
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The NULL Hypothesis was " 
         << MU0 <<".";
     msg.send();}
   Chars MuType;
   switch (MU0Type) {
     case '1': MuType="sigma \\u2260 "; break;
     case '2': MuType="sigma > "; break;
     case '3': MuType="sigma < "; break;
   }

  {  LogMessage msg( oid, 0);
     msg <<   "The Alternative Hypothesis was ";
	 msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<   "      " << MuType << MU0;
	 msg.send();}

  {  LogMessage msg( oid, 0);

     msg <<  "The computed Mean and Standard Deviation were" ;
     msg.send();}

  {  LogMessage msg( oid, 0);

     msg <<  "       ("
         << XBar << "," << ds.standardDeviation() << ")";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "based on " << ds.cardinality()
         << " Measurements.";
	 msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "The computed " << ZT << " value was " << Z << ".";
	 msg.send();}
  {
     LogMessage msg( oid, 0);
     msg <<  "The computed P-value was " 
         << P << ".";
	    msg.send();
  }
 {
     LogMessage msg( oid, 0);
     msg <<  "This " << isSignificant << " at the " 
         << 100* Level << "% level of significance.";
     msg.send();
  }
 {
     LogMessage msg( oid, 0);
     msg <<  "The results are significant at any significance level " 
         << "less than " << P << ".";
     msg.send();
  }
 {
     LogMessage msg( oid, 0);
     msg <<  "This was a " 
         << isLarge << " sample procedure.";
     msg.send();
 }

}

void VarianceAnalysisResultSend::sendVarianceConfidenceResult(OID oid,OneVariableDataSet& ds,double Level) const{
  double Min=getIntervalMin(ds,Level);
  double Max=getIntervalMax(ds,Level);
  Chars isLarge="large";
  if (ds.cardinality()<30)
     isLarge="small";
  {  LogMessage msg( oid, 0);
     msg <<  "         CONFIDENCE INTERVAL FOR THE VARIANCE";
              
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "A " << 100*Level 
         << "% confidence interval for the variance is ";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<   "           " 
         << Min << "< SIGMA <" << Max;
	 msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "A " << 100*Level 
         << "% confidence interval for the standard variance is ";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<   "           " 
         << Min*Min << "< SIGMA^2 <" << Max*Max;
	 msg.send();}

 {
     LogMessage msg( oid, 0);
     msg <<  "This was a " 
         << isLarge << " sample procedure.";
     msg.send();
 }

}

void SMDataSetVarianceAnalysis::sendResult() const{
  OneVariableDataSet ds(theDataSet.getSet());
  if(m_Type=='H')
     sendVarianceHypoResult(oid(),ds,m_Level,m_MU0,m_MU0Type);
  else 
     sendVarianceConfidenceResult(oid(),ds,m_Level);
}

double VarianceAnalysisResultSend::getIntervalMin(OneVariableDataSet& ds,double Level) const{
  double r;
  int N=ds.cardinality();
  if(N>=30){
    double Z=statsFunctions::NormInv((1.0+Level)/2.0);
    double E1=1+Z/sqrt(2.0*N);
    r=ds.standardDeviation()/E1;
  }else{
    double Chi1=statsFunctions::ChiInv((1.0-Level)/2.0,N-1);
    r=sqrt(N-1.0)*ds.standardDeviation()/sqrt(Chi1);
  }  
  return r;
}
double VarianceAnalysisResultSend::getIntervalMax(OneVariableDataSet& ds,double Level) const{
  double r;
  int N=ds.cardinality();
  if(N>=30){
    double Z=statsFunctions::NormInv((1.0+Level)/2.0);
    double E2=1-Z/sqrt(2.0*N);
    r=ds.standardDeviation()/E2;
  }else{
    double Chi2=statsFunctions::ChiInv((1.0+Level)/2.0,N-1);
    r=sqrt(N-1)*ds.standardDeviation()/sqrt(Chi2);
  }  
  return r;
}

double VarianceAnalysisResultSend::getP(OneVariableDataSet& ds,double Z,char MU0Type) const{
  int size=ds.cardinality();
  if(size>=30)
    switch(MU0Type){
       case '1':  ///MU != MU0
         return 1-statsFunctions::Norm(fabs(Z));
    
       case '2':  ///MU > MU0
          return 1-statsFunctions::Norm(Z);

       case '3':  ///MU < MU0
          return statsFunctions::Norm(Z);
    }
  else
    switch(MU0Type){
       case '1':  ///MU != MU0
         return 1-statsFunctions::Chi(Z,size-1);
    
       case '2':  ///MU > MU0
          return 1-statsFunctions::Chi(fabs(Z),size-1);

       case '3':  ///MU < MU0
          return statsFunctions::Chi(Z,size-1);
    }

  return 0.00;
}

void SMDataSetGoodnessOfFit::sendResult( ) const{
  OneVariableDataSet ds(theDataSet.getSet());
  switch(m_Type){
       case '1':  ///normal
         sendNormalResult(oid(),ds,m_Level);
         break;
       case '2':  ///expotential
         sendExpResult(oid(),ds,m_Level);
         break;
       case '3':  ///uniform
         sendUniformResult(oid(),ds,m_Level);
         break;
  }
}

void GoodnessOfFitAnalysisResultSend::sendNormalResult(OID oid,OneVariableDataSet& ds,double Level) const{
  int N=ds.cardinality();
  double U=sqrt(PICONST/(2.0*N))*ds.variance()/(ds.standardDeviation()*sqrt(N-1));
  double Z=sqrt(N)/0.2661*(U-1);
  double P=getP(N,Z,'1');
  if(N<20){ 
     {LogMessage msg( oid, 0);
     msg <<  "         GOODNESS OF FIT ANALYSIS ";
     msg.send();}
     {LogMessage msg( oid, 0);
     msg <<  "The goodness of fit test cannot be applied because the data set "
         << "size less than 20.";
     msg.send();}

    return;
  }
  {  LogMessage msg( oid, 0);
     msg <<  "         GOODNESS OF FIT TEST ";
              
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The data was tested to fit a normal distribution.";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<   "There was a mean of " 
         << ds.mean()
         << " and  a standard deviation of "
         << ds.standardDeviation() << ".";
	 msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "The P-value for Geary's goodness of fit test for normality was ";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "               "
         << P <<"";
     msg.send();}

  Chars isFit=" fits ";
  if(P<=Level)
     isFit=" does not fit ";
  {  LogMessage msg( oid, 0);
     msg <<   "This data"<< isFit <<"a normal distribution at a " 
         << Level*100 <<"% level of significance.";
	 msg.send();}
}

double GoodnessOfFitAnalysisResultSend::getExpZ(OneVariableDataSet& ds,DataSetClass& dc) const{
  double mean=ds.mean();
  double randa=1.0/mean;
  int N=ds.cardinality();
  double E[CLASSMAXSIZE];
  double sumPrev=0;
  E[0]=statsFunctions::Expo(dc[0].upper,randa)*N;
  sumPrev=E[0];
  for(int i=1;i<dc.GetSize()-1;i++){
     E[i]=(statsFunctions::Expo(dc[i].upper,randa)-statsFunctions::Expo(dc[i].lower,randa))*N;
     sumPrev+=E[i];
  }
  E[dc.GetSize()-1]=N-sumPrev;
  double sum=0;
  for(int i=0;i<dc.GetSize();i++){
     sum+=pow(dc[i].frequency-E[i],2)/E[i];
  }
  return sum;
}

double GoodnessOfFitAnalysisResultSend::getUnifZ(OneVariableDataSet& ds,DataSetClass& dc) const{
  double mean=ds.mean();
  double randa=1.0/mean;
  int N=ds.cardinality();
  double E[CLASSMAXSIZE];
  double sumPrev=0;
  E[0]=statsFunctions::Unif(dc[0].upper,ds.min(),ds.max())*N;
  sumPrev=E[0];
  for(int i=1;i<dc.GetSize()-1;i++){
     E[i]=(statsFunctions::Unif(dc[i].upper,ds.min(),ds.max())-statsFunctions::Unif(dc[i].lower,ds.min(),ds.max()))*N;
     sumPrev+=E[i];
  }
  E[dc.GetSize()-1]=N-sumPrev;
  double sum=0;
  for(int i=0;i<dc.GetSize();i++){
     sum+=pow(dc[i].frequency-E[i],2)/E[i];
  }
  return sum;}

void GoodnessOfFitAnalysisResultSend::sendExpResult(OID oid,OneVariableDataSet& ds,double Level) const{
  int N=ds.cardinality();
  DataSetClass dc(ds);
  dc.init();
  dc.adjust();
  double Z=getExpZ(ds,dc);
  double P=getP(dc.GetSize(),Z,'2');
  if(N<40){ 
     {LogMessage msg( oid, 0);
     msg <<  "         GOODNESS OF FIT TEST ";
     msg.send();}
     {LogMessage msg( oid, 0);
     msg <<  "The goodness of fit test cannot be applied because the data set "
         << "size less than 40.";
     msg.send();}

    return;
  }
  {  LogMessage msg( oid, 0);
     msg <<  "         GOODNESS OF FIT TEST ";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "The data was tested to fit an expotential distribution.";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<   "There was a mean of (" 
         << ds.mean()
         << ") and therefore an estimated parameter of "
         << 1.0/ds.mean() << ".";
	 msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "The P-value for a Chi-Square goodness of fit test for normality was ";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "           "
         << P;
     msg.send();}

  Chars isFit=" fits ";
  if(P<=Level)
     isFit=" does not fit ";
  {  LogMessage msg( oid, 0);
     msg <<   "This data"<< isFit <<"an exponential distribution at a " 
         << Level*100 << "% level of significance.";
	 msg.send();}

}

void printDC(DataSetClass& dc){
  for(int i=0;i<dc.GetSize();i++){
     LogMessage msg;
     msg << dc[i].lower <<"  " << dc[i].upper<< " " <<dc[i].frequency;
     msg.send();
  }
}

void GoodnessOfFitAnalysisResultSend::sendUniformResult(OID oid,OneVariableDataSet& ds,double Level) const{
  int N=ds.cardinality();
  DataSetClass dc(ds);
  dc.init();
//  printDC(dc);
  dc.adjust();
//  printDC(dc);
  double Z=getUnifZ(ds,dc);
  double P=getP(dc.GetSize(),Z,'3');
//  printDC(dc);
  if(N<40){ 
     {LogMessage msg( oid, 0);
     msg <<  "         GOODNESS OF FIT TEST ";
     msg.send();}
     {LogMessage msg( oid, 0);
     msg <<  "The goodness of fit test cannot be applied because the data set "
         << "size less than 40.";
     msg.send();}

    return;
  }
  {  LogMessage msg( oid, 0);
     msg <<  "         GOODNESS OF FIT TEST ";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The data was tested to fit a uniform distribution.";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<   "There was a minimum of " 
         << ds.min()
         << " and a maximum "
         << ds.max() << "."; 
	 msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "The P-value for a Chi-Square goodness of fit test for normality was ";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "           "
         << P;
     msg.send();}

  Chars isFit=" fits ";
  if(P<=Level)
       isFit=" does not fit ";
  {  LogMessage msg( oid, 0);
     msg <<   "This data"<< isFit <<"a uniform distribution at a " 
         << Level*100 << "% level of significance.";
	 msg.send();}
}

double GoodnessOfFitAnalysisResultSend::getP(int N,double Z,char Type) const{
    switch(Type){
       case '1':  ///normal
         return 1.0-statsFunctions::Norm(fabs(Z));
       case '2':  ///expotential
         return 1.0-statsFunctions::Chi(Z,N-2);
       case '3':  ///uniform
         return 1.0-statsFunctions::Chi(Z,N-2);
    }
}

void DataSetCompareMeanResultSend::sendCompareMeanConfidenceResult(OID oid,OneVariableDataSet ds1, 
                   OneVariableDataSet ds2, double Level)const {
  double X1=ds1.mean();
  double S1=ds1.standardDeviation();
  int N1=ds1.cardinality();
  double X2=ds2.mean();  
  double S2=ds2.standardDeviation();
  int N2=ds2.cardinality();
  double W=fmax(X1,X2)-fmin(X1,X2);
  double J1=sqrt(pow(S1,2)/N1+pow(S2,2)/N2);
  double Vd=pow(pow(S1,2)/N1+pow(S2,2)/N2,2)/(pow(S1,4)/(pow(N1,2)*(N1-1))+pow(S2,4)/(pow(N2,2)*(N2-1)));
  int V=(int)Vd;
  double E;
  Chars isLarge="large";
  if(N1>=30&&N2>=30)
    E=statsFunctions::NormInv(1-Level/2.0)*J1;
  else {
    isLarge="small";
    E=statsFunctions::TInv(1-Level/2.0,V)*J1;
  }

  {  LogMessage msg( oid, 0);
     msg <<  "    COMPARISON OF MEANS FOR A TWO VARIABLE DATA SET ";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "    CONFIDENCE INTERVAL FOR THE DIFFERENCE OF MEANS ";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "A (" << 100*Level << ")% confidence interval for the difference mean is ";
     
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "       " << W-E << "< u < " << W+E;
     
     msg.send();}
  {  LogMessage msg( oid, 0);
  msg <<  "This was a " << isLarge << " sample procedure. " 
      << " The appropriate confidence coefficient was " 
      << "Z,T";
     msg.send();} 

}

void DataSetCompareMeanResultSend::sendCompareMeanHypothesisResult(OID oid,OneVariableDataSet ds1,OneVariableDataSet ds2, 
               double Level,double MU0,char MU0Type,bool EqualAssumed) const{
  double X1=ds1.mean();
  double S1=ds1.standardDeviation();
  int N1=ds1.cardinality();
  double X2=ds2.mean();  
  double S2=ds2.standardDeviation();
  int N2=ds2.cardinality();
  double W=fmax(X1,X2)-fmin(X1,X2);
  double J1=sqrt(pow(S1,2)/N1+pow(S2,2)/N2);
  double S=sqrt(((N1-1)*S1*S1+(N2-1)*S2*S2)/(N1+N2-2));
  double J2=S*sqrt(1.0/N1+1.0/N2);
  double V1d=pow(pow(S1,2)/N1+pow(S2,2)/N2,2)/(pow(S1,4)/(pow(N1,2)*(N1-1))+pow(S2,4)/(pow(N2,2)*(N2-1)));
  int V1=(int)V1d;
  int V2=N1+N2-2;
  Chars isZ="T";
  Chars isSignificant="is";
  
  ///calculate Z
  double Z;
  Chars MuType;
  switch (MU0Type) {
     case '1': MuType="Mu \\u2260 "; break;
     case '2': MuType="Mu > "; break;
     case '3': MuType="Mu < "; break;
  }

  if(N1>=30 && N2>=30){
      isZ="Z";
      Z=(X1-X2-MU0)/J1;
  }else if(EqualAssumed)
           Z=(X1-X2-MU0)/J2;
       else
           Z=(X1-X2-MU0)/J1;
  
  double P=GetP(Z,N1,N2,V1,V2,MU0Type,EqualAssumed);
  if(Level<P)
      isSignificant="is not";

  {  LogMessage msg( oid, 0);
     msg <<  "    COMPARISON OF MEANS FOR A TWO VARIABLE DATA SET ";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "         HYPOTHESIS TEST FOR COMPARING TWO MEANS ";
     msg.send();}
 
  {  LogMessage msg( oid, 0);
     msg <<  "The Null Hypothesis was (Mu1-Mu2=" 
         << MU0  << ")";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The Alternative Hypothesis was " << MuType << MU0;
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "The Computed Means and standard Deviations were " 
         << "(" << X1 << "," << S1 << ")"
         << "(" << X2 << "," << S2 << ")";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  " based on " << ds1.cardinality() << " Measurements from the first  sample "
         << "and " << ds2.cardinality() << " from the second sample.";
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The Computed " << isZ << " value was " << Z;
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg <<  "The Computed P-value was " << P;
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "This " << isSignificant << " significant at the " << 100*Level << "% level of significance.";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg <<  "The result are significant at any significance level less than " << P << ".";
     msg.send();}
  if(N1>=30 && N2>=30){
        LogMessage msg( oid, 0);
        msg <<  "This was a large sample procedure.";
        msg.send();}
  else{
        int V;
        V= EqualAssumed ? V1 : V2 ;
        LogMessage msg( oid, 0);
        msg <<  "This was a  small sample procedure, based on "
            << V << " degrees of freedom.";
        msg.send();}

}

double DataSetCompareMeanResultSend::GetP(double Z,int N1,int N2,int V1,int V2,char MU0Type,bool EqualAssumed) const{
   if(N1>=30 && N2>=30){
     switch(MU0Type){
       case '1':  ///MU != MU0
         return 1-statsFunctions::Norm(fabs(Z));
    
       case '2':  ///MU > MU0
          return 1-statsFunctions::Norm(Z);

       case '3':  ///MU < MU0
          return statsFunctions::Norm(Z);
     }
   }else {
      if(EqualAssumed){
        switch(MU0Type){
          case '1':  ///MU != MU0
             return 1-statsFunctions::TDis(fabs(Z),V2);
    
          case '2':  ///MU > MU0
             return 1-statsFunctions::TDis(Z,V2);

          case '3':  ///MU < MU0
             return statsFunctions::TDis(Z,V2);
        }
      }
      else{
        switch(MU0Type){
          case '1':  ///MU != MU0
             return 1-statsFunctions::TDis(fabs(Z),V1);
    
          case '2':  ///MU > MU0
             return 1-statsFunctions::TDis(Z,V1);

          case '3':  ///MU < MU0
             return statsFunctions::TDis(Z,V1);
        }
     }
  }
}


void SMDataSetCompareMeanAnalysis::sendResult() const{
  OneVariableDataSet ds1(theDataSet.getSet(),0);
  OneVariableDataSet ds2(theDataSet.getSet(),1);
  if(m_Type=='H')
    sendCompareMeanHypothesisResult(oid(),ds1,ds2, m_Level,m_MU0,m_MU0Type,m_EqualAssumed);
  else 
    sendCompareMeanConfidenceResult(oid(),ds1,ds2, m_Level);
}

void DataSetCompareVarianceResultSend:: sendCompareVarianceHypothesisResult(OID oid,OneVariableDataSet ds1,OneVariableDataSet ds2,
                          double Level) const{
  double X2=ds2.mean();  
  double X1=ds1.mean();
  double S1=ds1.standardDeviation();
  int N1=ds1.cardinality();
  double S2=ds2.standardDeviation();
  int N2=ds2.cardinality();
  double J=sqrt(pow(S1,2)/(2*N1)+pow(S2,2)/(2*N2));
  double Z=(S1-S2)/J;
  double P;
  char isZ='Z';
  Chars isLarge="large";
  Chars isSignificant="significant";
  if(N1>=30&&N2>=30)
    P=1-statsFunctions::Norm(fabs(Z));
  else {
    isLarge="small";
    isZ='F';
    double SA=fmax(S1,S2);
    double SB=fmin(S1,S2);
    
    Z=pow(SA,2)/pow(SB,2);
    if(S1>S2)
      P=1-statsFunctions::FDis(Z,N1-1,N2-1);
    else
      P=1-statsFunctions::FDis(Z,N2-1,N1-1);
  }
  if(P>Level) 
    isSignificant="not significant";
  {  LogMessage msg( oid, 0);
     msg <<  "         HYPOTHESIS TEST FOR COMPARING TWO VARIANCES  ";

     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << " The NULL Hypothesis was SIGMA 1=SIGMA 2 ";
     
     msg.send();}
  {  LogMessage msg( oid, 0);
     msg << "The Alternative Hypothesis was SIGMA 1\\u2260SIGMA 2 ";
     
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "The Computed Means and Standard Deviations were ";
     
     msg.send();}


  {  LogMessage msg( oid, 0);
     msg << "      (" << X1 << "," << S1 << ")";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "      (" << X2 << "," << S2 << ")";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << " based on " << N1 << "Measurements from the first sample "
         << " and " << N2 << " from the second sample.";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "The Computed " << isZ << " value was " << Z;
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "The Computed P-value was " << P;
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "This is  " << isSignificant << " at the " << 100* Level << "% level of significance.";
     msg.send();}

  {  LogMessage msg( oid, 0);
     msg << "The results are significant at any significance level less than " << P;
     msg.send();}

  {  LogMessage msg( oid, 0);
  msg <<  "This was a " << isLarge << " sample procedure. " 
      << " The appropriate confidence coefficient was " 
      << "Z,T";
     msg.send();} 
   if (N1<30 || N2<30) {
      LogMessage msg( oid, 0);
      msg <<  " based on (" << N1-1 <<"," << N2-1 << ") degrees of freedom."; 
      msg.send();} 

}
void SMDataSetCompareVarianceAnalysis::sendResult() const{
  OneVariableDataSet ds1(theDataSet.getSet(),0);
  OneVariableDataSet ds2(theDataSet.getSet(),1);
  sendCompareVarianceHypothesisResult(oid(),ds1,ds2,m_Level);
}

void SMOVDSReportAnalysis::viewStructure(ostream& ostr) const
{
  ostr
	 << "viewBuilder " << m_ComputationView << " viewParameters {viewID "
	 << oid()
         << " forms { " << m_Params.m_strSummaryGraphicalParam << "}"
	 << " heritage {"
	 << heritage()
	 << "}}";
}

void OVDSReportParam::eatWhite( istream& istr, int& pos )
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

void OVDSReportParam::readFrom( istream& istr, Chars& errMsg )
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
     }
     eatWhite(istr,pos);
     istr.get();  ///read first }
     eatWhite(istr,pos);
     istr.get();  ///read second }
     eatWhite(istr,pos);
  }
}
//F{ }
void OVDSReportParam::readFull(istream& istr, Chars& errMsg ){
}

// d{type cell width startpoint} 
void OVDSReportParam::readDataSummary(istream& istr, Chars& errMsg ){
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
void OVDSReportParam::readGraphic(istream& istr, Chars& errMsg ){
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
void OVDSReportParam::readMean(istream& istr, Chars& errMsg ){
  char MeanType;
  istr >> MeanType;
  if(MeanType=='C'){
      istr >> m_meanConfidenceLevel;
      m_Forms+='m';
  } else {
      istr >> m_meanMU0;
      istr >> m_meanHypothesisLevel;
      istr >> m_meanMUType;
      m_Forms+='M';
  }
}

//v{type(H,C) MU0 level MUType}
void OVDSReportParam::readVariance(istream& istr, Chars& errMsg ){
  char MeanType;
  istr >> MeanType;
  if(MeanType=='C'){
      istr >> m_varConfidenceLevel;
      m_Forms+='v';
  } else {
      istr >> m_varMU0;
      istr >> m_varHypothesisLevel;
      istr >> m_varMUType;
      m_Forms+='V';
  }
}

//f{type(1 2 3 ) level } 

void OVDSReportParam::readFitness(istream& istr, Chars& errMsg ){
  istr >> m_fitType;
  istr >> m_fitLevel;
}

void SMOVDSReportAnalysis::sendResult() const{
   OneVariableDataSet ds(theDataSet.getSet());
  {  LogMessage msg( oid(), 0);
     msg <<  "         Report for Dataset  ";

     msg.send();}

/*  {  LogMessage msg( oid(), 0);
     msg <<  "Summary Param: " << m_Params.m_strSummaryGraphicalParam 
         <<   "Fit type: " << m_Params.m_fitType
         <<   "mean Type " << m_Params.m_meanMUType
         <<   "variance Mu Type: " << m_Params.m_varMUType
         <<   "mean Mu0:" << m_Params.m_meanMU0
         <<   "Mean Level: " << m_Params.m_meanConfidenceLevel
         <<   "Vriance Mu0: " << m_Params.m_varMU0
         <<   "variance Level: " << m_Params.m_varConfidenceLevel
         <<   "Fit Level: "  << m_Params.m_fitLevel
         <<   "Forms: "  << m_Params.m_Forms;
     msg.send();} */
  for (int i=0;i<m_Params.m_Forms.length();i++) {
     switch (m_Params.m_Forms[i]) {
        case 'm':
           sendMeanConfidence(oid(),ds,m_Params.m_meanConfidenceLevel);
           break;
        case 'M':
           sendMeanHyporesult(oid(),ds,m_Params.m_meanHypothesisLevel,m_Params.m_meanMU0,m_Params.m_meanMUType);
           break;
       case 'v':
           sendVarianceConfidenceResult(oid(),ds,m_Params.m_varConfidenceLevel);
           break;
       case 'V':
           sendVarianceHypoResult(oid(),ds,m_Params.m_varHypothesisLevel,m_Params.m_varMU0,m_Params.m_varMUType);
           break;

       case 'f':
           switch (m_Params.m_fitType) {
              case '1':
                  sendNormalResult(oid(),ds,m_Params.m_fitLevel);
                  break;
              case '2':
                  sendExpResult(oid(),ds,m_Params.m_fitLevel);
                  break;
              case '3':
                  sendUniformResult(oid(),ds,m_Params.m_fitLevel);
                  break;
           }
           break;
       case 'F':
           sendFullAnaysisResult(oid(),ds);
           break;
     }
  }
}

void DataSetFullAnalysisResultSend::sendFullAnaysisResult(OID oid,OneVariableDataSet& ds) const{
 {  LogMessage msg( oid, 0);
         msg <<  " Mean=" 
             <<    ds.mean() ;
         msg.send();}
  {  LogMessage msg( oid, 0);
          msg << " Variance="
              << ds.deviation() ;
         msg.send();}
  {  LogMessage msg( oid, 0);
          msg <<   " Standard Deviation="
              << ds.standardDeviation() ;
         msg.send();}
  {  LogMessage msg( oid, 0);
         msg <<   " Range="
             << ds.range() ; 
         msg.send();}
  {  LogMessage msg( oid, 0);
         msg <<  " Median="
             << ds.median() ; 
         msg.send();}
  {  LogMessage msg( oid, 0);
         msg <<  " First Quartile="
             << ds.firstQuartile() ; 
         msg.send();}
  {  LogMessage msg( oid, 0);
         msg <<   " Third Quartile="
             << ds.thirdQuartile() ; 
         msg.send();}
  {  LogMessage msg( oid, 0);
         msg <<  " Coefficient of Skewness="
             << ds.coefficientSkewness(); 
         msg.send();}
  {  LogMessage msg( oid, 0);
         msg <<  " Number of Measurements=" 
             << ds.cardinality(); 
         msg.send();}
}
