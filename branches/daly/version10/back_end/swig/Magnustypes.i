/*
Magnustypes.i

$Id$

This is one of the crucial (and nontrivial) parts of the Magnus scripting
interface. Its purpose is to negotiate between Python data types (such as
int, string, list, ...) and C++/Magnus data types (such as int, Chars,
Generator, Word, VectorOf...).

SWIG does most of the conversions automatically, but it needs help with
nonstandard data types. Moreover, many of the data type used by Magnus
are too general for the intended uses of the scripting interface. Some of
the typemaps (such as the one for istream& (Alex's idea)) introduce a
considerable simplification from the point of view of the user. Let me
repeat this because it's a crucial point: The idea here is not to map
C++ types to their closest counterparts in Python. Rather, the idea is
to map complicated types in C++ to the simplest type in Python that
gets the job done.

Just to get an idea of the complexity of the work done by these
typemaps, consider the following conversation with the scripting
interface:
	>>> import Magnus
	>>> F=Magnus.FreeGroup(5)
	>>> w1=Magnus.WordFromList([1,2,3])
	>>> w2=Magnus.WordFromList([1,2,3,4,-3])
	>>> v=F.nielsenBasis([w1,w2])
	>>> for w in v:
	...     print w
	... 
	[ 1 2 3 ]
	[ 3 -4 ]
	>>> 
The method 'VectorOf<Word> nielsenBasis(const VectorOf<Word>& V) const;'
takes a vector parameter and returns a vector. The typemaps in this file
implicitly did the following:
	- convert the Python list of Python pointers [w1,w2] into a vector
	of objects of type Word
	- convert the vector of objects of type Word that Magnus returned
	into a Python list of pointers that are properly shadowed by Python.
The second part in particular is highly nontrivial and required a lot of
meditation on the typemap FAQ (the FAQ is available at
 http://www-cad.eecs.berkeley.edu/~pinhong/scriptEDA/pyTypemapFAQ.html).

Peter Brinkmann, 05/18/2001
brinkman@math.uiuc.edu

*/


%module Magnus
%include MagnusConstTypes.i

%{
#include "Trichotomy.h"
%}

// The idea of this typemap is to turn functions that require an istream
// parameter into functions that read their data from a string.
%typemap(python,in) istream& {
	strstream *s=new strstream();
	(*s) << PyString_AsString($source) << '\0' << flush;
	$target=(istream *) s;
}


// not yet implemented
%typemap(python,in) ostream& {
	cerr << "typemap for ostream not yet implemented!" << endl;
}

/*
// Trichotomy --> PyInt
%typemap(python,out) Trichotomy {
   if($source->undefined())
      $target=PyInt_FromLong(-1);
   else
      $target=PyInt_FromLong((int)(*$source));
}
*/

// Chars --> PyString
%typemap(python,out) Chars {
	strstream s;
	s << *$source << '\0' << flush;
	$target=PyString_FromString(s.str());
}


// PyString --> Chars
%typemap(python,in) Chars& {
	$target=new Chars(PyString_AsString($source));
}
// PyString --> Chars
%typemap(python,in) const Chars& {
	$target=new Chars(PyString_AsString($source));
}

// PyInt --> Generator
%typemap(python,in) Generator& {
	$target=new Generator(PyInt_AsLong($source));
}


// Generator --> PyInt
%typemap(python,out) Generator {
	$target=PyInt_FromLong(ord(*$source));
}


// VectorOf<Chars> --> PyList of PyStrings
%typemap(python,out) VectorOf<Chars> {
	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		strstream s;
		s << (*$source)[i] << '\0' << flush;
		PyList_SetItem($target,i,PyString_FromString(s.str()));
	}
}


// PyList of PyStrings --> VectorOf<Chars>
%typemap(python,in) VectorOf<Chars>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<Chars>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Chars(PyString_AsString(item));
	}
}

// PyList of PyStrings --> VectorOf<Chars>
%typemap(python,in) VectorOf<Chars> {
	int length=PyObject_Length($source);
	$target=new VectorOf<Chars>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Chars(PyString_AsString(item));
	}
}

// PyList of PyInt --> VectorOf<Generator>
%typemap(python,in) VectorOf<Generator>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<Generator>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Generator((int) PyInt_AsLong(item));
	}
}


// VectorOf<Generator> --> PyList of PyInt
%typemap(python,out) VectorOf<Generator> {
	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		PyList_SetItem($target,i,
			PyInt_FromLong(ord((*$source)[i])));
	}
}

// ListOf<Generator> --> PyList of PyInt
%typemap(python,out) ListOf<Generator> {
	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		PyList_SetItem($target,i,
			PyInt_FromLong(ord((*$source)[i])));
	}
}

// VectorOf<Range> --> PyList of PyRange
%typemap(python,out) VectorOf<Range> {
	Range *res;
	PyObject *optr;
	char cbuf[512];
	PyObject *val = PyInt_FromLong(1L);
	PyObject *mod=PyImport_ImportModule("Magnus");
	PyObject *func=PyObject_GetAttrString(mod,"WordPtr");

	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		res=&((*$source)[i]);
		SWIG_MakePtr(cbuf,(void*) res,SWIGTYPE_p_Range);
		optr=PyObject_CallFunction(func, "(s)",cbuf);
		PyList_SetItem($target,i,optr);
	}

	Py_DECREF(func);
	Py_DECREF(mod);
}


// PyList of <C Word> --> VectorOf<Word>
// This was is tricky because the elements of the list aren't Python types.
%typemap(python,in) VectorOf<Word>& {
	Word *w;
	int length=PyObject_Length($source);
	$target=new VectorOf<Word>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_Word,1)) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _Word_p.");
			return NULL;
		} 
		(*$target)[i]=*w;
	}
}


// VectorOf<Word> --> PyList of <C Word>
// This one was really tricky because of issues involving Python shadow
// classes. For explanations, see the typemap FAQ at
// http://www-cad.eecs.berkeley.edu/~pinhong/scriptEDA/pyTypemapFAQ.html#28
%typemap(python,out) VectorOf<Word> {
	Word *res;
	PyObject *optr;
	char cbuf[512];
	PyObject *val = PyInt_FromLong(1L);
	PyObject *mod=PyImport_ImportModule("Magnus");
	PyObject *func=PyObject_GetAttrString(mod,"WordPtr");

	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		res=&((*$source)[i]);
		SWIG_MakePtr(cbuf,(void*) res,SWIGTYPE_p_Word);
		optr=PyObject_CallFunction(func, "(s)",cbuf);
		PyList_SetItem($target,i,optr);
	}

	Py_DECREF(func);
	Py_DECREF(mod);
}
/*
// PyList of <C Word> --> ListOf<Word>
%typemap(python,in) ListOf<Word>& {
	Word *w;
	int length=PyObject_Length($source);
	$target=new ListOf<Word>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_Word,1)) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _Word_p.");
			return NULL;
		} 
		(*$target)[i]=*w;
	}
} */


// PyList of <C Word>--->SetOf<Word>

%typemap(python,in) SetOf<Word>& {
	Word *w;
	int length=PyObject_Length($source);
	$target=new SetOf<Word>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_Word,1)==-1) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _Word_p.");
			return NULL;
		}
		$target->adjoinElement(*w);
	}
}


// not yet implemented
// SetOf<Word>--->PyList of <C Word>
%typemap(python,out) SetOf<Word> {
	Word *res;
	PyObject *optr;
	char cbuf[512];
        int i=0;
        long length=0;
	PyObject *val = PyInt_FromLong(1L);
	PyObject *mod=PyImport_ImportModule("Magnus");
	PyObject *func=PyObject_GetAttrString(mod,"WordPtr");
   
        SetIterator<Word> I(*$source);  
        ///get the size of the set
        while( !I.done() ) {
           length++;I.next(); 
        }
        $target=PyList_New(length);
        I.reset();
        while( !I.done() ) {
                res=new Word();
                *res=I.value();
		SWIG_MakePtr(cbuf,(void*) res,SWIGTYPE_p_Word);
		optr=PyObject_CallFunction(func, "(s)",cbuf);
		PyList_SetItem($target,i,optr);
                I.next(); i++;
        }
	Py_DECREF(func);
	Py_DECREF(mod);
}



// Bool --> PyInt
%typemap(python,out) Bool {
	if (*$source)
		$target=PyInt_FromLong(1);
	else
		$target=PyInt_FromLong(0);
}

/// the following is written by xiaowei xu(xuxw76@yahoo.com)

// PyInt --> Integer

%typemap(python,in) Integer& {
     $target=new Integer((long)PyInt_AsLong($source));
}

%typemap(python,in) Integer {
     $target=new Integer((long)PyInt_AsLong($source));
}

// Integer --> PyInt
%typemap(python,out) Integer {
     long sourceValue;
     sourceValue=$source->as_long(); 
     $target=PyInt_FromLong(sourceValue);
}

///

// PyList of <C long> --> VectorOf<Integer>
%typemap(python,in) VectorOf<Integer>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<Integer>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Integer((long) PyInt_AsLong(item));
	}

}

// VectorOf<Integer> --> PyList of <long>

%typemap(python,out) VectorOf<Integer> {

	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		PyList_SetItem($target,i,
			PyInt_FromLong(ord((*$source)[i].as_long())));
	}
}

// PyList of <C long> --> VectorOf<int>
%typemap(python,in) VectorOf<int>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<int>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=(long) PyInt_AsLong(item);
	}

}


///PyString ---->  File&,
%typemap(python,in) File& {
	$target=new File(PyString_AsString($source));
}

///  PyListAbelianWord--->VectorOf<AbelianWord>

%typemap(python,in) VectorOf<AbelianWord>& {
	AbelianWord *w;
	int length=PyObject_Length($source);
	$target=new VectorOf<AbelianWord>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_AbelianWord,1)) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _Word_p.");
			return NULL;
		}
		(*$target)[i]=*w;
	}
}


///VectorOf<AbelianWord>---->  PyListAbelianWord,

%typemap(python,out) VectorOf<AbelianWord> {
	AbelianWord *res;
	PyObject *optr;
	char cbuf[512];
	PyObject *val = PyInt_FromLong(1L);
	PyObject *mod=PyImport_ImportModule("Magnus");
	PyObject *func=PyObject_GetAttrString(mod,"AbelianWordPtr");

	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		res=&((*$source)[i]);
		SWIG_MakePtr(cbuf,(void*) res,SWIGTYPE_p_AbelianWord);
		optr=PyObject_CallFunction(func, "(s)",cbuf);
		PyList_SetItem($target,i,optr);
	}

	Py_DECREF(func);
	Py_DECREF(mod);
}

// PyList of <C Elt>--->SetOf<Elt>

%typemap(python,in) SetOf<Elt>& {
	Elt *w;
	int length=PyObject_Length($source);
	$target=new SetOf<Elt>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_Elt,1)) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _Elt_p.");
			return NULL;
		}
		$target->adjoinElement(*w);
	}
}

//VectorPtrOf<Endomorphism>
//VectorPtrOf<Automorphism>

%typemap(python,in) VectorOf<BasisWord>& {
	BasisWord *w;
	int length=PyObject_Length($source);
	$target=new VectorOf<BasisWord>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_BasisWord,1)) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _BasisWord_p.");
			return NULL;
		}
		(*$target)[i]=*w;
	}
}


///VectorOf<AbelianWord>---->  PyListAbelianWord,

%typemap(python,out) VectorOf<BasisWord> {
	BasisWord *res;
	PyObject *optr;
	char cbuf[512];
	PyObject *val = PyInt_FromLong(1L);
	PyObject *mod=PyImport_ImportModule("Magnus");
	PyObject *func=PyObject_GetAttrString(mod,"BasisWordPtr");

	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		res=&((*$source)[i]);
		SWIG_MakePtr(cbuf,(void*) res,SWIGTYPE_p_BasisWord);
		optr=PyObject_CallFunction(func, "(s)",cbuf);
		PyList_SetItem($target,i,optr);
	}

	Py_DECREF(func);
	Py_DECREF(mod);
}

%typemap(python,in) VectorOf<PolyWord>& {
	PolyWord *w;
	int length=PyObject_Length($source);
	$target=new VectorOf<PolyWord>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_PolyWord,1)) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _PolyWord_p.");
			return NULL;
		}
		(*$target)[i]=*w;
	}
}


///VectorOf<AbelianWord>---->  PyListAbelianWord,

%typemap(python,out) VectorOf<PolyWord> {
	PolyWord *res;
	PyObject *optr;
	char cbuf[512];
	PyObject *val = PyInt_FromLong(1L);
	PyObject *mod=PyImport_ImportModule("Magnus");
	PyObject *func=PyObject_GetAttrString(mod,"PolyWordPtr");

	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		res=&((*$source)[i]);
		SWIG_MakePtr(cbuf,(void*) res,SWIGTYPE_p_PolyWord);
		optr=PyObject_CallFunction(func, "(s)",cbuf);
		PyList_SetItem($target,i,optr);
	}

	Py_DECREF(func);
	Py_DECREF(mod);
}

////VectorOf<RelatorConjugate> ProductOfRelatorConjugates;
///VectorOf<RelatorConjugate><<<---->  PyListRelatorConjugate,
%typemap(python,in) ProductOfRelatorConjugates& {
	RelatorConjugate *w;
	int length=PyObject_Length($source);
	$target=new VectorOf<RelatorConjugate>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		// the following lines show the way to get pointers to
		//  C++ objects from Python
		if (SWIG_ConvertPtr(item,(void **) &w,SWIGTYPE_p_RelatorConjugate,1)) {
			PyErr_SetString(PyExc_TypeError,
				"Type error in list. Expected _RelatorConjugate_p.");
			return NULL;
		}
		(*$target)[i]=*w;
	}
}


///VectorOf<RelatorConjugate>---->  PyListRelatorConjugate,

%typemap(python,out) ProductOfRelatorConjugates {
	RelatorConjugate *res;
	PyObject *optr;
	char cbuf[512];
	PyObject *val = PyInt_FromLong(1L);
	PyObject *mod=PyImport_ImportModule("Magnus");
	PyObject *func=PyObject_GetAttrString(mod,"RelatorConjugatePtr");

	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		res=&((*$source)[i]);
		SWIG_MakePtr(cbuf,(void*) res,SWIGTYPE_p_RelatorConjugate);
		optr=PyObject_CallFunction(func, "(s)",cbuf);
		PyList_SetItem($target,i,optr);
	}

	Py_DECREF(func);
	Py_DECREF(mod);
}
/*
%typemap(python,default)  Integer orderofElt {
      $target=new Integer(-1);
}
*/