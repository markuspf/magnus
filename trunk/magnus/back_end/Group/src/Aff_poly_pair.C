#include "Affine.h"

poly_pair::poly_pair(vector<polynomial> &a) : grobner(a){
	plcm = new polynomial_term(grobner[0].degree);
}

poly_pair::poly_pair(int a, int b, vector<polynomial>& c) : grobner(c){
	first = a;
	second = b;
	plcm = new polynomial_term(grobner[0].degree);
	set_lcm();
}

void poly_pair::set(int a,int b ){
	first = a;
	second = b;
	set_lcm();
}

void poly_pair::set_lcm(){
	*plcm = lcm(grobner[first].head_term(),grobner[second].head_term());
}

poly_pair::poly_pair(const poly_pair &a): grobner(a.grobner){
	first = a.first;
	second = a.second;
	plcm = new polynomial_term(grobner[0].degree);
	set_lcm();
}

poly_pair::~poly_pair(){
	delete plcm;
}

bool operator < (const poly_pair &a,const poly_pair &b){
	if(*a.plcm > *b.plcm){
		return 1;
	}
	if(*a.plcm < *b.plcm){
		return 0;
	}
	if(a.first > b.first){
		return 1;
	}
	if(a.first == b.first && a.second>b.second){
		return 1;
	}
	return 0;
}

polynomial poly_pair::spol() const{
	polynomial c = monomial(grobner[second].head_coeff(),
		(*plcm/(grobner[first].head_term())))*
				grobner[first];
	polynomial d = monomial(grobner[first].head_coeff(),
		(*plcm/(grobner[second].head_term())))*
				grobner[second];
//	cout << "c = " << c << endl;
//	cout << "d = " << d << endl;
	c -= d;
	return c;
}
