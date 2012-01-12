#ifndef AFFINEHEADER
#define AFFINEHEADER

#include <vector>
#include <map>
#include "FPGroup.h"
#include "OutMessages.h"
using namespace std;

class polynomial;
class poly_pair;

class rational {
	public:
	int numerator;
	int denominator;
	rational(int a,int b){
		numerator=a;
		denominator = b;
	}
	rational(){numerator=0;denominator=1;}
	int alcm(int a, int b){
        	if(a<0){a*=(-a);}
        	if(b<0){b*=(-b);}
        	int max=(a>b?a:b);
        	int i;
        	for(i=max;(i%a)||(i%b);i+=max){}
        	return i;
	}
	friend rational operator-(const rational &a,const rational &b){
		return a + rational(b.numerator*(-1),b.denominator);}
	friend rational operator+(const rational &a,const rational &b){
		rational c;
		c.denominator = c.alcm(a.denominator, b.denominator);
		c.numerator = (a.numerator*(c.denominator/a.denominator)
			+b.numerator*(c.denominator/b.denominator));
		return c;}
	friend rational operator/(const rational &a,const rational &b){
			return a*rational(b.denominator,b.numerator);}
	friend rational operator*(const rational &a,const rational &b)
			{return rational(a.numerator*b.numerator,a.denominator*b.denominator);}
	friend polynomial operator*(rational a,const polynomial &b);
	friend bool operator<(const rational &a,const rational &b){return a.numerator*b.denominator 
					< b.numerator*a.denominator;}
	friend bool operator==(const rational &a,const rational &b){
	if(a.numerator*b.denominator == b.numerator*a.denominator){return 1;}
	}
	friend bool operator!=(const rational &a,const rational &b){
	if(a.numerator*b.denominator != b.numerator*a.denominator){return 1;}
	}
};

class polynomial_term {
	int powersum;
	int degree;
	vector<int> powers;
	public:
	polynomial_term(const polynomial_term&);
	polynomial_term(int deg);
	polynomial_term(int deg, const vector<int> &pow);
	bool disjoint(const polynomial_term &);
	bool generated_by(const vector<bool> &a);
	void set(const vector<int> &store);
	void set_powersum();
	friend bool operator>>(const polynomial_term &a,const polynomial_term &b);
	friend bool operator!(const polynomial_term &a);
	friend bool operator==(const polynomial_term &,const polynomial_term &);
	friend bool operator!=(const polynomial_term &a,const polynomial_term &b){return !(a==b);}
	friend polynomial operator*(const rational &a,const polynomial_term &b);
	friend polynomial_term lcm(const polynomial_term &a,const polynomial_term &b)
	{
        	int i;
        	polynomial_term c(a.degree);
        	for(i=0;i<a.degree;i++){
        	        c.powers[i] = (a.powers[i]>b.powers[i]?a.powers[i]:b.powers[i]);
        	}
		c.set_powersum();
        	return c;
	}
	friend polynomial_term operator*(const polynomial_term &a,const polynomial_term &b);
	friend bool operator>(const polynomial_term &a,const polynomial_term &b);
	friend bool operator<(const polynomial_term &a,const polynomial_term &b){
				return b>a;}
	friend polynomial operator/(const polynomial_term &a, const polynomial_term &b);
	friend ostream& operator <<(ostream &ostr,const polynomial_term &a);
	friend ostream& operator <(ostream& ostr, const polynomial_term &a)
	{
		int i;
		ostr < a.powers.size();
		for(i=0;i<a.powers.size();i++){
			ostr < a.powers[i];
		}
		return ostr;
	}
	friend istream& operator >(istream& istr, polynomial_term &a)
	{
		int size, i;
		int entry;
		a.powers.clear();
		istr > size;
		for(i=0;i<size;i++){
			istr > entry;
			a.powers.push_back(entry);
		}
		return istr;
	}
};

class polynomial {
	private:
	map<polynomial_term, rational> entries;
	int degree;
	public:
	int get_degree() const{return degree;}
	friend class poly_pair;
	polynomial(int);
	polynomial(int,rational,const vector<int> &pow);
	polynomial(const polynomial &a);
	friend void reduce(polynomial &,const vector<polynomial> &grob);
	friend void reduce(polynomial &,const vector<polynomial> &,const vector<polynomial> &);
	friend bool single_reduce(polynomial &,const vector<polynomial> &grob);
	void set(rational,const vector<int> &pow);
	polynomial_term head_term() const;
	rational head_coeff() const;
	void reset_head_coeff();
	friend bool operator!(const polynomial &a);
	friend polynomial operator+(const polynomial &a,const polynomial &b);
	friend polynomial operator*(const polynomial &a,const polynomial &b);
	friend polynomial operator*(rational a, const polynomial &b);
	friend polynomial operator-(const polynomial &a,int b);
	friend polynomial operator-(const polynomial &a,const polynomial &b);
	friend polynomial operator/(const polynomial &a,const polynomial &b);
	friend ostream& operator<(ostream& ostr, const polynomial &a)
	{
		ostr < a.entries.size();
		map<polynomial_term,rational>::const_iterator p = a.entries.begin();
		while(p!=a.entries.end()){
			ostr < p->second.numerator;
			ostr < p->second.denominator;
			ostr < p->first;
			p++;
		}
		return ostr;
	}
	friend istream& operator>(istream& istr, polynomial &a)
	{
		int size, i;
		rational b;
		polynomial_term c(a.degree);
		istr > size;
		a.entries.clear();
		for(i=0;i<size;i++){
			istr > b.numerator;
			istr > b.denominator;
			istr > c;
			a.entries[c] = b;
		}
		return istr;
	}
	friend bool operator<(const polynomial &a,const polynomial &b);
	friend ostream& operator << (ostream& ostr, const polynomial& a);
	void erase_zeros();
};

struct poly_pair {
	polynomial_term *plcm;
	poly_pair(const poly_pair&);
	int first;
	~poly_pair();
	int second;
	vector<polynomial> &grobner;
	void set_lcm();
	poly_pair(vector<polynomial> &a);
	poly_pair(int,int, vector<polynomial> &);
	void set(int,int);
	polynomial spol() const;
	friend bool operator < (const poly_pair &a,const poly_pair &b);
};


class poly_matrix {
	private:
	int dimension;
	int degree;
	vector<polynomial> entries;
	public:
	poly_matrix(int dim,int deg);
	poly_matrix(int dim,int deg,const vector<polynomial> &init);
	void set_entry(int,int,const polynomial &a);
	void set_entry(int,int,int);
	polynomial get_entry(int,int) const;
	void identity();
	poly_matrix invert();
	polynomial determinant() const;
	friend poly_matrix operator-(const poly_matrix &a,const poly_matrix &b);
	friend poly_matrix operator+(const poly_matrix &a,const poly_matrix &b);
	friend poly_matrix operator*(const poly_matrix &a,const poly_matrix &b);
	friend poly_matrix operator^(const poly_matrix &a,int);
	friend ostream& operator<<(ostream &ostr, const poly_matrix &a);
};

class Affine {
	FPGroup theGroup;
	vector<polynomial> grobner_basis;
	vector<polynomial> defining_eqns;
	vector<polynomial_term> grob_heads;
	int degree;
	int generators;
	SetOf<Word> relators;
	int num_relators;
	int poly_deg;
	int dim;
	void dimrec(int,int,vector<bool>);
	void create_defining_eqns();
	void create_grobner_basis();
	void reduce_grobner_basis();
public:
	int get_poly_deg() const {return poly_deg;}
	FPGroup group();
	int get_degree() const {return degree;}
	Affine(const FPGroup &G,int deg);
	vector<polynomial> get_grobner_basis();
	int get_dimension();

};

#endif
