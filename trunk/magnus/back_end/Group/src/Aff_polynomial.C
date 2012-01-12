
#include "Affine.h"
//#include "OutMessages.h"

void polynomial::set(int coeff, const vector<int> &pow){
	entries.clear();
	polynomial_term a = polynomial_term(degree, pow);
	a.set_powersum();
	if(coeff!=0){
		entries.push_back(monomial(coeff,a));
	}
}

monomial::monomial(int c,polynomial_term t) : term(t){
	coefficient = c;
}

monomial::monomial(int a) : term(a){
}

polynomial::polynomial(int deg){
	degree = deg;
}

void polynomial::reset_head_coeff(){
	list<monomial>::iterator p;
	p = entries.begin();
	if(p!=entries.end()){
		p->coefficient = 1;
	}
}

polynomial::polynomial(int deg, int coeff, const vector<int> &pow){
	degree = deg;
	polynomial_term a = polynomial_term(deg, pow);
	a.set_powersum();
	if(coeff!=0){
		entries.push_back(monomial(coeff,a));
	}
}

polynomial operator+(const polynomial &a, const polynomial &b){
	list<monomial>::const_iterator p = a.entries.begin();
	list<monomial>::const_iterator q = b.entries.begin();
	int sum;
	polynomial c = polynomial(a.degree);
	while(p!=a.entries.end() && q!=b.entries.end()){
		if(p->term < q->term){
			c.entries.push_back(monomial(p->coefficient,p->term));
			p++;
		}else if (q->term < p->term){
			c.entries.push_back(monomial(q->coefficient,q->term));
			q++;
		}else{
			sum = q->coefficient+p->coefficient;
			if(sum!=0){
				c.entries.push_back(monomial(sum,p->term));
			}
			p++;
			q++;
		}	
	}
	while(p!=a.entries.end()){
		c.entries.push_back(monomial(p->coefficient,p->term));
		p++;
	}
	while(q!=b.entries.end()){
		c.entries.push_back(monomial(q->coefficient,q->term));
		q++;
	}
	return c;
}

polynomial& operator-=(polynomial &a,const polynomial &b){
	list<monomial>::iterator p= a.entries.begin();
	list<monomial>::const_iterator q = b.entries.begin();
	int diff;
	while(p!=a.entries.end() && q!=b.entries.end()){
		if(p->term < q->term){
			p++;
		}else if (q->term < p->term){
			a.entries.insert(p,monomial(-1*q->coefficient,q->term));
			q++;
		}else {
			diff = p->coefficient - q->coefficient;
			if(diff==0){
				p = a.entries.erase(p);
			}else {
				p->coefficient = diff;
			}
			q++;
		}
	}
	while(q!=b.entries.end()){
		a.entries.push_back(monomial(-1*q->coefficient,q->term));
		q++;
	}
	return a;
}

polynomial operator-(const polynomial &a,const polynomial &b){
	return a + ((-1)*b);
}

//The following routine should only be called while constructing the matrices.
//It could probably be optimized.
polynomial operator*(const polynomial &a,const polynomial &b){
	polynomial c = polynomial(a.degree);
	list<monomial>::const_iterator p =a.entries.begin();
	list<monomial>::const_iterator q;
	while(p!=a.entries.end()){
		q=b.entries.begin();
		while(q!=b.entries.end()){
			c += monomial(p->coefficient*q->coefficient,p->term*q->term);
			q++;
		}
		p++;
	}
	return c;
}

polynomial& operator+=(polynomial &a, const monomial &b){
	list<monomial>::iterator p =a.entries.begin();
	while(p->term<b.term && p!=a.entries.end()){
		p++;
	}
	if(p == a.entries.end()){
		a.entries.push_back(b);
	}else if(p->term == b.term){
		p->coefficient = p->coefficient + b.coefficient;
	}else{
		a.entries.insert(p,b);
	}
	return a;
}

bool operator!(const polynomial &a){
	if(a.entries.begin() == a.entries.end()){
		return 1;
	}
	return 0;
}

int polynomial::head_coeff() const{
	return entries.front().coefficient;
}

int agcd(int a, int b) {
  if (b == 0)
    return a;

  return agcd(b, a % b);
}

int alcm(int a,int b){
        return (a*b)/agcd(a,b);
}


void polynomial::reduce_coeff(){
	list<monomial>::iterator p = entries.begin();
	int tgcd = p->coefficient;
	p++;
	while(p!=entries.end()){
		if(tgcd==1){return;}
		tgcd = agcd(tgcd,p->coefficient);
		p++;
	}
	for(p=entries.begin();p!=entries.end();p++){
		p->coefficient /= tgcd;
	}
}

void reduce(polynomial &h, const vector<polynomial> &grob){
	while(single_reduce(h, grob));
}

bool full_reduce(polynomial &h,const vector<polynomial> &grob1,const vector<polynomial> &grob2){
	int changed = 0;
	while(single_full_reduce(h, grob1) || single_full_reduce(h,grob2)) {changed = 1;}
	return changed;
}

bool single_full_reduce(polynomial &h, const vector<polynomial> &grob){
	int i,j;
	list<monomial>::iterator p;
	polynomial_term s(h.degree);
	if(!h){
		return 0;
	}
	for(i=0;i<grob.size();i++){
		for(p=h.entries.begin();p!=h.entries.end();p++){
			if(p->term >> grob[i].head_term() ){
				s = (p->term)/grob[i].head_term();
				if(grob[i].head_coeff() != 1){
					h *= alcm(grob[i].head_coeff(),p->coefficient)/p->coefficient;
				}
				h -= monomial(p->coefficient/grob[i].head_coeff(),s)*grob[i];
				return 1;
			}
		}
	}
	return 0;
}		

bool single_reduce(polynomial &h, const vector<polynomial> &grob){
	int i;
	polynomial_term s(h.degree);
	if(!h){
		return 0;
	}
	for(i=0;i<grob.size();i++){
		if(h.head_term() >> grob[i].head_term() ){
			s = h.head_term()/grob[i].head_term();
			if(grob[i].head_coeff() != 1){
				h *= alcm(grob[i].head_coeff(),h.head_coeff())/h.head_coeff();
			}
			h -= monomial(h.head_coeff()/grob[i].head_coeff(),s)*grob[i];
			return 1;
		}
	}
	return 0;
}

polynomial& operator*=(polynomial &a,int b){
	list<monomial>::iterator p;
	for(p=a.entries.begin();p!=a.entries.end();p++){
		p->coefficient *=b;
	}
	return a;
}

monomial operator*(const monomial &a,const monomial &b){
	return monomial(a.coefficient*b.coefficient,a.term*b.term);
}

polynomial operator*(const monomial &a, const polynomial &b){
	list<monomial>::const_iterator p =b.entries.begin();
	polynomial c(b.degree);
	while(p!=b.entries.end()){
		c.entries.push_back(a*monomial(p->coefficient,p->term));
		p++;
	}
	return c;
}

polynomial_term polynomial::head_term() const{
	return entries.front().term;
}

polynomial operator*(int a, const polynomial &b){
	if(a == 0){return 0;}
	polynomial c(b.degree);
	list<monomial>::const_iterator p= b.entries.begin();
	while(p!=b.entries.end()){
		c.entries.push_back(monomial(a * p->coefficient,p->term));
		p++;
	}
	return c;
}

polynomial::polynomial(const polynomial &a){
	degree = a.degree;
	entries = a.entries;
}

ostream& operator << (ostream& ostr, const polynomial &a){
	int i;
	list<monomial>::const_iterator p = a.entries.begin();
	while(p != a.entries.end()){
		ostr << "\t" << p->coefficient;
		ostr << p->term;
		p++;
	}
	return ostr;
}


bool operator<(const polynomial &a, const polynomial &b){
	list<monomial>::const_iterator p =a.entries.begin();
	list<monomial>::const_iterator q =b.entries.begin();
	while(p!=a.entries.end()){
		if(p->term<q->term || (p->term==q->term && p->coefficient < q->coefficient)){
			return 1;
		}
		if(q->term<p->term || (p->term==q->term && q->coefficient < p->coefficient) ||
			q==b.entries.end()){
			return 0;
		}
	}
	if(q!=b.entries.end()){
		return 0;
	}
	return 1;
}

polynomial operator-(const polynomial &a,int b){
	polynomial c = a;
	polynomial_term d(a.degree);
	c += monomial(-b,d);
	return c;
}
