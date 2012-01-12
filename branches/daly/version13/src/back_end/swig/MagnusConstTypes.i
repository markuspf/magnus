///by xuxw
///In swig 1.3a5  const TYPE is different from TYPE. so we have to map both of them seperately.
///MagnusConstType.i


// PyList of <C Word>--->SetOf<Word>
%module Magnus

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



// PyList of <C long> --> VectorOf<Integer>
%typemap(python,in) const VectorOf<Integer>& {
	int length=PyObject_Length($source);
	$target=new VectorOf<Integer>(length);
	for(int i=0;i<length;i++) {
		PyObject* item=PyObject_GetItem($source,PyInt_FromLong(i));
		(*$target)[i]=Integer((long) PyInt_AsLong(item));
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

