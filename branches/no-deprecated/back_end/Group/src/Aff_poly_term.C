#include "Affine.h"

bool operator ==(const polynomial_term &a,const polynomial_term &b){
	return (a.powers == b.powers);
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

polynomial_term operator/(const polynomial_term &a,const polynomial_term &b){
	int i;
	vector<int> s(a.degree);
	for(i=0;i<b.degree;i++){
		if(a.powers[i]<b.powers[i]){
			return 0;
		}
		s[i] = a.powers[i]-b.powers[i];
	}
	return polynomial_term(a.degree,s);
}

bool operator>>(const polynomial_term &a,const polynomial_term &b){
	for(int i=0;i<b.degree;i++){
		if(a.powers[i]<b.powers[i]){
			return 0;
		}
	}
	return 1;
}

polynomial operator*(const int &a,const polynomial_term &b){
	return polynomial(b.degree, a, b.powers);
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

