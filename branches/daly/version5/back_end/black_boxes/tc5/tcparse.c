#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 1 "tcparse.y"
 
#include <stdlib.h>
#include "ctype.h"
#include "tctime.h"
#include "tcstdlib.h"
#include "tcyacc.h" 
#include "parser.h"
#include "string.h" 
/* #include "tcstdlib.h"*/  
extern	char *progname;
Rel_stk_type *rel_pt, *rel_end; 
extern	Word_pt *w_pt, word_;
extern Enum_def  enumd, *pt;
int numb;
#line 27 "y.tab.c"
#define TOKEN_INT 257
#define TOKEN_GEN 258
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    0,    1,    1,    2,    2,    2,    3,    3,
    3,    3,    4,    4,    4,    4,    5,    5,    6,    6,
    7,    7,    7,    7,    8,    9,    9,    9,
};
short yylen[] = {                                         2,
    1,    2,    2,    1,    3,    1,    3,    2,    1,    3,
    2,    3,    1,    3,    5,    5,    1,    3,    1,    3,
    1,    1,    3,    3,    1,    3,    5,    5,
};
short yydefred[] = {                                      0,
   25,   13,    1,    0,    0,    0,    0,    0,    6,    0,
    0,    0,    0,   21,   22,    0,    0,    0,    0,    0,
    2,    0,    0,    0,    8,   11,    0,    0,    3,    0,
    0,    0,   14,    0,   26,    0,    0,    0,    7,   10,
   12,    0,    0,    0,    0,   24,   23,    0,    0,    0,
    0,   16,   28,   15,   27,
};
short yydgoto[] = {                                       6,
    7,   16,    9,   10,   11,   17,   13,   14,   15,
};
short yysindex[] = {                                    -37,
    0,    0,    0,  -29,  -29,    0,  -11,  -19,    0,  -68,
   -7,   -3,  -54,    0,    0,  -35,   -6,  -32,   44,  -23,
    0,  -23,  -23,  -23,    0,    0,  -20,  -16,    0,  -16,
  -15,  -23,    0,  -16,    0,  -23,  -16,  -19,    0,    0,
    0,  -16,  -16,   -3,  -54,    0,    0,  -27,   57,  -24,
  -13,    0,    0,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,   -2,    0,  -40,
    0,   -1,  -14,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   26,    0,    0,
    0,    0,    0,   38,  -10,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
    0,   41,   73,   17,    0,   50,   30,    0,   16,
};
#define YYTABLESIZE 242
short yytable[] = {                                       9,
    9,    9,    4,    9,   23,   33,   22,   23,   32,   22,
    4,   36,   23,   52,   22,   23,   23,   22,    9,   23,
   23,    3,   22,   42,   42,   27,   19,   19,   30,   19,
   20,   20,   20,   20,   35,   30,   28,   34,   30,   31,
    8,    4,   17,   41,   19,   18,   47,   21,   20,   12,
    9,   29,    9,    5,   19,   24,    4,   17,   24,   45,
   38,    5,    0,   24,   18,    0,   24,   24,   54,    5,
   24,   24,   48,    0,   43,   43,   50,   44,   19,   55,
   25,   18,   20,   49,    5,   30,   51,   37,   25,    0,
   25,    0,   19,    0,   39,    0,   18,   53,   30,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   25,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   25,    0,   25,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   26,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    9,    0,    1,
    2,    0,    2,    0,    0,    2,    0,    1,    2,    0,
    2,    0,    0,    2,    2,    0,   40,    2,    2,    0,
    1,   46,
};
short yycheck[] = {                                      40,
   41,   42,   40,   44,   40,   41,   42,   40,   44,   42,
   40,   44,   40,   41,   42,   40,   40,   42,   59,   40,
   40,   59,   42,   40,   40,   94,   41,   42,   42,   44,
   41,   42,   44,   44,   41,   42,   44,   44,   42,   94,
    0,   44,   44,   27,   59,    5,   31,   59,   59,    0,
   91,   59,   93,   91,    5,   91,   59,   59,   91,   30,
   20,   91,   -1,   91,   24,   -1,   91,   91,   93,   44,
   91,   91,   32,   -1,   91,   91,   36,   28,   93,   93,
    8,   44,   93,   34,   59,   42,   37,   44,   16,   -1,
   18,   -1,   43,   -1,   22,   -1,   59,   41,   42,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   38,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   48,   -1,   50,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  258,   -1,  257,
  258,   -1,  258,   -1,   -1,  258,   -1,  257,  258,   -1,
  258,   -1,   -1,  258,  258,   -1,  257,  258,  258,   -1,
  257,  257,
};
#define YYFINAL 6
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 258
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'",0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,"';'",0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'['",0,"']'","'^'",0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
"TOKEN_INT","TOKEN_GEN",
};
char *yyrule[] = {
"$accept : words",
"words : ';'",
"words : ordwords ';'",
"words : numwords ';'",
"ordwords : word",
"ordwords : ordwords ',' word",
"word : term",
"word : word '*' term",
"word : word term",
"term : bterm",
"term : bterm '^' TOKEN_INT",
"term : bterm TOKEN_INT",
"term : bterm '^' bterm",
"bterm : TOKEN_GEN",
"bterm : '(' word ')'",
"bterm : '[' word ',' word ']'",
"bterm : '(' word ',' word ')'",
"numwords : num_word",
"numwords : numwords ',' num_word",
"num_word : num_term",
"num_word : num_word '*' num_term",
"num_term : num_bterm",
"num_term : num_aterm",
"num_term : num_term '^' num_aterm",
"num_term : num_term '^' TOKEN_INT",
"num_bterm : TOKEN_INT",
"num_aterm : '(' num_word ')'",
"num_aterm : '[' num_word ',' num_word ']'",
"num_aterm : '(' num_word ',' num_word ')'",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 278 "tcparse.y"
 
#include "ctype.h"
#include "tcstdlib.h"
#include "stdio.h" 
/* #include "tcyacc.h"  
#include "tc.h"
#include "sys/types.h"
#include "miscellaneous.h"
*/
#include "tcerror.c" 
extern Gen_stk_type gen_stk[]; 
extern Word_pt *w_pt, word_;
extern Enum_def  enumd, *pt;
char 	*infile;	/* input file name */ 
int numb,lineno; 
execerror(s,t) /* print error message then exit */
char *s, *t;
{       warning(s,t);
        exit(1); /* cause normal program termination. */
}
warning(s, t)
/* print warning massege */
char *s, *t;
{       fprintf(stderr, "%s: %s", progname, s);
        if (t)
                fprintf(stderr, " %s", t);
        if (infile)
                fprintf(stderr, " in %s", infile);
        fprintf(stderr, " near line %d\n", lineno);
        exit(1);
}

char *eemalloc(n) /* check return from malloc */ 
unsigned n; 
{       char *p/*, *malloc()*/; 

        p = (char *)malloc(n); 
        if (p == 0){
                execerror("out of memory", (char *) 0); 
	}
        return p;
}  
init_gen_stk() /* initialize group generator stack */
{ 
	int i;

	for (i = 1; i <= Gen_no; i++)
                        Gen_st[i].gen_inv = -i;
}
Rel_stk_type *get_relator(s) /* put a relator to the relator stack */
char *s; 
{ 	Rel_stk_type *p,*st; 
	int length; 
	
	length = strlen(s); 
	p = (Rel_stk_type *) eemalloc(sizeof(Rel_stk_type)); 
/*
	p->rel = s;
*/
	p->rel = (char *) eemalloc(length + 1);
	p->len = length; 
	p->rel_1 = (int *) 0;
	p->exp = 1;
	strcpy(p->rel,s); 
	p->next = (Rel_stk_type *) 0;
 	return p; 
} 
int_power(p, cs, n) /* deal with integer power for a word */ 
char *p;
char *cs; 
int  n; 
{ 
register char s[1000];
register char tem, tbm; 
register	int i,pi,length,length_1; 
register int A2a;
	length = strlen(cs); 
	length_1 = length - 1; 
	for(i = 0; i < length; i++)
		s[i] = cs[i];
	s[i] = '\0';
	if (n < 0) { /* negative power */   
		pi = abs(n); /* get absolute n */ 
		A2a = 'a' - 'A';
		for (i = 0; i <= (length_1) / 2; i++) 
/* reverse s and convert every generator in s to its inverse */ 
		{	tem = s[i];
			tbm = s[length_1 - i];
			if (tem >= 'a' && tem <= 'z')
				s[length_1 - i] = tem - A2a;
			else
				s[length_1 - i] = tem + A2a;
			if (tbm >= 'a' && tbm <= 'z')
                                s[i] = tbm - A2a;
                        else
                                s[i] = tbm + A2a;
		} 
	} 
	else   pi = n;    
/* allocate space for making power */
	if (pi) { /* pi != 0 */ 
		strcpy(p, s); 
		for (i = 2; i <= pi; i++) /* making power */ 
		 	p = strcat(p, s); 
	} else *p = '\0'; /* when int_power == 0 */ 
} 
char *word_power(w,p)  /* work out (word)^(power)=(power)^-1(word)(power) */ 
char *w;  /* word */ 
char *p;  /* power */ 
{	char *st, *ct; 

	ct = (char *) eemalloc(strlen(p) * 2 + strlen(w) + 1); /* +1 for '\0' */
	int_power(ct, p, -1); /* make (power)^-1 */ 
	strcat(ct,w); /* make (power)^-1(word) */ 
	strcat(ct,p); /* make (power)^-1(word)(power) */ 
	return ct; 
} 
		
#line 315 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 25 "tcparse.y"
{ /* no words */
                NumOfWord = 0;
                Pt2Word = (Rel_stk_type *) 0;
                init_gen_stk(); /* initialize generator stack */
                return;
                          }
break;
case 2:
#line 31 "tcparse.y"
{
                NumOfWord = numb;
                Pt2Word = rel_pt;
                init_gen_stk(); /* initialize generator stack */
                return;
                }
break;
case 3:
#line 37 "tcparse.y"
{
                NumOfWord = numb;
                Pt2Word = rel_pt;
                init_gen_stk(); /* initialize generator stack */
                return;
                }
break;
case 4:
#line 44 "tcparse.y"
{  /* get word one by one.  */
		numb = 1;
		rel_pt = (Rel_stk_type *) 0; /* word stack pointer */  
		rel_pt = get_relator(yyvsp[0].words); /* put a word into word stack */  
		rel_end = rel_pt;
		free(yyvsp[0].words);
                }
break;
case 5:
#line 51 "tcparse.y"
{numb++; rel_end->next = get_relator(yyvsp[0].words);
		rel_end = rel_end->next; 
		free(yyvsp[0].words);
	  }
break;
case 6:
#line 56 "tcparse.y"
{ yyval.words = yyvsp[0].words;  }
break;
case 7:
#line 57 "tcparse.y"
{  yyval.words = (char *)eemalloc(strlen(yyvsp[-2].words) + strlen(yyvsp[0].words) + 1);
				strcpy(yyval.words, yyvsp[-2].words);
				strcat(yyval.words, yyvsp[0].words);
				free(yyvsp[-2].words); free(yyvsp[0].words);
		}
break;
case 8:
#line 62 "tcparse.y"
{  yyval.words = (char *)eemalloc(strlen(yyvsp[-1].words) + strlen(yyvsp[0].words) + 1);
                                strcpy(yyval.words, yyvsp[-1].words);
                                strcat(yyval.words, yyvsp[0].words);
                                free(yyvsp[-1].words); free(yyvsp[0].words);
		}
break;
case 9:
#line 68 "tcparse.y"
{
		yyval.words  = yyvsp[0].words;
	  }
break;
case 10:
#line 71 "tcparse.y"
{ /* integer power */
                int power;
                power = (yyvsp[0].vali < 0) ? -yyvsp[0].vali : yyvsp[0].vali;
                yyval.words = (char *) eemalloc(power * strlen(yyvsp[-2].words) + 1);
                                int_power(yyval.words,yyvsp[-2].words,yyvsp[0].vali);
                free(yyvsp[-2].words);
                              }
break;
case 11:
#line 78 "tcparse.y"
{ /* integer power */
                int power;
                power = (yyvsp[0].vali < 0) ? -yyvsp[0].vali : yyvsp[0].vali;
                yyval.words = (char *) eemalloc(power * strlen(yyvsp[-1].words) + 1);
                                int_power(yyval.words,yyvsp[-1].words,yyvsp[0].vali);
                 free(yyvsp[-1].words);
                              }
break;
case 12:
#line 85 "tcparse.y"
{ /* word power */
                            yyval.words = word_power(yyvsp[-2].words,yyvsp[0].words);
                          }
break;
case 13:
#line 89 "tcparse.y"
{ /* generator */
		yyval.words = (char *) eemalloc(2);
		*yyval.words = yyvsp[0].gen[0];
                *(yyval.words + 1) = '\0';
                }
break;
case 14:
#line 94 "tcparse.y"
{ yyval.words = yyvsp[-1].words; 
				/*strcpy($$,$2);*/
			 }
break;
case 15:
#line 97 "tcparse.y"
{ /* [w1,w2] */
                int  len1, len2, i;
                len1 = strlen(yyvsp[-3].words);
                len2 = strlen(yyvsp[-1].words);
		yyval.words = (char *) eemalloc(len1 + len1 +len2 + len2 + 1);
                int_power(yyval.words,yyvsp[-3].words,-1);
                /* put $4 into $$ */
                for (i = 0; i < len2; i++)
             /* reverse s and convert every generator in s to its inverse */
                        yyval.words[len1+i] = (islower(yyvsp[-1].words[len2-i-1]))? toupper(yyvsp[-1].words[len2-i-1]) : tolower(yyvsp[-1].words[len2-i-1]);
                yyval.words[len1+i] = '\0';
                strcat(yyval.words, yyvsp[-3].words);
                strcat(yyval.words, yyvsp[-1].words); 
		free(yyvsp[-3].words); free(yyvsp[-1].words);
                }
break;
case 16:
#line 112 "tcparse.y"
{ /* (w1,w2) same as [w1,w2] */
		int  len1, len2, i;
                len1 = strlen(yyvsp[-3].words);
                len2 = strlen(yyvsp[-1].words);
                yyval.words = (char *) eemalloc(len1 + len1 +len2 + len2 + 1);
                int_power(yyval.words,yyvsp[-3].words,-1);
                /* put $4 into $$ */
                for (i = 0; i < len2; i++)
             /* reverse s and convert every generator in s to its inverse */
                        yyval.words[len1+i] = (islower(yyvsp[-1].words[len2-i-1]))? toupper(yyvsp[-1].words[len2-i-1]) : tolower(yyvsp[-1].words[len2-i-1]);
                yyval.words[len1+i] = '\0';
                strcat(yyval.words, yyvsp[-3].words);
                strcat(yyval.words, yyvsp[-1].words);
                free(yyvsp[-3].words); free(yyvsp[-1].words);
                }
break;
case 17:
#line 128 "tcparse.y"
{
                int i;
                numb = 1; /* numb recoads the number of relators */
                rel_pt = (Rel_stk_type *) eemalloc(sizeof(Rel_stk_type));
                                        /* relator stack pointer */
                rel_pt->rel_1 = (int *) eemalloc(sizeof(int) * (yyvsp[0].nums->lengh + 1));
                for(i = 1; i <= yyvsp[0].nums->lengh; i++)
                        rel_pt->rel_1[i] = yyvsp[0].nums->numbs[i];
                rel_pt->len_1 = yyvsp[0].nums->lengh;
                rel_pt->next = (Rel_stk_type *) 0;
                rel_pt->rel = (char *) 0;
		rel_pt->exp = 1;
		rel_end = rel_pt;
	}
break;
case 18:
#line 142 "tcparse.y"
{
                Rel_stk_type *rel_ptt,*p;
                int i;
                numb++;
		rel_ptt = (Rel_stk_type *) eemalloc(sizeof(Rel_stk_type));
                rel_ptt->rel_1 = (int *)eemalloc(sizeof(int)*(yyvsp[0].nums->lengh+1));
                for(i = 1; i <= yyvsp[0].nums->lengh; i++)
                        rel_ptt->rel_1[i] = yyvsp[0].nums->numbs[i];
                rel_ptt->len_1 = yyvsp[0].nums->lengh;
                rel_ptt->rel = (char *) 0;
		rel_pt->exp = 1;
                rel_ptt->next = (Rel_stk_type *) 0;
		rel_end->next = rel_ptt; /* connect the new to the chain */
		rel_end = rel_ptt;
	 }
break;
case 19:
#line 158 "tcparse.y"
{
		yyval.nums  = yyvsp[0].nums;
	 }
break;
case 20:
#line 161 "tcparse.y"
{
                int i,j;
		yyval.nums = (Num_gen_word *) eemalloc(sizeof(Num_gen_word));
		yyval.nums->numbs = (int *) eemalloc(sizeof(int)*(yyvsp[-2].nums->lengh+yyvsp[0].nums->lengh+1));
		for(i = 1; i <= yyvsp[-2].nums->lengh; i++)
                        yyval.nums->numbs[i] = yyvsp[-2].nums->numbs[i];
                for(j = 1; j <= yyvsp[0].nums->lengh; j++,i++)
                        yyval.nums->numbs[i] = yyvsp[0].nums->numbs[j];
                yyval.nums->lengh = i - 1;
		free(yyvsp[-2].nums->numbs); free(yyvsp[0].nums->numbs);
		free(yyvsp[-2].nums); free(yyvsp[0].nums);
	 }
break;
case 21:
#line 174 "tcparse.y"
{
		yyval.nums  = yyvsp[0].nums;
         }
break;
case 22:
#line 177 "tcparse.y"
{
		yyval.nums  = yyvsp[0].nums;      
         }
break;
case 23:
#line 180 "tcparse.y"
{ /* (word)^(power)=(power)^-1(word)(power) */
                int i,j,leng;
                leng = yyvsp[0].nums->lengh;
		yyval.nums = (Num_gen_word *) eemalloc(sizeof(Num_gen_word));
                yyval.nums->numbs = (int *)eemalloc(sizeof(int)*(yyvsp[-2].nums->lengh+leng+leng+1));	
		for(i = 0; i < leng; i++)
                        yyval.nums->numbs[i + 1] = -yyvsp[0].nums->numbs[leng - i];
                i++;
                for(j = 1; j <= yyvsp[-2].nums->lengh; j++,i++)
                        yyval.nums->numbs[i] = yyvsp[-2].nums->numbs[j];
                for(j = 1; j <= yyvsp[0].nums->lengh; j++,i++)
                        yyval.nums->numbs[i] = yyvsp[0].nums->numbs[j];
                yyval.nums->lengh = i - 1;
		free(yyvsp[-2].nums->numbs); free(yyvsp[0].nums->numbs);
                free(yyvsp[-2].nums); free(yyvsp[0].nums);
	  }
break;
case 24:
#line 196 "tcparse.y"
{ /* (word)^n  */
                int i,j,t,power,leng,power1;
                power = yyvsp[0].vali;
		power1 = (power < 0) ? -power : power;
                leng = yyvsp[-2].nums->lengh;
		yyval.nums = (Num_gen_word *) eemalloc(sizeof(Num_gen_word));  
                yyval.nums->numbs = (int *)eemalloc(sizeof(int)*(leng*power1+1));
		if (power < 0) { /* (word)^-n  */
                        power = -power;
                        for(i = 0; i < leng / 2; i++) {
                                t = yyvsp[-2].nums->numbs[i + 1];
                                yyvsp[-2].nums->numbs[i + 1] = -yyvsp[-2].nums->numbs[leng - i];
                                yyvsp[-2].nums->numbs[leng - i] = -t;
                        }
                        if (leng % 2)
                           yyvsp[-2].nums->numbs[leng / 2 + 1] = -yyvsp[-2].nums->numbs[leng / 2 + 1];
                }
                i = 1;
                for ( ; power > 0; power--)
                        for(j = 1; j <= leng; j++,i++)
                                yyval.nums->numbs[i] = yyvsp[-2].nums->numbs[j];
                yyval.nums->lengh = i - 1;
		free(yyvsp[-2].nums->numbs); 
                free(yyvsp[-2].nums); 
          }
break;
case 25:
#line 222 "tcparse.y"
{
                if (yyvsp[0].vali < 0 && yyvsp[0].vali < -Gen_no || yyvsp[0].vali > 0 && yyvsp[0].vali > Gen_no) {
                        fprintf(fout,"%d is not a generator\n",yyvsp[0].vali);
                        exit(1);
                } else {
			yyval.nums = (Num_gen_word *) eemalloc(sizeof(Num_gen_word));  
                	yyval.nums->numbs = (int *)eemalloc(sizeof(int)*2);
			yyval.nums->numbs[1] = yyvsp[0].vali; yyval.nums->lengh =1;
		}
	  }
break;
case 26:
#line 233 "tcparse.y"
{
		yyval.nums = yyvsp[-1].nums;
	  }
break;
case 27:
#line 236 "tcparse.y"
{
                /* [word1,word2] = (word)^-1(word2)^-1(word1)(word2)  */
                int i,j,len1,len2;
		len1 = yyvsp[-3].nums->lengh;
		len2 = yyvsp[-1].nums->lengh;
		yyval.nums = (Num_gen_word *) eemalloc(sizeof(Num_gen_word));
		yyval.nums->numbs = (int *)eemalloc(sizeof(int)*(len1+len1+len2+len2+1));
		for(i = 1; i <= len1; i++)
                        yyval.nums->numbs[i] = -yyvsp[-3].nums->numbs[i];
                for(j = 1; j <= len2; j++,i++)
                        yyval.nums->numbs[i] = -yyvsp[-1].nums->numbs[j];
                for(j = 1; j <= len1;  j++,i++)
                        yyval.nums->numbs[i] = yyvsp[-3].nums->numbs[j];
                for(j = 1; j <= len2; j++,i++)
                        yyval.nums->numbs[i] = yyvsp[-1].nums->numbs[j];
                yyval.nums->lengh = i - 1;
		free(yyvsp[-3].nums->numbs); free(yyvsp[-1].nums->numbs);
		free(yyvsp[-3].nums);
		free(yyvsp[-1].nums);
         }
break;
case 28:
#line 256 "tcparse.y"
{
                /* [word1,word2] = (word)^-1(word2)^-1(word1)(word2)  */
                int i,j,len1,len2;
                len1 = yyvsp[-3].nums->lengh;
                len2 = yyvsp[-1].nums->lengh;
                yyval.nums = (Num_gen_word *) eemalloc(sizeof(Num_gen_word));
                yyval.nums->numbs = (int *)eemalloc(sizeof(int)*(len1+len1+len2+len2+1));
                for(i = 1; i <= len1; i++)     
                        yyval.nums->numbs[i] = -yyvsp[-3].nums->numbs[i];    
                for(j = 1; j <= len2; j++,i++) 
                        yyval.nums->numbs[i] = -yyvsp[-1].nums->numbs[j];    
                for(j = 1; j <= len1;  j++,i++)
                        yyval.nums->numbs[i] = yyvsp[-3].nums->numbs[j];     
                for(j = 1; j <= len2; j++,i++) 
                        yyval.nums->numbs[i] = yyvsp[-1].nums->numbs[j];     
                yyval.nums->lengh = i - 1;      
                free(yyvsp[-3].nums->numbs); free(yyvsp[-1].nums->numbs);
                free(yyvsp[-3].nums);
                free(yyvsp[-1].nums);
         }
break;
#line 782 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
