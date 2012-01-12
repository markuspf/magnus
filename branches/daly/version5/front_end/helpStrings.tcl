## Copyright (C) 1995 The New York Group Theory Cooperative
## See magnus/doc/COPYRIGHT for the full notice.

## Contents: Definitions of all help strings.
##
## Principal Author: Roger Needham
##
## Status: In progress.
##
## Revision History:
##
## Revised by GB, 11/29/96  
##
#@rn  These should be replaced with regular manual pages.


set helpStrings(fpGroupDialog) {
We use customary notation in describing a group
by means of a presentation. Thus 
[x,y]=x^-1 y^-1 x y, x^y = y^-1 x y 
and y^n is the n-th power of y. We also 
sometimes use upper case letters to denote the
inverses of the corresponding lower case letters,
e.g., A, B, X, Y are the respective inverses of 
a, b, x, y.

Here are some examples of how to enter a 
presentation:

< x, y; x^2  y^3, (x y)^3 >

<a,b:[a,b]>

< b, t | b^2^t = b b b >


< a, b, c ; [a,b]^C = 
            [a,c] = [b,c] = 1 >

}


set helpStrings(freeGroupDialog) {
Here are examples of how to define a free group:

<x,y>, <a,b,c;>, <x,y,z|> 

Generators can also be listed on different lines
and spaces can be inserted anywhere. It is 
important here to notice that in defining a 
free group, the set of defining relators (or 
relations) is always empty.
}


set helpStrings(abelGroupDialog) {
In working with abelian groups we work in the 
category of abelian groups. Thus presentations 
of abelian groups are all relative to a free 
abelian group, which means adding commutators 
to a presentation is redundant. 

So, for example,

< s, t >

describes the free abelian group on s and t,

< x, y; x^2  y^3 > 

is the factor group of the free abelian group 
on x and y by the subgroup generated by x^2 y^3 
and

< b, t,u | b^2 t = b b b, b^-2 t^4 u^8 >

is the quotient of the free abelian group on b,t 
and u by the subgroup generated by b^2 t B B B 
and B B t^4 u^8. Notice that we use upper case 
letters in place of the inverses of the 
corresponding lower case letters - thus B =b^-1 
and so on.
}


set helpStrings(nilpGroupDialog) {
In describing a nilpotent group we work in the 
appropriate category. So, in order to present a
nilpotent group of class at most c we describe it
as a quotient of a free nilpotent group of class
c by the normal closure of a finite set of relators
(or relations) by first specifying its class (i.e.,
the class of the ambient free nilpotent group), its
generators and defining relators. Thus, for example:

2
< a, b, c; [a,b] c^-1, [a,c], [b,c] >

describes the quotient of the free nilpotent group
of class 2 on a, b and c by the normal subgroup
generated by [a,b] c^-1, [a,c] and [b,c].

Similarly

1
< x, y; x^2, y^3 >

is the quotient of the free abelian group on x and y
factored by the subgroup generated by x^2 and y^3, 
i.e., the group of order 6.
}
set helpStrings(freeNilpGroupDialog) {
In describing a free nilpotent group we work in
the appropriate category. So, we specify its
class and its generators. Thus, for example:

2
< a, b, c >

describes a free nilpotent group
of class 2 on a, b and c.

Similarly

1
< x, y >

is just the free abelian group on x and y.
}

set helpStrings(permutationConventions) {

We represent permutation by it's bottom row, i.e., { 3 1 2 } means 1\
-> 3, 2 -> 1, 3 -> 2. It should be a list of integers separated by\
spaces and enclosed in braces. The list must define a permutation,\
i.e., it must be a list of integers without holes, contain no integer\
twice, contain no integer not in the range from 1 to the length of the\
list. Also, we assume that i -> i for all i greater than the length of\
the list.

}

set helpStrings(wordConventions) {
Conventions on word notation:

Generator names can be any letter, such as a, b,
..., x, y, ...  or any letter followed by a
sequence of digits such as a1, b23 and so on. In
putting generators together to form words, there
must be white space between them unless other
punctuation is used. For example we allow a (b c)
in place of a b c. We sometimes use capital
letters to denote inverses of generators. So the
inverses of a, b, c11, .. are denoted by A, B,
C11,... and also by a^-1, b^-1, c11^-1 and so
on. The ^ is used to denote exponentiation. The
exponents are allowed to be group elements as
well as integers, with a^b defined to be the
conjugate B a b of a by b. Thus a^b = B a b =
b^-1 a b .

Our convention is that ^ is left-associative. So

a^b^2 = (a^b)^2.

We use square brackets to denote commutators:

[a,b]= a^-1 b^-1 a b ( = A B a b ).

We then define, for n > 2, 

[a1,a2,...,an]= [[a1,a2],a3,...,an].

Thus multiple commutators are "left-normed".
}


set helpStrings(equationConventions) {
An equation over a group G is an expression 
of the form 
w=1
where w is an element in the free product of 
G and the free group freely generated by the 
unknowns or variables, usually denoted by x, 
y, z, ... and so on. A solution of such an 
equation w=1 is then a tuple f,g,h, ,.. of 
elements of G such that on setting x=f, y=g, 
z=h, ... in w the resultant element takes on
the value 1 in G.

In order to check in an equation, one needs
first to check in the group G and then the 
element w. As of now we have only implemented 
an algorithm in Magnus for solving equations 
when G is a free group and w is a quadratic word, 
i.e., all of the variables that occur in w, occur 
exactly twice. So, having checked in a free group, 
here are some examples of how to check in an 
equation:

(a b^2)^x Y^2 A

[x,y]=a^2

x^2 y^2 z^2 = 1

The first two examples show that it suffices to 
simply check-in the left-hand-side of an equation.

}

set helpStrings(SystemOfEquationsConventions) {
Enter the tuple of equations i.e. the tuple of 
words in the generators of the free product of
G and the free group freely generated by the 
unknowns or variables, usually denoted by x, 
y, z, ... and so on, separating the words with
commas,enclosing them by {}'s. For example:

{ a x, a x^2 y, z y^2 a^2 x^-3 b, x^3 [b^3 , y] }

}

set helpStrings(subgroupConventions) {
Subgroups are described by listing its 
generators, each such generator given by
a word in the generators of the supergroup. 
We separate these words by commas, enclosing
them by {}'s. For example the subgroup of a
group with generators a and b generated by
a^2 b and [a,b] is described by

{ a^2 b, [a,b] }

We express the fact that H is the the subgroup 
generated by a^2 b and [a,b] by writing

H = gp( a^2 b,[a,b] )
}


set helpStrings(setOfRelatorsConventions) {
Enter the relators as words in the generators 
of the ambient free group. Separate the words 
with commas, and enclose the list in {}'s. 
For example:

{ a^2 b^2, [a^2,b^2] }
}


set helpStrings(setOfWordsConventions) {
Enter the set of words in the generators of 
the supergroup. Separate the words with commas, 
and enclose the list in {}'s. For example:

{ (a b)^3, a^5, b^7, [A^2 b^2, a^2 b^3] }
}


set helpStrings(vectorOfWordsConventions) {
Enter the tuple of words in the generators of
the ambient group, separating the words with
commas and enclosing the list in ()'s. For
example:

(a, a, a^2, b^a, a, b^2)

}


set helpStrings(mapConventions) {
To define a map from the generators 
{ x, y, ... } 
of a group G to a group H with generators 
{ a, b, ... }
assign to each of x,y, ... its image as a word 
in the generators of H, enclsoing the description
by {}'s. For example:

{
  x -> a^b,
  y -> b^-1
}

If you omit the image of a generator, Magnus
assigns its image to be the identity.
}







