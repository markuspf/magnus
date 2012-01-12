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

// PyList of <C Double> --> VectorOf<double>
%typemap(python,in) VectorOf<double> {
	int length=PyObject_Length($source);
	$target=new VectorOf<double>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=(double) PyFloat_AsDouble(item);
	}

}

%typemap(python,out) VectorOf<double> {
	$target=PyList_New($source->length());
	for(int i=0;i<$source->length();i++) {
		PyList_SetItem($target,i,
			PyFloat_FromDouble((*$source)[i]));
	}
}

///PyString ---->  File&,
%typemap(python,in) File& {
	$target=new File(PyString_AsString($source));
}
