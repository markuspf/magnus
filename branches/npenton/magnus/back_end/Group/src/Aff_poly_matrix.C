
#include "Affine.h"


poly_matrix::poly_matrix(int dim,int deg){
	dimension = dim;
	degree = deg;
	entries.resize(dim*dim,0);
}

poly_matrix::poly_matrix(int dim, int deg, const vector<polynomial> &init){
	dimension = dim;
	degree = deg;
}

void poly_matrix::set_entry(int a,int b,const polynomial &c){
	entries[(a-1)*dimension+b-1] = c;
}

void poly_matrix::set_entry(int a,int b, int c){
	vector<int> d = vector<int>(degree);
	rational f(1,1);
	entries[(a-1)*dimension+b-1] =  polynomial(degree, f, d);
}

polynomial poly_matrix::get_entry(int a,int b) const{

	return entries[(a-1)*dimension+b-1];
}

//Of course the following two routines can be partly consolidated. There are
// also faster implementations, but since each routine only runs once for
// each generator, this seems relatively low priority (compared to optimizing
// the polynomial routines)

poly_matrix poly_matrix::invert(){
	poly_matrix inv(degree, dimension);
	poly_matrix sub(degree, dimension-1);
	int i;
	int j;
	int k;
	int l;	
	if(dimension==1){
		return *this;
	}
	poly_matrix m(dimension-1,degree);
	for(l=1;l<=dimension;l++){
		for(i=1;i<=dimension;i++){
			for(j=1;j<=dimension-1;j++){
				for(k=1;k<=dimension-1;k++){
					m.set_entry(j,k,entries[(j<=l)?j:(j-1)*dimension+
						(i<=k? k : (k-1))]);
				}
			}
			inv.set_entry(i,l,rational(((i+l)%2)?1:(-1),1)*m.determinant());
		}
	}
	return inv;
}

polynomial poly_matrix::determinant() const{
	polynomial d(degree);
	polynomial mfactor(degree);
	int i;
	int j;
	int k;
	if(dimension==1){
		return entries[0];
	}
	poly_matrix m(dimension-1,degree);
	for(i=1;i<=dimension;i++){
		for(j=1;j<=dimension-1;j++){
			for(k=1;k<=dimension-1;k++){
				m.set_entry(j,k,entries[j*dimension+
					(i<=k? k : (k-1))]);
			}
		}
		mfactor = rational((i%2?1:(-1)),1)*(entries[i-1]*m.determinant());
		d = d + mfactor;
	}
	return d;
}

poly_matrix operator+(const poly_matrix &a,const poly_matrix &b){
	poly_matrix sum(a.dimension, a.degree);
	int i;
	int j;
	for (i=1;i<=a.dimension;i++){
		for(j=1;j<=a.dimension;j++){
			sum.set_entry(i,j,a.get_entry(i,j)+b.get_entry(i,j));
		}
	}
	return sum;
}

poly_matrix operator-(const poly_matrix &a,const poly_matrix &b){
	poly_matrix diff(a.dimension, a.degree);
	int i;
	int j;
	for (i=1;i<=a.dimension;i++){
		for(j=1;j<=a.dimension;j++){
			diff.set_entry(i,j,a.get_entry(i,j)-b.get_entry(i,j));
		}
	}
	return diff;
}

poly_matrix operator*(const poly_matrix &a,const poly_matrix &b){
	poly_matrix prod(a.dimension,a.degree);
	int i;
	int j;
	int k;
	polynomial sum(a.degree);
	for(i=1;i<=a.dimension;i++){
		for(j=1;j<=a.dimension;j++){
			sum=polynomial(a.degree);
			for(k=1;k<=a.dimension;k++){
				sum=(a.get_entry(i,k)*b.get_entry(k,j) + sum);
			}
			prod.set_entry(i,j,sum);
		}
	}
	return prod;
}

ostream& operator << ( ostream& ostr, const poly_matrix &a) {
	for(int i=1;i<=a.dimension;i++){
		for (int j=1;j<=a.dimension;j++){
			ostr << "("<<i<<","<<j<<")="<<a.get_entry(i,j);
		}
	}
	return ostr;
}

poly_matrix operator^(const poly_matrix &a,int b){
	poly_matrix exp(a.dimension,a.degree);
	exp.identity();
	int i;
	if(b>=0){
		for(i=0;i<b;i++){
			exp = exp*a;
		}
	} else {	//shouldn't get here yet
		b *= (-1);
//		poly_matrix c = a.inverse();
		for(i=0;i<b;i++){
//			exp = exp*c;
		}
	}
	return exp;
}

void poly_matrix::identity(){
	entries.clear();
	int i;
	for(i=1;i<=dimension;i++){
		set_entry(i,i,1);
	}
}

