
/*
 *   $Id$
 */
 
// Copyright (C) 2001 The New York Group Theory Cooperative
// See magnus/doc/COPYRIGHT for the full notice.
// 
// Contents: Implementation of Stats functions class
//
// Principal Author: Xiaowei Xu
//
// Status: in progress
//
// Revision History:
//
//

#include "statsFunctions.h"
#include "AdaptSimpson.h"
#include <math.h>

double fmax(double x, double y){
  if(x>y)
    return x;
  else
    return y;
}

double fmin(double x, double y){
  if(x<y)
    return x;
  else
    return y;
}

double NormFunction(double t){
	double p=(-t)*t/2;
	double result=pow(ECONST,p);
	return result/sqrt(2*PICONST);
}

//double AdaptiveSimpson (double a, double b, 
//			double (*funct)(double x), 
//			double relerr)
double statsFunctions::Norm(double x){
	double r= AdaptiveSimpson(0,x,NormFunction,0.0001);
	if(x>=0)
		return r+0.5;
	else
		return 0.5-r;
}

double statsFunctions::NormInv(double x){
   double start=-4.0;
   double end=4.0;
   while(end-start>0.0001){
      double mid=(start+end)/2;
      double midValue=Norm(mid);
      if(midValue<=x)
         start=mid;
      else
         end=mid;
   }
   return (start+end)/2;
}
///Gamma(x)=(x-1)*Gamma(x-1)
///Gamma(1)=1
///Gamma(1.5)=sqrt(Pi)*0.5
///we only consider Gamma(n/2) or Gamma((n+1)/2)
double GammaFun(double x){
	if (x<1.2) return 1;///the only possibility is x=1.0
         ///the only possibility is x=1.5
	if (x>=1.2 && x<1.7) return sqrt(PICONST)*0.5;
	return (x-1)*GammaFun(x-1);
}

////tden=Gamma((n+1)/2)/sqrt(nPi)gamma(n/2)
////     *(1+t*t/n) ^ (-(n+1)/2)
double TFunction(double t,int n){
	double GammaChild=GammaFun((n+1)/2.0);
	double GammaParent=GammaFun(n/2.0);
	double part1=GammaChild/GammaParent/sqrt(n*PICONST);
    double part2=pow(1+t*t/n,-(n+1)/2.0);
	return part1*part2;
}

double statsFunctions::TDis(double x, int n){
    double r= AdaptiveSimpsonInt(0,fabs(x),n,TFunction,0.001);
	if(x>=0)
		return r+0.5;
	else
		return 0.5-r;
}

double statsFunctions::TInv(double x, int n){
   double start=-5.0;
   double end=5.0;
   while(end-start>0.0001){
      double mid=(start+end)/2;
      double midValue=TDis(mid,n);
      if(midValue<=x)
         start=mid;
      else
         end=mid;
   }
   return (start+end)/2;
}

double ChiFunction(double t,int n){
	double Child=pow(t,n/2.0-1)*pow(ECONST,-t/2.0);
	double Parent=GammaFun(n/2.0)*pow(2,n/2.0);
	return Child/Parent;
}

double statsFunctions::Chi(double x,int n){
    double r= AdaptiveSimpsonInt(0,x,n,ChiFunction,0.001);
    return r;   
}
double statsFunctions::ChiInv(double x,int n){
   double start=0;
   double end=50;
   while(end-start>0.0001){
      double mid=(start+end)/2;
      double midValue=1-Chi(mid,n);
      if(midValue>=x)
         start=mid;
      else
         end=mid;
   }
   return (start+end)/2;
}

double statsFunctions::Expo(double upper,double randa){
   return 1-pow(ECONST,-randa*upper);
}
double statsFunctions::Unif(double upper,double a,double b){
   return (upper-a)/(b-a);
}

double FFunction(double x, int n1, int n2){
  	double Child=GammaFun((n1+n2)/2.0)*pow(((double)n1)/n2,n1/2.0)*pow(x,n1/2.0-1);
	double Parent=GammaFun(n1/2.0)*GammaFun(n2/2.0)*pow(1.0+n1*x/n2,(n1+n2)/2.0);
	return Child/Parent;
}

double statsFunctions::FDis(double x, int n1, int n2){
    double r= AdaptiveSimpsonIntInt(0,x,n1,n2,FFunction,0.001);
    return r;
}

///FInv function is a increasing function

double statsFunctions::FInv(double x, int n1, int n2){
   double start=0;
   double end=20;
   while(end-start>0.0001){
      double mid=(start+end)/2;
      double midValue=FDis(mid,n1,n2);
      if(midValue<=x)
         start=mid;
      else
         end=mid;
   }
   return (start+end)/2;
}
