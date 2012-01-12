
#include "Affine.h"
#include "OutMessages.h"

void polynomial::set(rational coeff, const vector<int> &pow){
	entries.clear();
	polynomial_term a = polynomial_term(degree, pow);
	a.set_powersum();
	entries[a] = coeff;
}

polynomial::polynomial(int deg){
	degree = deg;
}

void polynomial::reset_head_coeff(){
	map<polynomial_term,rational>::iterator p;
	p = entries.begin();
	p->second = rational(1,1);
}

polynomial::polynomial(int deg, rational coeff, const vector<int> &pow){
	degree = deg;
	polynomial_term a = polynomial_term(deg, pow);
	a.set_powersum();
	entries[a] = coeff;
	this->erase_zeros();
}

polynomial operator+(const polynomial &a, const polynomial &b){
	map<polynomial_term,rational>::const_iterator p;
	polynomial c = polynomial(a.degree);
	c=a;
	p = b.entries.begin();
	while(p!=b.entries.end()){
		c.entries[p->first] = c.entries[p->first]+ p->second;
		p++;
	}
	c.erase_zeros();
	return c;
}

polynomial operator-(const polynomial &a,const polynomial &b){
	rational c(-1,1);
	return a + (c*b);
}

polynomial operator*(const polynomial &a,const polynomial &b){
	polynomial c = polynomial(a.degree);
	map<polynomial_term,rational>::const_iterator q;
	map<polynomial_term,rational>::const_iterator p =a.entries.begin();
	while(p!=a.entries.end()){
		q = b.entries.begin();
		while(q!=b.entries.end()){
			polynomial_term d = p->first*q->first;
			rational e = p->second*q->second;
			c.entries.insert(make_pair(d,rational(0,1)));
			c.entries[d] = c.entries[d] + e;
			q++;
		}
		p++;
	}
	c.erase_zeros();
	return c;
}

polynomial_term polynomial::head_term() const{
	if(!*this){
		return 0;
	}
	map<polynomial_term,rational>::const_iterator p = entries.begin();
	return p->first;
}

bool operator!(const polynomial &a){
	map<polynomial_term,rational>::const_iterator p = a.entries.begin();
	while(p!=a.entries.end()){
		if (p->second.numerator){
			return 0;
		}
		p++;
	}
	return 1;
}

rational polynomial::head_coeff() const{
	map<polynomial_term,rational>::const_iterator p = entries.begin();
	return p->second;
}


void reduce(polynomial &h, const vector<polynomial> &grob){
	while(single_reduce(h, grob));
}

void reduce(polynomial &h,const vector<polynomial> &grob1,const vector<polynomial> &grob2){
	while(single_reduce(h, grob1) || single_reduce(h,grob2));
}

bool single_reduce(polynomial &h, const vector<polynomial> &grob){
	int i;
	LogMessage msg(1);
	polynomial s(h.degree);
	if(!h){
		return 0;
	}
	msg << "h = ";
	msg << h;
	msg.send();
	for(i=0;i<grob.size();i++){
		s = h.head_term()/grob[i].head_term();
		msg << "s = " << s;
		msg.send();
		if(!!s){
			msg << "grob i has " << grob[i].entries.size() << "terms";
			msg.send();
			h = h - (h.head_coeff()/grob[i].head_coeff())*s*grob[i];
//			if(!(a.head_term() < h.head_term())){
//				msg << "reduction error, old h =" <<
//				a << " new h = " << h << "c = "
//				<< c << " s = " << s;
//				msg.send();
//			}
			msg <<" h now has " << h.entries.size() << "terms";
			msg.send();
			return 1;
		}
//		msg << "next loop";
//		msg.send();
	}
	return 0;
}

polynomial operator*(rational a, const polynomial &b){
	polynomial c(b.degree);
	map<polynomial_term,rational>::const_iterator p =b.entries.begin();
	while(p!=b.entries.end()){
		c.entries[p->first] = a * p->second;
		p++;
	}
	c.erase_zeros();
	return c;
}

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

polynomial::polynomial(const polynomial &a){
	degree = a.degree;
	entries = a.entries;
}

ostream& operator << (ostream& ostr, const polynomial &a){
	int i;
	map<polynomial_term,rational>::const_iterator p = a.entries.begin();
	while(p != a.entries.end()){
		ostr << "\t" << p->second.numerator << "/" << p->second.denominator;
		ostr << p->first;
		p++;
	}
	return ostr;
}

bool operator ==(const polynomial_term &a,const polynomial_term &b){
	return (a.powers == b.powers);
}

bool operator < (const poly_pair &a,const poly_pair &b){
	if(*a.plcm > *b.plcm){
		return 1;
	}
	if(*a.plcm < *b.plcm){
		return 0;
	}
	if(a.first < b.first){
		return 1;
	}
	if(a.first == b.first && a.second<b.second){
		return 1;
	}
	return 0;
}

polynomial poly_pair::spol() const{
	polynomial c = polynomial(grobner[second].head_coeff()
		*(*plcm/(grobner[first].head_term()))*
				grobner[first]);
	polynomial d = polynomial(grobner[first].head_coeff()
		*(*plcm/(grobner[second].head_term()))*
				grobner[second]);
	return c - d;
}

bool operator<(const polynomial &a, const polynomial &b){
	return a.entries < b.entries;
}

void polynomial::erase_zeros(){
	polynomial_term pterm(degree);
	map<polynomial_term,rational>::iterator pos;
	for(pos=entries.begin();pos!=entries.end();){
		if(pos->second.numerator == 0){
			entries.erase(pos++);
		}else{
			pos++;
		}
	}
}

polynomial operator-(const polynomial &a,int b){
	polynomial c = a;
	polynomial_term d(a.degree);
	rational e = rational((-b),1);
	c.entries[d] = c.entries[d] + e;
	c.erase_zeros();
	return c;
}

polynomial_term operator*(const polynomial_term &a,const polynomial_term &b){
	vector<int> c(a.degree);
	int i;
	for(i=0;i<a.degree;i++){
		c[i] = a.powers[i]+b.powers[i];
	}
	return polynomial_term(a.degree, c);
}

bool operator!(const polynomial_term &a){
	int i;
	for(i=0;i<a.powers.size();i++){
		if(a.powers[i]){
			return 0;
		}
	}
	return 1;
}

bool polynomial_term::generated_by(const vector<bool> &a){
	int i;
	for(i=0;i<degree;i++){
		if(!(a[i]) && powers[i]){
			return 0;
		}
	}
	return 1;
}

bool polynomial_term::disjoint(const polynomial_term &a){
	int i;
	for(i=0;i<degree;i++){
		if(a.powers[i] && powers[i]){
			return 0;
		}
	}
	return 1;
}

polynomial operator/(const polynomial_term &a,const polynomial_term &b){
	int i;
	vector<int> s(a.degree);
	for(i=0;i<b.degree;i++){
		if(a.powers[i]<b.powers[i]){
			return 0;
		}
		s[i] = a.powers[i]-b.powers[i];
	}
	return polynomial(a.degree,rational(1,1),s);
}

bool operator>>(const polynomial_term &a,const polynomial_term &b){
	for(int i=0;i<b.degree;i++){
		if(a.powers[i]<b.powers[i]){
			return 0;
		}
	}
	return 1;
}

polynomial operator*(const rational &a,const polynomial_term &b){
	return polynomial (b.degree, a, b.powers);
}

bool operator>(const polynomial_term &a,const polynomial_term &b){
	if (a.powersum<b.powersum){
		return 1;
	}
	if(b.powersum<a.powersum){
		return 0;
	}
	return(a.powers<b.powers);
}

polynomial_term::polynomial_term(int deg){
	degree = deg;
	powers.resize(degree);
	powersum = 0;
}

ostream& operator << (ostream &ostr, const polynomial_term &a){
	ostr << "(";
	int i;
	for(i=0;i<a.powers.size();i++){
		ostr << a.powers[i] << " ";
	}
	ostr << ")";
	return ostr;
}

polynomial_term::polynomial_term(int deg, const vector<int> &store){
	degree = deg;
	powers = store;
	set_powersum();
}

void polynomial_term::set_powersum(){
	powersum=0;
	for(int i=0;i<degree;i++){
		powersum+=powers[i];
	}
}		

polynomial_term::polynomial_term(const polynomial_term &a){
	degree = a.degree;
	powers = a.powers;
	powersum = a.powersum;
}

void polynomial_term::set(const vector<int> &store){
	powers = store;
	set_powersum();
}

