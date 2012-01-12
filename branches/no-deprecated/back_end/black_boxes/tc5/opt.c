/* file opt.c  */
#include "tcyacc.h"
#include "tc.h"
#include "tcyacc.h"
#include "miscellaneous.h"
#include "ct.h"
#include <string.h>
#define	MST(i,j)	MST_T[i+i+j-3]
tc_mst(tc_pvar)
struct  tc_str_vars     *tc_pvar;
{
/* subroutine to build a minimal spanning tree that is stored in MST[]
*  and set MSTFLG TRUE.
*  the coset table has been renumbered (contains cosets number instead of 
*  their address).
*  cosets in MST are represented by their number instead of their 
*  address in CT.
*/
register Int	i;
register Int	j;
register Int	k;
register Int	col;
register Int	nal;
register Int	thisl;
register Int	nextl;
register Int	kn;
Int	*ct;

	ct = COSET_TABLE;
/* free the space used by previous MST, and allocate space for this
*  MST.
*/
        free(MST_T);
        MST_T = (int *) eemalloc(sizeof(int) * (2 * NALIVE + 2));
/* initialize MST.  */
	for (i = 1; i <= NALIVE; i++) {
		MST(i, 1) = 0;
		MST(i, 2) = 0;
	}
/* construct the minimal spanning tree. root MST(1,1)=-1  */
	MST(1, 1) = -1;
/* nal is the number of the cosets in the tree .
*  thisl is the branch currently being processed.
*  nextl is the number of the previous coset in the tree.
*  kn is the number of the current coset being scanned(in coset table).
*/
	MST_LEVEL = 1;
	for (nal = 1,thisl = 1, nextl = 0; nal <= NALIVE;) {
		kn = thisl;
/* colunm 2 links rows in the order of formation.  */
		thisl = MST(kn,2);
		for (col = 1; col <= NCOL; col++) {
			j = CT(Num2Addr(kn), col);
			if (j < 0)
				break;
/* allow for the case of an incomplete coset table.  */
			if ( j == 0)
				continue;
/* if MST(j,1) is not 0 then coset j is already in the tree. */
			if (MST(j, 1))
				continue;
/* add a new coset in the tree.  */
			MST(j, 1) = kn;
			MST(j, 2) = nextl;
			nextl = j;
			nal++;
		}
/* are all cosets in the tree?  */
		if (nal >= NALIVE)
			break;
/* no, move on to the next know coset in the tree.  */
		if (thisl)
			continue;
		if (nextl <= 0)
			break;
/* move onto the next (level) branch in the tree.  */
		thisl = nextl;
		nextl = 0;	
		MST_LEVEL++;
	}
/* we have finished constructing the tree, fill in the column numbers
*  in column 2. (note that we could have done this already but because
*  of space limitation we haven't.)
*/
	for (i = 2; i <= NALIVE; i++) {
		j = MST(i, 1);
		if (j == 0)
			fprintf(fout,"THERE IS A ZERRO ENTRE IN MST(%d, 1)\n",i);
		else
			for (k = 1; k <= NCOL; k++)
				if (CT(Num2Addr(j), k) == i) {
					MST(i, 2) = k;
					break;
				}
	}
	MSTFLG = TRUE;
}

Int tc_cosrep(coseth, order, tc_pvar)
Int	coseth;
Int	*order;
struct  tc_str_vars     *tc_pvar;
{
register Int    i;
register Int    kn;
register Int    e;
register Int    t;
register Int    rep;
register Int    j;
/* subroutine to find the coset representative of coseth, and store it in 
*  COSREP[] in term of the subscript of group generators in Gen_st[] and in 
*  COSREP_A[] in terms of letters of the group generators and return the
* length of the coset representative.
*  space of COSREP[] has to be allocated before calling this subroutine.
*/
	i = 0;
	if (MSTFLG == FALSE) {
		fprintf(fout,"THERE IS NO MINIMAL SPANNING TREE.\n");
		return i;
	}
/* check if coseth is a valid coset in the coset table. */
	if (coseth > NALIVE || coseth == 1) {
	   fprintf(fout,"THERE IS NO COSET REPRESETATIVE FOR COSET %d\n",coseth);
		return i;
	}
/* produce coset representative for coseth.  */
	rep = coseth;
	for (i = 1; rep > 1; rep = MST(rep, 1),i++) {
		COSREP[i] = MST(rep, 2);	
	}
	i--;
/* now i is the length of the coset rep.
*  reverse COSREP[].  
*/
	for (j = 1, e = i; j <= i / 2; j++,e--) {
		t = COSREP[j];
		COSREP[j] = COSREP[e];
		COSREP[e] = t;
	}
/* find the order of the coset representative. */
	kn = coseth;
	*order = 0;	
	for (j = 1;; j++) {
/* trace out the coset represetative in the coset table once.  */
		kn = tc_tracew(kn, &COSREP[1], &COSREP[i], tc_pvar);
		if (kn > 1) 
			continue;
		if (kn < 1) 
/* we reached an undefined entry in the coset table, let order 0. */
			break;
		else if (kn == 1) {
/* we find the order of the coset, j. */
                        *order = j;
                        break;
                } 
	}
/* convert the representative into the representation of generators.*/
	for (j = 1; j <= i; j++)
		COSREP[j] = COL_TO_GEN[COSREP[j]];
/*   for letter generators, convert cosrep into letters. */
	if (Gen_st[1].gen != '0') 
		for (j = 1; j <= i; j++)
			if (COSREP[j] < 0)
				COSREP_A[j] = toupper(Gen_st[-COSREP[j]].gen);
			else
				COSREP_A[j] = Gen_st[COSREP[j]].gen;
	return i;
}
Int
tc_normal(n, tc_pvar)
Int 	n;
struct  tc_str_vars     *tc_pvar;
{
register Int    subg;
register Int    *beg;
register Int    *end;
/* a coset I is normalizing if in the subgroup H of group G generated by words
*  W1,...,Wp,
*		I Wj = I   j=1,...,p
* if I is normalizing return 1 otherwise 0.
*/
	if (NSGPG == 0) 
		return 0;
	for (subg = 1; subg <= NSGPG; subg++) {
                beg = &SUBGROUP_GEN[SUBG_INDEX[subg]];
                end = beg - 1 + SUBG_LENGTH[subg];
		if (n == tc_tracew(n, beg, end, tc_pvar))	
			continue;
		else
			return 0;
	}
	return 1;
}
Int
tc_tracew(n, beg, end, tc_pvar)
Int 	n;
Int	*beg;
Int	*end;
struct  tc_str_vars     *tc_pvar;
{
register Int ifront;
register Int	*forscan;
register Int	i;
Int	*ct;
/* trace the word starting at *beg ending at *end by coset n, and return
* as answer the coset one ends up at.
*/
	ct = COSET_TABLE;
	ifront = n;
	for(forscan = beg; forscan <= end; forscan++) {
                i = CT(Num2Addr(ifront), *forscan);

                if (i)
                        ifront = i;
                else
			break;
	}
	return i;
}			
tc_rc(coset,stop,desire,tc_pvar)
Int	coset;
Int	stop;
Int 	desire;
struct  tc_str_vars     *tc_pvar;
{
char	*tc_save();
/* this subroutine finds non trival subgroups in the coset table with index a
*  multiple of a desired index 'desire' by repeatedly putting cosets
*  coincident with coset 1 and observing what happens. The first
*  coset to be put coincident with coset 1 is 'coset' and the
*  previous coset to this coset is used if a favourable result
*  doesn't occur and this process is repeated until a favourable
*  result occurs or else we reach coset 1 or coset 'stop'.
*  Note that at the end of this option the original coset table
*  will be current and if we wish to work in the coset table resulting
*  from the final repeated coincidence with a suitable finite index then
*  a CC option will have to be done using the last coset put coincidednt
*  with coset 1.
*  desire==0 means any index. 
*  coset== 0 means NALIVE.
*  stop== 0  means coset 2.
*/

/* if STDCT is TRUE then the coset table was renumbered. we have to
*  unrenumber it.
*/
        if (STDCT)
                tc_unrenumber(tc_pvar);
	MSTFLG = 0;
        tc_compact(tc_pvar);
/* save the current coset table.  */
	strcpy(FILE_NAME, tc_save(tc_pvar));
/* test stop, coset, desire, if any of them are invalid make it default.  */
	if (stop <= 1)
		stop = 2;
	if (coset <= 1 || coset > NALIVE)
		coset = NALIVE;
	if (desire < 0)
		desire = 0;
        MSTFLG = FALSE;
/* initialize varibles for coinc processing.  */
        MSGLVE = 0;
        DED_PT = DED_END;
	for (;;) {
/* do coset coset coincident with coset 1.   */
	tc_coinc(1,Num2Addr(coset),tc_pvar);
	if (INDEX1) {
                tc_text(12, tc_pvar);
                INDEX = TRUE;
/* we got the index 1, restore the coset table, print out the coset.*/
                tc_restore(tc_pvar);
                unlink(FILE_NAME);
/* get a standard coset table.  */
                tc_ctrenumber(tc_pvar);
                tc_mst(tc_pvar); /* construct ms tree. */
                tc_print_ct(-coset,coset,1,tc_pvar);
                return;
        }
	if (INDEX == 0) 
/* resume the enumeration.   */
		tc_todd_coxeter(1, tc_pvar);
/* check if we get the right index?  */
	if (INDEX) {
		if (desire != 0) 
			if ((NALIVE % desire) != 0) {
/* try next coset. */
                                tc_restore(tc_pvar);
                                coset--;
				if (coset == stop) {
                			fprintf(fout,"REACH STOP %d\n", stop);
					unlink(FILE_NAME);
                			return;
        			}
				continue;
			}
/* we got the desire index, restore the coset table, print out the coset.*/
			tc_restore(tc_pvar);
			unlink(FILE_NAME);
/* get a standard coset table.  */
			tc_ctrenumber(tc_pvar);
			tc_mst(tc_pvar); /* construct ms tree. */
			tc_print_ct(-coset,coset,1,tc_pvar);
			return;
	}
	if (coset == stop) {
		fprintf(fout,"REACH STOP %d\n", stop);
		return;
	}
	}
}
tc_normcl(parmtr, tc_pvar)
Int 	parmtr;
struct  tc_str_vars     *tc_pvar;
{
register Int 	conji;
register Int	i;
register Int 	j;
register Int	k;
register Int	thisl;
register Int	coset;
register Int	*beg;
register Int	*begs;
register Int	*end;
register Int 	*ends;
register Int	*ct;
register Int	t;
register Int	ii;
register Int	length;
register Int	subg;
register Int	newsg_len;
register Int    room_in_y;
register Int	n;
register Int	kill;
register Int 	killi;

Rel_stk_type *p, *pp;
/* subroutine to find the normal closure.
*  trace out ((gen i)^-1)*(subgroup generator)*(gen i) and
*  trace out ((gen i)*(subgroup generator)*(gen i)^-1 for all group
*  defining generators i and all subgroup generators, noting whether
*  we get back to coset 1 or not.
*  fixed a bug for working space: when all space is given to the
*  coset table, and it is all used, then when new sgs which are
*  produced by this subroutine arrive, there's no room for them,
*  in this case n cosets, at the end of the coset table, are deleted
*  to make room for new sgs. 3/2/1993
*  free reduce new sgp gens created by NC.   4/2/1993
*/
	ct = COSET_TABLE;
	Newsg = 0;
	for (conji = 1; conji >= -1; conji -= 2) {
	for (i = 1; i <= NDGEN; i++) {
		if (conji > 0)
			k = GEN_TO_COL[i];
		else
			k = GEN_TO_COL[-i];
		thisl = CT(1, k);
		for (subg = 1; subg <= NSGPG; subg++) {
                beg = &SUBGROUP_GEN[SUBG_INDEX[subg]];
                end = beg - 1 + SUBG_LENGTH[subg];
		coset = thisl;
		coset = tc_tracew(coset, beg, end, tc_pvar);
		if (coset == thisl) 
			continue;
		else {
/* output this conjugate. */
			if(Gen_st[1].gen == '0') 
				fprintf(fout," Group generator %d^%d does not normalize subgroup generator No. %d\n", i, conji, subg);
			else 
				fprintf(fout," Group generator %c^%d does not normalize subgroup generator No. %d\n", Gen_st[i].gen, conji, subg);
/* use COSREP[] as a buffer for the conjugate. allocate space for COSREP[]. */
			if (parmtr) {
/* need 2 positions for i^1 and i^-1. */
			length = 2 + SUBG_LENGTH[subg];
			free(COSREP);
			free(COSREP_A);
			COSREP = (int *) eemalloc(sizeof(int) * (1 + length));
			COSREP_A = (char *) eemalloc(1 + length);
/* assemble this word as an additional subgroup generator. */
			if (Gen_st[1].gen == '0') {
/* for number generators.  */
				if (conji > 0) {
					COSREP[length] = -i;
					COSREP[1] = i;
				} else {
					COSREP[length] = i; 
					COSREP[1] = -i;
				}
				for (begs = &SUBGROUP_GEN[SUBG_INDEX[subg]], ends = beg - 3 + length, j = 2; begs <= ends; begs++,j++) {
                                	t = *begs;
                                	t = COL_TO_GEN[t];
                                	COSREP[j] = t;
				}
			} else {
/* for letter generators.  */
			if (conji > 0) {
				COSREP[1] = i;
				COSREP[length] = -i;
				COSREP_A[1] = Gen_st[i].gen;
				COSREP_A[length] = toupper(Gen_st[i].gen);
			} else {
				COSREP[1] = -i;
				COSREP[length] = i;
				COSREP_A[length] = Gen_st[i].gen;
				COSREP_A[1] = toupper(Gen_st[i].gen);
			}
			for (begs = &SUBGROUP_GEN[SUBG_INDEX[subg]], ends = beg - 3 + length, j = 2; begs <= ends; begs++,j++) {
				t = *begs;
				t = COL_TO_GEN[t];
				COSREP[j] = t;
				if (t > 0)
					COSREP_A[j] = Gen_st[t].gen;
				else
					COSREP_A[j] = toupper(Gen_st[-t].gen);
			}	
			}
/* allocate space for the new subgroup generator and connect it to
*  the sugroup generator chain.
*/
			p = (Rel_stk_type *) eemalloc(sizeof(Rel_stk_type));
/* put COSREP into subg generator.  */
        		if (Gen_st[1].gen == '0') {
                		p->rel_1 = (int *) eemalloc(sizeof(int) * (length + 1));
                		p->len_1 = length;
                		for (ii = 1; ii <= length; ii++)
                        		p->rel_1[ii] = COSREP[ii];
                		p->rel = (char *) 0;
        		} else {
                		p->rel = (char *) eemalloc(length + 1); /* +1 for '\0' */
                		p->len = length;
                		for (ii = 1; ii <= length; ii++)
                        		p->rel[ii - 1] = COSREP_A[ii];
                		p->rel[ii-1] = '\0';
                		p->rel_1 = (int *) 0;
        		}
			p->exp = 1;
        		p->next = (Rel_stk_type *) 0;
        		Sgen_no++;
        		Newsg++;
/* connect the new to the chain of subg generators.  */
        		INDEX = FALSE;
			if (S_gens) {
				for (pp = S_gens; pp->next; pp = pp->next);
				pp->next = p;
			} else 
				S_gens = p;
			}
		}
		}
	}
	} 			
	if (Newsg != 0) {
/* check if there's enough room for new subgroup gens */
/* what length is the new sgs? newsg_len. */
	newsg_len = 0;
	for (pp = S_gens, i = 1; i <= Sgen_no - Newsg; i++, pp = pp->next);
	if (Gen_st[1].gen == '0')
		for (;pp != (Rel_stk_type *) 0; pp = pp->next)
			newsg_len += pp->len_1;
	else
		for (;pp != (Rel_stk_type *) 0; pp = pp->next)
                        newsg_len += pp->len;
	newsg_len += 2 * Newsg;
/* room in array y? */
	room_in_y = DED_PT - &CT(NEXTDF, 1);
	if (room_in_y < newsg_len) {
/* have to make room for new sg by discarding n cosets. */
		n = (newsg_len - room_in_y) / NCOL + 1;
/* fix NALIVE and MAXROW by n.  */
		NALIVE -= n;
		MAXROW -= n;
		NEXTDF = Num2Addr(NALIVE + 1);
		DED_PT = &CT(NEXTDF, 1);
		DED_END = DED_PT;
/* fix CT table entres which are related to discarded cosets by 0. */
		for (kill = NALIVE + n; kill > NALIVE; kill--) 
			for (i = 1; i <= NCOL; i++) {
				killi = CT(Num2Addr(kill), i);
				if (killi == 0)
					continue;
				j = INV_TO_COL[i];
				CT(Num2Addr(killi), j) = 0;
			}
	fprintf (fout, " %d cosets are discarded \n Maxmum table size %d\n", n,MAXROW);
	}
	}
}
