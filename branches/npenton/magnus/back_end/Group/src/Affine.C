#include "Affine.h"
#include "Generator.h"
#include "Set.h"
#include "OutMessages.h"
#include <set>

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
	reduce_grobner_basis();
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
	LogMessage msg(1);
	msg << "Checkpoint 1";
	msg.send();
	msg << "Checkpoint 2";
	msg.send();
	// Populate the generator matrices
	int choice_it = 0;
	for(i=0;i<generators;i++){
		for(j=1;j<=degree;j++){
			for(k=1;k<=degree;k++){
				cur_vector = vector<int>(poly_deg,0);
				cur_vector[choice_it]=1;
				cur_matrix.set_entry(j,k,
					polynomial(poly_deg,rational(1,1),cur_vector));
				choice_it++;
			}
		}
		cur_poly = cur_matrix.determinant();
		grobner_basis.push_back(cur_poly - 1);
		gen_matrices.push_back(cur_matrix);
	}
	msg << "Checkpoint 3";
	msg.send();
	//Create some standard constants
	one_poly = polynomial(poly_deg,rational(1,1),vector<int>(poly_deg,0));
	zero_poly = polynomial(poly_deg,rational(0,1),vector<int>(poly_deg,0));
	for(i=1;i<=degree;i++){
		for(j=1;j<=degree;j++){
			msg << "i = " << i << " j = " << j;
			msg.send();
			ident.set_entry(i,j,(i==j?one_poly:zero_poly));
		}
	}
	msg << "Checkpoint 4";
	msg.send();
	//Create the relational equations
	SetIteratorData<Word> relate_iter(relators);
	relate_iter.reset();
	int mat_count = 1;
	msg << "num relators = " << num_relators;
	msg.send();
	for(i=0;i<num_relators;i++){
		current_word = relate_iter.value();
		relate_iter.next();
		cur_matrix = ident;
		msg << "current word length = " << current_word.length();
		msg.send();
		for(j=0;j<current_word.length();j++){
			msg << "current letter = " << current_word[j];
			msg.send();	
			if(!matrix_chart[abs(current_word[j])]){
				msg << "mat_count = " << mat_count;
				msg.send();
				matrix_chart[abs(current_word[j])] = mat_count;
				mat_count++;
			}
			cur_mat = matrix_chart[abs(current_word[j])]-1;
			if(power(current_word[j])<0){
				cur_matrix = cur_matrix *
					gen_matrices[cur_mat].invert();
			}else{
				msg << "cur_matrix = "<< cur_matrix;
				msg.send();
				msg << "index = "<< matrix_chart[cur_mat];
				msg.send();
				msg << "mult matrix = " << gen_matrices[cur_mat];
				msg.send();
				cur_matrix = cur_matrix * gen_matrices[cur_mat];
			}
		}
		msg << "cur_matrix = ";
		msg << cur_matrix;
		msg.send();
		cur_matrix = cur_matrix - ident;
		for(j=1;j<=degree;j++){
			for(k=1;k<=degree;k++){
				grobner_basis.push_back(cur_matrix.get_entry(j,k));
			}
		}
	}	
	for(i=0;i<grobner_basis.size();i++){
	msg << "??";
	msg.send();
		msg << "grobner_basis("<<i<<")=";
		msg << grobner_basis[i];
		msg.send();
	}
}

void Affine::create_grobner_basis(){
	set<poly_pair> pairs;
	set<poly_pair>::const_iterator p;
	map<pair<int,int>,bool> checked;
	LogMessage msg(1);
	poly_pair B(grobner_basis);
	polynomial h(poly_deg);
	int i,j;
	bool skip;
	msg << "Checkpoint 5";
	msg.send();
	//Grobnerize!
	for(i=1;i<grobner_basis.size();i++){
		for(j=0;j<i;j++){
			if(grobner_basis[i].head_term().disjoint(grobner_basis[j].head_term())){
				checked[pair<int,int>(i,j)] = 1;
                                checked[pair<int,int>(j,i)] = 1;
			}else{
				pairs.insert(poly_pair(i,j,grobner_basis));
			}
		}
	}
	msg << "Checkpoint 6";
	msg.send();
	while(pairs.size()){
		B.set(pairs.begin()->first,pairs.begin()->second);
		pairs.erase(pairs.begin());
		msg << "removing " << B.first << " and " << B.second;
		msg.send();
		skip =0;
		checked[pair<int,int>(B.first,B.second)] = 1;
		checked[pair<int,int>(B.second,B.first)] = 1;
		for(i=0;i<grobner_basis.size();i++){
			if(checked[pair<int,int>(i,B.first)] && checked[pair<int,int>(i,B.second)]
				&& (!!(lcm(grobner_basis[B.first].head_term(),
					grobner_basis[B.second].head_term())
					>> grobner_basis[i].head_term()))) {
				msg << "skipped by the second criterion";
				msg.send();
				skip = 1;
			}
		}
		if(!skip){
			h = B.spol();
			msg << "reducing spol";
			msg.send();
			reduce(h,grobner_basis);
			msg << "reduce returned";
			msg.send();
			if(!(!h)){
				grobner_basis.push_back(h);
				for(i=0;i<grobner_basis.size()-1;i++){
					if(grobner_basis[i].head_term().disjoint(h.head_term())){
						checked[pair<int,int>(i,grobner_basis.size()-1)];
                                                checked[pair<int,int>(grobner_basis.size()-1,i)];
						msg << "skipped by the first criterion";
						msg.send();
					}else{
						msg <<"adding "<<i<<" and " << grobner_basis.size()-1;
						msg.send();
						pairs.insert(poly_pair
							(i,grobner_basis.size()-1,grobner_basis));
					}
				}
			}
		}
	}
	msg << "Checkpoint 7";
	msg.send();
	msg << "grobner basis is:";
	msg.send();
	for (i=0;i<grobner_basis.size();i++){
		msg << grobner_basis[i];
		msg.send();
	}
}

void Affine::reduce_grobner_basis() {
	bool bad;
	LogMessage msg(1);
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
	msg << "Checkpoint 8";
	msg.send();
	//finish reducing the basis
	while(reduced_grobner_basis.size()){
		cur_poly = reduced_grobner_basis.back();
		reduced_grobner_basis.pop_back();
		reduce(cur_poly,grobner_basis,reduced_grobner_basis);
		if(!(!(cur_poly))){
			grobner_basis.push_back(cur_poly);
		}
	}
	for(i=0;i<grobner_basis.size();i++){
		grobner_basis[i].reset_head_coeff();
		grob_heads.push_back(grobner_basis[i].head_term());
		msg << grobner_basis[i];
		msg.send();
	}
	msg << "Checkpoint 9, poly deg = " << poly_deg << ",grobnersize is " << grobner_basis.size();
	msg.send();
}



int Affine::get_dimension(){
	vector<bool> U(poly_deg,0);
	dimrec(0,0,U);
	LogMessage msg(1);
	msg << "checkpt 9.5";
	msg.send();
	return dim;
}

void Affine::dimrec(int k, int d, vector<bool> U) {
	int i,j;
	bool bad;
	LogMessage msg(1);
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
