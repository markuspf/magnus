###this is a test file by xuxw(xuxw76@yahoo.com)
import Magnus
a=Magnus.DFSA( )
###a=DFSAFromVector) DFSA(const VectorOf<Chars> & genNames) ;
###a=DFSAFromCharVector)  DFSA(Chars name,const VectorOf<Chars> & genNames) ;
###a=DFSAFromVectorInt)  DFSA( const VectorOf<Chars> & genNames, int numOfStates) ;
###a=DFSAFromCharVectorInt)  DFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates) ;
###a=DFSAFromVectorIntInt)  DFSA( const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings);
###a=DFSAFromCharVectorIntInt)  DFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings) ;
###a.State target(State s,Generator g) const;
a.getNumStates()
#####GroupDFSA begins
a=Magnus.GroupDFSA() 
###a=Magnus.GroupDFSAFromVector) GroupDFSA(const VectorOf<Chars> & genNames) ;
###a=Magnus.GroupDFSAFromWordOrder) GroupDFSA(const VectorOf<Chars> & genNames, const WordOrder & word_order) ;
###a=Magnus.GroupDFSAFromCharVector) GroupDFSA(Chars name,const VectorOf<Chars> & genNames) ;
###a=Magnus.GroupDFSAFromCharVectorWordOrder) GroupDFSA(Chars name,const VectorOf<Chars> & genNames,
###                                             const WordOrder & word_order) ;
###a=Magnus.GroupDFSAFromCharVectorInt) GroupDFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates) ;
###a=Magnus.GroupDFSAFromVectorWordOrderInt) GroupDFSA( const VectorOf<Chars> & genNames, const WordOrder & word_order,
###                                                             int numOfStates) ;
###a=Magnus.GroupDFSAFromVectorIntInt) GroupDFSA( const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings) ;
###a=Magnus.GroupDFSAFromVectorWordOrderIntInt) GroupDFSA( const VectorOf<Chars> & genNames, const WordOrder & word_order,
###                                            int numOfStates, int numOfStrings) ;
###a=Magnus.GroupDFSAFromCharVectorIntInt) GroupDFSA( const Chars & Name, const VectorOf<Chars> & genNames, int numOfStates, int numOfStrings) ;
###State target(State s,Generator g) const;
a.getNumStates()
