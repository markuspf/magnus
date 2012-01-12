///by xuxw
///In swig 1.3a5  const TYPE is different from TYPE. so we have to map both of them seperately.
///MagnusConstType.i


// PyList of <C Word>--->SetOf<Word>
%module Magnus

%typemap(python,in) const SetOf<Word>& {
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

// PyList of PyStrings --> VectorOf<Chars>
%typemap(python,in) const VectorOf<Chars> {
	int length=PyObject_Length($source);
	$target=new VectorOf<Chars>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Chars(PyString_AsString(item));
	}
}

// PyList of PyStrings --> VectorOf<Chars>
%typemap(python,in) const VectorOf<Chars>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<Chars>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Chars(PyString_AsString(item));
	}
}


// PyList of PyInt --> VectorOf<Generator>
%typemap(python,in) const VectorOf<Generator>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<Generator>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Generator((int) PyInt_AsLong(item));
	}
}


// PyList of <C Word> --> VectorOf<Word>
// This was is tricky because the elements of the list aren't Python types.
%typemap(python,in) const VectorOf<Word>& {
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

// PyList of <C Word> --> ListOf<Word>
%typemap(python,in) const ListOf<Word>& {
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
}

// PyList of <C long> --> VectorOf<Integer>
%typemap(python,in) const VectorOf<Integer>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<Integer>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Integer((long) PyInt_AsLong(item));
	}

}

///  PyListAbelianWord--->VectorOf<AbelianWord>

%typemap(python,in) const VectorOf<AbelianWord>& {
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

// PyList of <C Elt>--->SetOf<Elt>

%typemap(python,in) const SetOf<Elt>& {
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

// PyList of <C long> --> VectorOf<int>
%typemap(python,in) const VectorOf<int>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<int>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=(long) PyInt_AsLong(item);
	}

}

%typemap(python,in) const VectorOf<PolyWord>& {
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

///VectorOf<RelatorConjugate><<<---->  PyListRelatorConjugate,
%typemap(python,in) const ProductOfRelatorConjugates& {
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