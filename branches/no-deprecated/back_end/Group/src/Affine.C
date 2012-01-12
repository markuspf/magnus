#include "Affine.h"
#include "Generator.h"
#include "Set.h"
//#include "OutMessages.h"
#include <set>

ostream& operator <(ostream& ostr, const polynomial_term &a)
{
   int i;
   ostr < a.powers.size();
   for(i=0;i<a.powers.size();i++){
      ostr < a.powers[i];
   }
   return ostr;
}

istream& operator >(istream& istr, polynomial_term &a)
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

ostream& operator<(ostream& ostr, const polynomial &a)
{
   ostr < a.entries.size();
   list<monomial>::const_iterator p = a.entries.begin();
   while(p!=a.entries.end()){
      ostr < p->coefficient;
      ostr < p->term;
      p++;
   }
   return ostr;
}

istream& operator>(istream& istr, polynomial &a)
{
   int size, i;
   int b;
   polynomial_term c(a.degree);
   istr > size;
   a.entries.clear();
   for(i=0;i<size;i++){
      istr > b;
      istr > c;
      a.entries.push_back(monomial(b,c));
   }
   return istr;
}

Affine::Affine(const FPGroup &G, int deg){
	degree = deg;
	theGroup = G;
	generators = theGroup.numberOfGenerators();
	relators = theGroup.getRelators();
	num_relators = relators.cardinality();
	dim=0;
	poly_deg = degree*degree*generators;
}

vector<polynomial> Affine::get_grobner_basis() {
	create_defining_eqns();
	create_grobner_basis();
	reduce_grobner_basis();
	return grobner_basis;
}

void Affine::create_defining_eqns() {
	int i,j,k,cur_mat;
	map<int,int> matrix_chart;
	poly_matrix cur_matrix(degree,poly_deg);
	polynomial cur_poly(poly_deg);
	polynomial zero_poly(poly_deg);
	polynomial one_poly(poly_deg);
	poly_matrix ident(degree,poly_deg);
	vector<int> cur_vector;
	Word current_word;
	vector<poly_matrix> gen_matrices;
	// Construct polynomials for the terms	
	// Populate the generator matrices
	int choice_it = 0;
	for(i=0;i<generators;i++){
		for(j=1;j<=degree;j++){
			for(k=1;k<=degree;k++){
				cur_vector = vector<int>(poly_deg,0);
				cur_vector[choice_it]=1;
				cur_matrix.set_entry(j,k,
					polynomial(poly_deg,1,cur_vector));
				choice_it++;
			}
		}
		cur_poly = cur_matrix.determinant();
		grobner_basis.push_back(cur_poly - 1);
		gen_matrices.push_back(cur_matrix);
	}
	//Create some standard constants
	one_poly = polynomial(poly_deg,1,vector<int>(poly_deg,0));
	zero_poly = polynomial(poly_deg,0,vector<int>(poly_deg,0));
	for(i=1;i<=degree;i++){
		for(j=1;j<=degree;j++){
			ident.set_entry(i,j,(i==j?one_poly:zero_poly));
		}
	}
	//Create the relational equations
	SetIteratorData<Word> relate_iter(relators);
	relate_iter.reset();
	int mat_count = 1;
	for(i=0;i<num_relators;i++){
		current_word = relate_iter.value();
		relate_iter.next();
		cur_matrix = ident;
		for(j=0;j<current_word.length();j++){
			if(!matrix_chart[abs(current_word[j])]){
				matrix_chart[abs(current_word[j])] = mat_count;
				mat_count++;
			}
			cur_mat = matrix_chart[abs(current_word[j])]-1;
			if(power(current_word[j])<0){
				cur_matrix = cur_matrix *
					gen_matrices[cur_mat].invert();
			}else{
				cur_matrix = cur_matrix * gen_matrices[cur_mat];
			}
		}
		cur_matrix = cur_matrix - ident;
		for(j=1;j<=degree;j++){
			for(k=1;k<=degree;k++){
				if(!!cur_matrix.get_entry(j,k)){
					grobner_basis.push_back(cur_matrix.get_entry(j,k));
				}
			}
		}
	}
}

void Affine::create_grobner_basis(){
	set<poly_pair> pairs;
	set<poly_pair>::const_iterator p;
	int i,j;
	long k = 0;
	map<pair<int,int>,bool> checked;
	poly_pair B(grobner_basis);
	polynomial h(poly_deg);
	bool skip;
	//Grobnerize!
	for(i=0;i!=grobner_basis.size();i++){
		for(j=0;j!=i;j++){
			if(grobner_basis[i].head_term().disjoint(grobner_basis[j].head_term())){
				checked[pair<int,int>(i,j)] = 1;
                                checked[pair<int,int>(j,i)] = 1;
			}else{
				pairs.insert(poly_pair(i,j,grobner_basis));
			}
		}
	}
	//PLEASE NOTE: 99.999% (estimated) of the computation time is spent in the following
	//while loop.  All optimization efforts should be concentrated here until its runtime is
	//reasonable. 
	while(!pairs.empty()){
		k++;
		p=pairs.begin();
		B.set(p->first,p->second);
		pairs.erase(p);
		skip =0;
		checked[pair<int,int>(B.first,B.second)] = 1;
		checked[pair<int,int>(B.second,B.first)] = 1;
		for(i=0;i<grobner_basis.size();i++){
			if(checked[pair<int,int>(i,B.first)] && checked[pair<int,int>(i,B.second)]
				&& (!!(*B.plcm) >> grobner_basis[i].head_term())) {
				skip = 1;
			}
		}
		if(!skip){
			h = B.spol();
			reduce(h,grobner_basis);
			if(!(!h)){
				h.reduce_coeff();
				grobner_basis.push_back(h);
				for(i=0;i<grobner_basis.size()-1;i++){
					if(grobner_basis[i].head_term().disjoint(h.head_term())){
						checked[pair<int,int>(i,grobner_basis.size()-1)];
                                                checked[pair<int,int>(grobner_basis.size()-1,i)];
					}else{
						pairs.insert(poly_pair
							(i,grobner_basis.size()-1,grobner_basis));
					}
				}
			}
		}
	}
}

void Affine::reduce_grobner_basis() {
	bool bad;
	bool changed = 1;
	polynomial cur_poly(poly_deg);
	vector<polynomial> reduced_grobner_basis;
	int i;
	while(grobner_basis.size()){
		bad=0;
		cur_poly = grobner_basis.back();
		grobner_basis.pop_back();
		for(i=0;i<grobner_basis.size();i++){
			if(!!(cur_poly.head_term()/grobner_basis[i].head_term())){
				bad=1;
				break;
			}
		}
		if(bad==1){continue;}
		for(i=0;i<reduced_grobner_basis.size();i++){
			if(!!(cur_poly.head_term()/reduced_grobner_basis[i].head_term())){
				bad=1;
				break;
			}
		}
		if(bad==1){continue;}
		reduced_grobner_basis.push_back(cur_poly);
	}
	//finish reducing the basis
	while(changed){
		changed = 0;
		grobner_basis.clear();
		while(reduced_grobner_basis.size()){
			cur_poly = reduced_grobner_basis.back();
			reduced_grobner_basis.pop_back();
			if(full_reduce(cur_poly,grobner_basis,reduced_grobner_basis)){
				changed = 1;
			}
			if(!(!(cur_poly))){
				grobner_basis.push_back(cur_poly);
			}
		}
		reduced_grobner_basis = grobner_basis;
	}
	for(i=0;i<grobner_basis.size();i++){
		grobner_basis[i].reset_head_coeff();
		grob_heads.push_back(grobner_basis[i].head_term());
	}
}



int Affine::get_dimension(){
	vector<bool> U(poly_deg,0);
	dimrec(0,0,U);
	return dim;
}

void Affine::dimrec(int k, int d, vector<bool> U) {
	int i,j;
	bool bad;
	for(i=k;i<poly_deg;i++){
		U[i] = 1;
		bad = 0;
		for(j=0;j<grob_heads.size() && bad == 0;j++){
			if(grob_heads[j].generated_by(U)){
				bad=1;
			}
		}
		if(bad==0){
			dimrec(i+1, d+1, U);
			if(d+1>dim){
				dim=d+1;
			}
		}
		U[i]=0;
		if(d+poly_deg-i<dim){
			return;
		}
	}
	return;
}
